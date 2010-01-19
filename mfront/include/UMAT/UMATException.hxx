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

    UMATException(const std::string&);

    const char* 
    what (void) const throw();

    std::string 
    getMsg(void) const throw();
    
    virtual ~UMATException() throw();

  private:
    
    std::string msg;
    
  }; // end of struct UMATException

  struct UMATInvalidNTENSValue
    : public UMATException
  {
    UMATInvalidNTENSValue(const unsigned short);
    virtual ~UMATInvalidNTENSValue() throw();
  private:
    UMATInvalidNTENSValue();
    UMATInvalidNTENSValue(const UMATInvalidNTENSValue&);
    UMATInvalidNTENSValue&
    operator=(const UMATInvalidNTENSValue&);
  }; // end of struct UMATInvalidNTENSValue

  struct UMATInvalidDimension
    : public UMATException
  {
    UMATInvalidDimension(const std::string&,
			 const unsigned short);
    virtual ~UMATInvalidDimension() throw();
  private:
    UMATInvalidDimension();
    UMATInvalidDimension(const UMATInvalidDimension&);
    UMATInvalidDimension&
    operator=(const UMATInvalidDimension&);
  }; // end of struct UMATInvalidDimension
  
} // end of namespace umat

#endif /* _LIB_MFRONT_UMATEXCEPTION_H */

