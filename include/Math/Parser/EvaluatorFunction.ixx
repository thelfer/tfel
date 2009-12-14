/*!
 * \file   EvaluatorFunction.ixx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   15 jan 2009
 */

#ifndef _LIB_TFEL_MATH_EVALUATORFUNCTION_IXX_
#define _LIB_TFEL_MATH_EVALUATORFUNCTION_IXX_ 

namespace tfel
{

  namespace math
  {

    namespace parser
    {

      template<double (*f)(int,double)>
      tfel::utilities::SmartPtr<tfel::math::parser::Expr>
      EvaluatorFunction1P1VGenerator(const std::vector<std::string>& params,
				     std::vector<tfel::utilities::SmartPtr<tfel::math::parser::Expr> >& args)
      {
	using namespace std;
	using namespace tfel::utilities;
	using namespace tfel::math;
	using namespace tfel::math::parser;
	int nbr;
	Evaluator::checkParameterNumber(1,params.size());
	Evaluator::checkVariableNumber(1,args.size());
	nbr = Evaluator::convertToInt("EvaluatorFunction1P1VGenerator",params[0]);
	return SmartPtr<Expr>(new EvaluatorFunction1P1V(f,nbr,args[0]));
      } // end of EvaluatorFunction1P1VGenerator

      template<double (*f)(int,int,double)>
      tfel::utilities::SmartPtr<tfel::math::parser::Expr>
      EvaluatorFunction2P1VGenerator(const std::vector<std::string>& params,
				     std::vector<tfel::utilities::SmartPtr<tfel::math::parser::Expr> >& args)
      {
	using namespace std;
	using namespace tfel::utilities;
	using namespace tfel::math;
	using namespace tfel::math::parser;
	int nbr;
	int nbr2;
	Evaluator::checkParameterNumber(2,params.size());
	Evaluator::checkVariableNumber(1,args.size());
	nbr  = Evaluator::convertToInt("EvaluatorFunction2P1VGenerator",params[0]);
	nbr2 = Evaluator::convertToInt("EvaluatorFunction2P1VGenerator",params[1]);
	return SmartPtr<Expr>(new EvaluatorFunction2P1V(f,nbr,nbr2,args[0]));
      } // end of EvaluatorFunction2P1VGenerator

      template<double (*f)(int,double,double)>
      tfel::utilities::SmartPtr<tfel::math::parser::Expr>
      EvaluatorFunction1P2VGenerator(const std::vector<std::string>& params,
				     std::vector<tfel::utilities::SmartPtr<tfel::math::parser::Expr> >& args)
      {
	using namespace std;
	using namespace tfel::utilities;
	using namespace tfel::math;
	using namespace tfel::math::parser;
	int nbr;
	Evaluator::checkParameterNumber(1,params.size());
	Evaluator::checkVariableNumber(2,args.size());
	nbr = Evaluator::convertToInt("EvaluatorFunction1P2VGenerator",params[0]);
	return SmartPtr<Expr>(new EvaluatorFunction1P2V(f,nbr,args[0],args[1]));
      } // end of EvaluatorFunction1P2VGenerator

      template<double (*f)(int,int,double,double)>
      tfel::utilities::SmartPtr<tfel::math::parser::Expr>
      EvaluatorFunction2P2VGenerator(const std::vector<std::string>& params,
				     std::vector<tfel::utilities::SmartPtr<tfel::math::parser::Expr> >& args)
      {
	using namespace std;
	using namespace tfel::utilities;
	using namespace tfel::math;
	using namespace tfel::math::parser;
	int nbr;
	int nbr2;
	Evaluator::checkParameterNumber(2,params.size());
	Evaluator::checkVariableNumber(2,args.size());
	nbr  = Evaluator::convertToInt("EvaluatorFunction2P2VGenerator",params[0]);
	nbr2 = Evaluator::convertToInt("EvaluatorFunction2P2VGenerator",params[1]);
	return SmartPtr<Expr>(new EvaluatorFunction2P2V(f,nbr,nbr2,args[0],args[1]));
      } // end of EvaluatorFunction2P2VGenerator

