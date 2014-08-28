/*!
 * \file   include/TFEL/Math/General/BasicOperations.hxx
 * \author Helfer Thomas
 */

#ifndef _TFEL_MATH_BASICOPERATIONS_LIB_
#define _TFEL_MATH_BASICOPERATIONS_LIB_ 1

#include <string>


#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/Metaprogramming/EnableIf.hxx"
#include"TFEL/TypeTraits/IsScalar.hxx"
#include"TFEL/TypeTraits/IsComplex.hxx"
#include"TFEL/TypeTraits/IsUnaryOperator.hxx"
#include"TFEL/TypeTraits/Promote.hxx"
#include"TFEL/Math/General/ComputeBinaryResult.hxx"
#include"TFEL/Math/General/ComputeUnaryResult.hxx"

#define TFEL_MATH_RESULT_TYPE_(X,Y)                                   \
    /*!                                                               \
     * \brief Partial specialisation for X and Y                      \
     */                                                               \
    template<>                                                        \
    struct ResultType<X,Y,OpPlus>{                                    \
      typedef tfel::typetraits::Promote< X , Y >::type   type;        \
    };                                                                \
    /*!                                                               \
     * \brief Partial specialisation for X and Y                      \
     */                                                               \
    template<>                                                        \
    struct ResultType<X,Y,OpMinus>{                                   \
      typedef tfel::typetraits::Promote< X , Y >::type type;          \
    };                                                                \
                                                                      \
    /*!                                                               \
     * \brief Partial specialisation for X and Y                      \
     */                                                               \
    template<>                                                        \
    struct ResultType<X,Y,OpMult>{                                    \
      typedef tfel::typetraits::Promote< X , Y >::type type;          \
    };                                                                \
                                                                      \
    /*!                                                               \
     * \brief Partial specialisation for X and Y                      \
     */                                                               \
    template<>                                                        \
    struct ResultType<X,Y,OpDiv>{                                     \
      typedef tfel::typetraits::Promote< X , Y >::type type;          \
    }

#define TFEL_MATH_RESULT_TYPE(X)                                 \
    TFEL_MATH_RESULT_TYPE_(X,unsigned short);                    \
    TFEL_MATH_RESULT_TYPE_(X,unsigned int);                      \
    TFEL_MATH_RESULT_TYPE_(X,long unsigned int);                 \
    TFEL_MATH_RESULT_TYPE_(X,short);                             \
    TFEL_MATH_RESULT_TYPE_(X,int);                               \
    TFEL_MATH_RESULT_TYPE_(X,long int);                          \
    TFEL_MATH_RESULT_TYPE_(X,float);                             \
    TFEL_MATH_RESULT_TYPE_(X,double);                            \
    TFEL_MATH_RESULT_TYPE_(X,long double);                       \
    /*!                                                          \
     * \brief Partial specialisation for X                       \
     * \see   UnaryResultType                                    \
     */                                                          \
    template<>                                                   \
    struct UnaryResultType<X,OpNeg>{                             \
      typedef tfel::typetraits::Promote<X,short>::type type;     \
    };                                                           \
    /*!                                                          \
     * \brief Partial specialisation for X                       \
     * \see   UnaryResultType                                    \
     */                                                          \
    template<>                                                   \
    struct UnaryResultType<Complex<X>,OpNeg>{               \
      typedef tfel::typetraits::Promote<Complex<X>,Complex<short> >::type type;     \
    }                                                      

#define TFEL_MATH_RESULT_TYPE_COMPLEX_(X,Y)                           \
    template<>                                                        \
    struct ResultType<X,Y,OpPlus>{                                    \
      typedef tfel::typetraits::Promote<X,Y>::type   type;            \
    };                                                                \
                                                                      \
    template<>                                                        \
    struct ResultType<X,Y,OpMinus>{                                   \
      typedef tfel::typetraits::Promote<X,Y>::type   type;            \
    };                                                                \
                                                                      \
    template<>                                                        \
    struct ResultType<X,Y,OpMult>{                                    \
      typedef tfel::typetraits::Promote<X,Y>::type type;              \
    };                                                                \
                                                                      \
    template<>                                                        \
    struct ResultType<X,Y,OpDiv>{                                     \
      typedef tfel::typetraits::Promote<X,Y>::type type;              \
    }

