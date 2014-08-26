/*!
 * \file   TinyVectorFromTinyVectorView.hxx
 * \brief  TinyVectorFromTVectorView
 * \author Helfer Thomas
 * \date   16 oct 2008
 */

#ifndef _LIB_TFEL_MATH_TINYVECTORFROMTINYVECTORVIEW_HXX_
#define _LIB_TFEL_MATH_TINYVECTORFROMTINYVECTORVIEW_HXX_ 

#include"TFEL/Metaprogramming/StaticAssert.hxx"

#include"TFEL/Math/Vector/VectorUtilities.hxx"
#include"TFEL/Math/Vector/VectorConcept.hxx"
#include"TFEL/Math/Vector/VectorExpr.hxx"
#include"TFEL/Math/General/EmptyRunTimeProperties.hxx"
#include"TFEL/Math/Forward/tvector.hxx"
#include"TFEL/Math/tvector.hxx"

namespace tfel
{
  
  namespace math
  {

    /*!
     *  Tiny vector from Tiny Vector view expression
     * \param[in] N  : number of object holed
     * \param[in] Mn : dimension of the underlying vector
     * \param[in] In : Index of the beginning in the underlying vector
     * \param[in] T  : underlying type
     */
    template<unsigned short N,
	     unsigned short Mn,
	     unsigned short In,
	     typename T,bool b>
    struct TinyVectorFromTinyVectorViewExpr
    {
      /*!
       * numerical type holded by the expression
       */
      typedef T NumType;
    }; // end of struct TinyVectorFromTinyVectorViewExpr

    /*!
     * \param[in] N : size of the tvector holed
     * \param[in] Mn : dimension of the underlying vector
     * \param[in] In : Index of the beginning in the underlying vector
     * \param[in] T  : underlying type
     * \param[in] b  : if true the underlying tvector is const
     */
    template<unsigned short N,
	     unsigned short Mn,
	     unsigned short In,
	     typename T,bool b>
    struct VectorExpr<tvector<N,T>,TinyVectorFromTinyVectorViewExpr<N,Mn,In,T,b> >
      : public VectorConcept<VectorExpr<tvector<N,T>,TinyVectorFromTinyVectorViewExpr<N,Mn,In,T,b> > >,
	public tvector_base<VectorExpr<tvector<N,T>,TinyVectorFromTinyVectorViewExpr<N,Mn,In,T,b> >,N,T>
    {
      typedef typename tfel::meta::IF<b,const tvector<Mn,T>&,
				      tvector<Mn,T>&>::type ref_type;

      //! a simple alias
      typedef EmptyRunTimeProperties RunTimeProperties;
      /*!
       * constructor
       */
      VectorExpr(ref_type v_)
	: v(v_)
      {} // end of VectorExpr
      /*!
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
	return this->v[static_cast<unsigned short>(In+i)];
      } // end of operator[] const

      T&
      operator[](const unsigned short i)
      {
	return this->v[static_cast<unsigned short>(In+i)];
      } // end of operator[]

      const T&
      operator()(const unsigned short i) const
      {
	return this->v[static_cast<unsigned short>(In+i)];
      } // end of operator() const

      T&
      operator()(const unsigned short i)
      {
	return this->v[static_cast<unsigned short>(In+i)];
      } // end of operator()

      using tvector_base<VectorExpr,N,T>::operator=;

    protected:
      //! underlying vector
      ref_type v;
    private:
      /*!
       * Simple checks
       */
      TFEL_STATIC_ASSERT((In<Mn));
      TFEL_STATIC_ASSERT((N<=Mn-In));
    }; // end of struct VectorExpr


    template<unsigned short N,
	     unsigned short Mn,
	     unsigned short In,
	     typename T,bool b>
    struct TinyVectorFromTinyVectorView
    {
      typedef VectorExpr<tvector<N,T>,TinyVectorFromTinyVectorViewExpr<N,Mn,In,T,b> > type;
    }; // end of struct TinyVectorFromTinyVectorView

    // Serialisation operator
    template<unsigned short N,
	     unsigned short Mn,
	     unsigned short In,
	     typename T,bool b>
    std::ostream&
    operator << (std::ostream & os,
		 const VectorExpr<tvector<N,T>,TinyVectorFromTinyVectorViewExpr<N,Mn,In,T,b> >& s)
    {
      os << "[ ";
      for(unsigned short i=0;i<N;++i){
	os << s(i) << " ";
      }
      os << "]";
      return os;
    } // end of operator << 
    
  } // end of namespace math

  namespace typetraits{

    template<unsigned short N,
  	     unsigned short Mn,
  	     unsigned short In,
  	     typename T,bool b>
    struct IsTemporary<tfel::math::VectorExpr<tfel::math::tvector<N,T>,
					      tfel::math::TinyVectorFromTinyVectorViewExpr<N,Mn,In,T,b> > >
    {
      static const bool cond = false;
    };

  }// end of namespace typetraits

} // end of namespace tfel

#endif /* _LIB_TFEL_MATH_TINYVECTORFROMTINYVECTORVIEW_HXX */

