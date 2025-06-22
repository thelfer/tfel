/*!
 * \file   include/TFEL/Material/PiPlane.hxx
 * \brief
 * \author Thomas Helfer
 * \date   14/12/2017
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATERIAL_PIPLANE_HXX
#define LIB_TFEL_MATERIAL_PIPLANE_HXX

#include <tuple>
#include "TFEL/Math/tvector.hxx"
#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/General/MathConstants.hxx"

namespace tfel {

  namespace material {

    /*!
     * \brief this function project a stress state defined by the the
     * principal stress axes \f$\left(S_{1},S_{2},S_{3}\right)\f$ on
     * the \f$\pi\f$-plane, which is defined by:
     * \f[
     * S_{1}+S_{2}+S_{3}=0
     * \f]
     * A basis of the \f$\pi\f$-plane is defined by the following vectors:
     * \f[
     * \vec{n}_{0}=
     * \frac{1}{\sqrt{2}}\,
     * \begin{pmatrix}
     * 1  \\
     * -1 \\
     * 0
     * \end{pmatrix}
     * \quad\text{and}\quad
     * \vec{n}_{1}=
     * \frac{1}{\sqrt{6}}\,
     * \begin{pmatrix}
     * -1 \\
     * -1 \\
     *  2
     * \end{pmatrix}
     * \f]
     * \tparam stress: stress type
     * \param[in] s: eigen values of the stress
     * \return the projection of the stresses on the \f$\pi\f$-plane
     */
    template <typename stress>
    std::tuple<stress, stress> projectOnPiPlane(
        const tfel::math::tvector<3u, stress>&);
    /*!
     * \brief this function project a stress state defined by the the
     * principal stress axes \f$\left(S_{0},S_{1},S_{2}\right)\f$ on
     * the \f$\pi\f$-plane, which is defined by:
     * \f[
     * S_{0}+S_{1}+S_{2}=0
     * \f]
     * A basis of the \f$\pi\f$-plane is defined by the following vectors:
     * \f[
     * \vec{n}_{0}=
     * \frac{1}{\sqrt{2}}\,
     * \begin{pmatrix}
     * 1  \\
     * -1 \\
     * 0
     * \end{pmatrix}
     * \quad\text{and}\quad
     * \vec{n}_{1}=
     * \frac{1}{\sqrt{6}}\,
     * \begin{pmatrix}
     * -1 \\
     * -1 \\
     *  2
     * \end{pmatrix}
     * \f]
     * \tparam stress: stress type
     * \param[in] s0: first eigen value
     * \param[in] s1: second eigen value
     * \param[in] s2: third eigen value
     * \return the projection of the stresses on the \f$\pi\f$-plane
     */
    template <typename stress>
    std::tuple<stress, stress> projectOnPiPlane(const stress,
                                                const stress,
                                                const stress);
    /*!
     * \brief this function project a stress state defined by the the
     * principal stress axes \f$\left(S_{0},S_{1},S_{2}\right)\f$ on
     * the \f$\pi\f$-plane, which is defined by:
     * \f[
     * S_{0}+S_{1}+S_{2}=0
     * \f]
     * A basis of the \f$\pi\f$-plane is defined by the following vectors:
     * \f[
     * \vec{n}_{0}=
     * \frac{1}{\sqrt{2}}\,
     * \begin{pmatrix}
     * 1  \\
     * -1 \\
     * 0
     * \end{pmatrix}
     * \quad\text{and}\quad
     * \vec{n}_{1}=
     * \frac{1}{\sqrt{6}}\,
     * \begin{pmatrix}
     * -1 \\
     * -1 \\
     *  2
     * \end{pmatrix}
     * \f]
     * \tparam StressStensor: stress tensor type
     * \param[in] s: stress tensor
     * \return the projection of the stresses on the \f$\pi\f$-plane
     */
    template <typename StressStensor,
              tfel::math::stensor_common::EigenSolver es =
                  tfel::math::stensor_common::TFELEIGENSOLVER>
    std::tuple<tfel::math::StensorNumType<StressStensor>,
               tfel::math::StensorNumType<StressStensor>>
    projectOnPiPlane(const StressStensor&);
    /*!
     * \brief this function returns the stress state
     * \f$s_{0}\,\vec{n}_{0}+s_{0}\,\vec{n}_{1}\f$, defined by three
     * eigenvalues, where \f$\vec{n}_{0}\f$ and \f$\vec{n}_{1}\f$ is
     * the basis of the \f$\pi\f$-plane defined as follows:
     * \f[
     * \vec{n}_{0}=
     * \frac{1}{\sqrt{2}}\,
     * \begin{pmatrix}
     * 1  \\
     * -1 \\
     * 0
     * \end{pmatrix}
     * \quad\text{and}\quad
     * \vec{n}_{1}=
     * \frac{1}{\sqrt{6}}\,
     * \begin{pmatrix}
     * -1 \\
     * -1 \\
     *  2
     * \end{pmatrix}
     * \f]
     * \tparam stress: stress type
     * \param[in] s0: first position in the \f$\pi\f$-plane.
     * \param[in] s1: second position in the \f$\pi\f$-plane.
     * \return a stress state corresponding to the point in the
     * \f$\pi\f$-plane.
     */
    template <typename stress>
    std::tuple<stress, stress, stress> buildFromPiPlane(const stress,
                                                        const stress);

  }  // end of namespace material

}  // end of namespace tfel

#include "TFEL/Material/PiPlane.ixx"

#endif /* LIB_TFEL_MATERIAL_PIPLANE_HXX */
