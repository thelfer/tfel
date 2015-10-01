macro(tfel_install_article article)
  install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/${article}
    DESTINATION share/doc/tfel/articles
    COMPONENT docs)
  install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/${article}
    DESTINATION share/doc/tfel/web/documents/articles/
    COMPONENT website)
endmacro(tfel_install_article article)

tfel_install_article(topfuel2015.pdf)
tfel_install_article(topfuel2015-poster.pdf)