/*!
 * \file   tests/Math/include/CubicRoots-header.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   04 fév 2008
 */

#ifndef _LIB_TFEL_CUBICROOTS_HEADER_HXX_
#define _LIB_TFEL_CUBICROOTS_HEADER_HXX_

double
cubic(const double,const double,const double,const double, const double);

void
generate_coef(double&,double&,double&,double&,const double,const double,const double);

void
solve(const double,const double,const double,const double);

#endif /* _LIB_TFEL_CUBICROOTS-HEADER_HXX */

