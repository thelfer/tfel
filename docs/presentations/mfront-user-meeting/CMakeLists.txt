 add_custom_command(
   OUTPUT    mfront.html
   DEPENDS   ${CMAKE_CURRENT_SOURCE_DIR}/mfront.md
   COMMAND   ${PANDOC}
   ARGS      -s -f markdown+tex_math_single_backslash
   ARGS      --bibliography=${CMAKE_CURRENT_SOURCE_DIR}/bibliography.bib --filter pandoc-citeproc
   ARGS      --slide-level 2 --toc --mathjax -i -t revealjs -V theme:beige
   ARGS      -H ${CMAKE_CURRENT_SOURCE_DIR}/mysettings.css
   ARGS      ${CMAKE_CURRENT_SOURCE_DIR}/mfront.md -o mfront.html)
add_custom_target(mfront-user-meeting-html echo DEPENDS mfront.html)
add_dependencies(doc         mfront-user-meeting-html)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/mfront.html
    DESTINATION share/doc/tfel/web/documents/mfront-user-meeting
    COMPONENT website OPTIONAL)
install(DIRECTORY img
    DESTINATION share/doc/tfel/web/documents/mfront-user-meeting
    COMPONENT website
    PATTERN ".svn" EXCLUDE)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/docs/html/reveal.js
    DESTINATION share/doc/tfel/web/documents/mfront-user-meeting
    COMPONENT website
    PATTERN ".svn" EXCLUDE)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/mfront.html
    DESTINATION share/doc/mfront/mfront-user-meeting
    COMPONENT docs OPTIONAL)
install(DIRECTORY img
    DESTINATION share/doc/mfront/mfront-user-meeting
    COMPONENT docs
    PATTERN ".svn" EXCLUDE)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/docs/html/reveal.js
    DESTINATION share/doc/mfront/mfront-user-meeting
    COMPONENT docs
    PATTERN ".svn" EXCLUDE)


