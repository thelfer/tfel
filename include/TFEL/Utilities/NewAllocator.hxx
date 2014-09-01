/*!
 * \file   include/TFEL/Utilities/NewAllocator.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   01 jui 2006
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_TFEL_NEWALLOCATOR_HXX_
#define _LIB_TFEL_NEWALLOCATOR_HXX_ 

#include<string>
#include<new>

namespace tfel{
  
  namespace utilities{

    /*!
     * \class  NewAllocator 
     * \brief  Allocates and desallocate memory using new[] and delete[].
     * \see    NewAllocator
     * \author Helfer Thomas
     * \date   31 Jul 2006
     */
    struct NewAllocator
    {

      /*!
       * \brief  Return the name of the class.
       * \param  void.
       * \return std::string, the name of the class.
       * \see    Name.
       */
      static
      const std::string
      getName(void){
	return "NewAllocator";
      }

      /*!
       * A simple typedef
       */
      typedef size_t size_type;

      /*!
       * \brief allocates memory for size objects.
       * 
       * \param T, type of the object manipulated.
       * \param size, number of objects for which memory is requested.
       * \return T*, pointer to the memory allocated.
       * 
       * \throw std::bad_alloc if no memory is available.
       *
       */
      template<typename T>
      static
      T*
      alloc(size_type size)
      {
	return new T[size];
      }
      
      /*!
       * \brief desallocates memory.
       * 
       * \param T, type of the object manipulated.
       * \param p, pointer to the memory to be freed.
       *
       * \note the pointer can be 0.
       */
      template<typename T>
      static
      void
      free(T *p)
      {
	delete [] p;
      }

    };

  } // end of namespace utilities

} // end of namespace tfel  

#endif /* _LIB_TFEL_NEWALLOCATOR_HXX */

