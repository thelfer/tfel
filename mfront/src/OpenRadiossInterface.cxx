/*!
 * \file   mfront/src/OpenRadiossInterface.cxx
 * \brief
 * \author Thomas Helfer
 * \date   10/04/2025
 */

#include <iostream>

#include <fstream>
#include "TFEL/Raise.hxx"
#include "MFront/BehaviourDescription.hxx"
#include "MFront/OpenRadiossInterface.hxx"

namespace mfront {

  std::string OpenRadiossInterface::getName() { return "OpenRadioss"; }

  std::string OpenRadiossInterface::getInterfaceName() const { return "OpenRadioss"; }

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
    GenericBehaviourInterface::endTreatment(bd, fd);
    // generating the starter sources
    const auto& d = bd.getBehaviourData(h);
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
    auto out = std::ofstream("src/lecmuser01.f");
    if (!out) {
      tfel::raise("could not open file 'src/lecmuser01.f'");
    }
    out << "      SUBROUTINE "
           "LECMUSER01(IIN,IOUT,UPARAM,MAXUPARAM,NUPARAM, \n"
        << "     .                NUVAR,IFUNC,MAXFUNC,NFUNC,STIFINT,\n"
        << "     .                USERBUF)\n"
        << "      USE LAW_USER\n"
        << "      IMPLICIT NONE\n"
        << "      INTEGER IIN,IOUT,MAXUPARAM,NUPARAM,NUVAR,\n"
        << "     .        MAXFUNC,NFUNC,IFUNC(MAXFUNC)\n"
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
    // C
    // C-------------------------------------------------
    // C     OUTPUT FILE PRINT
    // C-------------------------------------------------
    //       WRITE(IOUT,1000)
    //       WRITE(IOUT,1100)E,NU,FID,SCAL
    // C
    //  1000 FORMAT(
    //      & 5X,'  MY Temperature dependant ELASTIC USER LAW ',/,
    //      & 5X,'          compiled 2017.04.25               ',/,
    //      & 5X,'          with MinGW compiler               ',/,
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
    out << "      RETURN\n"
        << "      END\n";
    // generating the engine source
  }  // end of endTreatment

  OpenRadiossInterface::~OpenRadiossInterface() = default;

}  // end of namespace mfront