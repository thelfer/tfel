/*!
 * \file   Unit.ixx
 * \brief  This file implements the classes declared in Unit.hxx
 * \author Helfer Thomas
 * \date   17 Jul 2006
 */

#ifndef _LIB_TFEL_UNIT_I_
#define _LIB_TFEL_UNIT_I_ 

#include"TFEL/Math/Quantity/FracSimplify.hxx"

namespace tfel{

  namespace math{

    template<int N1,int N2,int N3,int N4,int N5,int N6,int N7,
	     unsigned int D1,unsigned int D2,unsigned int D3,
	     unsigned int D4,unsigned int D5,unsigned int D6,unsigned int D7>
    const std::string Unit<tfel::meta::Int2Type<N1>,
		     tfel::meta::Int2Type<N2>,
		     tfel::meta::Int2Type<N3>,
		     tfel::meta::Int2Type<N4>,
		     tfel::meta::Int2Type<N5>,
		     tfel::meta::Int2Type<N6>,
		     tfel::meta::Int2Type<N7>,
		     tfel::meta::UInt2Type<D1>,
		     tfel::meta::UInt2Type<D2>,
		     tfel::meta::UInt2Type<D3>,
		     tfel::meta::UInt2Type<D4>,
		     tfel::meta::UInt2Type<D5>,
		     tfel::meta::UInt2Type<D6>,
		     tfel::meta::UInt2Type<D7> >::getName(void)
    {
      std::ostringstream os;
      os << "[";
      os << "kg^" << N1;
      if(D1!=1){
	os << "/" << D1;
      }
      os << ",";
      os << "m^" << N2;
      if(D2!=1){
	os << "/" << D2;
      }
      os << ",";
      os << "s^" << N3;
      if(D3!=1){
	os << "/" << D3;
      }
      os << ",";
      os << "A^" << N4;
      if(D4!=1){
	os << "/" << D4;
      }
      os << ",";
      os << "K^" << N5;
      if(D5!=1){
	os << "/" << D5;
      }
      os << ",";
      os << "cd^" << N6;
      if(D6!=1){
	os << "/" << D6;
      }
      os << ",";
      os << "mol^" << N7;
      if(D7!=1){
	os << "/" << D7;
      }
      os << "]";
      return os.str();
    }
    
    template<int N1,int N2,int N3,int N4,int N5,int N6,int N7,
	     unsigned int D1,unsigned int D2,unsigned int D3,
	     unsigned int D4,unsigned int D5,unsigned int D6,
	     unsigned int D7,int O1,int O2,int O3,int O4,int O5,
	     int O6,int O7,unsigned int AP1,unsigned int AP2,
	     unsigned int AP3,unsigned int AP4,unsigned int AP5,
	     unsigned int AP6,unsigned int AP7>
    struct AddUnit_<Unit<tfel::meta::Int2Type<N1>,
			 tfel::meta::Int2Type<N2>,
			 tfel::meta::Int2Type<N3>,
			 tfel::meta::Int2Type<N4>,
			 tfel::meta::Int2Type<N5>,
			 tfel::meta::Int2Type<N6>,
			 tfel::meta::Int2Type<N7> ,
			 tfel::meta::UInt2Type<D1>,
			 tfel::meta::UInt2Type<D2>,
			 tfel::meta::UInt2Type<D3>,
			 tfel::meta::UInt2Type<D4>,
			 tfel::meta::UInt2Type<D5>,
			 tfel::meta::UInt2Type<D6>,
			 tfel::meta::UInt2Type<D7> >,
		    Unit<tfel::meta::Int2Type<O1>,
			 tfel::meta::Int2Type<O2>,
			 tfel::meta::Int2Type<O3>,
			 tfel::meta::Int2Type<O4>,
			 tfel::meta::Int2Type<O5>,
			 tfel::meta::Int2Type<O6>,
			 tfel::meta::Int2Type<O7>,
			 tfel::meta::UInt2Type<AP1>,
			 tfel::meta::UInt2Type<AP2>,
			 tfel::meta::UInt2Type<AP3>,
			 tfel::meta::UInt2Type<AP4>,
			 tfel::meta::UInt2Type<AP5>,
			 tfel::meta::UInt2Type<AP6>,
			 tfel::meta::UInt2Type<AP7> > >
    {
      typedef typename GenerateUnit<FracSimplify<N1*AP1+O1*D1,D1*AP1>::N,
				    FracSimplify<N2*AP2+O2*D2,D2*AP2>::N,
				    FracSimplify<N3*AP3+O3*D3,D3*AP3>::N,
				    FracSimplify<N4*AP4+O4*D4,D4*AP4>::N,
				    FracSimplify<N5*AP5+O5*D5,D5*AP5>::N,
				    FracSimplify<N6*AP6+O6*D6,D6*AP6>::N,
				    FracSimplify<N7*AP7+O7*D7,D7*AP7>::N,
				    FracSimplify<N1*AP1+O1*D1,D1*AP1>::D,
				    FracSimplify<N2*AP2+O2*D2,D2*AP2>::D,
				    FracSimplify<N3*AP3+O3*D3,D3*AP3>::D,
				    FracSimplify<N4*AP4+O4*D4,D4*AP4>::D,
				    FracSimplify<N5*AP5+O5*D5,D5*AP5>::D,
				    FracSimplify<N6*AP6+O6*D6,D6*AP6>::D,
				    FracSimplify<N7*AP7+O7*D7,D7*AP7>::D>::type type;
    };

