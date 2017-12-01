 add_custom_command(
   OUTPUT    portability.html
   DEPENDS   ${CMAKE_CURRENT_SOURCE_DIR}/portability.md
   COMMAND   ${PANDOC}
   ARGS      -s -f markdown+tex_math_single_backslash
   ARGS      --bibliography=${CMAKE_CURRENT_SOURCE_DIR}/bibliography.bib --filter pandoc-citeproc
   ARGS      --slide-level 1 --toc --mathjax -i -t revealjs -V theme:beige
   ARGS      -H ${CMAKE_CURRENT_SOURCE_DIR}/mysettings.css
   ARGS      ${CMAKE_CURRENT_SOURCE_DIR}/portability.md -o portability.html)
add_custom_target(portable-behaviour-html ALL DEPENDS portability.html)
add_dependencies(doc portable-behaviour-html)

if(TFEL_APPEND_SUFFIX)
  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/portability.html
    DESTINATION share/doc/tfel-${TFEL_SUFFIX}/web/documents/portable-behaviour
    COMPONENT website)
  install(DIRECTORY img
    DESTINATION share/doc/tfel-${TFEL_SUFFIX}/web/documents/portable-behaviour
    COMPONENT website
    PATTERN ".svn" EXCLUDE)
  install(DIRECTORY ${CMAKE_SOURCE_DIR}/docs/html/reveal.js
    DESTINATION share/doc/tfel-${TFEL_SUFFIX}/web/documents/portable-behaviour
    COMPONENT website
    PATTERN ".svn" EXCLUDE)
  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/portability.html
    DESTINATION share/doc/mfront-${TFEL_SUFFIX}/portable-behaviour
    COMPONENT docs)
  install(DIRECTORY img
    DESTINATION share/doc/mfront-${TFEL_SUFFIX}/portable-behaviour
    COMPONENT docs
    PATTERN ".svn" EXCLUDE)
  install(DIRECTORY ${CMAKE_SOURCE_DIR}/docs/html/reveal.js
    DESTINATION share/doc/mfront-${TFEL_SUFFIX}/portable-behaviour
    COMPONENT docs
    PATTERN ".svn" EXCLUDE)
else(TFEL_APPEND_SUFFIX)
  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/portability.html
    DESTINATION share/doc/tfel/web/documents/portable-behaviour
    COMPONENT website)
  install(DIRECTORY img
    DESTINATION share/doc/tfel/web/documents/portable-behaviour
    COMPONENT website
    PATTERN ".svn" EXCLUDE)
  install(DIRECTORY ${CMAKE_SOURCE_DIR}/docs/html/reveal.js
    DESTINATION share/doc/tfel/web/documents/portable-behaviour
    COMPONENT website
    PATTERN ".svn" EXCLUDE)
  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/portability.html
    DESTINATION share/doc/mfront/portable-behaviour
    COMPONENT docs)
  install(DIRECTORY img
    DESTINATION share/doc/mfront/portable-behaviour
    COMPONENT docs
    PATTERN ".svn" EXCLUDE)
  install(DIRECTORY ${CMAKE_SOURCE_DIR}/docs/html/reveal.js
    DESTINATION share/doc/mfront/portable-behaviour
    COMPONENT docs
    PATTERN ".svn" EXCLUDE)
endif(TFEL_APPEND_SUFFIX)

