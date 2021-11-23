[![Actions Status](https://github.com/JonasToth/jt-computing/workflows/MacOS/badge.svg)](https://github.com/JonasToth/jt-computing/actions)
[![Actions Status](https://github.com/JonasToth/jt-computing/workflows/Windows/badge.svg)](https://github.com/JonasToth/jt-computing/actions)
[![Actions Status](https://github.com/JonasToth/jt-computing/workflows/Ubuntu/badge.svg)](https://github.com/JonasToth/jt-computing/actions)
[![codecov](https://codecov.io/gh/JonasToth/jt-computing/branch/master/graph/badge.svg)](https://codecov.io/gh/JonasToth/jt-computing)
[![GitHub release (latest by date)](https://img.shields.io/github/v/release/JonasToth/jt-computing)](https://github.com/JonasToth/jt-computing/releases)

# `jt-computing` - Me Discovering Computation

This project covers my journey to discover the heart of computing using `C++`
and its generic coding strenghts.

## Running the tests

```bash
$ cd build
$ # * or `ctest -C Debug` or any other configuration you wish to test
$ # * you can also run tests with the `-VV` flag for a more verbose output
$ ctest -C Release
> Everything passes always, of course
```

### Coding style tests

* `clang-tidy` must be clean
* `clang-format` must be clean
* `cmake-format` must be clean

## Contributing

Please read [CONTRIBUTING.md](CONTRIBUTING.md) for details on our how you can
become a contributor and the process for submitting pull requests to us.

## Versioning

This project does not follow semantic versioning or anything else. It will
simply change the interfaces and don't care, unless it does.
There might be some point releases, but its not the goal.

It must be always green though.

## Authors

* **Jonas Toth** - [@filipdutescu](https://github.com/JonasToth)

## License

This project is licensed under the [GPLv3](https://www.gnu.org/licenses/gpl-3.0.en.html),
see the [LICENSE](LICENSE) file for details.
