/*!
 *\file   qt.hxx
 *\brief  This file declares the qt class.
 *\author Helfer Thomas
 *\date   06 Jun 2006
 */

#ifndef _LIB_TFEL_QT_H_
#define _LIB_TFEL_QT_H_ 

#include<cmath>
#include<string>
#include<ostream>

#include "Config/TFELConfig.hxx"

#include "Utilities/Name.hxx"

#include"Metaprogramming/StaticAssert.hxx"
#include"Metaprogramming/TypeList.hxx"
#include"Metaprogramming/IntToType.hxx"

#include"TypeTraits/IsScalar.hxx"
#include"TypeTraits/IsReal.hxx"
#include"TypeTraits/IsComplex.hxx"
#include"TypeTraits/IsFundamentalNumericType.hxx"
#include"TypeTraits/IsSafelyReinterpretCastableTo.hxx"
#include"TypeTraits/IsAssignableTo.hxx"
#include"TypeTraits/Promote.hxx"
#include"TypeTraits/BaseType.hxx"

#include"Math/General/BasicOperations.hxx"

#include"Math/Quantity/Unit.hxx"

namespace tfel{

  namespace math{

    /*!
     * \class qt
     * \brief This class describes numbers with unit.
     * qt must be has efficient and simple to use that standard numerical types.
     * \param unit, the unit of the qt.
     * \param T, the underlying numerical type.
     * \pre T must be a fundamental numerical type.
     * \see unit_samples.hxx, IsFundamentalNumericType
     * \author Helfer Thomas
     * \date   06 Jun 2006
     */
    template<typename unit,typename T=double>
    class qt{
      
      /*!
       * A simple check, T must be a fundamental numerical type.
       */
      TFEL_STATIC_ASSERT(tfel::typetraits::IsFundamentalNumericType<T>::cond);
      /*!
       * A simple check, T must be a scalar.
       */
      TFEL_STATIC_ASSERT(tfel::typetraits::IsScalar<T>::cond);
      /*!
       * The value of the qt
       */
      T value;

    public :

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
	return string("qt<")+Name<unit>::getName()+string(",")+Name<T>::getName()+string(">");
      }

      /*!
       * A simple typedef to indicate the numerical type used
       */
      typedef T value_type;

      /*!
       * \brief Default constructor
       * value is left uninitialised.
       */
      TFEL_MATH_INLINE qt()
      {}
      
      /*!
       * \brief Copy constructor.
       * \param qt src.
       * value takes the src's value.
       */
      TFEL_MATH_INLINE qt(const qt<unit,T>& src)
	: value(src.value)
      {}

      /*!
       * \brief constructor from a value
       * \param T src, the src.
       */
      TFEL_MATH_INLINE explicit qt(const T src)
	: value(src)
      {}

      /*!
       * Return the value of the qt.
       */
      TFEL_MATH_INLINE T& getValue(void);

      /*!
       * Return the value of the qt, const version
       */
      TFEL_MATH_INLINE const T  getValue(void) const;
      
      /*!
       * Assignement operator
       */
      template<typename T2>
      TFEL_MATH_INLINE typename tfel::meta::EnableIf<
	tfel::meta::IsSameType<typename tfel::typetraits::Promote<T,T2>::type,T>::cond,
	qt<unit,T>&>::type
      operator = (const qt<unit,T2>&src);

      /*!
       * Operator +=
       */
      template<typename T2>
      TFEL_MATH_INLINE typename tfel::meta::EnableIf<
	tfel::meta::IsSameType<typename tfel::typetraits::Promote<T,T2>::type,T>::cond,
	qt<unit,T>&>::type
      operator += (const qt<unit,T2>&src);

      /*!
       * Operator -=
       */
      template<typename T2>
      TFEL_MATH_INLINE typename tfel::meta::EnableIf<
	tfel::meta::IsSameType<typename tfel::typetraits::Promote<T,T2>::type,T>::cond,
	qt<unit,T>&>::type
      operator -= (const qt<unit,T2>&src);

      /*!
       * Operator *=
       * \param T2 a, a scalar
       */
      template<typename T2>
      TFEL_MATH_INLINE typename tfel::meta::EnableIf<
	tfel::typetraits::IsFundamentalNumericType<T2>::cond&&
        tfel::typetraits::IsScalar<T>::cond&&
        tfel::meta::IsSameType<typename tfel::typetraits::Promote<T,T2>::type,T>::cond,
	qt<unit,T>&
      >::type
      operator *= (const T2 a);
      
      /*!
       * Operator /=
       * \param T2 a, a scalar
       */
      template<typename T2>
      TFEL_MATH_INLINE typename tfel::meta::EnableIf<
	tfel::typetraits::IsFundamentalNumericType<T2>::cond&&
        tfel::typetraits::IsScalar<T>::cond&&
        tfel::meta::IsSameType<typename tfel::typetraits::Promote<T,T2>::type,T>::cond,
	qt<unit,T>&
      >::type
      operator /= (const T2 a);

      /*!
       * negation operator
       */
      qt<unit,T> operator -() const
      {
	return qt<unit,T>(-(this->value));
      }

    };

    /*!
     * \brief Specialization in case NoUnit.
     * This class shall be implicitly convertible to T and T must be convertible to qt<T,nounit>.
     * \see qt.
     */ 
    template<typename T>
    class qt<NoUnit,T>{
      
      /*!
       * A simple check, T must be a fundamental numerical type.
       */
      TFEL_STATIC_ASSERT(tfel::typetraits::IsFundamentalNumericType<T>::cond);
      /*!
       * A simple check, T must be a scalar.
       */
      TFEL_STATIC_ASSERT(tfel::typetraits::IsScalar<T>::cond);
      /*!
       * The value of the qt
       */
      T value;

