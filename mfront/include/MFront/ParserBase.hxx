/*!
 * \file   ParserBase.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   04 jun 2007
 */

#ifndef _LIB_MFRONT_PARSERBASE_HXX_
#define _LIB_MFRONT_PARSERBASE_HXX_ 

#include<set>
#include<map>
#include<string>

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/Utilities/CxxTokenizer.hxx"

#include"MFront/MFrontGenericData.hxx"
#include"MFront/VariableModifier.hxx"
#include"MFront/WordAnalyser.hxx"
#include"MFront/VariableDescription.hxx"
#include"MFront/StaticVariableDescription.hxx"

namespace mfront
{

  /*!
   * base structure for parsers
   */
  struct TFEL_VISIBILITY_EXPORT ParserBase
    : public tfel::utilities::CxxTokenizer,
      public MFrontGenericData
  {

    /*!
     * \brief open a file and add given external instructions at the
     * beginning
     * \param[in] f     : file
     * \param[in] ecmds : external commands
     */
    virtual void
    openFile(const std::string&,
	     const std::vector<std::string>&);
    
  protected:

    /*!
     * constructor
     */
    ParserBase();
    /*!
     * \brief analyse a file
     * \param[in] f     : file name
     * \param[in] ecmds : additionnal commands inserted treated before
     * the input file commandes (those commands are given through the
     * --@?? option of the command line
     */
    virtual void
    analyseFile(const std::string&,
		const std::vector<std::string>&) = 0;
    /*!
     * \brief add a static variable description
     * \param[in] v : variable description
     */
    virtual void
    addStaticVariableDescription(const StaticVariableDescription&) = 0;
    /*!
     * destructor
     */
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
    readVarList(VariableDescriptionContainer&,
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
    readVarList(VariableDescriptionContainer&,
		const bool,
		const bool);
    /*!
     * extract a string from the current token and go the next token
     * \param[in] m : calling method name (used for error message)
     * \return the extracted string
     */
    std::string
    readString(const std::string&);
    /*!
     * extract an array of string starting from the current token and
     * go the token following the end of the array
     * \param[in] m : calling method name (used for error message)
     * \return the extracted array of strings
     */
    std::vector<std::string>
    readArrayOfString(const std::string&);
    /*!
     * extract an array of string starting from the current token and
     * go the token following the end of the array
     * \param[in] m : calling method name (used for error message)
     * \return the extracted array of strings which contains only one
     * element if a string was read
     */
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
    treatIntegerConstant(void);
    virtual void
    treatDescription(void);
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
    /*!
     * \brief read parameters declaration
     * \param[in] c : parameters container
     * \param[in] v : parameters default values
     */
    void
    handleParameter(VariableDescriptionContainer&,
		    std::map<std::string,double>&);
    
    std::vector<std::string> librariesDependencies;
    std::map<std::string,int> integerConstants;
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

