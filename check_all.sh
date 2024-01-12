#! /usr/bin/env bash
#
# Usage: check_all.sh [options]
#
# The following options are available:
#
# - `-b` (short for broken-lib-math): shall be specified if the libmath does
#        not support changing the rounding mode
# - `-d` (short for documentation, requires an argument): specify if the
#        documentation must be built. The default value is ON.
#        Valid options are ON and OFF.
# - `-f` (short of fast-check): if specified, builds in debug mode are skipped
# - `-j` (short of parallel-build, requires an argument): if specified, builds
#        in parallel. The arguments is the number of thread allowed.
# - `-m` (short of march=native, requires an argument): specify if processor specific
#        instructions must be used. Valid options are ON and OFF. The default value
#        is ON.
# - `-p` (short of python-bindings): if specified, builds in debug mode are skipped
# - `-r` (short for random-tests, requires an argument): specify if
#        tests where the rounding modes changes randomly shall be executed.
#        Valid options are ON and OFF. The default value is ON.
# - `-w` (short for win-build): if specified, try to cross-compile TFEL
#        with mingw32

# Exit if any error detected
set -e
# parallel build
pbuild=no
# reference documentation
rdoc=ON
# random tests
rtests=ON
# fast check
fcheck=no
# cross compilation using mingw
wbuild=no
# broken_libmath
broken_libmath=OFF
# python-bindings
python_configure_arg=""
python_bindings_configure_arg=""
python_cmake_arg=""
python_bindings_cmake_arg=""
# portable-build
march_native=ON

while getopts "fwbd:r:j:pm:" opt;
do
  case $opt in
    f) fcheck=yes
       ;;
    b) broken_libmath=ON
      ;;
    w) wbuild=yes
      ;;
    d) rdoc="$OPTARG";
       ;;
    r) rtests="$OPTARG";
       ;;
    j) pbuild=yes;
       nbproc="$OPTARG";
       ;;
    m) march_native="$OPTARG";
       ;;
    p)
	python_configure_arg="--enable-python"
	python_bindings_configure_arg="--enable-python-bindings"
	python_cmake_arg="-Denable-python=ON"
	python_bindings_cmake_arg="-Denable-python-bindings=ON"
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

make_exec=make
if test "x$pbuild" == "xyes" ;
then
    make_exec="$make_exec -j $nbproc"
fi

if test "x$march_native" == "xOFF" ;
then
    portable_build="ON"
else
    portable_build="OFF"
fi


# remove previous temporary
if [ -d build-check ];
then
    chmod +w -R build-check
    rm -fr build-check
fi

# source directory
src=$(cd $(dirname $0) && pwd)
# current directory
build=$(pwd)

mkdir build-check
pushd build-check

mkdir install-cmake
mkdir build-cmake
if test "x$fcheck" == "xno" ;
then
    mkdir install-cmake-release
    mkdir build-cmake-release
    mkdir install-cmake-debug
    mkdir build-cmake-debug
    mkdir install-cmake-release-static
    mkdir build-cmake-release-static
fi
pushd build-cmake
cmake ${src} -DTFEL_BROKEN_LIBMATH=$broken_libmath -Dlocal-castem-header=ON -Denable-fortran=ON ${python_cmake_arg} ${python_bindings_cmake_arg} -Denable-aster=ON -Denable-abaqus=ON -Denable-calculix=ON -Denable-comsol=ON -Denable-diana-fea=ON -Denable-ansys=ON -Denable-europlexus=ON -Denable-zmat=ON -Denable-cyrano=ON -Denable-lsdyna=ON -Denable-random-tests=${rtests} -Denable-reference-doc=${rdoc} -Denable-portable-build=${portable_build} -DCMAKE_INSTALL_PREFIX=$build/build-check/cmake/install-cmake
$make_exec 
if [ test "x$pbuild" == "xyes" ];
then
    make check ARGS="-j $nbproc"
else
    $make_exec check 
