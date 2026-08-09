# Ghidra 12.1.2 headless for ESP32 (Xtensa LX6).
#
# linux/amd64 on purpose: the Ghidra release ships native decompiler/sleigh
# binaries only for linux_x86_64, mac_*, win_x86_64 — there is no linux_arm_64
# build, so an arm64 image would come up without a decompiler. On Apple Silicon
# this runs through Docker Desktop's Rosetta (must be enabled), not qemu.
FROM --platform=linux/amd64 eclipse-temurin:21-jdk

RUN apt-get update \
 && apt-get install -y --no-install-recommends unzip fontconfig \
 && rm -rf /var/lib/apt/lists/*

COPY ghidra.zip /tmp/ghidra.zip
RUN unzip -q /tmp/ghidra.zip -d /opt \
 && rm /tmp/ghidra.zip \
 && mv /opt/ghidra_12.1.2_PUBLIC /opt/ghidra \
 && chmod +x /opt/ghidra/support/analyzeHeadless \
 && printf 'VMARGS=-Xmx8G\n' >> /opt/ghidra/support/launch.properties

ENV GHIDRA_HOME=/opt/ghidra
WORKDIR /work
ENTRYPOINT ["/opt/ghidra/support/analyzeHeadless"]
