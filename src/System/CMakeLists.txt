include_directories("${PROJECT_SOURCE_DIR}/include")
link_directories("${PROJECT_BINARY_DIR}/src/Exception")

set(TFELSystem_SOURCES
    ExternalLibraryManager.cxx
    ExternalBehaviourDescription.cxx
    System.cxx
    SystemError.cxx)

if(UNIX)
set(TFELSystem_SOURCES
    RecursiveFind.cxx
    ProcessManager-c.c
    ProcessManager.cxx
    ChildProcess.cxx
    SignalHandler.cxx
    SignalManager.cxx
    basic_rstream.cxx
    basic_wstream.cxx
    rfstream.cxx
    wfstream.cxx
    BinaryRead.cxx
    BinaryWrite.cxx
    getFunction.c
    ${TFELSystem_SOURCES})
else(UNIX)
set(TFELSystem_SOURCES
    getFunction-win.cxx
    ${TFELSystem_SOURCES})
endif(UNIX)

tfel_library(TFELSystem ${TFELSystem_SOURCES})
target_link_libraries(TFELSystem TFELException @TFEL_DL_LIBRARY@)