    template<int N1,int N2,int N3,int N4,int N5,int N6,int N7,
	     int O1,int O2,int O3,int O4,int O5,int O6,int O7>
    struct AddUnit_<Unit<tfel::meta::Int2Type<N1>,
			 tfel::meta::Int2Type<N2>,
			 tfel::meta::Int2Type<N3>,
			 tfel::meta::Int2Type<N4>,
			 tfel::meta::Int2Type<N5>,
			 tfel::meta::Int2Type<N6>,
			 tfel::meta::Int2Type<N7> ,
			 tfel::meta::UInt2Type<1u>,
			 tfel::meta::UInt2Type<1u>,
			 tfel::meta::UInt2Type<1u>,
			 tfel::meta::UInt2Type<1u>,
			 tfel::meta::UInt2Type<1u>,
			 tfel::meta::UInt2Type<1u>,
			 tfel::meta::UInt2Type<1u> >,
		    Unit<tfel::meta::Int2Type<O1>,
			 tfel::meta::Int2Type<O2>,
			 tfel::meta::Int2Type<O3>,
			 tfel::meta::Int2Type<O4>,
			 tfel::meta::Int2Type<O5>,
			 tfel::meta::Int2Type<O6>,
			 tfel::meta::Int2Type<O7>,
			 tfel::meta::UInt2Type<1u>,
			 tfel::meta::UInt2Type<1u>,
			 tfel::meta::UInt2Type<1u>,
			 tfel::meta::UInt2Type<1u>,
			 tfel::meta::UInt2Type<1u>,
			 tfel::meta::UInt2Type<1u>,
			 tfel::meta::UInt2Type<1u> > >
    {
      typedef typename GenerateUnit<N1+O1,N2+O2,N3+O3,N4+O4,N5+O5,N6+O6,N7+O7>::type type;
    };

    
    template<int N1,int N2,int N3,int N4,int N5,int N6,int N7,
	     unsigned int D1,unsigned int D2,unsigned int D3,
	     unsigned int D4,unsigned int D5,unsigned int D6,
	     unsigned int D7,int O1,int O2,int O3,
	     int O4,int O5,int O6,int O7,unsigned int AP1,
	     unsigned int AP2,unsigned int AP3,unsigned int AP4,
	     unsigned int AP5,unsigned int AP6,unsigned int AP7>
    struct SubUnit_<Unit<tfel::meta::Int2Type<N1>,
			 tfel::meta::Int2Type<N2>,
			 tfel::meta::Int2Type<N3>,
			 tfel::meta::Int2Type<N4>,
			 tfel::meta::Int2Type<N5>,
			 tfel::meta::Int2Type<N6>,
			 tfel::meta::Int2Type<N7>,
			 tfel::meta::UInt2Type<D1>,
			 tfel::meta::UInt2Type<D2>,
			 tfel::meta::UInt2Type<D3>,
			 tfel::meta::UInt2Type<D4>,
			 tfel::meta::UInt2Type<D5>,
			 tfel::meta::UInt2Type<D6>,
			 tfel::meta::UInt2Type<D7> >,
		    Unit<tfel::meta::Int2Type<O1>,
			 tfel::meta::Int2Type<O2>,
			 tfel::meta::Int2Type<O3>,
			 tfel::meta::Int2Type<O4>,
			 tfel::meta::Int2Type<O5>,
			 tfel::meta::Int2Type<O6>,
			 tfel::meta::Int2Type<O7>,
			 tfel::meta::UInt2Type<AP1>,
			 tfel::meta::UInt2Type<AP2>,
			 tfel::meta::UInt2Type<AP3>,
			 tfel::meta::UInt2Type<AP4>,
			 tfel::meta::UInt2Type<AP5>,
			 tfel::meta::UInt2Type<AP6>,
			 tfel::meta::UInt2Type<AP7> > >
    {
      typedef typename GenerateUnit<FracSimplify<N1*AP1-O1*D1,D1*AP1>::N,
				    FracSimplify<N2*AP2-O2*D2,D2*AP2>::N,
				    FracSimplify<N3*AP3-O3*D3,D3*AP3>::N,
				    FracSimplify<N4*AP4-O4*D4,D4*AP4>::N,
				    FracSimplify<N5*AP5-O5*D5,D5*AP5>::N,
				    FracSimplify<N6*AP6-O6*D6,D6*AP6>::N,
				    FracSimplify<N7*AP7-O7*D7,D7*AP7>::N,
				    FracSimplify<N1*AP1-O1*D1,D1*AP1>::D,
				    FracSimplify<N2*AP2-O2*D2,D2*AP2>::D,
				    FracSimplify<N3*AP3-O3*D3,D3*AP3>::D,
				    FracSimplify<N4*AP4-O4*D4,D4*AP4>::D,
				    FracSimplify<N5*AP5-O5*D5,D5*AP5>::D,
				    FracSimplify<N6*AP6-O6*D6,D6*AP6>::D,
				    FracSimplify<N7*AP7-O7*D7,D7*AP7>::D>::type type;
    };

