/*!
 * \file   include/TFEL/Exception/TFELException.hxx
 * \brief  This file declares the TFELException class. 
 * \author Helfer Thomas
 * \date   02 Aug 2006
 */

#ifndef _LIB_TFEL_EXCEPTIONS_H_
#define _LIB_TFEL_EXCEPTIONS_H_ 

#include<string>
#include<stdexcept>

#include"TFEL/Config/TFELConfig.hxx"

namespace tfel
{

  namespace exception
  {

    /*!
     * \class  TFELException
     * \brief  Generic Exception, base class for all the other exception used in TFEL.
     * \author Helfer Thomas
     * \date   02 Aug 2006
     */
    struct TFELEXCEPTION_VISIBILITY_EXPORT TFELException
      : public std::exception
    {

      /*!
       * \brief  Return the name of the class.
       * \param  void.
       * \return std::string, the name of the class.
       * \see    Name.
       */
      static std::string 
      getName(void);

      /*!
       *  \brief Default Constructor.
       *  \param s, string describing the exception that occured
       */
      TFELException(const std::string& s);

      /*!
       * \brief  Return a string describing the exception that occured.
       * \param  void.
       * \return const char*.
       */
      virtual const char* 
      what(void) const throw();

      /*!
       * \brief  Return a string describing the exception that occured.
       * \param  void.
       * \return const std::string.
       */
      virtual const std::string
      getMsg(void) const;

      virtual ~TFELException() throw();

    private:

      /*!
       * A string describing the exception that occured
       */
      std::string msg;

    };

  } // end of namespace exception

} // end of namespace tfel

#endif /* _LIB_TFEL_EXCEPTIONS_H */

