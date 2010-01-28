/*!
 * \file   st2tost2.hxx  
 * \brief    
 * \author Helfer Thomas
 * \date   04 May 2006
 */

#ifndef _LIB_TFEL_ST2TOST2_H_
#define _LIB_TFEL_ST2TOST2_H_ 

#include<string>
#include<cassert>

#include"TFEL/Config/TFELConfig.hxx"

#include"TFEL/Metaprogramming/StaticAssert.hxx"

#include"TFEL/Utilities/Name.hxx"
#include"TFEL/Utilities/ToString.hxx"

#include"TFEL/TypeTraits/IsScalar.hxx"
#include"TFEL/TypeTraits/BaseType.hxx"
#include"TFEL/TypeTraits/IsInvalid.hxx"
#include"TFEL/TypeTraits/IsAssignableTo.hxx"

#include"TFEL/Math/General/BasicOperations.hxx"

#include"TFEL/Math/Stensor/StensorSizeToDime.hxx"
#include"TFEL/Math/ST2toST2/ST2toST2Concept.hxx"
#include"TFEL/Math/ST2toST2/ST2toST2ConceptOperations.hxx"
#include"TFEL/Math/ST2toST2/ST2toST2Expr.hxx"

namespace tfel{
  
  namespace math {

    /*!
     * \class st2tost2
     * \brief finite size linear function on symmetric tensor.
     * \param N, the spatial dimension, see StensorDimeToSize for details. 
     * \param T, numerical type used, by default, double
     * \pre   This class is only defined for N=1u,2u and 3u.
     * \see   StensorDimeToSize and StensorSizeToDime.
     */
    template<unsigned short N,typename T=double>
    class st2tost2;

    /*
     * Partial specialisation for st2tost2
     */
    template<unsigned short N,typename T>
    struct ST2toST2Traits<st2tost2<N,T> >{
      typedef T NumType;
      static const unsigned short dime = N;
    };

