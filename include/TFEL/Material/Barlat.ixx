/*!
 * \file   Barlat.ixx
 * \brief    
 * \author HELFER Thomas 202608
 * \date   17 nov. 2017
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_TFEL_MATERIAL_BARLAT_IXX
#define LIB_TFEL_MATERIAL_BARLAT_IXX

namespace tfel{

  namespace material{

    namespace internals{

      /*!
       * \brief structure in charge of computing a linear
       * transformation of the stress tensor.
       * \tparam N: space dimension
       */
      template<unsigned short N>
      struct BarlatLinearTransformation;
      /*!
       * \brief partial specialisation of the `BarlatLinearTransformation`
       * in `1D`.
       */
      template<>
      struct BarlatLinearTransformation<1u>
      {
	/*!
	 * \return a linear transformation of the stresses
	 * \param[in] c12: coefficient of  linear transformation
	 * \param[in] c21: coefficient of  linear transformation
	 * \param[in] c13: coefficient of  linear transformation
	 * \param[in] c31: coefficient of  linear transformation
	 * \param[in] c23: coefficient of  linear transformation
	 * \param[in] c32: coefficient of  linear transformation
	 * \param[in] c44: coefficient of  linear transformation
	 * \param[in] c55: coefficient of  linear transformation
	 * \param[in] c66: coefficient of  linear transformation
	 * \see tfel::material::makeBarlatLinearTransformation
	 */
	template<typename real>
	static tfel::math::st2tost2<1u,real>
	exe(const real c12,const real c21,
	    const real c13,const real c31,
	    const real c23,const real c32,
	    const real,const real,
	    const real)
	{
	  TFEL_CONSTEXPR const auto zero = real{0};
	  return {zero,-c12,-c13,
	      -c21,zero,-c23,
	      -c31,-c32,zero};
	} // end of exe
      }; // end of struct BarlatLinearTransformation<1u>
      /*!
       * \brief partial specialisation of the `BarlatLinearTransformation`
       * in `2D`.
       */
      template<>
      struct BarlatLinearTransformation<2u>
      {
	/*!
	 * \return a linear transformation of the stresses
	 * \param[in] c12: coefficient of  linear transformation
	 * \param[in] c21: coefficient of  linear transformation
	 * \param[in] c13: coefficient of  linear transformation
	 * \param[in] c31: coefficient of  linear transformation
	 * \param[in] c23: coefficient of  linear transformation
	 * \param[in] c32: coefficient of  linear transformation
	 * \param[in] c44: coefficient of  linear transformation
	 * \param[in] c55: coefficient of  linear transformation
	 * \param[in] c66: coefficient of  linear transformation
	 * \see tfel::material::makeBarlatLinearTransformation
	 */
	template<typename real>
	static tfel::math::st2tost2<2u,real>
	exe(const real c12,const real c21,
	    const real c13,const real c31,
	    const real c23,const real c32,
	    const real c44,const real,
	    const real)
	{
	  TFEL_CONSTEXPR const auto zero = real{0};
	  return {zero,-c12,-c13,zero,
	      -c21,zero,-c23,zero,
	      -c31,-c32,zero,zero,
	      zero,zero,zero,c44};
	} // end of exe
      }; // end of struct BarlatLinearTransformation<2u>
      /*!
       * \brief partial specialisation of the `BarlatLinearTransformation`
       * in `3D`.
       */
      template<>
      struct BarlatLinearTransformation<3u>
      {
	/*!
	 * \return a linear transformation of the stresses
	 * \param[in] c12: coefficient of  linear transformation
	 * \param[in] c21: coefficient of  linear transformation
	 * \param[in] c13: coefficient of  linear transformation
	 * \param[in] c31: coefficient of  linear transformation
	 * \param[in] c23: coefficient of  linear transformation
	 * \param[in] c32: coefficient of  linear transformation
	 * \param[in] c44: coefficient of  linear transformation
	 * \param[in] c55: coefficient of  linear transformation
	 * \param[in] c66: coefficient of  linear transformation
	 * \see tfel::material::makeBarlatLinearTransformation
	 */
	template<typename real>
	static tfel::math::st2tost2<3u,real>
	exe(const real c12,const real c21,
	    const real c13,const real c31,
	    const real c23,const real c32,
	    const real c44,const real c55,
	    const real c66)
	{
	  TFEL_CONSTEXPR const auto zero = real{0};
	  return {zero,-c12,-c13,zero,zero,zero,
	      -c21,zero,-c23,zero,zero,zero,
	      -c31,-c32,zero,zero,zero,zero,
	      zero,zero,zero,c44,zero,zero,
	      zero,zero,zero,zero,c55,zero,
	      zero,zero,zero,zero,zero,c66};
	} // end of exe
      }; // end of struct BarlatLinearTransformation<3u>
      
    } // end namespace internals
    
    template<typename StressStensor,
	     tfel::math::stensor_common::EigenSolver es>
    BarlatStressType<StressStensor>
    computeBarlatStress(const StressStensor& s,
			const BarlatLinearTransformationType<StressStensor>& l1,
			const BarlatLinearTransformationType<StressStensor>& l2,
			const BarlatBaseType<StressStensor> a,
			const BarlatStressType<StressStensor> seps)
    {
      const auto seq = sigmaeq(s);
      if(seq<seps){
	return seq*0;
      }
      const auto iseq = 1/seq;
      const auto s1  = eval(l1*s);
      const auto s2  = eval(l2*s);
      const auto vp1 = s1.template computeEigenValues<es>()*iseq;
      const auto vp2 = s2.template computeEigenValues<es>()*iseq;
      return seq*std::pow((std::pow(std::abs(vp1[0]-vp2[0]),a)+
		      std::pow(std::abs(vp1[0]-vp2[1]),a)+
		      std::pow(std::abs(vp1[0]-vp2[2]),a)+
		      std::pow(std::abs(vp1[1]-vp2[0]),a)+
		      std::pow(std::abs(vp1[1]-vp2[1]),a)+
		      std::pow(std::abs(vp1[1]-vp2[2]),a)+
		      std::pow(std::abs(vp1[2]-vp2[0]),a)+
		      std::pow(std::abs(vp1[2]-vp2[1]),a)+
		      std::pow(std::abs(vp1[2]-vp2[2]),a))/4,1/a);
    } // end of computeBarlatStress

    template<typename StressStensor,
	     tfel::math::stensor_common::EigenSolver es>
    std::tuple<BarlatStressType<StressStensor>,
    	       BarlatStressNormalType<StressStensor>>
    computeBarlatStressNormal(const StressStensor& s,
			      const BarlatLinearTransformationType<StressStensor>& l1,
			      const BarlatLinearTransformationType<StressStensor>& l2,
			      const BarlatBaseType<StressStensor> a,
			      const BarlatStressType<StressStensor> seps)
    {
      constexpr const auto N = tfel::math::StensorTraits<StressStensor>::dime;
      using stress  = BarlatStressType<StressStensor>;
      using real    = BarlatBaseType<StressStensor>;
      using normal  = BarlatStressNormalType<StressStensor>;
      // Von Mises stress used to normalise the stress eigenvalues
      const auto seq   = sigmaeq(s);
      if(seq<seps){
    	return std::make_tuple(stress{0},normal{real{0}});
      }
      const auto iseq     = 1/seq;
      // linear transformations
      const auto s1 = eval(l1*s);
      const auto s2 = eval(l2*s);
      // no structured bindings yet
      tfel::math::tvector<3u,stress>  vp1;
      tfel::math::tmatrix<3u,3u,real> m1;
      std::tie(vp1,m1) = s1.template computeEigenVectors<es>();
      const auto n1    = tfel::math::stensor<N,stress>::computeEigenTensors(m1);
      tfel::math::tvector<3u,stress>  vp2;
      tfel::math::tmatrix<3u,3u,real> m2;
      std::tie(vp2,m2) = s2.template computeEigenVectors<es>();
      const auto n2    = tfel::math::stensor<N,stress>::computeEigenTensors(m2);
      // Barlat stress
      const auto rvp1  = vp1*iseq;
      const auto rvp2  = vp2*iseq;
      const auto Psi_a = (std::pow(std::abs(rvp1[0]-rvp2[0]),a)+
			  std::pow(std::abs(rvp1[0]-rvp2[1]),a)+
			  std::pow(std::abs(rvp1[0]-rvp2[2]),a)+
			  std::pow(std::abs(rvp1[1]-rvp2[0]),a)+
			  std::pow(std::abs(rvp1[1]-rvp2[1]),a)+
			  std::pow(std::abs(rvp1[1]-rvp2[2]),a)+
			  std::pow(std::abs(rvp1[2]-rvp2[0]),a)+
			  std::pow(std::abs(rvp1[2]-rvp2[1]),a)+
			  std::pow(std::abs(rvp1[2]-rvp2[2]),a))/4;
      // Barlat equivalent stress
      const auto Psi   = seq*std::pow(Psi_a,1/a);
      // For the derivatives, the stress eigenvalues are normalised by
      // the Barlat equivalent stress
      const auto rvp1b = vp1/Psi;
      const auto rvp2b = vp2/Psi;
      const tfel::math::tvector<9u,real> drvpb_am2 =
	{std::pow(std::abs(rvp1b[0]-rvp2b[0]),a-2),
	 std::pow(std::abs(rvp1b[0]-rvp2b[1]),a-2),
	 std::pow(std::abs(rvp1b[0]-rvp2b[2]),a-2),
	 std::pow(std::abs(rvp1b[1]-rvp2b[0]),a-2),
	 std::pow(std::abs(rvp1b[1]-rvp2b[1]),a-2),
	 std::pow(std::abs(rvp1b[1]-rvp2b[2]),a-2),
	 std::pow(std::abs(rvp1b[2]-rvp2b[0]),a-2),
	 std::pow(std::abs(rvp1b[2]-rvp2b[1]),a-2),
	 std::pow(std::abs(rvp1b[2]-rvp2b[2]),a-2)};
      const tfel::math::tvector<3u,real> dPsi_dsvp1 =
	{((rvp1b[0]-rvp2b[0])*drvpb_am2[0]+
	  (rvp1b[0]-rvp2b[1])*drvpb_am2[1]+
	  (rvp1b[0]-rvp2b[2])*drvpb_am2[2])/4,
	 ((rvp1b[1]-rvp2b[0])*drvpb_am2[3]+
	  (rvp1b[1]-rvp2b[1])*drvpb_am2[4]+
	  (rvp1b[1]-rvp2b[2])*drvpb_am2[5])/4,
	 ((rvp1b[2]-rvp2b[0])*drvpb_am2[6]+
	  (rvp1b[2]-rvp2b[1])*drvpb_am2[7]+
	  (rvp1b[2]-rvp2b[2])*drvpb_am2[8])/4};
      const tfel::math::tvector<3u,real> dPsi_dsvp2 =
	{((rvp2b[0]-rvp1b[0])*drvpb_am2[0]+
	  (rvp2b[0]-rvp1b[1])*drvpb_am2[3]+
	  (rvp2b[0]-rvp1b[2])*drvpb_am2[6])/4,
	 ((rvp2b[1]-rvp1b[0])*drvpb_am2[1]+
	  (rvp2b[1]-rvp1b[1])*drvpb_am2[4]+
	  (rvp2b[1]-rvp1b[2])*drvpb_am2[7])/4,	 
	 ((rvp2b[2]-rvp1b[0])*drvpb_am2[2]+
	  (rvp2b[2]-rvp1b[1])*drvpb_am2[5]+
	  (rvp2b[2]-rvp1b[2])*drvpb_am2[8])/4};
      const auto dPsi_ds1 = dPsi_dsvp1[0]*std::get<0>(n1)+
	dPsi_dsvp1[1]*std::get<1>(n1)+
	dPsi_dsvp1[2]*std::get<2>(n1);
      const auto dPsi_ds2 = dPsi_dsvp2[0]*std::get<0>(n2)+
	dPsi_dsvp2[1]*std::get<1>(n2)+
	dPsi_dsvp2[2]*std::get<2>(n2);
      return std::make_tuple(Psi,eval(dPsi_ds1*l1+dPsi_ds2*l2));
    } // end of computeBarlatStressNormal

    template<unsigned short N, typename real>
    tfel::math::st2tost2<N,real>
    makeBarlatLinearTransformation(const real c12,const real c21, const real c13,
				   const real c31,const real c23, const real c32,
				   const real c44,const real c55, const real c66)
    {
      return internals::BarlatLinearTransformation<N>::exe(c12,c21,c13,c31,c23,
							   c32,c44,c55,c66);
    } // end of makeBarlatLinearTransformationType

    
  } // end of namespace material
  
} // end of namespace tfel

#endif /* LIB_TFEL_MATERIAL_BARLAT_IXX */
