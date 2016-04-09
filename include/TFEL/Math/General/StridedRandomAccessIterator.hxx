/*!
 * \file   StridedRandomAccessIterator.hxx
 * \brief    
 * \author Thomas Helfer
 * \date   08 avril 2016
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_TFEL_MATH_STRIDEDRANDOMACCESSITERATOR_HXX
#define LIB_TFEL_MATH_STRIDEDRANDOMACCESSITERATOR_HXX

#include"TFEL/Config/TFELConfig.hxx"
#include<iterator>

namespace tfel{

  namespace math{

    /*!
     * \brief base class of the StridedRandomAccessIterator containing
     * some static common utility methods.
     */
    struct TFELMATH_VISIBILITY_EXPORT StridedRandomAccessIteratorBase
    {
      //! 
      TFEL_NORETURN static void throwNullStrideException(void);
      //! 
      TFEL_NORETURN static void throwUmatchedStrideException(void);
    };
    
    /*!
     * \tparam iterator_base: 
     * \brief an adaptator to random access iterator
     */
    template<typename iterator_base>
    struct StridedRandomAccessIterator
      : public StridedRandomAccessIteratorBase
    {
      //! a simple alias
      using traits            = std::iterator_traits<iterator_base>;
      using value_type        = typename traits::value_type;
      using reference         = typename traits::reference;
      using pointer           = typename traits::pointer;
      using iterator_category = typename traits::iterator_category;
      using difference_type   = typename traits::difference_type;
      /*!
       * \brief constructor
       * \param[in] c: current position
       * \param[in] s: stride
       */
      explicit StridedRandomAccessIterator(const iterator_base c,
					   const difference_type s)
	: current(c),
	  stride(s)
      {
	if(this->stride==0){throwNullStrideException();};
      } // end of StridedRandomAccessIterator
      //! default constructor
      explicit StridedRandomAccessIterator()
	: current(iterator_base()),
	  stride(difference_type())
      {}
      //! move constructor
      StridedRandomAccessIterator(StridedRandomAccessIterator&&) = default;
      //! copy constructor
      StridedRandomAccessIterator(const StridedRandomAccessIterator&) = default;
      //! move assignement
      StridedRandomAccessIterator&
      operator=(StridedRandomAccessIterator&&) = default;
      //! standard assignement
      StridedRandomAccessIterator&
      operator=(const StridedRandomAccessIterator&) = default;
      
      reference operator*() const {
	if(this->stride==0){throwNullStrideException();};
	return *(this->current);
      }

      reference operator[](const difference_type n) const
      {
	if(this->stride==0){throwNullStrideException();};
	return *(*this+n);
      }
      
      StridedRandomAccessIterator operator++(){
	if(this->stride==0){throwNullStrideException();};
	std::advance(this->current,this->stride);
	return *this;
      }
      
      StridedRandomAccessIterator operator++(int){
	if(this->stride==0){throwNullStrideException();};
	auto c = *this;
	std::advance(this->current,this->stride);
	return c;
      }

      StridedRandomAccessIterator operator+(const difference_type n) const{
	if(this->stride==0){throwNullStrideException();};
	auto c = *this;
	std::advance(c.current,n*(c.stride));
	return c;
      }
      
      StridedRandomAccessIterator operator-(const difference_type n) const{
	return *this+(-n);
      }

      StridedRandomAccessIterator operator+=(const difference_type n) const{
	if(this->stride==0){throwNullStrideException();};
	std::advance(this->current,n*(this->stride));
	return *this;
      }
      
      bool operator==(const StridedRandomAccessIterator& o) const{
	if(this->stride==0){throwNullStrideException();};
	return this->current==o.current;
      } // end of operator==

      bool operator!=(const StridedRandomAccessIterator& o) const{
	return !(this->operator==(o));
      } // end of operator==

      difference_type operator-(const StridedRandomAccessIterator& s) const{
	if(this->stride==0){throwNullStrideException();};
	if(this->stride!=s.stride){throwUmatchedStrideException();};
	return (this->current-s.current)/(this->stride);
      } // end of operator==
    private:
      //! current position
      iterator_base current;
      //! stride value
      difference_type stride;
    };

    template<typename iterator_base>
    StridedRandomAccessIterator<iterator_base>
    operator+(const typename StridedRandomAccessIterator<iterator_base>::difference_type n,
	      const StridedRandomAccessIterator<iterator_base> p){
      return p+n;
    }

    template<typename iterator_base>
    StridedRandomAccessIterator<iterator_base>
    operator-(const typename StridedRandomAccessIterator<iterator_base>::difference_type n,
	      const StridedRandomAccessIterator<iterator_base> p){
      return p-n;
    }
    
  } // end of namespace math

} // end of namespace tfel

#endif /* LIB_TFEL_MATH_STRIDEDRANDOMACCESSITERATOR_HXX_ */