      template<>
      struct EvaluatorFunctionWrapper<0u>
      {
	typedef double (* type)();
	typedef double (* type1P)(int);
	typedef double (* type2P)(int,int);
	typedef double (* type1UP)(unsigned int);
	typedef double (* type2UP)(unsigned int,
				   unsigned int);
	static double
	apply(const type,
	      const std::vector<tfel::utilities::SmartPtr<Expr> >&);
	static double
	apply(const type1P,int,
	      const std::vector<tfel::utilities::SmartPtr<Expr> >&);
	static double
	apply(const type2P,int,int,
	      const std::vector<tfel::utilities::SmartPtr<Expr> >&);
	static double
	apply(const type1UP,unsigned int,
	      const std::vector<tfel::utilities::SmartPtr<Expr> >&);
	static double
	apply(const type2UP,unsigned int,unsigned int,
	      const std::vector<tfel::utilities::SmartPtr<Expr> >&);
      }; // end of EvaluatorFunctionWrapper<1u>

      template<>
      struct EvaluatorFunctionWrapper<1u>
      {
	typedef double (* type)(double);
	typedef double (* type1P)(int,double);
	typedef double (* type2P)(int,int,double);
	typedef double (* type1UP)(unsigned int,
				   double);
	typedef double (* type2UP)(unsigned int,
				   unsigned int,
				   double);
	static double
	apply(const type,
	      const std::vector<tfel::utilities::SmartPtr<Expr> >&);
	static double
	apply(const type1P,int,
	      const std::vector<tfel::utilities::SmartPtr<Expr> >&);
	static double
	apply(const type2P,int,int,
	      const std::vector<tfel::utilities::SmartPtr<Expr> >&);
	static double
	apply(const type1UP,unsigned int,
	      const std::vector<tfel::utilities::SmartPtr<Expr> >&);
	static double
	apply(const type2UP,unsigned int,unsigned int,
	      const std::vector<tfel::utilities::SmartPtr<Expr> >&);
      }; // end of EvaluatorFunctionWrapper<1u>

      template<>
      struct EvaluatorFunctionWrapper<2u>
      {
	typedef double (* type)(double,double);
	typedef double (* type1P)(int,double,double);
	typedef double (* type2P)(int,int,double,double);
	typedef double (* type1UP)(unsigned int,
				   double,double);
	typedef double (* type2UP)(unsigned int,
				   unsigned int,
				   double,double);
	static double
	apply(const type,
	      const std::vector<tfel::utilities::SmartPtr<Expr> >&);
	static double
	apply(const type1P,int,
	      const std::vector<tfel::utilities::SmartPtr<Expr> >&);
	static double
	apply(const type2P,int,int,
	      const std::vector<tfel::utilities::SmartPtr<Expr> >&);
	static double
	apply(const type1UP,unsigned int,
	      const std::vector<tfel::utilities::SmartPtr<Expr> >&);
	static double
	apply(const type2UP,unsigned int,unsigned int,
	      const std::vector<tfel::utilities::SmartPtr<Expr> >&);
      }; // end of EvaluatorFunctionWrapper<2u>

      template<>
      struct EvaluatorFunctionWrapper<3u>
      {
	typedef double (* type)(double,double,double);
	typedef double (* type1P)(int,double,double,double);
	typedef double (* type2P)(int,int,double,double,double);
	typedef double (* type1UP)(unsigned int,
				   double,double,double);
	typedef double (* type2UP)(unsigned int,
				   unsigned int,
				   double,double,double);
	static double
	apply(const type,
	      const std::vector<tfel::utilities::SmartPtr<Expr> >&);
	static double
	apply(const type1P,int,
	      const std::vector<tfel::utilities::SmartPtr<Expr> >&);
	static double
	apply(const type2P,int,int,
	      const std::vector<tfel::utilities::SmartPtr<Expr> >&);
	static double
	apply(const type1UP,unsigned int,
	      const std::vector<tfel::utilities::SmartPtr<Expr> >&);
	static double
	apply(const type2UP,unsigned int,unsigned int,
	      const std::vector<tfel::utilities::SmartPtr<Expr> >&);
      }; // end of EvaluatorFunctionWrapper<3u>

      template<>
      struct EvaluatorFunctionWrapper<4u>
      {
	typedef double (* type)(double,double,
				double,double);
	typedef double (* type1P)(int,double,double,
				  double,double);
	typedef double (* type2P)(int,int,double,double,
				  double,double);
	typedef double (* type1UP)(unsigned int,
				   double,double,
				   double,double);
	typedef double (* type2UP)(unsigned int,
				   unsigned int,
				   double,double,
				   double,double);
	static double
	apply(const type,
	      const std::vector<tfel::utilities::SmartPtr<Expr> >&);
	static double
	apply(const type1P,int,
	      const std::vector<tfel::utilities::SmartPtr<Expr> >&);
	static double
	apply(const type2P,int,int,
	      const std::vector<tfel::utilities::SmartPtr<Expr> >&);
	static double
	apply(const type1UP,unsigned int,
	      const std::vector<tfel::utilities::SmartPtr<Expr> >&);
	static double
	apply(const type2UP,unsigned int,unsigned int,
	      const std::vector<tfel::utilities::SmartPtr<Expr> >&);
      }; // end of EvaluatorFunctionWrapper<4u>

