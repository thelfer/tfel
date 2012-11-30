## Makefile.am -- Process this file with automake to produce a Makefile.in file.
ACLOCAL_AMFLAGS = -I m4
SUBDIRS = include src mfront tests cmake tfel-doc

doc: mfront-doc
	cd @top_srcdir@ && doxygen @abs_top_builddir@/Doxyfile && cd -

mfront-doc:
	mkdir -p docs/mfront
	cd docs/mfront ;                                                      \
	ln -s @abs_top_srcdir@/docs/tex/texmf                             . ; \
	if test "@top_srcdir@" != "@top_builddir@" ;                          \
	then                                                                  \
		ln -s @abs_top_srcdir@/docs/mfront/Images                 . ; \
		ln -s @abs_top_srcdir@/docs/mfront/mfront                 . ; \
		ln -s @abs_top_srcdir@/docs/mfront/Bibliographie.bib      . ; \
		ln -s @abs_top_srcdir@/docs/mfront/Makefile.doc           . ; \
		ln -s @abs_top_srcdir@/docs/mfront/mfront.tex             . ; \
		ln -s @abs_top_srcdir@/docs/mfront/style.ist              . ; \
		ln -s @abs_top_srcdir@/docs/mfront/Champs.tex             . ; \
		ln -s @abs_top_srcdir@/docs/mfront/annexe-orthotropie.tex . ; \
	fi ;                                                                  \
	make -f Makefile.doc

clean-local:
	cd docs/mfront ;                             \
	rm -f texmf                                ; \
	if test "@top_srcdir@" != "@top_builddir@" ; \
	then                                         \
		 rm -f Images                      ; \
		 rm -f mfront                      ; \
		 rm -f Bibliographie.bib           ; \
		 rm -f Makefile.doc                ; \
		 rm -f mfront.tex                  ; \
		 rm -f style.ist                   ; \
		 rm -f Champs.tex                  ; \
		 rm -f annexe-orthotropie.tex      ; \
	fi ;