fi
$make_exec install
popd #from build-cmake
if test "x$fcheck" == "xno" ;
then
    pushd build-cmake-release
    cmake ${src} -DCMAKE_BUILD_TYPE=Release -DTFEL_BROKEN_LIBMATH=$broken_libmath -Dlocal-castem-header=ON -Denable-fortran=ON ${python_cmake_arg} ${python_bindings_cmake_arg} -Denable-aster=ON -Denable-abaqus=ON -Denable-calculix=ON -Denable-comsol=ON -Denable-diana-fea=ON -Denable-ansys=ON -Denable-europlexus=ON -Denable-zmat=ON -Denable-cyrano=ON -Denable-lsdyna=ON -Denable-random-tests=${rtests} -Denable-reference-doc=${rdoc} -Denable-portable-build=${portable_build} -DCMAKE_INSTALL_PREFIX=$build/build-check/cmake/install-cmake-release
    $make_exec
    if [ test "x$pbuild" == "xyes" ];
    then
	make check ARGS="-j $nbproc"
    else
	$make_exec check 
    fi
    $make_exec install
    popd #from build-cmake-release
    pushd build-cmake-debug
    cmake ${src} -DCMAKE_BUILD_TYPE=Debug -DTFEL_BROKEN_LIBMATH=$broken_libmath -Dlocal-castem-header=ON -Denable-fortran=ON ${python_cmake_arg} ${python_bindings_cmake_arg} -Denable-aster=ON -Denable-abaqus=ON -Denable-calculix=ON -Denable-comsol=ON -Denable-diana-fea=ON -Denable-ansys=ON -Denable-europlexus=ON -Denable-zmat=ON -Denable-cyrano=ON -Denable-lsdyna=ON -Denable-random-tests=${rtests} -Denable-reference-doc=${rdoc} -Denable-portable-build=${portable_build} -DCMAKE_INSTALL_PREFIX=$build/build-check/cmake/install-cmake-debug
    $make_exec
    if [ test "x$pbuild" == "xyes" ];
    then
	make check ARGS="-j $nbproc"
    else
	$make_exec check 
    fi
    $make_exec install
    popd #from build-cmake-debug
    pushd build-cmake-release-static
    cmake ${src} -DBUILD_SHARED_LIBS=OFF -DCMAKE_BUILD_TYPE=Release -DTFEL_BROKEN_LIBMATH=$broken_libmath -Dlocal-castem-header=ON -Denable-fortran=ON ${python_cmake_arg} ${python_bindings_cmake_arg} -Denable-aster=ON -Denable-abaqus=ON -Denable-calculix=ON -Denable-comsol=ON -Denable-diana-fea=ON -Denable-ansys=ON -Denable-europlexus=ON -Denable-zmat=ON -Denable-cyrano=ON -Denable-lsdyna=ON -Denable-random-tests=${rtests} -Denable-reference-doc=${rdoc} -Denable-portable-build=${portable_build} -DCMAKE_INSTALL_PREFIX=$build/build-check/cmake/install-cmake-release-static
    $make_exec
    if [ test "x$pbuild" == "xyes" ];
    then
	make check ARGS="-j $nbproc"
    else
	$make_exec check
    fi
    $make_exec install
    popd #from build-cmake-release-static
fi

if test "x$wbuild" == "xyes" ;
then
    if [ "x$(which i686-w64-mingw32-gcc)" != "x" ];
    then
	mkdir build-cmake-i686-w64-mingw32
	pushd build-cmake-i686-w64-mingw32
	cmake ${src} -DTFEL_BROKEN_LIBMATH=$broken_libmath -Dlocal-castem-header=ON -Denable-fortran=ON -Denable-python=OFF -Denable-python-bindings=OFF -Denable-aster=ON -Denable-abaqus=ON -Denable-calculix=ON -Denable-comsol=ON -Denable-diana-fea=ON -Denable-ansys=ON -Denable-europlexus=ON -Denable-zmat=ON -Denable-cyrano=ON -Denable-lsdyna=ON -Denable-random-tests=${rtests} -Denable-reference-doc=${rdoc} -Denable-portable-build=${portable_build} -DCMAKE_INSTALL_PREFIX=$build/build-check/cmake/install-cmake-i686-w64-mingw32 -DCMAKE_TOOLCHAIN_FILE=${src}cmake/ToolChain-i686-w64-mingw32.cmake
	$make_exec
	if [ "x$(which wine)" != "x" ];
	then
	    if [ test "x$pbuild" == "xyes" ];
	    then
		make check ARGS="-j $nbproc"
	    else
		$make_exec check 
	    fi
	fi
	$make_exec install
	popd #from build-cmake-i686-w64-mingw32
    fi
    
    if [ "x$(which i586-mingw32msvc-gcc)" != "x" ];
    then
	mkdir build-cmake-i586-mingw32msvc
	pushd build-cmake-i586-mingw32msvc
	cmake ${src} -DTFEL_BROKEN_LIBMATH=$broken_libmath -Dlocal-castem-header=ON -Denable-fortran=ON -Denable-python=OFF -Denable-python-bindings=OFF -Denable-aster=ON -Denable-abaqus=ON -Denable-calculix=ON -Denable-comsol=ON -Denable-diana-fea=ON -Denable-ansys=ON -Denable-europlexus=ON -Denable-zmat=ON -Denable-cyrano=ON -Denable-lsdyna=ON -Denable-random-tests=${rtests} -Denable-reference-doc=${rdoc} -Denable-portable-build=${portable_build} -DCMAKE_INSTALL_PREFIX=$build/build-check/cmake/install-cmake-i586-mingw32msvc -DCMAKE_TOOLCHAIN_FILE=${src}/cmake/ToolChain-i586-mingw32msvc.cmake
	$make_exec
	if [ "x$(which wine)" != "x" ];
	then
	    if [ test "x$pbuild" == "xyes" ];
	    then
		make check ARGS="-j $nbproc"
	    else
		$make_exec check 
	    fi
	fi
	$make_exec install
	popd #from build-cmake-i586-mingw32msvc
    fi
fi

popd #from build-check