      template<>
      struct EvaluatorFunctionWrapper<5u>
      {
	typedef double (* type)(double,double,double,
				double,double);
	typedef double (* type1P)(int,double,double,double,
				  double,double);
	typedef double (* type2P)(int,int,double,double,double,
				  double,double);
	typedef double (* type1UP)(unsigned int,
				   double,double,double,
				   double,double);
	typedef double (* type2UP)(unsigned int,
				   unsigned int,
				   double,double,double,
				   double,double);
	static double
	apply(const type,
	      const std::vector<tfel::utilities::SmartPtr<Expr> >&);
	static double
	apply(const type1P,int,
	      const std::vector<tfel::utilities::SmartPtr<Expr> >&);
	static double
	apply(const type2P,int,int,
	      const std::vector<tfel::utilities::SmartPtr<Expr> >&);
	static double
	apply(const type1UP,unsigned int,
	      const std::vector<tfel::utilities::SmartPtr<Expr> >&);
	static double
	apply(const type2UP,unsigned int,unsigned int,
	      const std::vector<tfel::utilities::SmartPtr<Expr> >&);
      }; // end of EvaluatorFunctionWrapper<5u>

      template<>
      struct EvaluatorFunctionWrapper<6u>
      {
	typedef double (* type)(double,double,double,
				double,double,double);
	typedef double (* type1P)(int,double,double,double,
				  double,double,double);
	typedef double (* type2P)(int,int,double,double,double,
				  double,double,double);
	typedef double (* type1UP)(unsigned int,
				   double,double,double,
				   double,double,double);
	typedef double (* type2UP)(unsigned int,
				   unsigned int,
				   double,double,double,
				   double,double,double);
	static double
	apply(const type,
	      const std::vector<tfel::utilities::SmartPtr<Expr> >&);
	static double
	apply(const type1P,int,
	      const std::vector<tfel::utilities::SmartPtr<Expr> >&);
	static double
	apply(const type2P,int,int,
	      const std::vector<tfel::utilities::SmartPtr<Expr> >&);
	static double
	apply(const type1UP,unsigned int,
	      const std::vector<tfel::utilities::SmartPtr<Expr> >&);
	static double
	apply(const type2UP,unsigned int,unsigned int,
	      const std::vector<tfel::utilities::SmartPtr<Expr> >&);
      }; // end of EvaluatorFunctionWrapper<6u>

      template<>
      struct EvaluatorFunctionWrapper<7u>
      {
	typedef double (* type)(double,double,double,
				double,double,double,
				double);
	typedef double (* type1P)(int,double,double,double,
				  double,double,double,
				  double);
	typedef double (* type2P)(int,int,double,double,double,
				  double,double,double,
				  double);
	typedef double (* type1UP)(unsigned int,
				   double,double,double,
				   double,double,double,
				   double);
	typedef double (* type2UP)(unsigned int,
				   unsigned int,
				   double,double,double,
				   double,double,double,
				   double);
	static double
	apply(const type,
	      const std::vector<tfel::utilities::SmartPtr<Expr> >&);
	static double
	apply(const type1P,int,
	      const std::vector<tfel::utilities::SmartPtr<Expr> >&);
	static double
	apply(const type2P,int,int,
	      const std::vector<tfel::utilities::SmartPtr<Expr> >&);
	static double
	apply(const type1UP,unsigned int,
	      const std::vector<tfel::utilities::SmartPtr<Expr> >&);
	static double
	apply(const type2UP,unsigned int,unsigned int,
	      const std::vector<tfel::utilities::SmartPtr<Expr> >&);
      }; // end of EvaluatorFunctionWrapper<7u>

