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
#include"MFront/VariableModifier.hxx"
#include"MFront/WordAnalyser.hxx"

namespace mfront
{

  struct TFEL_VISIBILITY_EXPORT ParserBase
    : public tfel::utilities::CxxTokenizer,
      protected MFrontGenericData
  {
    
  protected:

    ParserBase();

    virtual void
    analyseFile(const std::string&) = 0;

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

    /*!
     * read an unsigned short in the file
     * \param[in] m : calling method
     */
    unsigned short
    readUnsignedShort(const std::string&);

    /*
     * \param std::string&, first result
     * \param std::string&, second result
     * \param tfel::utilities::shared_ptr<VariableModifier>,
     * first modifier of variable names
     * \param tfel::utilities::shared_ptr<VariableModifier>, 
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
		  tfel::utilities::shared_ptr<VariableModifier>,
		  tfel::utilities::shared_ptr<VariableModifier>,
		  const bool = false,
		  const std::string = "{",
		  const std::string = "}",
		  const bool = true,
		  const bool = true);

    /*
     * \param tfel::utilities::shared_ptr<VariableModifier>,
     * modifier of variable names
     * \param const bool, add "this->" before variable names
     * \param const std::string, first delimiter
     * \param const std::string, second delimiter
     * \param const bool, allow ';' in the block
     * \param const bool, add line number between lines
     */
    std::string
    readNextBlock(tfel::utilities::shared_ptr<VariableModifier>,
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
     * \param tfel::utilities::shared_ptr<VariableModifier>,
     * modifier of variable names
     */
    std::string
    readNextBlock(const bool = false,
		  const std::string = "{",
		  const std::string = "}",
		  const bool = true,
		  const bool = true,
		  tfel::utilities::shared_ptr<VariableModifier> =
		  tfel::utilities::shared_ptr<VariableModifier>(),
		  tfel::utilities::shared_ptr<WordAnalyser> =
		  tfel::utilities::shared_ptr<WordAnalyser>());

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
    virtual void
    treatMFront(void);
    virtual void
    treatImport(void);
    virtual void
    treatMaterialLaw(void);
    virtual void
    treatLink(void);
    virtual void
    treatAuthor(void);
    virtual void
    treatUnknownKeyword(void);
    virtual void
    treatDate(void);
    virtual void
    treatIncludes(void);
    virtual void
    treatSources(void);
    virtual void
    treatPrivate(void);
    virtual void
    treatMembers(void);
    virtual void
    treatParser(void);
    virtual void
    treatStaticVar(void);
    virtual void
    treatDescription(void);
    virtual void
    treatMaterial(void);
    virtual void
    treatLibrary(void);
    virtual void
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

