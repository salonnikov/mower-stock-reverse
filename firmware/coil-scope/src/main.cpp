// coil-scope / пассажир-разведчик — ESP32 слушает мир, смотрим с телефона.
//  • Катушка (витая пара) → GPIO36 (ADC1): сигнал провода — форма, спектр, частота.
//  • IMU LSM6DS3 по I2C (SDA=21, SCL=22): наклон + угловая скорость (гироскоп).
//
// КАК СМОТРЕТЬ: подключись телефоном к Wi-Fi "coil-scope" (пароль mower1234),
//              открой http://192.168.4.1/
//
// IMU LSM6DS3: VIN→3V3, GND→GND, SCL→22, SDA→21, CS→3V3 (режим I2C). Остальное не нужно.
// ВЕРСИЯ 3 (AP + IMU).

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <Wire.h>
#include <SPI.h>
#include <arduinoFFT.h>
#include <driver/i2s.h>
#include <driver/adc.h>

// Домашний Wi-Fi (опц.) — из secrets.h (в git нет). Без него — только своя точка.
#if __has_include("secrets.h")
  #include "secrets.h"
#endif
#ifndef STA_SSID
  #define STA_SSID ""
  #define STA_PASS ""
#endif

// --- точка доступа (всегда поднимается, как запасной канал) ---
const char* AP_SSID = "coil-scope";
const char* AP_PASS = "mower1234";     // мин. 8 символов; "" = открытая сеть
String g_staIp = "—";                  // IP в домашней сети

// --- АЦП/выборка катушки ---
static const adc1_channel_t ADC_CH = ADC1_CHANNEL_0;   // GPIO36 (VP)
static const i2s_port_t I2S_PORT  = I2S_NUM_0;
static const uint32_t SAMPLE_RATE = 80000;
static const uint16_t SAMPLES     = 1024;

// --- I2C / IMU ---
static const int PIN_SDA = 21, PIN_SCL = 22;
uint8_t  imuAddr = 0;           // 0 = не найден; иначе 0x6A/0x6B
uint8_t  imuWho  = 0;           // WHO_AM_I
char     g_i2c[80] = "";        // что нашли на шине I2C
char     g_order[28] = "21/22"; // рабочий порядок пинов SDA/SCL
int      g_spiWho = -1;          // WHO_AM_I по SPI (-1 = не пробовали)

// --- SPI-пины для проверки чипа (VSPI): SCK=18, MISO=19, MOSI=23, CS=5 ---
static const int SPI_SCK=18, SPI_MISO=19, SPI_MOSI=23, SPI_CS=5;
void spiCheck() {
  pinMode(SPI_CS, OUTPUT); digitalWrite(SPI_CS, HIGH);
  SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI, SPI_CS);
  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE3)); // LSM6DS3 = mode 3
  digitalWrite(SPI_CS, LOW);
  SPI.transfer(0x8F);                       // 0x0F | 0x80 = чтение WHO_AM_I
  g_spiWho = SPI.transfer(0x00);
  digitalWrite(SPI_CS, HIGH);
  SPI.endTransaction();
}
float    g_ax=0, g_ay=0, g_az=0;       // g
float    g_gx=0, g_gy=0, g_gz=0;       // dps
float    g_tilt=0;                     // угол от вертикали, °

double vReal[SAMPLES], vImag[SAMPLES];
ArduinoFFT<double> FFT = ArduinoFFT<double>(vReal, vImag, SAMPLES, (double)SAMPLE_RATE);
volatile double g_peakFreq = 0, g_peakMag = 0;
volatile double g_holdFreq = 0, g_holdMag = 0;   // удержание максимума
uint32_t g_holdT = 0;
uint16_t g_wave[256], g_spec[256];
// окно поиска пика: игнор сети (<300 Гц) и артефакта у Найквиста (>38 кГц)
static const double F_MIN = 300.0, F_MAX = 38000.0;

WebServer server(80);

