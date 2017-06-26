/*!
 * \file   LogarithmicStrainHandler.ixx
 * \brief    
 * \author HELFER Thomas 202608
 * \date   25 juin 2017
 */

#ifndef LIB_TFEL_MATH_LOGARITHMICSTRAINHANDLER_IXX
#define LIB_TFEL_MATH_LOGARITHMICSTRAINHANDLER_IXX

#include<cmath>

namespace tfel
{

  namespace material
  {

    template<typename StressType>
    LogarithmicStrainHandler<1u,StressType>::LogarithmicStrainHandler(const DeformationGradient& F1)
      : F(F1)
    {} // end of LogarithmicStrainHandler<1u,StressTupe>::LogarithmicStrainHandler

    template<typename StressType>
    typename LogarithmicStrainHandler<1u,StressType>::StrainStensor
    LogarithmicStrainHandler<1u,StressType>::getHenckyLogarithmicStrain() const
    {
      return {this->e[0],this->e[1],this->e[2]};
    } // end of LogarithmicStrainHandler<1u,StressType>::getHenckyLogarithmicStrain
    
    template<typename StressType>
    typename LogarithmicStrainHandler<1u,StressType>::StressStensor
    LogarithmicStrainHandler<1u,StressType>::convertToSecondPiolaKirchhoffStress(const StressStensor& T) const
    {
      return {T[0]/(this->F[0]*this->F[0]),T[1]/(this->F[1]*this->F[1]),T[2]/(this->F[2]*this->F[2])};
    } // end of LogarithmicStrainHandler<1u,StressType>::convertToSecondPiolaKirchhoffStress

    template<typename StressType>
    typename LogarithmicStrainHandler<1u,StressType>::StressStensor
    LogarithmicStrainHandler<1u,StressType>::convertFromSecondPiolaKirchhoffStress(const StressStensor& S) const
    {
      return {S[0]*(this->F[0]*this->F[0]),S[1]*(this->F[1]*this->F[1]),S[2]*(this->F[2]*this->F[2])};
    } // end of LogarithmicStrainHandler<1u,StressType>::convertToSecondPiolaKirchhoffStress

    template<typename StressType>
    typename LogarithmicStrainHandler<1u,StressType>::StressStensor
    LogarithmicStrainHandler<1u,StressType>::convertToCauchyStress(const StressStensor& T) const
    {
      return T/tfel::math::det(this->F);
    } // end of LogarithmicStrainHandler<1u,StressType>::convertToSecondPiolaKirchhoffStress

    template<typename StressType>
    typename LogarithmicStrainHandler<1u,StressType>::StressStensor
    LogarithmicStrainHandler<1u,StressType>::convertFromCauchyStress(const StressStensor& s) const
    {
      return s*tfel::math::det(this->F);
    } // end of LogarithmicStrainHandler<1u,StressType>::convertFromCauchyStress
    
    template<typename StressType>
    typename LogarithmicStrainHandler<1u,StressType>::TangentOperator
    LogarithmicStrainHandler<1u,StressType>::convertToMaterialTangentModuli(const TangentOperator& Ks,
									    const StressStensor& T) const
    {
      const real iC[3]  = {1/(this->F[0]*this->F[0]),
			   1/(this->F[1]*this->F[1]),
			   1/(this->F[2]*this->F[2])};
      auto Kr = TangentOperator{};
      Kr(0,0)   = (Ks(0,0)-2*T[0])*iC[0]*iC[0];
      Kr(0,1)   =  Ks(0,1)*iC[0]*iC[1];
      Kr(0,2)   =  Ks(0,2)*iC[0]*iC[2];
      Kr(1,0)   =  Ks(1,0)*iC[0]*iC[1];
      Kr(1,1)   = (Ks(1,1)-2*T[1])*iC[1]*iC[1];
      Kr(1,2)   =  Ks(1,2)*iC[1]*iC[2];
      Kr(2,0)   =  Ks(2,0)*iC[0]*iC[2];
      Kr(2,1)   =  Ks(2,1)*iC[1]*iC[2];
      Kr(2,2)   = (Ks(2,2)-2*T[2])*iC[2]*iC[2];
      return Kr;
    }