      template<>
      struct EvaluatorFunctionWrapper<8u>
      {
	typedef double (* type)(double,double,double,
				double,double,double,
				double,double);
	typedef double (* type1P)(int,double,double,double,
				  double,double,double,
				  double,double);
	typedef double (* type2P)(int,int,double,double,double,
				  double,double,double,
				  double,double);
	typedef double (* type1UP)(unsigned int,
				   double,double,double,
				   double,double,double,
				   double,double);
	typedef double (* type2UP)(unsigned int,
				   unsigned int,
				   double,double,double,
				   double,double,double,
				   double,double);
	static double
	apply(const type,
	      const std::vector<tfel::utilities::SmartPtr<Expr> >&);
	static double
	apply(const type1P,int,
	      const std::vector<tfel::utilities::SmartPtr<Expr> >&);
	static double
	apply(const type2P,int,int,
	      const std::vector<tfel::utilities::SmartPtr<Expr> >&);
	static double
	apply(const type1UP,unsigned int,
	      const std::vector<tfel::utilities::SmartPtr<Expr> >&);
	static double
	apply(const type2UP,unsigned int,unsigned int,
	      const std::vector<tfel::utilities::SmartPtr<Expr> >&);
      }; // end of EvaluatorFunctionWrapper<8u>

      template<>
      struct EvaluatorFunctionWrapper<9u>
      {
	typedef double (* type)(double,double,double,
				double,double,double,
				double,double,double);
	typedef double (* type1P)(int,double,double,double,
				  double,double,double,
				  double,double,double);
	typedef double (* type2P)(int,int,double,double,double,
				  double,double,double,
				  double,double,double);
	typedef double (* type1UP)(unsigned int,double,
				   double,double,double,
				   double,double,double,
				   double,double);
	typedef double (* type2UP)(unsigned int,unsigned int,
				   double,double,double,
				   double,double,double,
				   double,double,double);
	static double
	apply(const type,
	      const std::vector<tfel::utilities::SmartPtr<Expr> >&);
	static double
	apply(const type1P,int,
	      const std::vector<tfel::utilities::SmartPtr<Expr> >&);
	static double
	apply(const type2P,int,int,
	      const std::vector<tfel::utilities::SmartPtr<Expr> >&);
	static double
	apply(const type1UP,unsigned int,
	      const std::vector<tfel::utilities::SmartPtr<Expr> >&);
	static double
	apply(const type2UP,unsigned int,unsigned int,
	      const std::vector<tfel::utilities::SmartPtr<Expr> >&);
      }; // end of EvaluatorFunctionWrapper<9u>

      template<>
      struct EvaluatorFunctionWrapper<10u>
      {
	typedef double (* type)(double,double,double,
				double,double,double,
				double,double,double,
				double);
	typedef double (* type1P)(int,double,double,double,
				  double,double,double,
				  double,double,double,
				  double);
	typedef double (* type2P)(int,int,double,double,double,
				  double,double,double,
				  double,double,double,
				  double);
	typedef double (* type1UP)(unsigned int,double,
				   double,double,double,
				   double,double,double,
				   double,double,double);
	typedef double (* type2UP)(unsigned int,unsigned int,
				   double,double,double,
				   double,double,double,
				   double,double,double,
				   double);
	static double
	apply(const type,
	      const std::vector<tfel::utilities::SmartPtr<Expr> >&);
	static double
	apply(const type1P,int,
	      const std::vector<tfel::utilities::SmartPtr<Expr> >&);
	static double
	apply(const type2P,int,int,
	      const std::vector<tfel::utilities::SmartPtr<Expr> >&);
	static double
	apply(const type1UP,unsigned int,
	      const std::vector<tfel::utilities::SmartPtr<Expr> >&);
	static double
	apply(const type2UP,unsigned int,unsigned int,
	      const std::vector<tfel::utilities::SmartPtr<Expr> >&);
      }; // end of EvaluatorFunctionWrapper<10u>

      template<unsigned short N>
      EvaluatorFunctionNV<N>::EvaluatorFunctionNV(typename EvaluatorFunctionWrapper<N>::type f_,
						  const std::vector<tfel::utilities::SmartPtr<Expr> >& args_)
	: f(f_),
	  args(args_)
      {} // end of EvaluatorFunctionNV<N>::EvaluatorFunctionNV

      template<unsigned short N>
      double
      EvaluatorFunctionNV<N>::getValue(void) const
      {
	return EvaluatorFunctionWrapper<N>::apply(f,args);
      } // end of EvaluatorFunctionNV<N>::getValue(void) const

      template<unsigned short N>
      tfel::utilities::SmartPtr<Expr>
      EvaluatorFunctionNV<N>::resolveDependencies(void) const
      {
	using namespace std;
	using namespace tfel::utilities;
	vector<SmartPtr<Expr> > nargs(this->args.size());
        vector<SmartPtr<Expr> >::const_iterator p;
        vector<SmartPtr<Expr> >::size_type i;
        for(p=this->args.begin(),i=0;p!=this->args.end();++p,++i){
	  nargs[i]=(*p)->resolveDependencies();
        }
        return SmartPtr<Expr>(new EvaluatorFunctionNV<N>(this->f,nargs));
      } // end of EvaluatorFunctionNV<N>::resolveDependencies const

