 add_custom_command(
   OUTPUT    materiaux2014.html
   DEPENDS   ${CMAKE_CURRENT_SOURCE_DIR}/materiaux2014.md
   COMMAND   ${PANDOC}
   ARGS      -s -f markdown+tex_math_single_backslash
   ARGS      --bibliography=${CMAKE_CURRENT_SOURCE_DIR}/bibliography.bib --filter pandoc-citeproc
   ARGS      --default-image-extension=svg
   ARGS      --slide-level 2 --toc --mathjax -i -t revealjs -V theme:beige
   ARGS      -H ${CMAKE_CURRENT_SOURCE_DIR}/mysettings.css
   ARGS      -A ${CMAKE_CURRENT_SOURCE_DIR}/reveal-config.html
   ARGS      ${CMAKE_CURRENT_SOURCE_DIR}/materiaux2014.md -o materiaux2014.html)
add_custom_target(materiaux2014-html ALL DEPENDS materiaux2014.html)
add_dependencies(doc materiaux2014-html)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/materiaux2014.html
    DESTINATION share/doc/tfel/web/documents/Materiaux2014
    COMPONENT website)
install(DIRECTORY img
    DESTINATION share/doc/tfel/web/documents/Materiaux2014
    COMPONENT website
    PATTERN ".svn" EXCLUDE)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/docs/html/reveal.js
    DESTINATION share/doc/tfel/web/documents/Materiaux2014
    COMPONENT website
    PATTERN ".svn" EXCLUDE)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/materiaux2014.html
    DESTINATION share/doc/mfront/Materiaux2014
    COMPONENT docs)
install(DIRECTORY img
    DESTINATION share/doc/mfront/Materiaux2014
    COMPONENT docs
    PATTERN ".svn" EXCLUDE)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/docs/html/reveal.js
    DESTINATION share/doc/mfront/Materiaux2014
    COMPONENT docs
    PATTERN ".svn" EXCLUDE)

