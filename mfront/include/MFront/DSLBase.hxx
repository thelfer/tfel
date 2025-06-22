/*!
 * \file   mfront/include/MFront/DSLBase.hxx
 * \brief
 *
 * \author Thomas Helfer
 * \date   04 jun 2007
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_DSLBASE_HXX
#define LIB_MFRONT_DSLBASE_HXX

#include <set>
#include <map>
#include <tuple>
#include <memory>
#include <string>
#include "TFEL/Utilities/CxxTokenizer.hxx"
#include "MFront/MFrontConfig.hxx"
#include "MFront/CodeBlock.hxx"
#include "MFront/AbstractDSL.hxx"
#include "MFront/SupportedTypes.hxx"
#include "MFront/FileDescription.hxx"
#include "MFront/TargetsDescription.hxx"
#include "MFront/VariableDescription.hxx"
#include "MFront/StaticVariableDescription.hxx"

namespace mfront {

  // forward declaration
  struct MaterialPropertyDescription;

  /*!
   * \return if the given name is valid
   * \param[in] n: material name
   */
  MFRONT_VISIBILITY_EXPORT bool isValidMaterialName(std::string_view);
  /*!
   * \return if the given name is valid
   * \param[in] n: library name
   */
  MFRONT_VISIBILITY_EXPORT bool isValidLibraryName(std::string_view);
  /*!
   * \return if the given name, provided by the user, is valid.
   * \param[in] n: variable name
   *
   * The variable name must be a valid C++ variable and shall not start with
   * `mfront_`.
   */
  MFRONT_VISIBILITY_EXPORT bool isValidUserDefinedVariableName(
      std::string_view);

  /*!
   * \brief base structure for domain specific languages
   */
  struct MFRONT_VISIBILITY_EXPORT DSLBase
      : public virtual AbstractDSL,
        public tfel::utilities::CxxTokenizer {
    //! \brief standard option name
    static const char* const defaultOutOfBoundsPolicyOption;
    //! \brief standard option name
    static const char* const runtimeModificationOfTheOutOfBoundsPolicyOption;
    //! \brief standard option name
    static const char* const parametersAsStaticVariablesOption;
    //! \brief standard option name
    static const char* const initializationFromFileOption;
    //! \brief standard option name
    static const char* const buildIdentifierOption;
    //! \brief standard option
    static const char* const overridingParameters;
    //! \return a validator for the options passed to the DSL
    static tfel::utilities::DataMapValidator getDSLOptionsValidator();
    //
    std::vector<DSLOptionDescription> getDSLOptions() const override;
    const FileDescription& getFileDescription() const override final;
    const TargetsDescription& getTargetsDescription() const override;
    /*!
     * \brief open a file and add given external instructions at the
     * beginning
     * \param[in] f     : file
     * \param[in] ecmds : external commands
     * \param[in] s : substitutions patterns inserted (those
     * substitutions are given through command-line options such as
     * `--@YYY@=XXX`)
     */
    virtual void openFile(const std::string&,
                          const std::vector<std::string>&,
                          const std::map<std::string, std::string>&);

   protected:
    /*!
     * \brief An helper structure used by the
     * BehaviourDSLCommon class to modify the code provided
     * by the user.
     *
     * These modifiers are called when the parser encounters variables.
     */
    struct MFRONT_VISIBILITY_EXPORT VariableModifier {
      /*!
       * \param[in] v : the variable name
       * \param[in] b : true if "this" shall be added
       */
      virtual std::string exe(const std::string&, const bool) = 0;
      /*!
       * destructor
       */
      virtual ~VariableModifier();
    };  // end of struct VariableModifier
        /*!
         * \brief An helper structure used by the DSLBase class to modify the
         * code provided by the user.
         *
         * These modifiers are called when the parser encounters variables.
         */
    struct MFRONT_VISIBILITY_EXPORT WordAnalyser {
      /*!
       * \param[in] cb : code block being treated
       * \param[in] k : the current word
       */
      virtual void exe(CodeBlock&, const std::string&) = 0;
      /*!
       * destructor
       */
      virtual ~WordAnalyser();
    };  // end of struct WordAnalyser
    //! \brief options passed to the readNextBlock member
    struct CodeBlockParserOptions {
      //! \brief default constructor
      CodeBlockParserOptions();
      //! \brief move constructor
      CodeBlockParserOptions(CodeBlockParserOptions&&) = default;
      //! \brief copy constructor
      CodeBlockParserOptions(const CodeBlockParserOptions&) = default;
      //! \brief move assignement
      CodeBlockParserOptions& operator=(CodeBlockParserOptions&&) = default;
      //! \brief standard assignement
      CodeBlockParserOptions& operator=(const CodeBlockParserOptions&) =
          default;
      //! \brief destructor
      ~CodeBlockParserOptions() noexcept;
      /*!
       * \brief list of known symbols.
       * The key is the symbolic version of the variable.
       * The value is the mangled of the variable.
       */
      std::map<std::string, std::string> symbols;
      //! \brief member names
      std::set<std::string> mn;
      //! \brief static member names
      std::set<std::string> smn;
      //! \brief modifier applied to variables
      std::shared_ptr<VariableModifier> modifier;
      //! \brief analyser applied to variables
      std::shared_ptr<WordAnalyser> analyser;
      //! \brief delimeter marking the beginning of the block to be read
      std::string delim1 = "{";
      //! \brief delimeter marking the end of the block to be read
      std::string delim2 = "}";
      //! \brief if this member is true, the class name will be added in
      //! \brief front of variables names
      bool qualifyStaticVariables = false;
      //! \brief if this member is true, the "this->" prefix will be added in
      //! \brief front of variables names
      bool qualifyMemberVariables = false;
      //! \brief if true, the semi-colon will be allowed
      bool allowSemiColon = true;
      //! \brief if true, line number will be registred
      bool registerLine = true;
    };  // end of CodeBlockParserOptions
    //! \return a list of names that shall be reserved
    static std::vector<std::string> getDefaultReservedNames();
    /*!
     * \brief handle DSL options
     * \param[in] d: material knowledge description
     * \param[in] opts: options
     */
    static void handleDSLOptions(MaterialKnowledgeDescription&,
                                 const DSLOptions&);
    /*!
     * \brief extract common options
     * \param[in] d: material knowledge description
     */
    static DSLOptions buildCommonDSLOptions(
        const MaterialKnowledgeDescription&);
    /*!
     * \brief constructor
     * \param[in] opts: options passed to the DSL
     */
    DSLBase(const DSLOptions&);
    /*!
     * \brief add a dependency to an external `MFront` file
     * \param[in] f: file path
     * \param[in] interfaces: list of interfaces used to treat the file
     */
    virtual void addExternalMFrontFile(const std::string&,
                                       const std::vector<std::string>&) = 0;
    // \return options for child DSL (DSL called by this DSL)
    virtual DSLOptions buildDSLOptions() const = 0;
    /*!
     * \brief register a name.
     * \param[in] n : name
     */
    virtual void reserveName(const std::string&) = 0;
    /*!
     * \brief look if a name is reserved
     * \param[in] n : name
     */
    virtual bool isNameReserved(const std::string&) const = 0;
    /*!
     * \brief specify the name of the implementation before parsing the
     * `MFront` source file. Any name specified in the `MFront` file, through
     * the `@Law`, `@Behaviour` or `@Model` keywords, will be ignored.
     * \param[in] i: name of the implementation
     */
    virtual void overrideMaterialKnowledgeIdentifier(const std::string&);
    /*!
     * \brief specify the name of the material before parsing the
     * `MFront` source file. Any material name specified with the `@Material`
     * keyword will be ignored.
     * \param[in] m: material' name
     */
    virtual void overrideMaterialName(const std::string&);
    /*!
     * \brief specify the name of the author before parsing the
     * `MFront` source file. Any author' name specified with the `@Author`
     * keyword will be ignored.
     * \param[in] a: author' name
     */
    virtual void overrideAuthorName(const std::string&);
    /*!
     * \brief specify the date of the `MFront` implementation before
     * parsing the `MFront` source file. Any date specified with the
     * `@Date` keyword will be ignored.
     * \param[in] d: date
     */
    virtual void overrideDate(const std::string&);
    /*!
     * \brief specify the description of the `MFront` implementation before
     * parsing the `MFront` source file. Any description specified with the
     * `@Description` keyword will be ignored.
     * \param[in] d: description
     */
    virtual void overrideDescription(const std::string&);
    /*!
     * \brief specify that the given variable must be overriden by a parameter.
     * \param[in] n: variable name
     * \param[in] v: variable value
     */
    virtual void overrideByAParameter(const std::string&, const double) = 0;
    /*!
     * \brief add a static variable description
     * \param[in] v : variable description
     */
    virtual void addStaticVariableDescription(
        const StaticVariableDescription&) = 0;
    //! \return all the integer constants (static variables of the integer type)
    virtual std::map<std::string, int> getIntegerConstants() const = 0;
    /*!
     * \return the value of an integer constant
     * \param[in] n: variable name
     */
    virtual int getIntegerConstant(const std::string&) const = 0;
    /*!
     * \return the name of the generated class
     */
    virtual std::string getClassName() const = 0;
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
     * \brief check that the end of file in not reached yet
     * \param[in] m: calling method name
     * \param[in] e: error message
     */
    void checkNotEndOfFile(const std::string&, const std::string& = "") const;
    /*!
     * \brief read a specified token and advance to the next token
     * \param[in] m: calling method name
     * \param[in] v: expected value
     */
    void readSpecifiedToken(const std::string&, const std::string&);
    /*!
     * \brief throw an std::runtime_error
     * \param[in] m: calling method name
     * \param[in] e: error message
     */
    [[noreturn]] void throwRuntimeError(const std::string&,
                                        const std::string&) const;
    /*!
     * \return concatenate all the tokens from the current position up
     * to the first ';' token
     */
    std::string readUntilEndOfInstruction();
    /*!
     * read an unsigned short in the file
     * \param[in] m : calling method
     */
    unsigned short readUnsignedShort(const std::string&);
    /*!
     * \brief parse the next code block twice.
     * \param[out] res1 : resulting code block with the first  option set.
     * \param[out] res2 : resulting code block with the second option set.
     * \param[in]  o1   : first  option set.
     * \param[in]  o2   : second option set.
     */
    void readNextBlock(CodeBlock&,
                       CodeBlock&,
                       const CodeBlockParserOptions&,
                       const CodeBlockParserOptions&);
    /*!
     * \return the code block read.
     * \param[in] o : options
     */
    CodeBlock readNextBlock(const CodeBlockParserOptions&);
    /*!
     * \brief read one token and increment "current"
     */
    std::string readOnlyOneToken();
    /*!
     *
     */
    template <typename T>
    std::pair<bool, T> readInitialisationValue(const std::string&, const bool);
    //! \return if the usage of quantities is alllowed
    virtual bool useQt() const = 0;
    //! \brief disable the usage of quantities if not already set
    virtual void disableQuantitiesUsageIfNotAlreadySet() = 0;
    //! \brief return the type parsing options used by the DSL
    virtual SupportedTypes::TypeParsingOptions getTypeParsingOptions() const;
    //! \brief read a C++ type
    std::string readType();
    //! \brief read a C++ type if present
    std::optional<std::string> readVariableTypeIfPresent();
    /*!
     * \return the size of an array of variables, or 1 if the variable are not
     * defined as an array.
     * \param[in] n: variable name
     * \param[in] b: allow arrays of variables to be defined
     */
    unsigned int readArrayOfVariablesSize(const std::string&, const bool);
    /*!
     * \param[in] cont : variable container to wich variables are
     * added
     * \param[in] type : type of the variable
     * \param[in] allowArray : allow arrays of variables to be defined
     */
    void readVarList(VariableDescriptionContainer&,
                     const std::string&,
                     const bool);
    /*!
     * \param[in] cont : variable container to wich variables are
     * added
     * \param[in] allowArray : allow arrays of variables to be defined
     */
    void readVarList(VariableDescriptionContainer&, const bool);
    /*!
     * extract a boolean value from the current token and go the next
     * token
     * \param[in] m : calling method name (used for error message)
     * \return the boolean value read
     */
    bool readBooleanValue(const std::string&);
    /*!
     * extract a string from the current token and go the next token
     * \param[in] m : calling method name (used for error message)
     * \return the extracted string
     */
    std::string readString(const std::string&);
    /*!
     * extract an array of string starting from the current token and
     * go the token following the end of the array
     * \param[in] m : calling method name (used for error message)
     * \return the extracted array of strings
     */
    std::vector<std::string> readArrayOfString(const std::string&);
    /*!
     * extract an array of double starting from the current token and
     * go the token following the end of the array
     * \param[in] m : calling method name (used for error message)
     * \return the extracted array of doubles
     */
    std::vector<double> readArrayOfDouble(const std::string&);
    /*!
     * extract an array of string starting from the current token and
     * go the token following the end of the array
     * \param[in] m : calling method name (used for error message)
     * \return the extracted array of strings which contains only one
     * element if a string was read
     */
    std::vector<std::string> readStringOrArrayOfString(const std::string&);
    /*!
     * \brief extract a list of token
     * \param[out] l  : list read
     * \param[in]  m  : calling method name (used for error message)
     * \param[in]  db : beginning delimiter
     * \param[in]  de : end delimiter
     * \param[in] b   : if the current value is not the beginning
     * delimiter (or is past the end of the file), nothing is returned
     * if this parameter is true. An exception is thrown otherwise
     */
    void readList(std::vector<tfel::utilities::Token>&,
                  const std::string&,
                  const std::string&,
                  const std::string&,
                  const bool);
    /*!
     * \return a list of token
     * \param[in]  m  : calling method name (used for error message)
     * \param[in]  db : beginning delimiter
     * \param[in]  de : end delimiter
     * \param[in] b   : if the current value is not the beginning
     * delimiter (or is past the end of the file), nothing is returned
     * if this parameter is true. An exception is thrown otherwise
     */
    std::vector<tfel::utilities::Token> readList(const std::string&,
                                                 const std::string&,
                                                 const std::string&,
                                                 const bool);

    std::string readSpecifiedValue(const std::string&, const std::string&);

    std::vector<std::string> readSpecifiedValues(const std::string&,
                                                 const std::string&,
                                                 const std::string&);

    std::vector<std::string> readSpecifiedValues(
        const std::string&, const std::vector<std::string>&);
    /*!
     * \brief read the bounds associated to a variable
     * \return a pair giving the variable name and the bounds
     */
    std::pair<std::string, VariableBoundsDescription> readVariableBounds();
    /*!
     * call mfront in a subprocess
     * \param[in] interfaces : list of interfaces
     * \param[in] files      : list of files
     */
    virtual void callMFront(const std::vector<std::string>&,
                            const std::vector<std::string>&);
    /*!
     * \brief This function handles a material property treated as a
     * dependency of the current file.
     *
     * This function:
     * - analyse the given file using the MaterialPropertyDSL
     * - register the function name generated by the MFront interface
     * - add the function name to the list of material laws used by
     *   the current file
     * - declared the headers generated by the MFront interface for inclusion
     * - launch mfront in a sub-process to generate the source files
     *   associated with this material property and have them compiled
     *   when mandatory (done by the callMFront method).
     *
     * \param[in] f : file in which the material law is
     * implemented. This must be the full path.
     */
    virtual std::shared_ptr<MaterialPropertyDescription>
    handleMaterialPropertyDescription(const std::string&);
    /*!
     * \brief set the material described by the implementation
     * \param[in] m: material name
     */
    virtual void setMaterial(const std::string&) = 0;
    /*!
     * \brief set the unit system
     * \param[in] s: unit system
     */
    virtual void setUnitSystem(const std::string_view) = 0;
    /*!
     * \brief set the name of the implementation, i.e. the material property'
     * name, the behaviour' name or the model' name.
     * \param[in] i: implementation name
     */
    virtual void setMaterialKnowledgeIdentifier(const std::string&) = 0;
    //! \brief set the author of the implementation
    virtual void setAuthor(const std::string&);
    //! \brief set the date at which the implementation has been written
    virtual void setDate(const std::string&);
    //! \brief set the description of the `MFront` file
    virtual void setDescription(const std::string&);
    //! \brief treat the `@Material` keyword
    virtual void treatMaterial();
    //! \brief treat the `@UnitSystem` keyword
    virtual void treatUnitSystem();
    //! \brief handle the `@MFront` keyword
    virtual void treatMFront();
    //! \brief handle the `@Import` keyword
    virtual void treatImport();
    //! \brief handle the `@MaterialLaw` keyword
    virtual void treatMaterialLaw();
    //! \brief handle the `@Link` keyword
    virtual void treatLink();
    //! \brief handle the `@Author` keyword
    virtual void treatAuthor();
    //! \brief handle the `@Data` keyword
    virtual void treatDate();
    //! \brief handle the `@Includes` keyword
    virtual void treatIncludes();
    //! \brief handle the `@Sources` keyword
    virtual void treatSources();
    //! \brief handle the `@Private` keyword
    virtual void treatPrivate();
    //! \brief handle the `@Members` keyword
    virtual void treatMembers();
    //! \brief handle the `@DSL` or `@Parser` keyword
    virtual void treatDSL();
    //! \brief handle the `@StaticVar` keyword
    virtual void treatStaticVar();
    //! \brief handle the `@IntegerConstant` keyword
    virtual void treatIntegerConstant();
    //! \brief handle the `@Description` keyword
    virtual void treatDescription();
    //! \brief handle the `@LonelySeparator` keyword
    virtual void treatLonelySeparator();
    //!
    virtual void ignoreKeyWord(const std::string&);
    //! \brief treat an unknown keyword
    virtual void treatUnknownKeyword();
    /*!
     * \brief this method must be called at the end of the treatment:
     * - to set each libraries defined the auxiliary targets as
     *   dependencies of the librairies defined in the main one.  -
     * - merge the auxiliary targets descriptions into the main one.
     * - clear the auxiliary targets descriptions
     */
    virtual void completeTargetsDescription();
    /*!
     * \brief register an integer constant.
     * \param[in] n: name of the constant
     * \param[in] l: line number
     * \param[in] v: value
     */
    virtual void registerIntegerConstant(const std::string&,
                                         const size_t,
                                         const int);
    /*!
     * \brief extract a double from the current token
     */
    double readDouble();
    //! \brief destructor
    ~DSLBase() override;
    //! \brief description of the file treated
    FileDescription fd;
    /*!
     * \brief main targets description
     *
     * Targets description are generated by the processing of the
     * input file. However, it is convenient to distinguish during the
     * processing phase:
     * - the main targets which directly result from the processing of
     *   the current file. Those targets can only completly be defined
     *   at the end of the processing.
     * - the auxiliary targets descriptions for targets generated
     *   indirectly (for example by importing material law using the
     *   `@MaterialLaw` keyword).
     *
     * Once the input is completely processed, the auxiliary targets
     * descriptions are merged into the main one using the
     * `completeTargetsDescription` method.
     */
    TargetsDescription td;
    //! \brief additional linker flags
    std::vector<std::string> ldflags;
    //! \brief auxiliary target descriptions
    std::vector<TargetsDescription> atds;
    //! \brief current position in the input stream
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
    //! \brief overriden implementation name
    std::string overriden_implementation_name;
    //! \brief overriden material
    std::string overriden_material;
    //! \brief overriden author
    std::string overriden_author;
    //! \brief overriden date
    std::string overriden_date;
    //! \brief overriden description
    std::string overriden_description;
  };  // end of class DSLBase

}  // end of namespace mfront

#include "MFront/DSLBase.ixx"

#endif /* LIB_MFRONT_DSLBASE_HXX */
