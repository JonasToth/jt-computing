# Pull Requests

Note that this is a personal project and your changes might be ignored,
rejected or reimplemented by me [;)].

## Code style and formatting

The code must be clean and follow proper software engineering practices.
Only modern `C++-20` is accepted, especially the use of concepts is required.
But in the end its not about how many features of `C++` were used, but
rather is the code understandable and correct.

Your code requires

* unit-tests
* contracts (use `gsl` assertions)
* consider benchmarks

### Closing Issues

Put `closes #XXXX` in your comment to auto-close the issue that your PR fixes
(if such).

---

Fixes #XXXX

## Pull Request Readiness Checklist

See details at [CONTRIBUTING.md](https://github.com/filipdutescu/modern-cpp-template/blob/master/CONTRIBUTING.md).

* [ ] I agree to contribute to the project under `jt-computing` (GPLv3) [License](LICENSE).
* [ ] To the best of my knowledge, the proposed patch is not based on a code under
      a license that is incompatible with `jt-computing`
* [ ] There is reference to original bug report and related work
* [ ] There is accuracy test, performance test and test data in the repository,
      if applicable
* [ ] The feature is well documented and sample code can be built with the project CMake
* [ ] The code provides contracts and their enforcement