#define TFEL_MATH_RESULT_TYPE_COMPLEX(X)                              \
    TFEL_MATH_RESULT_TYPE_(X,Complex<X>);                             \
    TFEL_MATH_RESULT_TYPE_(Complex<X>,X);                             \
    TFEL_MATH_RESULT_TYPE_(Complex<X>,Complex<X>)      

namespace tfel{
  
  namespace math {

    template<typename ValueType>
    struct Complex;

    struct TFELMATH_VISIBILITY_EXPORT OpPlus {

      /*!
       * \brief  Return the name of the class.
       * \param  void.
       * \return std::string, the name of the class.
       * \see    Name.
       */
      static std::string getName(void);

      /*
       * Add its two arguments.
       * \see ComputeBinaryResult.
       */
      template<typename T1, typename T2>
      static TFEL_MATH_INLINE 
      const typename ComputeBinaryResult<T1,T2,OpPlus>::Handle
      apply(const T1& a, const T2& b) {
	return a+b;
      }

    };
    
    struct TFELMATH_VISIBILITY_EXPORT OpMinus {

      /*!
       * \brief  Return the name of the class.
       * \param  void.
       * \return std::string, the name of the class.
       * \see    Name.
       */
      static std::string getName(void);

      /*
       * Substract its two arguments.
       * \see ComputeBinaryResult.
       */
      template<typename T1, typename T2>
      static TFEL_MATH_INLINE 
      const typename ComputeBinaryResult<T1,T2,OpMinus>::Handle
      apply(const T1& a, const T2& b) {
	return a-b;
      }

    }; // end of OpMinus

    struct TFELMATH_VISIBILITY_EXPORT OpMult {

      /*!
       * \brief  Return the name of the class.
       * \param  void.
       * \return std::string, the name of the class.
       * \see    Name.
       */
      static std::string getName(void);

      /*
       * Multiply its two arguments.
       * \see ComputeBinaryResult
       */
      template<typename T1, typename T2>
      static TFEL_MATH_INLINE 
      const typename ComputeBinaryResult<T1,T2,OpMult>::Handle
      apply(const T1& a, const T2& b) {
	return a*b;
      }

    }; // end of OpMult

    struct TFELMATH_VISIBILITY_EXPORT OpDiv {

      /*!
       * \brief  Return the name of the class.
       * \param  void.
       * \return std::string, the name of the class.
       * \see    Name.
       */
      static std::string getName(void);
      
      /*
       * Divide the two argument
       * \see ComputeBinaryResult.
       */
      template<typename T1, typename T2>
      static TFEL_MATH_INLINE 
      const typename ComputeBinaryResult<T1,T2,OpDiv>::Handle
      apply(const T1& a, const T2& b) {
	return a/b;
      }

    }; // end of OpDiv

    struct TFELMATH_VISIBILITY_EXPORT OpNeg {
	
      /*!
       * \brief  Return the name of the class.
       * \param  void.
       * \return std::string, the name of the class.
       * \see    Name.
       */
      static std::string getName(void);

      template<typename T1>
      static TFEL_MATH_INLINE 
      const typename ComputeUnaryResult<T1,OpNeg>::Handle
      apply(const T1& a) {
	return -a;
      }

    };

    struct TFELMATH_VISIBILITY_EXPORT OpDotProduct
    {
      /*!
       * \brief  Return the name of the class.
       * \param  void.
       * \return std::string, the name of the class.
       * \see    Name.
       */
      static std::string getName(void);
    }; // end of struct OpDotProduct

    struct TFELMATH_VISIBILITY_EXPORT OpDiadicProduct
    {
      /*!
       * \brief  Return the name of the class.
       * \param  void.
       * \return std::string, the name of the class.
       * \see    Name.
       */
      static std::string getName(void);
    };

  } // end of namespace math

  namespace typetraits{