    template<typename StressType>
    LogarithmicStrainHandler<2u,StressType>::LogarithmicStrainHandler(const DeformationGradient& F1)
      : LogarithmicStrainHandler(Builder(F1),F1)
    {} // end of LogarithmicStrainHandler<2u,StressTupe>::LogarithmicStrainHandler

    template<typename StressType>
    LogarithmicStrainHandler<2u,StressType>::Builder::Builder(const DeformationGradient& F1)
    {
      using namespace tfel::math;
      using stensor = stensor<2u,real>;
      const auto fl  = [](const real x){return std::log1p(x-1)/2;};
      const auto dfl = [](const real x){return 1/(2*x);};
      const auto C = computeRightCauchyGreenTensor(F1);
      std::tie(this->vp,this->m) = C.template computeEigenVectors<stensor::FSESJACOBIEIGENSOLVER>();
      this->e = map(fl,vp);
      const auto d = map(dfl,vp);
      // p is one half of the tensor defined by Miehe
      this->p = stensor::computeIsotropicFunctionDerivative(e,d,vp,m,eps);
    }

    template<typename StressType>
    LogarithmicStrainHandler<2u,StressType>::LogarithmicStrainHandler(Builder&& src,
								      const DeformationGradient& F1)
      : p(src.p),F(F1),m(src.m),vp(src.vp),e(src.e)
    {} // end of LogarithmicStrainHandler<2u,StressTupe>::LogarithmicStrainHandler

    template<typename StressType>
    typename LogarithmicStrainHandler<2u,StressType>::StrainStensor
    LogarithmicStrainHandler<2u,StressType>::getHenckyLogarithmicStrain() const
    {
      return StrainStensor::computeIsotropicFunction(this->e,this->m);
    } // end of LogarithmicStrainHandler<2u,StressType>::getHenckyLogarithmicStrain
    
    template<typename StressType>
    typename LogarithmicStrainHandler<2u,StressType>::StressStensor
    LogarithmicStrainHandler<2u,StressType>::convertToSecondPiolaKirchhoffStress(const StressStensor& T) const
    {
      return 2*(T|p);
    } // end of LogarithmicStrainHandler<2u,StressType>::convertToSecondPiolaKirchhoffStress

    template<typename StressType>
    typename LogarithmicStrainHandler<2u,StressType>::StressStensor
    LogarithmicStrainHandler<2u,StressType>::convertFromSecondPiolaKirchhoffStress(const StressStensor& S) const
    {
      return (S|tfel::math::invert(p))/2;
    } // end of LogarithmicStrainHandler<2u,StressType>::convertToSecondPiolaKirchhoffStress

    template<typename StressType>
    typename LogarithmicStrainHandler<2u,StressType>::StressStensor
    LogarithmicStrainHandler<2u,StressType>::convertToCauchyStress(const StressStensor& T) const
    {
      const auto S = this->convertToSecondPiolaKirchhoffStress(T);
      return tfel::math::convertSecondPiolaKirchhoffStressToCauchyStress(S,this->F);
    } // end of LogarithmicStrainHandler<2u,StressType>::convertToSecondPiolaKirchhoffStress

    template<typename StressType>
    typename LogarithmicStrainHandler<2u,StressType>::StressStensor
    LogarithmicStrainHandler<2u,StressType>::convertFromCauchyStress(const StressStensor& s) const
    {
      const auto S = tfel::math::convertCauchyStressToSecondPiolaKirchhoffStress(s,this->F);
      return  this->convertFromSecondPiolaKirchhoffStress(S);
    } // end of LogarithmicStrainHandler<2u,StressType>::convertFromCauchyStress
    
