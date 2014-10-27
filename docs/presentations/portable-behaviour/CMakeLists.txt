 add_custom_command(
   OUTPUT    portability.html
   DEPENDS   ${CMAKE_CURRENT_SOURCE_DIR}/portability.md
   COMMAND   ${PANDOC}
   ARGS      -s -f markdown+tex_math_single_backslash
   ARGS      --bibliography=${CMAKE_CURRENT_SOURCE_DIR}/bibliography.bib --filter pandoc-citeproc
   ARGS      --slide-level 1 --toc --mathjax -i -t revealjs -V theme:beige
   ARGS      -H ${CMAKE_CURRENT_SOURCE_DIR}/mysettings.css
   ARGS      ${CMAKE_CURRENT_SOURCE_DIR}/portability.md -o portability.html)
add_custom_target(portable-behaviour-html echo DEPENDS portability.html)

add_custom_target(portable-behaviour-html-install
  COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_INSTALL_PREFIX}/share/doc/tfel/web/documents/portable-behaviour
  COMMAND ${CMAKE_COMMAND} -E copy           ${CMAKE_CURRENT_BINARY_DIR}/portability.html
                                             ${CMAKE_INSTALL_PREFIX}/share/doc/tfel/web/documents/portable-behaviour
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/img
                                             ${CMAKE_INSTALL_PREFIX}/share/doc/tfel/web/documents/portable-behaviour/img
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/docs/html/reveal.js
                                             ${CMAKE_INSTALL_PREFIX}/share/doc/tfel/web/documents/portable-behaviour/reveal.js
  COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_INSTALL_PREFIX}/share/doc/mfront/portable-behaviour
  COMMAND ${CMAKE_COMMAND} -E copy           ${CMAKE_CURRENT_BINARY_DIR}/portability.html
                                             ${CMAKE_INSTALL_PREFIX}/share/doc/mfront/portable-behaviour/
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/img
                                             ${CMAKE_INSTALL_PREFIX}/share/doc/mfront/portable-behaviour/img
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/docs/html/reveal.js
                                             ${CMAKE_INSTALL_PREFIX}/share/doc/mfront/portable-behaviour/reveal.js)

add_dependencies(portable-behaviour-html-install portable-behaviour-html)
add_dependencies(doc         portable-behaviour-html)
add_dependencies(doc-install portable-behaviour-html-install)

