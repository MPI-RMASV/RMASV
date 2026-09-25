# Nondeterministic Message-Matching Examples

This directory contains five small MPI-RMA examples illustrating the coverage difference between concrete runtime checking and systematic exploration of legal MPI executions.

In every example, ranks 1 and 2 send values 0 and 1 to two `MPI_ANY_SOURCE` receives at rank 0. The value matched by the first receive determines whether a reachable RMA data race is activated. The program and external input remain unchanged across repeated executions; only the legal message matching and runtime schedule vary.

The examples cover five conflict patterns: a `Put` origin-buffer overwrite, a `Get` result-buffer overwrite, concurrent `Put-Put`, concurrent `Put-Get`, and a `Put` concurrent with a local target store.

### Observed Results

| Case | Conflict | RMASanitizer | RMASV |
|---|---|---:|---:|
| `01-put-origin` | `LOCAL_STORE---PUT` | 3/10 | Found |
| `02-get-result` | `LOCAL_STORE---GET` | 3/10 | Found |
| `03-put-put` | `PUT---PUT` | 2/10 | Found |
| `04-put-get` | `GET---PUT` | 1/10 | Found |
| `05-put-target-store` | `RMA_STORE---PUT` | 3/10 | Found |

RMASanitizer reported the reachable race in 12 of the 50 observed concrete executions. RMASV analyzed each example once, explored both legal first-message matchings, and reported the reachable race in all five examples.

The RMASanitizer counts are observations from ten repetitions per example, not estimates of fixed detection probabilities. They may vary with the MPI implementation, machine, and runtime schedule. The per-execution evidence is available in `results/rmasanitizer_trials.csv`; `first_value=1` activates the racing branch.
