/*!
 * \file   include/TFEL/Math/Vector/TinyVectorOfStensorFromTinyVectorView.hxx
 * \brief  TinyVectorOfStensorFromTVectorView
 * This class creates an object that acts like a tiny vector of
 * stensors.
 * \author Helfer Thomas
 * \date   16 oct 2008
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_TFEL_MATH_TINYVECTOROFSTENSORFROMTINYVECTORVIEW_HXX_
#define _LIB_TFEL_MATH_TINYVECTOROFSTENSORFROMTINYVECTORVIEW_HXX_ 

#include"TFEL/Metaprogramming/StaticAssert.hxx"

#include"TFEL/Math/Vector/VectorUtilities.hxx"
#include"TFEL/Math/Vector/VectorConcept.hxx"
#include"TFEL/Math/tvector.hxx"
#include"TFEL/Math/stensor.hxx"

namespace tfel
{
  
  namespace math
  {

    /*!
     *  Tvector of stensors  from Tiny Vector view expression
     * \param[in] N  : dimension used to define the stensor (1,2 or 3)
     * \param[in] Mn : dimension of the underlying vector
     * \param[in] In : Index of the first stensor in the underlying vector
     * \param[in] Nn : number of stensor holed
     * \param[in] T  : underlying type
     */
    template<unsigned short N,
	     unsigned short Mn,
	     unsigned short In,
	     unsigned short Nn,
	     typename T>
    struct TinyVectorOfStensorFromTinyVectorViewExpr
    {
      /*!
       * numerical type holded by the expression
       */
      typedef stensor<N,T> NumType;
    }; // end of struct TinyVectorOfStensorFromTinyVectorViewExpr

    /*!
     * \param[in] N  : dimension used to define the stensor (1,2 or 3)
     * \param[in] Mn : dimension of the underlying vector
     * \param[in] In : Index of the first stensor in the underlying vector
     * \param[in] Nn : number of stensor holed
     * \param[in] T  : underlying type
     */
    template<unsigned short N,
	     unsigned short Mn,
	     unsigned short In,
	     unsigned short Nn,
	     typename T>
    struct Expr<tvector<Nn,stensor<N,T> >,TinyVectorOfStensorFromTinyVectorViewExpr<N,Mn,In,Nn,T> >
      : public VectorConcept<Expr<tvector<Nn,stensor<N,T> >,TinyVectorOfStensorFromTinyVectorViewExpr<N,Mn,In,Nn,T> > >
    {

      typedef EmptyRunTimeProperties RunTimeProperties;

      Expr(tvector<Mn,T>& v_)
	: v(v_)
      {} // end of Expr

      /*
       * Return the RunTimeProperties of the tvector
       * \return tvector::RunTimeProperties
       */
      const RunTimeProperties
      getRunTimeProperties(void) const
      {
	return RunTimeProperties();
      }

      const stensor<N,T>&
      operator[](const unsigned short i) const
      {
	union{
	  const T * t_ptr;
	  const stensor<N,T> * s_ptr;
	} ptr;
	ptr.t_ptr = this->v.begin()+In+i*StensorDimeToSize<N>::value;
	return *(ptr.s_ptr);
      } // end of operator[] const

      stensor<N,T>&
      operator[](const unsigned short i)
      {
	union{
	  T * t_ptr;
	  stensor<N,T> * s_ptr;
	} ptr;
	ptr.t_ptr = this->v.begin()+In+i*StensorDimeToSize<N>::value;
	return *(ptr.s_ptr);
      } // end of operator[]

      const stensor<N,T>&
      operator()(const unsigned short i) const
      {
	union{
	  const T * t_ptr;
	  const stensor<N,T> * s_ptr;
	} ptr;
	ptr.t_ptr = this->v.begin()+In+i*StensorDimeToSize<N>::value;
	return *(ptr.s_ptr);
      } // end of operator() const

      stensor<N,T>&
      operator()(const unsigned short i)
      {
	union{
	  T * t_ptr;
	  stensor<N,T> * s_ptr;
	} ptr;
	ptr.t_ptr = this->v.begin()+In+i*StensorDimeToSize<N>::value;
	return *(ptr.s_ptr);
      } // end of operator()

      /*!
       * Assignement operator
       */
      template<typename T2>
      typename std::enable_if<
	tfel::typetraits::IsAssignableTo<T2,T>::cond,
	Expr&>::type
      operator=(const tvector<Nn,stensor<N,T2> >& s){
	VectorUtilities<N>::copy(s,*this);
	return *this;
      }
      
      /*!
       * Assignement operator
       */
      template<typename T2,typename Op>
      typename std::enable_if<
	tfel::typetraits::IsAssignableTo<T2,T>::cond,
	Expr&
      >::type
      operator=(const Expr<tvector<Nn,stensor<N,T2> >,Op>& s)
      {
	VectorUtilities<N>::copy(s,*this);
	return *this;
      }

      // Assignement operator
      template<typename T2>
      typename std::enable_if<
	tfel::typetraits::IsAssignableTo<T2,T>::cond,
	Expr&
      >::type
      operator+=(const tvector<Nn,stensor<N,T2> >& s){
	VectorUtilities<N>::PlusEqual(*this,s);
	return *this;
      }
    
      // Assignement operator
      template<typename T2,typename Op>
      typename std::enable_if<
	tfel::typetraits::IsAssignableTo<T2,T>::cond,
	Expr&
      >::type
      operator+=(const Expr<tvector<Nn,stensor<N,T2> >,Op>& s){
	VectorUtilities<N>::PlusEqual(*this,s);
	return *this;
      }

      // Assignement operator
      template<typename T2>
      typename std::enable_if<
	tfel::typetraits::IsAssignableTo<T2,T>::cond,
	Expr&
      >::type
      operator-=(const tvector<Nn,stensor<N,T2> >& s){
	VectorUtilities<N>::MinusEqual(*this,s);
	return *this;
      }
    
      // Assignement operator
      template<typename T2,typename Op>
      typename std::enable_if<
	tfel::typetraits::IsAssignableTo<T2,T>::cond,
	Expr&
      >::type
      operator-=(const Expr<tvector<Nn,stensor<N,T2> >,Op>& s){
	VectorUtilities<N>::MinusEqual(*this,s);
	return *this;
      }

      /*!
       * operator*=
       */
      template<typename T2>
      typename std::enable_if<
	tfel::typetraits::IsScalar<T2>::cond&&
      std::is_same<typename ResultType<T,T2,OpMult>::type,T>::cond,
	Expr&
      >::type
      operator*=(const T2 a){
	VectorUtilities<N>::scale(*this,a);
	return *this;
      }

      /*!
       * operator/=
       */
      template<typename T2>
      typename std::enable_if<
      tfel::typetraits::IsScalar<T2>::cond&&
      std::is_same<typename ResultType<T,T2,OpMult>::type,T>::cond,
	Expr&
	>::type
	operator/=(const T2 a){
	VectorUtilities<N>::scale(*this,1/a);
	return *this;
      }
      
    protected:
	
      tvector<Mn,T>& v;

      private:
	
	/*!
       * Simple checks
       */
	TFEL_STATIC_ASSERT((N==1u)||(N==2u)||(N==3u));
	TFEL_STATIC_ASSERT((In<Mn));
	TFEL_STATIC_ASSERT((Nn*StensorDimeToSize<N>::value<=Mn-In));

    }; // end of struct Expr


    template<unsigned short N,
	     unsigned short Mn,
	     unsigned short In,
	     unsigned short Nn,
	     typename T = double>
    struct TinyVectorOfStensorFromTinyVectorView
    {
      typedef Expr<tvector<Nn,stensor<N,T> >,TinyVectorOfStensorFromTinyVectorViewExpr<N,Mn,In,Nn,T> > type;
    }; // end of struct TinyVectorOfStensorFromTinyVectorView

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_MATH_TINYVECTOROFSTENSORFROMTINYVECTORVIEW_HXX */

