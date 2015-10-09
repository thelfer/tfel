/*! 
 * \file  mfront/include/MFront/Abaqus/Abaqus.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 24 janv. 2013
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_MFRONT_ABAQUS_H_
#define LIB_MFRONT_ABAQUS_H_ 

#ifndef TFEL_ABAQUS_INLINE
#define TFEL_ABAQUS_INLINE inline
#endif /* LIB_MFRONT_ABAQUS_H_ */

#ifndef TFEL_ABAQUS_INLINE2
#define TFEL_ABAQUS_INLINE2 inline
#endif /* LIB_MFRONT_ABAQUS_H_ */

namespace abaqus{

  using AbaqusInt  = int;
  using AbaqusReal = double;

  /*!
   * prototype of a function handling stress-free expansion at the
   * beginning of the time step
   *
   * - first  paramater : driving variable
   * - second paramater : driving variable at the end of the time step
   *   or driving variable increment
   * - third paramater  : expansion value at the beginning of the time
   *   step
   * - fourth paramater : expansion value at the end of the time step
   * - third  parameter : spatial dimension
   */
  typedef void (*StressFreeExpansionHandler)(AbaqusReal * const,
					     AbaqusReal * const,
					     const AbaqusReal *const,
					     const AbaqusReal *const,
					     const AbaqusInt);

} // end of namespace abaqus

#endif /* LIB_MFRONT_ABAQUS_H_ */