    template<typename StressType>
    typename LogarithmicStrainHandler<2u,StressType>::TangentOperator
    LogarithmicStrainHandler<2u,StressType>::convertToMaterialTangentModuli(const TangentOperator& Ks,
									    const StressStensor& T) const
    {
      auto F3d = tfel::math::tensor<3u,real>(real(0));
      auto T3d = tfel::math::stensor<3u,real>(real(0));
      auto K3d = tfel::math::st2tost2<3u,real>(real(0));
      for(size_type i=0;i!=5;++i){
      	F3d[i] = F[i];
      }
      for(size_type i=0;i!=4;++i){
      	T3d[i] = T[i];
      	for(size_type j=0;j!=4;++j){
      	  K3d(i,j)=Ks(i,j);
      	}
      }
      LogarithmicStrainHandler<3u,StressType> l(F3d);
      const auto Kr3d = l.convertToMaterialTangentModuli(K3d,T3d);
      auto Kr = tfel::math::st2tost2<2u,real>{};
      for(size_type i=0;i!=4;++i){
      	for(size_type j=0;j!=4;++j){
      	  Kr(i,j)=Kr3d(i,j);
      	}
      }
      return Kr;

      // using tvector = tfel::math::tvector<3u,real>;
      // using stensor = tfel::math::stensor<2u,real>;
      // using tfel::math::tmatrix;
      // const auto dfl = [](const real x){return 1/(2*x);};
      // const auto d = map(dfl,vp); // half compared to Miehe definition
      // // compute the derivative of the Hencky strain with respect to C
      // const auto f = map([](const real x){return -2/(x*x);},this->vp);
      // // real work starts here
      // const auto M = [this](){
      // 	const tvector v[2] = {this->m.template column_view<0u>(),
      // 			      this->m.template column_view<1u>()};
      // 	auto r = tfel::math::tvector<4u,stensor>();
      // 	r(0) = stensor::buildFromVectorsSymmetricDiadicProduct(v[0],v[0]);
      // 	r(1) = stensor::buildFromVectorsSymmetricDiadicProduct(v[1],v[1]);
      // 	r(2) = {0,0,1,0};
      // 	r(3) = stensor::buildFromVectorsSymmetricDiadicProduct(v[0],v[1]);
      // 	return r;
      // }();
      // const auto xsi = [this,&d,&f]() -> tfel::math::tvector<2u,real> {
      // 	if(std::abs(this->vp[0]-this->vp[1])<eps){
      // 	  const auto rv = (f[0]+f[1])/16;
      // 	  return {rv,rv};
      // 	}
      // 	const auto idvp = 1/(this->vp[0]-this->vp[1]);
      // 	const auto r0 =  ((this->e[0]-this->e[1])*idvp-d[0])*idvp;
      // 	const auto r1 = -((this->e[0]-this->e[1])*idvp-d[1])*idvp;
      // 	return {r0,r1};
      // }();
      // const auto dzeta = [&M,&T](){
      // 	auto r = tfel::math::tvector<4u,real>{};
      // 	for(size_type i=0;i!=4;++i){
      // 	  r(i)=(T|M(i))/2;
      // 	}
      // 	return r;
      // }();
      // TangentOperator Kr = 4*transpose(p)*Ks*p;
      // Kr+=(f[0]*dzeta(0)*(M(0)^M(0))+
      // 	   f[1]*dzeta(1)*(M(1)^M(1)))/4;
      // Kr(2,2)+=f[2]/4*dzeta(2);
      // Kr+=2*xsi[0]*(dzeta(3)*((M(3)^M(1))+(M(1)^M(3)))+
      // 		    dzeta(1)*(M(3)^M(3)));
      // Kr+=2*xsi[1]*(dzeta(3)*((M(3)^M(0))+(M(0)^M(3)))+
      // 		    dzeta(0)*(M(3)^M(3)));
      // return Kr;
    }
    
    template<typename StressType>
    LogarithmicStrainHandler<3u,StressType>::LogarithmicStrainHandler(const DeformationGradient& F1)
      : LogarithmicStrainHandler(Builder(F1),F1)
    {} // end of LogarithmicStrainHandler<3u,StressTupe>::LogarithmicStrainHandler

    template<typename StressType>
    LogarithmicStrainHandler<3u,StressType>::Builder::Builder(const DeformationGradient& F1)
    {
      using namespace tfel::math;
      using stensor = stensor<3u,real>;
      const auto fl  = [](const real x){return std::log1p(x-1)/2;};
      const auto dfl = [](const real x){return 1/(2*x);};
      const auto C = computeRightCauchyGreenTensor(F1);
      std::tie(this->vp,this->m) = C.template computeEigenVectors<stensor::FSESJACOBIEIGENSOLVER>();
      this->e = map(fl,vp);
      const auto d = map(dfl,vp);
      // p is one half of the tensor defined by Miehe
      this->p = stensor::computeIsotropicFunctionDerivative(e,d,vp,m,eps);
    }

