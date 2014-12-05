/*!
 * \file   include/TFEL/Utilities/Name.ixx
 * 
 * \brief  Specialise Name for the most used standard types.  
 * \author Helfer Thomas
 * \date   17 Jul 2006
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_TFEL_NAME_I_
#define _LIB_TFEL_NAME_I_ 

#include <string>

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/Utilities/ToString.hxx"

#include"TFEL/Metaprogramming/IntToType.hxx"
#include"TFEL/Metaprogramming/InvalidType.hxx"
#include"TFEL/Metaprogramming/Forward/TypeList.hxx"

#include"TFEL/Math/Forward/qt.hxx"
#include"TFEL/Math/Forward/vector.hxx"
#include"TFEL/Math/Forward/lambda.hxx"
#include"TFEL/Math/Forward/stensor.hxx"
#include"TFEL/Math/Forward/tvector.hxx"
#include"TFEL/Math/Forward/tmatrix.hxx"
#include"TFEL/Math/Forward/Complex.hxx"
#include"TFEL/Math/Forward/st2tost2.hxx"
#include"TFEL/Math/Forward/Function.hxx"
#include"TFEL/Math/Forward/VectorConcept.hxx"
#include"TFEL/Math/Forward/MatrixConcept.hxx"
#include"TFEL/Math/Forward/StensorConcept.hxx"
#include"TFEL/Math/Forward/ST2toST2Concept.hxx"

#include"TFEL/Math/Forward/TinyNewtonRaphson.hxx"
#include"TFEL/Math/General/Forward/General.hxx"
#include"TFEL/Math/Forward/ExpressionTemplates.hxx"

/*!
 * \def    TFEL_UTILITIES_NAME
 * \brief  An helper macro which help specialisation of Name for standard types
 * \author Helfer Thomas
 * \date   17 Jul 2006
 */
#define TFEL_UTILITIES_NAME(X)                               \
    /*!                                                      \
     * Partial specialisation for X                          \
     */                                                      \
    template<>                                               \
    struct TFELUTILITIES_VISIBILITY_EXPORT Name< X >                  \
    {                                                        \
      /*!                                                    \
       * \brief returns the Name of X.                       \
       * \param  void                                        \
       * \return std::string, the name of X.                 \
       */                                                    \
      static                                                 \
      std::string				             \
      getName(void);                                         \
    };                                                       \
    /*!                                                      \
     * Partial specialisation for X*                         \
     */                                                      \
    template<>                                               \
    struct TFELUTILITIES_VISIBILITY_EXPORT Name<X*>                   \
    {                                                        \
      /*!                                                    \
       * \brief returns the Name of X.                       \
       * \param  void                                        \
       * \return std::string, the name of X.                 \
       */                                                    \
      static                                                 \
      std::string                                            \
      getName(void);                                         \
    };                                                       \
    /*!                                                      \
     * Partial specialisation for const X * const            \
     */                                                      \
    template<>                                               \
    struct TFELUTILITIES_VISIBILITY_EXPORT Name<const X * const>      \
    {                                                        \
      /*!                                                    \
       * \brief returns the Name of X.                       \
       * \param  void                                        \
       * \return std::string, the name of X.                 \
       */                                                    \
      static                                                 \
      std::string		   	  	             \
      getName(void);                                         \
    };                                                       \
    /*!                                                      \
     * Partial specialisation for const X*                   \
     */                                                      \
    template<>                                               \
    struct TFELUTILITIES_VISIBILITY_EXPORT Name<const X *>            \
    {                                                        \
      /*!                                                    \
       * \brief returns the Name of X.                       \
       * \param  void                                        \
       * \return std::string, the name of X.                 \
       */                                                    \
      static                                                 \
      std::string                                            \
      getName(void);                                         \
    };                                                       \
    /*!                                                      \
     * Partial specialisation for  X&                        \
     */                                                      \
    template<>                                               \
    struct TFELUTILITIES_VISIBILITY_EXPORT Name<X &>                  \
    {                                                        \
      /*!                                                    \
       * \brief returns the Name of X.                       \
       * \param  void                                        \
       * \return std::string, the name of X.                 \
       */                                                    \
      static                                                 \
      std::string                                            \
      getName(void);                                         \
    };                                                       \
    /*!                                                      \
     * Partial specialisation for const X&                   \
     */                                                      \
    template<>                                               \
    struct TFELUTILITIES_VISIBILITY_EXPORT Name<const X &>            \
    {                                                        \
      /*!                                                    \
       * \brief returns the Name of X.                       \
       * \param  void                                        \
       * \return std::string, the name of X.                 \
       */                                                    \
      static                                                 \
      std::string        				     \
      getName(void);                                         \
    }

