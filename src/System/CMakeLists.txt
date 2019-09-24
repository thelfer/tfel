include_directories("${PROJECT_SOURCE_DIR}/include"
  "${CMAKE_CURRENT_SOURCE_DIR}")
link_directories("${PROJECT_BINARY_DIR}/src/Exception")

set(TFELSystem_SOURCES
  RecursiveFind.cxx
  LibraryInformation.cxx
  ExternalLibraryManager.cxx
  ExternalMaterialPropertyDescription.cxx
  ExternalBehaviourDescription.cxx
  System.cxx
  SystemError.cxx)

if((NOT i586-mingw32msvc_COMPILER) AND (NOT i686-w64-mingw32_COMPILER))
  set(TFELSystem_SOURCES
    ThreadPool.cxx
    ThreadedTaskResult.cxx
    ${TFELSystem_SOURCES})
endif((NOT i586-mingw32msvc_COMPILER) AND (NOT i686-w64-mingw32_COMPILER))

if(UNIX)
  set(TFELSystem_SOURCES
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
    ${TFELSystem_SOURCES})
  if(CYGWIN)
    set(TFELSystem_SOURCES
      getFunction-win.cxx
      ${TFELSystem_SOURCES})
  else(CYGWIN)
    set(TFELSystem_SOURCES
      getFunction.c
      ${TFELSystem_SOURCES})
  endif(CYGWIN)    
else(UNIX)
  set(TFELSystem_SOURCES
    getFunction-win.cxx
    ${TFELSystem_SOURCES})
endif(UNIX)

tfel_library(TFELSystem ${TFELSystem_SOURCES})
if(TFEL_DL_LIBRARY)
  target_link_libraries(TFELSystem TFELException ${TFEL_DL_LIBRARY} ${CMAKE_THREAD_LIBS_INIT})
else(TFEL_DL_LIBRARY)
  if((NOT i586-mingw32msvc_COMPILER) AND (NOT i686-w64-mingw32_COMPILER))
    if(Threads_FOUND)
      target_link_libraries(TFELSystem TFELException Threads::Threads)
    else(Threads_FOUND)
      target_link_libraries(TFELSystem TFELException)
    endif(Threads_FOUND)
  else((NOT i586-mingw32msvc_COMPILER) AND (NOT i686-w64-mingw32_COMPILER))
    target_link_libraries(TFELSystem TFELException)
  endif((NOT i586-mingw32msvc_COMPILER) AND (NOT i686-w64-mingw32_COMPILER))
endif(TFEL_DL_LIBRARY)
