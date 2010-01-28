/*!
 * \file   IntegersSet.hxx  
 * \brief  This file implements the IntergerSet class.
 * \author Helfer Thomas
 * \date   13 mai 2006
 */

#ifndef _LIB_TFEL_INTEGERSSET_HXX_
#define _LIB_TFEL_INTEGERSSET_HXX_ 

#include<string>
#include<utility>
#include<vector>
#include<limits>

#include<ostream>

#include"TFEL/Utilities/Name.hxx"

namespace tfel{

  namespace utilities{

    /*!
     * \class  IntegersSet
     * \brief  The IntegerSet is a container of integers. 
     * The integers are stocked from the smallest one the largest one
     * and are packed by intervals.
     *
     * \param  IntergerType, type of the integers being stored.
     *
     * \author Helfer Thomas
     * \date   13 mai 2006
     */    
    template<typename IntegerType = unsigned int>
    class IntegersSet{
      
      /*!
       * \brief ContainerType typedef.
       */
      typedef std::vector<std::pair<IntegerType,IntegerType> > ContainerType;

      /*!
       * Members
       */
      ContainerType container;

      /*!
       * Size of the Container
       */
      typename ContainerType::size_type size_;

      /*!
       * \brief  Returns the smallest element of the container.
       * \note   Returns std::numeric_limits<IntegerType>::max() if the container is empty.
       * \param  void.
       * \return IntegerType.
       */
      IntegerType min(void) const{
	if(container.empty()){
	  return std::numeric_limits<IntegerType>::max();
	}
	return container[0].first;
      }

      /*!
       * \brief  Return the largest element of the container.
       * \note   Returns std::numeric_limits<IntegerType>::min() if the container is empty.
       * \param  void.
       * \return IntegerType.
       */
      IntegerType max(void) const{
	if(container.empty()){
	  return std::numeric_limits<IntegerType>::min();
	}      
	return container[container.size()-1].second;
      }

    public:

      /*!
       * \brief  Return the name of the class.
       * \param  void.
       * \return const std::string.
       * \see Name
       */
      static
      const std::string 
      getName(void){
	return std::string("IntegerSet<")+Name<IntegerType>::getName()+std::string(">");
      }

      /*!
       * \brief Default Constructor
       */
      IntegersSet()
	: size_(0)
      {}

      /*!
       * \brief Copy Constructor
       * \param IntegersSet, the source.
       */
      IntegersSet(const IntegersSet& src)
	: container(src.container),
	  size_(0)
      {}

      /*!
       * \brief Constructor for an interval.
       * \param Ibegin, beginning of the interval.
       * \param Iend, end of the interval.
       */
      IntegersSet(const IntegerType Ibegin, const IntegerType Iend){
	if(Ibegin>Iend){
	  container.push_back(std::make_pair(Iend,Ibegin));
	  this->size_=Ibegin-Iend+1;
	} else{
	  container.push_back(std::make_pair(Ibegin,Iend));
	  this->size_=Iend-Ibegin+1;
	}
      };

      /*!
       * \brief Assignement Opertor
       * \param IntegersSet, the source.
       */
      inline IntegersSet& operator = (const IntegersSet& src){
	this->container = src.container;
	this->size_     = src.size_;
	return *this;
      };

      /*!
       * \brief Add an integer.
       * \param src, the integer to be added.
       * \return void.
       */
      inline void add(const IntegerType src){

	typename ContainerType::iterator p;
	typename ContainerType::iterator q;
	IntegerType Imin=min();
	IntegerType Imax=max();

	if(container.empty()){
	  container.push_back(std::make_pair(src,src));
	  ++(this->size_);
	  return;
	}
	if(src<Imin){
	  if(src==Imin-1){
	    --(container[0].first);
	    ++(this->size_);
	    return;
	  } else {
	    container.insert(container.begin(),std::make_pair(src,src));	  
	    ++(this->size_);
	    return;
	  }
	}
	if(src>Imax){
	  if(src==Imax+1){
	    ++(container[container.size()-1]).second;
	    ++(this->size_);
	    return;
	  } else {
	    container.push_back(std::make_pair(src,src));	  
	    ++(this->size_);
	    return;
	  }
	}

	// Add somewhere in the middle of the list
	for(p=container.begin();p!=container.end();++p){
	  if((src>=p->first)&&(src<=p->second)){
	    // Already in
	    return;
	  }

	  if(src==p->second+1){
	    // Add to the end of an existing pair
	    q = p;
	    ++q;
	    if(q==container.end()){
	      p->second=src;
	      ++(this->size_);
	      return;
	    } 
	    if(src==q->first-1){
	      p->second=q->second;
	      container.erase(q);
	      ++(this->size_);
	      return;
	    } 
	    p->second=src;
	    ++(this->size_);
	    return;
	  }

	  if(src<p->first){
	    if(src==p->first-1){
	      --(p->first);
	      ++(this->size_);
	      return;
	    }
	    container.insert(p,std::make_pair(src,src));
	    ++(this->size_);
	    return;
	  }
	
	}
	return;
      }

