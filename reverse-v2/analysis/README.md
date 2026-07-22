# reverse-v2/analysis/ — mechanically extracted call graph

Generated with `awk` over `reverse-v2/{chip1,chip2}/decompiled_all.c` (2026-07-06). **Purely mechanical — no AI, no interpretation.** An edge = a call to `FUN_xxxx` inside a function body.

| File | What | Format |
|---|---|---|
| `callgraph-chip1.tsv` | All call edges of chip1 (4723) | `caller<TAB>callee<TAB>count` |
| `callgraph-chip2.tsv` | All call edges of chip2 (729) | `caller<TAB>callee<TAB>count` |
| `indegree-chipN.tsv` | Who is called most often (in-degree), descending | `func<TAB>callers` |
| `outdegree-chipN.tsv` | Who calls the most (out-degree) | `func<TAB>callees` |

Top in-degree chip1: `FUN_08048adc` (331 — probably a lock/malloc/assert helper), `FUN_08071620` (60), `FUN_0801869c` (58).

The semantics of the functions (what each one does) — in `reverse-v2/reference/FUNCTIONS-chip1.md` / `FUNCTIONS-chip2.md`.
How to regenerate: `awk '/^\/\* ==== FUN_/{cur=$3;next}{...}' decompiled_all.c` (see the session history).