// =================== IMU (LSM6DS3) ===================
void imuWrite(uint8_t reg, uint8_t val) {
  Wire.beginTransmission(imuAddr); Wire.write(reg); Wire.write(val); Wire.endTransmission();
}
uint8_t imuRead8(uint8_t addr, uint8_t reg) {
  Wire.beginTransmission(addr); Wire.write(reg); Wire.endTransmission(false);
  Wire.requestFrom((int)addr, 1);
  return Wire.available() ? Wire.read() : 0;
}
int16_t imuRead16(uint8_t reg) {
  Wire.beginTransmission(imuAddr); Wire.write(reg); Wire.endTransmission(false);
  Wire.requestFrom((int)imuAddr, 2);
  uint8_t lo = Wire.read(), hi = Wire.read();
  return (int16_t)((hi << 8) | lo);
}
int i2cScan() {
  int n = 0; g_i2c[0] = 0;
  for (uint8_t a = 1; a < 127; a++) {
    Wire.beginTransmission(a);
    if (Wire.endTransmission() == 0) {
      char t[8]; snprintf(t, sizeof(t), "0x%02X ", a);
      strncat(g_i2c, t, sizeof(g_i2c) - strlen(g_i2c) - 1);
      n++;
    }
  }
  if (n == 0) strcpy(g_i2c, "пусто (ничего не отвечает)");
  return n;
}
void imuSetup() {
  // пробуем оба порядка: 21=SDA/22=SCL и наоборот (вдруг провода перепутаны)
  Wire.end(); Wire.begin(21, 22); Wire.setClock(100000);
  int n = i2cScan();
  if (n == 0) {
    Wire.end(); Wire.begin(22, 21); Wire.setClock(100000);
    int n2 = i2cScan();
    if (n2 > 0) strcpy(g_order, "22/21 (провода перепутаны!)");
  }
  for (uint8_t a = 0x6A; a <= 0x6B; a++) {
    uint8_t who = imuRead8(a, 0x0F);          // WHO_AM_I
    if (who == 0x69 || who == 0x6A) { imuAddr = a; imuWho = who; break; }
  }
  if (!imuAddr) return;
  imuWrite(0x10, 0x60);   // CTRL1_XL: акс 104 Гц, ±2g
  imuWrite(0x11, 0x60);   // CTRL2_G:  гиро 104 Гц, 245 dps
}
void imuPoll() {
  if (!imuAddr) return;
  g_gx = imuRead16(0x22) * 8.75f / 1000.0f;   // dps
  g_gy = imuRead16(0x24) * 8.75f / 1000.0f;
  g_gz = imuRead16(0x26) * 8.75f / 1000.0f;
  g_ax = imuRead16(0x28) * 0.061f / 1000.0f;  // g
  g_ay = imuRead16(0x2A) * 0.061f / 1000.0f;
  g_az = imuRead16(0x2C) * 0.061f / 1000.0f;
  g_tilt = atan2f(sqrtf(g_ax*g_ax + g_ay*g_ay), g_az) * 57.2958f;
}

