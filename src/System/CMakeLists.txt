set(TFELSystem_SOURCES
  RecursiveFind.cxx
  LibraryInformation.cxx
  ExternalLibraryManager.cxx
  ExternalMaterialKnowledgeDescription.cxx
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
target_include_directories(TFELSystem
   PUBLIC 
   $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
   $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/mfront/include>
   $<INSTALL_INTERFACE:include>
   PRIVATE
   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
# type of architecture
if( CMAKE_SIZEOF_VOID_P EQUAL 8 )
  target_compile_definitions(TFELSystem PUBLIC "TFEL_ARCH64")
else( CMAKE_SIZEOF_VOID_P EQUAL 8 )
  target_compile_definitions(TFELSystem PUBLIC "TFEL_ARCH32")
endif( CMAKE_SIZEOF_VOID_P EQUAL 8 )
if(TFEL_DL_LIBRARY)
  target_link_libraries(TFELSystem 
	PUBLIC tfel::TFELException
    PRIVATE ${TFEL_DL_LIBRARY}
    PRIVATE ${CMAKE_THREAD_LIBS_INIT})
else(TFEL_DL_LIBRARY)
  if((NOT i586-mingw32msvc_COMPILER) AND (NOT i686-w64-mingw32_COMPILER))
    if(Threads_FOUND)
      target_link_libraries(TFELSystem 
		PUBLIC tfel::TFELException
		PRIVATE Threads::Threads)
    else(Threads_FOUND)
      target_link_libraries(TFELSystem
        PUBLIC tfel::TFELException)
    endif(Threads_FOUND)
  else((NOT i586-mingw32msvc_COMPILER) AND (NOT i686-w64-mingw32_COMPILER))
    target_link_libraries(TFELSystem
      PUBLIC tfel::TFELException)
  endif((NOT i586-mingw32msvc_COMPILER) AND (NOT i686-w64-mingw32_COMPILER))
endif(TFEL_DL_LIBRARY)
if(enable-static)
  target_include_directories(TFELSystem-static
    PUBLIC 
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/mfront/include>
    $<INSTALL_INTERFACE:include>
    PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
  # type of architecture
  if( CMAKE_SIZEOF_VOID_P EQUAL 8 )
    target_compile_definitions(TFELSystem-static PUBLIC "TFEL_ARCH64")
  else( CMAKE_SIZEOF_VOID_P EQUAL 8 )
    target_compile_definitions(TFELSystem-static PUBLIC "TFEL_ARCH32")
  endif( CMAKE_SIZEOF_VOID_P EQUAL 8 )
  if(TFEL_DL_LIBRARY)
    target_link_libraries(TFELSystem-static 
    PUBLIC tfel::TFELException-static
      PRIVATE ${TFEL_DL_LIBRARY}
      PRIVATE ${CMAKE_THREAD_LIBS_INIT})
  else(TFEL_DL_LIBRARY)
    if((NOT i586-mingw32msvc_COMPILER) AND (NOT i686-w64-mingw32_COMPILER))
      if(Threads_FOUND)
        target_link_libraries(TFELSystem-static 
          PUBLIC tfel::TFELException-static
          PRIVATE Threads::Threads)
      else(Threads_FOUND)
        target_link_libraries(TFELSystem-static
      PUBLIC tfel::TFELException-static)
      endif(Threads_FOUND)
    else((NOT i586-mingw32msvc_COMPILER) AND (NOT i686-w64-mingw32_COMPILER))
      target_link_libraries(TFELSystem-static
        PUBLIC TFELException-static)
    endif((NOT i586-mingw32msvc_COMPILER) AND (NOT i686-w64-mingw32_COMPILER))
  endif(TFEL_DL_LIBRARY)
endif(enable-static)
