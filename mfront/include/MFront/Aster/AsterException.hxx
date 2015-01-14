/*! 
 * \file  mfront/include/MFront/Aster/AsterException.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 24 janv. 2013
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_MFRONT_ASTEREXCEPTION_H_
#define _LIB_MFRONT_ASTEREXCEPTION_H_ 

#include<string>
#include<exception>

#include"MFront/Aster/AsterConfig.hxx"

namespace aster {

  struct MFRONT_ASTER_VISIBILITY_EXPORT AsterException
    : public std::exception
  {

    AsterException(const std::string&);

    AsterException(const AsterException&);

    const char* 
    what (void) const noexcept;

    std::string 
    getMsg(void) const noexcept;
    
    virtual ~AsterException() noexcept;

  private:
    
    AsterException();

    AsterException&
    operator=(const AsterException&);

    const std::string msg;
    
  }; // end of struct AsterException

  struct MFRONT_ASTER_VISIBILITY_EXPORT AsterInvalidNTENSValue
    : public AsterException
  {
    AsterInvalidNTENSValue(const unsigned short);
    AsterInvalidNTENSValue(const AsterInvalidNTENSValue&);
    virtual ~AsterInvalidNTENSValue() noexcept;
  private:
    AsterInvalidNTENSValue();
    AsterInvalidNTENSValue&
    operator=(const AsterInvalidNTENSValue&);
  }; // end of struct AsterInvalidNTENSValue

  struct MFRONT_ASTER_VISIBILITY_EXPORT AsterInvalidDimension
    : public AsterException
  {
    AsterInvalidDimension(const std::string&,
			 const unsigned short);
    AsterInvalidDimension(const AsterInvalidDimension&);
    virtual ~AsterInvalidDimension() noexcept;
  private:
    AsterInvalidDimension();
    AsterInvalidDimension&
    operator=(const AsterInvalidDimension&);
  }; // end of struct AsterInvalidDimension
  
} // end of namespace aster

#endif /* _LIB_MFRONT_ASTEREXCEPTION_H */

