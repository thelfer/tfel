/*!
 * \file   Cst.hxx
 * \brief  This file declares the Cst class
 * \author Helfer Thomas
 * \date   04 Jan 2007
 */

#ifndef _LIB_TFEL_CST_H_
#define _LIB_TFEL_CST_H_ 

#include<string>
#include"TFEL/Utilities/ToString.hxx"
#include"TFEL/Math/qt.hxx"
#include"TFEL/Math/Function/FunctionConcept.hxx"

namespace tfel{

  namespace math{

    template<short N,unsigned short D=1u>
    struct Cst;

    template<short N,unsigned short D>
    struct FunctionTraits<Cst<N,D> >
    {
      typedef Cst<0> DerivateFunc;
    };

    template<short N,unsigned short D>
    struct Cst
      : public FunctionConcept<Cst<N,D> >
    {
      							                                  
      /*!						                                  
       * \brief  Return the name of the class.		                                  
       * \param  void.					                                  
       * \return std::string, the name of the class.	                                  
       * \see    Name.
       */						                                  
      static std::string getName(void)
      {
	using namespace std;
	using namespace tfel::utilities;
	string name("Cst<");
	name += ToString(N);
	name += string("/");
	name += ToString(D);
	name+=string(">");
	return name;
      }

      float 
      operator()(const float) const
      {
	return static_cast<float>(N)/D;
      }

      template<typename Unit>
      qt<Unit,float> 
      operator()(const qt<Unit,float>) const                    
      {
	return qt<Unit,float>(static_cast<float>(N)/D);
      }

      double 
      operator()(const double) const                               
      {
	return static_cast<double>(N)/D;
      }

      template<typename Unit>
      qt<Unit,double> 
      operator()(const qt<Unit,double>) const                  
      {
	return qt<Unit,double>(static_cast<double>(N)/D);
      }

      long double 
      operator()(const long double) const                    
      {
	return static_cast<long double>(N)/D;
      }

      template<typename Unit>
      qt<Unit,long double> 
      operator()(const qt<Unit,long double>) const
      {
	return qt<Unit,long double>(static_cast<long double>(N)/D);
      }
							                                  
      Complex<float>                                                                
      operator()(const Complex<float>) const                                  
      {
	return static_cast<float>(N)/D;
      }
     						                                          
      template<typename Unit>
      qt<Unit,Complex<float> >                                                    
      operator()(const qt<Unit,Complex<float> >) const
      {
	return qt<Unit,Complex<float> >(static_cast<float>(N)/D);
      }
                                                                                          
      Complex<double>                                                               
      operator()(const Complex<double>) const                                 
      {
	return static_cast<double>(N)/D;
      }
							                                  
      template<typename Unit>
      qt<Unit,Complex<double> >                                                   
      operator()(const qt<Unit,Complex<double> >) const
      {
	return qt<Unit,Complex<double> >(static_cast<double>(N)/D);
      }
							                                  
      Complex<long double>                                                          
      operator()(const Complex<long double>) const                            
      {
	return static_cast<long double>(N)/D;
      }
                                                                                          
      template<typename Unit>
      qt<Unit,Complex<long double> >                                              
      operator()(const qt<Unit,Complex<long double> >) const
      {
	return qt<Unit,Complex<long double> >(static_cast<long double>(N)/D);
      }
                                                                                          
      template<typename T>      			                                  
      typename tfel::meta::EnableIf<                                                      
        !tfel::typetraits::IsInvalid<typename ComputeUnaryResult<T,Cst<N,D> >::Result>::cond,
	typename ComputeUnaryResult<T,Cst<N,D> >::Handle                               
      >::type                                                                             
      operator()(const T& v) const
      {
	typedef typename ComputeUnaryResult<T,Cst<N,D> >::Handle Handle;                     
	return Handle(v,*this);                                                           
      }

    };

