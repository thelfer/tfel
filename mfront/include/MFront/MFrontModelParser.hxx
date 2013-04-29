/*! 
 * \file  MFrontModelParser.hxx
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

    /*!
     * \brief return the list of keywords usable with this parser
     * \param[out] k : the list of keywords registred for this parser
     */
    virtual void
    getKeywordsList(std::vector<std::string>&) const;

    virtual void
    treatFile(const std::string&);

    virtual void
    analyseFile(const std::string&);

    virtual void
    setInterfaces(const std::set<std::string>&);

  }; // end of MFrontModelParser

} // end of namespace mfront

#endif /* _LIB_MFRONTMODELPARSER_H */

