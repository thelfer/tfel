/*!
 * \file   include/TFEL/Material/MicrostructureDescription.hxx
 * \author Antoine Martin
 * \date   25 January 2025
 * \brief  This file defines some objects to describe a microstructure.
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All
 * rights reserved. This project is publicly released under either the GNU GPL
 * Licence or the CECILL-A licence. A copy of thoses licences are delivered with
 * the sources of TFEL. CEA or EDF may also distribute this project under
 * specific licensing conditions.
 */

#ifndef LIB_TFEL_MATERIAL_MICROSTRUCTUREDESCRIPTION_HXX
#define LIB_TFEL_MATERIAL_MICROSTRUCTUREDESCRIPTION_HXX

#include "TFEL/Math/st2tost2.hxx"
#include "TFEL/Material/LinearHomogenizationSchemes.hxx"
#include <stdexcept>

namespace tfel::material{

   namespace internals {
  /*!
   * This function compares two tensors of type T. It returns false if 
   * the norm of C1-C2 is bigger than eps * norm(C2)
   * \tparam N: dimension
   * \tparam T: type of the tensors
   * \param C1,C2 st2tost2 objects
   * \return a bool
   */
   template<unsigned short int N,typename real, typename T>
   real relative_error(const tfel::math::st2tost2<N,T> &C1,const tfel::math::st2tost2<N,T> &C2){
       auto val=tfel::math::norm(C1-C2)/tfel::math::norm(C2);
       return val;
      }//end of relative_error
      
   }  // end of namespace internals


   // TO DO : check the expressions in 2D
      
    /*!
   * This function permits to know if a 3d stiffness tensor is Isotropic
   * It uses the double contraction of \f$C_i\f$ with \f$J\f$
   * and \f$K\f$ to compare \f$C_i\f$ with an isotropic tensor.
   * \tparam real: underlying type
   * \tparam StressType: type of the stiffness
   * \param Ci : stiffness tensor of type st2tost2<3u,StressType>
   * \return a boolean
   */
    template<unsigned short int N,typename real,typename StressType>
    bool isIsotropic(const tfel::math::st2tost2<N,StressType> &Ci){
       static constexpr auto eps = std::numeric_limits<real>::epsilon();
       auto J=tfel::math::st2tost2<N,real>::J();
       auto K=tfel::math::st2tost2<N,real>::K();
       const auto kappai = tfel::math::quaddot(J,Ci) / 3;
       const auto mui = tfel::math::quaddot(K,Ci) / 10;
       auto C_comp=3*kappai*J+2*mui*K;
       auto val=std::sqrt(tfel::math::quaddot(Ci-C_comp,Ci-C_comp)/tfel::math::quaddot(Ci,Ci));
       if (val > 100*eps){
          return false;
       }
       return true;
      }//end of isIsotropic
    
    
      /*!
   * This function permits to compute coefficients kappa and mu from
   * a 3d stiffness tensor, even if it is not isotropic.
   * It uses the double contraction of \f$C_i\f$ with \f$J\f$
   * and \f$K\f$.
   * \tparam real: underlying type
   * \tparam StressType: type of the stiffness
   * \param Ci : stiffness tensor of type st2tost2<3u,StressType>
   * \return a std::pair<StressType,StressType> (kappa,mu)
   */
    template<unsigned short int N,typename real,typename StressType>
    std::pair<StressType,StressType> computeKappaMu(const tfel::math::st2tost2<N,StressType> &Ci){
       auto J=tfel::math::st2tost2<N,real>::J();
       auto K=tfel::math::st2tost2<N,real>::K();
       const StressType kappai = tfel::math::quaddot(J,Ci) / 3;
       const StressType mui = tfel::math::quaddot(K,Ci) / 10;
       return {kappai,mui};
      }//end of computeKappaMu
      
      
        /*!
   * This function permits to compute coefficients E and nu from
   * a 3d stiffness tensor, even if it is not isotropic.
   * It first compute kappa and mu.
   * \tparam real: underlying type
   * \tparam StressType: type of the stiffness
   * \param Ci : stiffness tensor of type st2tost2<3u,StressType>
   * \return a std::pair<StressType,real> (E,nu)
   */
    template<unsigned short int N,typename real,typename StressType>
    std::pair<StressType,real> computeYoungNu(const tfel::math::st2tost2<N,StressType> &Ci){
       std::pair<StressType,StressType> pair= computeKappaMu<N,real,StressType>(Ci);
       const auto kappa = std::get<0>(pair);
       const auto mu = std::get<1>(pair);
       auto E = 9 * kappa * mu / (3 * kappa + mu);
       auto nu= (3 * kappa - 2 * mu) / 2 / (3 * kappa + mu);
       return {E,nu};
      }//end of computeYoungNu


namespace homogenization::elasticity {
  
  
  /////////Definition of inclusions
  