    template<typename StressType>
    LogarithmicStrainHandler<3u,StressType>::LogarithmicStrainHandler(Builder&& src,
								      const DeformationGradient& F1)
      : p(src.p),F(F1),m(src.m),vp(src.vp),e(src.e)
    {} // end of LogarithmicStrainHandler<3u,StressTupe>::LogarithmicStrainHandler

    template<typename StressType>
    typename LogarithmicStrainHandler<3u,StressType>::StrainStensor
    LogarithmicStrainHandler<3u,StressType>::getHenckyLogarithmicStrain() const
    {
      return StrainStensor::computeIsotropicFunction(this->e,this->m);
    } // end of LogarithmicStrainHandler<3u,StressType>::getHenckyLogarithmicStrain
    
    template<typename StressType>
    typename LogarithmicStrainHandler<3u,StressType>::StressStensor
    LogarithmicStrainHandler<3u,StressType>::convertToSecondPiolaKirchhoffStress(const StressStensor& T) const
    {
      return 2*(T|p);
    } // end of LogarithmicStrainHandler<3u,StressType>::convertToSecondPiolaKirchhoffStress

    template<typename StressType>
    typename LogarithmicStrainHandler<3u,StressType>::StressStensor
    LogarithmicStrainHandler<3u,StressType>::convertFromSecondPiolaKirchhoffStress(const StressStensor& S) const
    {
      return (S|tfel::math::invert(p))/2;
    } // end of LogarithmicStrainHandler<3u,StressType>::convertToSecondPiolaKirchhoffStress

    template<typename StressType>
    typename LogarithmicStrainHandler<3u,StressType>::StressStensor
    LogarithmicStrainHandler<3u,StressType>::convertToCauchyStress(const StressStensor& T) const
    {
      const auto S = this->convertToSecondPiolaKirchhoffStress(T);
      return tfel::math::convertSecondPiolaKirchhoffStressToCauchyStress(S,this->F);
    } // end of LogarithmicStrainHandler<3u,StressType>::convertToSecondPiolaKirchhoffStress

    template<typename StressType>
    typename LogarithmicStrainHandler<3u,StressType>::StressStensor
    LogarithmicStrainHandler<3u,StressType>::convertFromCauchyStress(const StressStensor& s) const
    {
      const auto S = tfel::math::convertCauchyStressToSecondPiolaKirchhoffStress(s,this->F);
      return  this->convertFromSecondPiolaKirchhoffStress(S);
    } // end of LogarithmicStrainHandler<3u,StressType>::convertFromCauchyStress
    