    /*
     * Partial Specialisation for OpNeg
     */
    template<>
    struct IsUnaryOperator<tfel::math::OpNeg>
    {
      static const bool cond = true;
    };

  } // end of namespace typetraits

  namespace math{

    TFEL_MATH_RESULT_TYPE(unsigned short);
    TFEL_MATH_RESULT_TYPE(unsigned int);
    TFEL_MATH_RESULT_TYPE(long unsigned int);
    TFEL_MATH_RESULT_TYPE(short);
    TFEL_MATH_RESULT_TYPE(int);
    TFEL_MATH_RESULT_TYPE(long int);
    TFEL_MATH_RESULT_TYPE(float);
    TFEL_MATH_RESULT_TYPE(double);
    TFEL_MATH_RESULT_TYPE(long double);
    
    template<typename T1,typename T2>
    TFEL_MATH_INLINE typename tfel::meta::EnableIf<
      tfel::typetraits::IsScalar<T1>::cond&&
    (!tfel::typetraits::IsComplex<T1>::cond)&&
    tfel::typetraits::IsScalar<T2>::cond&&
    (!tfel::typetraits::IsComplex<T2>::cond)&&
    (!tfel::meta::IsSameType<T1,T2>::cond),
      Complex<typename tfel::math::ResultType<T1,T2,tfel::math::OpPlus>::type> >::type
    operator + (const T1 a,const Complex<T2>&b)
    {
      typedef Complex<typename tfel::math::ResultType<T1,T2,tfel::math::OpPlus>::type> ResultType;
      return ResultType(a+b.real(),b.imag());
    }

    template<typename T1,typename T2>
    TFEL_MATH_INLINE typename tfel::meta::EnableIf<
      tfel::typetraits::IsScalar<T1>::cond&&
    (!tfel::typetraits::IsComplex<T1>::cond)&&
    tfel::typetraits::IsScalar<T2>::cond&&
    (!tfel::typetraits::IsComplex<T2>::cond)&&
    (!tfel::meta::IsSameType<T1,T2>::cond),
      Complex<typename tfel::math::ResultType<T1,T2,tfel::math::OpPlus>::type> >::type
    operator + (const Complex<T1>& a,const T2 b)
    {
      typedef Complex<typename tfel::math::ResultType<T1,T2,tfel::math::OpPlus>::type> ResultType;
      return ResultType(a.real()+b,a.imag());
    }

    template<typename T1,typename T2>
    TFEL_MATH_INLINE typename tfel::meta::EnableIf<
      tfel::typetraits::IsScalar<T1>::cond&&
    (!tfel::typetraits::IsComplex<T1>::cond)&&
    tfel::typetraits::IsScalar<T2>::cond&&
    (!tfel::typetraits::IsComplex<T2>::cond)&&
    (!tfel::meta::IsSameType<T1,T2>::cond),
      Complex<typename tfel::math::ResultType<T1,T2,tfel::math::OpPlus>::type> >::type
    operator + (const Complex<T1>& a,const Complex<T2>&b)
    {
      typedef Complex<typename tfel::math::ResultType<T1,T2,tfel::math::OpPlus>::type> ResultType;
      return ResultType(a.real()+b.real(),a.real()+b.imag());
    }

    template<typename T1,typename T2>
    TFEL_MATH_INLINE typename tfel::meta::EnableIf<
      tfel::typetraits::IsScalar<T1>::cond&&
    (!tfel::typetraits::IsComplex<T1>::cond)&&
    tfel::typetraits::IsScalar<T2>::cond&&
    (!tfel::typetraits::IsComplex<T2>::cond)&&
    (!tfel::meta::IsSameType<T1,T2>::cond),
      Complex<typename tfel::math::ResultType<T1,T2,tfel::math::OpMinus>::type> >::type
    operator - (const T1 a,const Complex<T2>&b)
    {
      typedef Complex<typename tfel::math::ResultType<T1,T2,tfel::math::OpMinus>::type> ResultType;
      return ResultType(a-b.real(),b.imag());
    }

