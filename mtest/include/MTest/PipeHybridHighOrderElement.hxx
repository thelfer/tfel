/*!
 * \file   PipeHybridHighOrderElement.hxx
 * \brief
 * \author Thomas Helfer
 * \date   10 déc. 2015
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MTEST_PIPEHYBRIDHIGHORDERELEMENT_HXX
#define LIB_MTEST_PIPEHYBRIDHIGHORDERELEMENT_HXX

#include <memory>

#include "TFEL/Math/vector.hxx"
#include "TFEL/Math/matrix.hxx"
#include "TFEL/Math/General/ConstExprMathFunctions.hxx"
#include "MTest/Types.hxx"
#include "MTest/SolverOptions.hxx"
#include "MTest/PipeElementBase.hxx"

namespace mtest {

  /*!
   * \brief structure describing an Hybrid High Order element for pipes
   */
  struct PipeHybridHighOrderElement : PipeElementBase {
    /*!
     * \brief constructor
     * \param[in] m: mesh
     * \param[in] b: behaviour
     * \param[in] n: element number
     */
    PipeHybridHighOrderElement(const PipeMesh&,
                               const Behaviour&,
                               const size_t);

    size_type getNumberOfNodes() const override;

    size_type getNumberOfIntegrationPoints() const override;

    void setIntegrationPointsPositions(StructureCurrentState&) const override;

    void computeStrain(StructureCurrentState&,
                       const tfel::math::vector<real>&,
                       const bool) const override;

    std::pair<bool, real> updateStiffnessMatrixAndInnerForces(
        tfel::math::matrix<real>&,
        tfel::math::vector<real>&,
        StructureCurrentState&,
        const tfel::math::vector<real>&,
        const real,
        const StiffnessMatrixType) const override;

    //! \brief destructor
    ~PipeHybridHighOrderElement() noexcept override;

   private:
    //! \brief a structure containing the element private members
    struct PrivateMembers;
    //! \brief private members
    std::unique_ptr<PrivateMembers> pm;
  };  // end of struct PipeHybridHighOrderElement

}  // end of namespace mtest

#endif /* LIB_MTEST_PIPEHYBRIDHIGHORDERELEMENT_HXX */
