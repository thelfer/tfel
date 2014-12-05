/*!
 * \file   mfront/src/InitInterfaces.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   12 sep 2008
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include"MFront/BehaviourInterfaceProxy.hxx"
#include"MFront/MaterialPropertyInterfaceProxy.hxx"
#include"MFront/ModelInterfaceProxy.hxx"

#include"MFront/MFrontMaterialPropertyInterface.hxx"
#include"MFront/CMaterialPropertyInterface.hxx"
#include"MFront/ExcelMaterialPropertyInterface.hxx"
#include"MFront/ExcelMaterialPropertyInternalInterface.hxx"
#include"MFront/LaTeXMaterialPropertyInterface.hxx"

#ifdef HAVE_FORTRAN
#include"MFront/FortranMaterialPropertyInterface.hxx"
#endif /* HAVE_FORTRAN */

#include"MFront/CppMaterialPropertyInterface.hxx"
#include"MFront/CppTestMaterialPropertyInterface.hxx"

#ifdef HAVE_PYTHON
#include"MFront/PythonMaterialPropertyInterface.hxx"
#endif /* HAVE_PYTHON */

#ifdef HAVE_GNUPLOT
#include"MFront/GnuplotMaterialPropertyInterface.hxx"
#endif

#ifdef HAVE_CASTEM
#include"MFront/CastemMaterialPropertyInterface.hxx"
#include"MFront/UMATInterface.hxx"
#endif

#ifdef HAVE_CYRANO
#include"MFront/CyranoInterface.hxx"
#endif

#ifdef HAVE_ASTER
#include"MFront/AsterInterface.hxx"
#endif

#include"MFront/ZMATInterface.hxx"

namespace mfront
{

  void
  initInterfaces(void)
  {

    MaterialPropertyInterfaceProxy<CMaterialPropertyInterface> cLawProxy;

#ifdef HAVE_FORTRAN
    MaterialPropertyInterfaceProxy<FortranMaterialPropertyInterface> fLawProxy;
#endif /* HAVE_FORTRAN */

    MaterialPropertyInterfaceProxy<MFrontMaterialPropertyInterface>  mLawProxy;

    MaterialPropertyInterfaceProxy<ExcelMaterialPropertyInternalInterface> excelInternalLawProxy;

    MaterialPropertyInterfaceProxy<ExcelMaterialPropertyInterface> excelLawProxy(ExcelMaterialPropertyInterface::getName(),
										 ExcelMaterialPropertyInternalInterface::getName());

    constexpr const char * texLawProxyNames [2] = {"latex","LaTex"};
    MaterialPropertyInterfaceProxy<LaTeXMaterialPropertyInterface> latexLawProxy(texLawProxyNames,texLawProxyNames+2);
    
    constexpr const char * cppLawProxyNames [6] = {"c++","C++",
						   "cxx","Cxx",
						   "cpp","Cpp"};
    MaterialPropertyInterfaceProxy<CppMaterialPropertyInterface> cppLawProxy(cppLawProxyNames,cppLawProxyNames+6);

    MaterialPropertyInterfaceProxy<CppTestMaterialPropertyInterface> cppTestLawProxy(CppTestMaterialPropertyInterface::getName(),
							   CppMaterialPropertyInterface::getName());

#ifdef HAVE_GNUPLOT
    MaterialPropertyInterfaceProxy<GnuplotMaterialPropertyInterface> gnuplotLawProxy(GnuplotMaterialPropertyInterface::getName(),
							   CppTestMaterialPropertyInterface::getName());
#endif /* HAVE_GNUPLOT */

#ifdef HAVE_PYTHON
    constexpr const char * pythonProxyNames[2] = {"python","Python"};
    MaterialPropertyInterfaceProxy<PythonMaterialPropertyInterface> pythonLawProxy(pythonProxyNames,pythonProxyNames+2);
#endif /* HAVE_PYTHON */

#ifdef HAVE_CASTEM
    constexpr const char * castemProxyNames[3] = {"castem","Castem","Cast3M"};
    MaterialPropertyInterfaceProxy<CastemMaterialPropertyInterface> castemLawProxy(castemProxyNames,castemProxyNames+3);
    BehaviourInterfaceProxy<UMATInterface> umatProxy;
#endif /* HAVE_CASTEM */

#ifdef HAVE_CYRANO
    BehaviourInterfaceProxy<CyranoInterface> cyranoProxy;
#endif /* HAVE_CYRANO */

#ifdef HAVE_ASTER
    BehaviourInterfaceProxy<AsterInterface> asterProxy;
#endif /* HAVE_ASTER */

#ifdef HAVE_ZMAT
    BehaviourInterfaceProxy<ZMATInterface> zmatProxy;
#endif /* HAVE_ZMAT */

  } // end of initInterfaces(void)

} // end of namespace mfront
