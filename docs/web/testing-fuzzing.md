---
title: Fuzz testing the tokenizer with libFuzzer
author: Thomas Helfer
date: 11/11/2026
lang: en-EN
link-citations: true
colorlinks: true
figPrefixTemplate: "$$i$$"
tblPrefixTemplate: "$$i$$"
secPrefixTemplate: "$$i$$"
eqnPrefixTemplate: "($$i$$)"
---

This page describes how to build and run the `libFuzzer` based analyses of the
`tfel::utilities::CxxTokenizer` class, which is the tokenizer `MFront` uses to
read the files of its domain specific language and `tfel-doc` and `tfel-check`
use to read `C++` files. Those analyses are gathered in the
`additional-tests/fuzzing/CxxTokenizer` directory of the sources.

## Why the tokenizer is fuzzed

The tokenizer is the first stage of the processing of every `MFront` file, so
a defect of this class affects all the tools of the project. Moreover, it
processes arbitrary text, which is exactly the kind of code coverage guided
fuzzing is efficient at testing.

A fuzzing analysis generates inputs from a database, called the corpus of the
analysis, and runs the analysed code with them as long as it is left running:
the duration of an analysis is thus not bounded a priori, which is why those
analyses are neither built nor run by `make check`, unlike the other tests of
`TFEL`.

## Requirements

`libFuzzer` is a sanitizer of `clang`, which is provided by `compiler-rt`. The
analyses can only be built with `clang` (or `AppleClang`) and require the
runtimes of the sanitizers to be installed (package `libclang-rt-<version>-dev`
or `libclang-common-<version>-dev` on the distributions where they are separate
packages). Both requirements are checked when the directory is configured: a
configuration using another compiler or a `clang` without the `libFuzzer`
runtime fails with an explicit message instead of building broken targets.

The analysed sources are compiled with the address sanitizer in addition to
`libFuzzer`. As the sanitizers are only effective if the analysed code is
instrumented, the sources of the tokenizer are compiled with the fuzzing
executables instead of being taken from the `TFELUtilities` library: adding the
sanitizers to the libraries of the project would affect all the targets of the
build.

## Configuring the analyses

The analyses are configured by the `enable-libfuzzer-tests` option, which is
`OFF` by default:

~~~~ {#fuzzing-configure .bash}
$ cmake <srcdir> -B build -DCMAKE_BUILD_TYPE=Release \
      -DCMAKE_CXX_COMPILER=clang++ -Denable-libfuzzer-tests=ON
~~~~

The following `cmake` options control the analyses:

| Option | Default | Description |
|--------|---------|-------------|
| `enable-libfuzzer-tests` | `OFF` | configure the libFuzzer based analyses |
| `libfuzzer-tests-max-total-time` | `60` | duration of each analysis, in seconds |
| `libfuzzer-tests-max-len` | `65536` | maximum length of the generated inputs, in bytes (`0`: no limit) |
| `mfront-fuzzing-database` | `single-file` | database of the `mfront-fuzzing` analysis: `single-file` or `all` |
| `cxx-fuzzing-database` | `single-file` | database of the `cxx-fuzzing` analysis: `single-file` or `all` |

## Building and running the analyses

Two targets are dedicated to those analyses:

- `build-cxxtokenzier-libfuzzer-tests` builds the fuzzing executables,
- `cxxtokenzier-libfuzzer-tests` builds them and runs the analyses.

~~~~ {#fuzzing-build .bash}
$ cmake --build build --target build-cxxtokenzier-libfuzzer-tests
$ cmake --build build --target cxxtokenzier-libfuzzer-tests
~~~~

The fuzzing executables are excluded from the default target and no test is
registered with `add_test`: neither `make all` nor `make check` builds or runs
them. Only the second target above runs the analyses, for the duration given by
the `libfuzzer-tests-max-total-time` option.

A fuzzing executable is also a normal program, which can be run by hand to
control the analysed executable, the duration of the analysis or the inputs it
processes:

~~~~ {#fuzzing-run-by-hand .bash}
$ build/additional-tests/fuzzing/CxxTokenizer/mfront-fuzzing \
      -max_total_time=3600 -max_len=0 \
      build/additional-tests/fuzzing/CxxTokenizer/mfront-fuzzing-corpus
~~~~

The flags of `libFuzzer` are documented by the `-help=1` flag of the fuzzing
executables.

## Corpus and artifacts

Each analysis has two directories in the build tree:

- `build/additional-tests/fuzzing/CxxTokenizer/<name>-corpus` is the corpus of
  the analysis: it is fed with the database at configuration time and
  `libFuzzer` adds to it the inputs of interest it generates. Only that
  directory is given to the fuzzing executables, so that the inputs generated
  by an analysis are kept in the build tree and never in the sources. Since the
  corpus of the previous runs is kept, an analysis is resumed when it is run
  again, which is what makes long campaigns useful:

  ~~~~ {#fuzzing-merge .bash}
  $ cmake --build build --target cxxtokenzier-libfuzzer-tests
  $ cmake --build build --target cxxtokenzier-libfuzzer-tests    # resumes the analysis
  ~~~~

- `build/additional-tests/fuzzing/CxxTokenizer/<name>-artifacts` gathers the
  inputs generated by the analysis which lead to a crash, to a timeout or to a
  suspiciously slow execution (`crash-*`, `timeout-*` and `slow-*` files). It is
  empty when the analysis does not find anything: checking that this directory
  stays empty is the whole point of running an analysis.

Note that `libFuzzer` requires the directory of its corpus to exist, which is
why those directories are created at configuration time and not at run time.

## The `mfront-fuzzing` analysis

The `mfront-fuzzing` analysis tokenizes the contents of `MFront` files with the
`tfel::utilities::CxxTokenizer` class, using the same options as `MFront`: the
default options of the tokenizer and the separators specific to `MFront` files,
which are added by the `MFront::DSLBase` structure. Such a file is a mixture of
`C++` code, `MFront` keywords, comments, strings with unicode characters and
preprocessor directives, which is a very good source of inputs.

The database of the analysis is controlled by the `mfront-fuzzing-database`
option:

| Value | Database |
|-------|----------|
| `single-file` | the `mfront/tests/behaviours/Norton.mfront` file |
| `all` | all the `MFront` files of `mfront/tests/behaviours` |

The `single-file` value is the default: it makes the setup of the analysis much
faster, as a large database is first merged, that is tokenized once, when the
analysis starts, and as a small corpus makes the first runs of a new analysis
much quicker to explore. Using all those files is the intended use of the
analysis:

~~~~ {#fuzzing-all .bash}
$ cmake <srcdir> -B build -DCMAKE_BUILD_TYPE=Release \
      -DCMAKE_CXX_COMPILER=clang++ -Denable-libfuzzer-tests=ON \
      -Dmfront-fuzzing-database=all -Dlibfuzzer-tests-max-total-time=3600
~~~~

Beyond the absence of any sanitizer report, the analysis checks that the tokens
the tokenizer produces describe the file it is read from, that is the contract
of the `tfel::utilities::Token` class, which the methods of `MFront` rely on:

- the lines of the successive tokens are non decreasing,
- the offsets of the tokens of a given line are strictly increasing,
- the offset of a token is the position of a character of the line the token
  starts on,
- a token which cannot span several lines, that is a standard token, a number or
  a character literal, is contained in its line,
- a token which is not a comment has a non empty value,
- no comment token is kept by the `stripComments` method.

Those invariants hold for every token, without exception: the value of a raw
string literal holds the whole literal, as a quoted string holds its quotes, so
that its value is never empty and its offset is that of its `R` delimiter, see
issue #1390, and the offset of an empty comment is that of the delimiter which
opens it, see issue #1391. Two kinds of tokens have a value shorter than their
spelling in the file, which does not harm those invariants: a comment, whose
value leaves out its delimiters, and a number, whose value leaves out the digit
separators of the `C++14` integer literals.

The lexical constructs those invariants and those exceptions come from are covered
by the `CxxTokenizerContractTest` unit test of `tests/Utilities`.

As the tokenizer is expected to reject the invalid inputs `libFuzzer` generates
by throwing an exception, those checks do not use `tfel::raise`, whose exception
would be caught by the harness: an inconsistency is reported on the standard
error output and followed by a call to `std::abort`, which is reported by
`libFuzzer` as a failure of the analysis and stops it.

## The `cxx-fuzzing` analysis

The `cxx-fuzzing` analysis tokenizes its inputs with the default options of the
`tfel::utilities::CxxTokenizer` class, that is as `C++` files, as `tfel-doc` and
`tfel-check` do: the preprocessor directives are treated, the strings are
extracted, the two characters separators are joined and no separator specific to
a domain specific language is added. The `MFront` files of the `mfront-fuzzing`
analysis are a mixture of `C++` code and `MFront` keywords, so that this analysis
completes it with the constructs of a real world `C++` code which the `MFront`
files hardly use: the macro expansions and the line continuations of the
preprocessor directives, the escape sequences of the quoted strings, which are
meaningless to the tokenizer since the strings are not interpreted, the adjacent
string literals which `C++` concatenates, the doxygen comments of all the forms,
etc...

The database of the analysis is controlled by the `cxx-fuzzing-database` option:

| Value | Database |
|-------|----------|
| `single-file` | the `src/Utilities/CxxTokenizer.cxx` file |
| `all` | all the `C++` files of the project |

With the `all` value, the database gathers the `C++` files of the project, those
whose extension is `.c`, `.cc`, `.cpp`, `.cxx`, `.h`, `.hh`, `.hpp`, `.hxx` or
`.ixx`, which is nearly 2,300 files, 16 MB and 417,000 lines, four times the
number of files of the database of the `mfront-fuzzing` analysis, excepted the
five files described below. Those files are copied to the corpus of the analysis
with a name built from their path, as 112 of their names are duplicated, so that
the origin of every input of the corpus is kept:

~~~~ {#fuzzing-cxx .bash}
$ cmake <srcdir> -B build -DCMAKE_BUILD_TYPE=Release \
      -DCMAKE_CXX_COMPILER=clang++ -Denable-libfuzzer-tests=ON \
      -Dcxx-fuzzing-database=all -Dlibfuzzer-tests-max-total-time=3600
$ ls build/additional-tests/fuzzing/CxxTokenizer/cxx-fuzzing-corpus | head -2
additional-tests_fuzzing_CxxTokenizer_cxx-fuzzing.cxx
additional-tests_fuzzing_CxxTokenizer_mfront-fuzzing.cxx
~~~~

Seventeen files of this database are larger than the `65536` bytes the
`libfuzzer-tests-max-len` option defaults to, and `libFuzzer` ignores the inputs
longer than that limit: use `-Dlibfuzzer-tests-max-len=0` to process those files
too.

Five files of the project are left out of this database since the tokenizer
rejects them as they are: four inputs of the tools of the project whose names kept
the extension of the sources they were made from, an `MFront` file, two inputs of
`Pleiades` and an input of `tfel-check`, and a source which is not built by the
project anymore. A file the tokenizer rejects cannot be a part of a corpus:
`libFuzzer` would report the exception thrown for such an input as soon as it
replays it, whereas an exception is the expected behaviour for the invalid inputs
the fuzzer generates.

This analysis checks the invariants of the `mfront-fuzzing` analysis, that is
those of the `tfel::utilities::Token` class, and two more, which the number and
the variety of the inputs it processes makes useful:

- the value of a token which is neither a comment nor a number is the substring of
  its line taken at its offset, as stated by the documentation of the
  `tfel::utilities::Token` class. This invariant does not apply to the tokens
  which span several lines, whose value gathers those lines,
- the contents of a literal, that is what the `tfel::utilities::getContents`
  function returns, is a part of the value of its token, whether the literal is
  closed or the file ends inside it, see issues #1390 and #1395.

Both invariants are checked on every input of the database when the analysis
starts, since `libFuzzer` replays its corpus, and are satisfied by all the
`C++` files of the project.

## Treating a failure

When an analysis fails, the offending input is written in the artifacts
directory of the analysis (`crash-*` or `timeout-*` files) and the report of the
sanitizers, if any, is printed by the target. An input is replayed by giving it
to the fuzzing executable in place of a corpus:

~~~~ {#fuzzing-replay .bash}
$ build/additional-tests/fuzzing/CxxTokenizer/mfront-fuzzing \
      build/additional-tests/fuzzing/CxxTokenizer/mfront-fuzzing-artifacts/crash-xxxx
~~~~

The input can then be reduced to the smallest input triggering the same report,
which is the input worth adding to the regression tests:

~~~~ {#fuzzing-minimize .bash}
$ ASAN_OPTIONS=dedup_token_length=3 \
  build/additional-tests/fuzzing/CxxTokenizer/mfront-fuzzing \
      -minimize_crash=1 -runs=100000 \
      -exact_artifact_path=minimized-xxxx \
      build/additional-tests/fuzzing/CxxTokenizer/mfront-fuzzing-artifacts/crash-xxxx
~~~~

Note that an analysis can also fail on an inconsistency detected by the harness
itself, in which case there is no sanitizer report: the report is the message
the harness prints before aborting, which names the violated invariant, such as
`decreasing line number obtained tokenizing`, `non increasing offset obtained
tokenizing`, `offset out of the line obtained tokenizing`, `empty token obtained
tokenizing`, `value of a token which is not its spelling in the file obtained
tokenizing`, `contents of a literal which is not a part of the value of its token
obtained tokenizing` or `comment token kept by stripComments`.

## Adding an analysis

An analysis is a source file defining the `LLVMFuzzerTestOneInput` function,
which is called by `libFuzzer` with the generated inputs, and a call to the
`tfel_add_cxxtokenizer_libfuzzer_test` function, which creates the fuzzing
executable, adds it to the target `build-cxxtokenzier-libfuzzer-tests` and
registers it in the target `cxxtokenzier-libfuzzer-tests`:

~~~~ {#fuzzing-add .cmake}
tfel_add_cxxtokenizer_libfuzzer_test(
  name name.cxx ${PROJECT_SOURCE_DIR}/path/to/the/database)
~~~~

The database is a list of files or directories: a directory is meant
recursively and all the files it contains are added to the corpus of the
analysis. If the first element of that list is `FLATTEN_NAMES`, the files are
copied to the corpus with a name built from their path instead of their only
name, which is what the `cxx-fuzzing` analysis needs since the databases of the
analyses of a whole project spread over many directories hold files sharing the
same name.

A harness shall catch the exceptions thrown by the analysed code when they
correspond to a rejection of an invalid input, as those exceptions are the
normal outcome of most of the generated inputs, and shall not use them to
report a defect of the analysed code, as such a report would be silently
ignored:

~~~~ {#fuzzing-harness .cpp}
extern "C" int LLVMFuzzerTestOneInput(const uint8_t* const data,
                                      const size_t size) {
  try {
    analyse(reinterpret_cast<const char*>(data), size);
  } catch (const std::exception&) {
    // the tokenizer throws an exception when the input is not a valid
    // C++ like file: unterminated string or comment, stray character, etc...
    // Such an exception is not a failure of the analysis
  }
  return 0;
}  // end of LLVMFuzzerTestOneInput
~~~~

The source files the analysed code depends on shall be added to the
`tfel_cxxtokenizer_sources` variable for them to be instrumented by the
sanitizers as well.

## Known pitfalls

### Debugging a fuzzing executable

The address sanitizer intercepts the signals it reports, so that a genuine
crash does not stop the debugger on the faulty instruction: use
`ASAN_OPTIONS=handle_segv=0` to let the debugger handle the segmentation faults.
Moreover, the leak sanitizer does not work under a `ptrace` based debugger, such
as `gdb`, and exits with a fatal error: use `ASAN_OPTIONS=detect_leaks=0` when
running a fuzzing executable under such a debugger.

### Crashes at start up

`libFuzzer` requires the directory of its corpus to exist: a missing directory
leads to a crash without any report, which is why the corpus directories are
created at configuration time.

The address sanitizer maps its allocator at a fixed address of the address
space, which can be the address the dynamic loader chooses for a position
independent executable. Such a collision makes the executable crash before the
analysis starts, without any report, and depends on how the address space is
laid out, so that it only happens from time to time on systems where the layout
of the address space is randomised. The fuzzing executables are therefore built
with the `-no-pie` flag: they are analysis tools, which are not installed, so
that position independent code is not needed.

### Long running analyses

The memory used by an analysis grows with the size of its corpus: the
`rss_limit_mb` flag of `libFuzzer`, which is `2048` by default, makes the
analysis stop when it exceeds this limit, which is reported as an
`out of memory` error. Long campaigns are thus better run by slices, using the
same corpus directory, than by a single very long run.

The `-jobs=<n>` and `-workers=<n>` flags of `libFuzzer` run several fuzzing
processes simultaneously, which is the only efficient way of using many cores.
They are not used by the targets of the project since `libFuzzer` returns a non
zero status when a parallel analysis does not find any new interesting input, so
that a normal run of the analysis would be reported as a failure of the target.

## Going further

- The sanitizers used to build the tests of `TFEL/MFront` are described in the
  documentation on [running sanitized tests](sanitizers.html).
- The static analysis of `TFEL` by `Coverity` is described in the documentation
  on [Coverity Scan](coverity-scan.html), which is complementary to fuzzing.
- The `-help=1` flag of a fuzzing executable lists all the flags of `libFuzzer`,
  and the `libFuzzer` documentation describes how to extend the analyses with
  dictionaries, token dictionaries and merge strategies.