  /*!
   * This struct defines a N (N=2 or N=3) dimensional inclusion (its childs
   * are only geometrical ellipsoids, which can be particular like spheres,
   * disks...)
   */
  template<unsigned short int N,typename LengthType>
  struct Inclusion {
    std::array<LengthType, N> semiLengths;
    Inclusion(std::array<LengthType, N> semiL): semiLengths(semiL){};
    virtual ~Inclusion(){};
  };
  
  /*!
   * This struct defines an ellipsoid.
   * \tparam unsigned short int: dimension
   * \tparam LengthType: type of the lengths
   */
  template <typename LengthType>
  struct Ellipsoid : public Inclusion<3u,LengthType>{
    
    Ellipsoid(LengthType a,LengthType b,LengthType c)
        : Inclusion<3u,LengthType>(std::array<LengthType,3u>({a,b,c})){};
        
    virtual ~Ellipsoid(){};
  };

  /*!
   * This struct defines a 3d spheroid as a child of Inclusion<3>.
   * The second semi-length is shared by two axes
   * \tparam LengthType: type of the lengths
   */
  template <typename LengthType>
  struct Spheroid : public Inclusion<3u,LengthType> {
    
    Spheroid(LengthType a,LengthType b)
        : Inclusion<3u,LengthType>(std::array<LengthType,3u>({a,b,b})){};
        
    virtual ~Spheroid(){};
  };

  /*!
   * This struct defines a 3d sphere as a child of Inclusion<3>
   */
   template <typename LengthType>
  struct Sphere : public Inclusion<3u,LengthType> {
  
    Sphere(): Inclusion<3u,LengthType>(std::array<LengthType,3u>({LengthType(1),LengthType(1),LengthType(1)})){};
    
    virtual ~Sphere(){};
  };

  /*!
   * This struct defines a 2d disk as a child of Inclusion<2>
   * \tparam LengthType: type of the radius
   */
   template <typename LengthType>
  struct Disk : public Inclusion<2u,LengthType> {
  
    Disk(): Inclusion<2u,LengthType>(std::array<LengthType,2u>({LengthType(1),LengthType(1)})){};
    
    virtual ~Disk(){};
  };
  
  /*!
   * This struct defines an ellipse
   * \tparam unsigned short int: dimension
   * \tparam LengthType: type of the lengths
   */
  template <typename LengthType>
  struct Ellipse : public Inclusion<2u,LengthType>{
    
    Ellipse(LengthType a,LengthType b)
        : Inclusion<2u,LengthType>(std::array<LengthType,2u>({a,b})){};
        
    virtual ~Ellipse(){};
  };

  
  
   /////////Definition of phases
  
