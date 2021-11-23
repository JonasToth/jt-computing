# Pull Requests

**IMPORTANT: Please do not create a Pull Request without creating an issue first.**

*Any change needs to be discussed before proceeding. Failure to do so may result
in the rejection of the pull request.*

Please provide enough information so that others can review your pull request. You
can skip this if you're fixing a typo or adding an app to the Showcase.

Explain the **details** for making this change. What existing problem does the pull
request solve?

Ex:

* If you "Added a/changed the function to do X", explain why:
  * it is necessary to have a way to do X.
  * if there already exists a way, why is your implementation better
* If you "Fixed bug/error in X", explain:
  * what was the bug/error (if you already made an issue, please link to it here)
  * how does your implementation fix the issue

## Code style and formatting

The code must be clean with `clang-format`, `cmake-format` and all other
linters. This should be enforced by CI. Unclean code is ignored until clean.

The code must be clean and follow proper software engineering practices.
Only modern `C++-20` is accepted, especially use concepts. But in the end its
not about how many features of `C++` were used, but rather is the code
understandable and correct.

Your code requires

* unit-tests
* contracts (use `gsl` assertions)
* benchmarks

### Closing Issues

Put `closes #XXXX` in your comment to auto-close the issue that your PR fixes
(if such).

---

Fixes #XXXX

## Proposed changes

*
*
*

## Motivation behind changes

### Test plan

Make sure tests pass on all of the [relevant CI workflows](https://github.com/JonasToth/jt-computing/actions).

### Pull Request Readiness Checklist

See details at [CONTRIBUTING.md](https://github.com/filipdutescu/modern-cpp-template/blob/master/CONTRIBUTING.md).

* [ ] I agree to contribute to the project under `jt-computing` (GPLv3) [License](LICENSE).
* [ ] To the best of my knowledge, the proposed patch is not based on a code under
  a license that is incompatible with `jt-computing`
* [ ] The PR is proposed to proper branch
* [ ] There is reference to original bug report and related work
* [ ] There is accuracy test, performance test and test data in the repository,
  if applicable
* [ ] The feature is well documented and sample code can be built with the project
  CMake
* [ ] The code provides contracts and their enforcement
