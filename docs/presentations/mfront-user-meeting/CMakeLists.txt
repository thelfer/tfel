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

add_custom_target(mfront-user-meeting-html-install
  COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_INSTALL_PREFIX}/share/doc/web/documents/mfront-user-meeting
  COMMAND ${CMAKE_COMMAND} -E copy           ${CMAKE_CURRENT_BINARY_DIR}/mfront.html
                                             ${CMAKE_INSTALL_PREFIX}/share/doc/web/documents/mfront-user-meeting
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/img
                                             ${CMAKE_INSTALL_PREFIX}/share/doc/web/documents/mfront-user-meeting/img
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/docs/html/reveal.js
                                             ${CMAKE_INSTALL_PREFIX}/share/doc/web/documents/mfront-user-meeting/reveal.js
  COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_INSTALL_PREFIX}/share/doc/mfront-user-meeting
  COMMAND ${CMAKE_COMMAND} -E copy           ${CMAKE_CURRENT_BINARY_DIR}/mfront.html
                                             ${CMAKE_INSTALL_PREFIX}/share/doc/mfront-user-meeting/
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/img
                                             ${CMAKE_INSTALL_PREFIX}/share/doc/mfront-user-meeting/img
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/docs/html/reveal.js
                                             ${CMAKE_INSTALL_PREFIX}/share/doc/mfront-user-meeting/reveal.js)
add_dependencies(mfront-user-meeting-html-install mfront-user-meeting-html)
add_dependencies(doc         mfront-user-meeting-html)
add_dependencies(doc-install mfront-user-meeting-html-install)