   /*!
   * This struct defines a phase. A phase is defined
   * by physical information (stiffness, volume fraction).
   * \tparam unsigned short int: dimension
   * \tparam StressType: type of the elastic constants
   */
  template <unsigned short int N,typename real,typename StressType>
  struct Phase {
    real fraction;
    tfel::math::st2tost2<N, StressType> stiffness;
    // check if stiffnessTensor is symetric ?
    Phase(real f, tfel::math::st2tost2<N, StressType> &C) : fraction(f), stiffness(C){};
    virtual ~Phase(){};
  };

 
  /*!
   * GrainPhase is defined for polycrystal microstructure as a child of Phase.
   * It has an Inclusion<N>, and vectors for its orientation.
   * \f$n_a\f$ corresponds to the first semi-length and \f$n_b\f$ to the
   * second semi-length.
   * The elasticity must be expressed in the basis given by these orientation
   * vectors.
   */
  template <unsigned short int N,
            typename real,
            typename StressType,
            typename LengthType>
  struct GrainPhase : public Phase<N, real, StressType> {
    Inclusion<N,LengthType> inclusion;
    tfel::math::tvector<N,real> n_a;
    tfel::math::tvector<N,real> n_b;

    GrainPhase(real frac, tfel::math::st2tost2<N, StressType> &C, Inclusion<N,LengthType> &inc, tfel::math::tvector<N,real> &n_a_, tfel::math::tvector<N,real> &n_b_)
        : Phase<N, real, StressType>(frac, C), inclusion(inc), n_a(n_a_), n_b(n_b_){
        static constexpr auto eps = std::numeric_limits<real>::epsilon();
          if (not((n_a|n_b) < eps)) {
            tfel::reportContractViolation("n_a and n_b not normals");
          }
       };
        
    tfel::math::st2tost2<N,real> computeMeanLocalisator(const tfel::math::st2tost2<3u,StressType> &C0, bool isotropic_matrix,const int max_iter_anisotropic_integration = 12){
        tfel::math::st2tost2<N,StressType> Ci = this->stiffness;
        auto semiL= (this->inclusion).semiLengths;
        auto n_a_i=this->n_a;
        auto n_b_i=this->n_b;
        auto ai=semiL[0];
        auto bi=semiL[1];
        if (N==2){////TO DO 2D //////////////////////////////////////////////////////
            tfel::reportContractViolation("dimension 2 coming soon");
        }
        auto ci=semiL[2];
        if (isotropic_matrix){
           auto pair0=computeYoungNu<N,real,StressType>(C0);
           const auto E0 = std::get<0>(pair0);
           const auto nu0 = std::get<1>(pair0);
	// prevision of a general C_i !
	   auto pairi=computeYoungNu<N,real,StressType>(Ci);
           const auto Ei = std::get<0>(pairi);
           const auto nui = std::get<1>(pairi);
           return computeEllipsoidLocalisationTensor<StressType>(E0,nu0, Ei,nui, n_a_i, ai,n_b_i, bi, ci);
        }
        else{
           return computeAnisotropicLocalisationTensor<StressType>(C0,Ci,n_a_i,ai,n_b_i,bi,ci,max_iter_anisotropic_integration);
        }
    };
    
    virtual ~GrainPhase(){};
  };
 
   
  /////////Definition of particular phases with distributions of inclusions
 
  /*!
   * This struct defines a distribution of inclusions as an abstract
   * class, child of class Phase. It contains an inclusion and a pure virtual method
   * computeMeanLocalisator which returns the mean localisator of the
   * distribution of inclusions, for a given matrix (Phase) in parameter.
   */
   template<unsigned short int N,typename real,typename StressType,typename LengthType>
  struct InclusionDistribution : Phase<N,real,StressType> {
    Inclusion<N,LengthType> inclusion;
    InclusionDistribution(Inclusion<N,LengthType> &inc, real frac, tfel::math::st2tost2<N,StressType> &C): Phase<N,real,StressType>(frac,C), inclusion(inc){};
    virtual tfel::math::st2tost2<N,real> computeMeanLocalisator(const tfel::math::st2tost2<N,StressType> &C0, bool isotropic_matrix,const int max_iter_anisotropic_integration) = 0;
    virtual ~InclusionDistribution(){};
  };
  
  
  /*!
   * This struct defines a distribution of spheres, which is
   * a child of InclusionDistribution, with inclusion of type
   * Sphere.
   */
   template<typename real,typename StressType,typename LengthType>
  struct SphereDistribution : public InclusionDistribution<3u,real,StressType,LengthType> {
    SphereDistribution(Sphere<LengthType> &sph, real frac, tfel::math::st2tost2<3u,StressType> &Ci): InclusionDistribution<3u,real,StressType,LengthType>(sph, frac, Ci){};
    
