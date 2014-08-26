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
#include"TFEL/Metaprogramming/IsSameType.hxx"
#include"TFEL/TypeTraits/IsReal.hxx"
#include"TFEL/TypeTraits/IsFundamentalNumericType.hxx"
#include"TFEL/Math/General/CubicRoots.hxx"
#include"TFEL/Math/tvector.hxx"
#include"TFEL/Math/tmatrix.hxx"
#include"TFEL/Math/stensor.hxx"

namespace tfel{

  namespace math {

    namespace internals{

      template<unsigned short N>
      class StensorComputeEigenVectors_;

      template<>
      class StensorComputeEigenVectors_<1u>
      {
      public:

	template<typename VectorType,typename T>
	static bool computeEigenVector(VectorType& v,const T* const s,const T vp)
	{
	  using namespace std;
	  TFEL_STATIC_ASSERT(tfel::typetraits::IsFundamentalNumericType<typename VectorTraits<VectorType>::NumType>::cond);
	  TFEL_STATIC_ASSERT((tfel::meta::IsSameType<typename VectorTraits<VectorType>::NumType,T>::cond));
	  TFEL_STATIC_ASSERT(tfel::typetraits::IsFundamentalNumericType<T>::cond);
	  TFEL_STATIC_ASSERT(tfel::typetraits::IsReal<T>::cond);
	  if(abs(s[0]-vp)<10*numeric_limits<T>::min()){
	    v(0)=T(1);
	    v(1)=T(0);
	    v(2)=T(0);
	    return true;
	  }
	  if(abs(s[1]-vp)<10*numeric_limits<T>::min()){
	    v(0)=T(0);
	    v(1)=T(1);
	    v(2)=T(0);
	    return true;
	  }
	  if(abs(s[2]-vp)<10*numeric_limits<T>::min()){
	    v(0)=T(0);
	    v(1)=T(0);
	    v(2)=T(1);
	    return true;
	  }
	  return false;
	}

	template<typename T>
	static bool exe(const T* const s,tfel::math::tvector<3u,T>& vp,tfel::math::tmatrix<3u,3u,T>& vec,
			const bool)
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

	  static const T M1_sqrt2  = T(1)/std::sqrt(T(2.)); 
	  
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
	  if(std::abs(s3)<std::max(std::min(s_0,s_1)*std::numeric_limits<T>::epsilon(),
				   10*std::numeric_limits<T>::min())){
	    if(std::abs(s0)>std::abs(s1)){
	      x = T(0);
	      y = T(1);
	      return true;
	    } else {
	      x = T(1);
	      y = T(0);
	      return true;
	    }
	  }
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

	template<typename VectorType,typename T>
	static bool computeEigenVector(VectorType& v,const T* const s,const T vp)
	{
	  using namespace std;
	  TFEL_STATIC_ASSERT(tfel::typetraits::IsFundamentalNumericType<typename VectorTraits<VectorType>::NumType>::cond);
	  TFEL_STATIC_ASSERT((tfel::meta::IsSameType<typename VectorTraits<VectorType>::NumType,T>::cond));
	  TFEL_STATIC_ASSERT(tfel::typetraits::IsFundamentalNumericType<T>::cond);
	  TFEL_STATIC_ASSERT(tfel::typetraits::IsReal<T>::cond);
	  if(abs(s[2]-vp)<10*numeric_limits<T>::min()){
	    v(0)=0;
	    v(1)=0;
	    v(2)=1;
	    return true;
	  }
	  v(2)=0.;
	  return computeEigenVector(s[0],s[1],s[3],vp,v(0),v(1));
	}