      template<unsigned short N>
      tfel::utilities::SmartPtr<Expr>
      EvaluatorFunctionNV<N>::clone(const std::vector<double>& v) const
      {
	using namespace std;
	using namespace tfel::utilities;
	vector<SmartPtr<Expr> > nargs(this->args.size());
        vector<SmartPtr<Expr> >::const_iterator p;
        vector<SmartPtr<Expr> >::size_type i;
        for(p=this->args.begin(),i=0;p!=this->args.end();++p,++i){
	  nargs[i]=(*p)->clone(v);
        }
        return SmartPtr<Expr>(new EvaluatorFunctionNV<N>(this->f,nargs));
      } // end of EvaluatorFunctionNV<N>::resolveDependencies const

      template<unsigned short N>
      void
      EvaluatorFunctionNV<N>::checkCyclicDependency(const std::vector<std::string>& names) const
	throw(std::runtime_error)
      {
	using namespace std;
	using namespace tfel::utilities;
        vector<SmartPtr<Expr> >::const_iterator p;
        for(p=this->args.begin();p!=this->args.end();++p){
	  (*p)->checkCyclicDependency(names);
        }
      } // end of EvaluatorFunctionNV<N>::checkCyclicDependency

      template<unsigned short N>
      EvaluatorFunctionNV<N>::~EvaluatorFunctionNV()
      {} // end of EvaluatorFunctionNV<N>::~EvaluatorFunctionNV()

      template<unsigned short N>
      EvaluatorFunction1PNV<N>::EvaluatorFunction1PNV(typename EvaluatorFunctionWrapper<N>::type1P f_,
						      const int n_,
						      const std::vector<tfel::utilities::SmartPtr<Expr> >& args_)
	: f(f_),
	  n(n_),
	  args(args_)
      {} // end of EvaluatorFunction1PNV<N>::EvaluatorFunction1PNV

      template<unsigned short N>
      double
      EvaluatorFunction1PNV<N>::getValue(void) const
      {
	return EvaluatorFunctionWrapper<N>::apply(this->f,this->n,this->args);
      } // end of EvaluatorFunction1PNV<N>::getValue(void) const

      template<unsigned short N>
      tfel::utilities::SmartPtr<Expr>
      EvaluatorFunction1PNV<N>::resolveDependencies(void) const
      {
	using namespace std;
	using namespace tfel::utilities;
	vector<SmartPtr<Expr> > nargs(this->args.size());
        vector<SmartPtr<Expr> >::const_iterator p;
        vector<SmartPtr<Expr> >::size_type i;
        for(p=this->args.begin(),i=0;p!=this->args.end();++p,++i){
	  nargs[i]=(*p)->resolveDependencies();
        }
        return SmartPtr<Expr>(new EvaluatorFunction1PNV<N>(this->f,this->n,nargs));
      } // end of EvaluatorFunction1PNV<N>::resolveDependencies const

      template<unsigned short N>
      tfel::utilities::SmartPtr<Expr>
      EvaluatorFunction1PNV<N>::clone(const std::vector<double>& v) const
      {
	using namespace std;
	using namespace tfel::utilities;
	vector<SmartPtr<Expr> > nargs(this->args.size());
        vector<SmartPtr<Expr> >::const_iterator p;
        vector<SmartPtr<Expr> >::size_type i;
        for(p=this->args.begin(),i=0;p!=this->args.end();++p,++i){
	  nargs[i]=(*p)->clone(v);
        }
        return SmartPtr<Expr>(new EvaluatorFunction1PNV<N>(this->f,this->n,nargs));
      } // end of EvaluatorFunction1PNV<N>::resolveDependencies const

      template<unsigned short N>
      void
      EvaluatorFunction1PNV<N>::checkCyclicDependency(const std::vector<std::string>& names) const
	throw(std::runtime_error)
      {
	using namespace std;
	using namespace tfel::utilities;
        vector<SmartPtr<Expr> >::const_iterator p;
        for(p=this->args.begin();p!=this->args.end();++p){
	  (*p)->checkCyclicDependency(names);
        }
      } // end of EvaluatorFunction1PNV<N>::checkCyclicDependency

      template<unsigned short N>
      EvaluatorFunction1PNV<N>::~EvaluatorFunction1PNV()
      {} // end of EvaluatorFunction1PNV<N>::~EvaluatorFunction1PNV()

