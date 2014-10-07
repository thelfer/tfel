macro(pandoc_html file)
  ADD_CUSTOM_COMMAND(
    OUTPUT    ${file}.html
    DEPENDS   ${CMAKE_CURRENT_SOURCE_DIR}/${file}.md
    DEPENDS   ${CMAKE_CURRENT_SOURCE_DIR}/mfront-template.html
    DEPENDS   ${CMAKE_CURRENT_SOURCE_DIR}/css/main.css
    COMMAND   ${PANDOC}
    ARGS      -f markdown-markdown_in_html_blocks+tex_math_single_backslash
    ARGS      --bibliography=${CMAKE_CURRENT_SOURCE_DIR}/bibliography.bib --filter pandoc-citeproc
    ARGS      --template=${CMAKE_CURRENT_SOURCE_DIR}/mfront-template.html
    ARGS      --mathjax
    ARGS      --email-obfuscation=javascript
    ARGS      ${ARGN}
    ARGS      ${CMAKE_CURRENT_SOURCE_DIR}/${file}.md -o ${file}.html)
  add_custom_target(${file}-html echo DEPENDS ${file}.html)
  add_custom_target(${file}-html-install
    COMMAND ${CMAKE_COMMAND} -E copy ${file}.html ${CMAKE_INSTALL_PREFIX}/share/doc/web/${file}.html)
  add_dependencies(${file}-html-install ${file}-html)
  add_dependencies(website-install ${file}-html-install)
endmacro(pandoc_html)

add_custom_target(website-install
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/js        ${CMAKE_INSTALL_PREFIX}/share/doc/web/js
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/css       ${CMAKE_INSTALL_PREFIX}/share/doc/web/css
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/doc       ${CMAKE_INSTALL_PREFIX}/share/doc/web/doc
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/img       ${CMAKE_INSTALL_PREFIX}/share/doc/web/img
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/404.html ${CMAKE_INSTALL_PREFIX}/share/doc/web
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/apple-touch-icon-precomposed.png ${CMAKE_INSTALL_PREFIX}/share/doc/web
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/CHANGELOG.md ${CMAKE_INSTALL_PREFIX}/share/doc/web
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/CONTRIBUTING.md ${CMAKE_INSTALL_PREFIX}/share/doc/web
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/crossdomain.xml ${CMAKE_INSTALL_PREFIX}/share/doc/web
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/favicon.ico ${CMAKE_INSTALL_PREFIX}/share/doc/web
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/humans.txt ${CMAKE_INSTALL_PREFIX}/share/doc/web
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.md ${CMAKE_INSTALL_PREFIX}/share/doc/web
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/README.md ${CMAKE_INSTALL_PREFIX}/share/doc/web
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/robots.txt ${CMAKE_INSTALL_PREFIX}/share/doc/web
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/TODO.md ${CMAKE_INSTALL_PREFIX}/share/doc/web)
add_dependencies(doc-install website-install )

pandoc_html(index "-c css/slideshow.css")
pandoc_html(install)
pandoc_html(about)
pandoc_html(svn)
pandoc_html(mtest)
pandoc_html(tutorial)
pandoc_html(benchmark)
pandoc_html(news)
pandoc_html(castem)
pandoc_html(documentations)
pandoc_html(glossary)
pandoc_html(material-properties)
pandoc_html(models)
