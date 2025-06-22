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
#include "MFront/GenericBehaviourInterface.hxx"
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
#include "MFront/CyranoMaterialPropertyInterface.hxx"
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

#ifdef HAVE_COMSOL
#include "MFront/ComsolInterface.hxx"
#endif

#ifdef HAVE_DIANAFEA
#include "MFront/DianaFEAInterface.hxx"
#endif

#ifdef HAVE_LSDYNA
#include "MFront/LSDYNAInterface.hxx"
#endif /* HAVE_LSDYNA */

#ifdef HAVE_ZMAT
#include "MFront/ZMATInterface.hxx"
#endif /* HAVE_ZMAT */

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
    constexpr const char* castemINames[3] = {"castem", "Castem", "Cast3M"};
    MaterialPropertyInterfaceProxy<CastemMaterialPropertyInterface>
        castemLawProxy(castemINames, castemINames + 3);
    constexpr const char* castemBNames[3] = {"umat", "Castem", "Cast3M"};
    BehaviourInterfaceProxy<CastemInterface> umatProxy(castemBNames,
                                                       castemBNames + 3);
#endif /* HAVE_CASTEM */

#ifdef HAVE_CYRANO
    constexpr const char* cyranoINames[3] = {"cyrano", "Cyrano"};
    MaterialPropertyInterfaceProxy<CyranoMaterialPropertyInterface>
        cyranoLawProxy(cyranoINames, cyranoINames + 2);
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

#ifdef HAVE_DIANAFEA
    BehaviourInterfaceProxy<DianaFEAInterface> dianaFEAProxy;
#endif /* HAVE_DIANAFEA */

#ifdef HAVE_CALCULIX
    BehaviourInterfaceProxy<CalculiXInterface> ccxProxy;
#endif /* HAVE_CALCULIX */

#ifdef HAVE_COMSOL
    BehaviourInterfaceProxy<ComsolInterface> comsolProxy;
#endif /* HAVE_COMSOL */

#ifdef HAVE_ZMAT
    BehaviourInterfaceProxy<ZMATInterface> zmatProxy;
#endif /* HAVE_ZMAT */

#ifdef HAVE_LSDYNA
    BehaviourInterfaceProxy<LSDYNAInterface> lsdynaProxy;
#endif /* HAVE_LSDYNA */

    BehaviourInterfaceProxy<GenericBehaviourInterface> gbProxy;

    ModelInterfaceProxy<MFrontModelInterface> mmi;

  }  // end of initInterfaces()

}  // end of namespace mfront
