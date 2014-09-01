/*!
 * \file   mfront/include/MFront/UMAT/UMATRotationMatrix.hxx
 * \brief  This file declares the UMATRotationMatrix class
 * \author É. Castelier
 * \date   17 mar 2008
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_MFRONT_UMATROTATIONMATRIX_H_
#define _LIB_MFRONT_UMATROTATIONMATRIX_H_ 

#include"MFront/UMAT/UMATConfig.hxx"
#include"MFront/UMAT/UMAT.hxx"

namespace umat
{

  struct MFRONT_UMAT_VISIBILITY_EXPORT UMATRotationMatrix2D
  {
    UMATRotationMatrix2D(const UMATReal *const,
			 const UMATReal *const);
    
    // Compute stresses in the material space
    void rotateStressesForward(const UMATReal *const,
			             UMATReal *const);
    // Compute stresses back in the global space
    void rotateStressesBackward(const UMATReal *const,
                                      UMATReal *const);

    // Compute strain in the material space
    void rotateStrainsForward(const UMATReal *const,
                                    UMATReal *const);
    // Compute stresses back in the global space
    void rotateStrainsBackward(const UMATReal *const,
                                     UMATReal *const);
    // compute the stiffness matrix in the global space
    void rotateTangentOperatorBackward(UMATReal *const) const;
  private:

    UMATReal MN[4][4];
  
  }; // end of struct UMATRotationMatrix2D
  
  struct MFRONT_UMAT_VISIBILITY_EXPORT UMATRotationMatrix3D
  {
    
    UMATRotationMatrix3D(const UMATReal *const,
			 const UMATReal *const);
    
    // Compute stresses in the material space
    void rotateStressesForward(const UMATReal *const,
			             UMATReal *const);
    // Compute stresses back in the global space
    void rotateStressesBackward(const UMATReal *const,
                                      UMATReal *const);

    // Compute strain in the material space
    void rotateStrainsForward(const UMATReal *const,
                                    UMATReal *const);
    // Compute stresses back in the global space
    void rotateStrainsBackward(const UMATReal *const,
                                     UMATReal *const);
    // compute the stiffness matrix in the global space
    void rotateTangentOperatorBackward(UMATReal *const) const;

  private:

    UMATReal MN[6][6];
  
  }; // end of struct UMATRotationMatrix3D

} // end of namespace umat

#endif /* _LIB_MFRONT_UMATROTATIONMATRIX_H */

