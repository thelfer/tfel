/*!
 * \file   include/TFEL/Material/Hill.hxx
 * \author Helfer Thomas
 * \date   26 March 2009
 * \brief  This file declares the hillTensor function.
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include"TFEL/Math/st2tost2.hxx"

#ifndef LIB_TFEL_MATERIAL_HILL_H_
#define LIB_TFEL_MATERIAL_HILL_H_ 

namespace tfel
{

  namespace material
  {

    /*!
     * \brief compute the Hill tensor.
     * \tparam    N : space dimension
     * \tparam    T : underlying type
     * \param[in] F : orthotropic coefficient
     * \param[in] G : orthotropic coefficient
     * \param[in] H : orthotropic coefficient
     * \param[in] L : orthotropic coefficient
     * \param[in] M : orthotropic coefficient
     * \param[in] N : orthotropic coefficient
     * This function builds the Hill Tensor defined by :
     * \f[\left(
     * \begin{array}{cccccc}
     * F+H & -F  & -H  & 0 & 0 & 0 \\
     * -F  & G+F & -G  & 0 & 0 & 0 \\
     * -H  & -G  & H+G & 0 & 0 & 0 \\
     * 0   & 0   & 0   & L & 0 & 0 \\
     * 0   & 0   & 0   & 0 & M & 0 \\
     * 0   & 0   & 0   & 0 & 0 & N \\
     * \end{array}
     * \right)
     * \f]
     * if \f$\sigma\f$ is a second order symetric tensor (stensor), 
     * \f$\sigma|H*\sigma\f$ computes the Hill stress :
     * \f[
     * F\left(\sigma_{11}-\sigma_{22}\right)^2+G\left(\sigma_{22}-\sigma_{33}\right)^2+H\left(\sigma_{33}-\sigma_{11}\right)^2
     * + 2L\sigma_{12}^{2} + 2M\sigma_{13}^{2} + 2N\sigma_{23}^{2}
     * \f]
     *
     * \warning This convention is given in the book of Lemaître et
     * Chaboche and seems to differ from the one described in most
     * other books.
     *
     * \return an object of type st2tost2
     */    
    template<unsigned short N,
	     typename T>
    tfel::math::st2tost2<N,T>
    hillTensor(const T,
	       const T,
	       const T,
	       const T,
	       const T,
	       const T);

  } // end of namespace material

} // end of namespace tfel

#include"TFEL/Material/Hill.ixx"

#endif /* LIB_TFEL_MATERIAL_HILL_H_ */
