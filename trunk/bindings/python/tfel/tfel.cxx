/*!
 * \file   tfel.cxx
 * \brief    
 * \author Thomas Helfer
 * \date   18 août 2017
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<boost/python.hpp>

void declarePhysicalConstants();

BOOST_PYTHON_MODULE(_tfel)
{
  declarePhysicalConstants();
}
