/*!
 * \file   Name.ixx
 * 
 * \brief  Specialise Name for the most used standard types.  
 * \author Helfer Thomas
 * \date   17 Jul 2006
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
#include"TFEL/Math/Forward/array.hxx"
#include"TFEL/Math/Forward/vector.hxx"
#include"TFEL/Math/Forward/lambda.hxx"
#include"TFEL/Math/Forward/stensor.hxx"
#include"TFEL/Math/Forward/tvector.hxx"
#include"TFEL/Math/Forward/tmatrix.hxx"
#include"TFEL/Math/Forward/Complex.hxx"
#include"TFEL/Math/Forward/st2tost2.hxx"
#include"TFEL/Math/Forward/Function.hxx"
#include"TFEL/Math/Forward/ArrayConcept.hxx"
#include"TFEL/Math/Forward/LambdaConcept.hxx"
#include"TFEL/Math/Forward/VectorConcept.hxx"
#include"TFEL/Math/Forward/MatrixConcept.hxx"
#include"TFEL/Math/Forward/StensorConcept.hxx"
#include"TFEL/Math/Forward/ST2toST2Concept.hxx"
#include"TFEL/Math/Forward/CompositeConcept.hxx"
#include"TFEL/Math/Forward/TinyNewtonRaphson.hxx"
#include"TFEL/Math/General/Forward/General.hxx"

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
    struct TFEL_VISIBILITY_EXPORT Name< X >                  \
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
    struct TFEL_VISIBILITY_EXPORT Name<X*>                   \
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
    struct TFEL_VISIBILITY_EXPORT Name<const X * const>      \
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
    struct TFEL_VISIBILITY_EXPORT Name<const X *>            \
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
    struct TFEL_VISIBILITY_EXPORT Name<X &>                  \
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
    struct TFEL_VISIBILITY_EXPORT Name<const X &>            \
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
    struct TFEL_VISIBILITY_EXPORT Name<void>                    \
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
    struct TFEL_VISIBILITY_EXPORT Name<void*>                   \
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
    struct TFEL_VISIBILITY_EXPORT Name<const void* const>       \
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
    struct TFEL_VISIBILITY_EXPORT Name<const void *>            \
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

    template<typename A>
    struct Name<tfel::math::ArrayConcept<A> >
    {
      static std::string 
      getName(void){ 
	using namespace std;
	using namespace tfel::utilities;
	return string("ArrayConcept<")
	  +Name<A>::getName()+string(">");        
      }
    };

    template<typename T_type, typename Expr>
    struct Name<tfel::math::ArrayExpr<T_type,Expr> >
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
	return string("ArrayExpr<")
	  +Name<T_type>::getName()+string(",")
	  +Name<Expr>::getName()+string(">");
      }
    };

    template<typename A,
	     typename B,
	     typename Op>
    struct Name<tfel::math::ScalarArrayExpr<A,B,Op> >
    {
      static std::string getName(void){
	using namespace tfel::utilities;
	using namespace std;
	return string("ScalarArrayExpr<")+
	  Name<A>::getName()+
	  string(",")+
	  Name<B>::getName()+
	  string(",")+
	  Name<Op>::getName()+
	  string(">");
      }
    };

    template<typename A,
	     typename B,
	     typename Op>
    struct Name<tfel::math::ArrayScalarExpr<A,B,Op> >
    {
      static std::string getName(void){
      	using namespace std;
      	using namespace tfel::utilities;
      	return string("ArrayScalarExpr<")+
	  Name<A>::getName()+string(",")+
	  Name<B>::getName()+string(",")+
	  Name<Op>::getName()+string(">");
      }
    }; // end of struct Name<tfel::math::ArrayScalarExpr<A,B,Op> >

    template<typename A,
	     typename B,
	     typename Op>
    struct Name<tfel::math::ScalarArrayExprWithoutConstIterator<A,B,Op> >
    {
      static std::string getName(void){
	using namespace tfel::utilities;
	using namespace std;
	return string("ScalarArrayExprWithoutConstIterator<")+
	  Name<A>::getName()+
	  string(",")+
	  Name<B>::getName()+
	  string(",")+
	  Name<Op>::getName()+
	  string(">");
      }
    };

    template<typename A,
	     typename B,
	     typename Op>
    struct Name<tfel::math::ArrayScalarExprWithoutConstIterator<A,B,Op> >
    {
      static std::string getName(void){
      	using namespace std;
      	using namespace tfel::utilities;
      	return string("ArrayScalarExprWithoutConstIterator<")+
	  Name<A>::getName()+string(",")+
	  Name<B>::getName()+string(",")+
	  Name<Op>::getName()+string(">");
      }
    }; // end of struct Name<tfel::math::ArrayScalarExprWithoutConstIterator<A,B,Op> >

    template<typename A, typename B,typename Op>
    struct Name<tfel::math::ArrayArrayExpr<A,B,Op> >
    {
      static std::string getName(void){
      	using namespace std;
      	using namespace tfel::utilities;
      	return string("ArrayArrayExpr<")+
	  Name<A>::getName()+string(",")+
	  Name<B>::getName()+string(",")+
	  Name<Op>::getName()+string(">");
      }
    }; // end of struct Name<tfel::math::ArrayArrayExpr<A,B,Op> >

    template<typename A,
	     typename B,
	     typename Op>
    struct TFEL_VISIBILITY_LOCAL Name<tfel::math::ArrayArrayExprWithoutConstIterator<A,B,Op> >
    {
      static std::string getName(void){
      	using namespace std;
      	using namespace tfel::utilities;
      	return string("ArrayArrayExprWithoutConstIterator<")+
	  Name<A>::getName()+
	  string(",")+
	  Name<B>::getName()+
	  string(",")+
	  Name<Op>::getName()+
	  string(">");
      }
    };

    template<typename F>
    struct Name<tfel::math::ArrayNegExpr<F> >
    {
      static std::string 
      getName(void){
	using namespace std;
	using namespace tfel::utilities;
	return string("ArrayNegExpr<")+Name<F>::getName()+string(">");
      }
    };

    template<typename A,typename Func>
    struct Name<tfel::math::FctArrayExpr<A,Func> >
    {
      static std::string 
      getName(void){ 
	using namespace std;
	using namespace tfel::utilities;
	return string("FctArrayExpr<")
	  +Name<A>::getName()+string(",")
	  +Name<Func>::getName()+string(">");        
      }
    };

    template<typename A,
	     typename Func>
    struct Name<tfel::math::FctArrayExprWithoutConstIterator<A,Func> >
    {
      static std::string 
      getName(void){ 
      	using namespace std;
      	using namespace tfel::utilities;
      	return string("FctArrayExprWithoutConstIterator<")
      	  +Name<A>::getName()+string(",")
      	  +Name<Func>::getName()+string(">");
      }
    };

    template<typename T,
	     typename Ordering,
	     typename TAllocator>
    struct TFEL_VISIBILITY_LOCAL Name<tfel::math::ArrayStorage_<T,Ordering,TAllocator> >
    {
      static std::string getName(void){
	using namespace std;
	using namespace tfel::utilities;
	return string("ArrayStorage_<")+
	  Name<T>::getName()+string(",")+
	  Name<Ordering>::getName()+string(",")+
	  Name<TAllocator>::getName()+string(">");
      }
    };

    template<unsigned short N,
	     typename T,
	     typename Ordering,
	     typename TAllocator>
    struct Name<tfel::math::ArrayStorage<N,T,Ordering,TAllocator> >
    {
      static std::string getName(void){
	using namespace std;
	using namespace tfel::utilities;
	return string("ArrayStorage<,")+ToString(N)+
	  Name<T>::getName()+string(",")+
	  Name<Ordering>::getName()+string(",")+
	  Name<TAllocator>::getName()+string(">");
      }
    };

    template<unsigned short N,typename T,
	     template<unsigned short,typename> class TStorage>
    struct TFEL_VISIBILITY_LOCAL Name<tfel::math::array<N,T,TStorage> >
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
	string tmp = "array<"+ToString(N)+","+Name<T>::getName();
	tmp+=","+Name<TStorage<N,T> >::getName()+">";
	return tmp;
      }
    }; // end of Name<array<N,T,TStorage> >

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

    template<typename A,
	     typename B,
	     typename Op>
    struct TFEL_VISIBILITY_LOCAL Name<tfel::math::VectorVectorExprWithoutConstIterator<A,B,Op> >
    {
      static std::string getName(void){
      	using namespace std;
      	using namespace tfel::utilities;
      	return string("VectorVectorExprWithoutConstIterator<")+
	  Name<A>::getName()+
	  string(",")+
	  Name<B>::getName()+
	  string(",")+
	  Name<Op>::getName()+
	  string(">");
      }
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

    template<typename A, typename B,typename Op>
    struct TFEL_VISIBILITY_LOCAL Name<tfel::math::VectorVectorExpr<A,B,Op> >
    {
      static std::string getName(void){
      	using namespace std;
      	using namespace tfel::utilities;
      	return string("VectorVectorExpr<")+
	  Name<A>::getName()+string(",")+
	  Name<B>::getName()+string(",")+
	  Name<Op>::getName()+string(">");
      }
    }; // end of struct TFEL_VISIBILITY_LOCAL Name<tfel::math::VectorVectorExpr<A,B,Op> >

    template<typename A,typename Func>
    struct TFEL_VISIBILITY_LOCAL Name<tfel::math::FctVectorExpr<A,Func> >
    {
      static std::string 
      getName(void){ 
	using namespace std;
	using namespace tfel::utilities;
	return string("FctVectorExpr<")
	  +Name<A>::getName()+string(",")
	  +Name<Func>::getName()+string(">");        
      }
    };

    template<typename F>
    struct TFEL_VISIBILITY_LOCAL Name<tfel::math::VectorNegExpr<F> >
    {
      static std::string 
      getName(void){
	using namespace std;
	using namespace tfel::utilities;
	return string("VectorNegExpr<")+Name<F>::getName()+string(">");
      }
    };

    template<typename F>
    struct TFEL_VISIBILITY_LOCAL Name<tfel::math::VectorNegExprWithoutConstIterator<F> >
    {
      static std::string 
      getName(void){
	using namespace std;
	using namespace tfel::utilities;
	return string("VectorNegExprWithoutConstIterator<")+Name<F>::getName()+string(">");
      }
    };

    template<typename A,
	     typename Func>
    struct TFEL_VISIBILITY_LOCAL Name<tfel::math::FctVectorExprWithoutConstIterator<A,Func> >
    {
      static std::string 
      getName(void){ 
      	using namespace std;
      	using namespace tfel::utilities;
      	return string("FctVectorExprWithoutConstIterator<")
      	  +Name<A>::getName()+string(",")
      	  +Name<Func>::getName()+string(">");
      }
    };

    template<typename A,
	     typename B>
    struct TFEL_VISIBILITY_LOCAL Name<tfel::math::VectorVectorDiadicProductExpr<A,B> >
    {
      static std::string getName(void){
      	using namespace std;
      	using namespace tfel::utilities;
      	return string("VectorVectorDiadicProductExpr<")+
      	  Name<A>::getName()+string(",")+
      	  Name<B>::getName()+string(">");
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

    template<typename A, typename B,typename Op>
    struct TFEL_VISIBILITY_LOCAL Name<tfel::math::StensorStensorExpr<A,B,Op> >
    {
      static std::string getName(void){
      	using namespace std;
      	using namespace tfel::utilities;
      	return string("StensorStensorExpr<")+
	  Name<A>::getName()+string(",")+
	  Name<B>::getName()+string(",")+
	  Name<Op>::getName()+string(">");
      }
    }; // end of struct TFEL_VISIBILITY_LOCAL Name<tfel::math::StensorStensorExpr<A,B,Op> >

    template<typename A,
	     typename B,
	     typename Op>
    struct TFEL_VISIBILITY_LOCAL Name<tfel::math::StensorStensorExprWithoutConstIterator<A,B,Op> >
    {
      static std::string getName(void){
      	using namespace std;
      	using namespace tfel::utilities;
      	return string("StensorStensorExprWithoutConstIterator<")+
	  Name<A>::getName()+
	  string(",")+
	  Name<B>::getName()+
	  string(",")+
	  Name<Op>::getName()+
	  string(">");
      }
    };

    template<typename A,
	     typename B,
	     typename Op>
    struct TFEL_VISIBILITY_LOCAL Name<tfel::math::ScalarStensorExpr<A,B,Op> >
    {
      static std::string getName(void){
	using namespace tfel::utilities;
	using namespace std;
	return string("ScalarStensorExpr<")+
	  Name<A>::getName()+
	  string(",")+
	  Name<B>::getName()+
	  string(",")+
	  Name<Op>::getName()+
	  string(">");
      }
    };

    template<typename A,
	     typename B,
	     typename Op>
    struct TFEL_VISIBILITY_LOCAL Name<tfel::math::StensorScalarExpr<A,B,Op> >
    {
      static std::string getName(void){
      	using namespace std;
      	using namespace tfel::utilities;
      	return string("StensorScalarExpr<")+
	  Name<A>::getName()+string(",")+
	  Name<B>::getName()+string(",")+
	  Name<Op>::getName()+string(">");
      }
    }; // end of struct TFEL_VISIBILITY_LOCAL Name<tfel::math::StensorScalarExpr<A,B,Op> >

    template<typename A,
	     typename B,
	     typename Op>
    struct Name<tfel::math::ScalarStensorExprWithoutConstIterator<A,B,Op> >
    {
      static std::string getName(void){
	using namespace tfel::utilities;
	using namespace std;
	return string("ScalarStensorExprWithoutConstIterator<")+
	  Name<A>::getName()+
	  string(",")+
	  Name<B>::getName()+
	  string(",")+
	  Name<Op>::getName()+
	  string(">");
      }
    };

    template<typename A,
	     typename B,
	     typename Op>
    struct Name<tfel::math::StensorScalarExprWithoutConstIterator<A,B,Op> >
    {
      static std::string getName(void){
      	using namespace std;
      	using namespace tfel::utilities;
      	return string("StensorScalarExprWithoutConstIterator<")+
	  Name<A>::getName()+string(",")+
	  Name<B>::getName()+string(",")+
	  Name<Op>::getName()+string(">");
      }
    }; // end of struct Name<tfel::math::StensorScalarExprWithoutConstIterator<A,B,Op> >

    template<typename F>
    struct TFEL_VISIBILITY_LOCAL Name<tfel::math::StensorNegExpr<F> >
    {
      static std::string 
      getName(void){
	using namespace std;
	using namespace tfel::utilities;
	return string("StensorNegExpr<")+Name<F>::getName()+string(">");
      }
    };

    template<typename F>
    struct TFEL_VISIBILITY_LOCAL Name<tfel::math::StensorNegExprWithoutConstIterator<F> >
    {
      static std::string 
      getName(void){
	using namespace std;
	using namespace tfel::utilities;
	return string("StensorNegExprWithoutConstIterator<")+Name<F>::getName()+string(">");
      }
    };

    template<typename A,
	     typename B>
    struct TFEL_VISIBILITY_LOCAL Name<tfel::math::StensorStensorDiadicProductExpr<A,B> >
    {
      static std::string getName(void){
      	using namespace std;
      	using namespace tfel::utilities;
      	return string("StensorStensorDiadicProductExpr<")+
      	  Name<A>::getName()+string(",")+
      	  Name<B>::getName()+string(">");
      }
    };

    template<unsigned short N>
    struct TFEL_VISIBILITY_LOCAL Name<tfel::math::Lambda<N> >
    {
      /*!
       * \return the name of the class.
       * \see    Name.
       */
      static std::string
      getName(void){
	using namespace std;
	using namespace tfel::utilities;
	return std::string("x")+ToString(N)+string("_");
      }
    }; // end of struct TFEL_VISIBILITY_LOCAL Name<Lambda<N> >

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
    struct TFEL_VISIBILITY_EXPORT Name<tfel::meta::InvalidType>
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
    }; // end of struct TFEL_VISIBILITY_LOCAL Name<tfel::meta::TLNode<T,U> >
    
    template<>
    struct TFEL_VISIBILITY_EXPORT Name<tfel::meta::TLE>
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

    template<typename A,
	     typename B,
	     typename Op>
    struct TFEL_VISIBILITY_LOCAL Name<tfel::math::ScalarVectorExpr<A,B,Op> >
    {
      static std::string getName(void){
	using namespace tfel::utilities;
	using namespace std;
	return string("ScalarVectorExpr<")+
	  Name<A>::getName()+
	  string(",")+
	  Name<B>::getName()+
	  string(",")+
	  Name<Op>::getName()+
	  string(">");
      }
    };

    template<typename A,
	     typename B,
	     typename Op>
    struct TFEL_VISIBILITY_LOCAL Name<tfel::math::VectorScalarExpr<A,B,Op> >
    {
      static std::string getName(void){
      	using namespace std;
      	using namespace tfel::utilities;
      	return string("VectorScalarExpr<")+
	  Name<A>::getName()+string(",")+
	  Name<B>::getName()+string(",")+
	  Name<Op>::getName()+string(">");
      }
    }; // end of struct TFEL_VISIBILITY_LOCAL Name<tfel::math::VectorScalarExpr<A,B,Op> >

    template<typename A,
	     typename B,
	     typename Op>
    struct Name<tfel::math::ScalarVectorExprWithoutConstIterator<A,B,Op> >
    {
      static std::string getName(void){
	using namespace tfel::utilities;
	using namespace std;
	return string("ScalarVectorExprWithoutConstIterator<")+
	  Name<A>::getName()+
	  string(",")+
	  Name<B>::getName()+
	  string(",")+
	  Name<Op>::getName()+
	  string(">");
      }
    };

    template<typename A,
	     typename B,
	     typename Op>
    struct Name<tfel::math::VectorScalarExprWithoutConstIterator<A,B,Op> >
    {
      static std::string getName(void){
      	using namespace std;
      	using namespace tfel::utilities;
      	return string("VectorScalarExprWithoutConstIterator<")+
	  Name<A>::getName()+string(",")+
	  Name<B>::getName()+string(",")+
	  Name<Op>::getName()+string(">");
      }
    }; // end of struct Name<tfel::math::VectorScalarExprWithoutConstIterator<A,B,Op> >

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
    struct TFEL_VISIBILITY_EXPORT Name<tfel::math::NoUnit>
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
    struct TFEL_VISIBILITY_EXPORT Name<tfel::math::Mass>
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
    struct TFEL_VISIBILITY_EXPORT Name<tfel::math::Length>
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
    struct TFEL_VISIBILITY_EXPORT Name<tfel::math::Time>
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
    struct TFEL_VISIBILITY_EXPORT Name<tfel::math::Ampere>
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
    struct TFEL_VISIBILITY_EXPORT Name<tfel::math::Temperature>
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
    struct TFEL_VISIBILITY_EXPORT Name<tfel::math::Candela>
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
    struct TFEL_VISIBILITY_EXPORT Name<tfel::math::Mole>
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
    struct TFEL_VISIBILITY_EXPORT Name<tfel::math::Velocity>
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
    struct TFEL_VISIBILITY_EXPORT Name<tfel::math::Acceleration>
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
    struct TFEL_VISIBILITY_EXPORT Name<tfel::math::Momentum>
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
    struct TFEL_VISIBILITY_EXPORT Name<tfel::math::Force>
    {
      /*!
       * \brief  Return the name of the class Force.
       * \return the name of the class.
       * \see    Name.
       */
      static std::string getName(void);
    };

    template<typename T>
    struct TFEL_VISIBILITY_LOCAL Name<tfel::math::LambdaConcept<T> >
    {
      static std::string getName(void)
      {
	return std::string("LambdaConcept<")
	  +tfel::utilities::Name<T>::getName()
	  +std::string(">");
      } // end of LambdaConcept<T>::getName()
    };

    template<typename T_type, typename Expr>
    struct TFEL_VISIBILITY_LOCAL Name<tfel::math::LambdaExpr<T_type,Expr> >
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
	return string("LambdaExpr<")
	  +Name<T_type>::getName()+string(",")
	  +Name<Expr>::getName()+string(">");
      }
    };

    template<typename A, typename B,typename Op>
    struct TFEL_VISIBILITY_LOCAL Name<tfel::math::LambdaLambdaExpr<A,B,Op> >
    {
      static std::string getName(void){
      	using namespace std;
      	using namespace tfel::utilities;
      	return string("LambdaLambdaExpr<")+
	  Name<A>::getName()+string(",")+
	  Name<B>::getName()+string(",")+
	  Name<Op>::getName()+string(">");
      }
    }; // end of struct TFEL_VISIBILITY_LOCAL Name<tfel::math::LambdaLambdaExpr<A,B,Op> >

    template<typename A,typename Func>
    struct TFEL_VISIBILITY_LOCAL Name<tfel::math::FctLambdaExpr<A,Func> >
    {
      static std::string 
      getName(void){ 
	using namespace std;
	using namespace tfel::utilities;
	return string("FctLambdaExpr<")
	  +Name<A>::getName()+string(",")
	  +Name<Func>::getName()+string(">");        
      }
    };

    template<typename F>
    struct TFEL_VISIBILITY_LOCAL Name<tfel::math::LambdaNegExpr<F> >
    {
      static std::string 
      getName(void){
	using namespace std;
	using namespace tfel::utilities;
	return string("LambdaNegExpr<")+Name<F>::getName()+string(">");
      }
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

    template<typename A,
	     typename B,
	     typename Op>
    struct Name<tfel::math::ScalarMatrixExpr<A,B,Op> >
    {
      static std::string getName(void){
	using namespace tfel::utilities;
	using namespace std;
	return string("ScalarMatrixExpr<")+
	  Name<A>::getName()+
	  string(",")+
	  Name<B>::getName()+
	  string(",")+
	  Name<Op>::getName()+
	  string(">");
      }
    };

    template<typename A,
	     typename B,
	     typename Op>
    struct Name<tfel::math::MatrixScalarExpr<A,B,Op> >
    {
      static std::string getName(void){
      	using namespace std;
      	using namespace tfel::utilities;
      	return string("MatrixScalarExpr<")+
	  Name<A>::getName()+string(",")+
	  Name<B>::getName()+string(",")+
	  Name<Op>::getName()+string(">");
      }
    }; // end of struct Name<tfel::math::MatrixScalarExpr<A,B,Op> >

    template<typename A,
	     typename B,
	     typename Op>
    struct Name<tfel::math::ScalarMatrixExprWithoutConstIterator<A,B,Op> >
    {
      static std::string getName(void){
	using namespace tfel::utilities;
	using namespace std;
	return string("ScalarMatrixExprWithoutConstIterator<")+
	  Name<A>::getName()+
	  string(",")+
	  Name<B>::getName()+
	  string(",")+
	  Name<Op>::getName()+
	  string(">");
      }
    };

    template<typename A,
	     typename B,
	     typename Op>
    struct Name<tfel::math::MatrixScalarExprWithoutConstIterator<A,B,Op> >
    {
      static std::string getName(void){
      	using namespace std;
      	using namespace tfel::utilities;
      	return string("MatrixScalarExprWithoutConstIterator<")+
	  Name<A>::getName()+string(",")+
	  Name<B>::getName()+string(",")+
	  Name<Op>::getName()+string(">");
      }
    }; // end of struct Name<tfel::math::MatrixScalarExprWithoutConstIterator<A,B,Op> >

    template<typename A, typename B,typename Op>
    struct Name<tfel::math::MatrixMatrixExpr<A,B,Op> >
    {
      static std::string getName(void){
      	using namespace std;
      	using namespace tfel::utilities;
      	return string("MatrixMatrixExpr<")+
	  Name<A>::getName()+string(",")+
	  Name<B>::getName()+string(",")+
	  Name<Op>::getName()+string(">");
      }
    }; // end of struct Name<tfel::math::MatrixMatrixExpr<A,B,Op> >

    template<typename A,
	     typename B,
	     typename Op>
    struct TFEL_VISIBILITY_LOCAL Name<tfel::math::MatrixMatrixExprWithoutConstIterator<A,B,Op> >
    {
      static std::string getName(void){
      	using namespace std;
      	using namespace tfel::utilities;
      	return string("MatrixMatrixExprWithoutConstIterator<")+
	  Name<A>::getName()+
	  string(",")+
	  Name<B>::getName()+
	  string(",")+
	  Name<Op>::getName()+
	  string(">");
      }
    };

    template<typename F>
    struct Name<tfel::math::MatrixNegExpr<F> >
    {
      static std::string 
      getName(void){
	using namespace std;
	using namespace tfel::utilities;
	return string("MatrixNegExpr<")+Name<F>::getName()+string(">");
      }
    };

    template<typename A,typename Func>
    struct Name<tfel::math::FctMatrixExpr<A,Func> >
    {
      static std::string 
      getName(void){ 
	using namespace std;
	using namespace tfel::utilities;
	return string("FctMatrixExpr<")
	  +Name<A>::getName()+string(",")
	  +Name<Func>::getName()+string(">");        
      }
    };

    template<typename A,
	     typename Func>
    struct Name<tfel::math::FctMatrixExprWithoutConstIterator<A,Func> >
    {
      static std::string 
      getName(void){ 
      	using namespace std;
      	using namespace tfel::utilities;
      	return string("FctMatrixExprWithoutConstIterator<")
      	  +Name<A>::getName()+string(",")
      	  +Name<Func>::getName()+string(">");
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

    template<typename A>
    struct Name<tfel::math::ST2toST2Concept<A> >
    {
      static std::string 
      getName(void){ 
	using namespace std;
	using namespace tfel::utilities;
	return string("ST2toST2Concept<")
	  +Name<A>::getName()+string(">");        
      }
    };

    template<typename T_type, typename Expr>
    struct Name<tfel::math::ST2toST2Expr<T_type,Expr> >
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
	return string("ST2toST2Expr<")
	  +Name<T_type>::getName()+string(",")
	  +Name<Expr>::getName()+string(">");
      }
    };

    template<typename A,
	     typename B,
	     typename Op>
    struct Name<tfel::math::ScalarST2toST2Expr<A,B,Op> >
    {
      static std::string getName(void){
	using namespace tfel::utilities;
	using namespace std;
	return string("ScalarST2toST2Expr<")+
	  Name<A>::getName()+
	  string(",")+
	  Name<B>::getName()+
	  string(",")+
	  Name<Op>::getName()+
	  string(">");
      }
    };

    template<typename A,
	     typename B,
	     typename Op>
    struct Name<tfel::math::ST2toST2ScalarExpr<A,B,Op> >
    {
      static std::string getName(void){
      	using namespace std;
      	using namespace tfel::utilities;
      	return string("ST2toST2ScalarExpr<")+
	  Name<A>::getName()+string(",")+
	  Name<B>::getName()+string(",")+
	  Name<Op>::getName()+string(">");
      }
    }; // end of struct Name<tfel::math::ST2toST2ScalarExpr<A,B,Op> >

    template<typename A, typename B,typename Op>
    struct Name<tfel::math::ST2toST2ST2toST2Expr<A,B,Op> >
    {
      static std::string getName(void){
      	using namespace std;
      	using namespace tfel::utilities;
      	return string("ST2toST2ST2toST2Expr<")+
	  Name<A>::getName()+string(",")+
	  Name<B>::getName()+string(",")+
	  Name<Op>::getName()+string(">");
      }
    }; // end of struct Name<tfel::math::ST2toST2ST2toST2Expr<A,B,Op> >

    template<typename A,
	     typename B,
	     typename Op>
    struct TFEL_VISIBILITY_LOCAL Name<tfel::math::ST2toST2ST2toST2ExprWithoutConstIterator<A,B,Op> >
    {
      static std::string getName(void){
      	using namespace std;
      	using namespace tfel::utilities;
      	return string("ST2toST2ST2toST2ExprWithoutConstIterator<")+
	  Name<A>::getName()+
	  string(",")+
	  Name<B>::getName()+
	  string(",")+
	  Name<Op>::getName()+
	  string(">");
      }
    };

    template<typename F>
    struct Name<tfel::math::ST2toST2NegExpr<F> >
    {
      static std::string 
      getName(void){
	using namespace std;
	using namespace tfel::utilities;
	return string("ST2toST2NegExpr<")+Name<F>::getName()+string(">");
      }
    };

    template<typename T>
    struct TFEL_VISIBILITY_LOCAL Name<tfel::math::CompositeConcept<T> >
    {
      static std::string getName(void)
      {
	return std::string("CompositeConcept<")
	  +tfel::utilities::Name<T>::getName()
	  +std::string(">");
      } // end of CompositeConcept<T>::getName()
    };

    template<typename T_type, typename Expr>
    struct TFEL_VISIBILITY_LOCAL Name<tfel::math::CompositeExpr<T_type,Expr> >
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
	return string("CompositeExpr<")
	  +Name<T_type>::getName()+string(",")
	  +Name<Expr>::getName()+string(">");
      }
    };

    template<typename A, typename B,typename Op>
    struct TFEL_VISIBILITY_LOCAL Name<tfel::math::CompositeCompositeExpr<A,B,Op> >
    {
      static std::string getName(void){
      	using namespace std;
      	using namespace tfel::utilities;
      	return string("CompositeCompositeExpr<")+
	  Name<A>::getName()+string(",")+
	  Name<B>::getName()+string(",")+
	  Name<Op>::getName()+string(">");
      }
    }; // end of struct TFEL_VISIBILITY_LOCAL Name<tfel::math::CompositeCompositeExpr<A,B,Op> >

    template<typename A,typename Func>
    struct TFEL_VISIBILITY_LOCAL Name<tfel::math::FctCompositeExpr<A,Func> >
    {
      static std::string 
      getName(void){ 
	using namespace std;
	using namespace tfel::utilities;
	return string("FctCompositeExpr<")
	  +Name<A>::getName()+string(",")
	  +Name<Func>::getName()+string(">");        
      }
    };

    template<typename F>
    struct TFEL_VISIBILITY_LOCAL Name<tfel::math::CompositeNegExpr<F> >
    {
      static std::string 
      getName(void){
	using namespace std;
	using namespace tfel::utilities;
	return string("CompositeNegExpr<")+Name<F>::getName()+string(">");
      }
    };


      // static std::string 
      // getName(void){
      // 	using namespace std;
      // 	using namespace tfel::utilities;
      // 	return string("StensorConcept<")+Name<T>::getName()+string(">");
      // }

      // static std::string getName(void){
      // 	using namespace std;
      // 	using namespace tfel::utilities;
      // 	return string("StensorExpr<")+Name<T_type>::getName()
      // 	  +string(",")+Name<Expr>::getName()+string(">");
      // }

      // static std::string getName(void){
      // 	using namespace std;
      // 	using namespace tfel::utilities;
      // 	return string("ScalarStensorExpr<")+Name<A>::getName()+string(",")
      // 	  +Name<B>::getName()+string(",")+Name<Op>::getName()+string(">");
      // }


      // static std::string getName(void){
      // 	using namespace std;
      // 	using namespace tfel::utilities;
      // 	return string("StensorScalarExpr<")+Name<A>::getName()+string(",")
      // 	  +Name<B>::getName()+string(",")+Name<Op>::getName()+string(">");
      // }


      // static std::string getName(void){
      // 	using namespace std;
      // 	using namespace tfel::utilities;
      // 	return string("StensorScalarExpr<")+Name<A>::getName()+string(",")
      // 	  +Name<B>::getName()+string(",")+Name<OpDiv>::getName()+string(">");
      // }

      // static std::string 
      // getName(void){ 
      // 	using namespace std;
      // 	using namespace tfel::utilities;
      // 	return string("FctMatrixExpr<")
      // 	  +Name<A>::getName()
      // 	  +Name<Func>::getName()
      // 	  +string(">");        
      // }

      // static std::string getName(void){
      // 	using namespace std;
      // 	using namespace tfel::utilities;
      // 	return string("StensorStensorExpr<")+Name<A>::getName()+string(",")
      // 	  +Name<B>::getName()+string(",")+Name<Op>::getName()+string(">");
      // }

      // static std::string getName(void){
      // 	using namespace std;
      // 	using namespace tfel::utilities;
      // 	return string("StensorNegExpr<")+Name<A>::getName()+string(">");
      // }

      // static std::string getName(void){
      // 	using namespace std;
      // 	using namespace tfel::utilities;
      // 	return string("StensorStensorDiadicProductExpr<")+
      // 	  Name<A>::getName()+string(",")+
      // 	  Name<B>::getName()+string(",")+
      // 	  Name<OpDiadicProduct>::getName()+string(">");
      // }


      // static std::string getName(void){
      // 	using namespace std;
      // 	using namespace tfel::utilities;
      // 	return string("ScalarStensorExprWithoutConstIterator<")+Name<A>::getName()+string(",")
      // 	  +Name<B>::getName()+string(",")+Name<Op>::getName()+string(">");
      // }

      // static std::string getName(void){
      // 	using namespace std;
      // 	using namespace tfel::utilities;
      // 	return string("StensorScalarExprWithoutConstIterator<")+Name<A>::getName()+string(",")
      // 	  +Name<B>::getName()+string(",")+Name<Op>::getName()+string(">");
      // }

      // static std::string getName(void){
      // 	using namespace std;
      // 	using namespace tfel::utilities;
      // 	return string("StensorStensorExprWithoutConstIterator<")+Name<A>::getName()+string(",")
      // 	  +Name<B>::getName()+string(",")+Name<Op>::getName()+string(">");
      // }

      // static std::string getName(void){
      // 	using namespace std;
      // 	using namespace tfel::utilities;
      // 	return string("StensorStensorExprWithoutConstIterator<")+Name<A>::getName()+string(",")
      // 	  +Name<B>::getName()+string(",")+Name<OpMinus>::getName()+string(">");
      // }

      // static std::string getName(void){
      // 	using namespace std;
      // 	using namespace tfel::utilities;
      // 	return string("StensorNegExprWithoutConstIterator<")+Name<A>::getName()+string(">");
      // }

      // static std::string getName(void){
      // 	using namespace std;
      // 	using namespace tfel::utilities;
      // 	return string("ST2toST2Expr<")+Name<T_type>::getName()
      // 	  +string(",")+Name<Expr>::getName()+string(">");
      // }
    // 
    // template<class T>
    // std::string
    // ST2toST2Concept<T>::getName(void)
    // {
    //   using namespace std;
    //   using namespace tfel::utilities;
    //   return string("ST2toST2Concept<")+Name<T>::getName()+string(">");
    // } // end of ST2toST2Concept<T>::getName(void)

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
      struct TFEL_VISIBILITY_EXPORT TLGetNames_<TLE>
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