#define TFEL_UTILITIES_NAME_VOID                                \
    /*!                                                         \
     * Partial specialisation for void                          \
     */                                                         \
    template<>                                                  \
    struct TFELUTILITIES_VISIBILITY_EXPORT Name<void>                    \
    {                                                           \
      /*!                                                       \
       * \brief returns the Name of X.                          \
       * \param  void                                           \
       * \return std::string, the name of X.                    \
       */                                                       \
      static                                                    \
      std::string                                               \
      getName(void);                                            \
    };                                                          \
    /*!                                                         \
     * Partial specialisation for void*                         \
     */                                                         \
    template<>                                                  \
    struct TFELUTILITIES_VISIBILITY_EXPORT Name<void*>                   \
    {                                                           \
      /*!                                                       \
       * \brief returns the Name of X.                          \
       * \param  void                                           \
       * \return std::string, the name of X.                    \
       */                                                       \
      static                                                    \
      std::string                                               \
      getName(void);                                            \
    };                                                          \
    /*!                                                         \
     * Partial specialisation for const void* const             \
     */                                                         \
    template<>                                                  \
    struct TFELUTILITIES_VISIBILITY_EXPORT Name<const void* const>       \
    {                                                           \
      /*!                                                       \
       * \brief returns the Name of X.                          \
       * \param  void                                           \
       * \return std::string, the name of X.                    \
       */                                                       \
      static                                                    \
      std::string                                               \
      getName(void);                                            \
    };                                                          \
    /*!                                                         \
     * Partial specialisation for const void*                   \
     */                                                         \
    template<>                                                  \
    struct TFELUTILITIES_VISIBILITY_EXPORT Name<const void *>            \
    {                                                           \
      /*!                                                       \
       * \brief returns the Name of X.                          \
       * \param  void                                           \
       * \return std::string, the name of X.                    \
       */                                                       \
      static                                                    \
      std::string                                               \
      getName(void);                                            \
    }                                                       

namespace tfel
{
  
  namespace utilities
  {

    /*!
     * Partial specialisation for some pointer functions.
     */
    template<typename T>
    struct TFEL_VISIBILITY_LOCAL Name<T (*)(T)>
    {
      /*!
       * \brief returns the Name of the pointer functions.
       * \param  void
       * \return std::string.
       */
      static
      std::string 
      getName(void){
	return Name<T>::getName()+std::string("(*)(")+Name<T>::getName()+std::string(")");
      }
    };

    /*!
     * Partial specialisation for some pointer functions.
     */
    template<typename T1,typename T2>
    struct TFEL_VISIBILITY_LOCAL Name<T1 (*)(T2)>
    {
      /*!
       * \brief returns the Name of the pointer function.
       * \param  void
       * \return std::string.
       */
      static
      std::string 
      getName(void){
	return Name<T1>::getName()+std::string("(*)(")+Name<T2>::getName()+std::string(")");
      }
    };

    /*!
     * Partial specialisation for some pointer functions.
     */
    template<typename T1,typename T2,typename T3>
    struct TFEL_VISIBILITY_LOCAL Name<T1 (*)(T2,T3)>
    {
      /*!
       * \brief returns the Name of the pointer function.
       * \param  void
       * \return std::string.
       */
      static std::string
      getName(void){
	return Name<T1>::getName()+std::string("(*)(")+Name<T2>::getName()+std::string(",")+Name<T3>::getName()+std::string(")");
      }
    };

    /*!
     * Partial specialisation for some pointer functions.
     */
    template<typename T1,typename T2,typename T3,typename T4>
    struct TFEL_VISIBILITY_LOCAL Name<T1 (*)(T2,T3,T4)>
    {
      /*!
       * \brief returns the Name of the pointer function.
       * \param  void
       * \return std::string.
       */
      static std::string
      getName(void){
	return Name<T1>::getName()+std::string("(*)(")+Name<T2>::getName()+std::string(",")+Name<T3>::getName()+std::string(",")+Name<T4>::getName()+std::string(")");
      }
    };

    /*!
     * Partial specialisation for const variable.
     */
    template<typename T>
    struct TFEL_VISIBILITY_LOCAL Name<const T>
    {
      /*!
       * \brief returns the Name of the pointer function.
       * \param  void
       * \return std::string.
       */
      static std::string
      getName(void){
	return std::string("const ")+Name<T>::getName();
      }
    };

    /*!
     * Partial specialisation for pointers.
     */
    template<typename T>
    struct TFEL_VISIBILITY_LOCAL Name<T*>
    {
      /*!
       * \brief returns the Name of the pointer.
       * \param  void
       * \return std::string.
       */
      static
      std::string getName(void){
	return std::string("pointer to type ")+Name<T>::getName();
      }
    };

    /*!
     * Partial specialisation for pointers to const object.
     */
    template<typename T>
    struct TFEL_VISIBILITY_LOCAL Name<const T*>
    {
      /*!
       * \brief returns the Name of the pointer to const.
       * \param  void
       * \return std::string.
       */
      static std::string
      getName(void){
	return std::string("pointer to const type ")+Name<T>::getName();
      }
    };

    /*!
     * Partial specialisation for const pointers to object.
     */
    template<typename T>
    struct TFEL_VISIBILITY_LOCAL Name<T*const>
    {
      /*!
       * \brief returns the Name of the const pointer.
       * \param  void
       * \return std::string.
       */
      static std::string
      getName(void){
	return std::string("const pointer to type ")+Name<T>::getName();
      }
    };