    virtual tfel::math::st2tost2<3u,real> computeMeanLocalisator(const tfel::math::st2tost2<3u,StressType> &C0, bool isotropic_matrix,const int max_iter_anisotropic_integration = 12){
        auto Ci = this->stiffness;
        if (not(isIsotropic<3u,real,StressType>(Ci))){
        	tfel::reportContractViolation("Currently, I cannot make computation on distribution of anisotropic inclusions");
        }
        auto pairi=computeYoungNu<3u,real,StressType>(Ci);
        const auto Ei = std::get<0>(pairi);
        const auto nui = std::get<1>(pairi);
        if (isotropic_matrix){
           auto pair0=computeYoungNu<3u,real,StressType>(C0);
           const auto E0 = std::get<0>(pair0);
           const auto nu0 = std::get<1>(pair0);
           return computeSphereLocalisationTensor<StressType>(E0,nu0,Ei,nui);
        }
        else{
           tfel::math::tvector<3u,real> n_a={1.,0.,0.};
           tfel::math::tvector<3u,real> n_b={0.,1.,0.};
           return computeAnisotropicLocalisationTensor<StressType>(C0,Ci,n_a,real(1),n_b,real(1),real(1),max_iter_anisotropic_integration);
        }
    };
    
    virtual ~SphereDistribution(){};
  };
  
  
   /*!
   * This struct defines a distribution of disks, which is
   * a child of InclusionDistribution, with inclusion of type
   * Disk.
   */
   template<typename real,typename StressType,typename LengthType>
  struct DiskDistribution : public InclusionDistribution<2u,real,StressType,LengthType> {
    DiskDistribution(Disk<LengthType> &disk, real frac, tfel::math::st2tost2<2u,StressType> &Ci): InclusionDistribution<2u,real,StressType,LengthType>(disk,frac,Ci){};
    
    virtual tfel::math::st2tost2<2u,real> computeMeanLocalisator(const tfel::math::st2tost2<2u,StressType> &C0, bool isotropic_matrix,const int max_iter_anisotropic_integration = 12){
        auto Ci = this->stiffness;
        if (not(isIsotropic<2u,real,StressType>(Ci))){
              tfel::reportContractViolation("Currently, I cannot make computation on distribution of anisotropic inclusions");
        }
        auto pairi=computeYoungNu<2u,real,StressType>(Ci);
        const auto Ei = std::get<0>(pairi);
        const auto nui = std::get<1>(pairi);
        if (isotropic_matrix){
           auto pair0=computeYoungNu<2u,real,StressType>(C0);
           const auto E0 = std::get<0>(pair0);
           const auto nu0 = std::get<1>(pair0);    //TO DO /////////////////////////////////////////
           return tfel::math::st2tost2<2u,real>::Id();//computeDiskLocalisationTensor<StressType>(E0,nu0,Ei,nui);
        }
        else{
           tfel::math::tvector<2u,real> n_a={1.,0.};
           return computePlainStrainAnisotropicLocalisationTensor<StressType>(C0,Ci,n_a,real(1),real(1),max_iter_anisotropic_integration);
        }
    };
    
    virtual ~DiskDistribution(){};
  };
  
