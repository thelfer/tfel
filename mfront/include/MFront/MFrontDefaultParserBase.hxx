/*!
 * \file   mfront/include/MFront/MFrontDefaultParserBase.hxx
 * \brief  
 * \author Helfer Thomas
 * \date   08 nov 2006
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_MFRONT_MFRONTPARSERBASE_HXX_
#define _LIB_MFRONT_MFRONTPARSERBASE_HXX_ 

#include"MFront/MFrontBehaviourParserBase.hxx"

namespace mfront{

  struct MFrontDefaultParserBase
    : public MFrontBehaviourParserBase<MFrontDefaultParserBase>
  {

    MFrontDefaultParserBase();

    ~MFrontDefaultParserBase();

  protected:

    virtual void
    writeBehaviourParserSpecificIncludes(void);
    
    virtual void
    treatProvidesTangentOperator(void);

    virtual void
    treatProvidesSymmetricTangentOperator(void);

  }; // end of struct MFrontDefaultParserBase

} // end of namespace mfront  

#endif /* _LIB_MFRONT_MFRONTPARSERBASE_HXX */