    /*!
     * Partial specialisation for const pointers to const object.
     */
    template<typename T>
    struct TFEL_VISIBILITY_LOCAL Name<const T* const>
    {
      /*!
       * \brief returns the Name of the const pointer to const.
       * \param  void
       * \return std::string.
       */
      static std::string
      getName(void){
	return std::string("const pointer to const type ")+Name<T>::getName();
      }
    };

    /*!
     * Partial specialisation for reference to object.
     */
    template<typename T>
    struct TFEL_VISIBILITY_LOCAL Name<T&>
    {
      /*!
       * \brief returns the Name of the reference.
       * \param  void
       * \return std::string.
       */
      static std::string
      getName(void){
	return std::string("reference to type ")+Name<T>::getName();
      }
    };

    /*!
     * Partial specialisation for const reference to object.
     */
    template<typename T>
    struct TFEL_VISIBILITY_LOCAL Name<const T&>
    {
      /*!
       * \brief returns the Name of the const reference.
       * \param  void
       * \return std::string.
       */
      static std::string
      getName(void){
	return std::string("const reference to type ")+Name<T>::getName();
      }
    };

    TFEL_UTILITIES_NAME_VOID;
    TFEL_UTILITIES_NAME(bool);
    TFEL_UTILITIES_NAME(char);
    TFEL_UTILITIES_NAME(unsigned short);
    TFEL_UTILITIES_NAME(unsigned int);
    TFEL_UTILITIES_NAME(long unsigned int);
    TFEL_UTILITIES_NAME(short);
    TFEL_UTILITIES_NAME(int);
    TFEL_UTILITIES_NAME(long int);
    TFEL_UTILITIES_NAME(float);
    TFEL_UTILITIES_NAME(double);
    TFEL_UTILITIES_NAME(long double);
    TFEL_UTILITIES_NAME(tfel::math::Complex<unsigned short>);
    TFEL_UTILITIES_NAME(tfel::math::Complex<unsigned int>);
    TFEL_UTILITIES_NAME(tfel::math::Complex<long unsigned int>);
    TFEL_UTILITIES_NAME(tfel::math::Complex<short>);
    TFEL_UTILITIES_NAME(tfel::math::Complex<int>);
    TFEL_UTILITIES_NAME(tfel::math::Complex<long int>);
    TFEL_UTILITIES_NAME(tfel::math::Complex<float>);
    TFEL_UTILITIES_NAME(tfel::math::Complex<double>);
    TFEL_UTILITIES_NAME(tfel::math::Complex<long double>);
    TFEL_UTILITIES_NAME(std::string);

    template<typename T>
    struct TFEL_VISIBILITY_LOCAL Name<tfel::math::VectorConcept<T> >
    {
      
      static std::string getName(void)
      {
	return std::string("VectorConcept<")
	  +tfel::utilities::Name<T>::getName()
	  +std::string(">");
      } // end of VectorConcept<T>::getName()

    };

    template<typename T_type, typename Expr>
    struct TFEL_VISIBILITY_LOCAL Name<tfel::math::VectorExpr<T_type,Expr> >
    {
      /*!
       * \brief  Return the name of the class.
       * \return the name of the class.
       * \see    Name.
       */
      static std::string
      getName(void){
	using namespace std;
	using namespace tfel::utilities;
	return string("VectorExpr<")
	  +Name<T_type>::getName()+string(",")
	  +Name<Expr>::getName()+string(">");
      }
    };

    template<unsigned short N, typename T>
    struct TFEL_VISIBILITY_LOCAL Name<tfel::math::tvector<N,T> >
    {
      /*!
       * \brief  Return the name of the class.
       * \return const std::string, the name of the class.
       * \see    Name.
       */
      static std::string 
      getName(void)
      {
	using namespace std;
	using namespace tfel::utilities;
	return string("tvector<")+ToString(N)+string(",")+
	  Name<T>::getName()+string(">");
      }
    };

    template<typename T>
    struct TFEL_VISIBILITY_LOCAL Name<tfel::math::vector<T> >
    {
      /*!
       * \brief  Return the name of the class.
       * \return const std::string, the name of the class.
       * \see    Name.
       */
      static std::string 
      getName(void)
      {
	using namespace std;
	using namespace tfel::utilities;
	return string("tfel::math::vector<")+Name<T>::getName()+string(">");
      }
    };

    template<unsigned short N,unsigned short M, typename T>
    struct TFEL_VISIBILITY_LOCAL Name<tfel::math::tmatrix<N,M,T> >
    {

      /*!
       * \brief  Return the name of the class.
       * \return the name of the class.
       * \see    Name.
       */
      static std::string
      getName(void){
	using namespace std;
	using namespace tfel::utilities;
	return string("tmatrix<")+ToString(N)+string(",")+
	  ToString(M)+string(",")+Name<T>::getName()+string(">");
      }
    }; // end of struct TFEL_VISIBILITY_LOCAL Name<tmatrix<N,M,T> >

    template<unsigned short N, typename T>
    struct TFEL_VISIBILITY_LOCAL Name<tfel::math::StensorStatic<N,T> >
    {
      static std::string getName(void){
	using namespace std;
	using namespace tfel::utilities;
      	return string("StensorStatic<")+ToString(N)+string(",")+Name<T>::getName()+string(">");
      }
    };

