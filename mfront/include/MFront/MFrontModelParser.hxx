/*! 
 * \file  mfront/include/MFront/MFrontModelParser.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 11 jun 2010
 */

#ifndef _LIB_MFRONTMODELPARSER_H_
#define _LIB_MFRONTMODELPARSER_H_ 

#include"MFront/MFrontModelParserBase.hxx"

namespace mfront{

  struct MFrontModelParser
    : public MFrontModelParserBase<MFrontModelParser>
  {

    static std::string
    getName(void);

    static std::string
    getDescription(void);

    virtual void
    treatFile(const std::string&,
	      const std::vector<std::string>&);

    virtual void
    setInterfaces(const std::set<std::string>&);

  }; // end of MFrontModelParser

} // end of namespace mfront

#endif /* _LIB_MFRONTMODELPARSER_H */

