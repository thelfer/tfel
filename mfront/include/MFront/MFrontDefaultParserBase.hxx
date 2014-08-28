/*!
 * \file   mfront/include/MFront/MFrontDefaultParserBase.hxx
 * \brief  
 * \author Helfer Thomas
 * \date   08 nov 2006
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

