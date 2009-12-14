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

#include"Utilities/CxxTokenizer.hxx"
#include"VarHandler.hxx"
#include"StaticVarHandler.hxx"

namespace mfront
{

  struct ParserBase
    : public tfel::utilities::CxxTokenizer
  {
    virtual std::string variableModifier1(const std::string&,const bool);

    virtual std::string variableModifier2(const std::string&,const bool);

    virtual std::string variableModifier3(const std::string&,const bool);

    virtual std::string variableModifier4(const std::string&,const bool);

    virtual std::string variableModifier5(const std::string&,const bool);
    
  protected:

    static
    std::string
    toString(const unsigned short);

    static
    std::string
    makeUpperCase(const std::string&);

    static
    std::string 
    makeLowerCase(const std::string&);

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

    void
    readVarList(const std::string&,VarContainer&,const bool = false);

    void
    readVarList(VarContainer&,const bool = false);

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
    treatLocalVar(void);
    void
    treatInitLocalVars(void);
    void
    treatMaterial(void);
    void
    treatLibrary(void);
    void
    ignoreKeyWord(const std::string&);
    double
    readDouble(void);
    VarContainer localVarsHolder;
    StaticVarContainer staticVars;
    std::vector<std::string> librariesDependencies;
    std::vector<std::string> materialLaws;
    std::set<std::string> varNames;
    std::set<std::string> typeNames;
    std::set<std::string> staticVarNames;
    std::string initLocalVars;
    std::string fileName;    
    std::string authorName;
    std::string date;
    std::string includes;
    std::string sources;
    std::string privateCode;
    std::string members;
    std::string className;
    std::string material;
    std::string library;
    std::string description;
    TokensContainer::const_iterator current;
    bool debugMode;
    bool verboseMode;
    bool warningMode;
  }; // end of class ParserBase

} // end of namespace mfront  

#endif /* _LIB_MFRONT_PARSERBASE_HXX */

