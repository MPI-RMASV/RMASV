# External RMA Checker Comparison

## English

This directory contains three complementary evaluations comparing RMASV with the runtime RMA checkers MUST-RMA and RMASanitizer.

### RMARaceBench comparison

The head-to-head comparison uses 69 RMARaceBench cases supported by all three tools. The reported time is end-to-end compilation plus analysis time from one run on the same machine. RMASV was executed natively; MUST-RMA and RMASanitizer were executed in their artifact container.

| Tool | TP | TN | FP | FN | Accuracy | Total (s) | Mean/case (s) |
|---|---:|---:|---:|---:|---:|---:|---:|
| RMASV | 40 | 29 | 0 | 0 | 100% | 417.43 | 6.05 |
| MUST-RMA | 36 | 28 | 1 | 4 | 92.8% | 269.70 | 3.91 |
| RMASanitizer | 40 | 29 | 0 | 0 | 100% | 277.67 | 4.02 |

The selected sources and BSD-3-Clause license are under `rmaracebench/cases/` and `rmaracebench/LICENSE`. Per-case verdicts are in `rmaracebench/results/per_case.csv`; aggregate accuracy and runtime are in `rmaracebench/results/summary.csv`. `rmaracebench/SHA256SUMS` records the source hashes.

### Schedule-dependent coverage

Five additional examples keep the program and external input unchanged while legal `MPI_ANY_SOURCE` matching determines whether a racing branch executes. In the timed batch, RMASanitizer reported the reachable race in 17 of 50 concrete executions. RMASV explored both legal first-message matchings and reported all five races. Counts and timing are under `nondeterministic_matching/`; the concrete counts are schedule observations, not fixed detection probabilities.

### Input-dependent coverage

Five further examples isolate input dependence. For each example, concrete input `0` is safe and input `1` activates a reachable race. RMASanitizer reports according to the supplied concrete input, whereas RMASV treats the binary choice symbolically and covers both values in one analysis. Sources and results are under `input_dependent/`.

For the two supplemental groups, RMASV analysis-only wall time is the mean of five runs after one-time bitcode generation. RMASanitizer wall time is measured after compilation: ten concrete runs per schedule-dependent case and five runs for each concrete input per input-dependent case. These small examples illustrate coverage behavior; their timings are not a replacement for the paper's benchmark-scale performance evaluation.
