/*! 
 * \file  mfront/include/MFront/UMAT/UMATFiniteStrainRotationMatrix.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 25 mars 2014
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_MFRONT_UMATFINITESTRAINROTATIONMATRIX_H_
#define LIB_MFRONT_UMATFINITESTRAINROTATIONMATRIX_H_ 

#include"MFront/UMAT/UMATConfig.hxx"
#include"MFront/UMAT/UMAT.hxx"

namespace umat
{

  struct MFRONT_UMAT_VISIBILITY_EXPORT UMATFiniteStrainRotationMatrix2D
  {
    UMATFiniteStrainRotationMatrix2D(const UMATReal *const,
				     const UMATReal *const);
    // Compute strains in the material space
    void rotateStrainsForward(const UMATReal *const,
			      UMATReal *const);
    // Compute strains back in the global space
    void rotateStrainsBackward(const UMATReal *const,
			       UMATReal *const);
    // Compute stresses in the material space
    void rotateStressesForward(const UMATReal *const,
			       UMATReal *const);
    // Compute stresses back in the global space
    void rotateStressesBackward(const UMATReal *const,
				UMATReal *const);
    // Compute strain in the material space
    void rotateDeformationGradientForward(const UMATReal *const,
					  UMATReal *const);
    // compute the stiffness matrix in the global space
    TFEL_NORETURN void rotateTangentOperatorBackward(UMATReal *const) const;
  private:

    UMATReal a[4];
  
  }; // end of struct UMATFiniteStrainRotationMatrix2D
  
  struct MFRONT_UMAT_VISIBILITY_EXPORT UMATFiniteStrainRotationMatrix3D
  {
    
    UMATFiniteStrainRotationMatrix3D(const UMATReal *const,
				     const UMATReal *const);
    // Compute strains in the material space
    void rotateStrainsForward(const UMATReal *const,
			      UMATReal *const);
    // Compute strains back in the global space
    void rotateStrainsBackward(const UMATReal *const,
			       UMATReal *const);
    // Compute stresses in the material space
    void rotateStressesForward(const UMATReal *const,
			       UMATReal *const);
    // Compute stresses back in the global space
    void rotateStressesBackward(const UMATReal *const,
				UMATReal *const);
    // Compute strain in the material space
    void rotateDeformationGradientForward(const UMATReal *const,
					  UMATReal *const);
    // Compute stresses back in the global space
    void rotateDeformationGradientBackward(const UMATReal *const,
					   UMATReal *const);
    // compute the stiffness matrix in the global space
    TFEL_NORETURN void rotateTangentOperatorBackward(UMATReal *const) const;
  private:

    UMATReal a [9];
  
  }; // end of struct UMATFiniteStrainRotationMatrix3D

} // end of namespace umat

#endif /* LIB_MFRONT_UMATFINITESTRAINROTATIONMATRIX_H_ */

