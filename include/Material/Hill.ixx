/*!
 * \file   Hill.ixx
 * \author Helfer Thomas
 * \date   26 March 2009
 */

#ifndef _LIB_TFEL_MATERIAL_HILL_IXX_
#define _LIB_TFEL_MATERIAL_HILL_IXX_ 

namespace tfel
{

  namespace material
  {

    namespace internals
    {

      template<unsigned short N,
	       typename T>
      struct HillTensor;
      
      template<typename T>
      struct HillTensor<1u,T>
      {
	static tfel::math::st2tost2<1u,T>
	hillTensor(const T F,
		   const T G,
		   const T H,
		   const T,
		   const T,
		   const T)
	{
	  tfel::math::st2tost2<1u,T> h(static_cast<T>(0));
	  h(0,0)=F+H;
	  h(0,1)=-F;
	  h(0,2)=-H;
	  h(1,0)=-F;
	  h(1,1)=G+F;
	  h(1,2)=-G;
	  h(2,0)=-H;
	  h(2,1)=-G;
	  h(2,2)=H+G;
	  return h;
	}
      }; // end of struct HillTensor
      
      template<typename T>
      struct HillTensor<2u,T>
      {
	static tfel::math::st2tost2<2u,T>
	hillTensor(const T F,
		   const T G,
		   const T H,
		   const T L,
		   const T,
		   const T)
	{   
	  tfel::math::st2tost2<2u,T> h(static_cast<T>(0));
	  h(0,0)=F+H;
	  h(0,1)=-F;
	  h(0,2)=-H;
	  h(1,0)=-F;
	  h(1,1)=G+F;
	  h(1,2)=-G;
	  h(2,0)=-H;
	  h(2,1)=-G;
	  h(2,2)=H+G;
	  h(3,3)=L;
	  return h;
	}
      }; // end of struct HillTensor

      template<typename T>
      struct HillTensor<3u,T>    
      {
	static tfel::math::st2tost2<3u,T>
        hillTensor(const T F,
		   const T G,
		   const T H,
		   const T L,
		   const T M,
		   const T N)
	{
	  tfel::math::st2tost2<3u,T> h(static_cast<T>(0));
	  h(0,0)=F+H;
	  h(0,1)=-F;
	  h(0,2)=-H;
	  h(1,0)=-F;
	  h(1,1)=G+F;
	  h(1,2)=-G;
	  h(2,0)=-H;
	  h(2,1)=-G;
	  h(2,2)=H+G;
	  h(3,3)=L;
	  h(4,4)=M;
	  h(5,5)=N;
	  return h;
	}

      }; // end of struct HillTensor
    
    } // end of namespace internals

    template<unsigned short D,
	     typename T>
    tfel::math::st2tost2<D,T>
    hillTensor(const T F,
	       const T G,
	       const T H,
	       const T L,
	       const T M,
	       const T N)
    {
      using namespace tfel::material::internals;
      return HillTensor<D,T>::hillTensor(F,G,H,L,M,N);
    }

  } // end of namespace material

} // end of namespace tfel

#endif /* _LIB_TFEL_MATERIAL_HILL_IXX_ */
