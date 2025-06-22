/*!
 * \file  mtest/include/MTest/CastemFiniteStrainBehaviour.hxx
 * \brief
 * \author Thomas Helfer
 * \brief 07 avril 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MTEST_CASTEMFINITESTRAINUMATBEHAVIOUR2_HXX
#define LIB_MTEST_CASTEMFINITESTRAINUMATBEHAVIOUR2_HXX

#include "MTest/CastemSmallStrainBehaviour.hxx"

namespace mtest {

  /*!
   * \brief a class to handle finite strain mechanical beheaviours written
   * using the castem interface based on the ETO/PK1 kinematic. This class is
   * based on the legacy version of the `Cast3M` interface.
   */
  struct TFEL_VISIBILITY_LOCAL CastemFiniteStrainBehaviour2
      : public CastemSmallStrainBehaviour {
    /*!
     * \brief constructor
     * \param[in] h : modelling hypothesis
     * \param[in] l : library name
     * \param[in] b : behaviour name
     */
    CastemFiniteStrainBehaviour2(const Hypothesis,
                                 const std::string&,
                                 const std::string&);
    //! \brief destructor
    ~CastemFiniteStrainBehaviour2() override;
  };  // end of struct Behaviour

  /*!
   * \brief a class to handle mechanical beheaviours written using the umat
   * interface of the `Cast3M` finite element solver  based on the ETO/PK1
   * kinematic for `Cast3M` versions greater than 2021.
   */
  struct TFEL_VISIBILITY_LOCAL Castem21FiniteStrainBehaviour2
      : public CastemFiniteStrainBehaviour2 {
    // inheriting constructors
    using CastemFiniteStrainBehaviour2::CastemFiniteStrainBehaviour2;
    //! \brief destructor
    ~Castem21FiniteStrainBehaviour2();

   protected:
    //! \return the version of the `Cast3M` interface
    CastemInterfaceVersion getCastemInterfaceVersion() const override;
  };  // end of struct Castem21FiniteStrainBehaviour2

}  // end of namespace mtest

#endif /* LIB_MTEST_CASTEMFINITESTRAINUMATBEHAVIOUR2_HXX */
