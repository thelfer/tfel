AC_DEFUN([CHECK_BOOST_NUMPY],[
	AC_CHECKING(for BOOST NUMPY Library)
	AC_REQUIRE([CHECK_BOOST])dnl

	AC_REQUIRE([ENABLE_PTHREADS])dnl

	AC_LANG_SAVE
	AC_LANG_CPLUSPLUS
	
	echo "$BOOST_LIBSUFFIX"
	if test "x${BOOSTDIR}" != "x";
	then
	    BOOST_INSTALL_DIR=${BOOSTDIR}
	    BOOST_NUMPY_LIBS="-L${BOOSTDIR}/lib/"
	else
	    BOOST_INSTALL_DIR="/usr"
	fi
	AC_CHECK_FILE(${BOOST_INSTALL_DIR}/lib/libboost_numpy${BOOST_LIBSUFFIX}.so,
            boost_lib_dir_ok=yes,
            boost_lib_dir_ok=no)
	if test "x${boost_lib_dir_ok}" = "xyes" ;
	then
	    BOOST_NUMPY_LIBS="${BOOST_NUMPY_LIBS} -lboost_numpy${BOOST_LIBSUFFIX}"
	fi
	if test "x${boost_lib_dir_ok}" = "xno" ;
	then
	    # for squeeze
	    AM_PATH_PYTHON([2.4])
	    BOOST_NUMPYVERSION=$(echo $PYTHON_VERSION |sed 's/\.//')
	    AC_CHECK_FILE(${BOOST_INSTALL_DIR}/lib/libboost_numpy${BOOST_LIBSUFFIX}${BOOST_NUMPYVERSION}.so,
                boost_lib_dir_ok=yes,
                boost_lib_dir_ok=no)
 	    if test "x${boost_lib_dir_ok}" = "xyes" ;
	    then
	       BOOST_NUMPY_LIBS="${BOOST_NUMPY_LIBS} -lboost_numpy${BOOST_LIBSUFFIX}${BOOST_NUMPYVERSION}"
	    fi
	fi
	if test "x${boost_lib_dir_ok}" = "xno" ;
	then
	    # for squeeze
	    AM_PATH_PYTHON([2.4])
	    BOOST_NUMPYVERSION=$(echo $PYTHON_VERSION |sed 's/\.//')
	    AC_CHECK_FILE(${BOOST_INSTALL_DIR}/lib/libboost_numpy${BOOST_LIBSUFFIX}-py${BOOST_NUMPYVERSION}.so,
                boost_lib_dir_ok=yes,
                boost_lib_dir_ok=no)
 	    if test "x${boost_lib_dir_ok}" = "xyes" ;
	    then
	       BOOST_NUMPY_LIBS="${BOOST_NUMPY_LIBS} -lboost_numpy${BOOST_LIBSUFFIX}-py${BOOST_NUMPYVERSION}"
	    fi
	fi
	if test "x${boost_lib_dir_ok}" = "xno" ;
	then
	    # for jessy
	    AM_PATH_PYTHON([2.4])
	    BOOST_NUMPYVERSION=$(echo $PYTHON_VERSION |sed 's/\.//')
	    AC_CHECK_FILE(${BOOST_INSTALL_DIR}/lib/x86_64-linux-gnu/libboost_numpy${BOOST_LIBSUFFIX}-py${BOOST_NUMPYVERSION}.so,
                boost_lib_dir_ok=yes,
                boost_lib_dir_ok=no)
 	    if test "x${boost_lib_dir_ok}" = "xyes" ;
	    then
	       BOOST_NUMPY_LIBS="${BOOST_NUMPY_LIBS} -lboost_numpy${BOOST_LIBSUFFIX}-py${BOOST_NUMPYVERSION}"
	    fi
	fi
	if test "x${boost_lib_dir_ok}" = "xno" ;
	then
		AC_MSG_WARN("no suitable version of the boost numpy library found");
        tfel_numpy_support="no"
    else
		AC_MSG_NOTICE("boost numpy library found");
        tfel_numpy_support="yes"             
     	AC_SUBST([BOOST_NUMPY_LIBS])
	fi
	AC_LANG_RESTORE
])dnl
