/*!
 * \file   tests/Math/include/CubicRoots-header.hxx
 * \brief
 *
 * \author Thomas Helfer
 * \date   04/02/2008
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_CUBICROOTS_HEADER_HXX
#define LIB_TFEL_CUBICROOTS_HEADER_HXX

double cubic(
    const double, const double, const double, const double, const double);

void generate_coef(double&,
                   double&,
                   double&,
                   double&,
                   const double,
                   const double,
                   const double);

void solve(const double, const double, const double, const double);

#endif /* LIB_TFEL_CUBICROOTS-HEADER_HXX */
