/*!
 * \file   mfront/src/CalculiXComputeStiffnessTensor.cxx
 * \brief    
 * \author Helfer Thomas
 * \date   18 mar 2008
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<algorithm>

#include"TFEL/Math/st2tost2.hxx"
#include"TFEL/Material/StiffnessTensor.hxx"
#include"MFront/CalculiX/CalculiXComputeStiffnessTensor.hxx"

namespace calculix
{
 
  template<typename real>
  static inline void
  CalculiXComputeIsotropicStiffnessTensor2DImpl(typename tfel::config::Types<2u,real,false>::StiffnessTensor& C,
					      const real* const props)
  {
    using namespace tfel::material;
    using STAC = StiffnessTensorAlterationCharacteristic;
    computeIsotropicStiffnessTensorII<2u,STAC::UNALTERED>(C,props[0],props[1]);
  }

  template<typename real>
  static inline void
  CalculiXComputeIsotropicPlaneStressAlteredStiffnessTensorImpl(typename tfel::config::Types<2u,real,false>::StiffnessTensor& C,
							      const real* const props)
					       
  {
    using namespace tfel::material;
    using STAC = StiffnessTensorAlterationCharacteristic;
    computeIsotropicStiffnessTensorII<2u,STAC::ALTERED>(C,props[0],props[1]);
  } // end of struct CalculiXComputeIsotropicPlaneStressAlteredStiffnessTensor

  template<typename real>
  static inline void
  CalculiXComputeIsotropicStiffnessTensor3DImpl(typename tfel::config::Types<3u,real,false>::StiffnessTensor& C,
					      const real* const props)
  {
    using namespace tfel::material;
    using STAC = StiffnessTensorAlterationCharacteristic;
    computeIsotropicStiffnessTensorII<3u,STAC::UNALTERED>(C,props[0],props[1]);
  } // end of struct CalculiXComputeStiffnessTensor

  template<typename real>
  static inline void
  CalculiXComputeOrthotropicStiffnessTensor2DImpl(typename tfel::config::Types<2u,real,false>::StiffnessTensor& C,
						const real* const props)
  {
    using namespace tfel::material;
    using STAC = StiffnessTensorAlterationCharacteristic;
    computeOrthotropicStiffnessTensorII<2u,STAC::UNALTERED>(C,props[0],props[1],props[2],
							    props[3],props[4],props[5],
							    props[6],props[6],props[6]);
  } // end of struct CalculiXComputeStiffnessTensor

  template<typename real>
  static inline void
  CalculiXComputeOrthotropicPlaneStressAlteredStiffnessTensorImpl(typename tfel::config::Types<2u,real,false>::StiffnessTensor& C,
								const real* const props)
  {
    using namespace tfel::material;
    using STAC = StiffnessTensorAlterationCharacteristic;
    computeOrthotropicStiffnessTensorII<2u,STAC::ALTERED>(C,props[0],props[1],props[2],
							  props[3],props[4],props[5],
							  props[6],props[6],props[6]);
  } // end of struct CalculiXComputeOrthotropicPlaneStressAlteredStiffnessTensor

  template<typename real>
  static inline void
  CalculiXComputeOrthotropicStiffnessTensor3DImpl(typename tfel::config::Types<3u,real,false>::StiffnessTensor& C,
						const real* const props)
  {
    using namespace tfel::material;
    using STAC = StiffnessTensorAlterationCharacteristic;
    computeOrthotropicStiffnessTensorII<3u,STAC::UNALTERED>(C,props[0],props[1],props[2],
							    props[3],props[4],props[5],
							    props[6],props[7],props[8]);
  } // end of struct CalculiXComputeStiffnessTensor

  void
  CalculiXComputeIsotropicStiffnessTensor2D(tfel::config::Types<2u,float,false>::StiffnessTensor& C,
					  const float* const props)
  {
    CalculiXComputeIsotropicStiffnessTensor2DImpl(C,props);
  }

  void
  CalculiXComputeIsotropicPlaneStressAlteredStiffnessTensor(tfel::config::Types<2u,float,false>::StiffnessTensor& C,
							  const float* const props)
					       
  {
    CalculiXComputeIsotropicPlaneStressAlteredStiffnessTensorImpl(C,props);
  } // end of struct CalculiXComputeIsotropicPlaneStressAlteredStiffnessTensor

  void
  CalculiXComputeIsotropicStiffnessTensor3D(tfel::config::Types<3u,float,false>::StiffnessTensor& C,
					  const float* const props)
  {
    CalculiXComputeIsotropicStiffnessTensor3DImpl(C,props);
  } // end of struct CalculiXComputeStiffnessTensor

  void
  CalculiXComputeOrthotropicStiffnessTensor2D(tfel::config::Types<2u,float,false>::StiffnessTensor& C,
					    const float* const props)
  {
    CalculiXComputeOrthotropicStiffnessTensor2DImpl(C,props);
  } // end of struct CalculiXComputeStiffnessTensor

  void
  CalculiXComputeOrthotropicPlaneStressAlteredStiffnessTensor(tfel::config::Types<2u,float,false>::StiffnessTensor& C,
							    const float* const props)
  {
    CalculiXComputeOrthotropicPlaneStressAlteredStiffnessTensorImpl(C,props);
  } // end of struct CalculiXComputeOrthotropicPlaneStressAlteredStiffnessTensor

  void
  CalculiXComputeOrthotropicStiffnessTensor3D(tfel::config::Types<3u,float,false>::StiffnessTensor& C,
					    const float* const props)
  {
    CalculiXComputeOrthotropicStiffnessTensor3DImpl(C,props);
  } // end of struct CalculiXComputeStiffnessTensor

  void
  CalculiXComputeIsotropicStiffnessTensor2D(tfel::config::Types<2u,double,false>::StiffnessTensor& C,
					  const double* const props)
  {
    CalculiXComputeIsotropicStiffnessTensor2DImpl(C,props);
  }

  void
  CalculiXComputeIsotropicPlaneStressAlteredStiffnessTensor(tfel::config::Types<2u,double,false>::StiffnessTensor& C,
							  const double* const props)
					       
  {
    CalculiXComputeIsotropicPlaneStressAlteredStiffnessTensorImpl(C,props);
  } // end of struct CalculiXComputeIsotropicPlaneStressAlteredStiffnessTensor

  void
  CalculiXComputeIsotropicStiffnessTensor3D(tfel::config::Types<3u,double,false>::StiffnessTensor& C,
					  const double* const props)
  {
    CalculiXComputeIsotropicStiffnessTensor3DImpl(C,props);
  } // end of struct CalculiXComputeStiffnessTensor

  void
  CalculiXComputeOrthotropicStiffnessTensor2D(tfel::config::Types<2u,double,false>::StiffnessTensor& C,
					    const double* const props)
  {
    CalculiXComputeOrthotropicStiffnessTensor2DImpl(C,props);
  } // end of struct CalculiXComputeStiffnessTensor

  void
  CalculiXComputeOrthotropicPlaneStressAlteredStiffnessTensor(tfel::config::Types<2u,double,false>::StiffnessTensor& C,
							    const double* const props)
  {
    CalculiXComputeOrthotropicPlaneStressAlteredStiffnessTensorImpl(C,props);
  } // end of struct CalculiXComputeOrthotropicPlaneStressAlteredStiffnessTensor

  void
  CalculiXComputeOrthotropicStiffnessTensor3D(tfel::config::Types<3u,double,false>::StiffnessTensor& C,
					    const double* const props)
  {
    CalculiXComputeOrthotropicStiffnessTensor3DImpl(C,props);
  } // end of struct CalculiXComputeStiffnessTensor
  
} // end of namespace calculix