      /*!
       * \class   IntegersSetConstIterator
       * \brief   Const iterator through an IntegerSet
       * \warning This is not a conforming implementation.
       * \author  Helfer Thomas
       * \date    13 mai 2006
       */
      class IntegersSetConstIterator{
      
	/*!
	 * iterator for the container
	 */
	typename ContainerType::const_iterator p;
	/*!
	 * The current value
	 */
	IntegerType current;
      
      public:

	/*!
	 * Default Constructor
	 */
	IntegersSetConstIterator()
	  : p(0),
	    current(static_cast<IntegerType>(0))
	{}

	/*!
	 * Copy Constructor
	 */
	IntegersSetConstIterator(const IntegersSetConstIterator& src)
	  : p(src.p),
	    current(src.current)
	{}

	/*! 
	 * Constructor
	 */
	IntegersSetConstIterator(const typename ContainerType::const_iterator& src)
	  : p(src),
	    current(0)
	{}

	/*!
	 * Assignement Operator
	 */
	IntegersSetConstIterator& operator = (const IntegersSetConstIterator& src)
	{
	  p= src.p;
	  current = src.current;
	  return *this;
	}

	/*!
	 * Deference operator
	 */
	IntegerType operator * (void) const{
	  return p->first+current;
	}

	/*!
	 * operator !=
	 */
	bool operator != (const IntegersSetConstIterator& src)
	{
	  if((p != src.p)||(current!=src.current)){
	    return true;
	  }
	  return false;
	}

	/*!
	 * operator++
	 */
	void operator++(void){
	  if(p->first+current == p->second){
	    ++p;
	    current = 0;
	    return;
	  }
	  ++current;
	}

	/*!
	 * operator++(int)
	 */
	void operator++(int){
	  if(p->first+current == p->second){
	    ++p;
	    current = 0;
	    return;
	  }
	  ++current;
	}

      };
    
      /*!
       * Type of the const_iterator
       */
      typedef IntegersSetConstIterator const_iterator;
    
      /*!
       * Return an iterator to the beginning of the container
       */
      inline const_iterator begin(void) const{
	return const_iterator(container.begin());
      };

      /*!
       * Return an iterator to the beginning of the end of the container
       */
      inline const_iterator end(void) const{
	return const_iterator(container.end());
      };
    
      /*!
       * Return the position of a given integer.
       */
      inline typename ContainerType::size_type position(const IntegerType src) const {
	typename ContainerType::size_type i = 0;
	typename ContainerType::const_iterator p;
	for(p=container.begin();p!=container.end();++p){
	  if((src>=p->first)&&(src<=p->second)){
	    i+=src-p->first+1;
	    return i-1;
	  }
	  i+=p->second-p->first+1;
	}
	return i-1;
      } 

      /*!
       * Return the number of integers in the container.
       */
      inline typename ContainerType::size_type size(void) const {
	return this->size_;
      } 

      /*!
       * Return the number of intervals in the container.
       */
      inline typename ContainerType::size_type block_number(void) const {
	return this->container.size();
      } 

      /*!
       * Prints the intervals contained int the container.
       */
      void printInfo(std::ostream& os) const {
	typename ContainerType::const_iterator p;
	for(p=container.begin();p!=container.end();++p){
	  os << "(" << p->first << "," << p->second << ")" << " ";
	}
	os << std::endl;
      }

    };

  } // end of namespace utilities

} // end of namespace tfel

#endif /* _LIB_TFEL_INTEGERSSET_HXX */

