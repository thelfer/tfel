/*!
 * \file   AndersonBase.hxx
 * \brief
 * \author Étienne Castelier
 * \date 25 févr. 2016
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef TFEL_MATH_ANDERSON_HXX
#define TFEL_MATH_ANDERSON_HXX 1

#include <vector>
#include <functional>
#include "TFEL/Math/AccelerationAlgorithms/CovarianceMatrix.hxx"

namespace tfel::math {

  //! \brief Weights for the Anderson accelation algorithm
  template <typename Field, typename real>
  struct AndersonBase {
    //! \brief a simple alias
    using size_type = unsigned char;
    //! \brief memory allocator
    using Allocator = std::function<Field*()>;
    //! \brief constructor
    AndersonBase(const Allocator);
    //
    [[nodiscard]] const std::vector<Field*>& getU() const;
    [[nodiscard]] const std::vector<Field*>& getD() const;
    //! \brief Set Anderson algorithm
    //! \param Nmax:  method order
    //! \param alMax: alternance order
    void setAnderson(const size_type, const size_type);
    //! \brief Destructor
    virtual ~AndersonBase();

   protected:
    //! \brief Memory allocation
    void alloc();
    //! \return the Anderson method order
    [[nodiscard]] size_type size() const;
    //! \brief Reseting the Covariance matrix with a new vector
    void reset();
    anderson::CovarianceMatrix<real> cM;  //!< Covariance matrix
    std::vector<Field*> u;                //!< Deplacement Fields
    std::vector<Field*> D;                //!< Result fields
    //! \brief memory allocator
    Allocator ma;
    //! \brief Number of stored solutions
    size_type n = size_type{};  //!< Number of stored displacement fields
    //! \brief Anderson weights
    std::vector<real> w;
    size_type alt = 0;
    size_type alMax = 0;
  };

}  // end of namespace tfel::math

namespace tfel::math::anderson {

  template <typename Field, typename real>
  void linear_combinaison(Field& u,
                          const std::vector<Field*>& fields,
                          const std::vector<real>& w,
                          const unsigned char n);

  template <typename Field>
  auto scalar_product(const Field& f1, const Field& f2)
      -> decltype(f1[0] * f2[0]);

}  // end of namespace tfel::math::anderson

#include "TFEL/Math/AccelerationAlgorithms/AndersonBase.ixx"

#endif /* _ANDERSON_HXX */
