/*!
 * \file   Hill.hxx
 * \author Helfer Thomas
 * \date   26 March 2009
 */

#include"Math/st2tost2.hxx"

#ifndef _LIB_TFEL_MATERIAL_HILL_H_
#define _LIB_TFEL_MATERIAL_HILL_H_ 

namespace tfel
{

  namespace material
  {

    /*!
     * \param F :
     * \param G :
     * \param H :
     * \param L :
     * \param M :
     * \param N :
     * build the Hill Tensor :
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
     * \f$\sigma|H*\sigma\$ computes the Hill stress :
     * \[
     * F\left(\sigma_{11}-\sigma_{22}\right)^2+G\left(\sigma_{22}-\sigma_{33}\right)^2+H\left(\sigma_{33}-\sigma_{11}\right)^2
     * + 2L\sigma_{12}^{2} + 2M\sigma_{13}^{2} + 2N\sigma_{23}^{2}
     * \]
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

#include"Material/Hill.ixx"

#endif /* _LIB_TFEL_MATERIAL_HILL_H_ */
