#! /usr/bin/env bash
# Exit if any error detected
set -e

pbuild=no
while getopts ":j:" opt;
do
  case $opt in
    j) pbuild=yes;
       nbproc="$OPTARG";
      ;;
    \?)
      echo "$0 : invalid option '$OPTARG'" >&2
      echo "bailing out." >&2
      exit 1
      ;;
    :)
      echo "$0 : option '-$OPTARG' requires an argument." >&2
      echo "bailing out." >&2
      exit 1
      ;;
  esac
done

# source directory
src=$(dirname $(realpath $0))
# current directory
build=$(pwd)

cppcheck_exec=cppcheck
if test "x$pbuild" == "xyes" ;
then
    cppcheck_exec="$cppcheck_exec -j $nbproc"
fi

if test x"$(uname -m)" == "x86_64";
then
    TFEL_ARCH=TFEL_ARCH64
    ASTER_ARCH=64
    CYRANO_ARCH=64
else
    TFEL_ARCH=TFEL_ARCH32
    ASTER_ARCH=32
    CYRANO_ARCH=32
fi

$cppcheck_exec -UTFEL_MATH_USE_HARD_INLINING -D$TFEL_ARCH -DASTER_ARCH=$ASTER_ARCH -DCYRANO_ARCH=$CYRANO_ARCH -DLOCAL_CASTEM_HEADER -I$src/include -I$src/mfront/include -I$src/tfel-doc/include $(for f in $(find $src -name "*.cxx"|grep -v docs); do echo $(dirname $f); done|uniq) --suppress=missingIncludeSystem  --enable=all
