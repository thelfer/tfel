/*! 
 * \file  DDIF2Base.ixx
 * \brief
 * \author Helfer Thomas
 * \brief 12 mar 2010
 */

#ifndef _LIB_TFEL_MATERIAL_DDIF2BASE_I_
#define _LIB_TFEL_MATERIAL_DDIF2BASE_I_ 

#include<cmath>
#include<algorithm>

namespace tfel
{

  namespace material
  {
    
    template<typename Stensor2Type1,
	     typename Stensor,
	     typename real>
    void
    DDIF2Base::treatFracture(Stensor2Type1& dfe_ddeel,
			     real& dfe_dde,
			     real& fe,
			     const real em_1,
			     const real e,
			     const real de,
			     const Stensor& sig,
			     const Stensor& n,
			     const real sigr,
			     const real Rp,
			     const real young,
			     const real lambda,
			     const real mu,
			     const unsigned short i)
    {
      using namespace std;
      const real emin1 = 1.e-7;    /* small parameter which guarantees
				      that Ef (defined below) is finite */
      const real eps  = 1.e-10;
      const real em   = max(em_1,e+de); 
      const real sigm = max(sigr+Rp*em,0.); 
      // loading surface
      const real r = (sig(i)-sigm)/young;
      if (((r>eps)||(e+de>em_1))&&(e+de>=0.)){
	// damage increase
	fe        = r;
	if(sigr+Rp*em>0.){
	  dfe_dde   = -Rp/young;
	} else {
	  dfe_dde   = 0;
	}
	dfe_ddeel = (lambda*Stensor::Id()+2*mu*n)/young;
      } else {
	// no damage increase
	if(e+de<0){
	    fe      += e;
	} else {
	  if(em>emin1){
	    // material previously damage increase
	    if((e+de>emin1)||(sig(i)>=0)){
	      const real Ef  = sigm/em;
	      fe             = (sig(i)-Ef*(e+de))/young;
	      dfe_ddeel      = (lambda*Stensor::Id()+2*mu*n)/young;
	      dfe_dde        = -Ef/young;
	    } else {
	      fe      += e;
	    }
	  } else {
	    fe      += e;
	  }
	}
      }  
    } // end of DDIF2Base::treatFacture

    template<typename real>
    void
    DDIF2Base::cart2cyl(tfel::math::stensor<1u,real>&,
			const real)
    {} // end of DDIF2Base::cart2cyl
    
    template<typename real>
    void
    DDIF2Base::cart2cyl(tfel::math::stensor<2u,real>& s,
			const real theta)
    {
      using namespace std;
      const real a[4]  = {s(0),s(1),s(2),s(3)*M_SQRT1_2};
      const real cost  = cos(theta);
      const real sint  = sin(theta);
      const real alpha = cost*a[0]+sint*a[3];
      const real beta  = cost*a[3]+sint*a[1];
      s(0) = cost*alpha+sint*beta;
      s(1) = sint*(sint*a[0]-cost*a[3])+cost*(cost*a[1]-sint*a[3]);
      s(2) = a[2];
      s(3) = (cost*beta-sint*alpha)*M_SQRT2;
    } // end of DDIF2Base::cart2cyl
      
    template<typename real>
    void
    DDIF2Base::cart2cyl(tfel::math::stensor<3u,real>& s,
			const real theta)
    {
      using namespace std;
      const real a[6]  = {s(0),s(1),s(2),s(3)*M_SQRT1_2,
			  s(4)*M_SQRT1_2,s(5)*M_SQRT1_2};
      const real cost  = cos(theta);
      const real sint  = sin(theta);
      const real alpha = cost*a[0]+sint*a[3];
      const real beta  = cost*a[3]+sint*a[1];
      s(0) = cost*alpha+sint*beta;
      s(1) = sint*(sint*a[0]-cost*a[3])+cost*(cost*a[1]-sint*a[3]);
      s(2) = a[2];
      s(3) = (cost*beta-sint*alpha)*M_SQRT2;
      s(4) = (cost*a[4]+sint*a[5])*M_SQRT2;
      s(5) = (cost*a[5]-sint*a[4])*M_SQRT2;
    } // end of DDIF2Base::cart2cyl

    template<typename real>
    void
    DDIF2Base::cyl2cart(tfel::math::stensor<1u,real>&,
			const real)
    {} // end of DDIF2Base::cyl2cart
    
    template<typename real>
    void
    DDIF2Base::cyl2cart(tfel::math::stensor<2u,real>& s,
			const real theta)
    {
      using namespace std;
      const real a[4]  = {s(0),s(1),s(2),s(3)*M_SQRT1_2};
      const real cost  = cos(theta);
      const real sint  = sin(theta);
      const real alpha = cost*a[0]-sint*a[3];
      const real beta  = cost*a[3]-sint*a[1];
      s(0) = cost*alpha-sint*beta;
      s(1) = sint*(sint*a[0]+cost*a[3])+cost*(sint*a[3]+cost*a[1]);
      s(2) = a[2];
      s(3) = (sint*alpha+cost*beta)*M_SQRT2;
    } // end of DDIF2Base::cyl2cart
      
