/*! 
 * \file  VariableModifier.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 14 févr. 2013
 */

#ifndef _LIB_MFRONT_VARIABLEMODIFIER_H_
#define _LIB_MFRONT_VARIABLEMODIFIER_H_ 

#include<string>

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/Utilities/SmartPtr.hxx"

namespace mfront
{

  /*!
   * \brief An helper structure used by the ParserBase class to modify the
   * code provided by the user.
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

  template<typename T,typename T2>
  struct TFEL_VISIBILITY_LOCAL StandardVariableModifier
    : public VariableModifier
  {
    //! a simple alias
    typedef std::string (T2::* MPtr)(const std::string&,const bool);
    /*!
     * constructor
     */
    StandardVariableModifier(T&,const MPtr);
    /*!
     * \param[in] v : the variable name
     * \param[in] b : true if "this" shall be added
     */
    std::string
    exe(const std::string&,
	const bool);
    /*!
     * destructor
     */
    ~StandardVariableModifier();

  private:

    T& instance;
    const MPtr mptr;

  };

  /*!
   * create a standard variable modifier from an instance of a class
   * and a pointer to a member
   */
  template<typename T,typename T2>
  tfel::utilities::shared_ptr<VariableModifier>
  makeVariableModifier(T&,std::string (T2::*)(const std::string&,const bool));

} // end of namespace mfront

#include"MFront/VariableModifier.ixx"

#endif /* _LIB_MFRONT_VARIABLEMODIFIER_H */