    template<short N>
    struct Cst<N,1u>
      : public FunctionConcept<Cst<N,1u> >
    {
      							                                  
      /*!						                                  
       * \brief  Return the name of the class.		                                  
       * \param  void.					                                  
       * \return std::string, the name of the class.	                                  
       * \see    Name.					                                  
       */						                                  
      static
      std::string
      getName(void)
      {
	using namespace std;
	using namespace tfel::utilities;
	string name("Cst<");
	name += ToString(N);
	name+=string(">");
	return name;
      }

      float 
      operator()(const float) const
      {
	return N;
      }

      template<typename Unit>
      qt<Unit,float> 
      operator()(const qt<Unit,float>) const                    
      {
	return qt<Unit,float>(N);
      }

      double 
      operator()(const double) const                               
      {
	return N;
      }

      template<typename Unit>
      qt<Unit,double> 
      operator()(const qt<Unit,double>) const                  
      {
	return qt<Unit,double>(N);
      }

      long double 
      operator()(const long double) const                    
      {
	return N;
      }

      template<typename Unit>
      qt<Unit,long double> 
      operator()(const qt<Unit,long double>) const
      {
	return qt<Unit,long double>(N);
      }
							                                  
      Complex<float>                                                                
      operator()(const Complex<float>) const                                  
      {
	return N;
      }
     						                                          
      template<typename Unit>
      qt<Unit,Complex<float> >                                                    
      operator()(const qt<Unit,Complex<float> >) const
      {
	return qt<Unit,Complex<float> >(N);
      }
                                                                                          
      Complex<double>                                                               
      operator()(const Complex<double>) const                                 
      {
	return N;
      }
							                                  
      template<typename Unit>
      qt<Unit,Complex<double> >                                                   
      operator()(const qt<Unit,Complex<double> >) const
      {
	return qt<Unit,Complex<double> >(N);
      }
							                                  
      Complex<long double>                                                          
      operator()(const Complex<long double>) const                            
      {
	return N;
      }
                                                                                          
      template<typename Unit>
      qt<Unit,Complex<long double> >                                              
      operator()(const qt<Unit,Complex<long double> >) const
      {
	return qt<Unit,Complex<long double> >(N);
      }
                                                                                          
      template<typename T>      			                                  
      typename tfel::meta::EnableIf<                                                      
        !tfel::typetraits::IsInvalid<typename ComputeUnaryResult<T,Cst<N,1u> >::Result>::cond,
	typename ComputeUnaryResult<T,Cst<N,1u> >::Handle                               
      >::type                                                                             
      operator()(const T& v) const
      {
	typedef typename ComputeUnaryResult<T,Cst<N,1u> >::Handle Handle;                     
	return Handle(v,*this);                                                           
      }
      
    };

    template<short N,unsigned short D>
    Cst<0> derivate(const Cst<N,D>);
    
    template<short N,unsigned short D>
    class UnaryResultType<float,Cst<N,D> >
    {
    public:
      typedef float type;
    };

    template<short N,unsigned short D>
    class UnaryResultType<double,Cst<N,D> >
    {
    public:
      typedef double type;
    };

    template<short N,unsigned short D>
    class UnaryResultType<long double,Cst<N,D> >
    {
    public:
      typedef long double type;
    };

    template<typename Unit,short N,unsigned short D>
    class UnaryResultType<qt<Unit,float>,Cst<N,D> >
    {
    public:
      typedef qt<Unit,float> type;
    };

    template<typename Unit,short N,unsigned short D>
    class UnaryResultType<qt<Unit,double>,Cst<N,D> >
    {
    public:
      typedef qt<Unit,double> type;
    };

    template<typename Unit,short N,unsigned short D>
    class UnaryResultType<qt<Unit,long double>,Cst<N,D> >
    {
    public:
      typedef qt<Unit,long double> type;
    };

    template<short N,unsigned short D>
    class UnaryResultType<Complex<float>,Cst<N,D> >
    {
    public:
      typedef Complex<float> type;
    };

    template<short N,unsigned short D>
    class UnaryResultType<Complex<double>,Cst<N,D> >
    {
    public:
      typedef Complex<double> type;
    };

    template<short N,unsigned short D>
    class UnaryResultType<Complex<long double>,Cst<N,D> >
    {
    public:
      typedef Complex<long double> type;
    };

    template<typename Unit,short N,unsigned short D>
    class UnaryResultType<qt<Unit,Complex<float> >,Cst<N,D> >
    {
    public:
      typedef qt<Unit,Complex<float> > type;
    };

    template<typename Unit,short N,unsigned short D>
    class UnaryResultType<qt<Unit,Complex<double> >,Cst<N,D> >
    {
    public:
      typedef qt<Unit,Complex<double> > type;
    };

    template<typename Unit,short N,unsigned short D>
    class UnaryResultType<qt<Unit,Complex<long double> >,Cst<N,D> >
    {
    public:
      typedef qt<Unit,Complex<long double> > type;
    };

  } // end of namespace math

} // end of namespace tfel

#include"TFEL/Math/Function/Cst.ixx"

#endif /* _LIB_TFEL_CST_H */

