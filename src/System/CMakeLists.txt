include_directories("${PROJECT_SOURCE_DIR}/include")
link_directories("${PROJECT_BINARY_DIR}/src/Exception")

set(TFELSystem_SOURCES
    ExternalLibraryManager.cxx
    RecursiveFind.cxx
    ProcessManager-c.c
    ProcessManager.cxx
    ChildProcess.cxx
    SignalHandler.cxx
    SignalManager.cxx
    System.cxx
    SystemError.cxx
    basic_rstream.cxx
    basic_wstream.cxx
    rfstream.cxx
    wfstream.cxx
    getFunction.c)

tfel_library(TFELSystem ${TFELSystem_SOURCES})
target_link_libraries(TFELSystem TFELException dl)
