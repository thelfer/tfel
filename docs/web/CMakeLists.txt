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
    ARGS       --default-image-extension=svg
    ARGS      ${ARGN}
    ARGS      ${CMAKE_CURRENT_SOURCE_DIR}/${file}.md -o ${file}.html)
  add_custom_target(${file}-html ALL DEPENDS ${file}.html)
  add_dependencies(website ${file}-html)
  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${file}.html
    DESTINATION share/doc/tfel/web
    COMPONENT website)
endmacro(pandoc_html)

add_custom_target(website)
add_dependencies(doc website)

pandoc_html(index "-c css/slideshow.css")
pandoc_html(faq "--toc")
pandoc_html(interfaces)
pandoc_html(install)
pandoc_html(install-windows)
pandoc_html(packages)
pandoc_html(about)
pandoc_html(svn)
pandoc_html(devel)
pandoc_html(mtest)
pandoc_html(tutorial)
pandoc_html(benchmark)
pandoc_html(news)
pandoc_html(castem)
pandoc_html(castem-fr)
pandoc_html(aster)
pandoc_html(zmat)
pandoc_html(documentations)
pandoc_html(glossary)
pandoc_html(material-properties)
pandoc_html(behaviours)
pandoc_html(models)
pandoc_html(parameters)

install(DIRECTORY js css doc img downloads
    DESTINATION share/doc/tfel/web
    COMPONENT website
    PATTERN ".svn" EXCLUDE)
install(FILES 404.html apple-touch-icon-precomposed.png 
  CHANGELOG.md CONTRIBUTING.md crossdomain.xml favicon.ico
  humans.txt LICENSE.md README.md robots.txt TODO.md
  DESTINATION share/doc/tfel/web
  COMPONENT website)