    template<typename StressType>
    typename LogarithmicStrainHandler<3u,StressType>::TangentOperator
    LogarithmicStrainHandler<3u,StressType>::convertToMaterialTangentModuli(const TangentOperator& Ks,
									    const StressStensor& T) const
    {
      using tvector = tfel::math::tvector<3u,real>;
      using stensor = tfel::math::stensor<3u,real>;
      using tfel::math::tmatrix;
      auto are_all_vp_equals = [this](){
	return ((std::abs(this->vp(1)-this->vp(0))<eps)&&
		(std::abs(this->vp(1)-this->vp(2))<eps)&&
		(std::abs(this->vp(2)-this->vp(0))<eps));
      };
      auto find_uniq_vp = [this,&are_all_vp_equals](){
	if(are_all_vp_equals()){
	  throw(std::runtime_error("FiniteStrainBehaviourTangentOperatorConverter"
				   "<Base::DT_DELOG,Base::DS_DEGL>::exe: "
				   "find_uniq_vp shall not be called if all "
				   "eigen values are equal."));
	}
	if((std::abs(this->vp(1)-this->vp(0))>eps)&&
	   (std::abs(this->vp(1)-this->vp(2))>eps)&&
	   (std::abs(this->vp(2)-this->vp(0))>eps)){
	  // all eigenvalues are different
	  return 3;
	}
	if((std::abs(this->vp(1)-this->vp(0))<eps)){
	  return 2;
	}
	if((std::abs(this->vp(2)-this->vp(0))<eps)){
	  return 1;
	}
	return 0;
      };
      const auto lk = [](const size_type i, const size_type j) -> size_type{
	if(i==0){
	  return (j==1) ? 2 : 1;
	}
	if(i==1){
	  return (j==0) ? 2 : 0;
	}
	return (j==0) ? 1 : 0;
      };
      const auto dfl = [](const real x){return 1/(2*x);};
      const auto d = map(dfl,vp); // half compared to Miehe definition
      // compute the derivative of the Hencky strain with respect to C
      const auto f = map([](const real x){return -2/(x*x);},this->vp);
      // real work starts here
      const auto M = [this]{
	const tvector v[3] = {this->m.template column_view<0u>(),
			      this->m.template column_view<1u>(),
			      this->m.template column_view<2u>()};
	tmatrix<3u,3u,stensor> r;
	for(size_type i=0;i!=3;++i){
	  for(size_type j=0;j!=3;++j){
	    r(i,j)=stensor::buildFromVectorsSymmetricDiadicProduct(v[i],v[j]);
	  }
	}
	return r;
      }();
      const auto xsi   = [this,&d,&f,&are_all_vp_equals,&find_uniq_vp](){
	if(are_all_vp_equals()){
	  constexpr const auto zero = real{0};
	  const auto rv = (f[0]+f[1]+f[2])/24;
	  return tmatrix<3u,3u,real>{zero,rv,rv,
	      rv,zero,rv,
	      rv,rv,zero};
	}
	auto r = tmatrix<3u,3u,real>{};
	const auto k = find_uniq_vp();
	if(k!=3){
	  for(size_type i=0;i!=3;++i){
	    for(size_type j=0;j!=3;++j){
	      if(i==j){
		r(i,j) = real{};
	      } else if((i==k)||(j==k)){
		const auto idvp = 1/(this->vp[i]-this->vp[j]);
		r(i,j)=((this->e[i]-this->e[j])*idvp-d[j])*idvp;
	      } else {
		r(i,j)=(f[i]+f[j])/16;
	      }
	    }
	  }
	  return r;
	}
	for(size_type i=0;i!=3;++i){
	  for(size_type j=0;j!=3;++j){
	    if(i==j){
	      r(i,j) = real{};
	    } else {
	      const auto idvp = 1/(this->vp[i]-this->vp[j]);
	      r(i,j)=((this->e[i]-this->e[j])*idvp-d[j])*idvp;
	    }
	  }
	}
	return r;
      }();
      const auto eta = [this,&lk,&f,&are_all_vp_equals,&find_uniq_vp,&d]{
	if(are_all_vp_equals()){
	  return (f[0]+f[1]+f[2])/24;
	}
	const auto u = find_uniq_vp();
	if(u!=3){
	  const auto i = (u==2) ? 0 : 2;
	  const auto idvp = 1/(this->vp[i]-this->vp[u]);
	  return ((this->e[i]-this->e[u])*idvp-d[u])*idvp;
	}
	auto r = real{};
	for(size_type i=0;i!=3;++i){
	  for(size_type j=0;j!=3;++j){
	    if(i==j){
	      continue;
	    }
	    const auto k = lk(i,j);
	    r += this->e[i]/(2*(this->vp[i]-this->vp[j])*(this->vp[i]-this->vp[k]));
	  }
	}
	return r;
      }();
      const auto dzeta = [&M,&T](){
	auto r = tmatrix<3u,3u,real>{};
	for(size_type i=0;i!=3;++i){
	  for(size_type j=0;j!=3;++j){
	    r(i,j)=(T|M(i,j))/2;
	  }
	}
	return r;
      }();
      TangentOperator Kr = 4*transpose(p)*Ks*p;
      for(size_type i=0;i!=3;++i){
	Kr+=f[i]/4*dzeta(i,i)*(M(i,i)^M(i,i));
	for(size_type j=0;j!=3;++j){
	  if(i==j){
	    continue;
	  }
	  const auto k = lk(i,j);
	  Kr+=2*eta*dzeta(i,j)*(M(i,k)^M(j,k))
	    +2*xsi(i,j)*(dzeta(i,j)*((M(i,j)^M(j,j))+(M(j,j)^M(i,j)))+
			 dzeta(j,j)*(M(i,j)^M(i,j)));
	}
      }
      return Kr;
    }
    
  } // end of namespace material

} // end of namespace material

#endif /* LIB_TFEL_MATH_LOGARITHMICSTRAINHANDLER_IXX */
