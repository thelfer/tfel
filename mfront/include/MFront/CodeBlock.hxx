/*! 
 * \file  CodeBlock.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 18 mai 2014
 */

#ifndef _LIB_MFRONT_CODEBLOCK_H_
#define _LIB_MFRONT_CODEBLOCK_H_ 

#include<set>
#include<string>

#include"MFront/MFrontConfig.hxx"

namespace mfront{

  /*!
   * Raw code block
   */
  struct TFEL_VISIBILITY_EXPORT CodeBlock
  {
    //! code
    std::string code;
    //! description
    std::string description;
    //! static variables used in the block
    std::set<std::string> static_variables;
      //! variables used in the block
    std::set<std::string> variables;
  }; // end of struct CodeBlock
  
} // end of namespace mfront

#endif /* _LIB_MFRONT_CODEBLOCK_H */

