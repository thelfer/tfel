#!/bin/bash
# Update packages/fedora to a new TFEL release.
#
# Usage: ./update-release.sh <version>   (e.g. ./update-release.sh 4.2.5)
#
# Downloads the GitHub release tarball, checks its checksums and
# updates the `tfel_version` global in tfel.spec and the `sources`
# file (md5 + tarball name, the format used by Fedora dist-git and
# `rpmbuild -ra` / `mock --init`).

set -eu

if [ $# -ne 1 ]; then
  echo "usage: $0 <version>  (e.g. 4.2.5)" >&2
  exit 1
fi

version=$1
dir=$(dirname "$(readlink -f "$0")")
tarball="TFEL-${version}.tar.gz"
url="https://github.com/thelfer/tfel/archive/refs/tags/TFEL-${version}.tar.gz"

echo "downloading ${url}"
mkdir -p "${dir}/SOURCES"
tarball_path="${dir}/SOURCES/${tarball}"
curl -sfL "${url}" -o "${tarball_path}"

md5=$(md5sum "${tarball_path}" | cut -d' ' -f1)
sha256=$(sha256sum "${tarball_path}" | cut -d' ' -f1)

echo "  md5:    ${md5}"
echo "  sha256: ${sha256}"

sed -i "s@^%{!?tfel_version: %global tfel_version .*@%{!?tfel_version: %global tfel_version ${version}}@" \
    "${dir}/tfel.spec"
echo "${md5}  ${tarball}" > "${dir}/sources"

echo "tfel.spec and sources updated to ${version}."
echo "the archive is available in SOURCES/${tarball}"
echo "remember to update %%changelog and to test with:"
echo "  rpmbuild --define '_topdir \${PWD}' -ba tfel.spec"
