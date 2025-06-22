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

namespace tfel {

  namespace math {

    //! Weights for the Anderson accelation algorithm
    template <typename Field, typename real>
    struct AndersonBase {
      //! a simple alias
      using size_type = unsigned char;
      //! memory allocator
      using Allocator = std::function<Field*()>;
      //! Default constructor
      AndersonBase(const Allocator);
      //! Destructor
      virtual ~AndersonBase();
      const std::vector<Field*>& getU() const;
      const std::vector<Field*>& getD() const;
      //! Set Anderson algorithm
      //! \param Nmax:  method order
      //! \param alMax: alternance order
      void setAnderson(const size_type, const size_type);

     protected:
      //! Memory allocation
      void alloc();
      //! \return the Anderson method order
      size_type size() const;
      //! Reseting the Covariance matrix with a new vector
      void reset();
      anderson::CovarianceMatrix<real> cM;  //!< Covariance matrix
      std::vector<Field*> u;                //!< Deplacement Fields
      std::vector<Field*> D;                //!< Result fields
      //! memory allocator
      Allocator ma;
      //! Number of stored solutions
      size_type n;  //!< Number of stored displacement fields
      //! Anderson weights
      std::vector<real> w;
      size_type alt = 0;
      size_type alMax = 0;
    };

    namespace anderson {

      template <typename Field, typename real>
      void linear_combinaison(Field& u,
                              const std::vector<Field*>& fields,
                              const std::vector<real>& w,
                              const unsigned char n);

      template <typename Field>
      auto scalar_product(const Field& f1, const Field& f2)
          -> decltype(f1[0] * f2[0]);

    }  // end of namespace anderson

  }  // end of namespace math

}  // end of namespace tfel

#include "TFEL/Math/AccelerationAlgorithms/AndersonBase.ixx"

#endif /* _ANDERSON_HXX */
