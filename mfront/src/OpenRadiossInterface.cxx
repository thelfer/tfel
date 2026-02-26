/*!
 * \file   mfront/src/OpenRadiossInterface.cxx
 * \brief
 * \author Thomas Helfer
 * \date   10/04/2025
 */

#include <iostream>

#include <fstream>
#include "TFEL/Raise.hxx"
#include "TFEL/Glossary/Glossary.hxx"
#include "TFEL/Glossary/GlossaryEntry.hxx"
#include "MFront/TargetsDescription.hxx"
#include "MFront/LibraryDescription.hxx"
#include "MFront/MFrontUtilities.hxx"
#include "MFront/BehaviourDescription.hxx"
#include "MFront/OpenRadiossInterface.hxx"

namespace mfront {

  std::string OpenRadiossInterface::getName() { return "OpenRadioss"; }

  std::string OpenRadiossInterface::getInterfaceName() const {
    return "OpenRadioss";
  }

  std::string OpenRadiossInterface::getLibraryName(
      const BehaviourDescription& bd) const {
    if (bd.getLibrary().empty()) {
      if (!bd.getMaterialName().empty()) {
        return bd.getMaterialName() + "-" + this->getInterfaceName();
      } else {
        return "OpenRadiossBehaviour";
      }
    }
    return bd.getLibrary() + "-" + this->getInterfaceName();
  }  // end of getLibraryName

  void OpenRadiossInterface::getTargetsDescription(
      TargetsDescription& td, const BehaviourDescription& bd) {
    GenericBehaviourInterface::getTargetsDescription(td, bd);
    const auto lib = this->getLibraryName(bd);
    auto& l = td.getLibrary(lib);
    insert_if(l.sources, "openradioss-lecmuser01.f");
    insert_if(l.sources, "openradioss-luser01.f");
    insert_if(l.sources, "openradioss-luser01-interface.cxx");
  }  // end of getTargetsDescription

  std::pair<bool, OpenRadiossInterface::tokens_iterator>
  OpenRadiossInterface::treatKeyword(BehaviourDescription&,
                                     const std::string& k,
                                     const std::vector<std::string>& i,
                                     tokens_iterator current,
                                     const tokens_iterator) {
    if (!i.empty()) {
      if (std::find(i.begin(), i.end(), this->getName()) != i.end()) {
        tfel::raise("unsupported key '" + k + "'");
      }
    }
    return {false, current};
  }  // end of OpenRadiossInterface::treatKeyword

  std::set<OpenRadiossInterface::Hypothesis>
  OpenRadiossInterface::getModellingHypothesesToBeTreated(
      const BehaviourDescription& bd) const {
    const auto& bh = bd.getModellingHypotheses();
    tfel::raise_if(bh.find(ModellingHypothesis::TRIDIMENSIONAL) == bh.end(),
                   "OpenRadiossInterface::getModellingHypothesesToBeTreated : "
                   "the 'Tridimensional' hypothesis is not supported, "
                   "which is required for the OpenRadioss interface");
    return {ModellingHypothesis::TRIDIMENSIONAL};
  }  // end of getModellingHypothesesToBeTreated