EXTRA_DIST = bootstrap.sh        				     \
	     CMakeLists.txt      				     \
	     LGPL.txt            				     \
	     CeCILL-B.txt        				     \
	     INSTALL						     \
	     docs/tex/texmf/mise_en_page_2010.tex		     \
	     docs/tex/texmf/LSC.tex				     \
	     docs/tex/texmf/CEA_logo_quadri-sur-fond-rouge.eps	     \
	     docs/tex/texmf/note_technique_2010.cls		     \
	     docs/tex/texmf/CEA_logo_quadri-sur-fond-rouge.svg	     \
	     docs/tex/texmf/Energie2010.eps			     \
	     docs/tex/texmf/helvetCEA.sty			     \
	     docs/tex/texmf/mathematiques.sty			     \
	     docs/tex/texmf/bibtex				     \
	     docs/tex/texmf/bibtex/fr-insa.bst			     \
	     docs/tex/texmf/SESC.tex				     \
	     docs/tex/texmf/presentation.sty			     \
	     docs/tex/texmf/CEA_logo_quadri-sur-fond-rouge.pdf	     \
	     docs/tex/texmf/mecanique.sty			     \
	     docs/tex/texmf/couleurs.sty			     \
	     docs/tex/texmf/gnuplot.sty				     \
	     docs/tex/texmf/variables_2010.tex			     \
	     docs/tex/texmf/ceaeaDEC.eps			     \
	     docs/mfront/Bibliographie.bib			     \
	     docs/mfront/Images/tubeaxi.eps			     \
	     docs/mfront/Images/2norton-rk.mfront		     \
	     docs/mfront/Images/mfront.eps			     \
	     docs/mfront/Images/mfrontexcel.eps			     \
	     docs/mfront/Images/2norton-rk.mfront.tex		     \
	     docs/mfront/Images/tubeaxi2.eps			     \
	     docs/mfront/Images/NortonRK.eps			     \
	     docs/mfront/Images/Methode_Newton.pstricks		     \
	     docs/mfront/Images/2norton-rk.tex			     \
	     docs/mfront/Images/Algo_temps.eps			     \
	     docs/mfront/Images/CompSeq.eps			     \
	     docs/mfront/annexe-orthotropie.tex			     \
	     docs/mfront/style.ist				     \
	     docs/mfront/Champs.tex				     \
	     docs/mfront/mfront.tex				     \
	     docs/mfront/Makefile.doc				     \
	     docs/mfront/mfront/nbzrSwellingModel.cxx		     \
	     docs/mfront/mfront/SIC_SpecificHeat_AUDUBERT.cxx	     \
	     docs/mfront/mfront/SICCreepBehaviour-ple.tex	     \
	     docs/mfront/mfront/MaterialLaw.tex			     \
	     docs/mfront/mfront/test-T.tex			     \
	     docs/mfront/mfront/GetEvol.ps			     \
	     docs/mfront/mfront/sic.cxx				     \
	     docs/mfront/mfront/ImplicitOrthotropicCreep.mfront	     \
	     docs/mfront/mfront/loimat.tex			     \
	     docs/mfront/mfront/SICCreepBehaviour.mfront	     \
	     docs/mfront/mfront/vba.tex				     \
	     docs/mfront/mfront/SICCreepBehaviour.cxx		     \
	     docs/mfront/mfront/UPuC_SolidSwellingModel.tex	     \
	     docs/mfront/mfront/loimat.vb			     \
	     docs/mfront/mfront/SIC_Density.mfront		     \
	     docs/mfront/mfront/GetEvol.tex			     \
	     docs/mfront/mfront/SIC_YoungModulus_SNEAD.cxx	     \
	     docs/mfront/mfront/nbzrSwellingModel.mfront	     \
	     docs/mfront/mfront/sic.tex				     \
	     docs/mfront/mfront/test-T.ple			     \
	     docs/mfront/mfront/SIC_ThermalExpansion_Snead.tex	     \
	     docs/mfront/mfront/sicSwelling.tex			     \
	     docs/mfront/mfront/SIC_ThermalConductivity_Snead.mfront \
	     docs/mfront/mfront/SIC_SpecificHeat_AUDUBERT.tex	     \
	     docs/mfront/mfront/OrthotropicElastic.tex		     \
	     docs/mfront/mfront/SIC_YoungModulus_SNEAD.mfront	     \
	     docs/mfront/mfront/SIC_ThermalExpansion_Snead.mfront    \
	     docs/mfront/mfront/GetEvol.dgibi			     \
	     docs/mfront/mfront/SICCreepBehaviour.ple		     \
	     docs/mfront/mfront/OrthotropicCreep.tex		     \
	     docs/mfront/mfront/sicSwelling.cxx			     \
	     docs/mfront/mfront/nbzrSwellingModel.tex		     \
	     docs/mfront/mfront/SIC_ThermalConductivity_Snead.tex    \
	     docs/mfront/mfront/MaterialLaw.mfront		     \
	     docs/mfront/mfront/SICCreepBehaviour-ple.cxx	     \
	     docs/mfront/mfront/OrthotropicCreep.mfront		     \
	     docs/mfront/mfront/ImplicitOrthotropicCreep.tex	     \
	     docs/mfront/mfront/SICCreepBehaviour.tex		     \
	     docs/mfront/mfront/hillTensor.tex			     \
	     docs/mfront/mfront/test-T.cxx			     \
	     docs/mfront/mfront/SICCreepBehaviour-gibi2.tex	     \
	     docs/mfront/mfront/TestGetEvol.tex			     \
	     docs/mfront/mfront/SIC_YoungModulus_SNEAD.tex	     \
	     docs/mfront/mfront/SIC_Density.cxx			     \
	     docs/mfront/mfront/SIC_Density.tex			     \
	     docs/mfront/mfront/UPuC_SolidSwellingModel.mfront	     \
	     docs/mfront/mfront/GetEvol.eps			     \
	     docs/mfront/mfront/SICCreepBehaviour.dgibi		     \
	     docs/mfront/mfront/sic.mfront			     \
	     docs/mfront/mfront/SIC_ThermalExpansion_Snead.cxx	     \
	     docs/mfront/mfront/SIC_SpecificHeat_AUDUBERT.mfront     \
	     docs/mfront/mfront/SIC_ThermalConductivity_Snead.cxx    \
	     docs/mfront/mfront/SICCreepBehaviour-gibi.tex	     \
	     docs/mfront/mfront/sicSwelling.mfront		     \
	     docs/mfront/mfront/OrthotropicElastic.mfront	     \
	     docs/mfront/mfront/loimat.vba			     \
	     docs/mfront/mfront/hillTensor.cxx			     \
	     docs/mfront/mfront/highlight.sty
