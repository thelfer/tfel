/*!
 * \file   SupportedTypes.hxx
 * \brief  This file declares the SupportedTypes class
 * \author Helfer Thomas
 * \date   12 Jan 2007
 */

#ifndef _LIB_MFRONT_SUPPORTEDTYPES_H_
#define _LIB_MFRONT_SUPPORTEDTYPES_H_ 

#include<string>
#include<ostream>
#include<map>

#include"TFEL/Config/TFELConfig.hxx"

namespace mfront
{

  struct TFEL_VISIBILITY_EXPORT SupportedTypes
  {

    enum TypeFlag{Scalar,Stensor};

  private:
    
    std::map<std::string,TypeFlag> flags;
    
    TFEL_VISIBILITY_LOCAL
    void registerTypes(void);

  public:

    struct TypeSize;

    SupportedTypes();

    TypeFlag
    getTypeFlag(const std::string&) const;

    TypeSize
    getTypeSize(const std::string&) const;

    std::string
    getTimeDerivativeType(const std::string&) const;

  }; // end of class SupportedTypes

  class TFEL_VISIBILITY_EXPORT SupportedTypes::TypeSize
  {
    friend std::ostream& 
    operator<< (std::ostream&, const TypeSize&);

    typedef unsigned short ushort;
    ushort scalarSize;
    ushort stensorSize;

  public:

    TypeSize();

    TypeSize(const TypeSize&);

    TypeSize(const ushort,const ushort);

    TypeSize&
    operator=(const TypeSize&);
    
    TypeSize&
    operator+=(const TypeSize&);
    
    ushort
    getScalarSize() const;
    
    ushort
    getStensorSize() const;

  }; // end of class SupportedTypes::TypeSize


} // end of namespace mfront

#endif /* _LIB_MFRONT_SUPPORTEDTYPES_H */

