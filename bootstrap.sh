mkdir -p config
aclocal -I m4
libtoolize --force --automake
autoconf
automake --add-missing
