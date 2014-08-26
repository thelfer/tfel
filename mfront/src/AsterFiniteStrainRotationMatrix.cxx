/*! 
 * \file  AsterFiniteStrainRotationMatrix.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 26 mars 2014
 */

#include<stdexcept>

#include"MFront/Aster/AsterFiniteStrainRotationMatrix.hxx"

namespace aster
{

  AsterFiniteStrainRotationMatrix2D::AsterFiniteStrainRotationMatrix2D(const AsterReal *const V,
								     const AsterReal *const drot)
  {
    // Premier vecteur
    // a[1,1]
    a[0]=drot[0]*V[0]+drot[3]*V[1];
    // a[2,1]
    a[1]=drot[1]*V[0]+drot[4]*V[1];
    // Deuxième vecteur :
    //   Produit vectoriel
    // a[1,2]
    a[2]=-a[1];
    // a[2,2]
    a[3]=a[0];
  } // end of AsterFiniteStrainRotationMatrix2D::AsterFiniteStrainRotationMatrix2D
    
  // Compute strains in the material space
  void
  AsterFiniteStrainRotationMatrix2D::rotateStrainsForward(const AsterReal *const e,
							  AsterReal *const em)
  {
    em[0]=a[0]*a[1]*e[3]+a[1]*a[1]*e[1]+a[0]*a[0]*e[0];
    em[1]=a[2]*a[3]*e[3]+a[3]*a[3]*e[1]+a[2]*a[2]*e[0];
    em[2]=e[2];
    em[3]=(a[0]*a[3]+a[1]*a[2])*e[3]+2*a[1]*a[3]*e[1]+2*a[0]*a[2]*e[0];
  } // end of AsterFiniteStrainRotationMatrix2D::rotateStrainsForward

  // Compute strains back in the global space
  void
  AsterFiniteStrainRotationMatrix2D::rotateStrainsBackward(const AsterReal *const e,
							   AsterReal *const eg)
  {
    eg[0]=a[0]*a[2]*e[3]+a[2]*a[2]*e[1]+a[0]*a[0]*e[0];
    eg[1]=a[1]*a[3]*e[3]+a[3]*a[3]*e[1]+a[1]*a[1]*e[0];
    eg[2]=e[2];
    eg[3]=(a[0]*a[3]+a[1]*a[2])*e[3]+2*a[2]*a[3]*e[1]+2*a[0]*a[1]*e[0];
  } // end of AsterFiniteStrainRotationMatrix2D::rotateStrainsBackward

  // Compute stresses in the material space
  void
  AsterFiniteStrainRotationMatrix2D::rotateStressesForward(const AsterReal *const s,
							  AsterReal *const sm)
  {
    sm[0]=2*a[0]*a[1]*s[3]+a[1]*a[1]*s[1]+a[0]*a[0]*s[0];
    sm[1]=2*a[2]*a[3]*s[3]+a[3]*a[3]*s[1]+a[2]*a[2]*s[0];
    sm[2]=s[2];
    sm[3]=(a[0]*a[3]+a[1]*a[2])*s[3]+a[1]*a[3]*s[1]+a[0]*a[2]*s[0];
  } // end of AsterFiniteStrainRotationMatrix2D::rotateStressesForward

  // Compute stresses back in the global space
  void
  AsterFiniteStrainRotationMatrix2D::rotateStressesBackward(const AsterReal *const s,
							   AsterReal *const sg)
  {
    sg[0]=2*a[0]*a[2]*s[3]+a[2]*a[2]*s[1]+a[0]*a[0]*s[0];
    sg[1]=2*a[1]*a[3]*s[3]+a[3]*a[3]*s[1]+a[1]*a[1]*s[0];
    sg[2]=s[2];
    sg[3]=(a[0]*a[3]+a[1]*a[2])*s[3]+a[2]*a[3]*s[1]+a[0]*a[1]*s[0];
  } // end of AsterFiniteStrainRotationMatrix2D::rotateStressesBackward