    template<unsigned short N, typename T,
	     template<unsigned short,typename> class Storage>
    struct TFEL_VISIBILITY_LOCAL Name<tfel::math::stensor<N,T,Storage> >
    {

      /*!
       * \brief  Return the name of the class.
       * \return the name of the class.
       * \see    Name.
       */
      static std::string
      getName(void){
	using namespace std;
	using namespace tfel::utilities;
	return string("stensor<")
	  +ToString(N)+string(",")
	  +Name<T>::getName()+string(",")
	  +Name<Storage<N,T> >::getName()
	  +string(">");
      }
    }; // end of Name<stensor<N,T,Storage> >

    template<unsigned short N,
	     typename T>
    struct Name<tfel::math::StensorNullStorage<N,T> >
    {
      static std::string getName(void){
	using namespace std;
	using namespace tfel::utilities;
      	return string("StensorNullStorage<")+
	  ToString(N)+string(",")+
	  Name<T>::getName()+string(">");
      }
    };

    template<unsigned short N,
	     typename T>
    struct Name<tfel::math::StensorExternalVoigt<N,T> >
    {
      static std::string getName(void){
	using namespace std;
	using namespace tfel::utilities;
	return string("StensorExternalVoigt<")+
	  ToString(N)+string(",")+
	  Name<T>::getName()+string(">");
      }
    };

    template<unsigned short N,
	     typename T>
    struct Name<tfel::math::StensorExternalTab<N,T> >
    {
      static std::string getName(void){
	using namespace std;
	using namespace tfel::utilities;
	return string("StensorExternalTab<")+
	  ToString(N)+string(",")+
	  Name<T>::getName()+string(">");
      }
    };

    template<typename T>
    struct TFEL_VISIBILITY_LOCAL Name<tfel::math::StensorConcept<T> >
    {
      
      static std::string getName(void)
      {
	return std::string("StensorConcept<")
	  +tfel::utilities::Name<T>::getName()
	  +std::string(">");
      } // end of StensorConcept<T>::getName()

    };

    template<typename T_type, typename Expr>
    struct TFEL_VISIBILITY_LOCAL Name<tfel::math::StensorExpr<T_type,Expr> >
    {
      /*!
       * \brief  Return the name of the class.
       * \return the name of the class.
       * \see    Name.
       */
      static std::string
      getName(void){
	using namespace std;
	using namespace tfel::utilities;
	return string("StensorExpr<")
	  +Name<T_type>::getName()+string(",")
	  +Name<Expr>::getName()+string(">");
      }
    };

    template<typename unit,typename T>
    struct TFEL_VISIBILITY_LOCAL Name<tfel::math::qt<unit,T> >
    {
      /*!
       * \brief  Return the name of the class.
       * \return const std::string, the name of the class.
       * \see    Name.
       */
      static std::string 
      getName(void){
	using namespace std;
	using namespace tfel::utilities;
	return string("qt<")+Name<unit>::getName()+
	  string(",")+Name<T>::getName()+string(">");
      }
    };

    template<unsigned short N,typename T>
    struct TFEL_VISIBILITY_LOCAL Name<tfel::math::st2tost2<N,T> >
    {
      /*!
       * \return the name of the class.
       * \see    Name.
       */
      static std::string
      getName(void){
	using namespace std;
	using namespace tfel::utilities;
	return string("st2tost2<")+ToString(N)+string(",")+Name<T>::getName()+string(">");
      }
    };

    /*!
     * Partial specialisation for 
     */
    template<>
    struct TFELUTILITIES_VISIBILITY_EXPORT Name<tfel::meta::InvalidType>
    {
      static std::string
      getName(void)
      {
	return "InvalidType";
      }
    };

    template<typename T, typename U>
    struct TFEL_VISIBILITY_LOCAL Name<tfel::meta::TLNode<T,U> >
    {
      /*!
       * Return the Name of the TL beginning with this Node.
       * \param void.
       * \return std::string, the Name of the TL.
       */
      static std::string
      getName(void){
	using namespace std;
	using namespace tfel::utilities;
	return string("TLNode<")
	  +Name<T>::getName()+string(",")
	  +Name<U>::getName()+string(">"); 
      }
    }; // end of struct Name<tfel::meta::TLNode<T,U> >
    
    template<>
    struct TFELUTILITIES_VISIBILITY_EXPORT Name<tfel::meta::TLE>
    {
      /*
       * Return the Name of this class.
       * \return the name.
       */
      static std::string
      getName(void);
    };

    template<short N,unsigned short D>
    struct TFEL_VISIBILITY_LOCAL Name<tfel::math::Cst<N,D> >
    {
      /*!						                                  
       * \brief  Return the name of the class.		                                  
       * \return the name of the class.	                                  
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
    };

    template<short N>
    struct TFEL_VISIBILITY_LOCAL Name<tfel::math::Cst<N,1u> >
    {
      /*!						                                  
       * \brief  Return the name of the class.		                                  
       * \return the name of the class.	                                  
       * \see    Name.
       */						                                  
      static std::string getName(void)
      {
	using namespace std;
	using namespace tfel::utilities;
	string name("Cst<");
	name += ToString(N);
	name+=string(">");
	return name;
      }
    };