    template<int N1,int N2,int N3,int N4,int N5,int N6,int N7,
	     int O1,int O2,int O3,int O4,int O5,int O6,int O7>
    struct SubUnit_<Unit<tfel::meta::Int2Type<N1>,
			 tfel::meta::Int2Type<N2>,
			 tfel::meta::Int2Type<N3>,
			 tfel::meta::Int2Type<N4>,
			 tfel::meta::Int2Type<N5>,
			 tfel::meta::Int2Type<N6>,
			 tfel::meta::Int2Type<N7>,
			 tfel::meta::UInt2Type<1u>,
			 tfel::meta::UInt2Type<1u>,
			 tfel::meta::UInt2Type<1u>,
			 tfel::meta::UInt2Type<1u>,
			 tfel::meta::UInt2Type<1u>,
			 tfel::meta::UInt2Type<1u>,
			 tfel::meta::UInt2Type<1u> >,
		    Unit<tfel::meta::Int2Type<O1>,
			 tfel::meta::Int2Type<O2>,
			 tfel::meta::Int2Type<O3>,
			 tfel::meta::Int2Type<O4>,
			 tfel::meta::Int2Type<O5>,
			 tfel::meta::Int2Type<O6>,
			 tfel::meta::Int2Type<O7>,
			 tfel::meta::UInt2Type<1u>,
			 tfel::meta::UInt2Type<1u>,
			 tfel::meta::UInt2Type<1u>,
			 tfel::meta::UInt2Type<1u>,
			 tfel::meta::UInt2Type<1u>,
			 tfel::meta::UInt2Type<1u>,
			 tfel::meta::UInt2Type<1u> > >
    {
      typedef typename GenerateUnit<N1-O1,N2-O2,N3-O3,N4-O4,N5-O5,N6-O6,N7-O7>::type type;
    };

    template<int N, unsigned int D,int N1,int N2,int N3,int N4,int N5,int N6,int N7,
	     unsigned int D1,unsigned int D2,unsigned int D3,unsigned int D4,
	     unsigned int D5,unsigned int D6,unsigned int D7>
    struct PowUnit_<N,D,Unit<tfel::meta::Int2Type<N1>,
			     tfel::meta::Int2Type<N2>,
			     tfel::meta::Int2Type<N3>,
			     tfel::meta::Int2Type<N4>,
			     tfel::meta::Int2Type<N5>,
			     tfel::meta::Int2Type<N6>,
			     tfel::meta::Int2Type<N7> ,
			     tfel::meta::UInt2Type<D1>,
			     tfel::meta::UInt2Type<D2>,
			     tfel::meta::UInt2Type<D3>,
			     tfel::meta::UInt2Type<D4>,
			     tfel::meta::UInt2Type<D5>,
			     tfel::meta::UInt2Type<D6>,
			     tfel::meta::UInt2Type<D7> > >
    {
      typedef typename GenerateUnit<FracSimplify<N1*N,D1*D>::N,
				    FracSimplify<N2*N,D2*D>::N,
				    FracSimplify<N3*N,D3*D>::N,
				    FracSimplify<N4*N,D4*D>::N,
				    FracSimplify<N5*N,D5*D>::N,
				    FracSimplify<N6*N,D6*D>::N,
				    FracSimplify<N7*N,D7*D>::N,
				    FracSimplify<N1*N,D1*D>::D,
				    FracSimplify<N2*N,D2*D>::D,
				    FracSimplify<N3*N,D3*D>::D,
				    FracSimplify<N4*N,D4*D>::D,
				    FracSimplify<N5*N,D5*D>::D,
				    FracSimplify<N6*N,D6*D>::D,
				    FracSimplify<N7*N,D7*D>::D>::type type;
    };

  } // end of Namesapce math

} // end of namespace tfel

#endif /* _LIB_TFEL_UNIT_I_ */

