#! /usr/bin/env bash

src=$(dirname $(realpath $0))
mkdir build-check
pushd build-check
build=$(pwd)
mkdir autotools
pushd autotools
mkdir install-autotools
mkdir build-autotools
pushd build-autotools
$src/configure --enable-python --enable-python-bindings --enable-fortran --enable-aster --enable-tests --enable-local-castem-header --enable-cyrano --prefix=$build/autotools/install-autotools 
make
make check
make distcheck
make doc
make install
make doc-install
popd # from build-autotools
popd # from autotools
mkdir cmake
pushd cmake
tar -xvjf $build/autotools/build-autotools/tfel-2.0-dev.tar.bz2
mkdir install-cmake-release
mkdir build-cmake-release
mkdir install-cmake-debug
mkdir build-cmake-debug
pushd build-cmake-release
cmake ../tfel-2.0-dev/ -DCMAKE_BUILD_TYPE=Release -Dlocal-castem-header=ON -Denable-fortran=ON -Denable-python=ON -Denable-python-bindings=ON -Denable-aster=ON -Denable-cyrano=ON -DCMAKE_INSTALL_PREFIX=$build/cmake/install-cmake-release
make
make check
make install
make doc-install
make tests-install
popd #from build-cmake-release
pushd build-cmake-debug
cmake ../tfel-2.0-dev/ -DCMAKE_BUILD_TYPE=Debug -Dlocal-castem-header=ON -Denable-fortran=ON -Denable-python=ON -Denable-python-bindings=ON -Denable-aster=ON -Denable-cyrano=ON -DCMAKE_INSTALL_PREFIX=$build/cmake/install-cmake-release
make
make check
make install
make doc-install
make tests-install
popd #from build-cmake-debug
popd #from cmake
popd #from build-check