    template<unsigned short N,typename T>
    struct st2tost2
      : public ST2toST2Concept<st2tost2<N,T> >
    {

      /*!
       * \brief  Return the name of the class.
       * \param  void.
       * \return const std::string, the name of the class.
       * \see    Name.
       */
      static
      const std::string
      getName(void){
	using namespace std;
	using namespace tfel::utilities;
	return string("st2tost2<")+ToString(N)+string(",")+Name<T>::getName()+string(">");
      }

      typedef T 		value_type;
      typedef value_type* 	pointer;
      typedef const value_type* const_pointer;
      typedef pointer           iterator;
      typedef const_pointer     const_iterator;
      typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
      typedef std::reverse_iterator<iterator>       reverse_iterator;
      typedef value_type& 	reference;
      typedef const value_type& const_reference;
      typedef size_t 		size_type;
      typedef ptrdiff_t 	difference_type;

      /*!
       * \brief Default Constructor 
       */
      explicit st2tost2()
      {};

      /*!
       * \brief Default Constructor 
       * \param T, value used to initialise the components of the st2tost2 
       */
      explicit st2tost2(const T);

      /*!
       * \brief Default Constructor.
       * \param const typename tfel::typetraits::BaseType<T>::type* const,
       *  pointer to a tabular used to initialise the components of
       *  the st2tost2. This tabular is left unchanged.
       */
      explicit st2tost2(const typename tfel::typetraits::BaseType<T>::type* const init);

      /*!
       * \brief Copy Constructor
       */
      st2tost2(const st2tost2<N,T>& src);

      // Copy Constructor
      template<typename T2,typename Expr>
      TFEL_MATH_INLINE 
      st2tost2(const ST2toST2Expr<st2tost2<N,T2>,Expr>& src);

      /*!
       * Import values
       */
      template<typename T2>
      TFEL_MATH_INLINE2 typename tfel::meta::EnableIf<
	tfel::typetraits::IsSafelyReinterpretCastableTo<T2,typename tfel::typetraits::BaseType<T>::type>::cond,
	void>::type
      import(const T2* const);

      /*!
       * Assignement operator
       */
      st2tost2<N,T>&
      operator=(const st2tost2<N,T>&);

      /*!
       * Assignement operator
       */
      template<typename T2>
      TFEL_MATH_INLINE typename tfel::meta::EnableIf<
	tfel::typetraits::IsAssignableTo<T2,T>::cond,
	st2tost2<N,T>&>::type
      operator=(const st2tost2<N,T2>&);

      /*!
       * Assignement operator
       */
      template<typename T2,typename Expr>
      TFEL_MATH_INLINE 
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsAssignableTo<T2,T>::cond,
	st2tost2<N,T>&
      >::type
      operator=(const ST2toST2Expr<st2tost2<N,T2>,Expr>&);

      // Assignement operator
      template<typename T2>
      TFEL_MATH_INLINE typename tfel::meta::EnableIf<
	tfel::typetraits::IsAssignableTo<T2,T>::cond,
	st2tost2<N,T>&
      >::type
      operator+=(const st2tost2<N,T2>&);
    
      // Assignement operator
      template<typename T2,typename Expr>
      TFEL_MATH_INLINE typename tfel::meta::EnableIf<
	tfel::typetraits::IsAssignableTo<T2,T>::cond,
	st2tost2<N,T>&
      >::type
      operator+=(const ST2toST2Expr<st2tost2<N,T2>,Expr>&);

      // Assignement operator
      template<typename T2>
      TFEL_MATH_INLINE typename tfel::meta::EnableIf<
	tfel::typetraits::IsAssignableTo<T2,T>::cond,
	st2tost2<N,T>&
      >::type
      operator-=(const st2tost2<N,T2>&);
    
      // Assignement operator
      template<typename T2,typename Expr>
      TFEL_MATH_INLINE typename tfel::meta::EnableIf<
	tfel::typetraits::IsAssignableTo<T2,T>::cond,
	st2tost2<N,T>&
      >::type
      operator-=(const ST2toST2Expr<st2tost2<N,T2>,Expr>&);

      /*!
       * operator*=
       */
      template<typename T2>
      TFEL_MATH_INLINE 
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsScalar<T2>::cond&&
      tfel::meta::IsSameType<typename ResultType<T,T2,OpMult>::type,T>::cond,
	st2tost2<N,T>&
      >::type
      operator*=(const T2);

      /*!
       * operator/=
       */
      template<typename T2>
      TFEL_MATH_INLINE 
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsScalar<T2>::cond&&
        tfel::meta::IsSameType<typename ResultType<T,T2,OpMult>::type,T>::cond,
	st2tost2<N,T>&
      >::type
      operator/=(const T2);

      TFEL_MATH_INLINE const T& operator()(const unsigned short,const unsigned short) const;      
      TFEL_MATH_INLINE       T& operator()(const unsigned short,const unsigned short);

      TFEL_MATH_INLINE2 iterator       begin(void);
      TFEL_MATH_INLINE2 const_iterator begin(void) const;
      TFEL_MATH_INLINE2 iterator       end(void);
      TFEL_MATH_INLINE2 const_iterator end(void) const;

      TFEL_MATH_INLINE2 reverse_iterator       rbegin(void);
      TFEL_MATH_INLINE2 const_reverse_iterator rbegin(void) const;
      TFEL_MATH_INLINE2 reverse_iterator       rend(void);
      TFEL_MATH_INLINE2 const_reverse_iterator rend(void) const;

      template<typename InputIterator>
      TFEL_MATH_INLINE2 void copy(const InputIterator src);

      static const st2tost2& Id(void);
      static const st2tost2& J(void);
      static const st2tost2& K(void);
      static const st2tost2& IxI(void);

    private:
            
      /*!
       * A simple check
       */
      TFEL_STATIC_ASSERT((N==1u)||(N==2u)||(N==3u));

      static const unsigned short StensorSize  = StensorDimeToSize<N>::value;
      static const unsigned short St2tost2Size = StensorSize*StensorSize;

      T v[St2tost2Size];

      enum ParticularSt2toSt2{
	ST2TOST2_IDENTITY,
	ST2TOST2_J,
	ST2TOST2_K,
	ST2TOST2_IxI
      }; // end of enum ParticularSt2toSt2

      st2tost2(const ParticularSt2toSt2);

    }; // end of struct st2tost2
        
  } // end of namespace math

  namespace typetraits{
    
    template<unsigned short N,typename T2,typename T>
    struct IsAssignableTo<tfel::math::st2tost2<N,T2>,
			  tfel::math::st2tost2<N,T> >
    {
      /*!
       *  Result
       */
      static const bool cond = IsAssignableTo<T2,T>::cond;
    };
    
  } // end of namespace typetraits

} // end of namespace tfel

#include"TFEL/Math/ST2toST2/st2tost2.ixx"
#include"TFEL/Math/ST2toST2/st2tost2ResultType.hxx"

#endif /* _LIB_TFEL_ST2TOST2_H */

