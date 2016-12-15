find_program(PANDOC NAMES pandoc pandoc.exe)
find_program(PANDOC_CITEPROC NAMES pandoc-citeproc pandoc-citeproc.exe)
find_program(PANDOC_CROSSREF NAMES pandoc-crossref pandoc-crossref.exe)
mark_as_advanced(PANDOC)
mark_as_advanced(PANDOC_CITEPROC)
mark_as_advanced(PANDOC_CROSSREF)
if(PANDOC AND PANDOC_CITEPROC AND PANDOC_CROSSREF)
    message(STATUS "Found Pandoc")
    # execute_process(
    #     COMMAND ${PANDOC} --version
    #     OUTPUT_VARIABLE PANDOC_VERSION_TEXT
    #     ERROR_VARIABLE PANDOC_VERSION_TEXT)
    set(HAVE_PANDOC ON)
endif(PANDOC AND PANDOC_CITEPROC AND PANDOC_CROSSREF)
