/*!
 * \file   ArrayRefCountedStorage.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   20 aoû 2006
 */

#ifndef _LIB_TFEL_ARRAYSTORAGE_HXX_
#define _LIB_TFEL_ARRAYSTORAGE_HXX_ 

#include<cstdlib>
#include<cassert>
#include<cstddef>
#include<algorithm>

#include"TFEL/Config/TFELConfig.hxx"

#include"TFEL/Utilities/Info.hxx"

namespace tfel{
  
  namespace math {

    template<typename T>
    class ArrayRefCountedStorage
    {

    protected:

      //! Default constructor.
      TFEL_MATH_INLINE2
      ArrayRefCountedStorage()
	: count(0), v(0), size(0u)
      {}

      //! Constructor from the size of the array.
      /*
       * \param const unsigned int, the size of the array.
       */
      TFEL_MATH_INLINE2
      ArrayRefCountedStorage(const unsigned int size_)
	: size(size_)
      {
	assert(this->size!=0u);
	this->count = new unsigned short(1u);
	this->v     = new T[size_];
      }

      //! Copy Constructor
      /*
       * \param const ArrayRefCountedStorage&, the source.
       * \warning the values of the array are not copied.
       * the two objects points to the same memory zone.
       */
      TFEL_MATH_INLINE2
      ArrayRefCountedStorage(const ArrayRefCountedStorage& src)
	: count(src.count), v(src.v), size(src.size)
      {
	if(this->count!=0){
	  ++(*(this->count));
	} else {
	  assert(this->v==0);
	  assert(this->size==0u);
	}
      }

      //! Assignement operator
      /*
       * \param  const ArrayRefCountedStorage&, the source.
       * \return ArrayRefCountedStorage&, a reference to this.
       * \warning the values of the array are not copied.
       * the two objects points to the same memory zone.
       */
      TFEL_MATH_INLINE2
      ArrayRefCountedStorage&
      operator = (const ArrayRefCountedStorage& src)
      {
	// check for self-assignement
	if(this!=&src){
	  if(this->v!=src.v){
	    this->decreaseCounter();
	    this->v     = src.v;
	    this->size  = src.size;
	    this->count = src.count;
	    ++(*this->count);
	  } else {
	    // the two arrays points to the same memory, so we do some
	    // sanity check
	    assert(this->count==src.count);
	    assert(this->size==src.size);
	  }
	}
	return *this;
      }

      //! Destructor
      TFEL_MATH_INLINE2
      ~ArrayRefCountedStorage()
      {
	this->decreaseCounter();
      }

    public:

      void
      makeUniq(void)
      {
	using namespace std;
	if(this->count!=0){
	  if(*(this->count)!=1){
	    if(this->v!=0){
	      T *p = new T[this->size];
	      copy(this->v,this->v+this->size,p);
	      this->v = p; 
	    }
	    --(*(this->count));
	    this->count = new unsigned short(1);
	  }
	}
      } // end of makeUniq

      //! return the size of the array
      /*
       * \return const unsigned int, the size of the array.
       */
      TFEL_MATH_INLINE2
      unsigned int
      getSize(void) const
      {
	return this->size;
      }

      /*!
       * type of the stensor's values.
       * (this is a stl requirement).
       */
      typedef T value_type;
      /*!
       * type of a pointer to the value contained.
       * (this is a stl requirement).
       */
      typedef value_type* pointer;
      /*!
       * type of a const pointer to the value contained.
       * (this is a stl requirement).
       */
      typedef const value_type* const_pointer;
      /*!
       * type of the stensor's iterator.
       * (provided for stl compatibility).
       */
      typedef pointer iterator;
      /*!
       * type of the stensor's const iterator.
       * (provided for stl compatibility).
       */
      typedef const_pointer const_iterator;
      /*!
       * type of the stensor's reverse iterator.
       * (provided for stl compatibility).
       */
      typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
      /*!
       * type of the stensor's const reverse iterator.
       * (provided for stl compatibility).
       */
      typedef std::reverse_iterator<iterator> reverse_iterator;
      /*!
       * type of a reference to the value contained.
       * (this is a stl requirement).
       */
      typedef value_type& reference;
      /*!
       * type of a const reference to the value contained.
       * (this is a stl requirement).
       */
      typedef const value_type& const_reference;
      /*!
       * type of the size of the container.
       * (this is a stl requirement).
       */
      typedef size_t size_type;
      /*!
       * type of the difference between two iterators.
       * (this is a stl requirement).
       */
      typedef ptrdiff_t difference_type;

      /*
       * return an iterator to the first element of the vector
       * (provided for stl compatibility)
       * \return iterator, an iterator to the first element
       */
      TFEL_MATH_INLINE2
      iterator
      begin(void)
      {
	return this->v;
      }

      /*
       * return an const iterator to the first element of the vector
       * (provided for stl compatibility)
       * \return const_iterator, a const iterator to the first element
       */
      TFEL_MATH_INLINE2
      const_iterator
      begin(void) const
      {
	return this->v;
      }
      
      /*
       * return an iterator after the last element of the vector
       * (provided for stl compatibility)
       * \return iterator, an iterator after the last element
       */
      TFEL_MATH_INLINE2
      iterator
      end(void)
      {
	return this->v+this->size;
      }

      /*
       * return an const iterator after the last element of the vector
       * (provided for stl compatibility)
       * \return const_iterator, a const iterator after the last element
       */
      TFEL_MATH_INLINE2
      const_iterator
      end(void) const
      {
	return this->v+this->size;
      }

      /*
       * return an reverse iterator to the last element of the vector
       * (provided for stl compatibility)
       * \return reverse_iterator, a reverse iterator to the last element
       */
      TFEL_MATH_INLINE2
      reverse_iterator
      rbegin(void)
      {
	return reverse_iterator(this->v+this->size);
      }

      /*
       * return an const reverse iterator to the last element of the vector
       * (provided for stl compatibility)
       * \return const_reverse_iterator, a const reverse iterator to the last element
       */
      TFEL_MATH_INLINE2
      const_reverse_iterator
      rbegin(void) const
      {
	return const_reverse_iterator(this->v+this->size);
      }

      /*
       * return an  reverse iterator before the first element of the vector
       * (provided for stl compatibility)
       * \return reverse_iterator, a reverse iterator before the first element
       */
      TFEL_MATH_INLINE2
      reverse_iterator
      rend(void)
      {
	return reverse_iterator(this->v);
      }

      /*
       * return an const reverse iterator before the first element of the vector
       * (provided for stl compatibility)
       * \return const_reverse_iterator, a const reverse iterator before the first element
       */
      TFEL_MATH_INLINE2
      const_reverse_iterator
      rend(void) const
      {
	return const_reverse_iterator(this->v);
      }

    private:

      //! reference counter
      mutable unsigned short *count;
      
      //! decrease the reference counter
      void
      decreaseCounter(void)
      {
	if((this->count) != 0){
	  if(--(*(this->count))==0u){
	    delete [] this->v;
	    delete this->count;
	  }
	} else {
	  assert(this->v==0);
	  assert(this->size==0u);
	}
      }

    protected:
      
      //! the value hold by the array
      T *v;
      //! size of the array
      unsigned int size;

    };

  } //end of namespace math
  
} // end of namespace tfel

#endif /* _LIB_TFEL_ARRAYSTORAGE_HXX */
