/*!
 * \file   Name.hxx
 * \brief  Defines the Name traits class which returns the Name of its template argument  
 * \author Helfer Thomas
 * \date   17 Jul 2006
 */

#ifndef _LIB_TFEL_NAME_H_
#define _LIB_TFEL_NAME_H_ 

#include<string>

namespace tfel{
  
  namespace utilities{
    
    /*!
     * \class  Name
     * \brief  The Name traits class returns the Name of its template argument.
     * \param  T, type.
     * \author Helfer Thomas
     * \date   28 Aug 2006
     */
    template<typename T>
    struct Name
    {
      /*!
       * \brief returns the Name of T.
       * By default, this supposes that T has defined a static member getName().
       * \param  void
       * \return const std::string, the name of the class.
       */
      static std::string 
      getName(void){
	return T::getName();
      }
    };

    /*!
     * \brief returns the name of the class of an object.
     * \param  T, type of which we want to know the name.
     * \return const std::string
     * \author Helfer Thomas
     * \date   01 Sept 2006
     */
    template<typename T>
    std::string 
    name(const T&);

  } // end of namespace utilities

  namespace meta{
    
    /*!
     * \class TLGetNames
     * \brief A metafunction which returns the name of each element.
     *
     * \param typename List, a TL.
     * 
     * \return std::string, the name of each element.
     * \author Helfer Thomas
     * \date   08 Januar 2006
     */
    template<typename List>
    class TLGetNames;

  } // end of namespace meta

} // end of namespace tfel

#include"TFEL/Utilities/Name.ixx"

#endif /* _LIB_TFEL_NAME_H */

