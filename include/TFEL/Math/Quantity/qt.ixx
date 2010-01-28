/*!
 * \file   qt.ixx
 * \brief  This file implements the methods defined in the qt class.
 * \author Helfer Thomas
 * \date   09 Jun 2006
 */

#ifndef _LIB_TFEL_QT_I_
#define _LIB_TFEL_QT_I_ 

namespace tfel{

  namespace math{

    template<typename unit,typename T>
    TFEL_MATH_INLINE T& qt<unit,T>::getValue(void){
      return this->value;
    }

    template<typename unit,typename T>
    TFEL_MATH_INLINE const T  qt<unit,T>::getValue(void) const{
      return value;
    }
    
    template<typename unit,typename T>
    template<typename T2>
    TFEL_MATH_INLINE typename tfel::meta::EnableIf<
      tfel::meta::IsSameType<typename tfel::typetraits::Promote<T,T2>::type,T>::cond,
      qt<unit,T>&>::type
    qt<unit,T>::operator = (const qt<unit,T2>&src){
      this->value = src.getValue();
      return *this;
    }

    template<typename unit,typename T>
    template<typename T2>
    TFEL_MATH_INLINE typename tfel::meta::EnableIf<
      tfel::meta::IsSameType<typename tfel::typetraits::Promote<T,T2>::type,T>::cond,
      qt<unit,T>&>::type
    qt<unit,T>::operator += (const qt<unit,T2>&src){
      this->value += src.getValue();
      return *this;
    }

    template<typename unit,typename T>
    template<typename T2>
    TFEL_MATH_INLINE typename tfel::meta::EnableIf<
      tfel::meta::IsSameType<typename tfel::typetraits::Promote<T,T2>::type,T>::cond,
      qt<unit,T>&>::type
    qt<unit,T>::operator -= (const qt<unit,T2>&src){
      this->value -= src.getValue();
      return *this;
    }

    template<typename unit,typename T>
    template<typename T2>
    TFEL_MATH_INLINE typename tfel::meta::EnableIf<
      tfel::typetraits::IsFundamentalNumericType<T2>::cond&&
      tfel::typetraits::IsScalar<T>::cond&&
      tfel::meta::IsSameType<typename tfel::typetraits::Promote<T,T2>::type,T>::cond,
      qt<unit,T>&
    >::type
    qt<unit,T>::operator *= (const T2 a){
      this->value *= a;
      return *this;
    }

    template<typename unit,typename T>
    template<typename T2>
    TFEL_MATH_INLINE typename tfel::meta::EnableIf<
      tfel::typetraits::IsFundamentalNumericType<T2>::cond&&
      tfel::typetraits::IsScalar<T>::cond&&
      tfel::meta::IsSameType<typename tfel::typetraits::Promote<T,T2>::type,T>::cond,
      qt<unit,T>&
    >::type
    qt<unit,T>::operator /= (const T2 a){
      this->value /= a;
      return *this;
    }

    template<typename T>
    TFEL_MATH_INLINE T& qt<NoUnit,T>::getValue(void){
      return value;
    }

    template<typename T>
    TFEL_MATH_INLINE const T qt<NoUnit,T>::getValue(void) const{
      return value;
    }
    
    template<typename T>
    template<typename T2>
    TFEL_MATH_INLINE typename tfel::meta::EnableIf<
      tfel::meta::IsSameType<
      typename tfel::typetraits::Promote<T,T2>::type,T>::cond,
      qt<NoUnit,T>&
    >::type
    qt<NoUnit,T>::operator = (const qt<NoUnit,T2>&src){
      this->value = src.getValue();
      return *this;
    }

    template<typename T>
    template<typename T2>
    TFEL_MATH_INLINE typename tfel::meta::EnableIf<
      tfel::meta::IsSameType<typename tfel::typetraits::Promote<T,T2>::type,T>::cond&&
      tfel::typetraits::IsFundamentalNumericType<T2>::cond&&
      tfel::typetraits::IsScalar<T2>::cond,
      qt<NoUnit,T>&
    >::type
    qt<NoUnit,T>::operator = (const T2 src){
      this->value = src;
      return *this;
    }