  /*!
   * This struct defines an isotropic distribution of 
   * Ellipsoid<N> or Spheroid, as a child of InclusionDistribution<N>
   */
   template<unsigned short int N,typename real,typename StressType,typename LengthType>
  struct IsotropicDistribution : public InclusionDistribution<N,real,StressType,LengthType> {
  
    IsotropicDistribution(Ellipsoid<LengthType> &ell, real frac, tfel::math::st2tost2<3u,StressType> &Ci): InclusionDistribution<N,real,StressType,LengthType>(ell,frac,Ci){};
    IsotropicDistribution(Spheroid<LengthType> &sphero, real frac, tfel::math::st2tost2<2u,StressType> &Ci): InclusionDistribution<N,real,StressType,LengthType>(sphero,frac,Ci){};
    IsotropicDistribution(Ellipse<LengthType> &ell, real frac, tfel::math::st2tost2<2u,StressType> &Ci): InclusionDistribution<N,real,StressType,LengthType>(ell,frac,Ci){};
    
    virtual tfel::math::st2tost2<N,real> computeMeanLocalisator(const tfel::math::st2tost2<N,StressType> &C0, bool isotropic_matrix = true,const int max_iter_anisotropic_integration = 12){
        tfel::math::st2tost2<N,StressType> Ci = this->stiffness;
        if (not(isIsotropic<N,real,StressType>(Ci))){
              tfel::reportContractViolation("Currently, I cannot make computation on distribution of anisotropic inclusions");
        }
        auto pairi=computeYoungNu<N,real,StressType>(Ci);
        const auto Ei = std::get<0>(pairi);
        const auto nui = std::get<1>(pairi);
        if (not(isIsotropic<N,real,StressType>(C0))){
            std::cout<<"warning: your matrix is not isotropic, and it will be made isotropic for computing the average localisator in the distribution"<<std::endl;
        }
        auto pair0=computeYoungNu<N,real,StressType>(C0);
        const auto E0 = std::get<0>(pair0);
        const auto nu0 = std::get<1>(pair0);
        if (N==2){////TO DO 2D //////////////////////////////////////////////////////
            tfel::reportContractViolation("dimension 2 coming soon");
        }
        auto semiL= (this->inclusion).semiLengths;
        auto ai=semiL[0];
        auto bi=semiL[1];
        auto ci = semiL[2];
        auto pair = EllipsoidMeanLocalisator<3u, real, StressType, LengthType>::Isotropic(E0,nu0, Ei, nui, ai, bi, ci);
        auto kappaA = std::get<0>(pair);
        auto muA = std::get<1>(pair);
        return 3*kappaA*tfel::math::st2tost2<3u,real>::J()+2*muA*tfel::math::st2tost2<3u,real>::K();
    };
    
    virtual ~IsotropicDistribution(){};
  };
  
  
  /*!
   * This struct defines a transverse isotropic distribution of inclusions
   * in 3d, which can be only Spheroid or Ellipsoid. The vector \f$n\f$
   * is the axis of transverse isotropy. The index corresponds to the index of 
   * the semi-length which corresponds to the axis of transverse isotropy (which does
   * not rotate).
   */
   template<typename real,typename StressType,typename LengthType>
  struct TransverseIsotropicDistribution : public InclusionDistribution<3u,real,StressType,LengthType> {
    
    tfel::math::tvector<3u,real> n;
    unsigned short int index;
    
    TransverseIsotropicDistribution(Ellipsoid<LengthType> &ell, real frac, tfel::math::st2tost2<3u,StressType> &C,tfel::math::tvector<3u,real> &n_, unsigned short int &ind): InclusionDistribution<3u,real,StressType,LengthType>(ell,frac,C), n(n_), index(ind){
    ///////WARNING index
    };
    
    TransverseIsotropicDistribution(Spheroid<LengthType> &sphero, real frac, tfel::math::st2tost2<3u,StressType> &C,tfel::math::tvector<3u,real> &n_, unsigned short int &ind): InclusionDistribution<3u,real,StressType,LengthType>(sphero,frac,C), n(n_), index(ind){
    ///////WARNING index
    };
    