      template<unsigned short N>
      EvaluatorFunction2PNV<N>::EvaluatorFunction2PNV(typename EvaluatorFunctionWrapper<N>::type2P f_,
						      const int n_,
						      const int m_,
						      const std::vector<tfel::utilities::SmartPtr<Expr> >& args_)
	: f(f_),
	  n(n_),
	  m(m_),
	  args(args_)
      {} // end of EvaluatorFunction2PNV<N>::EvaluatorFunction2PNV

      template<unsigned short N>
      double
      EvaluatorFunction2PNV<N>::getValue(void) const
      {
	return EvaluatorFunctionWrapper<N>::apply(this->f,this->n,this->m,this->args);
      } // end of EvaluatorFunction2PNV<N>::getValue(void) const

      template<unsigned short N>
      tfel::utilities::SmartPtr<Expr>
      EvaluatorFunction2PNV<N>::resolveDependencies(void) const
      {
	using namespace std;
	using namespace tfel::utilities;
	vector<SmartPtr<Expr> > nargs(this->args.size());
        vector<SmartPtr<Expr> >::const_iterator p;
        vector<SmartPtr<Expr> >::size_type i;
        for(p=this->args.begin(),i=0;p!=this->args.end();++p,++i){
	  nargs[i]=(*p)->resolveDependencies();
        }
        return SmartPtr<Expr>(new EvaluatorFunction2PNV<N>(this->f,this->n,this->m,nargs));
      } // end of EvaluatorFunction2PNV<N>::resolveDependencies const

      template<unsigned short N>
      tfel::utilities::SmartPtr<Expr>
      EvaluatorFunction2PNV<N>::clone(const std::vector<double>& v) const
      {
	using namespace std;
	using namespace tfel::utilities;
	vector<SmartPtr<Expr> > nargs(this->args.size());
        vector<SmartPtr<Expr> >::const_iterator p;
        vector<SmartPtr<Expr> >::size_type i;
        for(p=this->args.begin(),i=0;p!=this->args.end();++p,++i){
	  nargs[i]=(*p)->clone(v);
        }
        return SmartPtr<Expr>(new EvaluatorFunction2PNV<N>(this->f,this->n,this->m,nargs));
      } // end of EvaluatorFunction2PNV<N>::resolveDependencies const

      template<unsigned short N>
      void
      EvaluatorFunction2PNV<N>::checkCyclicDependency(const std::vector<std::string>& names) const
	throw(std::runtime_error)
      {
	using namespace std;
	using namespace tfel::utilities;
        vector<SmartPtr<Expr> >::const_iterator p;
        for(p=this->args.begin();p!=this->args.end();++p){
	  (*p)->checkCyclicDependency(names);
        }
      } // end of EvaluatorFunction2PNV<N>::checkCyclicDependency

      template<unsigned short N>
      EvaluatorFunction2PNV<N>::~EvaluatorFunction2PNV()
      {} // end of EvaluatorFunction2PNV<N>::~EvaluatorFunctionNV()

      template<unsigned short N>
      EvaluatorFunction1UPNV<N>::EvaluatorFunction1UPNV(typename EvaluatorFunctionWrapper<N>::type1UP f_,
							const unsigned int n_,
							const std::vector<tfel::utilities::SmartPtr<Expr> >& args_)
	: f(f_),
	  n(n_),
	  args(args_)
      {} // end of EvaluatorFunction1UPNV<N>::EvaluatorFunction1UPNV

      template<unsigned short N>
      double
      EvaluatorFunction1UPNV<N>::getValue(void) const
      {
	return EvaluatorFunctionWrapper<N>::apply(this->f,this->n,this->args);
      } // end of EvaluatorFunction1UPNV<N>::getValue(void) const

      template<unsigned short N>
      tfel::utilities::SmartPtr<Expr>
      EvaluatorFunction1UPNV<N>::resolveDependencies(void) const
      {
	using namespace std;
	using namespace tfel::utilities;
	vector<SmartPtr<Expr> > nargs(this->args.size());
        vector<SmartPtr<Expr> >::const_iterator p;
        vector<SmartPtr<Expr> >::size_type i;
        for(p=this->args.begin(),i=0;p!=this->args.end();++p,++i){
	  nargs[i]=(*p)->resolveDependencies();
        }
        return SmartPtr<Expr>(new EvaluatorFunction1UPNV<N>(this->f,this->n,nargs));
      } // end of EvaluatorFunction1UPNV<N>::resolveDependencies const

