/*!
 * \file   StensorComputeEigenVectors.hxx
 * \brief    
 * \author Helfer Thomas
 * \date   13 Jul 2006
 */

#ifndef _LIB_TFEL_STENSORCOMPUTEEIGENVECTORS_H_
#define _LIB_TFEL_STENSORCOMPUTEEIGENVECTORS_H_ 

#include<cmath>
#include<cassert>
#include<algorithm>

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/Math/General/CubicRoots.hxx"
#include"TFEL/Math/tvector.hxx"
#include"TFEL/Math/tmatrix.hxx"
#include"TFEL/Math/stensor.hxx"

#ifndef M_SQRT2
#define M_SQRT2 1.41421356237309504880
#endif

namespace tfel{

  namespace math {

    namespace internals{

      template<unsigned short N>
      class StensorComputeEigenVectors_;

      template<>
      class StensorComputeEigenVectors_<1u>
      {
      public:
	template<typename T>
	static bool exe(const T* const s,tfel::math::tvector<3u,T>& vp,tfel::math::tmatrix<3u,3u,T>& vec)
	{
	  TFEL_STATIC_ASSERT(tfel::typetraits::IsFundamentalNumericType<T>::cond);
	  TFEL_STATIC_ASSERT(tfel::typetraits::IsReal<T>::cond);

	  vp(0)=s[0];
	  vp(1)=s[1];
	  vp(2)=s[2];
	  vec = tfel::math::tmatrix<3u,3u,T>::Id();
	  return true;
	}
      };

      template<>
      class StensorComputeEigenVectors_<2u>
      {

	template<typename T>
	static bool test(const T* const s,const tfel::math::tvector<3u,T>& vp,const tfel::math::tmatrix<3u,3u,T>& m)
	{
	  TFEL_STATIC_ASSERT(tfel::typetraits::IsFundamentalNumericType<T>::cond);
	  TFEL_STATIC_ASSERT(tfel::typetraits::IsReal<T>::cond);

	  static const T M1_sqrt2  = 1.f/std::sqrt(static_cast<T>(2.)); 
	  
	  T tmp;
	  T y0;
	  T y1;

	  // first eigenvalue
	  y0 = s[0]*m(0,0)+s[3]*M1_sqrt2*m(1,0)-vp(0)*m(0,0);
	  y1 = s[1]*m(1,0)+s[3]*M1_sqrt2*m(0,0)-vp(0)*m(1,0);

	  tmp = std::sqrt(y0*y0+y1*y1);
	  if(tmp>100*std::numeric_limits<T>::epsilon()){
	    TFEL_UTILITIES_INFO("first eigenvalue false : " << tmp);
	    TFEL_UTILITIES_INFO("s = " );
	    TFEL_UTILITIES_INFO(s);
	    TFEL_UTILITIES_INFO("vp = " );
	    TFEL_UTILITIES_INFO(vp);
	    TFEL_UTILITIES_INFO("m = " );
	    TFEL_UTILITIES_INFO(m);
	    return false;
	  }
  
	  // second eigenvalue
	  y0 = s[0]*m(0,1)+s[3]*M1_sqrt2*m(1,1)-vp(1)*m(0,1);
	  y1 = s[1]*m(1,1)+s[3]*M1_sqrt2*m(0,1)-vp(1)*m(1,1);

	  tmp = std::sqrt(y0*y0+y1*y1);
	  if(tmp>100*std::numeric_limits<T>::epsilon()){
	    TFEL_UTILITIES_INFO("second eigenvalue false : " << tmp);
	    TFEL_UTILITIES_INFO("s = " );
	    TFEL_UTILITIES_INFO(s);
	    TFEL_UTILITIES_INFO("vp = " );
	    TFEL_UTILITIES_INFO(vp);
	    TFEL_UTILITIES_INFO("m = " );
	    TFEL_UTILITIES_INFO(m);
	    return false;
	  }

	  return true;
	}