    virtual tfel::math::st2tost2<3u,real> computeMeanLocalisator(const tfel::math::st2tost2<3u,StressType> &C0, bool isotropic_matrix,const int max_iter_anisotropic_integration = 12){
        tfel::math::st2tost2<3u,StressType> Ci = this->stiffness;
        if (not(isIsotropic<3u,real,StressType>(Ci))){
              tfel::reportContractViolation("Currently, I cannot make computation on distribution of anisotropic inclusions");
        }
        auto pairi=computeYoungNu<3u,real,StressType>(Ci);
        const auto Ei = std::get<0>(pairi);
        const auto nui = std::get<1>(pairi);
        if (not(isIsotropic<3u,real,StressType>(C0))){
            std::cout<<"warning: your matrix is not isotropic, and it will be made isotropic for computing the average localisator in the distribution"<<std::endl;
        }
        auto pair0=computeYoungNu<3u,real,StressType>(C0);
        const auto E0 = std::get<0>(pair0);
        const auto nu0 = std::get<1>(pair0);
        auto semiL= (this->inclusion).semiLengths;
        auto ind=this->index;
        auto ai=semiL[ind];
        LengthType bi;
        LengthType ci;
        if (ind==0){
          bi=semiL[1];
          ci = semiL[2];
        }
        if (ind==1){
          bi=semiL[0];
          ci=semiL[2];
        }
        if (ind==2){
          bi=semiL[0];
          ci=semiL[1];
        }
        return EllipsoidMeanLocalisator<3u, real, StressType, LengthType>::TransverseIsotropic(E0,nu0, Ei, nui, this->n, ai, bi, ci);
    };
    
    virtual ~TransverseIsotropicDistribution(){};
  };
  
   /*!
   * This struct defines a distribution of inclusions with a unique orientation
   * in 2d or 3d, which can be only Spheroid or Ellipsoid or Ellipse. The vectors \f$n_a\f$
   * and \f$n_b\f$ define the orientation. The first semi-length of the inclusion
   * corresponds to \f$n_a\f$. The second semi-length corresponds to
   * \f$n_b\f$.
   */
   template<unsigned short int N, typename real,typename StressType,typename LengthType>
  struct OrientedDistribution : public InclusionDistribution<N,real,StressType,LengthType> {
  
    tfel::math::tvector<N,real> n_a;
    tfel::math::tvector<N,real> n_b;
    
    OrientedDistribution(Ellipsoid<LengthType> &ell,real frac, tfel::math::st2tost2<3u,StressType> &C,tfel::math::tvector<3u,real> &n_a_,tfel::math::tvector<3u,real> &n_b_): InclusionDistribution<3u,real,StressType,LengthType>(ell,frac,C), n_a(n_a_), n_b(n_b_) {
       if (not(tfel::math::ieee754::fpclassify(
                  n_a|n_b) == FP_ZERO)) {
         tfel::reportContractViolation("n_a and n_b not normals");
       }
    };
    
    OrientedDistribution(Ellipse<LengthType> &ell,real frac, tfel::math::st2tost2<2u,StressType> &C,tfel::math::tvector<2u,real> &n_a_,tfel::math::tvector<2u,real> &n_b_): InclusionDistribution<2u,real,StressType,LengthType>(ell,frac,C), n_a(n_a_), n_b(n_b_) {
       if (not(tfel::math::ieee754::fpclassify(
                  n_a|n_b) == FP_ZERO)) {
         tfel::reportContractViolation("n_a and n_b not normals");
       }
    };
    
