#! /usr/bin/env bash
# Exit if any error detected
set -e
# parallel build
pbuild=no
# reference documentation
rdoc=ON
# fast check
fcheck=no
# cross compilation using mingw
wbuild=no
# fast check
broken_libmath=OFF

while getopts ":w:dbfj:" opt;
do
  case $opt in
    f) fcheck=yes
      ;;
    b) broken_libmath=ON
      ;;
    d) rdoc="$OPTARG";
      ;;
    j) pbuild=yes;
       nbproc="$OPTARG";
      ;;
    w) wbuild=yes
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

# remove previous temporary
if [ -d build-check ];
then
    chmod +w -R build-check
    rm -fr build-check
fi

# source directory
src=$(dirname $(realpath $0))
# current directory
build=$(pwd)

# get the package name
pkg_name=$(cat $src/configure.ac|grep AC_INIT|awk 'BEGIN{FS=","} {print $2}')

mkdir build-check
pushd build-check
mkdir autotools
pushd autotools
mkdir install-autotools
if test "x$fcheck" == "xno" ;
then
    mkdir install-autotools-debug
fi
mkdir build-autotools
pushd build-autotools
$src/configure --enable-python --enable-python-bindings --enable-fortran --enable-abaqus --enable-calculix --enable-ansys --enable-europlexus --enable-zmat --enable-tests --enable-local-castem-header --enable-cyrano --prefix=$build/build-check/autotools/install-autotools 
$make_exec
$make_exec check
$make_exec distcheck
$make_exec doc-pdf
$make_exec install
popd # from build-autotools
if test "x$fcheck" == "xno" ;
then
    mkdir build-autotools-debug
    pushd build-autotools-debug
    $src/configure --enable-debug --enable-python --enable-python-bindings --enable-fortran --enable-abaqus --enable-calculix --enable-ansys --enable-europlexus --enable-zmat --enable-tests --enable-local-castem-header --enable-cyrano --prefix=$build/build-check/autotools/install-autotools-debug 
    $make_exec
    $make_exec check
    $make_exec distcheck
    $make_exec doc-pdf
    $make_exec install
    popd # from build-autotools-debug
fi
popd # from autotools

mkdir cmake
pushd cmake
tar -xvjf $build/build-check/autotools/build-autotools/tfel-$pkg_name.tar.bz2
mkdir install-cmake
mkdir build-cmake
if test "x$fcheck" == "xno" ;
then
    mkdir install-cmake-release
    mkdir build-cmake-release
    mkdir install-cmake-debug
    mkdir build-cmake-debug
fi
pushd build-cmake
cmake ../tfel-$pkg_name/ -DTFEL_BROKEN_LIBMATH=$broken_libmath -Dlocal-castem-header=ON -Denable-fortran=ON -Denable-python=ON -Denable-python-bindings=ON -Denable-aster=ON -Denable-abaqus=ON -Denable-calculix=ON -Denable-ansys=ON -Denable-europlexus=ON -Denable-zmat=ON -Denable-cyrano=ON -Denable-reference-doc=${rdoc} -DCMAKE_INSTALL_PREFIX=$build/build-check/cmake/install-cmake
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
    cmake ../tfel-$pkg_name/ -DCMAKE_BUILD_TYPE=Release -DTFEL_BROKEN_LIBMATH=$broken_libmath -Dlocal-castem-header=ON -Denable-fortran=ON -Denable-python=ON -Denable-python-bindings=ON -Denable-aster=ON -Denable-abaqus=ON -Denable-calculix=ON -Denable-ansys=ON -Denable-europlexus=ON -Denable-zmat=ON -Denable-cyrano=ON -Denable-reference-doc=${rdoc} -DCMAKE_INSTALL_PREFIX=$build/build-check/cmake/install-cmake-release
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
    cmake ../tfel-$pkg_name/ -DCMAKE_BUILD_TYPE=Debug -DTFEL_BROKEN_LIBMATH=$broken_libmath -Dlocal-castem-header=ON -Denable-fortran=ON -Denable-python=ON -Denable-python-bindings=ON -Denable-aster=ON -Denable-abaqus=ON -Denable-calculix=ON -Denable-ansys=ON -Denable-europlexus=ON -Denable-zmat=ON -Denable-cyrano=ON -Denable-reference-doc=${rdoc} -DCMAKE_INSTALL_PREFIX=$build/build-check/cmake/install-cmake-debug
    $make_exec
    if [ test "x$pbuild" == "xyes" ];
    then
	make check ARGS="-j $nbproc"
    else
	$make_exec check 
    fi
    $make_exec install
    popd #from build-cmake-debug
fi

if test "x$wbuild" == "xyes" ;
then
    if [ "x$(which i686-w64-mingw32-gcc)" != "x" ];
    then
	mkdir build-cmake-i686-w64-mingw32
	pushd build-cmake-i686-w64-mingw32
	cmake ../tfel-$pkg_name/ -DTFEL_BROKEN_LIBMATH=$broken_libmath -Dlocal-castem-header=ON -Denable-fortran=ON -Denable-python=OFF -Denable-python-bindings=OFF -Denable-aster=ON -Denable-abaqus=ON -Denable-calculix=ON -Denable-ansys=ON -Denable-europlexus=ON -Denable-zmat=ON -Denable-cyrano=ON -Denable-reference-doc=${rdoc} -DCMAKE_INSTALL_PREFIX=$build/build-check/cmake/install-cmake-i686-w64-mingw32 -DCMAKE_TOOLCHAIN_FILE=../tfel-$pkg_name/cmake/ToolChain-i686-w64-mingw32.cmake
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
	cmake ../tfel-$pkg_name/ -DTFEL_BROKEN_LIBMATH=$broken_libmath -Dlocal-castem-header=ON -Denable-fortran=ON -Denable-python=OFF -Denable-python-bindings=OFF -Denable-aster=ON -Denable-abaqus=ON -Denable-calculix=ON -Denable-ansys=ON -Denable-europlexus=ON -Denable-zmat=ON -Denable-cyrano=ON -Denable-reference-doc=${rdoc} -DCMAKE_INSTALL_PREFIX=$build/build-check/cmake/install-cmake-i586-mingw32msvc -DCMAKE_TOOLCHAIN_FILE=../tfel-$pkg_name/cmake/ToolChain-i586-mingw32msvc.cmake
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

popd #from cmake
popd #from build-check
