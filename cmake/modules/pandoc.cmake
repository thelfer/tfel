find_program(PANDOC NAMES pandoc pandoc.exe)
find_program(PANDOC_CITEPROC NAMES pandoc-citeproc pandoc-citeprocx.exe)
mark_as_advanced(PANDOC)
mark_as_advanced(PANDOC_CITEPROC)
if(PANDOC AND PANDOC_CITEPROC)
    message(STATUS "Found Pandoc")
    # execute_process(
    #     COMMAND ${PANDOC} --version
    #     OUTPUT_VARIABLE PANDOC_VERSION_TEXT
    #     ERROR_VARIABLE PANDOC_VERSION_TEXT)
    set(HAVE_PANDOC ON)
endif(PANDOC AND PANDOC_CITEPROC)
