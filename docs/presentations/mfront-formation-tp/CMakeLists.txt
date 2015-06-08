configure_file(Chaboche.tex.in Chaboche.tex @ONLY)
configure_file(lemaitre.tex.in lemaitre.tex @ONLY)
configure_file(norton.tex.in norton.tex @ONLY)

if(HAVE_LATEX)
  pdflatex_presentation(tp)
endif(HAVE_LATEX)