  void
  AsterFiniteStrainRotationMatrix2D::rotateDeformationGradientForward(const AsterReal *const F,
								     AsterReal *const Fg)
  {
    Fg[0]=a[1]*a[1]*F[4]+a[0]*a[1]*F[3]+a[0]*a[1]*F[1]+a[0]*a[0]*F[0];
    Fg[1]=a[1]*a[3]*F[4]+a[1]*a[2]*F[3]+a[0]*a[3]*F[1]+a[0]*a[2]*F[0];
    Fg[3]=0;
    Fg[4]=a[1]*a[3]*F[4]+a[0]*a[3]*F[3]+a[1]*a[2]*F[1]+a[0]*a[2]*F[0];
    Fg[5]=a[3]*a[3]*F[4]+a[2]*a[3]*F[3]+a[2]*a[3]*F[1]+a[2]*a[2]*F[0];
    Fg[6]=0;
    Fg[7]=0;
    Fg[8]=F[8];
  } // end of AsterFiniteStrainRotationMatrix2D::rotateDeformationGradientForward

  void
  AsterFiniteStrainRotationMatrix2D::rotateTangentOperatorBackward(AsterReal *const) const
  {
    using namespace std;
    string msg("AsterFiniteStrainRotationMatrix2D::rotateTangentOperatorBackward : "
	       "unimplemented feature");
    throw(runtime_error(msg));
  } // end of AsterFiniteStrainRotationMatrix2D::rotateTangentOperatorBackward

  AsterFiniteStrainRotationMatrix3D::AsterFiniteStrainRotationMatrix3D(const AsterReal *const V,
								     const AsterReal *const drot)
  {
    // Premier vecteur
    a[0]=drot[0]*V[0]+drot[3]*V[1]+drot[6]*V[2];
    a[1]=drot[1]*V[0]+drot[4]*V[1]+drot[7]*V[2];
    a[2]=drot[2]*V[0]+drot[5]*V[1]+drot[8]*V[2];
    // Deuxième vecteur
    a[3]=drot[0]*V[3]+drot[3]*V[4]+drot[6]*V[5];
    a[4]=drot[1]*V[3]+drot[4]*V[4]+drot[7]*V[5];
    a[5]=drot[2]*V[3]+drot[5]*V[4]+drot[8]*V[5];
    // Troisième vecteur :
    //   produit vectoriel des deux premiers
    a[6]=a[1]*a[5]-a[4]*a[2];
    a[7]=a[2]*a[3]-a[5]*a[0];
    a[8]=a[0]*a[4]-a[3]*a[1];
  } // end of AsterFiniteStrainRotationMatrix3D::AsterFiniteStrainRotationMatrix3D

  // Compute strains in the material space
  void
  AsterFiniteStrainRotationMatrix3D::rotateStrainsForward(const AsterReal *const e,
							  AsterReal *const em)
  {
    em[0]=a[1]*a[2]*e[5]+a[0]*a[2]*e[4]+a[0]*a[1]*e[3]+a[2]*a[2]*e[2]+a[1]*a[1]*e[1]+a[0]*a[0]*e[0];
    em[1]=a[4]*a[5]*e[5]+a[3]*a[5]*e[4]+a[3]*a[4]*e[3]+a[5]*a[5]*e[2]+a[4]*a[4]*e[1]+a[3]*a[3]*e[0];
    em[2]=a[7]*a[8]*e[5]+a[6]*a[8]*e[4]+a[6]*a[7]*e[3]+a[8]*a[8]*e[2]+a[7]*a[7]*e[1]+a[6]*a[6]*e[0];
    em[3]=(a[1]*a[5]+a[2]*a[4])*e[5]+(a[0]*a[5]+a[2]*a[3])*e[4]+(a[0]*a[4]+a[1]*a[3])*e[3]+2*a[2]*a[5]*e[2]+2*a[1]*a[4]*e[1]+2*a[0]*a[3]*e[0];
    em[4]=(a[1]*a[8]+a[2]*a[7])*e[5]+(a[0]*a[8]+a[2]*a[6])*e[4]+(a[0]*a[7]+a[1]*a[6])*e[3]+2*a[2]*a[8]*e[2]+2*a[1]*a[7]*e[1]+2*a[0]*a[6]*e[0];
    em[5]=(a[4]*a[8]+a[5]*a[7])*e[5]+(a[3]*a[8]+a[5]*a[6])*e[4]+(a[3]*a[7]+a[4]*a[6])*e[3]+2*a[5]*a[8]*e[2]+2*a[4]*a[7]*e[1]+2*a[3]*a[6]*e[0];
  } // end of AsterFiniteStrainRotationMatrix3D::rotateStrainsForward

