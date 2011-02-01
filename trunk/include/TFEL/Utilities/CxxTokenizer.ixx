/*!
 * \file   CxxTokenizer.ixx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   12 nov 2006
 */

#ifndef _LIB_TFEL_CXXTOKENIZER_IXX_
#define _LIB_TFEL_CXXTOKENIZER_IXX_ 

namespace tfel{

  namespace utilities{

    template<typename InputIterator>
    void CxxTokenizer::import(const InputIterator p,
			      const InputIterator pe)
    {
      this->clear();
      this->fileTokens.insert(this->fileTokens.begin(),p,pe);
      this->treatPreprocessorDirectives();
      this->splitTokens();
    } // end of CxxTokenizer::import

  } // end of namespace utilities

} // end of namespace tfel

#endif /* _LIB_TFEL_CXXTOKENIZER_IXX */