	template<typename T>
	static void computeEigenValues(const T s0,const T s1,const T s3,T& vp1,T& vp2)
	{
	  TFEL_STATIC_ASSERT(tfel::typetraits::IsFundamentalNumericType<T>::cond);
	  TFEL_STATIC_ASSERT(tfel::typetraits::IsReal<T>::cond);

	  T tr    = s0 + s1;
	  T tmp   = s0 - s1;
	  T delta = tmp*tmp*0.25f+0.5f*s3*s3;
	  vp1 = 0.5f*tr+std::sqrt(delta); 
	  vp2 = 0.5f*tr-std::sqrt(delta); 
	}

	template<typename T>
	static bool computeEigenVector(const T s_0,const T s_1,const T s3,const T& vp,T& x,T& y)
	{
	  TFEL_STATIC_ASSERT(tfel::typetraits::IsFundamentalNumericType<T>::cond);
	  TFEL_STATIC_ASSERT(tfel::typetraits::IsReal<T>::cond);

	  static const T M_sqrt2 = std::sqrt(static_cast<T>(2.)); 

	  T s0 = s_0 - vp;
	  T s1 = s_1 - vp;
	  if(std::abs(s0)>std::abs(s1)){
	    if(std::abs(s0)<100*std::numeric_limits<T>::min()){
	      TFEL_UTILITIES_INFO("s0 is too small");
	      return false;
	    }
	    y=static_cast<T>(1.);
	    x=-s3/(M_sqrt2*s0);
	  } else {
	    if(std::abs(s1)<100*std::numeric_limits<T>::min()){
	      TFEL_UTILITIES_INFO("s1 is too small");
	      return false;
	    }
	    x=static_cast<T>(1.);
	    y=-s3/(M_sqrt2*s1);	    
	  }
	  s0 = std::sqrt(x*x+y*y);
	  if(s0<100*std::numeric_limits<T>::min()){
	    TFEL_UTILITIES_INFO("std::sqrt(x*x+y*y) is too small");
	    return false;
	  }
	  x/=s0;
	  y/=s0;
	  return true;
	}


      public:
	template<typename T>
	static bool exe(const T* const s,tfel::math::tvector<3u,T>& vp,tfel::math::tmatrix<3u,3u,T>& vec)
	{
	  TFEL_STATIC_ASSERT(tfel::typetraits::IsFundamentalNumericType<T>::cond);
	  TFEL_STATIC_ASSERT(tfel::typetraits::IsReal<T>::cond);

	  T prec;

	  vp(2)=s[2];
	  vec(2,0)=static_cast<T>(0.);
	  vec(2,1)=static_cast<T>(0.);
	  vec(0,2)=static_cast<T>(0.);
	  vec(1,2)=static_cast<T>(0.);
	  vec(2,2)=static_cast<T>(1.);
	  StensorComputeEigenVectors_<2u>::computeEigenValues(s[0],s[1],s[3],vp(0),vp(1));

	  prec = 10*std::max(vp(0),vp(1))*std::numeric_limits<T>::epsilon();
	  if(std::abs(vp(0)-vp(1))<=prec){
	    // The two eigenvalues are equal
	    vec(0,0)=static_cast<T>(1.);
	    vec(0,1)=static_cast<T>(0.);
	    vec(1,0)=static_cast<T>(0.);
	    vec(1,1)=static_cast<T>(1.);
	    return true;
	  }

	  if(StensorComputeEigenVectors_<2u>::computeEigenVector(s[0],s[1],s[3],vp(0),vec(0,0),vec(1,0))==false){
	    return false;
	  }
	  vec(0,1)=-vec(1,0);
	  vec(1,1)=vec(0,0);
#ifdef PARANOIC_CHECK
	    return tfel::math::internals::StensorComputeEigenVectors_<2u>::test(s,vp,vec);
#else
	    return true;
#endif
	}
      };

      template<>
      class StensorComputeEigenVectors_<3u>
      {