    template<typename Func>
    struct TFEL_VISIBILITY_LOCAL Name<tfel::math::FunctionConcept<Func> >
    {
      static std::string 
      getName(void){
	using namespace std;
	using namespace tfel::utilities;
	return string("FunctionConcept<")+Name<Func>::getName()+string(">");
      }
    }; // end of struct TFEL_VISIBILITY_LOCAL Name<tfel::math::FunctionConcept>

    template<typename Expr>
    struct TFEL_VISIBILITY_LOCAL Name<tfel::math::FunctionExpr<Expr> >
    {
      static std::string getName(void){
	using namespace std;
	using namespace tfel::utilities;
	return string("FunctionExpr<")+Name<Expr>::getName()+string(">");
      }
    };

    template<typename Func1,typename Func2, typename Op>
    struct TFEL_VISIBILITY_LOCAL Name<tfel::math::FunctionFunctionExpr<Func1,Func2,Op> >
    {
      static std::string 
      getName(void){
	using namespace std;
	using namespace tfel::utilities;
	return string("FunctionFunctionExpr<")
	  +Name<Func1>::getName()+string(",")
	  +Name<Func2>::getName()+string(",")
	  +Name<Op>::getName()+string(">");
      }
    };

    template<typename F>
    struct TFEL_VISIBILITY_LOCAL Name<tfel::math::FunctionNegExpr<F> >
    {
      static std::string 
      getName(void){
	using namespace std;
	using namespace tfel::utilities;
	return string("FunctionNegExpr<")+Name<F>::getName()+string(">");
      }
    };

    template<typename Func,typename Scal, typename Op>
    struct TFEL_VISIBILITY_LOCAL Name<tfel::math::FunctionScalarExpr<Func,Scal,Op> >
    {
      static std::string 
      getName(void){
	using namespace std;
	using namespace tfel::utilities;
	return string("FunctionScalarExpr<")
	  +Name<Func>::getName()+string(",")
	  +Name<Scal>::getName()+string(",")
	  +Name<Op>::getName()+string(">");
      }
    };

    template<typename Func1,typename Func2>
    struct TFEL_VISIBILITY_LOCAL Name<tfel::math::FunctionsCompositionExpr<Func1,Func2> >
    {
      static std::string 
      getName(void){
	using namespace std;
	using namespace tfel::utilities;
	return string("FunctionsCompositionExpr<")
	  +Name<Func1>::getName()+string(",")
	  +Name<Func2>::getName()+string(">");
      }
    };

    template<typename Scal,typename Func, typename Op>
    struct TFEL_VISIBILITY_LOCAL Name<tfel::math::ScalarFunctionExpr<Scal,Func,Op> >
    {
      static std::string 
      getName(void){
	using namespace std;
	using namespace tfel::utilities;
	return string("ScalarFunctionExpr<")
	  +Name<Scal>::getName()+string(",")
	  +Name<Func>::getName()+string(",")
	  +Name<Op>::getName()+string(">");
      }
    };

    template<typename A>
    struct TFEL_VISIBILITY_LOCAL Name<tfel::math::NegObjectRandomAccessConstIterator<A> >
    {
      /*!										    
       * brief  Return the name of the class.						    
       * param  void.									    
       * return std::string, the name of the class.					    
       * see    Name.									    
       */										    
      static std::string getName(void){							    
      	using namespace std;								    
      	using namespace tfel::utilities;						    
      	return string("NegObjectRandomAccessConstIterator<")+Name<A>::getName()+string(">");             
      }											    
    };

    template<typename A,
	     typename Fct>
    struct TFEL_VISIBILITY_LOCAL Name<tfel::math::FctObjectRandomAccessConstIterator<A,Fct> >
    {
      static std::string getName(void){							    
	using namespace std;								    
	using namespace tfel::utilities;						    
	return string("FctObjectRandomAccessConstIterator<")
	  +Name<A>::getName()+string(",")
	  +Name<Fct>::getName()+string(">");             
      }											    
    };

    template<typename A,
	     typename B,
	     typename Op>
    struct TFEL_VISIBILITY_LOCAL Name<tfel::math::ScalarObjectRandomAccessConstIterator<A,B,Op> >
    {
      /*!										    
       * brief  Return the name of the class.						    
       * param  void.									    
       * return std::string, the name of the class.					    
       * see    Name.									    
       */										    
      static std::string getName(void){							    
	using namespace std;								    
	using namespace tfel::utilities;						    
	return string("ScalarObjectRandomAccessConstIterator<")
	  +Name<A>::getName()+string(",")
	  +Name<B>::getName()+string(",")
	  +Name<Op>::getName()+string(">");        
      } // end of getName()
    };

