/*!
 * \file   Tuple.hxx
 * \see    GenerateTuple.cxx to see how this file was generated.
 * \brief  This file declares the Tuple template class.
 * \author Helfer Thomas
 */

#ifndef _LIB_TFEL_TUPLE_H_
#define _LIB_TFEL_TUPLE_H_ 

#include"TFEL/Metaprogramming/TypeList.hxx"
#include"TFEL/Metaprogramming/GenerateTypeList.hxx"

namespace tfel{

  namespace utilities{

    typedef tfel::meta::TLE TupleEndType;

    /*!
     * \class tuple
     *
     * tuple can contain up to 10 elements.
     *
     * All template parameters takes the default value TupleEndType.
     *
     * \author Helfer Thomas
     */
    template<typename T0=TupleEndType,typename T1=TupleEndType,
	     typename T2=TupleEndType,typename T3=TupleEndType,
	     typename T4=TupleEndType,typename T5=TupleEndType,
	     typename T6=TupleEndType,typename T7=TupleEndType,
	     typename T8=TupleEndType,typename T9=TupleEndType>
    class tuple;

    /*!
     * \return a tuple built on the given parameters
     * \param v0 : first element of the tupe
     * \param v1 : second element of the tupe
     * \param v2 : third element of the tupe
     * \param v3 : fourth element of the tupe
     * \param v4 : fifth element of the tupe
     * \param v5 : sixth element of the tupe
     * \param v6 : 7th element of the tupe
     * \param v7 : 8th element of the tupe
     * \param v8 : 9th element of the tupe
     * \param v9 : 10th element of the tupe
     * \author Helfer Thomas
     */
    template<typename T0,typename T1,typename T2,typename T3,
	     typename T4,typename T5,typename T6,typename T7,
	     typename T8,typename T9>
    tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9>
    makeTuple(const T0&,const T1&,
	      const T2&,const T3&,
	      const T4&,const T5&,
	      const T6&,const T7&,
	      const T8&,const T9&);

    /*!
     * \return a tuple built on the given parameters
     * \param v0 : first element of the tupe
     * \param v1 : second element of the tupe
     * \param v2 : third element of the tupe
     * \param v3 : fourth element of the tupe
     * \param v4 : fifth element of the tupe
     * \param v5 : sixth element of the tupe
     * \param v6 : 7th element of the tupe
     * \param v7 : 8th element of the tupe
     * \param v8 : 9th element of the tupe
     * \author Helfer Thomas
     */
    template<typename T0,typename T1,typename T2,typename T3,
	     typename T4,typename T5,typename T6,typename T7,
	     typename T8>
    tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8>
    makeTuple(const T0&,const T1&,
	      const T2&,const T3&,
	      const T4&,const T5&,
	      const T6&,const T7&,
	      const T8&);

    /*!
     * \return a tuple built on the given parameters
     * \param v0 : first element of the tupe
     * \param v1 : second element of the tupe
     * \param v2 : third element of the tupe
     * \param v3 : fourth element of the tupe
     * \param v4 : fifth element of the tupe
     * \param v5 : sixth element of the tupe
     * \param v6 : 7th element of the tupe
     * \param v7 : 8th element of the tupe
     * \author Helfer Thomas
     */
    template<typename T0,typename T1,typename T2,typename T3,
	     typename T4,typename T5,typename T6,typename T7>
    tuple<T0,T1,T2,T3,T4,T5,T6,T7>
    makeTuple(const T0&,const T1&,
	      const T2&,const T3&,
	      const T4&,const T5&,
	      const T6&,const T7&);

    /*!
     * \return a tuple built on the given parameters
     * \param v0 : first element of the tupe
     * \param v1 : second element of the tupe
     * \param v2 : third element of the tupe
     * \param v3 : fourth element of the tupe
     * \param v4 : fifth element of the tupe
     * \param v5 : sixth element of the tupe
     * \param v6 : 7th element of the tupe
     * \author Helfer Thomas
     */
    template<typename T0,typename T1,typename T2,typename T3,
	     typename T4,typename T5,typename T6>
    tuple<T0,T1,T2,T3,T4,T5,T6>
    makeTuple(const T0&,const T1&,
	      const T2&,const T3&,
	      const T4&,const T5&,
	      const T6&);
    
    /*!
     * \return a tuple built on the given parameters
     * \param v0 : first element of the tupe
     * \param v1 : second element of the tupe
     * \param v2 : third element of the tupe
     * \param v3 : fourth element of the tupe
     * \param v4 : fifth element of the tupe
     * \param v5 : sixth element of the tupe
     * \author Helfer Thomas
     */
    template<typename T0,typename T1,typename T2,typename T3,
	     typename T4,typename T5>
    tuple<T0,T1,T2,T3,T4,T5>
    makeTuple(const T0&,const T1&,
	      const T2&,const T3&,
	      const T4&,const T5&);

    /*!
     * \return a tuple built on the given parameters
     * \param v0 : first element of the tupe
     * \param v1 : second element of the tupe
     * \param v2 : third element of the tupe
     * \param v3 : fourth element of the tupe
     * \param v4 : fifth element of the tupe
     * \author Helfer Thomas
     */
    template<typename T0,typename T1,typename T2,typename T3,
	     typename T4>
    tuple<T0,T1,T2,T3,T4>
    makeTuple(const T0&,const T1&,
	      const T2&,const T3&,
	      const T4&);

    /*!
     * \return a tuple built on the given parameters
     * \param v0 : first element of the tupe
     * \param v1 : second element of the tupe
     * \param v2 : third element of the tupe
     * \param v3 : fourth element of the tupe
     * \author Helfer Thomas
     */
    template<typename T0,typename T1,typename T2,typename T3>
    tuple<T0,T1,T2,T3>
    makeTuple(const T0&,const T1&,
	      const T2&,const T3&);

    /*!
     * \return a tuple built on the given parameters
     * \param v0 : first element of the tupe
     * \param v1 : second element of the tupe
     * \param v2 : third element of the tupe
     * \author Helfer Thomas
     */
    template<typename T0,typename T1,typename T2>
    tuple<T0,T1,T2>
    makeTuple(const T0&,const T1&,
	      const T2&);

    /*!
     * \return a tuple built on the given parameters
     * \param v0 : first element of the tupe
     * \param v1 : second element of the tupe
     * \author Helfer Thomas
     */
    template<typename T0,typename T1>
    tuple<T0,T1>
    makeTuple(const T0&,const T1&);
    
    /*!
     * \return a tuple built on the given parameters
     * \param v0 : first element of the tupe
     * \author Helfer Thomas
     */
    template<typename T0>
    tuple<T0>
    makeTuple(const T0&);

  } // end of namespace utilities

} // end of namespace tfel

#include"TFEL/Utilities/Tuple.ixx"

#endif /* _LIB_TFEL_TUPLE_H_ */