	template<typename T>
	static T conditionning_number(const T a, const T b, const T c)
	{
	  TFEL_STATIC_ASSERT(tfel::typetraits::IsFundamentalNumericType<T>::cond);
	  TFEL_STATIC_ASSERT(tfel::typetraits::IsReal<T>::cond);

	  T tmp  = a+b;
	  T tmp2 = a-b;

	  T X = static_cast<T>(0.5)*(tmp);
	  T Y = static_cast<T>(0.5)*std::sqrt(tmp2*tmp2+2*c*c);
    
	  tmp  = std::abs(X+Y);
	  tmp2 = std::abs(X-Y);

	  if(tmp>tmp2){
	    if(tmp2<100.*std::numeric_limits<T>::min()){
	      return std::numeric_limits<T>::max();
	    }
	    return tmp/tmp2;
	  } else {
	    if(tmp<100.*std::numeric_limits<T>::min()){
	      return std::numeric_limits<T>::max();
	    }
	    return tmp2/tmp;
	  }
	}

	template<typename T>
	static void solve2D(const T a,const T b,const T c,const T u,const T v,T& x,T& y)
	{
	  TFEL_STATIC_ASSERT(tfel::typetraits::IsFundamentalNumericType<T>::cond);
	  TFEL_STATIC_ASSERT(tfel::typetraits::IsReal<T>::cond);

	  assert(std::abs(2.*a*b-c*c)>10*std::numeric_limits<double>::min());
	  T A       = static_cast<T>(M_SQRT2)*a;
	  T B       = static_cast<T>(M_SQRT2)*b;
	  T inv_det = (static_cast<T>(1.))/(A*B-c*c);

	  x = inv_det*(B*u-c*v);
	  y = inv_det*(A*v-c*u);

	}

	template<typename T>
	static T norm(const T& x0,const T& x1,const T& x2)
	{
	  TFEL_STATIC_ASSERT(tfel::typetraits::IsFundamentalNumericType<T>::cond);
	  TFEL_STATIC_ASSERT(tfel::typetraits::IsReal<T>::cond);

	  return std::sqrt(x0*x0+x1*x1+x2*x2);
	}

	template<typename T>
	static T norm2(const T x0,const T x1,const T x2)
	{
	  TFEL_STATIC_ASSERT(tfel::typetraits::IsFundamentalNumericType<T>::cond);
	  TFEL_STATIC_ASSERT(tfel::typetraits::IsReal<T>::cond);

	  return x0*x0+x1*x1+x2*x2;
	}


	template<typename T>
	static void cross_product(T& z0,T& z1,T& z2,const T x0,const T x1,const T x2,const T y0,const T y1,const T y2)
	{
	  TFEL_STATIC_ASSERT(tfel::typetraits::IsFundamentalNumericType<T>::cond);
	  TFEL_STATIC_ASSERT(tfel::typetraits::IsReal<T>::cond);

	  z0 = x1*y2-x2*y1;
	  z1 = x2*y0-x0*y2;
	  z2 = x0*y1-x1*y0;
	}

	template<typename T>
	static void find_perpendicular_vector(T& y0,T& y1,T& y2,const T x0,const T x1,const T x2)
	{
	  TFEL_STATIC_ASSERT(tfel::typetraits::IsFundamentalNumericType<T>::cond);
	  TFEL_STATIC_ASSERT(tfel::typetraits::IsReal<T>::cond);

	  T norm2_x = norm2(x0,x1,x2);
	  T tmp;

	  if(norm2_x<100*std::numeric_limits<T>::min()){
	    //x is null
	    y0 = static_cast<T>(1.);
	    y1 = static_cast<T>(0.);
	    y2 = static_cast<T>(0.);
	    return;
	  }

	  if(std::abs(x0)<std::abs(x1)){
	    if(std::abs(x0)<std::abs(x2)){
	      //|x0| is min, (1 0 0) is a good choice
	      y0 = static_cast<T>(1.)- x0*x0/norm2_x;
	      y1 =                   - x0*x1/norm2_x;
	      y2 =                   - x0*x2/norm2_x;
	    } else {
	      //|x2| is min, (0 0 1) is a good choice
	      y0 =                   - x2*x0/norm2_x;
	      y1 =                   - x2*x1/norm2_x;
	      y2 = static_cast<T>(1.)- x2*x2/norm2_x;
	    }
	  } else if (std::abs(x1)<std::abs(x2)) {
	    // |x1| is min, (0 0 1) is a good choice
	    y0 =                   - x1*x0/norm2_x;
	    y1 = static_cast<T>(1.)- x1*x1/norm2_x;
	    y2 =                   - x1*x2/norm2_x;
	  } else {
	    // |x2| is min, (0 0 1) is a good choice
	    y0 =                   - x2*x0/norm2_x;
	    y1 =                   - x2*x1/norm2_x;
	    y2 = static_cast<T>(1.)- x2*x2/norm2_x;
	  }

	  tmp = norm(y0,y1,y2);
	  y0 /=tmp;
	  y1 /=tmp;
	  y2 /=tmp; 

	}

