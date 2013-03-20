/*!
 * \file   AsterRotationMatrix.hxx
 * \brief  This file declares the AsterRotationMatrix class
 * \author É. Castelier
 * \date   17 mar 2008
 */

#ifndef _LIB_MFRONT_ASTERROTATIONMATRIX_H_
#define _LIB_MFRONT_ASTERROTATIONMATRIX_H_ 

#include"MFront/Aster/Aster.hxx"
#include"MFront/Aster/AsterConfig.hxx"

namespace aster
{

  struct MFRONT_ASTER_VISIBILITY_EXPORT AsterRotationMatrix2D
  {
    AsterRotationMatrix2D(const AsterReal *const);
    
    // Compute stresses in the material space
    void rotateStressesForward(const AsterReal *const,
			       AsterReal *const) const;
    // Compute stresses back in the global space
    void rotateStressesBackward(const AsterReal *const,
				AsterReal *const) const;

    // Compute strain in the material space
    void rotateStrainsForward(const AsterReal *const,
			      AsterReal *const) const;
    // Compute stresses back in the global space
    void rotateStrainsBackward(const AsterReal *const,
			       AsterReal *const) const;
    // compute the stiffness matrix in the global space
    void rotateStiffnessMatrixBackward(AsterReal *const) const;
  private:

    AsterReal MN[4][4];
  
  }; // end of struct AsterRotationMatrix2D
  
  struct MFRONT_ASTER_VISIBILITY_EXPORT AsterRotationMatrix3D
  {
    
    AsterRotationMatrix3D(const AsterReal *const);
    
    // Compute stresses in the material space
    void rotateStressesForward(const AsterReal *const,
			       AsterReal *const) const;
    // Compute stresses back in the global space
    void rotateStressesBackward(const AsterReal *const,
				AsterReal *const) const;

    // Compute strain in the material space
    void rotateStrainsForward(const AsterReal *const,
			      AsterReal *const) const;
    // Compute striain back in the global space
    void rotateStrainsBackward(const AsterReal *const,
			       AsterReal *const) const;
    // compute the stiffness matrix in the global space
    void rotateStiffnessMatrixBackward(AsterReal *const) const;

  private:

    AsterReal MN[6][6];
  
  }; // end of struct AsterRotationMatrix3D

} // end of namespace aster

#endif /* _LIB_MFRONT_ASTERROTATIONMATRIX_H */

