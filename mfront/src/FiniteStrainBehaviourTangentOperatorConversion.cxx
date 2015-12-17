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
    using namespace std;
    using namespace tfel::material;
    typedef FiniteStrainBehaviourTangentOperatorBase TangentOperator;
    typedef FiniteStrainBehaviourTangentOperatorConversion Converter;
    vector<Converter> converters;
    converters.push_back(Converter(TangentOperator::DSIG_DF,TangentOperator::DSIG_DDF,
    				   "const t2tost2<N,stress> tangentOperator_DSIG_DDF = tangentOperator_DSIG_DF*t2tot2<N,real>::tpld(this->F0);",
				   "this->Dt = tangentOperator_DSIG_DF*t2tot2<N,real>::tpld(this->F0);"));
    converters.push_back(Converter(TangentOperator::DSIG_DF,TangentOperator::DTAU_DF,
    				   "t2tost2<N,stress> tangentOperator_DTAU_DF;\n"
				   "computeKirchoffStressDerivativeFromCauchyStressDerivative(tangentOperator_DTAU_DF,tangentOperator_DSIG_DF,this->sig,this->F1);",
				   "this->Dt.template set_uninitialised<t2tost2<N,stress> >();\n"
				   "computeKirchoffStressDerivativeFromCauchyStressDerivative(this->Dt.template get<t2tost2<N,stress> >(),tangentOperator_DSIG_DF,this->sig,this->F1);"));
    converters.push_back(Converter(TangentOperator::DTAU_DF,TangentOperator::DSIG_DF,
    				   "t2tost2<N,stress> tangentOperator_DSIG_DF;\n"
    				   "computeCauchyStressDerivativeFromKirchoffStressDerivative(tangentOperator_DSIG_DF,tangentOperator_DTAU_DF,this->sig,this->F1);",
				   "this->Dt.template set_uninitialised<t2tost2<N,stress> >();\n"
				   "computeCauchyStressDerivativeFromKirchoffStressDerivative(this->Dt.template get<t2tost2<N,stress> >(),tangentOperator_DTAU_DF,this->sig,this->F1);"));
    converters.push_back(Converter(TangentOperator::DTAU_DF,TangentOperator::ABAQUS,
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
				   "this->Dt.template get<st2tost2<N,stress> >() = convertToTangentModuli(K_ABAQUS,this->F1)/det(this->F1);"));
    converters.push_back(Converter(TangentOperator::DTAU_DF,TangentOperator::DTAU_DDF,
    				   "const t2tost2<N,stress> tangentOperator_DTAU_DDF = tangentOperator_DTAU_DF*t2tot2<N,real>::tpld(this->F0);",
				   "this->Dt = tangentOperator_DTAU_DF*t2tot2<N,real>::tpld(this->F0);"));
    converters.push_back(Converter(TangentOperator::DTAU_DDF,TangentOperator::DTAU_DF,
    				   "const tensor<N,real> inv_F0_DTAU_DF = invert(this->F0);\n"
				   "const t2tost2<N,stress> tangentOperator_DTAU_DF = tangentOperator_DTAU_DDF*t2tot2<N,real>::tpld(inv_F0_DTAU_DF);",
    				   "const tensor<N,real> inv_F0_DTAU_DF = invert(this->F0);\n"
				   "this->Dt = tangentOperator_DTAU_DDF*t2tot2<N,real>::tpld(inv_F0_DTAU_DF);"));
    converters.push_back(Converter(TangentOperator::DS_DC,TangentOperator::DS_DF,
    				   "const t2tost2<N,stress> tangentOperator_DS_DF = tangentOperator_DS_DC*t2tost2<N,strain>::dCdF(this->F1);",
				   "this->Dt = tangentOperator_DS_DC*t2tost2<N,real>::dCdF(this->F1);"));
    converters.push_back(Converter(TangentOperator::DS_DF,TangentOperator::DTAU_DF,
    				   "t2tost2<N,stress> tangentOperator_DTAU_DF;\n"
				   "StressStensor tangentOperator_sk2 = convertCauchyStressToSecondPiolaKirchhoffStress(this->sig,this->F1);\n"
				   "computePushForwardDerivative(tangentOperator_DTAU_DF,tangentOperator_DS_DF,tangentOperator_sk2,this->F1);",
				   "StressStensor tangentOperator_sk2 = convertCauchyStressToSecondPiolaKirchhoffStress(this->sig,this->F1);\n"
				   "computePushForwardDerivative(this->Dt.template get<t2tost2<N,stress> >(),tangentOperator_DS_DF,tangentOperator_sk2,this->F1);"));
    converters.push_back(Converter(TangentOperator::DS_DEGL,TangentOperator::DS_DC,
    				   "const st2tost2<N,stress> tangentOperator_DS_DC = 0.5*tangentOperator_DS_DEGL;",
				   "this->Dt = 0.5*tangentOperator_DS_DEGL;"));
    converters.push_back(Converter(TangentOperator::DS_DC,TangentOperator::DS_DEGL,
    				   "const st2tost2<N,stress> tangentOperator_DS_DEGL = 2*tangentOperator_DS_DC;",
				   "this->Dt = 2*tangentOperator_DS_DC;"));
    converters.push_back(Converter(TangentOperator::DTAU_DF,TangentOperator::DSIG_DD,
				   "const real J = det(this->F1);\n"
				   "const st2tost2<N,stress> Cstar=st2tost2<N,real>::dsquare(this->sig)-((this->sig)^Stensor::Id());\n"
				   "const st2tost2<N,stress> tangentOperator_DSIG_DD = st2tost2<N,real>::convert((1/J)*tangentOperator_DTAU_DF*t2tot2<N,real>::tprd(transpose(this->F1))) - Cstar;\n",
				   "const real J = det(this->F1);\n"
				   "const st2tost2<N,stress> Cstar=st2tost2<N,real>::dsquare(this->sig)-((this->sig)^Stensor::Id());\n"
				   "this->Dt = st2tost2<N,real>::convert((1/J)*tangentOperator_DTAU_DF*t2tot2<N,real>::tprd(transpose(this->F1))) - Cstar;"));
    return converters;
  } // end of FiniteStrainBehaviourTangentOperatorConversion::getAvailableFiniteStrainBehaviourTangentOperatorConversions
  
  FiniteStrainBehaviourTangentOperatorConversion::~FiniteStrainBehaviourTangentOperatorConversion() noexcept
  {} // end of FiniteStrainBehaviourTangentOperatorConversion::~FiniteStrainBehaviourTangentOperatorConversion

} // end of namespace mfront
