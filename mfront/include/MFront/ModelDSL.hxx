/*! 
 * \file  mfront/include/MFront/ModelDSL.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 11 jun 2010
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_MFRONTMODELPARSER_H_
#define LIB_MFRONTMODELPARSER_H_ 

#include"MFront/ModelDSLBase.hxx"

namespace mfront{

  struct ModelDSL
    : public ModelDSLBase<ModelDSL>
  {

    static std::string
    getName(void);

    static std::string
    getDescription(void);

    virtual void
    setInterfaces(const std::set<std::string>&);
  }; // end of ModelDSL

} // end of namespace mfront

#endif /* LIB_MFRONTMODELPARSER_H_ */

