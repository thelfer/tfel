/*!
 * \file   StensorFromTinyVectorView.hxx
 * \brief  StensorFromTVectorView
 * 
 * \author Helfer Thomas
 * \date   16 oct 2008
 */

#ifndef _LIB_TFEL_MATH_STENSORFROMTINYVECTORVIEW_HXX_
#define _LIB_TFEL_MATH_STENSORFROMTINYVECTORVIEW_HXX_ 

#include"TFEL/Metaprogramming/StaticAssert.hxx"

#include"TFEL/Math/General/EmptyRunTimeProperties.hxx"
#include"TFEL/Math/Vector/VectorUtilities.hxx"
#include"TFEL/Math/Stensor/StensorConcept.hxx"
#include"TFEL/Math/Stensor/StensorExpr.hxx"
#include"TFEL/Math/stensor.hxx"
#include"TFEL/Math/tmatrix.hxx"

namespace tfel
{
  
  namespace math
  {

    /*!
     *  Stensor From Tiny Vector
     * \param N  : space dimension
     * \param Mn : underlying tiny vector size
     * \param In : starting index in the underlying tiny vector
     * \param T  : value type
     */
    template<unsigned short N,
	     unsigned short Mn,
	     unsigned short In,
	     typename T>
    struct StensorFromTinyVectorViewExpr
    {}; // end of struct StensorFromTinyMatrixColumnViewExpr

    /*!
     *  Stensor From Tiny Vector expression
     * \param N  : space dimension
     * \param Mn : underlying tiny vector size
     * \param In : starting index in the underlying tiny vector
     * \param T  : value type
     */
    template<unsigned short N, unsigned short Mn,
	     unsigned short In,typename T>
    struct StensorExpr<stensor<N,T>,StensorFromTinyVectorViewExpr<N,Mn,In,T> >
      : public StensorConcept<StensorExpr<stensor<N,T>,StensorFromTinyVectorViewExpr<N,Mn,In,T> > >,
	public stensor_base<StensorExpr<stensor<N,T>,StensorFromTinyVectorViewExpr<N,Mn,In,T> > >
    {
      typedef EmptyRunTimeProperties RunTimeProperties;
      typedef typename stensor<N,T,StensorStatic>::value_type      value_type;      
      typedef typename stensor<N,T,StensorStatic>::pointer	   pointer;	    
      typedef typename stensor<N,T,StensorStatic>::const_pointer   const_pointer; 
      typedef typename stensor<N,T,StensorStatic>::reference	   reference;	    
      typedef typename stensor<N,T,StensorStatic>::const_reference const_reference;
      typedef typename stensor<N,T,StensorStatic>::size_type 	   size_type;	    
      typedef typename stensor<N,T,StensorStatic>::difference_type difference_type;

      typedef tvector<Mn,T>           first_arg;
      typedef tfel::meta::InvalidType second_arg;

      RunTimeProperties
      getRunTimeProperties() const
      {
	return RunTimeProperties();
      }

      StensorExpr(tvector<Mn,T>& v_)
	: v(v_)
      {} // end of StensorExpr

      const T&
      operator()(const unsigned short i) const
      {
	return this->v(In+i);
      } // end of operator() const

      T&
      operator()(const unsigned short i)
      {
	return this->v(In+i);
      } // end of operator()

      const T&
      operator[](const unsigned short i) const
      {
	return this->v(In+i);
      } // end of operator[] const

      T&
      operator[](const unsigned short i)
      {
	return this->v(In+i);
      } // end of operator[]

      //! using stensor_base::operator=
      using stensor_base<StensorExpr>::operator=;

    protected:

      tvector<Mn,T>& v;

    private:

      /*!
       * Simple checks
       */
      TFEL_STATIC_ASSERT((N==1u)||(N==2u)||(N==3u));
      TFEL_STATIC_ASSERT((In<Mn));
      TFEL_STATIC_ASSERT((StensorDimeToSize<N>::value<=Mn-In));

    }; // end of struct StensorExpr

    template<unsigned short N,
	     unsigned short Mn,
	     unsigned short In,
	     typename T = double>
    struct StensorFromTinyVectorView
    {
      typedef StensorExpr<stensor<N,T>,StensorFromTinyVectorViewExpr<N,Mn,In,T> > type;
    }; // end of struct StensorFromTinyVectorView
    
  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_MATH_STENSORFROMTINYVECTORVIEW_HXX */

