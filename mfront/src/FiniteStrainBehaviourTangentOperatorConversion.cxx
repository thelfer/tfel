/*! 
 * \file  mfront/src/FiniteStrainBehaviourTangentOperatorConversion.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 18 juin 2014
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<ostream>
#include"MFront/FiniteStrainBehaviourTangentOperatorConversion.hxx"

namespace mfront
{

  FiniteStrainBehaviourTangentOperatorConversion::FiniteStrainBehaviourTangentOperatorConversion(const TangentOperatorFlag f,
												 const TangentOperatorFlag t,
												 const std::string& s,
												 const std::string& s2)
    : b(f),
      e(t),
      c(s),
      cf(s2)
  {} // end of FiniteStrainBehaviourTangentOperatorConversion::FiniteStrainBehaviourTangentOperatorConversion

  FiniteStrainBehaviourTangentOperatorConversion::TangentOperatorFlag
  FiniteStrainBehaviourTangentOperatorConversion::from(void) const
  {
    return this->b; 
  } // end of FiniteStrainBehaviourTangentOperatorConversion::from

  FiniteStrainBehaviourTangentOperatorConversion::TangentOperatorFlag
  FiniteStrainBehaviourTangentOperatorConversion::to(void) const
  {
    return this->e;
  } // end of FiniteStrainBehaviourTangentOperatorConversion::to

  const std::string&
  FiniteStrainBehaviourTangentOperatorConversion::getIntermediateConversion(void) const
  {
    return this->c;
  }

  const std::string&
  FiniteStrainBehaviourTangentOperatorConversion::getFinalConversion(void) const
  {
    return this->cf;
  }

  std::vector<FiniteStrainBehaviourTangentOperatorConversion>
  FiniteStrainBehaviourTangentOperatorConversion::getAvailableFiniteStrainBehaviourTangentOperatorConversions(void)
  {
    using TangentOperator = tfel::material::FiniteStrainBehaviourTangentOperatorBase ;
    using Converter = FiniteStrainBehaviourTangentOperatorConversion ;
    std::vector<Converter> converters;
    auto add = [&converters](Converter&& c){
      converters.push_back(c);
    };
    auto std_add = [&add](const TangentOperator::Flag op1,
			  const TangentOperator::Flag op2){
      const auto s1 = convertFiniteStrainBehaviourTangentOperatorFlagToString(op1);
      const auto s2 = convertFiniteStrainBehaviourTangentOperatorFlagToString(op2);
      add({op1,op2,
	    "const auto tangentOperator_"+s2+" = convert<"+s2+","+s1+">(tangentOperator_"+s1+",this->F0,this->F1,this->sig);",
	    "this->Dt = convert<"+s2+","+s1+">(tangentOperator_"+s1+",this->F0,this->F1,this->sig);"
	    });
    };
    add({TangentOperator::DSIG_DF,TangentOperator::DSIG_DDF,
	  "const t2tost2<N,stress> tangentOperator_DSIG_DDF = tangentOperator_DSIG_DF*t2tot2<N,real>::tpld(this->F0);",
	  "this->Dt = tangentOperator_DSIG_DF*t2tot2<N,real>::tpld(this->F0);"});
    add({TangentOperator::DSIG_DF,TangentOperator::DTAU_DF,
	  "t2tost2<N,stress> tangentOperator_DTAU_DF;\n"
	  "computeKirchhoffStressDerivativeFromCauchyStressDerivative(tangentOperator_DTAU_DF,tangentOperator_DSIG_DF,this->sig,this->F1);",
	  "this->Dt.template set_uninitialised<t2tost2<N,stress> >();\n"
	  "computeKirchhoffStressDerivativeFromCauchyStressDerivative(this->Dt.template get<t2tost2<N,stress> >(),tangentOperator_DSIG_DF,this->sig,this->F1);"});
    add({TangentOperator::DTAU_DF,TangentOperator::DSIG_DF,
	  "t2tost2<N,stress> tangentOperator_DSIG_DF;\n"
	  "computeCauchyStressDerivativeFromKirchhoffStressDerivative(tangentOperator_DSIG_DF,tangentOperator_DTAU_DF,this->sig,this->F1);",
	  "this->Dt.template set_uninitialised<t2tost2<N,stress> >();\n"
	  "computeCauchyStressDerivativeFromKirchhoffStressDerivative(this->Dt.template get<t2tost2<N,stress> >(),tangentOperator_DTAU_DF,this->sig,this->F1);"});
    add({TangentOperator::DTAU_DF,TangentOperator::ABAQUS,
	  "const auto J_ABAQUS = det(this->F1);\n"
	  "const auto tau_ABAQUS = unsyme(this->sig/J_ABAQUS);\n"
	  "const auto dW_ABAQUS = computeSpinRateDerivative(this->F1);\n"
	  "const auto dW_tau_ABAQUS = convertToT2toST2(-t2tot2<N,real>::tpld(tau_ABAQUS,dW_ABAQUS)+"
	  "                                             t2tot2<N,real>::tprd(tau_ABAQUS,dW_ABAQUS));\n"
	  "const auto K_ABAQUS = tangentOperator_DTAU_DF+dW_tau_ABAQUS;\n"
	  "const auto tangentOperator_ABAQUS = convertToTangentModuli(K_ABAQUS,this->F1)/det(this->F1);\n",
	  "const real J_ABAQUS = det(this->F1);\n"
	  "const auto tau_ABAQUS = unsyme(this->sig/J_ABAQUS);\n"
	  "const auto dW_ABAQUS = computeSpinRateDerivative(this->F1);\n"
	  "const auto dW_tau_ABAQUS = convertToT2toST2( t2tot2<N,stress>::tprd(tau_ABAQUS,dW_ABAQUS)"
	  "                                            -t2tot2<N,stress>::tpld(tau_ABAQUS,dW_ABAQUS));\n"
	  "const auto K_ABAQUS = tangentOperator_DTAU_DF+dW_tau_ABAQUS;\n"
	  "this->Dt.template set_uninitialised<st2tost2<N,stress> >();\n"
	  "this->Dt.template get<st2tost2<N,stress> >() = convertToTangentModuli(K_ABAQUS,this->F1)/det(this->F1);"});
    add({TangentOperator::DTAU_DF,TangentOperator::DTAU_DDF,
	  "const t2tost2<N,stress> tangentOperator_DTAU_DDF = tangentOperator_DTAU_DF*t2tot2<N,real>::tpld(this->F0);",
	  "this->Dt = tangentOperator_DTAU_DF*t2tot2<N,real>::tpld(this->F0);"});
    add({TangentOperator::DTAU_DDF,TangentOperator::DTAU_DF,
	  "const tensor<N,real> inv_F0_DTAU_DF = invert(this->F0);\n"
	  "const t2tost2<N,stress> tangentOperator_DTAU_DF = tangentOperator_DTAU_DDF*t2tot2<N,real>::tpld(inv_F0_DTAU_DF);",
	  "const tensor<N,real> inv_F0_DTAU_DF = invert(this->F0);\n"
	  "this->Dt = tangentOperator_DTAU_DDF*t2tot2<N,real>::tpld(inv_F0_DTAU_DF);"});
    add({TangentOperator::DS_DC,TangentOperator::DS_DF,
	  "const t2tost2<N,stress> tangentOperator_DS_DF = tangentOperator_DS_DC*t2tost2<N,strain>::dCdF(this->F1);",
	  "this->Dt = tangentOperator_DS_DC*t2tost2<N,real>::dCdF(this->F1);"});
    add({TangentOperator::DS_DF,TangentOperator::DTAU_DF,
	  "t2tost2<N,stress> tangentOperator_DTAU_DF;\n"
	  "StressStensor tangentOperator_sk2 = convertCauchyStressToSecondPiolaKirchhoffStress(this->sig,this->F1);\n"
	  "computePushForwardDerivative(tangentOperator_DTAU_DF,tangentOperator_DS_DF,tangentOperator_sk2,this->F1);",
	  "StressStensor tangentOperator_sk2 = convertCauchyStressToSecondPiolaKirchhoffStress(this->sig,this->F1);\n"
	  "computePushForwardDerivative(this->Dt.template get<t2tost2<N,stress> >(),tangentOperator_DS_DF,tangentOperator_sk2,this->F1);"});
    add({TangentOperator::SPATIAL_MODULI,TangentOperator::DS_DEGL,
	  "const st2tost2<N,stress> tangentOperator_DS_DEGL = pull_back(tangentOperator_SPATIAL_MODULI,this->F1);",
	  "this->Dt = pull_back(tangentOperator_SPATIAL_MODULI,this->F1);"});
    add({TangentOperator::DS_DEGL,TangentOperator::SPATIAL_MODULI,
	  "const st2tost2<N,stress> tangentOperator_SPATIAL_MODULI = push_forward(tangentOperator_DS_DEGL,this->F1);",
	  "this->Dt = push_forward(tangentOperator_DS_DEGL,this->F1);"});
    add({TangentOperator::SPATIAL_MODULI,TangentOperator::ABAQUS,
	  "const st2tost2<N,stress> tangentOperator_ABAQUS = convertSpatialModuliToKirchhoffJaumanRateModuli(tangentOperator_SPATIAL_MODULI,this->sig*det(this->F1))/det(this->F1);",
	  "this->Dt = convertSpatialModuliToKirchhoffJaumanRateModuli(tangentOperator_SPATIAL_MODULI,this->sig*det(this->F1))/det(this->F1);"});
    add({TangentOperator::SPATIAL_MODULI,TangentOperator::C_TRUESDELL,
	  "const st2tost2<N,stress> tangentOperator_C_TRUESDELL = tangentOperator_SPATIAL_MODULI/(det(this->F1));",
	  "this->Dt = tangentOperator_SPATIAL_MODULI/(det(this->F1));"});
    add({TangentOperator::C_TRUESDELL,TangentOperator::SPATIAL_MODULI,
	  "const st2tost2<N,stress> tangentOperator_SPATIAL_MODULI = tangentOperator_C_TRUESDELL*det(this->F1);",
	  "this->Dt = tangentOperator_C_TRUESDELL*det(this->F1);"});
    add({TangentOperator::DTAU_DF,TangentOperator::C_TRUESDELL,
	  "const real J = det(this->F1);\n"
	  "const st2tost2<N,stress> Cstar=st2tost2<N,real>::dsquare(this->sig)-((this->sig)^Stensor::Id());\n"
	  "const st2tost2<N,stress> tangentOperator_C_TRUESDELL = st2tost2<N,real>::convert((1/J)*tangentOperator_DTAU_DF*t2tot2<N,real>::tprd(transpose(this->F1))) - Cstar;\n",
	  "const real J = det(this->F1);\n"
	  "const st2tost2<N,stress> Cstar=st2tost2<N,real>::dsquare(this->sig)-((this->sig)^Stensor::Id());\n"
	  "this->Dt = st2tost2<N,real>::convert((1/J)*tangentOperator_DTAU_DF*t2tot2<N,real>::tprd(transpose(this->F1))) - Cstar;"});
    std_add(TangentOperator::DS_DC,TangentOperator::DS_DEGL);
    std_add(TangentOperator::DS_DEGL,TangentOperator::DS_DC);
    return converters;
  } // end of FiniteStrainBehaviourTangentOperatorConversion::getAvailableFiniteStrainBehaviourTangentOperatorConversions
  
  FiniteStrainBehaviourTangentOperatorConversion::~FiniteStrainBehaviourTangentOperatorConversion() noexcept
  {} // end of FiniteStrainBehaviourTangentOperatorConversion::~FiniteStrainBehaviourTangentOperatorConversion

} // end of namespace mfront
