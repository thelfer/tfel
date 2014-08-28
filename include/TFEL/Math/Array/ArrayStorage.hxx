/*!
 * \file   include/TFEL/Math/Array/ArrayStorage.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   20 aoû 2006
 */

#ifndef _LIB_TFEL_ARRAY_STORAGE_HXX_
#define _LIB_TFEL_ARRAY_STORAGE_HXX_ 

#include<iterator>
#include<cstdlib>
#include<cstddef>

#include"TFEL/Config/TFELConfig.hxx"

namespace tfel{
  
  namespace math {

    template<typename T>
    class ArrayStorage
    {
    private:

      ArrayStorage(const ArrayStorage&);
      ArrayStorage& operator = (const ArrayStorage&);

    protected:
      
      T *v;
      unsigned int size;

      ArrayStorage(const unsigned int size_)
	: size(size_)
      {
	this->v = new T[size_];
      }

      ~ArrayStorage()
      {
	delete [] this->v;
      }

    public:

      unsigned int getSize()
      {
	return this->size;
      }

      typedef T 		value_type;
      typedef value_type* 	pointer;
      typedef const value_type* const_pointer;
      typedef pointer           iterator;
      typedef const_pointer     const_iterator;
      typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
      typedef std::reverse_iterator<iterator>       reverse_iterator;
      typedef value_type& 	reference;
      typedef const value_type& const_reference;
      typedef size_t 		size_type;
      typedef ptrdiff_t 	difference_type;

      TFEL_MATH_INLINE2 iterator begin(void)
      {
	return this->v;
      }
      TFEL_MATH_INLINE2 const_iterator begin(void) const
      {
	return this->v;
      }
      TFEL_MATH_INLINE2 iterator end(void)
      {
	return this->v+this->size();
      }
      TFEL_MATH_INLINE2 const_iterator end(void) const
      {
	return this->v+this->size();
      }

      TFEL_MATH_INLINE2 reverse_iterator rbegin(void)
      {
	return reverse_iterator(this->v+this->size());
      }
      TFEL_MATH_INLINE2 const_reverse_iterator rbegin(void) const
      {
	return const_reverse_iterator(this->v+this->size());
      }
      TFEL_MATH_INLINE2 reverse_iterator rend(void)
      {
	return reverse_iterator(this->v);
      }
      TFEL_MATH_INLINE2 const_reverse_iterator rend(void) const
      {
	return const_reverse_iterator(this->v);
      }

    };

  } //end of namespace math
  
} // end of namespace tfel

#endif /* _LIB_TFEL_ARRAY_STORAGE_HXX */