	template<typename T>
	static bool test(const T* const s,const tfel::math::tvector<3u,T>& vp,const tfel::math::tmatrix<3u,3u,T>& m)
	{
	  TFEL_STATIC_ASSERT(tfel::typetraits::IsFundamentalNumericType<T>::cond);
	  TFEL_STATIC_ASSERT(tfel::typetraits::IsReal<T>::cond);

	  static const T M1_sqrt2  = 1.f/std::sqrt(static_cast<T>(2.)); 
	  T y0,y1,y2;
	  T tmp;

	  // first eigenvalue
	  y0 = s[0]*m(0,0)+s[3]*M1_sqrt2*m(1,0)+s[4]*M1_sqrt2*m(2,0)-vp(0)*m(0,0);
	  y1 = s[1]*m(1,0)+s[3]*M1_sqrt2*m(0,0)+s[5]*M1_sqrt2*m(2,0)-vp(0)*m(1,0);
	  y2 = s[2]*m(2,0)+s[4]*M1_sqrt2*m(0,0)+s[5]*M1_sqrt2*m(1,0)-vp(0)*m(2,0);

	  tmp = norm(y0,y1,y2);
	  if(tmp>100*std::numeric_limits<T>::epsilon()){
	    TFEL_UTILITIES_INFO("first eigenvalue false : " << tmp);
	    TFEL_UTILITIES_INFO("s = " );
	    TFEL_UTILITIES_INFO(s);
	    TFEL_UTILITIES_INFO("vp = " );
	    TFEL_UTILITIES_INFO(vp);
	    TFEL_UTILITIES_INFO("m = " );
	    TFEL_UTILITIES_INFO(m);
	    return false;
	  }
  
	  // second eigenvalue
	  y0 = s[0]*m(0,1)+s[3]*M1_sqrt2*m(1,1)+s[4]*M1_sqrt2*m(2,1)-vp(1)*m(0,1);
	  y1 = s[1]*m(1,1)+s[3]*M1_sqrt2*m(0,1)+s[5]*M1_sqrt2*m(2,1)-vp(1)*m(1,1);
	  y2 = s[2]*m(2,1)+s[4]*M1_sqrt2*m(0,1)+s[5]*M1_sqrt2*m(1,1)-vp(1)*m(2,1);

	  tmp = norm(y0,y1,y2);
	  if(tmp>100*std::numeric_limits<T>::epsilon()){
	    TFEL_UTILITIES_INFO("second eigenvalue false : " << tmp);
	    TFEL_UTILITIES_INFO("s = " );
	    TFEL_UTILITIES_INFO(s);
	    TFEL_UTILITIES_INFO("vp = " );
	    TFEL_UTILITIES_INFO(vp);
	    TFEL_UTILITIES_INFO("m = " );
	    TFEL_UTILITIES_INFO(m);
	    return false;
	  }

	  // third eigenvalue
	  y0 = s[0]*m(0,2)+s[3]*M1_sqrt2*m(1,2)+s[4]*M1_sqrt2*m(2,2)-vp(2)*m(0,2);
	  y1 = s[1]*m(1,2)+s[3]*M1_sqrt2*m(0,2)+s[5]*M1_sqrt2*m(2,2)-vp(2)*m(1,2);
	  y2 = s[2]*m(2,2)+s[4]*M1_sqrt2*m(0,2)+s[5]*M1_sqrt2*m(1,2)-vp(2)*m(2,2);

	  tmp = norm(y0,y1,y2);
	  if(tmp>1000*std::numeric_limits<T>::epsilon()){
	    TFEL_UTILITIES_INFO("third eigenvalue false : " << tmp);
	    TFEL_UTILITIES_INFO("s = " );
	    TFEL_UTILITIES_INFO(s);
	    TFEL_UTILITIES_INFO("vp = " );
	    TFEL_UTILITIES_INFO(vp);
	    TFEL_UTILITIES_INFO("m = " );
	    TFEL_UTILITIES_INFO(m);
	    return false;
	  }
 
	  return true;
 
	}

