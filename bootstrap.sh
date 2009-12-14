mkdir -p config
gettextize -f --no-changelog
aclocal -I m4
libtoolize --force --automake
autoconf
automake --add-missing
