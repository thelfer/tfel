/*! 
 * \file  AsterFiniteStrainRotationMatrix.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 03 juin 2014
 */

#ifndef _LIB_MFRONT_ASTER_ASTERFINITESTRAINROTATIONMATRIX_H_
#define _LIB_MFRONT_ASTER_ASTERFINITESTRAINROTATIONMATRIX_H_ 

#include"MFront/Aster/AsterConfig.hxx"
#include"MFront/Aster/Aster.hxx"

namespace aster
{

  struct MFRONT_ASTER_VISIBILITY_EXPORT AsterFiniteStrainRotationMatrix2D
  {
    AsterFiniteStrainRotationMatrix2D(const AsterReal *const,
				     const AsterReal *const);
    // Compute strains in the material space
    void rotateStrainsForward(const AsterReal *const,
			      AsterReal *const);
    // Compute strains back in the global space
    void rotateStrainsBackward(const AsterReal *const,
			       AsterReal *const);
    // Compute stresses in the material space
    void rotateStressesForward(const AsterReal *const,
			       AsterReal *const);
    // Compute stresses back in the global space
    void rotateStressesBackward(const AsterReal *const,
				AsterReal *const);
    // Compute strain in the material space
    void rotateDeformationGradientForward(const AsterReal *const,
					  AsterReal *const);
    // compute the stiffness matrix in the global space
    void rotateTangentOperatorBackward(AsterReal *const) const;
  private:

    AsterReal a[4];
  
  }; // end of struct AsterFiniteStrainRotationMatrix2D
  
  struct MFRONT_ASTER_VISIBILITY_EXPORT AsterFiniteStrainRotationMatrix3D
  {
    
    AsterFiniteStrainRotationMatrix3D(const AsterReal *const,
				      const AsterReal *const);
    // Compute strains in the material space
    void rotateStrainsForward(const AsterReal *const,
			      AsterReal *const);
    // Compute strains back in the global space
    void rotateStrainsBackward(const AsterReal *const,
			       AsterReal *const);
    // Compute stresses in the material space
    void rotateStressesForward(const AsterReal *const,
			       AsterReal *const);
    // Compute stresses back in the global space
    void rotateStressesBackward(const AsterReal *const,
				AsterReal *const);
    // Compute strain in the material space
    void rotateDeformationGradientForward(const AsterReal *const,
					  AsterReal *const);
    // Compute stresses back in the global space
    void rotateDeformationGradientBackward(const AsterReal *const,
					   AsterReal *const);
    // compute the stiffness matrix in the global space
    void rotateTangentOperatorBackward(AsterReal *const) const;
  private:

    AsterReal a [9];
  
  }; // end of struct AsterFiniteStrainRotationMatrix3D

} // end of namespace aster

#endif /* _LIB_MFRONT_ASTER_ASTERFINITESTRAINROTATIONMATRIX_H */

