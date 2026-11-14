# Generation and compilation of the libraries compared by the tests of a
# configuration (see compare.cxx), shared by the tests of all the
# configurations.
#
# usage: sh ../build-libraries.sh mfront configuration_file
#
# The libraries of the `generic` and `generic-parallel` interfaces of the
# test material properties are built in the current directory with the
# configuration file of the tested configuration: both libraries are
# compiled by the same compiler, with the same options.
set -e
mfront=$1
configuration=$2
case "$mfront" in
  /*) ;;
  */*) mfront=$PWD/$mfront ;;
esac
directory=$(cd "$(dirname "$0")" && pwd)
files=""
for m in PoissonRatioTest Inconel600_YoungModulus Inconel600_YoungModulus-qt \
  UO2_YoungModulus_Martin1989 LinearDataInterpolationTest BoundsOrderTest; do
  files="$files $directory/$m.mfront"
done
"$mfront" --obuild --search-path="$directory" --interface=generic \
  --interface=generic-parallel --configuration-file="$configuration" $files