    template<typename T1,typename T2>
    TFEL_MATH_INLINE typename tfel::meta::EnableIf<
      tfel::typetraits::IsScalar<T1>::cond&&
    (!tfel::typetraits::IsComplex<T1>::cond)&&
    tfel::typetraits::IsScalar<T2>::cond&&
    (!tfel::typetraits::IsComplex<T2>::cond)&&
    (!tfel::meta::IsSameType<T1,T2>::cond),
      Complex<typename tfel::math::ResultType<T1,T2,tfel::math::OpMinus>::type> >::type
    operator - (const Complex<T1>& a,const T2 b)
    {
      typedef Complex<typename tfel::math::ResultType<T1,T2,tfel::math::OpMinus>::type> ResultType;
      return ResultType(a.real()-b,a.imag());
    }

    template<typename T1,typename T2>
    TFEL_MATH_INLINE typename tfel::meta::EnableIf<
      tfel::typetraits::IsScalar<T1>::cond&&
    (!tfel::typetraits::IsComplex<T1>::cond)&&
    tfel::typetraits::IsScalar<T2>::cond&&
    (!tfel::typetraits::IsComplex<T2>::cond)&&
    (!tfel::meta::IsSameType<T1,T2>::cond),
      Complex<typename tfel::math::ResultType<T1,T2,tfel::math::OpMinus>::type> >::type
    operator - (const Complex<T1>& a,const Complex<T2>&b)
    {
      typedef Complex<typename tfel::math::ResultType<T1,T2,tfel::math::OpMinus>::type> ResultType;
      return ResultType(a.real()-b.real(),a.real()-b.imag());
    }

    template<typename T1,typename T2>
    TFEL_MATH_INLINE typename tfel::meta::EnableIf<
      tfel::typetraits::IsScalar<T1>::cond&&
    (!tfel::typetraits::IsComplex<T1>::cond)&&
    tfel::typetraits::IsScalar<T2>::cond&&
    (!tfel::typetraits::IsComplex<T2>::cond)&&
    (!tfel::meta::IsSameType<T1,T2>::cond),
      Complex<typename tfel::math::ResultType<T1,T2,tfel::math::OpMult>::type> >::type
    operator * (const T1 a,const Complex<T2>&b)
    {
      typedef Complex<typename tfel::math::ResultType<T1,T2,tfel::math::OpMult>::type> ResultType;
      return ResultType(a*(b.real()),a*(b.imag()));
    }

    template<typename T1,typename T2>
    TFEL_MATH_INLINE typename tfel::meta::EnableIf<
      tfel::typetraits::IsScalar<T1>::cond&&
    (!tfel::typetraits::IsComplex<T1>::cond)&&
    tfel::typetraits::IsScalar<T2>::cond&&
    (!tfel::typetraits::IsComplex<T2>::cond)&&
    (!tfel::meta::IsSameType<T1,T2>::cond),
      Complex<typename tfel::math::ResultType<T1,T2,tfel::math::OpMult>::type> >::type
    operator * (const Complex<T1>& a,const T2 b)
    {
      typedef Complex<typename tfel::math::ResultType<T1,T2,tfel::math::OpMult>::type> ResultType;
      return ResultType((a.real())*b,(a.imag())*b);
    }

    template<typename T1,typename T2>
    TFEL_MATH_INLINE typename tfel::meta::EnableIf<
      tfel::typetraits::IsScalar<T1>::cond&&
    (!tfel::typetraits::IsComplex<T1>::cond)&&
    tfel::typetraits::IsScalar<T2>::cond&&
    (!tfel::typetraits::IsComplex<T2>::cond)&&
    (!tfel::meta::IsSameType<T1,T2>::cond),
      Complex<typename tfel::math::ResultType<T1,T2,tfel::math::OpMinus>::type> >::type
    operator * (const Complex<T1>& a,const Complex<T2>&b)
    {
      typedef Complex<typename tfel::math::ResultType<T1,T2,tfel::math::OpMinus>::type> ResultType;
      return ResultType((a.real())*(b.real())-(a.imag())*(b.imag()),(a.real())*(b.imag())+(a.imag())*(b.real()));
    }