    template<typename real>
    void
    DDIF2Base::cyl2cart(tfel::math::stensor<3u,real>& s,
			const real theta)
    {
      using namespace std;
      using namespace std;
      const real a[6]  = {s(0),s(1),s(2),s(3)*M_SQRT1_2,
			  s(4)*M_SQRT1_2,s(5)*M_SQRT1_2};
      const real cost  = cos(theta);
      const real sint  = sin(theta);
      const real alpha = cost*a[0]-sint*a[3];
      const real beta  = cost*a[3]-sint*a[1];
      s(0) = cost*alpha-sint*beta;
      s(1) = sint*(sint*a[0]+cost*a[3])+cost*(sint*a[3]+cost*a[1]);
      s(2) = a[2];
      s(3) = (sint*alpha+cost*beta)*M_SQRT2;
      s(4) = (cost*a[4]-sint*a[5])*M_SQRT2;
      s(5) = (sint*a[4]+cost*a[5])*M_SQRT2;
    } // end of DDIF2Base::cyl2cart
    

    template<typename real>
    void
    DDIF2Base::rk(tfel::math::tmatrix<3,3,real>& m,
		  tfel::math::tvector<3,real>& v,
		  const real s,
		  const real dse, // elastic stress
		  const real e,
		  const real em,
		  const real sigr,
		  const real Rp,
		  const real young,
		  const real lambda,
		  const real mu,
		  const unsigned short i,
 		  const unsigned short i1,
		  const unsigned short i2)
    {
      using namespace std;
      const real emin1 = 1.e-8;    /* small parameter which guarantees
				      that Ef (defined below) is finite */
      const real sigm = max(sigr+Rp*young*max(em,e),0.); 
      real Rp1   = 0.;
      if(sigm>0.) 
      	Rp1 = Rp;
      const real r    = max(s-sigm,0.)/young;
      if(r>0){
	// loading
	m(i,i1) = m(i,i2) = lambda;
	m(i,i)  = lambda+2*mu+Rp1;
	v(i)    = dse;
      } else {
	if(((s>=0)||(e>0))&&(em>emin1)){
	  const real Ef  = (sigm/young)/(max(em,e));
	  m(i,i1) = m(i,i2) = lambda;
	  m(i,i)  = lambda+2*mu+Ef;
	  v(i)    = dse;
	} else {
	  m(i,i1) = m(i,i2) = 0.;
	  m(i,i)  = 1.;
	  v(i)    = 0.;
	}
      }
    }

    template<typename real>
    void
    DDIF2Base::rkbm(tfel::math::tmatrix<3,3,real>& m,
		  tfel::math::tvector<3,real>& v,
		  const real s,
		  const real dse, // elastic stress
		  const real e,
		  real& Hr,
		  real X,
		  const real Hf,
		  const real young,
		  const real lambda,
		  const real mu,
		  const unsigned short i,
 		  const unsigned short i1,
		  const unsigned short i2)
    {
      using namespace std;
      const real emin1 = 1.e-8;    /* small parameter which guarantees
				      that Ef (defined below) is finite */
      real r = (s/young-X);
      if (X<0)
	 Hr=0.;
//      cout<<" r*yg "<<r*young<<" e "<<e<<endl;
      if((r>0)&&(e>=-emin1)){
	// loading
	m(i,i1) = m(i,i2) = lambda;
	m(i,i)  = lambda+2*mu+Hr;
	v(i)    = dse;
      } else {
      if(e>emin1){
	Hr=0.;
	const real Ef  = (s/young)/e;
	m(i,i1) = m(i,i2) = lambda;
	m(i,i)  = lambda+2*mu+Ef;
	v(i)	= dse;
      } else {
	  m(i,i1) = m(i,i2) = 0.;
	  m(i,i)  = 1.;
	  v(i)    = 0.;
	}
      }
    }

    template<typename real>
    void
    DDIF2Base::rk2(real& de,
		   const real s,
		   const real ds,
		   const real e,
		   const real em,
		   const real sigr,
		   const real Rp,
		   const real young,
		   const real lambda,
		   const real mu,
		   const real dt)
    {
      using namespace std;
      const real emin1 = 1.e-8;    /* small parameter which guarantees
				      that Ef (defined below) is finite */
      const real sigm = max(sigr+Rp*young*max(em,e),0.); 
      const real r    = max(s-sigm,0.);
      if((r>0)&&(ds<=0)){
	de = ds/Rp;
      } else {
	if(((s>=0)||(e>0))&&(em>emin1)){
	  if(sigm>emin1*young){
	    const real Ef  = (sigm/young)/(max(em,e));
	    de = ds/Ef;
	  }
	} else {
	  de = 0;
	}
      }
    }

  } // end of namespace material
    
} // end of namespace tfel

#endif /* _LIB_TFEL_MATERIAL_DDIF2BASE_I */

