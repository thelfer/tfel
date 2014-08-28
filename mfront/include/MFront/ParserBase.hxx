/*!
 * \file   mfront/include/MFront/ParserBase.hxx
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
#include"TFEL/Utilities/SmartPtr.hxx"
#include"TFEL/Utilities/CxxTokenizer.hxx"

#include"MFront/CodeBlock.hxx"
#include"MFront/VariableDescription.hxx"
#include"MFront/MFrontFileDescription.hxx"
#include"MFront/MaterialPropertyDescription.hxx"
#include"MFront/StaticVariableDescription.hxx"

namespace mfront
{

  /*!
   * base structure for parsers
   */
  struct TFEL_VISIBILITY_EXPORT ParserBase
    : public tfel::utilities::CxxTokenizer,
      public MFrontFileDescription
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
    /*!
     * \return generic data about the MFront file being treated
     */
    virtual const MFrontFileDescription&
    getMFrontFileDescription(void) const;

  protected:
    /*!
     * \brief An helper structure used by the
     * MFrontBehaviourParserCommon class to modify the code provided
     * by the user.
     * 
     * These modifiers are called when the parser encounters variables.
     */
    struct TFEL_VISIBILITY_EXPORT VariableModifier
    {
      /*!
       * \param[in] v : the variable name
       * \param[in] b : true if "this" shall be added
       */
      virtual std::string
      exe(const std::string&,
	  const bool) = 0;
      /*!
       * destructor
       */
      virtual ~VariableModifier() = 0;
    }; // end of struct VariableModifier
    /*!
     * \brief An helper structure used by the ParserBase class to modify the
     * code provided by the user.
     * 
     * These modifiers are called when the parser encounters variables.
     */
    struct TFEL_VISIBILITY_EXPORT WordAnalyser
    {
      /*!
       * \param[in] k : the current word
       */
      virtual void
      exe(const std::string&) = 0;
      /*!
       * destructor
       */
      virtual ~WordAnalyser() = 0;
    }; // end of struct WordAnalyser
    /*!
     * constructor
     */
    ParserBase();
    /*!
     * \return the name of the generated class
     */
    virtual std::string getClassName(void) const = 0;
    /*!
     * \brief add a material law
     * \param[in] m : added material law name
     */
    virtual void addMaterialLaw(const std::string&) = 0;
    /*!
     * \brief append the given code to the includes
     */
    virtual void appendToIncludes(const std::string&) = 0;
    /*!
     * \brief append the given code to the members
     */
    virtual void appendToMembers(const std::string&) = 0;
    /*!
     * \brief append the given code to the private code
     */
    virtual void appendToPrivateCode(const std::string&) = 0;
    /*!
     * \brief append the given code to the sources
     */
    virtual void appendToSources(const std::string&) = 0;
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
		      const std::string& = "") const;

    void
    readSpecifiedToken(const std::string&,
		       const std::string&);

    void
    throwRuntimeError(const std::string&,
		      const std::string&) const;

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
    readNextBlock(CodeBlock&,
		  CodeBlock&,
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
    CodeBlock
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
    CodeBlock
    readNextBlock(const bool = false,
		  const std::string = "{",
		  const std::string = "}",
		  const bool = true,
		  const bool = true,
		  tfel::utilities::shared_ptr<VariableModifier> =
		  tfel::utilities::shared_ptr<VariableModifier>(),
		  tfel::utilities::shared_ptr<WordAnalyser> =
		  tfel::utilities::shared_ptr<WordAnalyser>());
    /*!
     * \brief read one token and increment "current"
     */
    std::string
    readOnlyOneToken(void);
    /*!
     *
     */
    template<typename T>
    std::pair<bool,T>
    readInitialisationValue(const std::string&,
			    const bool);
    /*!
     * \param[in] cont : variable container to wich variables are
     * added
     * \param[in] type : type of the variable
     * \param[in] allowArray      : allow arrays of variables to be defined
     * \param[in] addIncrementVar : for each variable read, add
     * another variable standing for the first variable increment
     * \param[in] b : accept that a variable with the same name was
     * already declared
     */
    void
    readVarList(VariableDescriptionContainer&,
		const std::string&,
		const bool,
		const bool,
		const bool);
    /*!
     * \param[in] cont : variable container to wich variables are
     * added
     * \param[in] allowArray      : allow arrays of variables to be defined
     * \param[in] addIncrementVar : for each variable read, add
     * another variable standing for the first variable increment
     * \param[in] b : accept that a variable with the same name was
     * already declared
     */
    void
    readVarList(VariableDescriptionContainer&,
		const bool,
		const bool,
		const bool);
    /*!
     * extract a boolean value from the current token and go the next
     * token
     * \param[in] m : calling method name (used for error message)
     * \return the boolean value read
     */
    bool
    readBooleanValue(const std::string&);
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
    /*!
     * extract a list of token
     * \param[out] l  : list read
     * \param[in]  m  : calling method name (used for error message)
     * \param[in]  db : beginning delimiter
     * \param[in]  de : end delimiter
     * \param[in] b   : if the current value is not the beginning
     * delimiter (or is past the end of the file), nothing is returned
     * if this parameter is true. An exception is thrown otherwise
     */
    void
    readList(std::vector<tfel::utilities::Token>&,
	     const std::string&,
	     const std::string&,
	     const std::string&,
	     const bool);
    
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
    /*!
     * call mfront in a subprocess
     * \param[in] interfaces : list of interfaces
     * \param[in] files      : list of files
     */
    virtual void
    callMFront(const std::vector<std::string>&,
	       const std::vector<std::string>&) const;
    /*!
     * \brief This function handles a material property treated as a
     * dependency of the current file.
     *
     * This function:
     * - analyse the given file using the MFrontMaterialLawParser
     * - register the function name generated by the MFront interface
     * - add the function name to the list of material laws used by
     *   the current file
     * - declared the headers generated by the MFront interface for inclusion
     * - launch mfront in a sub-process to generate the source files
     *   associated with this material property and have them compiled
     *   when mandatory (done by the callMFront method).
     *
     * \param[in] f : file in which the material law is implemented
     */
    virtual MaterialPropertyDescription
    handleMaterialLaw(const std::string&);

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
    ignoreKeyWord(const std::string&);
    /*!
     * \brief register a variable name.
     * \param[in] v : variable name
     * \param[in] b : if false, don't check if variable has already
     * been reserved yet. If true, check if the variable has alredy
     * been registred and throws an exception if it does, register it
     * otherwise
     */
    virtual void
    registerVariable(const std::string&,
		     const bool);
    virtual void
    registerStaticVariable(const std::string&);
    /*!
     * \brief register a name.
     * \param[in] w : name
     * \param[in] b : if false, don't check if variable has already
     * been reserved yet. If true, check if the variable has alredy
     * been registred and throws an exception if it does, register it
     * otherwise

     * \note this method is called internally by the registerVariable
     *       and registerStaticVariable methods.
     */
    virtual void
    reserveName(const std::string&,
		const bool);
    /*!
     * \return true if the given name has been reserved
     * \param[in] n : name 
     */
    virtual bool
    isNameReserved(const std::string&) const;
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
    std::set<std::string> varNames;
    std::set<std::string> staticVarNames;
    std::set<std::string> reservedNames;

    TokensContainer::const_iterator current;
    /*!
     * \brief current comment
     * This is the user comment associated with the current keyword,
     * if any.
     * 
     * \note This variable is automatically set by parsers before
     * calling the callback associated with the keyword and
     * automatically unset once the callback returns.
     */
    std::string currentComment;

  }; // end of class ParserBase

} // end of namespace mfront  

#include"MFront/ParserBase.ixx"

#endif /* _LIB_MFRONT_PARSERBASE_HXX */