    template<typename T1,typename T2>
    TFEL_MATH_INLINE typename tfel::meta::EnableIf<
      tfel::typetraits::IsScalar<T1>::cond&&
    (!tfel::typetraits::IsComplex<T1>::cond)&&
    tfel::typetraits::IsScalar<T2>::cond&&
    (!tfel::typetraits::IsComplex<T2>::cond)&&
    (!tfel::meta::IsSameType<T1,T2>::cond),
      Complex<typename tfel::math::ResultType<T1,T2,tfel::math::OpDiv>::type> >::type
    operator / (const T1 a,const Complex<T2>&b)
    {
      typedef Complex<typename tfel::math::ResultType<T1,T2,tfel::math::OpDiv>::type> ResultType;
      return ResultType((a*(b.real()))/((b.real())*(b.real())+(b.imag())*(b.imag())),(-a*(b.imag()))/((b.real())*(b.real())+(b.imag())*(b.imag())));
    }

    template<typename T1,typename T2>
    TFEL_MATH_INLINE typename tfel::meta::EnableIf<
      tfel::typetraits::IsScalar<T1>::cond&&
    (!tfel::typetraits::IsComplex<T1>::cond)&&
    tfel::typetraits::IsScalar<T2>::cond&&
    (!tfel::typetraits::IsComplex<T2>::cond)&&
    (!tfel::meta::IsSameType<T1,T2>::cond),
      Complex<typename tfel::math::ResultType<T1,T2,tfel::math::OpDiv>::type> >::type
    operator / (const Complex<T1>& a,const T2 b)
    {
      typedef Complex<typename tfel::math::ResultType<T1,T2,tfel::math::OpDiv>::type> ResultType;
      return ResultType((a.real())/b,(a.imag())/b);
    }

    template<typename T1,typename T2>
    TFEL_MATH_INLINE typename tfel::meta::EnableIf<
      tfel::typetraits::IsScalar<T1>::cond&&
    (!tfel::typetraits::IsComplex<T1>::cond)&&
    tfel::typetraits::IsScalar<T2>::cond&&
    (!tfel::typetraits::IsComplex<T2>::cond)&&
    (!tfel::meta::IsSameType<T1,T2>::cond),
      Complex<typename tfel::math::ResultType<T1,T2,tfel::math::OpDiv>::type> >::type
    operator / (const Complex<T1>& a,const Complex<T2>&b)
    {
      typedef Complex<typename tfel::math::ResultType<T1,T2,tfel::math::OpDiv>::type> ResultType;
      return ResultType(((a.real())*(b.real())+(a.imag())*(b.imag()))/((b.real())*(b.real())+(b.imag())*(b.imag())),(-(a.real())*(b.imag())+(a.imag())*(b.real()))/((b.real())*(b.real())+(b.imag())*(b.imag())));
    }

    template<typename T>
    TFEL_MATH_INLINE
    typename tfel::meta::EnableIf<
      tfel::typetraits::IsScalar<T>::cond&&
      (!tfel::typetraits::IsComplex<T>::cond),
      T>::type 
    conj(const T src)
    {
      return src;
    }

    template<typename T>
    TFEL_MATH_INLINE typename tfel::meta::EnableIf<
      tfel::typetraits::IsScalar<T>::cond&&
      (!tfel::typetraits::IsComplex<T>::cond),
      T>::type 
    real(const T src)
    {
      return src;
    }
  
    TFEL_MATH_RESULT_TYPE_COMPLEX(unsigned short);
    TFEL_MATH_RESULT_TYPE_COMPLEX(unsigned int);
    TFEL_MATH_RESULT_TYPE_COMPLEX(long unsigned int);
    TFEL_MATH_RESULT_TYPE_COMPLEX(short);
    TFEL_MATH_RESULT_TYPE_COMPLEX(int);
    TFEL_MATH_RESULT_TYPE_COMPLEX(long int);
    TFEL_MATH_RESULT_TYPE_COMPLEX(float);
    TFEL_MATH_RESULT_TYPE_COMPLEX(double);
    TFEL_MATH_RESULT_TYPE_COMPLEX(long double);
  
  } // end of namespace math

} // end of namespace tfel

#endif /* _TFEL_MATH_BASICOPERATIONS_LIB_ */
