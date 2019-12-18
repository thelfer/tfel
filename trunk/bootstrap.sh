mkdir -p config
aclocal -I m4
libtoolize --force --automake
autoconf

AUTOMAKE_VERSION=$(automake --version|head -n 1|awk '{print $NF}')
AUTOMAKE_MAJOR_VERSION=$(echo $AUTOMAKE_VERSION |awk -F. '{print $1}')
AUTOMAKE_MINOR_VERSION=$(echo $AUTOMAKE_VERSION |awk -F. '{print $2}')

if test $AUTOMAKE_MAJOR_VERSION -le 1 -a $AUTOMAKE_MINOR_VERSION -lt 14;
then
    automake --add-missing
else
    tmp=$(mktemp)
    cp configure.ac $tmp
    sed -i 's/tar-ustar/tar-ustar subdir-objects/g' configure.ac
    automake --add-missing
    cp $tmp configure.ac
fi
