find_program(PANDOC NAMES pandoc)
mark_as_advanced(PANDOC)
if(PANDOC)
    message(STATUS "Found Pandoc")
    # execute_process(
    #     COMMAND ${PANDOC} --version
    #     OUTPUT_VARIABLE PANDOC_VERSION_TEXT
    #     ERROR_VARIABLE PANDOC_VERSION_TEXT)
    set(HAVE_PANDOC ON)
endif(PANDOC)