    public :

      /*!
       * \brief  Return the name of the class.
       * \param  void.
       * \return std::string, the name of the class.
       * \see    Name.
       */
      static 
      const std::string 
      getName(void){
	using namespace std;
	using namespace tfel::utilities;
	return string("qt<")+Name<NoUnit>::getName()+string(",")+Name<T>::getName()+string(">");
      }

      /*!
       * A simple typedef to indicate the numerical type used
       */
      typedef T value_type;

      /*!
       * \brief Default constructor
       * value is left uninitialised.
       */
      TFEL_MATH_INLINE qt()
      {}

      /*!
       * \brief Copy constructor.
       * \param qt src.
       * value takes the src's value.
       */
      TFEL_MATH_INLINE qt(const qt<NoUnit,T>& src)
	: value(src.value)
      {}

      /*!
       * \brief constructor from a value
       * Here, it is not declared explicit.
       * \param T src, the src.
       */
      TFEL_MATH_INLINE qt(const T& src)
	: value(src)
      {}

      /*!
       * Return the value of the qt
       */
      TFEL_MATH_INLINE T& getValue(void);

      /*!
       * Return the value of the qt, const version
       */
      TFEL_MATH_INLINE const T getValue(void) const;
      
      /*!
       * Operator =
       */
      template<typename T2>
      TFEL_MATH_INLINE typename tfel::meta::EnableIf<
	tfel::meta::IsSameType<typename tfel::typetraits::Promote<T,T2>::type,T>::cond,
	qt<NoUnit,T>&>::type
      operator = (const qt<NoUnit,T2>&src);

      template<typename T2>
      TFEL_MATH_INLINE typename tfel::meta::EnableIf<
	tfel::meta::IsSameType<typename tfel::typetraits::Promote<T,T2>::type,T>::cond&&
        tfel::typetraits::IsFundamentalNumericType<T2>::cond&&
        tfel::typetraits::IsScalar<T2>::cond,
        qt<NoUnit,T>&
       >::type
      operator = (const T2 src);

      /*!
       * Operator +=
       */
      template<typename T2>
      TFEL_MATH_INLINE typename tfel::meta::EnableIf<
	tfel::meta::IsSameType<typename tfel::typetraits::Promote<T,T2>::type,T>::cond,
	qt<NoUnit,T>&>::type
      operator += (const qt<NoUnit,T2>&src);

      /*!
       * Operator -=
       */
      template<typename T2>
      TFEL_MATH_INLINE typename tfel::meta::EnableIf<
	tfel::meta::IsSameType<typename tfel::typetraits::Promote<T,T2>::type,T>::cond,
	qt<NoUnit,T>&>::type
      operator -= (const qt<NoUnit,T2>&src);

      /*!
       * Operator *=
       * \param T2 a, a scalar
       */
      template<typename T2>
      TFEL_MATH_INLINE typename tfel::meta::EnableIf<
	tfel::typetraits::IsFundamentalNumericType<T2>::cond&&
        tfel::typetraits::IsScalar<T>::cond&&
        tfel::meta::IsSameType<typename tfel::typetraits::Promote<T,T2>::type,T>::cond,
	qt<NoUnit,T>&
      >::type
      operator *= (const T2 a);
      
      /*!
       * Operator /=
       * \param T2 a, a scalar
       */
      template<typename T2>
      TFEL_MATH_INLINE typename tfel::meta::EnableIf<
	tfel::typetraits::IsFundamentalNumericType<T2>::cond&&
        tfel::typetraits::IsScalar<T>::cond&&
        tfel::meta::IsSameType<typename tfel::typetraits::Promote<T,T2>::type,T>::cond,
	qt<NoUnit,T>&
      >::type
      operator /= (const T2 a);

      /*!
       * Conversion operator
       */
      operator T() const;

      /*!
       * negation operator
       */
      qt<NoUnit,T> operator -() const
      {
	return qt<NoUnit,T>(-(this->value));
      }

    };
    
    template<typename unit,typename T>
    TFEL_MATH_INLINE2 std::ostream& operator << (std::ostream&, const qt<unit,T>&);

    template<typename unit,typename T>
    TFEL_MATH_INLINE bool operator < (const qt<unit,T>, const qt<unit,T>);

    template<typename unit,typename T>
    TFEL_MATH_INLINE bool operator <= (const qt<unit,T>, const qt<unit,T>);

    template<typename unit,typename T>
    TFEL_MATH_INLINE bool operator > (const qt<unit,T>, const qt<unit,T>);

    template<typename unit,typename T>
    TFEL_MATH_INLINE bool operator >= (const qt<unit,T>, const qt<unit,T>);

    template<typename unit,typename T>
    TFEL_MATH_INLINE bool operator == (const qt<unit,T>, const qt<unit,T>);

    template<typename unit,typename T>
    TFEL_MATH_INLINE bool operator != (const qt<unit,T>, const qt<unit,T>);

  } // end of Namesapce math

} // end of namespace tfel

namespace std{

  template<typename unit,typename T>
  TFEL_MATH_INLINE tfel::math::qt<unit,T> abs(const tfel::math::qt<unit,T>);

}// end of namespace std

#include "Math/Quantity/qtLimits.hxx"
#include "Math/Quantity/qtSpecific.hxx"
#include "Math/Quantity/qtSamples.hxx"
#include "Math/Quantity/qt.ixx"
#include "Math/Quantity/qtOperations.hxx"

#endif /* _LIB_TFEL_QT_H */

