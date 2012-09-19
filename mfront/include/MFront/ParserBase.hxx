/*!
 * \file   ParserBase.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   04 jun 2007
 */

#ifndef _LIB_MFRONT_PARSERBASE_HXX_
#define _LIB_MFRONT_PARSERBASE_HXX_ 

#include<string>
#include<set>

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/Utilities/CxxTokenizer.hxx"

#include"MFront/VarHandler.hxx"
#include"MFront/StaticVarHandler.hxx"
#include"MFront/MFrontGenericData.hxx"

namespace mfront
{

  struct TFEL_VISIBILITY_EXPORT ParserBase
    : public tfel::utilities::CxxTokenizer,
      protected MFrontGenericData
  {
    virtual std::string variableModifier1(const std::string&,const bool);

    virtual std::string variableModifier2(const std::string&,const bool);

    virtual std::string variableModifier3(const std::string&,const bool);

    virtual std::string variableModifier4(const std::string&,const bool);

    virtual std::string variableModifier5(const std::string&,const bool);
    
  protected:

    ParserBase();

    virtual ~ParserBase();

    void
    checkNotEndOfFile(const std::string&,
		      const std::string& = "");

    void
    readSpecifiedToken(const std::string&,
		       const std::string&);

    void
    throwRuntimeError(const std::string&,
		      const std::string&);

    std::string
    readUntilEndOfInstruction(void);

    /*
     * \param std::string&, first result
     * \param std::string&, second result
     * \param std::string (ParserBase::*)(const std::string&,const bool),
     * first modifier of variable names
     * \param std::string (ParserBase::*)(const std::string&,const bool), 
     * second modifier of variable names 
     * \param const bool, add "this->" before variable names
     * \param const std::string, first delimiter
     * \param const std::string, second delimiter
     * \param const bool, allow ';' in the block
     * \param const bool, add line number between lines
     */
    void
    readNextBlock(std::string&,
		  std::string&,
		  std::string (ParserBase:: *)(const std::string&,const bool),
		  std::string (ParserBase:: *)(const std::string&,const bool),
		  const bool = false,
		  const std::string = "{",
		  const std::string = "}",
		  const bool = true,
		  const bool = true);

    /*
     * \param std::string (ParserBase::*)(const std::string&,const bool),
     * modifier of variable names
     * \param const bool, add "this->" before variable names
     * \param const std::string, first delimiter
     * \param const std::string, second delimiter
     * \param const bool, allow ';' in the block
     * \param const bool, add line number between lines
     */
    std::string
    readNextBlock(std::string (ParserBase:: *)(const std::string&,const bool),
		  const bool = false,
		  const std::string = "{",
		  const std::string = "}",
		  const bool = true,
		  const bool = true);

    /*
     * \param const bool, add "this->" before variable names
     * \param const std::string, first delimiter
     * \param const std::string, second delimiter
     * \param const bool, allow ';' in the block
     * \param const bool, add line number between lines
     * \param std::string (ParserBase::*)(const std::string&,const bool),
     * modifier of variable names
     */
    std::string
    readNextBlock(const bool = false,
		  const std::string = "{",
		  const std::string = "}",
		  const bool = true,
		  const bool = true,
		  std::string (ParserBase::*)(const std::string&,const bool) = 0);

    std::string
    readOnlyOneToken(void);

    /*!
     * \param[in] cont : variable container to wich variables are
     * added
     * \param[in] type : type of the variable
     * \param[in] allowArray      : allow arrays of variables to be defined
     * \param[in] addIncrementVar : for each variable read, add
     * another variable standing for the first variable increment
     */
    void
    readVarList(VarContainer&,
		const std::string&,
		const bool,
		const bool);

    /*!
     * \param[in] cont : variable container to wich variables are
     * added
     * \param[in] allowArray      : allow arrays of variables to be defined
     * \param[in] addIncrementVar : for each variable read, add
     * another variable standing for the first variable increment
     */
    void
    readVarList(VarContainer&,
		const bool,
		const bool);

    std::vector<std::string>
    readArrayOfString(const std::string&);

    std::vector<std::string>
    readStringOrArrayOfString(const std::string&);

    std::string
    readSpecifiedValue(const std::string&,
		       const std::string&);

    std::vector<std::string>
    readSpecifiedValues(const std::string&,
			const std::string&,
			const std::string&);
    
    std::vector<std::string>
    readSpecifiedValues(const std::string&,
			const std::vector<std::string>&);

    void
    treatMFront(void);
    void
    treatMaterialLaw(void);
    void
    treatLink(void);
    void
    treatAuthor(void);
    void
    treatUnknownKeyword(void);
    void
    treatDate(void);
    void
    treatIncludes(void);
    void
    treatSources(void);
    void
    treatPrivate(void);
    void
    treatMembers(void);
    void
    treatParser(void);
    void
    treatStaticVar(void);
    void
    treatDescription(void);
    void
    treatMaterial(void);
    void
    treatLibrary(void);
    void
    ignoreKeyWord(const std::string&);
    void
    registerVariable(const std::string&);
    void
    registerStaticVariable(const std::string&);
    void
    reserveName(const std::string&);
    double
    readDouble(void);
    std::vector<std::string> librariesDependencies;
    std::string sources;
    std::string privateCode;
    std::string members;
    TokensContainer::const_iterator current;
    bool debugMode;
    bool verboseMode;
    bool warningMode;
  }; // end of class ParserBase

} // end of namespace mfront  

#endif /* _LIB_MFRONT_PARSERBASE_HXX */