  // Compute strains back in the global space
  void
  AsterFiniteStrainRotationMatrix3D::rotateStrainsBackward(const AsterReal *const e,
							  AsterReal *const eg)
  {
    eg[0]=a[3]*a[6]*e[5]+a[0]*a[6]*e[4]+a[0]*a[3]*e[3]+a[6]*a[6]*e[2]+a[3]*a[3]*e[1]+a[0]*a[0]*e[0];
    eg[1]=a[4]*a[7]*e[5]+a[1]*a[7]*e[4]+a[1]*a[4]*e[3]+a[7]*a[7]*e[2]+a[4]*a[4]*e[1]+a[1]*a[1]*e[0];
    eg[2]=a[5]*a[8]*e[5]+a[2]*a[8]*e[4]+a[2]*a[5]*e[3]+a[8]*a[8]*e[2]+a[5]*a[5]*e[1]+a[2]*a[2]*e[0];
    eg[3]=(a[3]*a[7]+a[4]*a[6])*e[5]+(a[0]*a[7]+a[1]*a[6])*e[4]+(a[0]*a[4]+a[1]*a[3])*e[3]+2*a[6]*a[7]*e[2]+2*a[3]*a[4]*e[1]+2*a[0]*a[1]*e[0];
    eg[4]=(a[3]*a[8]+a[5]*a[6])*e[5]+(a[0]*a[8]+a[2]*a[6])*e[4]+(a[0]*a[5]+a[2]*a[3])*e[3]+2*a[6]*a[8]*e[2]+2*a[3]*a[5]*e[1]+2*a[0]*a[2]*e[0];
    eg[5]=(a[4]*a[8]+a[5]*a[7])*e[5]+(a[1]*a[8]+a[2]*a[7])*e[4]+(a[1]*a[5]+a[2]*a[4])*e[3]+2*a[7]*a[8]*e[2]+2*a[4]*a[5]*e[1]+2*a[1]*a[2]*e[0];
  } // end of AsterFiniteStrainRotationMatrix3D::rotateStrainsBackward
    
  // Compute stresses in the material space
  void
  AsterFiniteStrainRotationMatrix3D::rotateStressesForward(const AsterReal *const s,
							  AsterReal *const sm)
  {
    sm[0]=2*a[1]*a[2]*s[5]+2*a[0]*a[2]*s[4]+2*a[0]*a[1]*s[3]+a[2]*a[2]*s[2]+a[1]*a[1]*s[1]+a[0]*a[0]*s[0];
    sm[1]=2*a[4]*a[5]*s[5]+2*a[3]*a[5]*s[4]+2*a[3]*a[4]*s[3]+a[5]*a[5]*s[2]+a[4]*a[4]*s[1]+a[3]*a[3]*s[0]; 
    sm[2]=2*a[7]*a[8]*s[5]+2*a[6]*a[8]*s[4]+2*a[6]*a[7]*s[3]+a[8]*a[8]*s[2]+a[7]*a[7]*s[1]+a[6]*a[6]*s[0];
    sm[3]=(a[1]*a[5]+a[2]*a[4])*s[5]+(a[2]*a[3]+a[0]*a[5])*s[4]+(a[0]*a[4]+a[1]*a[3])*s[3]+a[2]*a[5]*s[2]+a[1]*a[4]*s[1]+a[0]*a[3]*s[0];
    sm[4]=(a[1]*a[8]+a[2]*a[7])*s[5]+(a[2]*a[6]+a[0]*a[8])*s[4]+(a[0]*a[7]+a[1]*a[6])*s[3]+a[2]*a[8]*s[2]+a[1]*a[7]*s[1]+a[0]*a[6]*s[0];
    sm[5]=(a[4]*a[8]+a[5]*a[7])*s[5]+(a[5]*a[6]+a[3]*a[8])*s[4]+(a[3]*a[7]+a[4]*a[6])*s[3]+a[5]*a[8]*s[2]+a[4]*a[7]*s[1]+a[3]*a[6]*s[0];
  } // end of AsterFiniteStrainRotationMatrix3D::rotateStressesForward