	template<typename T>
	static bool computeEigenVector(const T* const src,const T vp, T& v0, T&v1, T&v2)
	{
	  TFEL_STATIC_ASSERT(tfel::typetraits::IsFundamentalNumericType<T>::cond);
	  TFEL_STATIC_ASSERT(tfel::typetraits::IsReal<T>::cond);

	  // Assume that vp is a single eigenvalue

	  tfel::math::stensor<3,T,tfel::math::StensorStatic> s;
	  s(0) = src[0]-vp; 
	  s(1) = src[1]-vp; 
	  s(2) = src[2]-vp; 
	  s(3) = src[3]; 
	  s(4) = src[4]; 
	  s(5) = src[5]; 
	  T tmp;

	  T cond1 = conditionning_number(s(0),s(1),s(3));
	  T cond2 = conditionning_number(s(0),s(2),s(4));
	  T cond3 = conditionning_number(s(1),s(2),s(5));
  
	  if(cond1<cond2){
	    if(cond3<cond1){
	      // cond 3 is min
	      v0=static_cast<T>(1.);
	      solve2D(s(1),s(2),s(5),-s(3),-s(4),v1,v2);
	    } else{
	      // cond 1 is min
	      v2=static_cast<T>(1.);
	      solve2D(s(0),s(1),s(3),-s(4),-s(5),v0,v1);
	    }
	  } else if (cond3 > cond2){
	    // cond2 is min
	    v1=static_cast<T>(1.);
	    solve2D(s(0),s(2),s(4),-s(3),-s(5),v0,v2);    
	  } else {
	    // cond 3 is min
	    v0=static_cast<T>(1.);
	    solve2D(s(1),s(2),s(5),-s(3),-s(4),v1,v2);    
	  }

	  tmp = norm(v0,v1,v2);
	  v0/=tmp;
	  v1/=tmp;
	  v2/=tmp;
  
	  return true;

	}

      public:


	template<typename T>
	static bool exe(const T* const s,tfel::math::tvector<3u,T>& vp,tfel::math::tmatrix<3u,3u,T>& vec)
	{
	  TFEL_STATIC_ASSERT(tfel::typetraits::IsFundamentalNumericType<T>::cond);
	  TFEL_STATIC_ASSERT(tfel::typetraits::IsReal<T>::cond);
	  static const T rel_prec = 1000*std::numeric_limits<T>::epsilon();

	  T prec;

	  StensorComputeEigenValues_<3u>::exe(s,vp(0),vp(1),vp(2));

	  if(std::abs(vp(0))>std::abs(vp(1))){
	    if(std::abs(vp(0))>std::abs(vp(2))){
	      //vp(0) is max
	      prec = std::max(std::abs(vp(0))*rel_prec,std::numeric_limits<T>::min());
	    } else {
	      //vp(2) is max
	      prec = std::max(std::abs(vp(2))*rel_prec,std::numeric_limits<T>::min());
	    }
	  } else if(vp(1)>vp(2)){
	    //vp(1) is max
	    prec = std::max(std::abs(vp(1))*rel_prec,std::numeric_limits<T>::min());
	  } else{
	    //vp(2) is max
	    prec = std::max(std::abs(vp(2))*rel_prec,std::numeric_limits<T>::min());
	  }

	  if((std::abs(vp(0)-vp(1))<=prec)&&(std::abs(vp(0)-vp(2))<=prec)){
	    // all eigenvalues are equal
	    vec = tmatrix<3u,3u,T>::Id();
#ifdef PARANOIC_CHECK
	    return tfel::math::internals::StensorComputeEigenVectors_<3>::test(s,vp,vec);
#else
	    return true;
#endif
	  }

	  if((std::abs(vp(0)-vp(1))>prec)&&(std::abs(vp(0)-vp(2))>prec)){
	    // vp0 is single
	    if(computeEigenVector(s,vp(0),vec(0,0),vec(1,0),vec(2,0))==false){
	      return false;
	    }
	    if(std::abs(vp(1)-vp(2))>prec){
	      //vp1 is single
	      if(computeEigenVector(s,vp(1),vec(0,1),vec(1,1),vec(2,1))==false){
		return false;
	      }
	      cross_product(vec(0,2),vec(1,2),vec(2,2),
			    vec(0,0),vec(1,0),vec(2,0),
			    vec(0,1),vec(1,1),vec(2,1));
	    } else {
	      // vp1 and vp2 are equal
	      find_perpendicular_vector(vec(0,1),vec(1,1),vec(2,1),vec(0,0),vec(1,0),vec(2,0));
	      cross_product(vec(0,2),vec(1,2),vec(2,2),
			    vec(0,0),vec(1,0),vec(2,0),
			    vec(0,1),vec(1,1),vec(2,1));
	    }
#ifdef PARANOIC_CHECK
	    return tfel::math::internals::StensorComputeEigenVectors_<3>::test(s,vp,vec);
#else
	    return true;
#endif
	  } else if((std::abs(vp(1)-vp(0))>prec)&&(std::abs(vp(1)-vp(2))>prec)){
	    // vp1 is single, vp0 and vp2 are equal
	    assert(std::abs(vp(0)-vp(2))<prec);
	    if(computeEigenVector(s,vp(1),vec(0,1),vec(1,1),vec(2,1))==false){
	      return false;
	    }
	    find_perpendicular_vector(vec(0,0),vec(1,0),vec(2,0),vec(0,1),vec(1,1),vec(2,1));
	    cross_product(vec(0,2),vec(1,2),vec(2,2),
			  vec(0,0),vec(1,0),vec(2,0),
			  vec(0,1),vec(1,1),vec(2,1));
#ifdef PARANOIC_CHECK
	    return tfel::math::internals::StensorComputeEigenVectors_<3>::test(s,vp,vec);
#else
	    return true;
#endif
	  } 
	  // vp2 is single, vp0 and vp1 are equal
	  assert((std::abs(vp(2)-vp(0))>prec)&&(std::abs(vp(2)-vp(1))>prec));
	  assert(std::abs(vp(0)-vp(1))<prec);
	  if(computeEigenVector(s,vp(2),vec(0,2),vec(1,2),vec(2,2))==false){
	    return false;
	  }
	  find_perpendicular_vector(vec(0,0),vec(1,0),vec(2,0),vec(0,2),vec(1,2),vec(2,2));
	  cross_product(vec(0,1),vec(1,1),vec(2,1),
			vec(0,2),vec(1,2),vec(2,2),
			vec(0,0),vec(1,0),vec(2,0));  
#ifdef PARANOIC_CHECK
	  return tfel::math::internals::StensorComputeEigenVectors_<3>::test(s,vp,vec);
#else
	  return true;
#endif
	}

      };

    } //end of namespace internals

  } //end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_STENSORCOMPUTEEIGENVECTORS_H */