// =================== АЦП катушки ===================
void adcSetup() {
  i2s_config_t cfg = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX | I2S_MODE_ADC_BUILT_IN),
    .sample_rate = SAMPLE_RATE,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
    .communication_format = I2S_COMM_FORMAT_STAND_I2S,
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 4, .dma_buf_len = 256, .use_apll = false,
  };
  adc1_config_width(ADC_WIDTH_BIT_12);
  adc1_config_channel_atten(ADC_CH, ADC_ATTEN_DB_11);
  i2s_driver_install(I2S_PORT, &cfg, 0, NULL);
  i2s_set_adc_mode(ADC_UNIT_1, ADC_CH);
  i2s_adc_enable(I2S_PORT);
}
void sampleBlock() {
  static uint16_t buf[SAMPLES];
  size_t bytesRead = 0, need = SAMPLES * sizeof(uint16_t), got = 0;
  while (got < need) {
    i2s_read(I2S_PORT, ((uint8_t*)buf) + got, need - got, &bytesRead, portMAX_DELAY);
    got += bytesRead;
  }
  for (uint16_t i = 0; i < SAMPLES; i++) { vReal[i] = (double)(buf[i] & 0x0FFF); vImag[i] = 0.0; }
}
void analyzeBlock() {
  for (int i = 0; i < 256; i++) g_wave[i] = (uint16_t)vReal[i * (SAMPLES / 256)];
  double mean = 0; for (uint16_t i = 0; i < SAMPLES; i++) mean += vReal[i];
  mean /= SAMPLES;
  for (uint16_t i = 0; i < SAMPLES; i++) { vReal[i] -= mean; vImag[i] = 0; }
  FFT.windowing(FFTWindow::Hamming, FFTDirection::Forward);
  FFT.compute(FFTDirection::Forward);
  FFT.complexToMagnitude();

  // пик ищем вручную в окне [F_MIN..F_MAX] — мимо сети 50 Гц и артефакта у 40 кГц
  const double binHz = (double)SAMPLE_RATE / SAMPLES;     // 78.125 Гц/бин
  int bMin = (int)(F_MIN / binHz), bMax = (int)(F_MAX / binHz);
  double pkMag = 0; int pkBin = bMin;
  for (int b = bMin; b < bMax; b++) if (vReal[b] > pkMag) { pkMag = vReal[b]; pkBin = b; }
  g_peakFreq = pkBin * binHz;
  g_peakMag  = pkMag;

  // удержание максимума ~3 с (чтобы реальный пик не терялся в дрожании)
  if (pkMag > g_holdMag || millis() - g_holdT > 3000) {
    g_holdMag = pkMag; g_holdFreq = g_peakFreq; g_holdT = millis();
  }

  for (int i = 0; i < 256; i++)
    g_spec[i] = (uint16_t)constrain(vReal[1 + i * 2], 0, 65535);
}