    template<typename A,
	     typename B,
	     typename Op>
    struct TFEL_VISIBILITY_LOCAL Name<tfel::math::ObjectScalarRandomAccessConstIterator<A,B,Op> >
    {
      /*!										    
       * brief  Return the name of the class.						    
       * param  void.									    
       * return std::string, the name of the class.					    
       * see    Name.									    
       */										    
      static std::string getName(void){							    
	using namespace std;								    
	using namespace tfel::utilities;						    
	return string("ObjectScalarRandomAccessConstIterator<")
	  +Name<A>::getName()+string(",")
	  +Name<B>::getName()+string(",")
	  +Name<Op>::getName()+string(">");        
      } // end of getName()
    };

    template<typename A,
	     typename B,
	     typename Op>
    struct TFEL_VISIBILITY_LOCAL Name<tfel::math::ObjectObjectRandomAccessConstIterator<A,B,Op> >
    {
      /*!										    
       * \brief  Return the name of the class.						    
       * \return the name of the class.					    
       * \see    Name.									    
       */
      static std::string
      getName(void){							    
      	using namespace std;								    
      	using namespace tfel::utilities;						    
      	return string("ObjectObjectRandomAccessConstIterator<")
      	  +Name<A>::getName()+string(",")+Name<B>::getName()
      	  +string(",")+Name<Op>::getName()+string(">");
      }
    }; // end of struct TFEL_VISIBILITY_LOCAL Name<tfel::math::ObjectObjectRandomAccessConstIterator<A,B,Op> >

    template<unsigned short N,
	     typename T,
	     typename Func>
    struct TFEL_VISIBILITY_LOCAL Name<tfel::math::TinyNewtonRaphson<N,T,Func> >
    {
      static std::string getName(void)
      {
	using namespace std;
	using namespace tfel::utilities;
	return string("TinyNewtonRaphson<")+
	  ToString(N)+string(",")+
	  Name<T>::getName()+string(",")+
	  Name<Func>::getName()+string(">");
      }
    }; // end of struct TFEL_VISIBILITY_LOCAL Name<tfel::math::TinyNewtonRaphson<N,T,Func> >
      
    /*!
     * \brief Partial specialisation for the NoUnit class. 
     */
    template<>
    struct TFELUTILITIES_VISIBILITY_EXPORT Name<tfel::math::NoUnit>
    {
      /*!
       * \brief  Return the name of the class NoUnit.
       * \return the name of the class.
       * \see    Name.
       */
      static std::string getName(void);
    };

    /*!
     * \brief Partial specialisation for the Mass class. 
     */
    template<>
    struct TFELUTILITIES_VISIBILITY_EXPORT Name<tfel::math::Mass>
    {
      /*!
       * \brief  Return the name of the class Mass.
       * \return the name of the class.
       * \see    Name.
       */
      static std::string getName(void);
    };

    /*!
     * \brief Partial specialisation for the Length class. 
     */
    template<>
    struct TFELUTILITIES_VISIBILITY_EXPORT Name<tfel::math::Length>
    {
      /*!
       * \brief  Return the name of the class Length.
       * \return the name of the class.
       * \see    Name.
       */
      static std::string getName(void);
    };

    /*!
     * \brief Partial specialisation for the Time class. 
     */
    template<>
    struct TFELUTILITIES_VISIBILITY_EXPORT Name<tfel::math::Time>
    {
      /*!
       * \brief  Return the name of the class Time.
       * \return the name of the class.
       * \see    Name.
       */
      static std::string getName(void);
    };

    /*!
     * \brief Partial specialisation for the Ampere class. 
     */
    template<>
    struct TFELUTILITIES_VISIBILITY_EXPORT Name<tfel::math::Ampere>
    {
      /*!
       * \brief  Return the name of the class Ampere.
       * \return the name of the class.
       * \see    Name.
       */
      static std::string getName(void);
    };

    /*!
     * \brief Partial specialisation for the Temperature class. 
     */
    template<>
    struct TFELUTILITIES_VISIBILITY_EXPORT Name<tfel::math::Temperature>
    {
      /*!
       * \brief  Return the name of the class Temperature.
       * \return the name of the class.
       * \see    Name.
       */
      static std::string getName(void);
    };

    /*!
     * \brief Partial specialisation for the Candela class. 
     */
    template<>
    struct TFELUTILITIES_VISIBILITY_EXPORT Name<tfel::math::Candela>
    {
      /*!
       * \brief  Return the name of the class Candela.
       * \return the name of the class.
       * \see    Name.
       */
      static std::string getName(void);
    };

    /*!
     * \brief Partial specialisation for the Mole class. 
     */
    template<>
    struct TFELUTILITIES_VISIBILITY_EXPORT Name<tfel::math::Mole>
    {
      /*!
       * \brief  Return the name of the class Mole.
       * \return the name of the class.
       * \see    Name.
       */
      static std::string getName(void);
    };

    /*!
     * \brief Partial specialisation for the Velocity class. 
     */
    template<>
    struct TFELUTILITIES_VISIBILITY_EXPORT Name<tfel::math::Velocity>
    {
      /*!
       * \brief  Return the name of the class Velocity.
       * \return the name of the class.
       * \see    Name.
       */
      static std::string getName(void);
    };

    /*!
     * \brief Partial specialisation for the Acceleration class. 
     */
    template<>
    struct TFELUTILITIES_VISIBILITY_EXPORT Name<tfel::math::Acceleration>
    {
      /*!
       * \brief  Return the name of the class Acceleration.
       * \return the name of the class.
       * \see    Name.
       */
      static std::string getName(void);
    };

