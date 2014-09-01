/*! 
 * \file  mfront/include/MFront/CodeBlock.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 18 mai 2014
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
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

