#!/usr/bin/env python3
# SPDX-License-Identifier: MIT
"""pcons build for JonasToth/jt-computing.

A real-world C++20-modules workout for pcons:
  - 4 module interface units (.cppm) that pull in many partition .cpp files
  - Internal partitions (`module M:P;` with no `export`)
  - Heavy use of `import std;` from .cpp partitions
  - 7 small executables in bin/ that consume the library

`import std;` works on both supported toolchains:
  - MSVC: pcons compiles `%VCToolsInstallDir%/modules/std.ixx`.
  - Clang/libc++: pcons consults `libc++.modules.json` (queried via
    `clang++ -print-file-name=c++/libc++.modules.json`), compiles the
    `std.cppm` it points to, and links the resulting `.o`.

  Apple Clang doesn't yet ship `libc++.modules.json`; on macOS use
  Homebrew LLVM (`brew install llvm`). Linux needs libc++-dev ≥ 18.
  Pcons ≥ 0.16.0 is required for the clang side.

`sha256sum.x` uses `std::execution::par` (parallel STL):
  - On MSVC the parallel-STL backend is Microsoft's PPL — no extra link
    inputs needed; sha256sum builds out of the box.
  - On Linux/Clang+libc++ the parallel-STL backend lives in
    `libc++experimental.a` and dispatches to TBB; pcons resolves TBB via
    Conan from the included `conanfile.txt` and links it in.
  - On macOS the same Conan-resolved TBB binary is linked against the
    system libc++ that Conan's `apple-clang` profile expects, while the
    rest of the binary uses Homebrew libc++ at run time. The two libc++
    ABIs disagree at load time, so sha256sum is skipped on this
    combination. Workarounds: build TBB from source (`conan install
    --build=tbb`) or use system Apple Clang (no `import std;` yet).
"""

import platform as _platform
import shutil as _shutil
from pathlib import Path as _Path

import pcons
from pcons import Project, find_c_toolchain
from pcons.configure.config import Configure

# Module sources: every C++ TU that participates in the module graph
# (interfaces *and* their partition implementations). They all need to
# go on the same target so the scanner can resolve `import :Foo;` across
# them. Mirrors `module_sources` in cmake/SourcesAndHeaders.cmake.
MODULE_SOURCES = [
    "lib/core/Core.cppm",
    "lib/core/Constants.cpp",
    "lib/core/Types.cpp",
    "lib/container/Container.cppm",
    "lib/container/BitVector.cpp",
    "lib/crypto/Crypto.cppm",
    "lib/crypto/Concepts.cpp",
    "lib/crypto/Sha256.cpp",
    "lib/crypto/TextbookRSA.cpp",
    "lib/math/Math.cppm",
    "lib/math/BigInt.cpp",
    "lib/math/BigUInt.cpp",
    "lib/math/BigUIntImpl.cpp",
    "lib/math/Concepts.cpp",
    "lib/math/FixedSquareMatrix.cpp",
    "lib/math/GenericPower.cpp",
    "lib/math/ModularArithmetic.cpp",
    "lib/math/NaturalN.cpp",
    "lib/math/NaturalNImpl.cpp",
    "lib/math/NaturalNumberAlgorithms.cpp",
    "lib/math/NumberIO.cpp",
    "lib/math/NumberIOImpl.cpp",
    "lib/math/Rational.cpp",
    "lib/math/Operations.cpp",
]

EXECUTABLES = [
    "calculate_fibonacci",
    "find_prime_numbers",
    "collatz_chain",
    "percolation_power",
    "shortest_path",
    "toy_rsa",
]

project = Project("jt-computing")
toolchain = find_c_toolchain(prefer=["msvc", "llvm"])
env = project.Environment(toolchain=toolchain)
env.cxx.modules = True

if toolchain.name == "msvc":
    env.cxx.flags.extend(["/std:c++latest", "/EHsc", "/permissive-"])
else:
    # C++23 (Sha256.cpp uses std::byteswap), libc++, and the
    # experimental-library switch that exposes `std::execution::par`
    # for sha256sum.cpp's parallel transform.
    env.cxx.flags.extend(
        ["-std=c++23", "-stdlib=libc++", "-fexperimental-library"]
    )

