/*!
 * \file   DifferenceRandomAccessIterator.hxx
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

#ifndef LIB_TFEL_MATH_DIFFERENCERANDOMACCESSITERATOR_HXX
#define LIB_TFEL_MATH_DIFFERENCERANDOMACCESSITERATOR_HXX

#include"TFEL/Config/TFELConfig.hxx"
#include<iterator>

namespace tfel{

  namespace math{

    /*!
     * \brief base class of the DifferenceRandomAccessIterator containing
     * some static common utility methods.
     */
    struct TFELMATH_VISIBILITY_EXPORT DifferenceRandomAccessIteratorBase
    {
      //! 
      TFEL_NORETURN static void throwInconsistentIteratorDifference(void);
    };
    
    /*!
     * \tparam iterator_base: 
     * \brief an adaptator to random access iterator
     */
    template<typename iterator_base>
    struct DifferenceRandomAccessIterator
      : public DifferenceRandomAccessIteratorBase
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
      explicit DifferenceRandomAccessIterator(const iterator_base i1,
					      const iterator_base i2)
	: first(i1),
	  second(i2)
      {} // end of DifferenceRandomAccessIterator
      //! default constructor
      explicit DifferenceRandomAccessIterator()
	: first(iterator_base()),
	  second(iterator_base())
      {}
      //! move constructor
      DifferenceRandomAccessIterator(DifferenceRandomAccessIterator&&) = default;
      //! copy constructor
      DifferenceRandomAccessIterator(const DifferenceRandomAccessIterator&) = default;
      //! move assignement
      DifferenceRandomAccessIterator&
      operator=(DifferenceRandomAccessIterator&&) = default;
      //! standard assignement
      DifferenceRandomAccessIterator&
      operator=(const DifferenceRandomAccessIterator&) = default;
      
      value_type operator*() const {
	return *(this->first)-*(this->second);
      }

      reference operator[](const difference_type n) const
      {
	return *(*this+n);
      }
      
      DifferenceRandomAccessIterator operator++(){
	std::advance(this->first,1);
	std::advance(this->second,1);
	return *this;
      }
      
      DifferenceRandomAccessIterator operator++(int){
	auto c = *this;
	std::advance(this->first,1);
	std::advance(this->second,1);
	return c;
      }

      DifferenceRandomAccessIterator operator+(const difference_type n) const{
	auto c = *this;
	std::advance(this->first,n);
	std::advance(this->second,n);
	return c;
      }
      
      DifferenceRandomAccessIterator operator-(const difference_type n) const{
	return *this+(-n);
      }

      DifferenceRandomAccessIterator operator+=(const difference_type n) const{
	std::advance(this->first,n);
	std::advance(this->second,n);
	return *this;
      }
      
      bool operator==(const DifferenceRandomAccessIterator& o) const{
	return ((this->first==o.first)&&(this->second==o.second));
      } // end of operator==

      bool operator!=(const DifferenceRandomAccessIterator& o) const{
	return !(this->operator==(o));
      } // end of operator==

      difference_type operator-(const DifferenceRandomAccessIterator& s) const{
	const difference_type d = this->first-s.first;
	if(d!=(this->second-s.second)){
	  throwInconsistentIteratorDifference();
	}
	return d;
      } // end of operator==
    private:
      //! first iterator of the operation
      iterator_base first;
      //! second iterator of the operation
      iterator_base second;
    };

    template<typename iterator_base>
    DifferenceRandomAccessIterator<iterator_base>
    operator+(const typename DifferenceRandomAccessIterator<iterator_base>::difference_type n,
	      const DifferenceRandomAccessIterator<iterator_base> p){
      return p+n;
    }

    template<typename iterator_base>
    DifferenceRandomAccessIterator<iterator_base>
    operator-(const typename DifferenceRandomAccessIterator<iterator_base>::difference_type n,
	      const DifferenceRandomAccessIterator<iterator_base> p){
      return p-n;
    }
    
  } // end of namespace math

} // end of namespace tfel

#endif /* LIB_TFEL_MATH_DIFFERENCERANDOMACCESSITERATOR_HXX_ */
