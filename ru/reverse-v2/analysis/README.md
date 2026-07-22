# reverse-v2/analysis/ — механически извлечённый граф вызовов

Сгенерировано `awk` по `reverse-v2/{chip1,chip2}/decompiled_all.c` (2026-07-06). **Чисто механически — без ИИ, без интерпретации.** Ребро = вызов `FUN_xxxx` внутри тела функции.

| Файл | Что | Формат |
|---|---|---|
| `callgraph-chip1.tsv` | Все рёбра вызовов chip1 (4723) | `caller<TAB>callee<TAB>count` |
| `callgraph-chip2.tsv` | Все рёбра вызовов chip2 (729) | `caller<TAB>callee<TAB>count` |
| `indegree-chipN.tsv` | Кого чаще всех вызывают (in-degree), по убыванию | `func<TAB>callers` |
| `outdegree-chipN.tsv` | Кто больше всех вызывает (out-degree) | `func<TAB>callees` |

Топ in-degree chip1: `FUN_08048adc` (331 — вероятно lock/malloc/assert-хелпер), `FUN_08071620` (60), `FUN_0801869c` (58).

Семантика функций (что каждая делает) — в `reverse-v2/reference/FUNCTIONS-chip1.md` / `FUNCTIONS-chip2.md`.
Как перегенерировать: `awk '/^\/\* ==== FUN_/{cur=$3;next}{...}' decompiled_all.c` (см. историю сессии).
