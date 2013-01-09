message(STATUS "looked for castem library.")

set(CASTEMHOME $ENV{CASTEMHOME})
if(CASTEMHOME)
    find_path(CASTEM_INCLUDE_DIR castem.h
              HINTS ${CASTEMHOME}/include)
    if(CASTEM_INCLUDE_DIR STREQUAL "CASTEM_INCLUDE_DIR-NOTFOUND")
    	message(FATAL_ERROR "castem.h not found")
    endif(CASTEM_INCLUDE_DIR STREQUAL "CASTEM_INCLUDE_DIR-NOTFOUND")
    message(STATUS "Include file detected: [${CASTEM_INCLUDE_DIR}].")
    add_definitions("-DCASTEM_CPPFLAGS=\\\"\"-DSUN -DUNIX64 -DTHREAD\"\\\"")
    add_definitions("-DSUN -DUNIX64 -DTHREAD")
    add_definitions("-DCASTEM_UNIX_TYPE=UNIX64")
    add_definitions("-DCASTEM_ROOT=\\\"\"${CASTEMHOME}\"\\\"")
    set(HAVE_CASTEM ON)
else(CASTEMHOME)
  message(WARNING "no CASTEMHOME environment defined")
endif(CASTEMHOME)

