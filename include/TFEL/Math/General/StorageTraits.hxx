/*!
  \file   StorageTraits.hxx
  \author Helfer Thomas
  \date   02 jun 2006
  
  \brief  
*/

#ifndef _LIB_TFEL_STORAGE_TRAITS_HXX_
#define _LIB_TFEL_STORAGE_TRAITS_HXX_ 

namespace tfel{
  
  namespace math {

    /*
     * \class StorageStraits
     * \brief Traits class for storage layouts.
     * \param typename T, Storage class
     */    
    template<typename T>
    struct StorageTraits{
      /*
       * Tells if memory used by the storage class 
       * is allocated on the heap.
       * The storage class is responsible for desallocating 
       * this memory.
       */
      static const bool storage_is_dynamic  = T::storage_is_dynamic;
      /*
       * Tells if memory used by the storage class 
       * is allocated on the stack.
       * Memory is automatically destroyed when the storage class is.
       */
      static const bool storage_is_static   = T::storage_is_static;
      /*
       * Tells if memory used by the storage class 
       * is not under the responsability of the storage class.
       */
      static const bool storage_is_external = T::storage_is_external;
    };

  } // end of namespace math

} // end of namespace tfel  

#endif /* _LIB_TFEL_STORAGE_TRAITS_HXX */

