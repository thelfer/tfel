/*!
 * \file   TFTV.hxx
 * \brief  TinyVectorFromTVectorView
 * \author Helfer Thomas
 * \date   16 oct 2008
 */

#ifndef _LIB_TFEL_MATH_TFTV_HXX_
#define _LIB_TFEL_MATH_TFTV_HXX_ 

#include"TFEL/Metaprogramming/StaticAssert.hxx"

#include"TFEL/Math/Vector/VectorUtilities.hxx"
#include"TFEL/Math/Vector/VectorConcept.hxx"
#include"TFEL/Math/Vector/VectorExpr.hxx"
#include"TFEL/Math/General/EmptyRunTimeProperties.hxx"
#include"TFEL/Math/tvector.hxx"

namespace tfel
{
  
  namespace math
  {

    /*!
     *  Tiny vector from Tiny Vector view expression
     * \param[in] Mn : dimension of the underlying vector
     * \param[in] In : Index of the beginning in the underlying vector
     * \param[in] Nn : number of object holed
     * \param[in] T  : underlying type
     */
    template<unsigned short Mn,
	     unsigned short In,
	     unsigned short Nn,
	     typename T>
    struct TFTVExpr
    {
      /*!
       * numerical type holded by the expression
       */
      typedef T NumType;
    }; // end of struct TFTVExpr

    /*!
     * \param[in] Mn : dimension of the underlying vector
     * \param[in] In : Index of the beginning in the underlying vector
     * \param[in] Nn : number of stensor holed
     * \param[in] T  : underlying type
     */
    template<unsigned short Mn,
	     unsigned short In,
	     unsigned short Nn,
	     typename T>
    struct VectorExpr<tvector<Nn,T>,TFTVExpr<Mn,In,Nn,T> >
      : public VectorConcept<VectorExpr<tvector<Nn,T>,TFTVExpr<Mn,In,Nn,T> > >
    {

      typedef EmptyRunTimeProperties RunTimeProperties;

      VectorExpr(tvector<Mn,T>& v_)
	: v(v_)
      {} // end of VectorExpr

      /*
       * Return the RunTimeProperties of the tvector
       * \return tvector::RunTimeProperties
       */
      const RunTimeProperties
      getRunTimeProperties(void) const
      {
	return RunTimeProperties();
      }

      const T&
      operator[](const unsigned short i) const
      {
	union{
	  const T * t_ptr;
	  const T * s_ptr;
	} ptr;
	ptr.t_ptr = this->v.begin()+In+i;
	return *(ptr.s_ptr);
      } // end of operator[] const

      T&
      operator[](const unsigned short i)
      {
	union{
	  T * t_ptr;
	  T * s_ptr;
	} ptr;
	ptr.t_ptr = this->v.begin()+In+i;
	return *(ptr.s_ptr);
      } // end of operator[]

      const T&
      operator()(const unsigned short i) const
      {
	union{
	  const T * t_ptr;
	  const T * s_ptr;
	} ptr;
	ptr.t_ptr = this->v.begin()+In+i;
	return *(ptr.s_ptr);
      } // end of operator() const

      T&
      operator()(const unsigned short i)
      {
	union{
	  T * t_ptr;
	  T * s_ptr;
	} ptr;
	ptr.t_ptr = this->v.begin()+In+i;
	return *(ptr.s_ptr);
      } // end of operator()

      /*!
       * Assignement operator
       */
      template<typename T2>
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsAssignableTo<T2,T>::cond,
	VectorExpr&>::type
      operator=(const tvector<Nn,T2 >& s){
	VectorUtilities<Nn>::copy(s,*this);
	return *this;
      }
      
      /*!
       * Assignement operator
       */
      template<typename T2,typename Expr>
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsAssignableTo<T2,T>::cond,
	VectorExpr&
      >::type
      operator=(const VectorExpr<tvector<Nn,T2 >,Expr>& s)
      {
	VectorUtilities<Nn>::copy(s,*this);
	return *this;
      }

      // Assignement operator
      template<typename T2>
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsAssignableTo<T2,T>::cond,
	VectorExpr&
      >::type
      operator+=(const tvector<Nn,T2 >& s){
	VectorUtilities<Nn>::PlusEqual(*this,s);
	return *this;
      }
    
      // Assignement operator
      template<typename T2,typename Expr>
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsAssignableTo<T2,T>::cond,
	VectorExpr&
      >::type
      operator+=(const VectorExpr<tvector<Nn,T2 >,Expr>& s){
	VectorUtilities<Nn>::PlusEqual(*this,s);
	return *this;
      }

      // Assignement operator
      template<typename T2>
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsAssignableTo<T2,T>::cond,
	VectorExpr&
      >::type
      operator-=(const tvector<Nn,T2 >& s){
	VectorUtilities<Nn>::MinusEqual(*this,s);
	return *this;
      }
    
      // Assignement operator
      template<typename T2,typename Expr>
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsAssignableTo<T2,T>::cond,
	VectorExpr&
      >::type
      operator-=(const VectorExpr<tvector<Nn,T2 >,Expr>& s){
	VectorUtilities<Nn>::MinusEqual(*this,s);
	return *this;
      }

      /*!
       * operator*=
       */
      template<typename T2>
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsScalar<T2>::cond&&
      tfel::meta::IsSameType<typename ResultType<T,T2,OpMult>::type,T>::cond,
	VectorExpr&
      >::type
      operator*=(const T2 a){
	VectorUtilities<Nn>::scale(*this,a);
	return *this;
      }

      /*!
       * operator/=
       */
      template<typename T2>
      typename tfel::meta::EnableIf<
      tfel::typetraits::IsScalar<T2>::cond&&
      tfel::meta::IsSameType<typename ResultType<T,T2,OpMult>::type,T>::cond,
	VectorExpr&
	>::type
	operator/=(const T2 a){
	VectorUtilities<Nn>::scale(*this,1/a);
	return *this;
      }
      
    protected:
	
      tvector<Mn,T>& v;

      private:
	
	/*!
       * Simple checks
       */
      TFEL_STATIC_ASSERT((In<Mn));
      TFEL_STATIC_ASSERT((Nn<=Mn-In));

    }; // end of struct VectorExpr


    template<unsigned short Mn,
	     unsigned short In,
	     unsigned short Nn,
	     typename T = double>
    struct TFTV
    {
      typedef VectorExpr<tvector<Nn,T >,TFTVExpr<Mn,In,Nn,T> > type;
    }; // end of struct TFTV

    // Serialisation operator
    template<unsigned short Mn,
	     unsigned short In,
	     unsigned short Nn,
	     typename T>
    std::ostream&
    operator << (std::ostream & os,
		 const VectorExpr<tvector<Nn,T >,TFTVExpr<Mn,In,Nn,T> >& s)
    {
      os << "[ ";
      for(unsigned short i=0;i<Nn;++i){
	os << s(i) << " ";
      }
      os << "]";
      return os;
    } // end of operator << 
    
  } // end of namespace math

  namespace typetraits{

    template<unsigned short Mn,
  	     unsigned short In,
  	     unsigned short Nn,
  	     typename T>
    struct IsTemporary<tfel::math::VectorExpr<tfel::math::tvector<Nn,T >,
					      tfel::math::TFTVExpr<Mn,In,Nn,T> > >
    {
      static const bool cond = false;
    };

  }// end of namespace typetraits

} // end of namespace tfel

#endif /* _LIB_TFEL_MATH_TFTV_HXX */

