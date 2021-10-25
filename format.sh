# /usr/bin/env bash
#
# This scripts uses:
#
# - `clang-format` to format `C++` sources and hearders
# - `yapf` to format `python` sources

if command -v clang-format &> /dev/null
then
  clang-format -i $(find . -name "*.hxx" -o -name "*.ixx" -o -name "*.cxx")
fi

if command -v yapf  &> /dev/null
then
    yapf --style pep8 -i $(find . -name "*.py")
fi
