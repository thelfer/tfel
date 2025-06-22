/*!
 * \file   CovarianceMatrix.hxx
 * \brief
 * \author Étienne Castelier
 * \date   10 oct. 2016
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_COVARIANCEMATRIX_HXX
#define LIB_COVARIANCEMATRIX_HXX

#include <vector>

namespace tfel {

  namespace math {

    namespace anderson {

      //! Covariance matrix
      template <typename real>
      struct CovarianceMatrix {
        //! a simple alias
        using pointer = typename std::vector<real>::iterator;
        //! a simple alias
        using const_pointer = typename std::vector<real>::const_iterator;
        //! a simple alias
        using size_type = unsigned char;
        //! Default constructor
        CovarianceMatrix();
        //! Constructor
        //! \param Nmax Matrix size
        CovarianceMatrix(size_type Nmax);
        //! Destructor
        virtual ~CovarianceMatrix();
        //! Resize the matrix
        //! \param Nmax Matrix size
        void resize(size_type Nmax);
        //! \return the matrix size
        size_type getSize() const;
        //! \param N Line index
        //! \return the address of the Nth line
        pointer getLine(size_type N);
        // Shift the covariances
        void shift();
        //! Anderson weights by Gram-Schmidt process
        //! Descendent order
        //! \param w: anderson weights
        void weightsGSchmidtD(std::vector<real>&);
        //! Memory allocation
        void alloc();
        //! Memory deletion
        void clear();
        //! Gram-Schmidt Factorisation
        //! Descendent order
        void GSFactorD();
        //! Maximal matrix size
        size_type Nmax = 0;
        //! Current Depth of Anderson acceleration
        size_type N = 0;
        //! Covariances
        std::vector<real> C;
        //! Covariance factors
        std::vector<real> a;
        std::vector<real> ne;
      };  // end of CovarianceMatrix

    }  // end of  namespace anderson

  }  // end of namespace math

}  // end of namespace tfel

#include "TFEL/Math/AccelerationAlgorithms/CovarianceMatrix.ixx"

#endif /* LIB_COVARIANCEMATRIX_HXX */