env.cxx.includes.append(str(project.root_dir / "include"))


def _resolve_tbb_for_sha256sum() -> object | None:
    """Resolve TBB via Conan for sha256sum's parallel STL backend.

    Returns None if Conan isn't available, the conanfile is missing, or
    the host toolchain combination is known not to work cleanly (macOS
    Homebrew clang, where Conan's `apple-clang`-built TBB binary is
    linked against system libc++ and clashes with brew libc++ at
    runtime). MSVC needs no TBB at all — Microsoft's PPL is the
    parallel-STL backend.
    """
    if toolchain.name == "msvc":
        return None  # PPL is built into MSVC's STL
    if _platform.system() == "Darwin":
        # See module docstring — libc++ ABI mismatch makes the linked
        # binary fail at load time.
        return None
    try:
        from pcons.packages.finders import ConanFinder
    except ImportError:
        return None
    conanfile = project.root_dir / "conanfile.txt"
    if not conanfile.is_file():
        return None
    try:
        config = Configure(build_dir=project.build_dir)
        finder = ConanFinder(
            config,
            conanfile=conanfile,
            output_folder=project.build_dir / "conan",
        )
        finder.sync_profile(toolchain, build_type="Release")
        packages = finder.install()
    except Exception as exc:
        print(f"pcons: sha256sum.x — Conan/TBB resolution failed: {exc}")
        return None
    return packages.get("onetbb") or packages.get("tbb")


lib = project.StaticLibrary("JTComputing", env)
lib.add_sources([project.root_dir / s for s in MODULE_SOURCES])

for name in EXECUTABLES:
    exe = project.Program(f"{name}.x", env)
    exe.add_sources([project.root_dir / "bin" / f"{name}.cpp"])
    exe.link(lib)

# sha256sum.x: parallel STL needs different link inputs per toolchain.
build_sha = False
sha_env = project.Environment(toolchain=toolchain)
sha_env.cxx.modules = True
sha_env.cxx.flags.extend(env.cxx.flags)
sha_env.cxx.includes.extend(env.cxx.includes)

if toolchain.name == "msvc":
    # MSVC: parallel STL is built into the runtime; no extra inputs.
    build_sha = True
    print("pcons: sha256sum.x will use MSVC's native PPL parallel-STL backend")
elif _platform.system() == "Darwin":
    print(
        "pcons: skipping sha256sum.x on macOS — libc++ runtime ABI "
        "mismatch with Conan's TBB binary (see pcons-build.py docstring)"
    )
else:
    tbb_pkg = _resolve_tbb_for_sha256sum()
    if tbb_pkg is not None:
        sha_env.use(tbb_pkg)
        # libc++'s parallel-STL backend lives in libc++experimental.a.
        sha_env.link.libs.append("c++experimental")
        # Resolve the libc++ dir from the clang we're using so we link
        # the matching libc++experimental, not a system one.
        clang_path = _shutil.which(env.cxx.cmd)
        if clang_path:
            libcxx_dir = _Path(clang_path).resolve().parent.parent / "lib" / "c++"
            if libcxx_dir.is_dir():
                sha_env.link.libdirs.append(str(libcxx_dir))
                sha_env.link.flags.append(f"-Wl,-rpath,{libcxx_dir}")
        for lib_dir in getattr(tbb_pkg, "library_dirs", []) or []:
            sha_env.link.flags.append(f"-Wl,-rpath,{lib_dir}")
        build_sha = True
        version = getattr(tbb_pkg, "version", "?")
        print(f"pcons: sha256sum.x will link against TBB {version}")
    else:
        print("pcons: skipping sha256sum.x — TBB not available")

if build_sha:
    sha = project.Program("sha256sum.x", sha_env)
    sha.add_sources([project.root_dir / "bin" / "sha256sum.cpp"])
    sha.link(lib)

project.generate()
print(f"pcons {pcons.__version__} ({toolchain.name}): wrote build.ninja")
