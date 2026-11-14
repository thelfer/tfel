# Generation and compilation of the `Inconel600_YoungModulus` material
# property with the runtime checks disabled (`disable_runtime_checks` DSL
# option), in the current directory, with the configuration file of the
# tested configuration (see test-no-runtime-checks.cxx).
#
# usage: sh ../build-no-runtime-checks.sh mfront configuration_file
#
# Some configuration files set this option to `false`, which conflicts
# with the option given on the command line: the material property is
# then generated with a copy of the configuration file setting it to
# `true`.
set -e
mfront=$1
configuration=$2
case "$mfront" in
  /*) ;;
  */*) mfront=$PWD/$mfront ;;
esac
# the configuration file is looked for in the current directory, then in
# the configuration files installed with TFEL, as MFront does
find_configuration() {
  if [ -f "$1" ]; then
    echo "$PWD/$1"
    return
  fi
  prefix=$(cd "$(dirname "$(command -v "$mfront")")/.." && pwd)
  for d in "$prefix"/share/tfel*/mfront/configuration-files; do
    if [ -f "$d/$1" ]; then
      echo "$d/$1"
      return
    fi
  done
  echo "configuration file '$1' not found" >&2
  exit 1
}
configuration=$(find_configuration "$configuration")
directory=$(cd "$(dirname "$0")" && pwd)
file=$directory/Inconel600_YoungModulus.mfront
if grep -q 'disable_runtime_checks' "$configuration"; then
  sed 's/disable_runtime_checks: *false/disable_runtime_checks: true/' \
    "$configuration" > no-runtime-checks.json
  "$mfront" --interface=generic-parallel \
    --configuration-file=no-runtime-checks.json "$file"
else
  cp "$configuration" no-runtime-checks.json
  "$mfront" --interface=generic-parallel \
    --dsl-option=disable_runtime_checks:true \
    --configuration-file=no-runtime-checks.json "$file"
fi
"$mfront" --obuild --configuration-file=no-runtime-checks.json
