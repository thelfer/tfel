AC_DEFUN([CHECK_BOOST_PYTHON],[
	AC_CHECKING(for BOOST PYTHON Library)
	AC_REQUIRE([CHECK_BOOST])dnl

	AC_REQUIRE([ENABLE_PTHREADS])dnl

	AC_LANG_SAVE
	AC_LANG_CPLUSPLUS
	
	echo "$BOOST_LIBSUFFIX"
	if test "x${BOOSTDIR}" != "x";
	then
	    BOOST_INSTALL_DIR=${BOOSTDIR}
	    BOOST_PYTHON_LIBS="-L${BOOSTDIR}/lib/"
	else
	    BOOST_INSTALL_DIR="/usr"
	fi
	AC_CHECK_FILE(${BOOST_INSTALL_DIR}/lib/libboost_python${BOOST_LIBSUFFIX}.so,
            boost_lib_dir_ok=yes,
            boost_lib_dir_ok=no)
	if test "x${boost_lib_dir_ok}" = "xyes" ;
	then
	    BOOST_PYTHON_LIBS="${BOOST_PYTHON_LIBS} -lboost_python${BOOST_LIBSUFFIX}"
	fi
	if test "x${boost_lib_dir_ok}" = "xno" ;
	then
	    # for squeeze
	    AM_PATH_PYTHON([2.4])
	    BOOST_PYTHONVERSION=$(echo $PYTHON_VERSION |sed 's/\.//')
	    AC_CHECK_FILE(${BOOST_INSTALL_DIR}/lib/libboost_python${BOOST_LIBSUFFIX}-py${BOOST_PYTHONVERSION}.so,
                boost_lib_dir_ok=yes,
                boost_lib_dir_ok=no)
 	    if test "x${boost_lib_dir_ok}" = "xyes" ;
	    then
	       BOOST_PYTHON_LIBS="${BOOST_PYTHON_LIBS} -lboost_python${BOOST_LIBSUFFIX}-py${BOOST_PYTHONVERSION}"
	    fi
	fi
	if test "x${boost_lib_dir_ok}" = "xno" ;
	then
	    # for jessy
	    AM_PATH_PYTHON([2.4])
	    BOOST_PYTHONVERSION=$(echo $PYTHON_VERSION |sed 's/\.//')
	    AC_CHECK_FILE(${BOOST_INSTALL_DIR}/lib/x86_64-linux-gnu/libboost_python${BOOST_LIBSUFFIX}-py${BOOST_PYTHONVERSION}.so,
                boost_lib_dir_ok=yes,
                boost_lib_dir_ok=no)
 	    if test "x${boost_lib_dir_ok}" = "xyes" ;
	    then
	       BOOST_PYTHON_LIBS="${BOOST_PYTHON_LIBS} -lboost_python${BOOST_LIBSUFFIX}-py${BOOST_PYTHONVERSION}"
	    fi
	fi
	if test "x${boost_lib_dir_ok}" = "xno" ;
	then
		AC_ERROR("no suitable version of the boost python library found");
	fi
	AC_SUBST([BOOST_PYTHON_LIBS])
	AC_LANG_RESTORE

])dnl