    /*!
     * \brief Partial specialisation for the Momentum class. 
     */
    template<>
    struct TFELUTILITIES_VISIBILITY_EXPORT Name<tfel::math::Momentum>
    {
      /*!
       * \brief  Return the name of the class Momentum.
       * \return the name of the class.
       * \see    Name.
       */
      static std::string getName(void);
    };

    /*!
     * \brief Partial specialisation for the Force class. 
     */
    template<>
    struct TFELUTILITIES_VISIBILITY_EXPORT Name<tfel::math::Force>
    {
      /*!
       * \brief  Return the name of the class Force.
       * \return the name of the class.
       * \see    Name.
       */
      static std::string getName(void);
    };

    template<typename A>
    struct Name<tfel::math::MatrixConcept<A> >
    {
      static std::string 
      getName(void){ 
	using namespace std;
	using namespace tfel::utilities;
	return string("MatrixConcept<")
	  +Name<A>::getName()+string(">");        
      }
    };

    template<typename T_type, typename Expr>
    struct Name<tfel::math::MatrixExpr<T_type,Expr> >
    {
      /*!
       * \brief  Return the name of the class.
       * \return the name of the class.
       * \see    Name.
       */
      static std::string
      getName(void){
	using namespace std;
	using namespace tfel::utilities;
	return string("MatrixExpr<")
	  +Name<T_type>::getName()+string(",")
	  +Name<Expr>::getName()+string(">");
      }
    };

    template<unsigned short N,
	     unsigned short M,
	     typename A, typename B>
    struct Name<tfel::math::TMatrixTVectorExpr<N,M,A,B> >
    {
      static std::string getName(void){
      	using namespace std;
      	using namespace tfel::utilities;
      	return string("TMatrixTVectorExpr<")+
	  Name<A>::getName()+string(",")+
      	  Name<B>::getName()+string(">");
      }
    };

    template<unsigned short N,
	     unsigned short M,
	     typename A,
	     typename B>
    struct Name<tfel::math::TVectorTMatrixExpr<N,M,A,B> >
    {
      static std::string getName(void){
      	using namespace std;
      	using namespace tfel::utilities;
      	return string("TVectorTMatrixExpr<")+
	  Name<A>::getName()+string(",")+
      	  Name<B>::getName()+string(">");
      }
    };

    template<template<typename> class Concept, template<typename> class Traits,
	     typename A, typename B>
    struct TFEL_VISIBILITY_LOCAL Name<tfel::math::FctMathObjectExpr<Concept,Traits,A,B> >
    {
      static std::string getName(void){
      	using namespace std;
      	using namespace tfel::utilities;
      	return string("FctMathObjectExpr<")+
	  Name<A>::getName()+string(",")+
	  Name<B>::getName()+string(">");
      }
    }; // end of struct TFEL_VISIBILITY_LOCAL Name<tfel::math::FctMathObjectExpr<Concept,Traits,A,B,Op> >

    template<template<typename> class Concept, template<typename> class Traits,
	     typename A, typename B>
    struct TFEL_VISIBILITY_LOCAL Name<tfel::math::FctMathObjectExprWithoutConstIterator<Concept,Traits,A,B> >
    {
      static std::string getName(void){
      	using namespace std;
      	using namespace tfel::utilities;
      	return string("FctMathObjectExprWithoutConstIterator<")+
	  Name<A>::getName()+string(",")+
	  Name<B>::getName()+string(">");
      }
    }; // end of struct TFEL_VISIBILITY_LOCAL Name<tfel::math::FctMathObjectExprWithoutConstIterator<Concept,Traits,A,B,Op> >

    template<template<typename> class Concept, template<typename> class Traits,
	     typename A, typename B,typename Op>
    struct TFEL_VISIBILITY_LOCAL Name<tfel::math::MathObjectMathObjectExpr<Concept,Traits,A,B,Op> >
    {
      static std::string getName(void){
      	using namespace std;
      	using namespace tfel::utilities;
      	return string("MathObjectMathObjectExpr<")+
	  Name<A>::getName()+string(",")+
	  Name<B>::getName()+string(",")+
	  Name<Op>::getName()+string(">");
      }
    }; // end of struct TFEL_VISIBILITY_LOCAL Name<tfel::math::MathObjectMathObjectExpr<Concept,Traits,A,B,Op> >

    template<template<typename> class Concept, template<typename> class Traits,
	     typename A, typename B,typename Op>
    struct TFEL_VISIBILITY_LOCAL Name<tfel::math::MathObjectMathObjectExprWithoutConstIterator<Concept,Traits,A,B,Op> >
    {
      static std::string getName(void){
      	using namespace std;
      	using namespace tfel::utilities;
      	return string("MathObjectMathObjectExprWithoutConstIterator<")+
	  Name<A>::getName()+string(",")+
	  Name<B>::getName()+string(",")+
	  Name<Op>::getName()+string(">");
      }
    }; // end of struct TFEL_VISIBILITY_LOCAL Name<tfel::math::MathObjectMathObjectExprWithoutConstIterator<Concept,Traits,A,B,Op> >

