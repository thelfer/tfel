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
												 const std::string& s,
												 const std::string& s2)
    : b(f),
      e(t),
      c(s),
      cf(s2)
  {} // end of FiniteStrainBehaviourTangentOperatorConversion::FiniteStrainBehaviourTangentOperatorConversion

  FiniteStrainBehaviourTangentOperatorConversion::FiniteStrainBehaviourTangentOperatorConversion(const FiniteStrainBehaviourTangentOperatorConversion& src)
    : b(src.b),
      e(src.e),
      c(src.c),
      cf(src.cf)
  {} // end of FiniteStrainBehaviourTangentOperatorConversion::FiniteStrainBehaviourTangentOperatorConversion

  FiniteStrainBehaviourTangentOperatorConversion&
  FiniteStrainBehaviourTangentOperatorConversion::operator=(const FiniteStrainBehaviourTangentOperatorConversion& src)
  {
    this->b  = src.b;
    this->e  = src.e;
    this->c  = src.c;
    this->cf = src.cf;
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
    converters.push_back(Converter(TangentOperator::DTAU_DF,TangentOperator::DTAU_DDF,
    				   "const t2tost2<N,stress> tangentOperator_DTAU_DDF = tangentOperator_DTAU_DF*t2tot2<N,real>::tpld(this->F0);",
				   "this->Dt = tangentOperator_DTAU_DF*t2tot2<N,real>::tpld(this->F0);"));
    converters.push_back(Converter(TangentOperator::DS_DC,TangentOperator::DS_DF,
    				   "const t2tost2<N,stress> tangentOperator_DS_DF = tangentOperator_DS_DC*t2tost2<N,strain>::dCdF(this->F1);",
				   "this->Dt = tangentOperator_DS_DC*t2tost2<N,strain>::dCdF(this->F1);"));
    converters.push_back(Converter(TangentOperator::DS_DF,TangentOperator::DTAU_DF,
    				   "t2tost2<N,stress> tangentOperator_DTAU_DF;\n"
				   "StressStensor tangentOperator_sk2 = convertCauchyStressToSecondPiolaKirchhoffStress(this->sig);"
				   "computePushForwardDerivative(tangentOperator_DTAU_DF,(tangentOperator_DS_DF,tangentOperator_sk2,this->F1);",
				   "computePushForwardDerivative(this->Dt.template get<t2tost2<N,stress> >(),tangentOperator_DS_DF,tangentOperator_sk2,this->F1);"));
    converters.push_back(Converter(TangentOperator::DS_DEGL,TangentOperator::DS_DC,
    				   "const st2tost2<N,stress> tangentOperator_DS_DC = 0.5*tangentOperator_DS_DEGL;",
				   "this->Dt = 0.5*tangentOperator_DS_DEGL;"));
    converters.push_back(Converter(TangentOperator::DS_DC,TangentOperator::DS_DEGL,
    				   "const st2tost2<N,stress> tangentOperator_DS_DEGL = 2*tangentOperator_DS_DC;",
				   "this->Dt = 2*tangentOperator_DS_DC;"));
    return converters;
  } // end of FiniteStrainBehaviourTangentOperatorConversion::getAvailableFiniteStrainBehaviourTangentOperatorConversions

} // end of namespace mfront