	template<typename T>
	static bool exe(const T* const s,
			tfel::math::tvector<3u,T>& vp,tfel::
			math::tmatrix<3u,3u,T>& vec,
			const bool)
	{
	  TFEL_STATIC_ASSERT(tfel::typetraits::IsFundamentalNumericType<T>::cond);
	  TFEL_STATIC_ASSERT(tfel::typetraits::IsReal<T>::cond);

	  T prec;

	  vp(2)=s[2];
	  vec = tmatrix<3u,3u,T>::Id();

	  StensorComputeEigenVectors_<2u>::computeEigenValues(s[0],s[1],s[3],vp(0),vp(1));
	  prec = 10*std::max(vp(0),vp(1))*std::numeric_limits<T>::epsilon();
	  if(std::abs(vp(0)-vp(1))<=prec){
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
	  using namespace tfel::math;
	  TFEL_STATIC_ASSERT(tfel::typetraits::IsFundamentalNumericType<T>::cond);
	  TFEL_STATIC_ASSERT(tfel::typetraits::IsReal<T>::cond);
	  // Assume that vp is a single eigenvalue
	  static const T M1_sqrt2  = T(1)/std::sqrt(T(2)); 
	  const T a = src[0]-vp;
	  const T b = src[3]*M1_sqrt2;
	  const T c = src[4]*M1_sqrt2;
	  const T d = src[1]-vp;
	  const T e = src[5]*M1_sqrt2;
	  const T f = src[2]-vp;
	  const T det3=a*d-b*b;
	  const T det2=a*f-c*c;
	  const T det1=d*f-e*e;
	  if((std::abs(det1)<std::max(10*std::abs(vp)*std::numeric_limits<T>::epsilon(),
				      10*std::numeric_limits<T>::min()))&&
	     (std::abs(det2)<std::max(10*std::abs(vp)*std::numeric_limits<T>::epsilon(),
				      10*std::numeric_limits<T>::min()))&&
	     (std::abs(det3)<std::max(10*std::abs(vp)*std::numeric_limits<T>::epsilon(),
				      10*std::numeric_limits<T>::min()))){
	    // This means that vp is *not* single
	    tvector<3u,T> v;
	    tmatrix<3u,3u,T> m;
	    if(!StensorComputeEigenVectors_<3u>::exe(src,v,m,true)){
	      v0 = v1 = v2 = T(0);	      
	      return false;
	    }
	    const T d0 = std::abs(v[0]-vp);
	    const T d1 = std::abs(v[1]-vp);
	    const T d2 = std::abs(v[2]-vp);
	    if((d0<d1)&&(d0<d2)){
	      v0 = m(0,0);
	      v1 = m(1,0);
	      v2 = m(2,0);
	      return true;
	    } else if((d1<d2)&&(d1<d0)){
	      v0 = m(0,1);
	      v1 = m(1,1);
	      v2 = m(2,1);
	      return true;
	    }
	    v0 = m(0,2);
	    v1 = m(1,2);
	    v2 = m(2,2);
	    return true;
	  }
	  if ((std::abs(det3)>=std::abs(det1))&&
	      (std::abs(det3)>=std::abs(det2))){
	    v0=(b*e-c*d)/det3;
	    v1=(b*c-a*e)/det3;
	    v2=T(1);
	  } else if((std::abs(det1)>=std::abs(det2))&&
		    (std::abs(det1)>=std::abs(det3))){
	    v0=T(1);
	    v1=(c*e-b*f)/det1;
	    v2=(b*e-c*d)/det1;
	  } else {
	    v0=(c*e-b*f)/det2;
	    v1=T(1);
	    v2=(b*c-a*e)/det2;
	  }
	  const T nr = norm(v0,v1,v2);
	  v0/=nr;
	  v1/=nr;
	  v2/=nr;
  	  return true;
	}

      public:


	template<typename VectorType,
		 typename T>
	static bool computeEigenVector(VectorType& v,const T* const src,const T vp)
	{
	  TFEL_STATIC_ASSERT(tfel::typetraits::IsFundamentalNumericType<typename VectorTraits<VectorType>::NumType>::cond);
	  TFEL_STATIC_ASSERT((tfel::meta::IsSameType<typename VectorTraits<VectorType>::NumType,T>::cond));
	  TFEL_STATIC_ASSERT(tfel::typetraits::IsFundamentalNumericType<T>::cond);
	  TFEL_STATIC_ASSERT(tfel::typetraits::IsReal<T>::cond);
	  return computeEigenVector(src,vp,v(0),v(1),v(2));
	}

	template<typename T>
	static bool exe(const T* const s,tfel::math::tvector<3u,T>& vp,tfel::math::tmatrix<3u,3u,T>& vec,
			const bool b)
	{
	  using namespace std;
	  TFEL_STATIC_ASSERT(tfel::typetraits::IsFundamentalNumericType<T>::cond);
	  TFEL_STATIC_ASSERT(tfel::typetraits::IsReal<T>::cond);
	  static const T rel_prec = 100*std::numeric_limits<T>::epsilon();
	  StensorComputeEigenValues_<3u>::exe(s,vp(0),vp(1),vp(2),b);
	  const T mvp  = max(max(abs(vp(0)),abs(vp(1))),abs(vp(2)));
	  const T prec = max(mvp*rel_prec,100*std::numeric_limits<T>::min());
	  if((std::abs(vp(0)-vp(1))<=prec)&&
	     (std::abs(vp(0)-vp(2))<=prec)){
	    // all eigenvalues are equal
	    vec = tmatrix<3u,3u,T>::Id();
#ifdef PARANOIC_CHECK
	    return tfel::math::internals::StensorComputeEigenVectors_<3>::test(s,vp,vec);
#else
	    return true;
#endif
	  }
	  if((abs(vp(0)-vp(1))>prec)&&
	     (abs(vp(0)-vp(2))>prec)){
	    // vp0 is single
	    if(computeEigenVector(s,vp(0),vec(0,0),vec(1,0),vec(2,0))==false){
	      return false;
	    }
	    if(abs(vp(1)-vp(2))>prec){
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
	  } else if((abs(vp(1)-vp(0))>prec)&&
		    (abs(vp(1)-vp(2))>prec)){
	    // vp1 is single, vp0 and vp2 are equal
	    assert(abs(vp(0)-vp(2))<prec);
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
	  assert((abs(vp(2)-vp(0))>prec)&&(abs(vp(2)-vp(1))>prec));
	  assert(abs(vp(0)-vp(1))<prec);
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

