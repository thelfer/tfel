macro(pandoc_html file)
  ADD_CUSTOM_COMMAND(
    OUTPUT    ${CMAKE_CURRENT_BINARY_DIR}/${file}.html
    DEPENDS   ${CMAKE_CURRENT_SOURCE_DIR}/${file}.md
    DEPENDS   ${CMAKE_CURRENT_SOURCE_DIR}/mfront-template.html
    DEPENDS   ${CMAKE_CURRENT_SOURCE_DIR}/css/main.css
    COMMAND   ${PANDOC}
    ARGS      -f markdown-markdown_in_html_blocks+tex_math_single_backslash
    ARGS      --filter ${PANDOC_REFERENCE_FILTER} -M crossrefYaml=${CMAKE_CURRENT_SOURCE_DIR}/pandoc-crossref.yaml
    ARGS      --bibliography=${CMAKE_CURRENT_SOURCE_DIR}/bibliography.bib --citeproc
    ARGS      --template=${CMAKE_CURRENT_SOURCE_DIR}/mfront-template.html
    ARGS      --mathjax
    ARGS      --email-obfuscation=javascript
    ARGS       --default-image-extension=svg
    ARGS      ${ARGN}
    ARGS      ${CMAKE_CURRENT_SOURCE_DIR}/${file}.md -o ${file}.html)
  add_custom_target(${file}-html ALL DEPENDS ${file}.html)
  add_dependencies(website ${file}-html)
  if(TFEL_APPEND_VERSION)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${file}.html
      DESTINATION share/doc/tfel-${TFEL_VERSION}/web
      COMPONENT website)
  else(TFEL_APPEND_VERSION)
  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${file}.html
    DESTINATION share/doc/tfel/web
    COMPONENT website)
  endif(TFEL_APPEND_VERSION)
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
pandoc_html(mtest-python  "--toc" "--toc-depth=3")
pandoc_html(tutorial)
pandoc_html(benchmark)
pandoc_html(news "-c css/slideshow.css")
pandoc_html(castem)
pandoc_html(castem-fr)
pandoc_html(aster)
pandoc_html(abaqus "-c css/slideshow.css")
pandoc_html(zmat)
pandoc_html(epx "-c css/slideshow.css")
pandoc_html(documentations)
pandoc_html(gallery  "--toc")
pandoc_html(glossary)
pandoc_html(material-properties)
pandoc_html(behaviours)
pandoc_html(models)
pandoc_html(parameters)
pandoc_html(publications)
pandoc_html(release-notes "--toc" "--toc-depth=1")
pandoc_html(mfront-doc)
pandoc_html(mfront-python "--toc" "--toc-depth=3")
pandoc_html(Norton-web)
pandoc_html(Norton-full)
pandoc_html(coverity-scan)

# mfront keywords

pandoc_html(DefaultDSL-keywords "--toc" "--toc-depth=1")
pandoc_html(DefaultFiniteStrainDSL-keywords "--toc" "--toc-depth=1")
pandoc_html(DefaultCZMDSL-keywords "--toc" "--toc-depth=1")
pandoc_html(Implicit-keywords "--toc" "--toc-depth=1")
pandoc_html(ImplicitFiniteStrain-keywords "--toc" "--toc-depth=1")
pandoc_html(ImplicitII-keywords "--toc" "--toc-depth=1")
pandoc_html(IsotropicMisesCreep-keywords "--toc" "--toc-depth=1")
pandoc_html(IsotropicPlasticMisesFlow-keywords "--toc" "--toc-depth=1")
pandoc_html(IsotropicStrainHardeningMisesCreep-keywords "--toc" "--toc-depth=1")
pandoc_html(MaterialLaw-keywords "--toc" "--toc-depth=1")
pandoc_html(Model-keywords "--toc" "--toc-depth=1")
pandoc_html(MultipleIsotropicMisesFlows-keywords "--toc" "--toc-depth=1")
pandoc_html(RungeKutta-keywords "--toc" "--toc-depth=1")
pandoc_html(BehaviourBricks "--toc" "--toc-depth=2")
pandoc_html(release-notes-3.0 "--toc" "--toc-depth=2")
pandoc_html(release-notes-3.0.1 "--toc" "--toc-depth=2")

# mtest keywords

pandoc_html(MTest-keywords "--toc" "--toc-depth=1")

if(TFEL_APPEND_VERSION)
  install(DIRECTORY js css doc img media downloads
    DESTINATION share/doc/tfel-${TFEL_VERSION}/web
    COMPONENT website
    PATTERN ".svn" EXCLUDE)
  install(FILES 404.html apple-touch-icon-precomposed.png 
    CHANGELOG.md CONTRIBUTING.md crossdomain.xml favicon.ico
    humans.txt LICENSE.md README.md robots.txt TODO.md
    DESTINATION share/doc/tfel-${TFEL_VERSION}/web
    COMPONENT website)
else(TFEL_APPEND_VERSION)
install(DIRECTORY js css doc img media downloads
    DESTINATION share/doc/tfel/web
    COMPONENT website
    PATTERN ".svn" EXCLUDE)
install(FILES 404.html apple-touch-icon-precomposed.png 
  CHANGELOG.md CONTRIBUTING.md crossdomain.xml favicon.ico
  humans.txt LICENSE.md README.md robots.txt TODO.md
  DESTINATION share/doc/tfel/web
  COMPONENT website)
endif(TFEL_APPEND_VERSION)
