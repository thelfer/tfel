/*! 
 * \file  WordAnalyser.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 14 févr. 2013
 */

#ifndef _LIB_MFRONT_WORDANALYSER_H_
#define _LIB_MFRONT_WORDANALYSER_H_ 

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

  template<typename T,typename T2>
  struct TFEL_VISIBILITY_LOCAL StandardWordAnalyser
    : public WordAnalyser
  {
    //! a simple alias
    typedef void (T2::* MPtr)(const std::string&);
    /*!
     * constructor
     */
    StandardWordAnalyser(T&,const MPtr);
    /*!
     * \param[in] k : the current word
     */
    void
    exe(const std::string&);
    /*!
     * destructor
     */
    ~StandardWordAnalyser();

  private:

    T& instance;
    const MPtr mptr;

  };

  /*!
   * create a standard variable modifier from an instance of a class
   * and a pointer to a member
   */
  template<typename T,typename T2>
  tfel::utilities::shared_ptr<WordAnalyser>
  makeWordAnalyser(T&,void (T2::*)(const std::string&));

} // end of namespace mfront

#include"MFront/WordAnalyser.ixx"

#endif /* _LIB_MFRONT_WORDANALYSER_H */

