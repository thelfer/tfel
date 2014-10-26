/*! 
 * \file  include/TFEL/Material/DDIF2Base.hxx
 * \brief
 * \author Helfer Thomas
 * \date 12 mar 2010
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef   _LIB_TFEL_MATERIAL_DDIF2BASE_H_
#define   _LIB_TFEL_MATERIAL_DDIF2BASE_H_ 

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/Math/tvector.hxx"
#include"TFEL/Math/tmatrix.hxx"
#include"TFEL/Math/stensor.hxx"

namespace tfel
{

  namespace material
  {
    
    struct TFELMATERIAL_VISIBILITY_EXPORT DDIF2Base
    {

      template<typename Stensor2Type1,
	       typename Stensor,
	       typename real>
      static TFEL_VISIBILITY_LOCAL void
      treatFracture(Stensor2Type1& dfe_ddeel,
		    real& dfe_dde,
		    real& fe,
		    const real em_1,
		    const real e,
		    const real de,
		    const Stensor& sig,
		    const Stensor& n,
		    const real sigr,
		    const real Rp,
		    const real young,
		    const real lambda,
		    const real mu);

      template<typename real>
      static TFEL_VISIBILITY_LOCAL void
      cart2cyl(tfel::math::stensor<1u,real>&,
	       const real);

      template<typename real>
      static TFEL_VISIBILITY_LOCAL void
      cart2cyl(tfel::math::stensor<2u,real>&,
	       const real);

      template<typename real>
      static TFEL_VISIBILITY_LOCAL void
      cart2cyl(tfel::math::stensor<3u,real>&,
	       const real);

      template<typename real>
      static TFEL_VISIBILITY_LOCAL void
      cyl2cart(tfel::math::stensor<1u,real>&,
	       const real);

      template<typename real>
      static TFEL_VISIBILITY_LOCAL void
      cyl2cart(tfel::math::stensor<2u,real>&,
	       const real);

      template<typename real>
      static TFEL_VISIBILITY_LOCAL void
      cyl2cart(tfel::math::stensor<3u,real>&,
	       const real);

      template<typename real>
      static TFEL_VISIBILITY_LOCAL void
      rk(tfel::math::tmatrix<3,3,real>&,
	 tfel::math::tvector<3,real>&,
	 real&,
	 const real,
	 const real,
	 const real,
	 const real,
	 const real,
	 const real,
	 const real,
	 const unsigned short,
	 const unsigned short,
	 const unsigned short);

      template<typename real>
      static TFEL_VISIBILITY_LOCAL void
      invmat(tfel::math::tmatrix<3,3,real>&,
             tfel::math::tmatrix<3,3,real>&,
	     tfel::math::tvector<3,real>&);

      //! small parameter which guarantees that Ef is finite
      static const double emin1;
      //! small parameter which guarantees that Ef is finite
      static const double emin_rk1;
      //! small numerical parameter
      static const double eps;

    }; // end of struct DDIF2Base
    
  } // end of namespace material

} // end of namespace tfel

#include"TFEL/Material/DDIF2Base.ixx"

#endif /* _LIB_TFEL_MATERIAL_DDIF2BASE_H */