      template<unsigned short N>
      tfel::utilities::SmartPtr<Expr>
      EvaluatorFunction1UPNV<N>::clone(const std::vector<double>& v) const
      {
	using namespace std;
	using namespace tfel::utilities;
	vector<SmartPtr<Expr> > nargs(this->args.size());
        vector<SmartPtr<Expr> >::const_iterator p;
        vector<SmartPtr<Expr> >::size_type i;
        for(p=this->args.begin(),i=0;p!=this->args.end();++p,++i){
	  nargs[i]=(*p)->clone(v);
        }
        return SmartPtr<Expr>(new EvaluatorFunction1UPNV<N>(this->f,this->n,nargs));
      } // end of EvaluatorFunction1UPNV<N>::resolveDependencies const

      template<unsigned short N>
      void
      EvaluatorFunction1UPNV<N>::checkCyclicDependency(const std::vector<std::string>& names) const
	throw(std::runtime_error)
      {
	using namespace std;
	using namespace tfel::utilities;
        vector<SmartPtr<Expr> >::const_iterator p;
        for(p=this->args.begin();p!=this->args.end();++p){
	  (*p)->checkCyclicDependency(names);
        }
      } // end of EvaluatorFunction1UPNV<N>::checkCyclicDependency

      template<unsigned short N>
      EvaluatorFunction1UPNV<N>::~EvaluatorFunction1UPNV()
      {} // end of EvaluatorFunction1UPNV<N>::~EvaluatorFunction1UPNV()

      template<unsigned short N>
      EvaluatorFunction2UPNV<N>::EvaluatorFunction2UPNV(typename EvaluatorFunctionWrapper<N>::type2UP f_,
							const unsigned int n_,
							const unsigned int m_,
							const std::vector<tfel::utilities::SmartPtr<Expr> >& args_)
	: f(f_),
	  n(n_),
	  m(m_),
	  args(args_)
      {} // end of EvaluatorFunction2UPNV<N>::EvaluatorFunction2UPNV

      template<unsigned short N>
      double
      EvaluatorFunction2UPNV<N>::getValue(void) const
      {
	return EvaluatorFunctionWrapper<N>::apply(this->f,this->n,this->m,this->args);
      } // end of EvaluatorFunction2UPNV<N>::getValue(void) const

      template<unsigned short N>
      tfel::utilities::SmartPtr<Expr>
      EvaluatorFunction2UPNV<N>::resolveDependencies(void) const
      {
	using namespace std;
	using namespace tfel::utilities;
	vector<SmartPtr<Expr> > nargs(this->args.size());
        vector<SmartPtr<Expr> >::const_iterator p;
        vector<SmartPtr<Expr> >::size_type i;
        for(p=this->args.begin(),i=0;p!=this->args.end();++p,++i){
	  nargs[i]=(*p)->resolveDependencies();
        }
        return SmartPtr<Expr>(new EvaluatorFunction2UPNV<N>(this->f,this->n,this->m,nargs));
      } // end of EvaluatorFunction2UPNV<N>::resolveDependencies const

      template<unsigned short N>
      tfel::utilities::SmartPtr<Expr>
      EvaluatorFunction2UPNV<N>::clone(const std::vector<double>& v) const
      {
	using namespace std;
	using namespace tfel::utilities;
	vector<SmartPtr<Expr> > nargs(this->args.size());
        vector<SmartPtr<Expr> >::const_iterator p;
        vector<SmartPtr<Expr> >::size_type i;
        for(p=this->args.begin(),i=0;p!=this->args.end();++p,++i){
	  nargs[i]=(*p)->clone(v);
        }
        return SmartPtr<Expr>(new EvaluatorFunction2UPNV<N>(this->f,this->n,this->m,nargs));
      } // end of EvaluatorFunction2UPNV<N>::resolveDependencies const

      template<unsigned short N>
      void
      EvaluatorFunction2UPNV<N>::checkCyclicDependency(const std::vector<std::string>& names) const
	throw(std::runtime_error)
      {
	using namespace std;
	using namespace tfel::utilities;
        vector<SmartPtr<Expr> >::const_iterator p;
        for(p=this->args.begin();p!=this->args.end();++p){
	  (*p)->checkCyclicDependency(names);
        }
      } // end of EvaluatorFunction2UPNV<N>::checkCyclicDependency

      template<unsigned short N>
      EvaluatorFunction2UPNV<N>::~EvaluatorFunction2UPNV()
      {} // end of EvaluatorFunction2UPNV<N>::~EvaluatorFunctionNV()