    OrientedDistribution(Spheroid<LengthType> &sphero,real frac, tfel::math::st2tost2<3u,StressType> &C,tfel::math::tvector<3u,real> &n_a_,tfel::math::tvector<3u,real> &n_b_): InclusionDistribution<3u,real,StressType,LengthType>(sphero,frac,C), n_a(n_a_), n_b(n_b_) {
       if (not(tfel::math::ieee754::fpclassify(
                  n_a|n_b) == FP_ZERO)) {
         tfel::reportContractViolation("n_a and n_b not normals");
       }
    };
    
    virtual tfel::math::st2tost2<N,real> computeMeanLocalisator(const tfel::math::st2tost2<N,StressType> &C0, bool isotropic_matrix,const int max_iter_anisotropic_integration = 12){
        tfel::math::st2tost2<N,StressType> Ci = this->stiffness;
        auto semiL= (this->inclusion).semiLengths;
        auto n_a_i=this->n_a;
        auto n_b_i=this->n_b;
        auto ai=semiL[0];
        auto bi=semiL[1];
        if (N==2){////TO DO 2D //////////////////////////////////////////////////////
            tfel::reportContractViolation("dimension 2 coming soon");
        }
        auto ci=semiL[2];
        if (isotropic_matrix){
           auto pair0=computeYoungNu<N,real,StressType>(C0);
           const auto E0 = std::get<0>(pair0);
           const auto nu0 = std::get<1>(pair0);
           return computeEllipsoidLocalisationTensor<StressType>(E0,nu0, Ci, n_a_i, ai,n_b_i, bi, ci);
        }
        else{
           computeAnisotropicLocalisationTensor<StressType>(C0,Ci,n_a_i,ai,n_b_i,bi,ci,max_iter_anisotropic_integration);
        }
    };
    
    virtual ~OrientedDistribution(){};
  };

   
   /////////Definition of microstructures
   
  /*!
   * This struct defines a microstructure.
   * \tparam unsigned short int: dimension
   * \tparam real: underlying type
   * \tparam LengthType: type of the lengths related to the inclusion
   * \tparam StressType: type of the elastic constants related to the inclusion
   */
  template <unsigned short int N,
            typename real,
            typename LengthType,
            typename StressType>
  struct Microstructure {
     Microstructure(){};
     virtual ~Microstructure(){};
  };

  /*!
   * This struct defines a matrix-inclusion microstructure as a child of
   * Microstructure. This type of microstructure is viewed as an array of
   * InclusionDistribution, with a matrixPhase.
   * \tparam unsigned short int: dimension
   * \tparam real: underlying type
   * \tparam LengthType: type of the lengths related to the inclusion
   * \tparam StressType: type of the elastic constants related to the inclusion
   */
  template <unsigned short int N,
            typename real,
            typename LengthType,
            typename StressType>
  struct MatrixInclusionMicrostructure
      : public Microstructure<N, real, LengthType, StressType> {
      //attributes below
      
    MatrixInclusionMicrostructure(tfel::math::st2tost2<N,StressType> &C0)
        : Microstructure<N, real, LengthType, StressType>(),
          number_of_phases(1),
          inclusionPhases(std::vector<InclusionDistribution<N, real, StressType,LengthType>*>({})),
          matrixPhase(Phase<N,real,StressType>(real(1),C0)){};

    virtual ~MatrixInclusionMicrostructure(){};

    int addInclusionPhase(
        InclusionDistribution<N, real, StressType,LengthType> &inclusionPhase) {
      if (this->matrixPhase.fraction - inclusionPhase.fraction < real(0)) {
        std::cout << "the volume fraction of inclusions is too high !"
                  << std::endl;
        return 0;
      } else {
        (this->number_of_phases)++;
        (this->matrixPhase.fraction) -= inclusionPhase.fraction;
        (this->inclusionPhases).push_back(&inclusionPhase);
        return 1;
      };
    };

    int removeInclusionPhase(unsigned int i) {
      if ((this->number_of_phases) == 1) {
        std::cout << "there are no more inclusions !" << std::endl;
        return 0;
      } else if ((this->number_of_phases) < i + 2) {
        std::cout << "there are less phases than what you think !" << std::endl;
        return 0;
      } else {
        if ((this->number_of_phases) == 2) {
          std::cout << "you have removed the last inclusion phase !"
                    << std::endl;
        };
        (this->number_of_phases)--;
        (this->matrixPhase.fraction) -= *(this->inclusionPhases[i]).fraction;
        (this->inclusionPhases).erase((this->inclusionPhases).begin() + i);
        return 1;
      };
    };

    unsigned int get_number_of_phases() { return (this->number_of_phases); };

    real get_matrix_fraction() { return (this->matrixPhase.fraction); };

    tfel::math::st2tost2<N,StressType> get_matrix_elasticity() {
      return (this->matrixPhase.stiffness);
    };

    InclusionDistribution<N, real, StressType,LengthType> * get_inclusionPhase(
        unsigned int i) {
      if ((this->number_of_phases) < i + 2) {
        tfel::reportContractViolation(
            "there are less phases than what you think !");
      }
      return (this->inclusionPhases)[i];
    };

   private:
    unsigned int number_of_phases;
    std::vector<InclusionDistribution<N, real, StressType,LengthType>*> inclusionPhases;
    Phase<N, real, StressType> matrixPhase;
  };