// =================== Веб ===================
const char* PAGE = R"HTML(<!doctype html><meta charset=utf-8><meta name=viewport content="width=device-width,initial-scale=1"><title>coil-scope</title>
<style>body{font-family:sans-serif;background:#111;color:#eee;margin:8px}
canvas{background:#000;display:block;margin:6px 0;width:100%;height:150px}
b{color:#6f6;font-size:22px}.r{color:#9cf}</style>
<h3>пассажир-разведчик</h3>
<div class=r>дом. сеть: <span id=ip>—</span> &nbsp; (или http://coil-scope.local/)</div>
<div>Провод — частота: <b id=f>—</b> Гц &nbsp; амплитуда: <span id=m>—</span></div>
<div class=r>макс за 3с: <span id=hf>—</span> Гц (ампл <span id=hm>—</span>)</div>
<div>Форма:</div><canvas id=w width=512 height=150></canvas>
<div>Спектр (0…40 кГц):</div><canvas id=s width=512 height=150></canvas>
<h4>IMU <span id=imu class=r></span></h4>
<div class=r>I2C на шине: <span id=bus>—</span> &nbsp; порядок: <span id=ord>—</span></div>
<div class=r>SPI WHO_AM_I: <b id=spi>—</b> (ждём 0x69 = чип жив)</div>
<div>Наклон: <b id=t>—</b>° &nbsp; угл.скорость Z: <span id=gz>—</span> °/с</div>
<div class=r>accel g: <span id=a>—</span> &nbsp; gyro °/с: <span id=g>—</span></div>
<script>
function draw(c,arr,max){let x=c.getContext('2d'),W=c.width,H=c.height;x.clearRect(0,0,W,H);
x.strokeStyle='#6f6';x.beginPath();for(let i=0;i<arr.length;i++){let y=H-arr[i]/max*H;
i?x.lineTo(i/arr.length*W,y):x.moveTo(0,y);}x.stroke();}
async function tick(){try{let d=await(await fetch('/data')).json();
ip.textContent=d.ip;f.textContent=d.freq.toFixed(0);m.textContent=d.mag.toFixed(0);
hf.textContent=d.hfreq.toFixed(0);hm.textContent=d.hmag.toFixed(0);
draw(w,d.wave,4096);draw(s,d.spec,Math.max(...d.spec,1));
imu.textContent=d.imu?('('+d.imu+')'):'НЕ НАЙДЕН';bus.textContent=d.bus;ord.textContent=d.ord;
spi.textContent='0x'+(d.spi<0?'--':('0'+(d.spi&255).toString(16)).slice(-2));
t.textContent=d.tilt.toFixed(0);gz.textContent=d.gz.toFixed(1);
a.textContent=d.ax.toFixed(2)+' / '+d.ay.toFixed(2)+' / '+d.az.toFixed(2);
g.textContent=d.gx.toFixed(1)+' / '+d.gy.toFixed(1)+' / '+d.gz.toFixed(1);}catch(e){}}
setInterval(tick,300);tick();
</script>)HTML";

String jsonData() {
  String s = "{\"ip\":\"" + g_staIp + "\",\"freq\":" + String(g_peakFreq,1) + ",\"mag\":" + String(g_peakMag,0);
  s += ",\"hfreq\":" + String(g_holdFreq,1) + ",\"hmag\":" + String(g_holdMag,0);
  s += ",\"imu\":\"" + (imuAddr ? String("0x")+String(imuWho,HEX) : String("")) + "\"";
  s += ",\"bus\":\"" + String(g_i2c) + "\",\"ord\":\"" + String(g_order) + "\",\"spi\":" + String(g_spiWho);
  s += ",\"tilt\":" + String(g_tilt,1) + ",\"ax\":" + String(g_ax,2) + ",\"ay\":" + String(g_ay,2) + ",\"az\":" + String(g_az,2);
  s += ",\"gx\":" + String(g_gx,1) + ",\"gy\":" + String(g_gy,1) + ",\"gz\":" + String(g_gz,1);
  s += ",\"wave\":[";
  for (int i = 0; i < 256; i++) { s += g_wave[i]; if (i < 255) s += ','; }
  s += "],\"spec\":[";
  for (int i = 0; i < 256; i++) { s += g_spec[i]; if (i < 255) s += ','; }
  s += "]}";
  return s;
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_AP_STA);                       // и своя точка, и домашняя сеть
  WiFi.softAP(AP_SSID, strlen(AP_PASS) >= 8 ? AP_PASS : nullptr);
  Serial.print("AP \""); Serial.print(AP_SSID);
  Serial.print("\" -> http://"); Serial.println(WiFi.softAPIP());

  if (strlen(STA_SSID)) {                        // подключение к домашнему Wi-Fi
    WiFi.begin(STA_SSID, STA_PASS);
    for (int i = 0; i < 40 && WiFi.status() != WL_CONNECTED; i++) delay(250);
    if (WiFi.status() == WL_CONNECTED) {
      g_staIp = WiFi.localIP().toString();
      Serial.print("Дом. сеть -> http://"); Serial.println(g_staIp);
    } else Serial.println("Дом. сеть: не подключился (проверь имя/пароль)");
  }
  if (MDNS.begin("coil-scope")) Serial.println("mDNS: http://coil-scope.local/");

  server.on("/", []() { server.send(200, "text/html", PAGE); });
  server.on("/data", []() { server.send(200, "application/json", jsonData()); });
  server.begin();

  imuSetup();
  if (imuAddr) Serial.printf("IMU LSM6DS3 на 0x%02X (WHO=0x%02X)\n", imuAddr, imuWho);
  else         Serial.println("IMU не найден по I2C");
  spiCheck();                                  // проверка чипа по SPI
  Serial.printf("SPI WHO_AM_I = 0x%02X (ждём 0x69)\n", g_spiWho);
  adcSetup();
}

uint32_t lastImu = 0;
void loop() {
  sampleBlock();
  analyzeBlock();
  if (millis() - lastImu > 100) { lastImu = millis(); imuPoll(); }
  server.handleClient();
}

// ----- ЗАМЕТКИ -----
// IMU "НЕ НАЙДЕН": проверь CS→3V3 (без этого чип в SPI), SDA=21/SCL=22, VIN→3V3.
// Сигнал провода слабый: ближе к проводу/базе, поверни катушку, добавь витков.
// Частота вне диапазона: скажи — пересоберу с другим SAMPLE_RATE.
