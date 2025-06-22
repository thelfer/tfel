/*!
 * \file   mfront/src/InitInterfaces.cxx
 * \brief
 *
 * \author Thomas Helfer
 * \date   12 sep 2008
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include "MFront/BehaviourInterfaceProxy.hxx"
#include "MFront/MaterialPropertyInterfaceProxy.hxx"
#include "MFront/ModelInterfaceProxy.hxx"

#include "MFront/MFrontMaterialPropertyInterface.hxx"
#include "MFront/CMaterialPropertyInterface.hxx"
#include "MFront/ExcelMaterialPropertyInterface.hxx"
#include "MFront/ExcelMaterialPropertyInternalInterface.hxx"

#ifdef HAVE_FORTRAN
#include "MFront/FortranMaterialPropertyInterface.hxx"
#include "MFront/Fortran03MaterialPropertyInterface.hxx"
#endif /* HAVE_FORTRAN */

#include "MFront/CppMaterialPropertyInterface.hxx"
#include "MFront/CppTestMaterialPropertyInterface.hxx"
#include "MFront/OctaveMaterialPropertyInterface.hxx"

#ifdef HAVE_PYTHON
#include "MFront/PythonMaterialPropertyInterface.hxx"
#endif /* HAVE_PYTHON */

#ifdef HAVE_JAVA
#include "MFront/JavaMaterialPropertyInterface.hxx"
#endif /* HAVE_JAVA */

#ifdef HAVE_GNUPLOT
#include "MFront/GnuplotMaterialPropertyInterface.hxx"
#endif

#ifdef HAVE_CASTEM
#include "MFront/CastemMaterialPropertyInterface.hxx"
#include "MFront/CastemInterface.hxx"
#endif

#ifdef HAVE_CYRANO
#include "MFront/CyranoInterface.hxx"
#endif

#ifdef HAVE_ASTER
#include "MFront/AsterInterface.hxx"
#endif

#ifdef HAVE_ABAQUS
#include "MFront/AbaqusInterface.hxx"
#include "MFront/AbaqusExplicitInterface.hxx"
#endif

#ifdef HAVE_ANSYS
#include "MFront/AnsysInterface.hxx"
#endif

#ifdef HAVE_EUROPLEXUS
#include "MFront/EuroplexusInterface.hxx"
#endif

#ifdef HAVE_CALCULIX
#include "MFront/CalculiXInterface.hxx"
#endif

#include "MFront/ZMATInterface.hxx"
#include "MFront/MFrontModelInterface.hxx"
#include "MFront/InitInterfaces.hxx"

namespace mfront {

  void initInterfaces() {
    MaterialPropertyInterfaceProxy<CMaterialPropertyInterface> cLawProxy;

#ifdef HAVE_FORTRAN
    MaterialPropertyInterfaceProxy<FortranMaterialPropertyInterface> fLawProxy;
    MaterialPropertyInterfaceProxy<Fortran03MaterialPropertyInterface>
        f03LawProxy;
#endif /* HAVE_FORTRAN */

    MaterialPropertyInterfaceProxy<MFrontMaterialPropertyInterface> mLawProxy;

    MaterialPropertyInterfaceProxy<ExcelMaterialPropertyInternalInterface>
        excelInternalLawProxy;

    MaterialPropertyInterfaceProxy<ExcelMaterialPropertyInterface>
        excelLawProxy(ExcelMaterialPropertyInterface::getName(),
                      ExcelMaterialPropertyInternalInterface::getName());

    constexpr const char* cppLawNames[6] = {"c++", "C++", "cxx",
                                            "Cxx", "cpp", "Cpp"};
    MaterialPropertyInterfaceProxy<CppMaterialPropertyInterface> cppLawProxy(
        cppLawNames, cppLawNames + 6);

    MaterialPropertyInterfaceProxy<CppTestMaterialPropertyInterface>
        cppTestLawProxy(CppTestMaterialPropertyInterface::getName(),
                        CppMaterialPropertyInterface::getName());

#ifdef HAVE_GNUPLOT
    MaterialPropertyInterfaceProxy<GnuplotMaterialPropertyInterface>
        gnuplotLawProxy(GnuplotMaterialPropertyInterface::getName(),
                        CppTestMaterialPropertyInterface::getName());
#endif /* HAVE_GNUPLOT */

    MaterialPropertyInterfaceProxy<OctaveMaterialPropertyInterface>
        octaveLawProxy;

#ifdef HAVE_PYTHON
    constexpr const char* pythonNames[2] = {"python", "Python"};
    MaterialPropertyInterfaceProxy<PythonMaterialPropertyInterface>
        pythonLawProxy(pythonNames, pythonNames + 2);
#endif /* HAVE_PYTHON */

#ifdef HAVE_JAVA
    constexpr const char* javaNames[2] = {"java", "Java"};
    MaterialPropertyInterfaceProxy<JavaMaterialPropertyInterface> javaLawProxy(
        javaNames, javaNames + 2);
#endif /* HAVE_JAVA */

#ifdef HAVE_CASTEM
    constexpr const char* castemNames[3] = {"castem", "Castem", "Cast3M"};
    MaterialPropertyInterfaceProxy<CastemMaterialPropertyInterface>
        castemLawProxy(castemNames, castemNames + 3);
    BehaviourInterfaceProxy<CastemInterface> umatProxy(castemNames,
                                                       castemNames + 3);
#endif /* HAVE_CASTEM */

#ifdef HAVE_CYRANO
    BehaviourInterfaceProxy<CyranoInterface> cyranoProxy;
#endif /* HAVE_CYRANO */

#ifdef HAVE_ASTER
    BehaviourInterfaceProxy<AsterInterface> asterProxy;
#endif /* HAVE_ASTER */

#ifdef HAVE_ABAQUS
    BehaviourInterfaceProxy<AbaqusInterface> abaqusProxy;
    BehaviourInterfaceProxy<AbaqusExplicitInterface> abaqusExplicitProxy;
#endif /* HAVE_ABAQUS */

#ifdef HAVE_ANSYS
    BehaviourInterfaceProxy<AnsysInterface> ansysProxy;
#endif /* HAVE_ANSYS */

#ifdef HAVE_EUROPLEXUS
    BehaviourInterfaceProxy<EuroplexusInterface> europlexusProxy;
#endif /* HAVE_EUROPLEXUS */

#ifdef HAVE_CALCULIX
    constexpr const char* ccxNames[1] = {"CalculiX"};
    BehaviourInterfaceProxy<CalculiXInterface> ccxProxy(ccxNames, ccxNames + 1);
#endif /* HAVE_CALCULIX */

#ifdef HAVE_ZMAT
    BehaviourInterfaceProxy<ZMATInterface> zmatProxy;
#endif /* HAVE_ZMAT */

    ModelInterfaceProxy<MFrontModelInterface> mmi;

  }  // end of initInterfaces()

}  // end of namespace mfront
