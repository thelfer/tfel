/*! 
 * \file  mfront/include/MFront/ZMAT/ZMATInterface.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 26 mai 2014
 */

#ifndef _LIB_MFRONT_ZMAT_ZMATINTERFACE_H_
#define _LIB_MFRONT_ZMAT_ZMATINTERFACE_H_ 

#include<Matrix.h>
#include"MFront/ZMAT/ZMATUndefs.hxx"

#include"TFEL/Math/stensor.hxx"
#include"TFEL/Math/tensor.hxx"
#include"TFEL/Math/st2tost2.hxx"

namespace zmat
{

  /*!
   * conversion from TFEL to ZMAT and back
   */
  struct ZMATInterface
  {
    /*!
     * convert an 1D ZMAT symmetric tensor to a TFEL one
     * \param[in] src  : source
     * \param[in] dest : destination
     */
    template<typename T,
	     template<unsigned short,typename> class Storage>
    static inline 
    typename tfel::meta::EnableIf<
      tfel::meta::IsSameType<typename tfel::typetraits::BaseType<T>::type,double>::cond,
      void>::type
    convert(tfel::math::stensor<1u,T,Storage>&,
	    const double* const);
    /*!
     * convert an 2D ZMAT symmetric tensor to a TFEL one
     * \param[in] src  : source
     * \param[in] dest : destination
     */
    template<typename T,
	     template<unsigned short,typename> class Storage>
    static inline 
    typename tfel::meta::EnableIf<
      tfel::meta::IsSameType<typename tfel::typetraits::BaseType<T>::type,double>::cond,
      void>::type
    convert(tfel::math::stensor<2u,T,Storage>&,
	    const double* const);
    /*!
     * convert an 3D ZMAT symmetric tensor to a TFEL one
     * \param[in] src  : source
     * \param[in] dest : destination
     */
    template<typename T,
	     template<unsigned short,typename> class Storage>
    static inline 
    typename tfel::meta::EnableIf<
      tfel::meta::IsSameType<typename tfel::typetraits::BaseType<T>::type,double>::cond,
      void>::type
    convert(tfel::math::stensor<3u,T,Storage>&,
	    const double* const);
    /*!
     * convert an 1D TFEL symmetric tensor to a ZMAT one
     * \param[in] src  : source
     * \param[in] dest : destination
     */
    template<typename T,
	     template<unsigned short,typename> class Storage>
    static inline 
    typename tfel::meta::EnableIf<
      tfel::meta::IsSameType<typename tfel::typetraits::BaseType<T>::type,double>::cond,
      void>::type
    convert(double* const,
	    const tfel::math::stensor<1u,T,Storage>&);
    /*!
     * convert an 2D TFEL symmetric tensor to a ZMAT one
     * \param[in] src  : source
     * \param[in] dest : destination
     */
    template<typename T,
	     template<unsigned short,typename> class Storage>
    static inline 
    typename tfel::meta::EnableIf<
      tfel::meta::IsSameType<typename tfel::typetraits::BaseType<T>::type,double>::cond,
      void>::type
    convert(double* const,
	    const tfel::math::stensor<2u,T,Storage>&);
    /*!
     * convert an 3D TFEL symmetric tensor to a ZMAT one
     * \param[in] src  : source
     * \param[in] dest : destination
     */
    template<typename T,
	     template<unsigned short,typename> class Storage>
    static inline 
    typename tfel::meta::EnableIf<
      tfel::meta::IsSameType<typename tfel::typetraits::BaseType<T>::type,double>::cond,
      void>::type
    convert(double* const,
	    const tfel::math::stensor<3u,T,Storage>&);
    /*!
     * convert an 1D ZMAT  tensor to a TFEL one
     * \param[in] src  : source
     * \param[in] dest : destination
     */
    template<typename T>
    static inline 
    typename tfel::meta::EnableIf<
      tfel::meta::IsSameType<typename tfel::typetraits::BaseType<T>::type,double>::cond,
      void>::type
    convert(tfel::math::tensor<1u,T>&,
	    const double* const);
    /*!
     * convert an 2D ZMAT  tensor to a TFEL one
     * \param[in] src  : source
     * \param[in] dest : destination
     */
    template<typename T>
    static inline 
    typename tfel::meta::EnableIf<
      tfel::meta::IsSameType<typename tfel::typetraits::BaseType<T>::type,double>::cond,
      void>::type
    convert(tfel::math::tensor<2u,T>&,
	    const double* const);
    /*!
     * convert an 3D ZMAT  tensor to a TFEL one
     * \param[in] src  : source
     * \param[in] dest : destination
     */
    template<typename T>
    static inline 
    typename tfel::meta::EnableIf<
      tfel::meta::IsSameType<typename tfel::typetraits::BaseType<T>::type,double>::cond,
      void>::type
    convert(tfel::math::tensor<3u,T>&,
	    const double* const);
    /*!
     * convert an 1D TFEL  tensor to a ZMAT one
     * \param[in] src  : source
     * \param[in] dest : destination
     */
    template<typename T>
    static inline 
    typename tfel::meta::EnableIf<
      tfel::meta::IsSameType<typename tfel::typetraits::BaseType<T>::type,double>::cond,
      void>::type
    convert(double* const,
	    const tfel::math::tensor<1u,T>&);
    /*!
     * convert an 2D TFEL  tensor to a ZMAT one
     * \param[in] src  : source
     * \param[in] dest : destination
     */
    template<typename T>
    static inline 
    typename tfel::meta::EnableIf<
      tfel::meta::IsSameType<typename tfel::typetraits::BaseType<T>::type,double>::cond,
      void>::type
    convert(double* const,
	    const tfel::math::tensor<2u,T>&);
    /*!
     * convert an 3D TFEL  tensor to a ZMAT one
     * \param[in] src  : source
     * \param[in] dest : destination
     */
    template<typename T>
    static inline 
    typename tfel::meta::EnableIf<
      tfel::meta::IsSameType<typename tfel::typetraits::BaseType<T>::type,double>::cond,
      void>::type
    convert(double* const,
	    const tfel::math::tensor<3u,T>&);
    /*!
     * convert an 1D TFEL stiffness tensor to a ZMAT one
     * \param[in] src  : source
     * \param[in] dest : destination
     */
    template<typename T>
    static inline 
    typename tfel::meta::EnableIf<
      tfel::meta::IsSameType<typename tfel::typetraits::BaseType<T>::type,double>::cond,
      void>::type
    convert(ZSET::MATRIX&,
	    const tfel::math::st2tost2<1u,T>&);
    /*!
     * convert an 2D TFEL stiffness tensor to a ZMAT one
     * \param[in] src  : source
     * \param[in] dest : destination
     */
    template<typename T>
    static inline 
    typename tfel::meta::EnableIf<
      tfel::meta::IsSameType<typename tfel::typetraits::BaseType<T>::type,double>::cond,
      void>::type
    convert(ZSET::MATRIX&,
	    const tfel::math::st2tost2<2u,T>&);
    /*!
     * convert an 3D TFEL stiffness tensor to a ZMAT one
     * \param[in] src  : source
     * \param[in] dest : destination
     */
    template<typename T>
    static inline 
    typename tfel::meta::EnableIf<
      tfel::meta::IsSameType<typename tfel::typetraits::BaseType<T>::type,double>::cond,
      void>::type
    convert(ZSET::MATRIX&,
	    const tfel::math::st2tost2<3u,T>&);
    /*!
     * convert an 1D ZMAT stiffness tensor to a TFEL one
     * \param[in] src  : source
     * \param[in] dest : destination
     */
    template<typename T>
    static inline 
    typename tfel::meta::EnableIf<
      tfel::meta::IsSameType<typename tfel::typetraits::BaseType<T>::type,double>::cond,
      void>::type
    convert(tfel::math::st2tost2<1u,T>&,
	    const ZSET::MATRIX&);
    /*!
     * convert a 2D ZMAT stiffness tensor to a TFEL one
     * \param[in] src  : source
     * \param[in] dest : destination
     */
    template<typename T>
    static inline 
    typename tfel::meta::EnableIf<
      tfel::meta::IsSameType<typename tfel::typetraits::BaseType<T>::type,double>::cond,
      void>::type
    convert(tfel::math::st2tost2<2u,T>&,
	    const ZSET::MATRIX&);
    /*!
     * convert an ZSET stiffness tensor to a TFEL one
     * \param[in] src  : source
     * \param[in] dest : destination
     */
    template<typename T>
    static inline 
    typename tfel::meta::EnableIf<
      tfel::meta::IsSameType<typename tfel::typetraits::BaseType<T>::type,double>::cond,
      void>::type
    convert(tfel::math::st2tost2<3u,T>&,
	    const ZSET::MATRIX&);
  };

} // end of namespace zmat

#include"MFront/ZMAT/ZMATInterface.ixx"

#endif /* _LIB_MFRONT_ZMAT_ZMATINTERFACE_H */

