/*! 
 * \file  FiniteStrainBehaviourTangentOperatorConversion.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 18 juin 2014
 */

#include"MFront/FiniteStrainBehaviourTangentOperatorConversion.hxx"

namespace mfront
{

  FiniteStrainBehaviourTangentOperatorConversion::FiniteStrainBehaviourTangentOperatorConversion(const TangentOperatorFlag f,
												 const TangentOperatorFlag t,
												 const std::string& s)
    : b(f),
      e(t),
      c(s)
  {} // end of FiniteStrainBehaviourTangentOperatorConversion::FiniteStrainBehaviourTangentOperatorConversion

  FiniteStrainBehaviourTangentOperatorConversion::FiniteStrainBehaviourTangentOperatorConversion(const FiniteStrainBehaviourTangentOperatorConversion& src)
    : b(src.b),
      e(src.e),
      c(src.c)
  {} // end of FiniteStrainBehaviourTangentOperatorConversion::FiniteStrainBehaviourTangentOperatorConversion

  FiniteStrainBehaviourTangentOperatorConversion&
  FiniteStrainBehaviourTangentOperatorConversion::operator=(const FiniteStrainBehaviourTangentOperatorConversion& src)
  {
    this->b = src.b;
    this->e = src.e;
    this->c = src.c;
    return *this;
  } // end of FiniteStrainBehaviourTangentOperatorConversion::operator=

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

  std::vector<FiniteStrainBehaviourTangentOperatorConversion>
  FiniteStrainBehaviourTangentOperatorConversion::getAvailableFiniteStrainBehaviourTangentOperatorConversions(void)
  {
    using namespace std;
    typedef FiniteStrainBehaviourTangentOperatorConversion Converter;
    vector<Converter> converters;
    // converters.push_back(Converter(TangentOperator::DSIG_DF,TangentOperator::DSIG_DDF,
    // 				   "TangentOperator_dsig_ddF = TangentOperator_dsig_ddF*t2tot2<N,real>::tpld(this->F0);"));
    // converters.push_back(Converter(TangentOperator::DSIG_DF,TangentOperator::DTAU_DF,
    // 				   "computeKirchoffStressDerivativeFromCauchyStressDerivative(TangentOperator_dsig_dF,TangentOperator_dtau_dF,this->sig,this->F1);"));
    // converters.push_back(Converter(TangentOperator::DTAU_DF,TangentOperator::DSIG_DF,
    // 				   "computeCauchyStressDerivativeFromKirchoffStressDerivative(TangentOperator_dsig_dF,TangentOperator_dtau_dF,this->sig,this->F1);"));
    // converters.push_back(Converter(TangentOperator::DTAU_DF,TangentOperator::DTAU_DDF,
    // 				   "TangentOperator_dtau_ddF = TangentOperator_dtau_ddF*t2tot2<N,real>::tpld(this->F0);"));
    // converters.push_back(Converter(TangentOperator::DS_DC,TangentOperator::DS_DF,
    // 				   "TangentOperator_dS_dF = TangentOperator_dS_dC*t2tost2<N,strain>::dCdF(this->F1)"));
    // converters.push_back(Converter(TangentOperator::DS_DF,TangentOperator::DTAU_DF,
    // 				   "computePushForwardDerivative(TangentOperator_dtau_dF,(TangentOperator_dS_dF,/* second pioa-kirchhoff stress */,this->F1);"));
    return converters;
  } // end of FiniteStrainBehaviourTangentOperatorConversion::getAvailableFiniteStrainBehaviourTangentOperatorConversions

} // end of namespace mfront