    template<template<typename> class Concept, template<typename> class Traits,
	     typename A,typename B,typename Op>
    struct TFEL_VISIBILITY_LOCAL Name<tfel::math::ScalarMathObjectExpr<Concept,Traits,A,B,Op> >
    {
      static std::string getName(void){
	using namespace tfel::utilities;
	using namespace std;
	return string("ScalarMathObjectExpr<")+
	  Name<A>::getName()+
	  string(",")+
	  Name<B>::getName()+
	  string(",")+
	  Name<Op>::getName()+
	  string(">");
      }
    };

    template<template<typename> class Concept, template<typename> class Traits,
	     typename A,typename B,typename Op>
    struct TFEL_VISIBILITY_LOCAL
    Name<tfel::math::MathObjectScalarExpr<Concept,Traits,A,B,Op> >
    {
      static std::string getName(void){
      	using namespace std;
      	using namespace tfel::utilities;
      	return string("MathObjectScalarExpr<")+
	  Name<A>::getName()+string(",")+
	  Name<B>::getName()+string(",")+
	  Name<Op>::getName()+string(">");
      }
    }; // end of struct TFEL_VISIBILITY_LOCAL Name<tfel::math::MathObjectScalarExpr<A,B,Op> >

    template<template<typename> class Concept, template<typename> class Traits,
	     typename A,typename B,typename Op>
    struct Name<tfel::math::ScalarMathObjectExprWithoutConstIterator<Concept,Traits,A,B,Op> >
    {
      static std::string getName(void){
	using namespace tfel::utilities;
	using namespace std;
	return string("ScalarMathObjectExprWithoutConstIterator<")+
	  Name<A>::getName()+
	  string(",")+
	  Name<B>::getName()+
	  string(",")+
	  Name<Op>::getName()+
	  string(">");
      }
    };

    template<template<typename> class Concept, template<typename> class Traits,
	     typename A,typename B,typename Op>
    struct Name<tfel::math::MathObjectScalarExprWithoutConstIterator<Concept,Traits,A,B,Op> >
    {
      static std::string getName(void){
      	using namespace std;
      	using namespace tfel::utilities;
      	return string("MathObjectScalarExprWithoutConstIterator<")+
	  Name<A>::getName()+string(",")+
	  Name<B>::getName()+string(",")+
	  Name<Op>::getName()+string(">");
      }
    }; // end of struct Name<tfel::math::MathObjectScalarExprWithoutConstIterator<A,B,Op> >

    template<template<typename> class Concept, template<typename> class Traits,
	     typename F>
    struct Name<tfel::math::MathObjectNegExpr<Concept,Traits,F> >
    {
      static std::string 
      getName(void){
	using namespace std;
	using namespace tfel::utilities;
	return string("MathObjectNegExpr<")+Name<F>::getName()+string(">");
      }
    };

    template<unsigned int N>
    struct TFEL_VISIBILITY_LOCAL Name<tfel::meta::Int2Type<N> >
    {
      /*!
       * Return the Name of this class 
       */
      static std::string 
      getName(void){
	using namespace std;
	using namespace tfel::utilities;
	return string("Int2Type<")+ToString(N)+string(">");
      }
    };

    template<unsigned int N>
    struct TFEL_VISIBILITY_LOCAL Name<tfel::meta::UInt2Type<N> >
    {
      /*!
       * Return the Name of this class 
       */
      static std::string 
      getName(void){
	using namespace std;
	using namespace tfel::utilities;
	return string("UInt2Type<")+ToString(N)+string(">");
      }
    };

    /*!
     * return the class name of an object
     * \param T : object class
     */
    template<typename T>
    std::string
    name(const T&)
    {
      return Name<T>::getName();
    } // end of name

  } // end of namespace utilities

  namespace meta{

    namespace internals {

      template<typename List>
      class TFEL_VISIBILITY_LOCAL TLGetNames_
      {
	typedef typename List::Current Current;
	typedef typename List::Next Next;
      public:
	static std::string exe(void)
	{
	  using namespace std;
	  using namespace tfel::utilities;
	  return string(",")+Name<Current>::getName()
	    +tfel::meta::internals::TLGetNames_<Next>::exe();
	}
      };

      template<>
      struct TFELUTILITIES_VISIBILITY_EXPORT TLGetNames_<TLE>
      {
	static std::string exe(void);
      };

    } // end of namespace internals

    template<typename List>
    class TFEL_VISIBILITY_LOCAL TLGetNames
    {
      typedef typename List::Current Current;
      typedef typename List::Next Next;
    public:
      static std::string exe(void)
      {
	using namespace std;
	using namespace tfel::utilities;
	return string("{")+Name<Current>::getName()
	  +tfel::meta::internals::TLGetNames_<Next>::exe();
      }
    };

    template<>
    struct TFEL_VISIBILITY_LOCAL TLGetNames<TLE>
    {
      static std::string
      exe(void)
      {
	return "{}";
      }
    };

  } // end of namespace meta

} // end of namespace tfel

#endif /* _LIB_TFEL_NAME_I_ */