  // Compute stresses back in the global space
  void
  AsterFiniteStrainRotationMatrix3D::rotateStressesBackward(const AsterReal *const s,
							   AsterReal *const sg)
  {
    sg[0]=2*a[3]*a[6]*s[5]+2*a[0]*a[6]*s[4]+2*a[0]*a[3]*s[3]+a[6]*a[6]*s[2]+a[3]*a[3]*s[1]+a[0]*a[0]*s[0];
    sg[1]=2*a[4]*a[7]*s[5]+2*a[1]*a[7]*s[4]+2*a[1]*a[4]*s[3]+a[7]*a[7]*s[2]+a[4]*a[4]*s[1]+a[1]*a[1]*s[0];
    sg[2]=2*a[5]*a[8]*s[5]+2*a[2]*a[8]*s[4]+2*a[2]*a[5]*s[3]+a[8]*a[8]*s[2]+a[5]*a[5]*s[1]+a[2]*a[2]*s[0];
    sg[3]=(a[3]*a[7]+a[4]*a[6])*s[5]+(a[1]*a[6]+a[0]*a[7])*s[4]+(a[0]*a[4]+a[1]*a[3])*s[3]+a[6]*a[7]*s[2]+a[3]*a[4]*s[1]+a[0]*a[1]*s[0];
    sg[4]=(a[3]*a[8]+a[5]*a[6])*s[5]+(a[2]*a[6]+a[0]*a[8])*s[4]+(a[0]*a[5]+a[2]*a[3])*s[3]+a[6]*a[8]*s[2]+a[3]*a[5]*s[1]+a[0]*a[2]*s[0]; 
    sg[5]=(a[4]*a[8]+a[5]*a[7])*s[5]+(a[2]*a[7]+a[1]*a[8])*s[4]+(a[1]*a[5]+a[2]*a[4])*s[3]+a[7]*a[8]*s[2]+a[4]*a[5]*s[1]+a[1]*a[2]*s[0]; 
  } // end of AsterFiniteStrainRotationMatrix3D::rotateStressesBackward

  void
  AsterFiniteStrainRotationMatrix3D::rotateDeformationGradientForward(const AsterReal *const F,
								     AsterReal *const Fm)
  {
    Fm[0]=a[2]*a[2]*F[8]+a[1]*a[2]*F[7]+a[0]*a[2]*F[6]+a[1]*a[2]*F[5]+a[1]*a[1]*F[4]+a[0]*a[1]*F[3]+a[0]*a[2]*F[2]+a[0]*a[1]*F[1]+a[0]*a[0]*F[0];
    Fm[1]=a[2]*a[5]*F[8]+a[2]*a[4]*F[7]+a[2]*a[3]*F[6]+a[1]*a[5]*F[5]+a[1]*a[4]*F[4]+a[1]*a[3]*F[3]+a[0]*a[5]*F[2]+a[0]*a[4]*F[1]+a[0]*a[3]*F[0];
    Fm[2]=a[2]*a[8]*F[8]+a[2]*a[7]*F[7]+a[2]*a[6]*F[6]+a[1]*a[8]*F[5]+a[1]*a[7]*F[4]+a[1]*a[6]*F[3]+a[0]*a[8]*F[2]+a[0]*a[7]*F[1]+a[0]*a[6]*F[0];
    Fm[3]=a[2]*a[5]*F[8]+a[1]*a[5]*F[7]+a[0]*a[5]*F[6]+a[2]*a[4]*F[5]+a[1]*a[4]*F[4]+a[0]*a[4]*F[3]+a[2]*a[3]*F[2]+a[1]*a[3]*F[1]+a[0]*a[3]*F[0];
    Fm[4]=a[5]*a[5]*F[8]+a[4]*a[5]*F[7]+a[3]*a[5]*F[6]+a[4]*a[5]*F[5]+a[4]*a[4]*F[4]+a[3]*a[4]*F[3]+a[3]*a[5]*F[2]+a[3]*a[4]*F[1]+a[3]*a[3]*F[0];
    Fm[5]=a[5]*a[8]*F[8]+a[5]*a[7]*F[7]+a[5]*a[6]*F[6]+a[4]*a[8]*F[5]+a[4]*a[7]*F[4]+a[4]*a[6]*F[3]+a[3]*a[8]*F[2]+a[3]*a[7]*F[1]+a[3]*a[6]*F[0];
    Fm[6]=a[2]*a[8]*F[8]+a[1]*a[8]*F[7]+a[0]*a[8]*F[6]+a[2]*a[7]*F[5]+a[1]*a[7]*F[4]+a[0]*a[7]*F[3]+a[2]*a[6]*F[2]+a[1]*a[6]*F[1]+a[0]*a[6]*F[0];
    Fm[7]=a[5]*a[8]*F[8]+a[4]*a[8]*F[7]+a[3]*a[8]*F[6]+a[5]*a[7]*F[5]+a[4]*a[7]*F[4]+a[3]*a[7]*F[3]+a[5]*a[6]*F[2]+a[4]*a[6]*F[1]+a[3]*a[6]*F[0];
    Fm[8]=a[8]*a[8]*F[8]+a[7]*a[8]*F[7]+a[6]*a[8]*F[6]+a[7]*a[8]*F[5]+a[7]*a[7]*F[4]+a[6]*a[7]*F[3]+a[6]*a[8]*F[2]+a[6]*a[7]*F[1]+a[6]*a[6]*F[0];
  } // end of AsterFiniteStrainRotationMatrix3D::rotateDeformationGradientForward

