/*!
 * \file   include/TFEL/Math/ST2toST2/st2tost2.ixx
 * \brief  This file implements the methods of the class st2tost2.
 * \author Helfer Thomas
 * \date   02 jun 2006
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_TFEL_ST2TOST2_IXX_
#define LIB_TFEL_ST2TOST2_IXX_ 

#include <cmath>
#include <iterator>
#include <algorithm>

#include"TFEL/FSAlgorithm/FSAlgorithm.hxx"

#include<type_traits>

#include"TFEL/TypeTraits/IsSafelyReinterpretCastableTo.hxx"
#include"TFEL/Math/General/BaseCast.hxx"
#include"TFEL/Math/Vector/VectorUtilities.hxx"
#include"TFEL/Math/Matrix/MatrixUtilities.hxx"
#include"TFEL/Math/TinyMatrixInvert.hxx"
#include"TFEL/Math/ST2toST2/ConvertT2toST2ToST2toST2Expr.hxx"

namespace tfel{

  namespace math {


    // Assignement operator
    template<typename Child>
    template<typename ST2toST2Type>
    typename std::enable_if<
      tfel::meta::Implements<ST2toST2Type,ST2toST2Concept>::cond &&
      ST2toST2Traits<Child>::dime==ST2toST2Traits<ST2toST2Type>::dime &&
      tfel::typetraits::IsAssignableTo<typename ST2toST2Traits<ST2toST2Type>::NumType,
				       typename ST2toST2Traits<Child>::NumType>::cond,
      Child&>::type
    st2tost2_base<Child>::operator=(const ST2toST2Type& src)
    {
      auto& child = static_cast<Child&>(*this);
      matrix_utilities<StensorDimeToSize<ST2toST2Traits<Child>::dime>::value,
		       StensorDimeToSize<ST2toST2Traits<Child>::dime>::value,
		       StensorDimeToSize<ST2toST2Traits<Child>::dime>::value>::copy(src,child);
      return child;
    }

    template<typename Child>
    template<typename ST2toST2Type>
    typename std::enable_if<
      tfel::meta::Implements<ST2toST2Type,ST2toST2Concept>::cond &&
      ST2toST2Traits<Child>::dime==ST2toST2Traits<ST2toST2Type>::dime &&
      tfel::typetraits::IsAssignableTo<typename ST2toST2Traits<ST2toST2Type>::NumType,
				       typename ST2toST2Traits<Child>::NumType>::cond,
      Child&>::type
    st2tost2_base<Child>::operator+=(const ST2toST2Type& src)
    {
      auto& child = static_cast<Child&>(*this);
      matrix_utilities<StensorDimeToSize<ST2toST2Traits<Child>::dime>::value,
		       StensorDimeToSize<ST2toST2Traits<Child>::dime>::value,
		       StensorDimeToSize<ST2toST2Traits<Child>::dime>::value>::plusEqual(child,src);
      return child;
    }

    // Assignement operator
    template<typename Child>
    template<typename ST2toST2Type>
    typename std::enable_if<
      tfel::meta::Implements<ST2toST2Type,ST2toST2Concept>::cond &&
      ST2toST2Traits<Child>::dime==ST2toST2Traits<ST2toST2Type>::dime &&
      tfel::typetraits::IsAssignableTo<typename ST2toST2Traits<ST2toST2Type>::NumType,
				       typename ST2toST2Traits<Child>::NumType>::cond,
      Child&>::type
    st2tost2_base<Child>::operator-=(const ST2toST2Type& src)
    {
      auto& child = static_cast<Child&>(*this);
      matrix_utilities<StensorDimeToSize<ST2toST2Traits<Child>::dime>::value,
		       StensorDimeToSize<ST2toST2Traits<Child>::dime>::value,
		       StensorDimeToSize<ST2toST2Traits<Child>::dime>::value>::minusEqual(child,src);
      return child;
    }

    // *= operator
    template<typename Child>
    template<typename T2>
    typename std::enable_if<
      tfel::typetraits::IsScalar<T2>::cond&&
      std::is_same<typename ResultType<typename ST2toST2Traits<Child>::NumType,
						 T2,OpMult>::type,
			     typename ST2toST2Traits<Child>::NumType>::cond,
      Child&>::type
    st2tost2_base<Child>::operator*=(const T2 s)
    {
      auto& child = static_cast<Child&>(*this);
      matrix_utilities<StensorDimeToSize<ST2toST2Traits<Child>::dime>::value,
		       StensorDimeToSize<ST2toST2Traits<Child>::dime>::value,
		       StensorDimeToSize<ST2toST2Traits<Child>::dime>::value>::multByScalar(child,s);
      return child;
    }

    // /= operator
    template<typename Child>
    template<typename T2>
    typename std::enable_if<
      tfel::typetraits::IsScalar<T2>::cond&&
      std::is_same<typename ResultType<typename ST2toST2Traits<Child>::NumType,
						 T2,OpDiv>::type,
			     typename ST2toST2Traits<Child>::NumType>::cond,
      Child&>::type
    st2tost2_base<Child>::operator/=(const T2 s)
    {
      auto& child = static_cast<Child&>(*this);
      matrix_utilities<StensorDimeToSize<ST2toST2Traits<Child>::dime>::value,
		       StensorDimeToSize<ST2toST2Traits<Child>::dime>::value,
		       StensorDimeToSize<ST2toST2Traits<Child>::dime>::value>::divByScalar(child,s);
      return child;
    }

    template<unsigned short N, typename T>
    template<typename StensorType>
    typename std::enable_if<
      tfel::meta::Implements<StensorType,StensorConcept>::cond &&
      StensorTraits<StensorType>::dime==N&&
      tfel::typetraits::IsAssignableTo<typename StensorTraits<StensorType>::NumType,T>::cond,
      Expr<st2tost2<N,T>,StensorSquareDerivativeExpr<N> > >::type
    st2tost2<N,T>::dsquare(const StensorType& s)
    {
      return Expr<st2tost2<N,T>,StensorSquareDerivativeExpr<N> >(s);
    }

    template<unsigned short N, typename T>
    template<typename StensorType,
	     typename ST2toST2Type>
    typename std::enable_if<
      tfel::meta::Implements<StensorType,StensorConcept>::cond &&
      tfel::meta::Implements<ST2toST2Type,ST2toST2Concept>::cond &&
      StensorTraits<StensorType>::dime==N&&
      ST2toST2Traits<ST2toST2Type>::dime==N&&
      tfel::typetraits::IsAssignableTo<typename ComputeBinaryResult<typename StensorTraits<StensorType>::NumType,
								    typename ST2toST2Traits<ST2toST2Type>::NumType,
								    OpMult>::Result,T>::cond,
      Expr<st2tost2<N,T>,StensorSquareDerivativeExpr<N> > >::type
    st2tost2<N,T>::dsquare(const StensorType&  s,
			   const ST2toST2Type& C)
    {
      return Expr<st2tost2<N,T>,StensorSquareDerivativeExpr<N> >(s,C);
    }

    template<unsigned short N, typename T>
    template<typename T2toST2Type>
    typename std::enable_if<
      tfel::meta::Implements<T2toST2Type,T2toST2Concept>::cond &&
      T2toST2Traits<T2toST2Type>::dime==N&&
      tfel::typetraits::IsAssignableTo<typename T2toST2Traits<T2toST2Type>::NumType,T>::cond,
      Expr<st2tost2<N,T>,ConvertT2toST2ToST2toST2Expr<N> > >::type
    st2tost2<N,T>::convert(const T2toST2Type& src)
    {
      return Expr<st2tost2<N,T>,ConvertT2toST2ToST2toST2Expr<N> >(src);
    }

    template<unsigned short N, typename T>
    template<typename T2,
	     typename std::enable_if<tfel::typetraits::IsAssignableTo<T2,T>::cond,bool>::type>
    constexpr st2tost2<N,T>::st2tost2(const T2& init)
      : fsarray<StensorDimeToSize<N>::value*
		StensorDimeToSize<N>::value,T>(init)
    {}

    template<unsigned short N, typename T>
    template<typename T2,
	     typename std::enable_if<tfel::typetraits::IsAssignableTo<T2,T>::cond,bool>::type>
    constexpr st2tost2<N,T>::st2tost2(const std::initializer_list<T2>& init)
      : fsarray<StensorDimeToSize<N>::value*
		StensorDimeToSize<N>::value,T>(init)
    {}

    template<unsigned short N, typename T>
    struct ComputeSpeciatlST2ST2Values;

    template<typename T>
    struct ComputeSpeciatlST2ST2Values<1u,T>
    {
      static TFEL_MATH_INLINE constexpr st2tost2<1u,T>
      getId(void){
	return st2tost2<1u,T>{{T{1},T{0},T{0},
			       T{0},T{1},T{0},
			       T{0},T{0},T{1}}};
      } // end of getId()
      static TFEL_MATH_INLINE constexpr st2tost2<1u,T>
      getIxI(void){
	return st2tost2<1u,T>{{T{1},T{1},T{1},
			       T{1},T{1},T{1},
			       T{1},T{1},T{1}}};
      } // end of getIxI()
      static TFEL_MATH_INLINE constexpr st2tost2<1u,T>
      getJ(void){
	return st2tost2<1u,T>{{T{1}/T{3},T{1}/T{3},T{1}/T{3},
			       T{1}/T{3},T{1}/T{3},T{1}/T{3},
			       T{1}/T{3},T{1}/T{3},T{1}/T{3}}};
      } // end of getJ()
      static TFEL_MATH_INLINE constexpr st2tost2<1u,T>
      getK(void){
	return st2tost2<1u,T>{{ T{2}/T{3},-T{1}/T{3},-T{1}/T{3},
			       -T{1}/T{3}, T{2}/T{3},-T{1}/T{3},
			       -T{1}/T{3},-T{1}/T{3}, T{2}/T{3}}};
      } // end of getK()
      static TFEL_MATH_INLINE constexpr st2tost2<1u,T>
      getM(void){
	return st2tost2<1u,T>{{ T{1}     ,-T{1}/T{2},-T{1}/T{2},
			       -T{1}/T{2}, T{1}     ,-T{1}/T{2},
			       -T{1}/T{2},-T{1}/T{2}, T{1}}};
      } // end of getM()
    };

    template<typename T>
    struct ComputeSpeciatlST2ST2Values<2u,T>
    {
      static TFEL_MATH_INLINE constexpr st2tost2<2u,T>
      getId(void){
	return st2tost2<2u,T>{{T{1},T{0},T{0},T{0},
			       T{0},T{1},T{0},T{0},
			       T{0},T{0},T{1},T{0},
			       T{0},T{0},T{0},T{1}}};
      } // end of getId()
      static TFEL_MATH_INLINE constexpr st2tost2<2u,T>
      getIxI(void){
	return st2tost2<2u,T>{{T{1},T{1},T{1},T{0},
			       T{1},T{1},T{1},T{0},
			       T{1},T{1},T{1},T{0},
			       T{0},T{0},T{0},T{0}}};
      } // end of getIxI()
      static TFEL_MATH_INLINE constexpr st2tost2<2u,T>
      getJ(void){
	return st2tost2<2u,T>{{T{1}/T{3},T{1}/T{3},T{1}/T{3},T{0},
			       T{1}/T{3},T{1}/T{3},T{1}/T{3},T{0},
			       T{1}/T{3},T{1}/T{3},T{1}/T{3},T{0},
			       T{0},T{0},T{0},T{0}}};
      } // end of getJ()
      static TFEL_MATH_INLINE constexpr st2tost2<2u,T>
      getK(void){
	return st2tost2<2u,T>{{ T{2}/T{3},-T{1}/T{3},-T{1}/T{3},T{0},
			       -T{1}/T{3}, T{2}/T{3},-T{1}/T{3},T{0},
			       -T{1}/T{3},-T{1}/T{3}, T{2}/T{3},T{0},
			        T{0},           T{0},      T{0},T{1}}};
      } // end of getK()
      static TFEL_MATH_INLINE constexpr st2tost2<2u,T>
      getM(void){
	return st2tost2<2u,T>{{      T{1},-T{1}/T{2},-T{1}/T{2},T{0},
			       -T{1}/T{2},      T{1},-T{1}/T{2},T{0},
			       -T{1}/T{2},-T{1}/T{2},      T{1},T{0},
    				     T{0},           T{0},      T{0},T{3}/T{2}}};
      } // end of getM()
    };
  
    template<typename T>
    struct ComputeSpeciatlST2ST2Values<3u,T>
    {
      static TFEL_MATH_INLINE constexpr st2tost2<3u,T>
      getId(void){
	return st2tost2<3u,T>{{T{1},T{0},T{0},T{0},T{0},T{0},
			       T{0},T{1},T{0},T{0},T{0},T{0},
			       T{0},T{0},T{1},T{0},T{0},T{0},
			       T{0},T{0},T{0},T{1},T{0},T{0},
			       T{0},T{0},T{0},T{0},T{1},T{0},
			       T{0},T{0},T{0},T{0},T{0},T{1}}};
      } // end of getId()
      static TFEL_MATH_INLINE constexpr st2tost2<3u,T>
      getIxI(void){
	return st2tost2<3u,T>{{T{1},T{1},T{1},T{0},T{0},T{0},
			       T{1},T{1},T{1},T{0},T{0},T{0},
			       T{1},T{1},T{1},T{0},T{0},T{0},
			       T{0},T{0},T{0},T{0},T{0},T{0},
			       T{0},T{0},T{0},T{0},T{0},T{0},
			       T{0},T{0},T{0},T{0},T{0},T{0}}};
      } // end of getIxI()
      static TFEL_MATH_INLINE constexpr st2tost2<3u,T>
      getJ(void){
	return st2tost2<3u,T>{{T{1}/T{3},T{1}/T{3},T{1}/T{3},T{0},T{0},T{0},
			       T{1}/T{3},T{1}/T{3},T{1}/T{3},T{0},T{0},T{0},
			       T{1}/T{3},T{1}/T{3},T{1}/T{3},T{0},T{0},T{0},
			       T{0},T{0},T{0},T{0},T{0},T{0},
			       T{0},T{0},T{0},T{0},T{0},T{0},
			       T{0},T{0},T{0},T{0},T{0},T{0}}};
      } // end of getJ()
      static TFEL_MATH_INLINE constexpr st2tost2<3u,T>
      getK(void){
	return st2tost2<3u,T>{{ T{2}/T{3},-T{1}/T{3},-T{1}/T{3},T{0},T{0},T{0},
			       -T{1}/T{3}, T{2}/T{3},-T{1}/T{3},T{0},T{0},T{0},
			       -T{1}/T{3},-T{1}/T{3}, T{2}/T{3},T{0},T{0},T{0},
				T{0},           T{0},      T{0},T{1},T{0},T{0},
				T{0},           T{0},      T{0},T{0},T{1},T{0},
			        T{0},           T{0},      T{0},T{0},T{0},T{1}}};
      } // end of getK()
      static TFEL_MATH_INLINE constexpr st2tost2<3u,T>
      getM(void){
	return st2tost2<3u,T>{{       T{1},-T{1}/T{2},-T{1}/T{2},T{0},T{0},T{0},
				-T{1}/T{2},      T{1},-T{1}/T{2},T{0},T{0},T{0},
				-T{1}/T{2},-T{1}/T{2},      T{1},T{0},T{0},T{0},
				 T{0},           T{0},      T{0},T{3}/T{2},T{0},T{0},
				 T{0},           T{0},      T{0},T{0},T{3}/T{2},T{0},
			         T{0},           T{0},      T{0},T{0},T{0},T{3}/T{2}}};
      } // end of getM()
    };

    template<unsigned short N, typename T>
    constexpr st2tost2<N,T>
    st2tost2<N,T>::Id(void){
      return ComputeSpeciatlST2ST2Values<N,T>::getId();
    } // end of st2tost2<N,T>::Id

    template<unsigned short N, typename T>
    constexpr st2tost2<N,T>
    st2tost2<N,T>::IxI(void){
      return ComputeSpeciatlST2ST2Values<N,T>::getIxI();
    } // end of st2tost2<N,T>::Id

    template<unsigned short N, typename T>
    constexpr st2tost2<N,T>
    st2tost2<N,T>::K(void){
      return ComputeSpeciatlST2ST2Values<N,T>::getK();
    } // end of st2tost2<N,T>::K

    template<unsigned short N, typename T>
    constexpr st2tost2<N,T>
    st2tost2<N,T>::M(void){
      return ComputeSpeciatlST2ST2Values<N,T>::getM();
    } // end of st2tost2<N,T>::M

    template<unsigned short N, typename T>
    constexpr st2tost2<N,T>
    st2tost2<N,T>::J(void){
      return ComputeSpeciatlST2ST2Values<N,T>::getJ();
    } // end of st2tost2<N,T>::J

    template<unsigned short N,typename T>
    template<typename T2,typename Op>
    st2tost2<N,T>::st2tost2(const Expr<st2tost2<N,T2>,Op>& src){
      matrix_utilities<StensorDimeToSize<N>::value,
		       StensorDimeToSize<N>::value,
		       StensorDimeToSize<N>::value>::copy(src,*this);
    }

    template<unsigned short N,typename T>
    st2tost2<N,T>::st2tost2(const ParticularSt2toSt2 id){
      using namespace std;
      unsigned short i,j;
      std::fill(this->v,this->v+StensorDimeToSize<N>::value*StensorDimeToSize<N>::value,T(0));
      if(id==ST2TOST2_IDENTITY){
	for(i=0;i!=StensorDimeToSize<N>::value;++i){
	  this->operator()(i,i)=T(1);
	}
      } else if (id==ST2TOST2_J){
	for(i=0;i!=3;++i){
	  for(j=0;j!=3;++j){
	    this->operator()(i,j)=1/T(3);
	  }
	}
      } else if (id==ST2TOST2_IxI){
	for(i=0;i!=3;++i){
	  for(j=0;j!=3;++j){
	    this->operator()(i,j)=T(1);
	  }
	}
      } else if (id==ST2TOST2_K){
	for(i=0;i!=3;++i){
	  for(j=0;j!=3;++j){
	    if(i==j){
	      this->operator()(i,j)=2/T(3);
	    } else {
	      this->operator()(i,j)=-1/T(3);
	    }
	  }
	}
	for(i=3;i!=StensorDimeToSize<N>::value;++i){
	  this->operator()(i,i)=T(1);
	}
      }
    } // end of st2tost2(const st2tost2<N,T>::ParticularSt2toSt2)

    template<unsigned short N, typename T>
    T& 
    st2tost2<N,T>::operator()(const unsigned short i,const unsigned short j){
      return this->v[StensorDimeToSize<N>::value*i+j];
    }

    template<unsigned short N, typename T>
    const T& 
    st2tost2<N,T>::operator()(const unsigned short i,const unsigned short j) const{
      return this->v[StensorDimeToSize<N>::value*i+j];
    }

    template<unsigned short N, typename T>
    template<typename InputIterator>
    TFEL_MATH_INLINE2 void st2tost2<N,T>::copy(const InputIterator src)
    {
      tfel::fsalgo::copy<StensorDimeToSize<N>::value*StensorDimeToSize<N>::value>::exe(src,*this);
    }

    template<unsigned short N, typename T>
    typename st2tost2<N,T>::RunTimeProperties
    st2tost2<N,T>::getRunTimeProperties(void) const
    {
      return RunTimeProperties();
    } // end of st2tost2<N,T>::getRunTimeProperties

    template<typename ST2toST2Type>
    TFEL_MATH_INLINE2
    typename std::enable_if<
      tfel::meta::Implements<ST2toST2Type,ST2toST2Concept>::cond,
      st2tost2<ST2toST2Traits<ST2toST2Type>::dime,
	       typename ComputeBinaryResult<typename tfel::typetraits::BaseType<typename ST2toST2Traits<ST2toST2Type>::NumType>::type,
					    typename ST2toST2Traits<ST2toST2Type>::NumType,OpDiv>::Result>
    >::type
    invert(const ST2toST2Type& s)
    {
      using tfel::typetraits::BaseType;
      static constexpr unsigned short N           = ST2toST2Traits<ST2toST2Type>::dime;
      static constexpr unsigned short StensorSize = StensorDimeToSize<N>::value;
      typedef typename ST2toST2Traits<ST2toST2Type>::NumType NumType;
      typedef typename BaseType<NumType>::type real;
      typedef typename ComputeBinaryResult<real,NumType,OpDiv>::Result iNumType;
      st2tost2<N,iNumType> is;
      tmatrix<StensorSize,StensorSize,real> m;
      for(unsigned short i=0;i!=StensorSize;++i){
	for(unsigned short j=0;j!=StensorSize;++j){
	  m(i,j) = base_cast(s(i,j));
	}
      }
      TinyMatrixInvert<StensorSize,real>::exe(m);
      for(unsigned short i=0;i!=StensorSize;++i){
	for(unsigned short j=0;j!=StensorSize;++j){
	  is(i,j) = iNumType(m(i,j));
	}
      }
      return is;
    } // end of invert

  } //end of namespace math

} // end of namespace tfel

#endif /* LIB_TFEL_ST2TOST2_IXX_ */

