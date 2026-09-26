# Input-Dependent Examples

## English

These five MPI-RMA examples isolate input-dependent coverage. Each program accepts a binary `choice`: input `0` skips the racing operation and is safe, while input `1` activates a reachable race. A runtime checker observes only the supplied concrete input. For RMASV, `choice` is symbolic and constrained to `{0,1}`, so one analysis covers both branches.

### Observed results and mean wall time

| Case | Conflict | RMASanitizer input 0 | Input 0 (s) | RMASanitizer input 1 | Input 1 (s) | RMASV | RMASV (s) |
|---|---|---:|---:|---:|---:|---:|---:|
| `01-put-origin` | `LOCAL_STORE---PUT` | 0/5 | 3.15 | 5/5 | 3.02 | Found | 6.39 |
| `02-get-result` | `LOCAL_STORE---GET` | 0/5 | 2.79 | 5/5 | 3.08 | Found | 6.59 |
| `03-put-put` | `PUT---PUT` | 0/5 | 2.75 | 5/5 | 3.61 | Found | 6.40 |
| `04-put-get` | `PUT---GET` | 0/5 | 2.50 | 5/5 | 3.35 | Found | 6.59 |
| `05-put-target-store` | `PUT---RMA_STORE` | 0/5 | 2.81 | 5/5 | 3.29 | Found | 6.72 |

RMASanitizer correctly reports no race for every input-`0` execution and reports the race for every input-`1` execution. This is not a false negative for a concrete run; it demonstrates that a safe test input cannot expose a violation reachable under another input. RMASV covers both values together and reports all five reachable races.

RMASV values are analysis-only means over five runs and exclude one-time bitcode generation. RMASanitizer values are means over five post-compilation runs for each concrete input. `results/summary.csv` contains the combined table, while `results/rmasanitizer_trials.csv` and `results/rmasv_trials.csv` preserve every published measurement. The `RMASV_SYMBOLIC` build branch in each source replaces the command-line choice with a symbolic binary value.