  void
  AsterFiniteStrainRotationMatrix3D::rotateDeformationGradientBackward(const AsterReal *const F,
								      AsterReal *const Fg)
  {
    Fg[0]=a[6]*a[6]*F[8]+a[3]*a[6]*F[7]+a[0]*a[6]*F[6]+a[3]*a[6]*F[5]+a[3]*a[3]*F[4]+a[0]*a[3]*F[3]+a[0]*a[6]*F[2]+a[0]*a[3]*F[1]+a[0]*a[0]*F[0];
    Fg[1]=a[6]*a[7]*F[8]+a[4]*a[6]*F[7]+a[1]*a[6]*F[6]+a[3]*a[7]*F[5]+a[3]*a[4]*F[4]+a[1]*a[3]*F[3]+a[0]*a[7]*F[2]+a[0]*a[4]*F[1]+a[0]*a[1]*F[0];
    Fg[2]=a[6]*a[8]*F[8]+a[5]*a[6]*F[7]+a[2]*a[6]*F[6]+a[3]*a[8]*F[5]+a[3]*a[5]*F[4]+a[2]*a[3]*F[3]+a[0]*a[8]*F[2]+a[0]*a[5]*F[1]+a[0]*a[2]*F[0];
    Fg[3]=a[6]*a[7]*F[8]+a[3]*a[7]*F[7]+a[0]*a[7]*F[6]+a[4]*a[6]*F[5]+a[3]*a[4]*F[4]+a[0]*a[4]*F[3]+a[1]*a[6]*F[2]+a[1]*a[3]*F[1]+a[0]*a[1]*F[0];
    Fg[4]=a[7]*a[7]*F[8]+a[4]*a[7]*F[7]+a[1]*a[7]*F[6]+a[4]*a[7]*F[5]+a[4]*a[4]*F[4]+a[1]*a[4]*F[3]+a[1]*a[7]*F[2]+a[1]*a[4]*F[1]+a[1]*a[1]*F[0];
    Fg[5]=a[7]*a[8]*F[8]+a[5]*a[7]*F[7]+a[2]*a[7]*F[6]+a[4]*a[8]*F[5]+a[4]*a[5]*F[4]+a[2]*a[4]*F[3]+a[1]*a[8]*F[2]+a[1]*a[5]*F[1]+a[1]*a[2]*F[0];
    Fg[6]=a[6]*a[8]*F[8]+a[3]*a[8]*F[7]+a[0]*a[8]*F[6]+a[5]*a[6]*F[5]+a[3]*a[5]*F[4]+a[0]*a[5]*F[3]+a[2]*a[6]*F[2]+a[2]*a[3]*F[1]+a[0]*a[2]*F[0];
    Fg[7]=a[7]*a[8]*F[8]+a[4]*a[8]*F[7]+a[1]*a[8]*F[6]+a[5]*a[7]*F[5]+a[4]*a[5]*F[4]+a[1]*a[5]*F[3]+a[2]*a[7]*F[2]+a[2]*a[4]*F[1]+a[1]*a[2]*F[0];
    Fg[8]=a[8]*a[8]*F[8]+a[5]*a[8]*F[7]+a[2]*a[8]*F[6]+a[5]*a[8]*F[5]+a[5]*a[5]*F[4]+a[2]*a[5]*F[3]+a[2]*a[8]*F[2]+a[2]*a[5]*F[1]+a[2]*a[2]*F[0];
  }

  void
  AsterFiniteStrainRotationMatrix3D::rotateTangentOperatorBackward(AsterReal *const) const
  {
    using namespace std;
    string msg("AsterFiniteStrainRotationMatrix3D::rotateTangentOperatorBackward : "
	       "unimplemented feature");
    throw(runtime_error(msg));
  } // end of AsterFiniteStrainRotationMatrix3D::rotateTangentOperatorBackward

} // end of namespace aster
