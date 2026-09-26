# Schedule-Dependent Message-Matching Examples

## English

These five MPI-RMA examples isolate the coverage difference between concrete runtime checking and systematic exploration of legal MPI executions. Ranks 1 and 2 send values `0` and `1` to two `MPI_ANY_SOURCE` receives at rank 0. The first legal match determines whether a reachable RMA race is activated; the program and external input are unchanged across repetitions.

### Observed results and mean wall time

| Case | Conflict | RMASanitizer reports | RMASanitizer (s) | RMASV | RMASV (s) |
|---|---|---:|---:|---:|---:|
| `01-put-origin` | `LOCAL_STORE---PUT` | 2/10 | 4.44 | Found | 7.26 |
| `02-get-result` | `LOCAL_STORE---GET` | 5/10 | 3.21 | Found | 7.00 |
| `03-put-put` | `PUT---PUT` | 5/10 | 3.13 | Found | 7.04 |
| `04-put-get` | `GET---PUT` | 3/10 | 3.31 | Found | 7.59 |
| `05-put-target-store` | `RMA_STORE---PUT` | 2/10 | 3.01 | Found | 6.90 |

RMASanitizer reported the race exactly when the first matched value was `1`, for 17 of 50 observed concrete executions. The counts are schedule observations and may vary across systems. RMASV explored both legal matchings and reported all five reachable races.

RMASV values are analysis-only means over five runs and exclude one-time bitcode generation. RMASanitizer values are means over ten post-compilation runs; they include each `mustrun` invocation, and the first entry includes its cold setup cost. `results/summary.csv` contains the combined table, while `results/rmasanitizer_trials.csv` and `results/rmasv_trials.csv` preserve every published measurement.