    template<typename T>
    template<typename T2>
    TFEL_MATH_INLINE typename tfel::meta::EnableIf<
      tfel::meta::IsSameType<typename tfel::typetraits::Promote<T,T2>::type,T>::cond,
      qt<NoUnit,T>&
    >::type
    qt<NoUnit,T>::operator += (const qt<NoUnit,T2>&src){
      this->value += src.getValue();
      return *this;
    }

    template<typename T>
    template<typename T2>
    TFEL_MATH_INLINE typename tfel::meta::EnableIf<tfel::meta::IsSameType<typename tfel::typetraits::Promote<T,T2>::type,T>::cond,qt<NoUnit,T>&>::type
    qt<NoUnit,T>::operator -= (const qt<NoUnit,T2>&src){
      this->value -= src.getValue();
      return *this;
    }

    template<typename T>
    template<typename T2>
    TFEL_MATH_INLINE typename tfel::meta::EnableIf<
      tfel::typetraits::IsFundamentalNumericType<T2>::cond&&
      tfel::typetraits::IsScalar<T>::cond&&
      tfel::meta::IsSameType<typename tfel::typetraits::Promote<T,T2>::type,T>::cond,
      qt<NoUnit,T>&
    >::type
    qt<NoUnit,T>::operator *= (const T2 a){
      this->value *= a;
      return *this;
    }

    template<typename T>
    template<typename T2>
    TFEL_MATH_INLINE typename tfel::meta::EnableIf<
      tfel::typetraits::IsFundamentalNumericType<T2>::cond&&
      tfel::typetraits::IsScalar<T>::cond&&
      tfel::meta::IsSameType<typename tfel::typetraits::Promote<T,T2>::type,T>::cond,
      qt<NoUnit,T>&
    >::type
    qt<NoUnit,T>::operator /= (const T2 a){
      this->value /= a;
      return *this;
    }

    template<typename T>
    qt<NoUnit,T>::operator T() const
    {
      return this->value;
    }

    template<typename unit,typename T>
    TFEL_MATH_INLINE bool operator < (const qt<unit,T> a, const qt<unit,T> b)
    {
      return a.getValue() < b.getValue();
    }

    template<typename unit,typename T>
    TFEL_MATH_INLINE bool operator <= (const qt<unit,T> a, const qt<unit,T> b)
    {
      return a.getValue() <= b.getValue();
    }

    template<typename unit,typename T>
    TFEL_MATH_INLINE bool operator > (const qt<unit,T> a, const qt<unit,T> b)
    {
      return a.getValue() > b.getValue();
    }

    template<typename unit,typename T>
    TFEL_MATH_INLINE bool operator >= (const qt<unit,T> a, const qt<unit,T> b)
    {
      return a.getValue() >= b.getValue();
    }

    template<typename unit,typename T>
    TFEL_MATH_INLINE bool operator == (const qt<unit,T> a, const qt<unit,T> b)
    {
      return a.getValue() == b.getValue();
    }

    template<typename unit,typename T>
    TFEL_MATH_INLINE bool operator != (const qt<unit,T> a, const qt<unit,T> b)
    {
      return a.getValue() != b.getValue();
    }

    
    template<typename unit,typename T>
    std::ostream& operator << (std::ostream& os, const qt<unit,T>& q)
    { 
      os << q.getValue();
      return os;
    }

  } // end of namespace math

} // end of namespace tfel

namespace std{

  template<typename unit,typename T>
  TFEL_MATH_INLINE tfel::math::qt<unit,T> abs(const tfel::math::qt<unit,T> a)
  {
    return tfel::math::qt<unit,T>(std::abs(a.getValue()));
  }

}// end of namespace std

#endif /* _LIB_TFEL_QT_I_ */