  /*!
   * This struct defines a polycrystal as a child of Microstructure. A
   * polycrystal is viewed as a vector of Grain.
   * \tparam unsigned short int: dimension
   * \tparam real: underlying type
   * \tparam LengthType: type of the lengths related to the inclusion
   * \tparam StressType: type of the elastic constants related to the inclusion
   */
  template <unsigned short int N,
            typename real,
            typename LengthType,
            typename StressType>
  struct Polycrystal : public Microstructure<N, real, LengthType, StressType> {
     //attributes below
     
    Polycrystal()
        : Microstructure<N, real, LengthType, StressType>(),
          number_of_grains(0),
          total_fraction(real(0)),
          grains(std::vector<GrainPhase<N, real, StressType,LengthType>*>({})){};

    virtual ~Polycrystal(){};

    int addGrain(GrainPhase<N, real, StressType,LengthType> &grain) {
      if ((this->total_fraction) + grain.fraction > real(1)) {
        std::cout << "the volume fraction is too high !" << std::endl;
        return 0;
      } else {
        (this->number_of_grains)++;
        (this->total_fraction) += grain.fraction;
        (this->grains).push_back(&grain);
        return 1;
      };
    };

    int removeGrain(unsigned int i) {
      if ((this->number_of_grains) == 0) {
        std::cout << "there are no more grains !" << std::endl;
        return 0;
      } else if ((this->number_of_grains) < i + 1) {
        std::cout << "there are less grains than what you think !" << std::endl;
        return 0;
      } else {
        if ((this->number_of_grains) == 1) {
          std::cout << "you have removed the last grain !" << std::endl;
        };
        (this->number_of_grains)--;
        (this->total_fraction) -= *(this->grains[i]).fraction;
        (this->grains).erase((this->grains).begin() + i);
      };
      return 1;
    };

    unsigned int get_number_of_grains() { return (this->number_of_grains); };

    real get_total_fraction() { return (this->total_fraction); };

    GrainPhase<N, real, StressType,LengthType> * get_grain(unsigned int i) {
      if ((this->number_of_grains) < i + 1) {
        std::cout<<this->number_of_grains<< " et vous voulez " << i<<std::endl;
        tfel::reportContractViolation(
            "there are less grains than what you think !");
      }
      return (this->grains)[i];
    };

   private:
    unsigned int number_of_grains;
    real total_fraction;
    std::vector<GrainPhase<N, real, StressType,LengthType>*> grains;
  };
  
 }//end of homogenization::elasticity

}  // end of namespace tfel::material

#endif /* LIB_TFEL_MATERIAL_MICROSTRUCTUREDESCRIPTION_HXX */
