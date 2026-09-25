# External RMA Checker Comparison

This directory contains two evaluations comparing RMASV with the runtime RMA checkers MUST-RMA and RMASanitizer.

### RMARaceBench comparison

The head-to-head comparison uses 69 RMARaceBench cases supported by all three tools. The reported time is end-to-end compilation plus analysis time from one run on the same machine. RMASV was executed natively; MUST-RMA and RMASanitizer were executed in their artifact container.

| Tool | TP | TN | FP | FN | Accuracy | Total (s) | Mean/case (s) |
|---|---:|---:|---:|---:|---:|---:|---:|
| RMASV | 40 | 29 | 0 | 0 | 100% | 417.43 | 6.05 |
| MUST-RMA | 36 | 28 | 1 | 4 | 92.8% | 269.70 | 3.91 |
| RMASanitizer | 40 | 29 | 0 | 0 | 100% | 277.67 | 4.02 |

The selected sources and BSD-3-Clause license are under `rmaracebench/cases/` and `rmaracebench/LICENSE`. Per-case verdicts are in `rmaracebench/results/per_case.csv`; aggregate accuracy and runtime are in `rmaracebench/results/summary.csv`. `rmaracebench/SHA256SUMS` records the source hashes.

### Nondeterministic message matching

The five additional examples illustrate why systematic interleaving exploration can expose reachable races missed by a concrete execution. With the program and external input unchanged, two senders transmit different values to an `MPI_ANY_SOURCE` receive, and the first legal match determines whether a racing RMA operation is executed.

| Case                  | Conflict            | RMASanitizer | RMASV |
| --------------------- | ------------------- | -----------: | ----: |
| `01-put-origin`       | `LOCAL_STORE---PUT` |         3/10 | Found |
| `02-get-result`       | `LOCAL_STORE---GET` |         3/10 | Found |
| `03-put-put`          | `PUT---PUT`         |         2/10 | Found |
| `04-put-get`          | `GET---PUT`         |         1/10 | Found |
| `05-put-target-store` | `RMA_STORE---PUT`   |         3/10 | Found |

Across ten observed RMASanitizer executions per example, the reachable race was reported in 12 of 50 executions. RMASV explored both legal first-message matchings in one analysis per example and reported all five reachable races. These observed counts are schedule-dependent and are not estimates of fixed detection probabilities. Examples sources and detailed results are under `nondeterministic_matching/`.
