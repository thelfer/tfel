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

#include"MFront/UMAT/UMATConfig.hxx"

namespace umat {

  struct MFRONT_UMAT_VISIBILITY_EXPORT UMATException
    : public std::exception
  {

    UMATException(const std::string&);

    UMATException(const UMATException&);

    const char* 
    what (void) const throw();

    std::string 
    getMsg(void) const throw();
    
    virtual ~UMATException() throw();

  private:
    
    UMATException();

    UMATException&
    operator=(const UMATException&);

    const std::string msg;
    
  }; // end of struct UMATException

  struct MFRONT_UMAT_VISIBILITY_EXPORT UMATInvalidNTENSValue
    : public UMATException
  {
    UMATInvalidNTENSValue(const unsigned short);
    UMATInvalidNTENSValue(const UMATInvalidNTENSValue&);
    virtual ~UMATInvalidNTENSValue() throw();
  private:
    UMATInvalidNTENSValue();
    UMATInvalidNTENSValue&
    operator=(const UMATInvalidNTENSValue&);
  }; // end of struct UMATInvalidNTENSValue

  struct MFRONT_UMAT_VISIBILITY_EXPORT UMATInvalidDimension
    : public UMATException
  {
    UMATInvalidDimension(const std::string&,
			 const unsigned short);
    UMATInvalidDimension(const UMATInvalidDimension&);
    virtual ~UMATInvalidDimension() throw();
  private:
    UMATInvalidDimension();
    UMATInvalidDimension&
    operator=(const UMATInvalidDimension&);
  }; // end of struct UMATInvalidDimension
  
} // end of namespace umat

#endif /* _LIB_MFRONT_UMATEXCEPTION_H */

