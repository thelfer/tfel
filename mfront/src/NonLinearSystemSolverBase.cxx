/*! 
 * \file  mfront/src/NonLinearSystemSolverBase.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 22 août 2014
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<cmath>
#include<limits>
#include<sstream>

#include"MFront/BehaviourDescription.hxx"
#include"MFront/NonLinearSystemSolverBase.hxx"

namespace mfront{

  std::vector<std::string>
  NonLinearSystemSolverBase::getReservedNames() const
  {
    return {"jacobian_error","error","iter","iterMax","converged"};
  } // end of NonLinearSystemSolverBase::getReservedNames

  std::string
  NonLinearSystemSolverBase::getJacobianPart(const BehaviourDescription& mb,
					     const VariableDescription&  v1,
					     const VariableDescription&  v2,
					     const SupportedTypes::TypeSize& n,
					     const SupportedTypes::TypeSize& n2,
					     const SupportedTypes::TypeSize& n3,
					     const std::string& j,
					     const std::string& p)
  {
    using namespace std;
    ostringstream d;
    if(mb.getTypeFlag(v1.type)==SupportedTypes::Stensor){
      if(mb.getTypeFlag(v2.type)==SupportedTypes::Stensor){
	d << "typename tfel::math::ST2toST2FromTinyMatrixView<N,"
	  << n2 << "," << n2 << ",\n"
	  << n  << "," << n3
	  << ",real>::type "+p+"df" << v1.name << "_dd" << v2.name << "("+j+");\n";
      } else if(mb.getTypeFlag(v2.type)==SupportedTypes::Scalar){
	d << "typename tfel::math::StensorFromTinyMatrixColumnView<N," 
	  << n2 << "," << n2 << ",\n"
	  << n  << "," << n3
	  << ",real>::type "+p+"df" << v1.name << "_dd" << v2.name << "("+j+");\n";
      } else {
	string msg("NonLinearSystemSolverBase::getJacobianPart : ");
	msg += "unsupported type for integration variable ";
	msg += v2.name;
	throw(runtime_error(msg));
      }
    } else if(mb.getTypeFlag(v1.type)==SupportedTypes::Scalar){
      if(mb.getTypeFlag(v2.type)==SupportedTypes::Stensor){
	d << "typename tfel::math::StensorFromTinyMatrixRowView<N,"
	  << n2 << "," << n2 << ",\n"
	  << n  << "," << n3
	  << ",real>::type "+p+"df" << v1.name 
	  << "_dd" << v2.name << "("+j+");\n";
      } else if(mb.getTypeFlag(v2.type)==SupportedTypes::Scalar){
	d << "real& "+p+"df" << v1.name << "_dd" << v2.name 
	  << " = "+j+"(" << n << "," << n3 << ");\n";
      } else {
	string msg("NonLinearSystemSolverBase::getJacobianPart : ");
	msg += "unsupported type for integration variable ";
	msg += v2.name;
	throw(runtime_error(msg));
      }
    } else {
      string msg("NonLinearSystemSolverBase::getJacobianPart : ");
      msg += "unsupported type for integration variable ";
      msg += v1.name;
      throw(runtime_error(msg));
    }
    return d.str();
  } // end of NonLinearSystemSolverBase::getJacobianPart

  void
  NonLinearSystemSolverBase::writeComparisonToNumericalJacobian(std::ostream& out,
								const BehaviourDescription& mb,
								const Hypothesis h,
								const std::string& nj)
  {
    const auto& d = mb.getBehaviourData(h);
    SupportedTypes::TypeSize n;
    SupportedTypes::TypeSize n3;
    const auto n2 = d.getIntegrationVariables().getTypeSize();
    if(mb.getAttribute(h,BehaviourData::compareToNumericalJacobian,false)){
      out << "auto jacobian_error = real{};"
	  << "this->computeNumericalJacobian(" << nj << ");\n";
      n = SupportedTypes::TypeSize();
      for(const auto& v : d.getIntegrationVariables()){
	if(v.arraySize==1){
	  n3 = SupportedTypes::TypeSize();
	  for(const auto& v2 : d.getIntegrationVariables()){
	    if((v.arraySize==1)&&(v2.arraySize==1)){
	      out << "// derivative of variable f" << v.name 
		  << " by variable " << v2.name << "\n"
		  << NonLinearSystemSolverBase::getJacobianPart(mb,v,v2,n,n2,n3)
		  << "// numerical derivative of variable f" << v.name 
		  << " by variable " << v2.name << "\n"
		  << NonLinearSystemSolverBase::getJacobianPart(mb,v,v2,n,n2,n3,
								nj,"n");
	      n3 += mb.getTypeSize(v2.type,v2.arraySize);
	    }
	  }
	}
	n += mb.getTypeSize(v.type,v.arraySize);
      }
      for(const auto& v1 : d.getIntegrationVariables()){
	for(const auto& v2 : d.getIntegrationVariables()){
	  auto nv1 = mb.getTypeSize(v1.type,1u);
	  auto nv2 = mb.getTypeSize(v2.type,1u);
	  out << "jacobian_error=" << nv1 << "*" << nv2 << "*"
	      << "(this->jacobianComparisonCriterion)" <<";\n";
	  if((v1.arraySize==1u)&&(v2.arraySize==1u)){
	    out << "if(abs(" << "df" << v1.name  << "_dd" << v2.name << "-"
		<< "ndf" << v1.name  << "_dd" << v2.name << ") > jacobian_error)\n" 
		<< "{\n";
	    out << "cout << abs(" << "df" << v1.name  << "_dd" << v2.name << "-"
		<< "ndf" << v1.name  << "_dd" << v2.name << ") << \" \" << jacobian_error << endl;\n";
	    out << "cout << \"df" << v1.name
		<< "_dd" << v2.name << " :\\n\" << " 
		<< "df" << v1.name  << "_dd" << v2.name << " << endl;\n";
	    out << "cout << \"ndf" << v1.name
		<< "_dd" << v2.name << " :\\n\" << " 
		<< "ndf" << v1.name  << "_dd" << v2.name << " << endl;\n";
	    out << "cout << \"df" << v1.name << "_dd" << v2.name 
		<< " - ndf" << v1.name << "_dd" << v2.name << " :\\n\" << "
		<< "df" << v1.name  << "_dd" << v2.name << "-"
		<< "ndf" << v1.name  << "_dd" << v2.name << " << endl;\n";
	    out << "cout << endl;\n";
	    out << "}\n";
	  } else if(((v1.arraySize!=1u)&&(v2.arraySize==1u))||
		    ((v2.arraySize!=1u)&&(v1.arraySize==1u))){
	    unsigned short asize;
	    if(v1.arraySize!=1u){
	      asize = v1.arraySize;
	    } else {
	      asize = v2.arraySize;
	    }
	    out << "for(unsigned short idx=0;idx!=" << asize << ";++idx){\n";
	    out << "if(abs(" << "df" << v1.name  << "_dd" << v2.name << "(idx)-"
		<< "df" << v1.name  << "_dd" << v2.name << "(" << nj << ",idx)) > jacobian_error)\n" 
		<< "{\n";
	    out << "cout << abs(" << "df" << v1.name  << "_dd" << v2.name << "(idx)-"
		<< "df" << v1.name  << "_dd" << v2.name << "(" << nj << ",idx)) << \" \" << jacobian_error << endl;\n";
	    out << "cout << \"df" << v1.name
		<< "_dd" << v2.name << "(\" << idx << \") :\\n\" << " 
		<< "df" << v1.name  << "_dd" << v2.name << "(idx) << endl;\n";
	    out << "cout << \"ndf" << v1.name
		<< "_dd" << v2.name << "(\" << idx << \") :\\n\" << " 
		<< "df" << v1.name  << "_dd" << v2.name << "(" << nj << ",idx) << endl;\n";
	    out << "cout << \"df" << v1.name << "_dd" << v2.name 
		<< " - ndf" << v1.name << "_dd" << v2.name << "(\" << idx << \") :\\n\" << "
		<< "df" << v1.name  << "_dd" << v2.name << "(idx) -"
		<< "df" << v1.name  << "_dd" << v2.name << "(" << nj << ",idx) << endl;\n";
	    out << "cout << endl;\n";
	    out << "}\n";
	    out << "}\n";
	  } else {
	    out << "for(unsigned short idx=0;idx!=" << v1.arraySize << ";++idx){\n";
	    out << "for(unsigned short idx2=0;idx2!=" << v2.arraySize << ";++idx2){\n";
	    out << "if(abs(" << "df" << v1.name  << "_dd" << v2.name << "(idx,idx2)-"
		<< "df" << v1.name  << "_dd" << v2.name << "(" << nj << ",idx,idx2)) > jacobian_error)\n" 
		<< "{\n";
	    out << "cout << abs(" << "df" << v1.name  << "_dd" << v2.name << "(idx,idx2)-"
		<< "df" << v1.name  << "_dd" << v2.name << "(" << nj << ",idx,idx2)) << \" \" << jacobian_error << endl;\n";
	    out << "cout << \"df" << v1.name
		<< "_dd" << v2.name << "(\" << idx << \",\" << idx2 << \") :\\n\" << " 
		<< "df" << v1.name  << "_dd" << v2.name << "(idx,idx2) << endl;\n";
	    out << "cout << \"ndf" << v1.name
		<< "_dd" << v2.name << "(\" << idx << \",\" << idx2 << \") :\\n\" << " 
		<< "df" << v1.name  << "_dd" << v2.name << "(" << nj << ",idx,idx2) << endl;\n";
	    out << "cout << \"df" << v1.name << "_dd" << v2.name 
		<< " - ndf" << v1.name << "_dd" << v2.name << "(\" << idx << \",\" << idx2 << \") :\\n\" << "
		<< "df" << v1.name  << "_dd" << v2.name << "(idx,idx2) -"
		<< "df" << v1.name  << "_dd" << v2.name << "(" << nj << ",idx,idx2) << endl;\n";
	    out << "cout << endl;\n";
	    out << "}\n";
	    out << "}\n";
	    out << "}\n";
	  }
	}
      }
    }
  } // end of NonLinearSystemSolverBase::writeComparisonToNumericalJacobian

  void
  NonLinearSystemSolverBase::writeLimitsOnIncrementValues(std::ostream& out,
							  const BehaviourDescription& mb,
							  const Hypothesis h,
							  const std::string& v)
  {
    using namespace std;
    const auto& d = mb.getBehaviourData(h);
    SupportedTypes::TypeSize n;
    for(const auto& iv : d.getIntegrationVariables()){
      const auto nv = mb.getTypeSize(iv.type,iv.arraySize);
      if(mb.hasParameter(h,iv.name+"_maximum_increment_value_per_iteration")){
	out << "for(unsigned short idx = 0; idx!=" << nv << ";++idx){\n";
	if(mb.hasAttribute(h,iv.name+"_normalisation_factor")){
	  const auto& nf = mb.getAttribute<string>(h,iv.name+"_normalisation_factor");
	  out << "if(std::abs(" << v << "[" << n 
	      << "+idx])>" << nf << "*(this->" << iv.name << "_maximum_increment_value_per_iteration)){\n";
	} else {
	  out << "if(std::abs(" << v << "[" << n 
	      << "+idx])>this->" << iv.name << "_maximum_increment_value_per_iteration){\n";
	}
	out << "if("<< v << "[" << n << "+idx]<0){\n";
	if(mb.hasAttribute(h,iv.name+"_normalisation_factor")){
	  const auto& nf = mb.getAttribute<string>(h,iv.name+"_normalisation_factor");
	  out << "" << v << "[" << n
	      << "+idx] = -"  << nf << "*(this->" << iv.name << "_maximum_increment_value_per_iteration);\n";
	} else {
	  out << "" << v << "[" << n
	      << "+idx] = -this->" << iv.name << "_maximum_increment_value_per_iteration;\n";
	}
	out << "} else {\n";
	if(mb.hasAttribute(h,iv.name+"_normalisation_factor")){
	  const auto& nf = mb.getAttribute<string>(h,iv.name+"_normalisation_factor");
	  out << "" << v << "[" << n
	      << "+idx] = " << nf << "*(this->" << iv.name << "_maximum_increment_value_per_iteration);\n";
	} else {
	  out << "" << v << "["
	      << n << "+idx] =  this->" << iv.name << "_maximum_increment_value_per_iteration;\n";
	}
	out << "}\n";
	out << "}\n";
	out << "}\n";
      } else if(mb.hasParameter(h,"maximum_increment_value_per_iteration")){
	out << "for(unsigned short idx = 0; idx!=" << nv << ";++idx){\n";
	out << "if(std::abs(" << v << "[" << n 
	    << "+idx])>this->maximum_increment_value_per_iteration){\n";
	out << "if("<< v << "[" << n << "+idx]<0){\n";
	out << "" << v << "[" << n
	    << "+idx] = -this->maximum_increment_value_per_iteration;\n";
	out << "} else {\n";
	out << "" << v << "["
	    << n << "+idx] =  this->maximum_increment_value_per_iteration;\n";
	out << "}\n";
	out << "}\n";
	out << "}\n";
      }
      n += nv;
    }
  } // end of NonLinearSystemSolverBase::writeLimitsOnIncrementValues
  
  void
  NonLinearSystemSolverBase::writeLimitsOnIncrementValuesBasedOnStateVariablesPhysicalBounds(std::ostream& out,
											     const BehaviourDescription& mb,
											     const Hypothesis h)
  {
    using namespace std;
    const auto& d = mb.getBehaviourData(h);
    auto n = SupportedTypes::TypeSize{};
    for(const auto& v : d.getStateVariables()){
      if(!v.hasPhysicalBounds()){
	n += mb.getTypeSize(v.type,v.arraySize);
	continue;
      }
      const auto& b = v.getPhysicalBounds();
      // treating lower bounds
      if((b.boundsType==VariableBoundsDescription::LOWER)||
	 (b.boundsType==VariableBoundsDescription::LOWERANDUPPER)){
	if((mb.getTypeFlag(v.type)==SupportedTypes::Scalar)&&(v.arraySize==1u)){
	  if(mb.hasAttribute(h,v.name+"_normalisation_factor")){
	    const auto& nf = mb.getAttribute<string>(h,v.name+"_normalisation_factor");
	    out << "if(this->" << v.name << "+ " << nf << "*(this->zeros[" << n << "]) <" << b.lowerBound << "){\n";
	  } else {
	    out << "if(this->" << v.name << "+this->zeros[" << n << "]<" << b.lowerBound << "){\n";
	  }
	  if(std::abs(b.lowerBound)>numeric_limits<long double>::min()){
	    if(mb.hasAttribute(h,v.name+"_normalisation_factor")){
	      const auto& nf = mb.getAttribute<string>(h,v.name+"_normalisation_factor");
	      out << "this->zeros[" << n << "] = (" << b.lowerBound << "- (this->" << v.name << "))/(" << nf << ");\n";
	    } else {
	      out << "this->zeros[" << n << "] = " << b.lowerBound << "- (this->" << v.name << ");\n";
	    }
	  } else {
	    if(mb.hasAttribute(h,v.name+"_normalisation_factor")){
	      const auto& nf = mb.getAttribute<string>(h,v.name+"_normalisation_factor");
	      out << "this->zeros[" << n << "] = - (this->" << v.name << ")/(" << nf << ");\n";
	    } else {
	      out << "this->zeros[" << n << "] = - (this->" << v.name << ");\n";
	    }
	  }
	  out << "}\n";
	}
	if((mb.getTypeFlag(v.type)==SupportedTypes::Scalar)&&(v.arraySize!=1u)){
	  out << "for(unsigned short idx=0;idx!=" << v.arraySize << ";++idx){" << endl;
	  if(mb.hasAttribute(h,v.name+"_normalisation_factor")){
	    const auto& nf = mb.getAttribute<string>(h,v.name+"_normalisation_factor");
	    out << "if(this->" << v.name << "[idx]+(" << nf << ")*(this->zeros[" << n << "+idx])<" << b.lowerBound << "){\n";
	  } else {
	    out << "if(this->" << v.name << "[idx]+this->zeros[" << n << "+idx]<" << b.lowerBound << "){\n";
	  }
	  if(std::abs(b.lowerBound)>numeric_limits<long double>::min()){
	    if(mb.hasAttribute(h,v.name+"_normalisation_factor")){
	      const auto& nf = mb.getAttribute<string>(h,v.name+"_normalisation_factor");
	      out << "this->zeros[" << n << "+idx] = (" << b.lowerBound << "- (this->" << v.name << "[idx]))/(" << nf <<");\n";
	    } else {
	      out << "this->zeros[" << n << "+idx] = " << b.lowerBound << "- (this->" << v.name << "[idx]);\n";
	    }
	  } else {
	    if(mb.hasAttribute(h,v.name+"_normalisation_factor")){
	      const auto& nf = mb.getAttribute<string>(h,v.name+"_normalisation_factor");
	      out << "this->zeros[" << n << "+idx] = - (this->" << v.name << "[idx])/(" << nf << ");\n";
	    } else {
	      out << "this->zeros[" << n << "+idx] = - (this->" << v.name << "[idx]);\n";
	    }
	  }
	  out << "}\n"
	      << "}\n";
	}
	if((mb.getTypeFlag(v.type)!=SupportedTypes::Scalar)&&(v.arraySize==1u)){
	  if(b.component==-1){
	    const auto n2 =  mb.getTypeSize(v.type,1u);
	    out << "for(unsigned short idx=0;idx!=" << n2 << ";++idx){" << endl;
	    if(mb.hasAttribute(h,v.name+"_normalisation_factor")){
	      const auto& nf = mb.getAttribute<string>(h,v.name+"_normalisation_factor");
	      out << "if(this->" << v.name << "[idx]+(" << nf << ")*(this->zeros[" << n << "+idx])<" << b.lowerBound << "){\n";
	    } else {
	      out << "if(this->" << v.name << "[idx]+this->zeros[" << n << "+idx]<" << b.lowerBound << "){\n";
	    }
	    if(std::abs(b.lowerBound)>numeric_limits<long double>::min()){
	      if(mb.hasAttribute(h,v.name+"_normalisation_factor")){
		const auto& nf = mb.getAttribute<string>(h,v.name+"_normalisation_factor");
		out << "this->zeros[" << n << "+idx] = (" << b.lowerBound << "- (this->" << v.name << "[idx]))/(" << nf << ");\n";
	      } else {
		out << "this->zeros[" << n << "+idx] = " << b.lowerBound << "- (this->" << v.name << "[idx]);\n";
	      }
	    } else {
	      if(mb.hasAttribute(h,v.name+"_normalisation_factor")){
		const auto& nf = mb.getAttribute<string>(h,v.name+"_normalisation_factor");
		out << "this->zeros[" << n << "+idx] = - (this->" << v.name << "[idx])/(" << nf << ");\n";
	      } else {
		out << "this->zeros[" << n << "+idx] = - (this->" << v.name << "[idx]);\n";
	      }
	    }
	    out << "}\n";
	    out << "}\n";
	  } else {
	    if(mb.hasAttribute(h,v.name+"_normalisation_factor")){
	      const auto& nf = mb.getAttribute<string>(h,v.name+"_normalisation_factor");
	      out << "if(this->" << v.name << "[" << b.component << "]+(" << nf << ")*(this->zeros[" << n << "+" << b.component << "])<" << b.lowerBound << "){\n";
	    } else {
	      out << "if(this->" << v.name << "[" << b.component << "]+this->zeros[" << n << "+" << b.component << "]<" << b.lowerBound << "){\n";
	    }
	    if(std::abs(b.lowerBound)>numeric_limits<long double>::min()){
	      if(mb.hasAttribute(h,v.name+"_normalisation_factor")){
		const auto& nf = mb.getAttribute<string>(h,v.name+"_normalisation_factor");
		out << "this->zeros[" << n << "+" << b.component << "] = (" << b.lowerBound << "- (this->" << v.name << "[" << b.component << "]))/(" << nf << ");\n";
	      } else {
		out << "this->zeros[" << n << "+" << b.component << "] = " << b.lowerBound << "- (this->" << v.name << "[" << b.component << "]);\n";
	      }
	    } else {
	      if(mb.hasAttribute(h,v.name+"_normalisation_factor")){
		const auto& nf = mb.getAttribute<string>(h,v.name+"_normalisation_factor");
		out << "this->zeros[" << n << "+" << b.component << "] = - (this->" << v.name << "[" << b.component << "])/(" << nf << ");\n";
	      } else {
		out << "this->zeros[" << n << "+" << b.component << "] = - (this->" << v.name << "[" << b.component << "]);\n";
	      }
	    }
	    out << "}\n";
	  }
	}
	if((mb.getTypeFlag(v.type)!=SupportedTypes::Scalar)&&(v.arraySize!=1u)){
	  const auto n2 =  mb.getTypeSize(v.type,1u);
	  out << "for(unsigned short idx=0;idx!=" << v.arraySize << ";++idx){" << endl;
	  if(b.component==-1){
	    out << "for(unsigned short idx2=0;idx2!=" << n2 << ";++idx2){" << endl;
	    if(mb.hasAttribute(h,v.name+"_normalisation_factor")){
	      const auto& nf = mb.getAttribute<string>(h,v.name+"_normalisation_factor");
	      out << "if(this->" << v.name << "[idx][idx2]+(" << nf << ")*(this->zeros[" << n << "+" << n2 << "*idx+idx2])<" << b.lowerBound << "){\n";
	    } else {
	      out << "if(this->" << v.name << "[idx][idx2]+this->zeros[" << n << "+" << n2 << "*idx+idx2]<" << b.lowerBound << "){\n";
	    }
	    if(std::abs(b.lowerBound)>numeric_limits<long double>::min()){
	      if(mb.hasAttribute(h,v.name+"_normalisation_factor")){
		const auto& nf = mb.getAttribute<string>(h,v.name+"_normalisation_factor");
		out << "this->zeros[" << n << "+" << n2 << "*idx+idx2] = (" << b.lowerBound << "- (this->" << v.name << "[idx][idx2]))/(" << nf << ");\n";
	      } else {
		out << "this->zeros[" << n << "+" << n2 << "*idx+idx2] = " << b.lowerBound << "- (this->" << v.name << "[idx][idx2]);\n";
	      }
	    } else {
	      if(mb.hasAttribute(h,v.name+"_normalisation_factor")){
		const auto& nf = mb.getAttribute<string>(h,v.name+"_normalisation_factor");
		out << "this->zeros[" << n << "+" << n2 << "*idx+idx2] = - (this->" << v.name << "[idx][idx2])/(" << nf << ");\n";
	      } else {
		out << "this->zeros[" << n << "+" << n2 << "*idx+idx2] = - (this->" << v.name << "[idx][idx2]);\n";
	      }
	    }
	    out << "}\n"
		<< "}\n";
	  } else {
	    if(mb.hasAttribute(h,v.name+"_normalisation_factor")){
	      const auto& nf = mb.getAttribute<string>(h,v.name+"_normalisation_factor");
	      out << "if(this->" << v.name << "[idx][" << b.component << "]+(" << nf << ")*(this->zeros[" << n << "+" << n2 << "*idx+" << b.component << "])<" << b.lowerBound << "){\n";
	    } else {
	      out << "if(this->" << v.name << "[idx][" << b.component << "]+this->zeros[" << n << "+" << n2 << "*idx+" << b.component << "]<" << b.lowerBound << "){\n";
	    }
	    if(std::abs(b.lowerBound)>numeric_limits<long double>::min()){
	      if(mb.hasAttribute(h,v.name+"_normalisation_factor")){
		const auto& nf = mb.getAttribute<string>(h,v.name+"_normalisation_factor");
		out << "this->zeros[" << n << "+" << n2 << "*idx+" << b.component << "] = (" << b.lowerBound << "- (this->" << v.name << "[idx][" << b.component << "]))/(" << nf << ");\n";
	      } else {
		out << "this->zeros[" << n << "+" << n2 << "*idx+" << b.component << "] = " << b.lowerBound << "- (this->" << v.name << "[idx][" << b.component << "]);\n";
	      }
	    } else {
	      if(mb.hasAttribute(h,v.name+"_normalisation_factor")){
		const auto& nf = mb.getAttribute<string>(h,v.name+"_normalisation_factor");
		out << "this->zeros[" << n << "+" << n2 << "*idx+" << b.component << "] = - (this->" << v.name << "[idx][" << b.component << "])/(" << nf << ");\n";
	      } else {
		out << "this->zeros[" << n << "+" << n2 << "*idx+" << b.component << "] = - (this->" << v.name << "[idx][" << b.component << "]);\n";
	      }
	    }
	    out << "}\n";
	  }
	  out << "}\n";
	}
      }
      // treating upper bounds
      if((b.boundsType==VariableBoundsDescription::UPPER)||
	 (b.boundsType==VariableBoundsDescription::LOWERANDUPPER)){
	if((mb.getTypeFlag(v.type)==SupportedTypes::Scalar)&&(v.arraySize==1u)){
	  if(mb.hasAttribute(h,v.name+"_normalisation_factor")){
	    const auto& nf = mb.getAttribute<string>(h,v.name+"_normalisation_factor");
	    out << "if(this->" << v.name << "+ " << nf << "*(this->zeros[" << n << "]) >" << b.upperBound << "){\n";
	  } else {
	    out << "if(this->" << v.name << "+this->zeros[" << n << "]>" << b.upperBound << "){\n";
	  }
	  if(std::abs(b.upperBound)>numeric_limits<long double>::min()){
	    if(mb.hasAttribute(h,v.name+"_normalisation_factor")){
	      const auto& nf = mb.getAttribute<string>(h,v.name+"_normalisation_factor");
	      out << "this->zeros[" << n << "] = (" << b.upperBound << "- (this->" << v.name << "))/(" << nf << ");\n";
	    } else {
	      out << "this->zeros[" << n << "] = " << b.upperBound << "- (this->" << v.name << ");\n";
	    }
	  } else {
	    if(mb.hasAttribute(h,v.name+"_normalisation_factor")){
	      const auto& nf = mb.getAttribute<string>(h,v.name+"_normalisation_factor");
	      out << "this->zeros[" << n << "] = - (this->" << v.name << ")/(" << nf << ");\n";
	    } else {
	      out << "this->zeros[" << n << "] = - (this->" << v.name << ");\n";
	    }
	  }
	  out << "}\n";
	}
	if((mb.getTypeFlag(v.type)==SupportedTypes::Scalar)&&(v.arraySize!=1u)){
	  out << "for(unsigned short idx=0;idx!=" << v.arraySize << ";++idx){" << endl;
	  if(mb.hasAttribute(h,v.name+"_normalisation_factor")){
	    const auto& nf = mb.getAttribute<string>(h,v.name+"_normalisation_factor");
	    out << "if(this->" << v.name << "[idx]+(" << nf << ")*(this->zeros[" << n << "+idx])>" << b.upperBound << "){\n";
	  } else {
	    out << "if(this->" << v.name << "[idx]+this->zeros[" << n << "+idx]>" << b.upperBound << "){\n";
	  }
	  if(std::abs(b.upperBound)>numeric_limits<long double>::min()){
	    if(mb.hasAttribute(h,v.name+"_normalisation_factor")){
	      const auto& nf = mb.getAttribute<string>(h,v.name+"_normalisation_factor");
	      out << "this->zeros[" << n << "+idx] = (" << b.upperBound << "- (this->" << v.name << "[idx]))/(" << nf <<");\n";
	    } else {
	      out << "this->zeros[" << n << "+idx] = " << b.upperBound << "- (this->" << v.name << "[idx]);\n";
	    }
	  } else {
	    if(mb.hasAttribute(h,v.name+"_normalisation_factor")){
	      const auto& nf = mb.getAttribute<string>(h,v.name+"_normalisation_factor");
	      out << "this->zeros[" << n << "+idx] = - (this->" << v.name << "[idx])/(" << nf << ");\n";
	    } else {
	      out << "this->zeros[" << n << "+idx] = - (this->" << v.name << "[idx]);\n";
	    }
	  }
	  out << "}\n";
	  out << "}\n";
	}
	if((mb.getTypeFlag(v.type)!=SupportedTypes::Scalar)&&(v.arraySize==1u)){
	  if(b.component==-1){
	    const auto n2 =  mb.getTypeSize(v.type,1u);
	    out << "for(unsigned short idx=0;idx!=" << n2 << ";++idx){" << endl;
	    if(mb.hasAttribute(h,v.name+"_normalisation_factor")){
	      const auto& nf = mb.getAttribute<string>(h,v.name+"_normalisation_factor");
	      out << "if(this->" << v.name << "[idx]+(" << nf << ")*(this->zeros[" << n << "+idx])>" << b.upperBound << "){\n";
	    } else {
	      out << "if(this->" << v.name << "[idx]+this->zeros[" << n << "+idx]>" << b.upperBound << "){\n";
	    }
	    if(std::abs(b.upperBound)>numeric_limits<long double>::min()){
	      if(mb.hasAttribute(h,v.name+"_normalisation_factor")){
		const auto& nf = mb.getAttribute<string>(h,v.name+"_normalisation_factor");
		out << "this->zeros[" << n << "+idx] = (" << b.upperBound << "- (this->" << v.name << "[idx]))/(" << nf << ");\n";
	      } else {
		out << "this->zeros[" << n << "+idx] = " << b.upperBound << "- (this->" << v.name << "[idx]);\n";
	      }
	    } else {
	      if(mb.hasAttribute(h,v.name+"_normalisation_factor")){
		const auto& nf = mb.getAttribute<string>(h,v.name+"_normalisation_factor");
		out << "this->zeros[" << n << "+idx] = - (this->" << v.name << "[idx])/(" << nf << ");\n";
	      } else {
		out << "this->zeros[" << n << "+idx] = - (this->" << v.name << "[idx]);\n";
	      }
	    }
	    out << "}\n";
	    out << "}\n";
	  } else {
	    if(mb.hasAttribute(h,v.name+"_normalisation_factor")){
	      const auto& nf = mb.getAttribute<string>(h,v.name+"_normalisation_factor");
	      out << "if(this->" << v.name << "[" << b.component << "]+(" << nf << ")*(this->zeros[" << n << "+" << b.component << "])>" << b.upperBound << "){\n";
	    } else {
	      out << "if(this->" << v.name << "[" << b.component << "]+this->zeros[" << n << "+" << b.component << "]>" << b.upperBound << "){\n";
	    }
	    if(std::abs(b.upperBound)>numeric_limits<long double>::min()){
	      if(mb.hasAttribute(h,v.name+"_normalisation_factor")){
		const auto& nf = mb.getAttribute<string>(h,v.name+"_normalisation_factor");
		out << "this->zeros[" << n << "+" << b.component << "] = (" << b.upperBound << "- (this->" << v.name << "[" << b.component << "]))/(" << nf << ");\n";
	      } else {
		out << "this->zeros[" << n << "+" << b.component << "] = " << b.upperBound << "- (this->" << v.name << "[" << b.component << "]);\n";
	      }
	    } else {
	      if(mb.hasAttribute(h,v.name+"_normalisation_factor")){
		const auto& nf = mb.getAttribute<string>(h,v.name+"_normalisation_factor");
		out << "this->zeros[" << n << "+" << b.component << "] = - (this->" << v.name << "[" << b.component << "])/(" << nf << ");\n";
	      } else {
		out << "this->zeros[" << n << "+" << b.component << "] = - (this->" << v.name << "[" << b.component << "]);\n";
	      }
	    }
	    out << "}\n";
	  }
	}
	if((mb.getTypeFlag(v.type)!=SupportedTypes::Scalar)&&(v.arraySize!=1u)){
	  const auto n2 =  mb.getTypeSize(v.type,1u);
	  out << "for(unsigned short idx=0;idx!=" << v.arraySize << ";++idx){" << endl;
	  if(b.component==-1){
	    out << "for(unsigned short idx2=0;idx2!=" << n2 << ";++idx2){" << endl;
	    if(mb.hasAttribute(h,v.name+"_normalisation_factor")){
	      const auto& nf = mb.getAttribute<string>(h,v.name+"_normalisation_factor");
	      out << "if(this->" << v.name << "[idx][idx2]+(" << nf << ")*(this->zeros[" << n << "+" << n2 << "*idx+idx2])>" << b.upperBound << "){\n";
	    } else {
	      out << "if(this->" << v.name << "[idx][idx2]+this->zeros[" << n << "+" << n2 << "*idx+idx2]>" << b.upperBound << "){\n";
	    }
	    if(std::abs(b.upperBound)>numeric_limits<long double>::min()){
	      if(mb.hasAttribute(h,v.name+"_normalisation_factor")){
		const auto& nf = mb.getAttribute<string>(h,v.name+"_normalisation_factor");
		out << "this->zeros[" << n << "+" << n2 << "*idx+idx2] = (" << b.upperBound << "- (this->" << v.name << "[idx][idx2]))/(" << nf << ");\n";
	      } else {
		out << "this->zeros[" << n << "+" << n2 << "*idx+idx2] = " << b.upperBound << "- (this->" << v.name << "[idx][idx2]);\n";
	      }
	    } else {
	      if(mb.hasAttribute(h,v.name+"_normalisation_factor")){
		const auto& nf = mb.getAttribute<string>(h,v.name+"_normalisation_factor");
		out << "this->zeros[" << n << "+" << n2 << "*idx+idx2] = - (this->" << v.name << "[idx][idx2])/(" << nf << ");\n";
	      } else {
		out << "this->zeros[" << n << "+" << n2 << "*idx+idx2] = - (this->" << v.name << "[idx][idx2]);\n";
	      }
	    }
	    out << "}\n";
	    out << "}\n";
	  } else {
	    if(mb.hasAttribute(h,v.name+"_normalisation_factor")){
	      const auto& nf = mb.getAttribute<string>(h,v.name+"_normalisation_factor");
	      out << "if(this->" << v.name << "[idx][" << b.component << "]+(" << nf << ")*(this->zeros[" << n << "+" << n2 << "*idx+" << b.component << "])>" << b.upperBound << "){\n";
	    } else {
	      out << "if(this->" << v.name << "[idx][" << b.component << "]+this->zeros[" << n << "+" << n2 << "*idx+" << b.component << "]>" << b.upperBound << "){\n";
	    }
	    if(std::abs(b.upperBound)>numeric_limits<long double>::min()){
	      if(mb.hasAttribute(h,v.name+"_normalisation_factor")){
		const auto& nf = mb.getAttribute<string>(h,v.name+"_normalisation_factor");
		out << "this->zeros[" << n << "+" << n2 << "*idx+" << b.component << "] = (" << b.upperBound << "- (this->" << v.name << "[idx][" << b.component << "]))/(" << nf << ");\n";
	      } else {
		out << "this->zeros[" << n << "+" << n2 << "*idx+" << b.component << "] = " << b.upperBound << "- (this->" << v.name << "[idx][" << b.component << "]);\n";
	      }
	    } else {
	      if(mb.hasAttribute(h,v.name+"_normalisation_factor")){
		const auto& nf = mb.getAttribute<string>(h,v.name+"_normalisation_factor");
		out << "this->zeros[" << n << "+" << n2 << "*idx+" << b.component << "] = - (this->" << v.name << "[idx][" << b.component << "])/(" << nf << ");\n";
	      } else {
		out << "this->zeros[" << n << "+" << n2 << "*idx+" << b.component << "] = - (this->" << v.name << "[idx][" << b.component << "]);\n";
	      }
	    }
	    out << "}\n";
	  }
	  out << "}\n";
	}
      }
      n += mb.getTypeSize(v.type,v.arraySize);
    }
  } // end of NonLinearSystemSolverBase::writeLimitsOnIncrementValuesBasedOnStateVariablesPhysicalBounds

  void
  NonLinearSystemSolverBase::writeLimitsOnIncrementValuesBasedOnIntegrationVariablesIncrementsPhysicalBounds(std::ostream&,
													     const BehaviourDescription&,
													     const Hypothesis)
  {} // end of NonLinearSystemSolverBase::writeLimitsOnIncrementValuesBasedOnIntegrationVariablesIncrementsPhysicalBounds

  NonLinearSystemSolverBase::~NonLinearSystemSolverBase() = default;

} // end of namespace mfront