      template<unsigned short N,
	       typename EvaluatorFunctionWrapper<N>::type f>
      tfel::utilities::SmartPtr<tfel::math::parser::Expr>
      EvaluatorFunctionNVGenerator(const std::vector<std::string>& params,
				   std::vector<tfel::utilities::SmartPtr<tfel::math::parser::Expr> >& args)
      {
	using namespace std;
	using namespace tfel::utilities;
	using namespace tfel::math;
	using namespace tfel::math::parser;
	Evaluator::checkParameterNumber(0,params.size());
	Evaluator::checkVariableNumber(N,args.size());
	return SmartPtr<Expr>(new EvaluatorFunctionNV<N>(f,args));
      } // end of EvaluatorFunctionNVGenerator

      template<unsigned short N,
	       typename EvaluatorFunctionWrapper<N>::type1P f>
      tfel::utilities::SmartPtr<tfel::math::parser::Expr>
      EvaluatorFunction1PNVGenerator(const std::vector<std::string>& params,
				     std::vector<tfel::utilities::SmartPtr<tfel::math::parser::Expr> >& args)
      {
	using namespace std;
	using namespace tfel::utilities;
	using namespace tfel::math;
	using namespace tfel::math::parser;
	int nbr;
	Evaluator::checkParameterNumber(1,params.size());
	Evaluator::checkVariableNumber(N,args.size());
	nbr  = Evaluator::convertToInt("EvaluatorFunction1PNVGenerator",params[0]);
	return SmartPtr<Expr>(new EvaluatorFunction1PNV<N>(f,nbr,args));
      } // end of EvaluatorFunction1PNVGenerator

      template<unsigned short N,
	       typename EvaluatorFunctionWrapper<N>::type2P f>
      tfel::utilities::SmartPtr<tfel::math::parser::Expr>
      EvaluatorFunction2PNVGenerator(const std::vector<std::string>& params,
				     std::vector<tfel::utilities::SmartPtr<tfel::math::parser::Expr> >& args)
      {
	using namespace std;
	using namespace tfel::utilities;
	using namespace tfel::math;
	using namespace tfel::math::parser;
	int nbr;
	int nbr2;
	Evaluator::checkParameterNumber(2,params.size());
	Evaluator::checkVariableNumber(N,args.size());
	nbr  = Evaluator::convertToInt("EvaluatorFunction2PNVGenerator",params[0]);
	nbr2 = Evaluator::convertToInt("EvaluatorFunction2PNVGenerator",params[1]);
	return SmartPtr<Expr>(new EvaluatorFunction2PNV<N>(f,nbr,nbr2,args));
      } // end of EvaluatorFunction1PNVGenerator

      template<unsigned short N,
	       typename EvaluatorFunctionWrapper<N>::type1UP f>
      tfel::utilities::SmartPtr<tfel::math::parser::Expr>
      EvaluatorFunction1UPNVGenerator(const std::vector<std::string>& params,
				      std::vector<tfel::utilities::SmartPtr<tfel::math::parser::Expr> >& args)
      {
	using namespace std;
	using namespace tfel::utilities;
	using namespace tfel::math;
	using namespace tfel::math::parser;
	unsigned int nbr;
	Evaluator::checkParameterNumber(1,params.size());
	Evaluator::checkVariableNumber(N,args.size());
	nbr  = Evaluator::convertToUnsignedInt("EvaluatorFunction1UPNVGenerator",params[0]);
	return SmartPtr<Expr>(new EvaluatorFunction1UPNV<N>(f,nbr,args));
      } // end of EvaluatorFunction1UPNVGenerator

      template<unsigned short N,
	       typename EvaluatorFunctionWrapper<N>::type2UP f>
      tfel::utilities::SmartPtr<tfel::math::parser::Expr>
      EvaluatorFunction2UPNVGenerator(const std::vector<std::string>& params,
				      std::vector<tfel::utilities::SmartPtr<tfel::math::parser::Expr> >& args)
      {
	using namespace std;
	using namespace tfel::utilities;
	using namespace tfel::math;
	using namespace tfel::math::parser;
	unsigned int nbr;
	unsigned int nbr2;
	Evaluator::checkParameterNumber(2,params.size());
	Evaluator::checkVariableNumber(N,args.size());
	nbr  = Evaluator::convertToUnsignedInt("EvaluatorFunction2UPNVGenerator",params[0]);
	nbr2 = Evaluator::convertToUnsignedInt("EvaluatorFunction2UPNVGenerator",params[1]);
	return SmartPtr<Expr>(new EvaluatorFunction2UPNV<N>(f,nbr,nbr2,args));
      } // end of EvaluatorFunction1UPNVGenerator

    } // end of namespace parser

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_MATH_EVALUATORPROXY_IXX_ */
