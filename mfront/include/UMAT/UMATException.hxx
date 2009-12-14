/*!
 * \file   UMATException.hxx
 * 
 * \brief    
 * \author Helfer Thomas
 * \date   02 Aug 2006
 */

#ifndef _LIB_MFRONT_UMATEXCEPTION_H_
#define _LIB_MFRONT_UMATEXCEPTION_H_ 

#include<string>
#include<exception>

namespace umat {

  struct UMATException
    : public std::exception
  {

    static const std::string
    getName(void);
    
    UMATException(const std::string&);

    const char* 
    what (void) const throw();

    const std::string 
    getMsg(void) const throw();
    
    ~UMATException() throw();

  private:
    
    std::string msg;
    
  }; // end of struct UMATException
  
} // end of namespace umat

#endif /* _LIB_MFRONT_UMATEXCEPTION_H */