  void OpenRadiossInterface::endTreatment(const BehaviourDescription& bd,
                                          const FileDescription& fd) const {
    constexpr auto h = ModellingHypothesis::TRIDIMENSIONAL;
    const auto& d = bd.getBehaviourData(h);
    //
    if (!bd.isTemperatureDefinedAsTheFirstExternalStateVariable()) {
      tfel::raise(
          "the temperature must be defined as the first external state "
          "variable to be compatible with the OpenRadioss's interface");
    }
    if(d.getExternalStateVariables().size() != 1u){
      tfel::raise(
          "external state variables other than the temperature are not "
          "supported by OpenRadioss's interface");
    }
    if (bd.getSymmetryType() != mfront::ISOTROPIC) {
      tfel::raise(
          "only isotropic behaviours are "
          "supported by OpenRadioss's interface");
    }
    if (bd.getBehaviourType() !=
        BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) {
      tfel::raise(
          "only small strain behaviours are "
          "supported by OpenRadioss's interface");
    }
    //
    GenericBehaviourInterface::endTreatment(bd, fd);
    // generating the starter sources
    const auto mprops = this->buildMaterialPropertiesList(bd, h);
    const auto nprops = [&mprops] {
      auto r = SupportedTypes::TypeSize{};
      if (!mprops.first.empty()) {
        const auto& m = mprops.first.back();
        r = m.offset;
        r += SupportedTypes::getTypeSize(m.type, m.arraySize);
        r -= mprops.second;
      }
      return r.getValueForModellingHypothesis(h);
    }();
    auto out = std::ofstream("src/openradioss-lecmuser01.f");
    if (!out) {
      tfel::raise("could not open file 'src/openradioss-lecmuser01.f'");
    }
    out << "      SUBROUTINE "
        << "LECMUSER01(IIN,IOUT,UPARAM,MAXUPARAM,NUPARAM, \n"
        << "     .                NUVAR,IFUNC,MAXFUNC,NFUNC,STIFINT,\n"
        << "     .                USERBUF)\n"
        << "      USE LAW_USER\n"
        << "      IMPLICIT NONE\n"
        << "      INTEGER IIN,IOUT,MAXUPARAM,NUPARAM,NUVAR,\n"
        << "     .        MAXFUNC,NFUNC,IFUNC(MAXFUNC)\n"
        << "      INTEGER DUMMY_INTEGER\n"
        << "      DOUBLE PRECISION   UPARAM(MAXUPARAM),STIFINT\n"
        << "      TYPE(ULAWBUF) :: USERBUF\n"
        << "      NUPARAM = " << nprops << "\n"
        << "      IF(NUPARAM.GT.MAXUPARAM)THEN\n"
        << "        WRITE(IOUT,*)' ** ERROR : NUPARAM GT MAXUPARAM'\n"
        << "        WRITE(IOUT,*)'      NUPARAM =',NUPARAM,\n"
        << "     .               ' MAXUPARAM =',MAXUPARAM\n"
        << "        RETURN\n"
        << "      ENDIF\n";
    if (nprops != 0) {
      out << "C\n"
          << "C-----------------------------------------------\n"
          << "C     INPUT FILE READING (USER DATA)\n"
          << "C-----------------------------------------------\n"
          << "      \n"
          << "      READ(IIN,'(" << nprops << "F20.0)') ";
      for (int i = 1; i != nprops + 1; ++i) {
        out << "UPARAM(" << i << ")";
        if (i != nprops) {
          out << ", ";
          if (i % 4 == 0) {
            out << '\n'  //
                << "     &";
          }
        }
      }
      out << "\n";
    }
    out << "C-------------------------------------------------\n"
        << "C     NUMBER OF USER ELEMENT VARIABLES AND CURVES\n"
        << "C-------------------------------------------------\n";
    const auto nstatev = mfront::getTypeSize(d.getPersistentVariables())
                             .getValueForModellingHypothesis(h);
    out << "      NUVAR = " << nstatev << '\n';
    out << "C-------------------------------------------------\n"
        << "C     INITIAL STIFFNESS                           \n"
        << "C-------------------------------------------------\n";
    auto found = false;
    if (bd.getSymmetryType() == mfront::ISOTROPIC) {
      for (const auto& mp : mprops.first) {
        if (mp.getExternalName() == tfel::glossary::Glossary::ShearModulus) {
          const auto offset = [&mp, &mprops] {
            auto r = mp.offset;
            r -= mprops.second;
            return r.getValueForModellingHypothesis(h);
          }();
          out << "      STIFINT = UMAT(" << offset + 1 << ")\n";
          found = true;
          break;
        }
      }
      if (!found) {
        for (const auto& mp : mprops.first) {
          if (mp.getExternalName() == tfel::glossary::Glossary::YoungModulus) {
            const auto offset = [&mp, &mprops] {
              auto r = mp.offset;
              r -= mprops.second;
              return r.getValueForModellingHypothesis(h);
            }();
            out << "      STIFINT = UMAT(" << offset + 1 << ")\n";
            found = true;
            break;
          }
        }
      }
      if (!found) {
        const auto& parameters = d.getParameters();
        const auto pp1 = findByExternalName(
            parameters, tfel::glossary::Glossary::ShearModulus);
        if (pp1 != parameters.end()) {
          out << "      STIFINT = "
              << d.getFloattingPointParameterDefaultValue(
                     tfel::glossary::Glossary::ShearModulus)
              << "\n";
          found = true;
        } else {
          const auto pp2 = findByExternalName(
              parameters, tfel::glossary::Glossary::YoungModulus);
          if (pp2 != parameters.end()) {
            out << "      STIFINT = "
                << d.getFloattingPointParameterDefaultValue(
                       tfel::glossary::Glossary::YoungModulus)
                << "\n";
            found = true;
          }
        }
      }
    }
    if (!found) {
      tfel::raise("no way to initialize the initial sitffness");
    }
    // C
    // C-------------------------------------------------
    // C     OUTPUT FILE PRINT
    // C-------------------------------------------------
    //       WRITE(IOUT,1000)
    //       WRITE(IOUT,1100)E,NU,FID,SCAL
    // C
    //  1000 FORMAT(
    //      & 5X,'  "+ bd.getClassName() + " ',/,
    //      & 5X,'          compiled with TFEL ',/,
    //      & 5X,'                                            ',/,
    //      & 5X,'         ---------------------              ',/,
    //      & 5X,'             only SOLIDS !!!                ',/,
    //      & 5X,'         ---------------------              ',//)
    //  1100 FORMAT(
    //      & 5X,'E . . . . . . . . . . . . . . . . . . .=',E12.4/
    //      & 5X,'NU. . . . . . . . . . . . . . . . . . .=',E12.4/
    //      & 5X,'Temperature FUNCTION ID . . . . . . . .=',I10/
    //      & 5X,'Scalling factor for FUNCTION Ordinate .=',E12.4//)
    // C
    // C-------------------------------------------------
    out << "C-------------------------------------------------\n"
        << "C     REMOVING UNSUED VARIABLES WARNINGS\n"
        << "C-------------------------------------------------\n"
        << "      IF(.FALSE.) DUMMY_INTEGER = NFUNC\n"
        << "      IF(.FALSE.) DUMMY_INTEGER = IFUNC(1)\n"
        << "      RETURN\n"
        << "      END\n";
    out.close();
    // generating the engine source
    out.open("src/openradioss-luser01.f");
    if (!out) {
      tfel::raise("could not open file 'src/openradioss-luser01.f'");
    }
    const auto estatev = nstatev == 0 ? 1 : nstatev;
    out << "      SUBROUTINE LUSER01 (\n"
        << "     1     NEL    ,NUPARAM,NUVAR   ,NFUNC   ,IFUNC   ,NPF    ,\n"
        << "     2     TF     ,TIME   ,TIMESTEP,UPARAM  ,RHO     ,VOLUME , \n"
        << "     3     EINT   ,NGL    ,SOUNDSP ,VISCMAX ,UVAR    ,OFF     , \n"
        << "     4     SIGY   ,PLA    ,USERBUF) \n"
        << "\n"
        << "      USE LAW_USERSO \n"
        << "      IMPLICIT NONE\n"
        << "C-----------------------------------------------\n"
        << "C    INPUT DATA\n"
        << "C-----------------------------------------------\n"
        << "      INTEGER NEL, NUPARAM, NUVAR, NGL(NEL), IDX\n"
        << "      INTEGER NPF(*), NFUNC, IFUNC(NFUNC)\n"
        << "      DOUBLE PRECISION\n"
        << "     .   TIME,TIMESTEP,UPARAM(NUPARAM),\n"
        << "     .   RHO(NEL), VOLUME(NEL), EINT(NEL), TEMP(NEL)\n"
        << "      DOUBLE PRECISION TF(*)\n"
        << "C-----------------------------------------------\n"
        << "C   OUTPUT VARIABLES\n"
        << "C-----------------------------------------------\n"
        << "      DOUBLE PRECISION SOUNDSP(NEL),VISCMAX(NEL)\n"
        << "      DOUBLE PRECISION UVAR(NUVAR)\n"
        << "      DOUBLE PRECISION OFF(NEL),PLA(NEL), SIGY(NEL)\n"
        << "C-----------------------------------------------\n"
        << "C   TEMPORARY VARIABLES\n"
        << "C-----------------------------------------------\n"
        << "      DOUBLE PRECISION\n"
        << "     .    SOUNDSP0,\n"
        << "     .    RHO0,\n"
        << "     .    EPS_BTS(6),\n"
        << "     .    EPS_ETS(6),\n"
        << "     .    SIG_BTS(6),\n"
        << "     .    SIG_ETS(6),\n"
        << "     .    STATEV_BTS(" << estatev << "),\n"
        << "     .    STATEV_ETS(" << estatev << "),\n"
        << "     .    T\n"
        << "C-----------------------------------------------\n"
        << "C   C-interface\n"
        << "C-----------------------------------------------\n"
        << "      interface\n"
        << "         subroutine openradioss_luser01_interface("
        << "     .                 sig_ets, soundsp, statev_ets,\n"
        << "     .                 eto_bts, eto_ets, sig_bts,\n"
        << "     .                 statev_bts, props, rho, T, dt)\n"
        << "     .         bind(c,name = 'openradioss_luser01_interface')\n"
        << "           use, intrinsic :: iso_c_binding, only: c_int,\n"
        << "     .                                            c_double\n"
        << "           implicit none\n"
        << "           real(kind = c_double), dimension(6), intent(out) :: "
           "sig_ets\n"
        << "           real(kind = c_double), intent(out) :: soundsp\n"
        << "           real(kind = c_double), dimension(" << estatev
        << "), intent(out) ::\n"
        << "     . statev_ets\n"
        << "           real(kind = c_double), dimension(6), intent(in) :: "
        << "eto_bts\n"
        << "           real(kind = c_double), dimension(6), intent(in) :: "
        << "eto_ets\n"
        << "           real(kind = c_double), dimension(6), intent(in) :: "
        << "sig_bts\n"
        << "           real(kind = c_double), dimension(" << estatev
        << "), intent(out) ::\n"
        << "     . statev_bts\n"
        << "           real(kind = c_double), dimension(" << nprops
        << "), intent(out) :: props\n"
        << "           real(kind = c_double), intent(in), value :: rho\n"
        << "           real(kind = c_double), intent(in), value :: T\n"
        << "           real(kind = c_double), intent(in), value :: dt\n"
        << "         end subroutine openradioss_luser01_interface\n"
        << "      end interface\n"
        << "C-----------------------------------------------\n"
        << "C   LOOP OVER INTEGRATION POINTS\n"
        << "C-----------------------------------------------\n"
        << "      DO IDX = 1,NEL\n"
        << "        SIG_BTS(1) =  USERBUF%SIGOXX(IDX)\n"
        << "        SIG_BTS(2) =  USERBUF%SIGOYY(IDX)\n"
        << "        SIG_BTS(3) =  USERBUF%SIGOZZ(IDX)\n"
        << "        SIG_BTS(4) =  USERBUF%SIGOXY(IDX)\n"
        << "        SIG_BTS(5) =  USERBUF%SIGOXZ(IDX)\n"
        << "        SIG_BTS(6) =  USERBUF%SIGOYZ(IDX)\n"
        << "        EPS_BTS(1) =  USERBUF%EPSXX(IDX)\n"
        << "        EPS_BTS(2) =  USERBUF%EPSYY(IDX)\n"
        << "        EPS_BTS(3) =  USERBUF%EPSZZ(IDX)\n"
        << "        EPS_BTS(4) =  USERBUF%EPSXY(IDX)\n"
        << "        EPS_BTS(5) =  USERBUF%EPSXZ(IDX)\n"
        << "        EPS_BTS(6) =  USERBUF%EPSYZ(IDX)\n"
        << "        EPS_ETS(1) =  USERBUF%EPSXX(IDX) + USERBUF%DEPSXX(IDX)\n"
        << "        EPS_ETS(2) =  USERBUF%EPSYY(IDX) + USERBUF%DEPSYY(IDX)\n"
        << "        EPS_ETS(3) =  USERBUF%EPSZZ(IDX) + USERBUF%DEPSZZ(IDX)\n"
        << "        EPS_ETS(4) =  USERBUF%EPSXY(IDX) + USERBUF%DEPSXY(IDX)\n"
        << "        EPS_ETS(5) =  USERBUF%EPSXZ(IDX) + USERBUF%DEPSXZ(IDX)\n"
        << "        EPS_ETS(6) =  USERBUF%EPSYZ(IDX) + USERBUF%DEPSYZ(IDX)\n"
        << "        RHO0 = USERBUF%RHO0(IDX)\n"
        << "        T = USERBUF%TEMP(IDX)\n";
    if (nstatev != 0) {
      for (int i = 0; i != nstatev; ++i) {
        out << "        STATEV_BTS(" << i + 1 << ") = UVAR(IDX, " << i + 1
            << ")\n";
      }
    }
    out << "        CALL MFRONT_LUSER01_WRAPPER(SIG_ETS, SOUNDSP0, STATEV_ETS,\n"
        << "     .       EPS_BTS, EPS_ETS, SIG_BTS, STATEV_BTS,\n"
        << "     .       UPARAM, RHO0, T, TIMESTEP)\n"
        << "        USERBUF%SIGNXX(IDX) = SIG_ETS(1)\n"
        << "        USERBUF%SIGNYY(IDX) = SIG_ETS(2)\n"
        << "        USERBUF%SIGNZZ(IDX) = SIG_ETS(3)\n"
        << "        USERBUF%SIGNXY(IDX) = SIG_ETS(4)\n"
        << "        USERBUF%SIGNXZ(IDX) = SIG_ETS(5)\n"
        << "        USERBUF%SIGNYZ(IDX) = SIG_ETS(6)\n"
        << "        SOUNDSP(IDX) = SOUNDSP0\n";
    if (nstatev != 0) {
      for (int i = 0; i != nstatev; ++i) {
        out << "        UVAR(IDX, " << i + 1 << ") = STATEV_ETS(" << i + 1
            << ")\n";
      }
    }
    out << "      ENDDO\n"
        << "      RETURN\n"
        << "      END\n";
    out.close();
    //
    const auto name = bd.getLibrary() + bd.getClassName();
    const auto header = name + "-" + this->getInterfaceName() + ".hxx";
    const auto f = this->getFunctionNameForHypothesis(name, h);
    out.open("src/openradioss-luser01-interface.cxx");
    if (!out) {
      tfel::raise("could not open file 'src/openradioss-luser01-interface.cxx'");
    }
    out << "#include \"MFront/GenericBehaviour/BehaviourData.hxx\"\n"
        << "#include \"MFront/GenericBehaviour/" << header << "\"\n\n"
        << "extern \"C\" {\n\n"
        << "void openradioss_luser01_interface(double* const sig_ets,\n"
        << "double *const speed_of_sound,\n"
        << "double *const statev_ets,\n"
        << "double *const eto_bts,\n"
        << "double *const eto_ets,\n"
        << "const double *const sig_bts,\n"
        << "const double *const statev_bts,\n"
        << "const double *const mps,\n"
        << "const double rho,\n"
        << "const double T,\n"
        << "const double dt){\n"
        << "static char error_msg[512];\n"
        << "double K = 100;\n"
        << "double rdt;\n"
        << "mfront_gb_BehaviourData d;\n"
        << "d.error_message = error_msg;\n"
        << "d.K = &K;\n"
        << "d.dt = dt;\n"
        << "d.rdt = &rdt;\n"
        << "d.speed_of_sound = speed_of_sound;\n"
        << "d.s0.gradients = eto_bts;\n"
        << "d.s0.thermodynamic_forces = sig_bts;\n"
        << "d.s0.mass_density = &rho;\n"
        << "d.s0.material_properties = mps;\n"
        << "d.s0.internal_state_variables = statev_bts;\n"
        << "d.s0.stored_energy = nullptr;\n"
        << "d.s0.dissipated_energy = nullptr;\n"
        << "d.s0.external_state_variables = &T;\n"
        << "d.s1.gradients = eto_ets;\n"
        << "d.s1.thermodynamic_forces = sig_ets;\n"
        << "d.s1.mass_density = &rho;\n"
        << "d.s1.material_properties = mps;\n"
        << "d.s1.internal_state_variables = statev_ets;\n"
        << "d.s1.stored_energy = nullptr;\n"
        << "d.s1.dissipated_energy = nullptr;\n"
        << "d.s1.external_state_variables = &T;\n"
        << "const auto r = " << f << "(&d);\n"
        << "} // end of openradioss_luser01_interface\n\n"
        << "} // end of extern \"C\"\n";
    out.close();
  }  // end of endTreatment

  OpenRadiossInterface::~OpenRadiossInterface() = default;

}  // end of namespace mfront