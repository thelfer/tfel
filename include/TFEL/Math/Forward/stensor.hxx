/*! 
 * \file  include/TFEL/Math/Forward/stensor.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 19 sept. 2011
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_TFEL_MATH_FORWARD_STENSOR_H_
#define _LIB_TFEL_MATH_FORWARD_STENSOR_H_ 

namespace tfel
{

  namespace math
  {

    template<class T>
    class StensorConcept ;

    template<class T>
    struct StensorTraits;

    template<unsigned short N, typename T>
    struct StensorNullStorage;

    template<unsigned short N, typename T>
    struct StensorStatic;

    template<unsigned short N, typename T>
    class StensorExternalTab;

    template<unsigned short N, typename T>
    class StensorExternalVoigt;

    /*
     * \class stensor
     * \brief finite size symmetric tensor.
     * \param N, the spatial dimension, see StensorDimeToSize for details. 
     * \param T, numerical type used, by default, double
     * \param Storage, type of storage used
     * \pre   This class is only defined for N=1u,2u and 3u.
     * \see   StensorDimeToSize and StensorSizeToDime. 
     * \see   StensorStatic, StensorExternalTab,StensorExternalVoigt.
     */
    template<unsigned short N,typename T=double,
	     template<unsigned short,typename> class Storage=StensorStatic>
    struct stensor;

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_MATH_FORWARD_STENSOR_H */

