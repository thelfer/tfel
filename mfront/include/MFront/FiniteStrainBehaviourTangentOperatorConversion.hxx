/*!
 * \file
 * mfront/include/MFront/FiniteStrainBehaviourTangentOperatorConversion.hxx
 * \brief
 * \author Thomas Helfer
 * \brief 18 juin 2014
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_FINITESTRAINBEHAVIOURTANGENTOPERATORCONVERSION_HXX
#define LIB_MFRONT_FINITESTRAINBEHAVIOURTANGENTOPERATORCONVERSION_HXX

#include <vector>
#include <string>
#include "TFEL/Material/FiniteStrainBehaviourTangentOperator.hxx"

namespace mfront {

  /*!
   * \brief structure describing a conversion between two tangent operator
   */
  struct FiniteStrainBehaviourTangentOperatorConversion {
    //! \brief a simple alias
    typedef tfel::material::FiniteStrainBehaviourTangentOperatorBase::Flag
        TangentOperatorFlag;
    //! \brief \return : the list of all available conversions
    static std::vector<FiniteStrainBehaviourTangentOperatorConversion>
    getAvailableFiniteStrainBehaviourTangentOperatorConversions();
    /* copy and move constructors/assignement operators */
    FiniteStrainBehaviourTangentOperatorConversion(
        const FiniteStrainBehaviourTangentOperatorConversion&) = default;
    FiniteStrainBehaviourTangentOperatorConversion(
        FiniteStrainBehaviourTangentOperatorConversion&&) = default;
    FiniteStrainBehaviourTangentOperatorConversion& operator=(
        const FiniteStrainBehaviourTangentOperatorConversion&) = default;
    FiniteStrainBehaviourTangentOperatorConversion& operator=(
        FiniteStrainBehaviourTangentOperatorConversion&&) = default;
    //! \return the conversion starting point
    [[nodiscard]] TangentOperatorFlag from() const;
    //! \return the conversion result point
    [[nodiscard]] TangentOperatorFlag to() const;
    //! \return ther intermediate conversion code
    [[nodiscard]] const std::string& getIntermediateConversion() const;
    //! \return ther intermediate conversion code
    [[nodiscard]] const std::string& getFinalConversion() const;
    //! \brief destrutor
    ~FiniteStrainBehaviourTangentOperatorConversion() noexcept;

   protected:
    /*!
     * \param[in] f  : conversion starting point
     * \param[in] t  : conversion result
     * \param[in] s  : intermediate conversion code
     * \param[in] s2 : final conversion code
     */
    FiniteStrainBehaviourTangentOperatorConversion(const TangentOperatorFlag,
                                                   const TangentOperatorFlag,
                                                   const std::string&,
                                                   const std::string&);
    //! \brief starting point
    TangentOperatorFlag b;
    //! \brief result
    TangentOperatorFlag e;
    //! \brief code associated with the intermediate conversion
    std::string c;
    //! \brief code associated with the final conversion
    std::string cf;
  };

}  // end of namespace mfront

#endif /* LIB_MFRONT_FINITESTRAINBEHAVIOURTANGENTOPERATORCONVERSION_HXX */
