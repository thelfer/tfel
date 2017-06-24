/*!
 * \file   mfront/include/MFront/CalculiX/CalculiXRotation.hxx
 * \brief  This file declares the CalculiXRotationMatrix class
 * \author É. Castelier
 * \date   17 mar 2008
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_MFRONT_CALCULIX_CALCULIXROTATION_HXX
#define LIB_MFRONT_CALCULIX_CALCULIXROTATION_HXX 

#include<iterator>
#include"TFEL/Math/tmatrix.hxx"
#include"MFront/CalculiX/CalculiX.hxx"
#include"MFront/CalculiX/CalculiXConfig.hxx"

namespace calculix
{

  struct MFRONT_CALCULIX_VISIBILITY_EXPORT CalculiXRotation3D
  {
    CalculiXRotation3D(const CalculiXReal *const);
    
    // Compute stresses in the material space
    void rotateStressesForward(const CalculiXReal *const,
			             CalculiXReal *const);
    // Compute stresses back in the global space
    void rotateStressesBackward(const CalculiXReal *const,
                                      CalculiXReal *const);
    // compute the deformation dradient in the material space
    void rotateDeformationGradientForward(const CalculiXReal *const,
					  CalculiXReal *const);
    // Compute strain in the material space
    void rotateStrainsForward(const CalculiXReal *const,
                                    CalculiXReal *const);
    // Compute stresses back in the global space
    void rotateStrainsBackward(const CalculiXReal *const,
                                     CalculiXReal *const);
    // compute the stiffness matrix in the global space
    void rotateTangentOperatorBackward(CalculiXReal *const) const;

  private:

    const CalculiXReal a[9];
  
  }; // end of struct CalculiXRotation3D

  MFRONT_CALCULIX_VISIBILITY_EXPORT
  tfel::math::tmatrix<3u,3u,CalculiXReal>
  getRotationMatrix3D(const CalculiXReal* const);
  
} // end of namespace calculix

#endif /* LIB_MFRONT_CALCULIX_CALCULIXROTATION_HXX */

