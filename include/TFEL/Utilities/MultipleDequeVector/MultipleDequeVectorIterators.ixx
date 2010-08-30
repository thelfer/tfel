/*!
 * \file   MultipleDequeVectorIterators.ixx
 * \brief  
 * \author Helfer Thomas
 * \date   01 jun 2006
 */

#ifndef _LIB_TFEL_MULTIPLEDEQUEVECTORITERATORS_IXX_
#define _LIB_TFEL_MULTIPLEDEQUEVECTORITERATORS_IXX_ 

namespace tfel{

  namespace utilities{

    template<typename T,
	     template<class> class Transform,
	     template<class,class> class container,
	     typename Allocator>
    template<typename T1>
    struct MultipleDequeVectorContainer<T,Transform,container,Allocator>::iterator
    {

      typedef typename container<typename Transform<T1>::type,
				 typename Allocator::template rebind<T1>::other>::iterator::iterator_category iterator_category;
      typedef typename Transform<T1>::value_type value_type;
      typedef typename Transform<T1>::pointer    pointer;
      typedef typename Transform<T1>::reference  reference;

      typedef MultipleDequeVectorContainer<T,Transform,container,Allocator> Container;
      
      // Constructor with an iterator
      inline iterator(typename container<typename Transform<T1>::type,
		      typename Allocator::template rebind<T1>::other>::iterator p)
	: p_(p)
      {}

      // Default constructor
      inline iterator()
      {}

      // Copy constructor
      inline iterator(const typename
		      Container::template iterator<T1>& src)
	: p_(src.getIterator())
      {}

      // Assignement operator
      inline iterator& operator= (const typename 
				  Container::template iterator<T1>&
				  src){
	this->p_ = src.getIterator();
	return *this;
      }
      
      inline iterator operator + (int n){
	return iterator(this->p_+n);
      }

      inline iterator operator - (int n){
	return iterator(this->p_-n);
      }

      inline iterator& operator += (int n){
	p_+=n;
	return *this;
      }

      inline iterator& operator -= (int n){
	p_-=n;
	return *this;
      }

      inline void operator++(){
	++(this->p_);
      }

      inline void operator++(int){
	(this->p_)++;
      }
      
      inline void operator--(){
	--(this->p_);
      }

      inline void operator--(int){
	(this->p_)--;
      }

      inline bool
      operator==(const typename
		 Container::template iterator<T1>& src) const {
	return this->p_==src.getIterator();
      }
      
      inline bool
      operator!=(const typename
		 Container::template iterator<T1>& src) const {
	return this->p_!=src.getIterator();
      }
      
      inline pointer operator->(void){
	return Transform<T1>::to_pointer(*(this->p_));
      }

      inline reference operator*(void){
	return Transform<T1>::reverse_transform(*(this->p_));
      }

      typename container<typename Transform<T1>::type,
			 typename Allocator::template rebind<T1>::other>::iterator
      getIterator()
      {
	return this->p_;
      }

      typename container<typename Transform<T1>::type,
                         typename Allocator::template rebind<T1>::other>::iterator
      getIterator() const
      {
	return this->p_;
      }

    private:

      typename container<typename Transform<T1>::type,
                         typename Allocator::template rebind<T1>::other>::iterator p_;

      TFEL_STATIC_ASSERT((tfel::meta::TLCountNbrOfT<T1,T>::value==1));

    };

    template<typename T,
	     template<class> class Transform,
	     template<class,class> class container,
	     typename Allocator>
    template<typename T1>
    struct MultipleDequeVectorContainer<T,Transform,container,Allocator>::const_iterator
    {

      typedef typename container<typename Transform<T1>::type,
typename Allocator::template rebind<T1>::other>::const_iterator::iterator_category iterator_category;
      typedef typename Transform<T1>::value_type      value_type;
      typedef typename Transform<T1>::pointer         pointer;
      typedef typename Transform<T1>::const_pointer   const_pointer;
      typedef typename Transform<T1>::const_reference const_reference;
      
      typedef MultipleDequeVectorContainer<T,Transform,container,Allocator> Container;

      // Constructor with an const_iterator
      inline const_iterator(typename container<typename Transform<T1>::type,
			    typename Allocator::template rebind<T1>::other>::const_iterator p)
	: p_(p)
      {}

      // Default constructor
      inline const_iterator(){}

      // Copy constructor
      inline const_iterator(const Container::const_iterator<T1>& src)
	: p_(src.getIterator())
      {}

      // Assignement operator
      inline const_iterator&
      operator= (const Container::const_iterator<T1>& src){
	this->p_ = src.getIterator();
	return *this;
      }
      
      // Assignement operator
      inline const_iterator&
      operator = (const Container::iterator<T1>& src){
	this->p_ = src.getIterator();
	return *this;
      }

      inline const_iterator operator + (int n){
	return const_iterator(this->p_+n);
      }

      inline const_iterator operator - (int n){
	return const_iterator(this->p_-n);
      }

      inline const_iterator& operator += (int n){
	p_+=n;
	return *this;
      }

      inline const_iterator& operator -= (int n){
	p_-=n;
	return *this;
      }

      inline void operator++(){
	++(this->p_);
      }

      inline void operator++(int){
	(this->p_)++;
      }
      
      inline void operator--(){
	--(this->p_);
      }
      
      inline void operator--(int){
	(this->p_)--;
      }

      inline bool operator==(const Container::const_iterator<T1>& src) const {
	return this->p_==src.getIterator();
      }
      
      inline bool operator!=(const Container::const_iterator<T1>& src) const{
	return p_!=src.getIterator();
      }
      
      inline bool operator==(const Container::iterator<T1>& src) const {
	return this->p_==src.getIterator();
      }
      
      inline bool operator!=(const Container::iterator<T1>& src) const{
	return this->p_!=src.getIterator();
      }

      inline const_pointer operator->(void) const{
	return Transform<T1>::to_pointer(*(this->p_));
      }

      inline const_reference operator*(void){
	return Transform<T1>::reverse_transform(*(this->p_));
      }

      inline typename container<typename Transform<T1>::type,
				typename Allocator::template rebind<T1>::other>::const_iterator
      getIterator() const
      {
	return this->p_;
      }

      inline typename container<typename Transform<T1>::type,
				typename Allocator::template rebind<T1>::other>::const_iterator
      getIterator()
      {
	return this->p_;
      }

    private:

      typename container<typename Transform<T1>::type,
			 typename Allocator::template rebind<T1>::other>::const_iterator p_;

      TFEL_STATIC_ASSERT((tfel::meta::TLCountNbrOfT<T1,T>::value==1));

    };


    template<typename T, template<class> class Transform,
	     template<class,class> class container,
	     typename Allocator>
    template<typename T1>
    struct MultipleDequeVectorContainer<T,Transform,container,Allocator>::reverse_iterator
    {

      typedef typename container<typename Transform<T1>::type,
				 typename Allocator::template rebind<T1>::other>::reverse_iterator::iterator_category  iterator_category;
      typedef typename Transform<T1>::value_type value_type;
      typedef typename Transform<T1>::pointer    pointer;
      typedef typename Transform<T1>::reference  reference;
      typedef MultipleDequeVectorContainer<T,Transform,container,Allocator> Container;

      // Constructor with an reverse_iterator
      inline reverse_iterator(typename container<typename Transform<T1>::type,
			      typename Allocator::template rebind<T1>::other>::reverse_iterator p)
	: p_(p)
      {}

      // Default constructor
      inline reverse_iterator(){}

      // Copy constructor
      inline reverse_iterator(const Container::reverse_iterator<T1>& src)
	: p_(src.getIterator())
      {}

      // Assignement operator
      inline reverse_iterator& operator= (const Container::reverse_iterator<T1>& src){
	this->p_ = src.getIterator();
	return *this;
      }
      
      inline reverse_iterator operator + (int n){
	return reverse_iterator(this->p_+n);
      }

      inline reverse_iterator operator - (int n){
	return reverse_iterator(this->p_-n);
      }

      inline reverse_iterator& operator += (int n){
	p_+=n;
	return *this;
      }

      inline reverse_iterator& operator -= (int n){
	p_-=n;
	return *this;
      }

      inline void operator++(){
	++(this->p_);
      }

      inline void operator++(int){
	(this->p_)++;
      }

      inline void operator--(){
	--(this->p_);
      }

      inline void operator--(int){
	(this->p_)--;
      }

      inline bool operator==(const Container::reverse_iterator<T1>& src) const {
	return this->p_==src.getIterator();
      }
      
      inline bool operator!=(const Container::reverse_iterator<T1>& src) const {
	return this->p_!=src.getIterator();
      }
      
      inline pointer operator->(void){
	return Transform<T1>::to_pointer(*(this->p_));
      }

      inline reference operator*(void){
	return Transform<T1>::reverse_transform(*(this->p_));
      }

      typename container<typename Transform<T1>::type,
			 typename Allocator::template rebind<T1>::other>::reverse_iterator
      getIterator() const
      {
	return this->p_;
      }

      typename container<typename Transform<T1>::type,
			 typename Allocator::template rebind<T1>::other>::reverse_iterator
      getIterator()
      {
	return this->p_;
      }
      
    private:

      typename container<typename Transform<T1>::type,
			 typename Allocator::template rebind<T1>::other>::reverse_iterator p_;

      TFEL_STATIC_ASSERT((tfel::meta::TLCountNbrOfT<T1,T>::value==1));

    };

    template<typename T,
	     template<class> class Transform,
	     template<class,class> class container,
	     typename Allocator>
    template<typename T1>
    struct MultipleDequeVectorContainer<T,Transform,container,Allocator>::const_reverse_iterator
    {

      typedef typename container<typename Transform<T1>::type,
				 typename Allocator::template rebind<T1>::other>::const_reverse_iterator::iterator_category iterator_category;
      typedef typename Transform<T1>::value_type value_type;
      typedef typename Transform<T1>::const_pointer    const_pointer;
      typedef typename Transform<T1>::const_reference  const_reference;
      typedef MultipleDequeVectorContainer<T,Transform,container,Allocator> Container;

      // Constructor with an const_reverse_iterator
      inline const_reverse_iterator(typename container<typename Transform<T1>::type,
				    typename Allocator::template rebind<T1>::other>::const_reverse_iterator p)
	: p_(p)
      {}

      // Default constructor
      inline const_reverse_iterator(){}

      // Copy constructor
      inline const_reverse_iterator(const Container::const_reverse_iterator<T1>& src)
	: p_(src.getIterator())
      {}

      // Assignement operator
      inline const_reverse_iterator& operator= (const Container::const_reverse_iterator<T1>& src){
	p_ = src.getIterator();
	return *this;
      }

      // Assignement operator
      inline const_reverse_iterator& operator= (const Container::reverse_iterator<T1>& src){
	this->p_ = src.getIterator();
	return *this;
      }

      
      inline const_reverse_iterator operator + (int n){
	return const_reverse_iterator(this->p_+n);
      }

      inline const_reverse_iterator operator - (int n){
	return const_reverse_iterator(this->p_-n);
      }

      inline const_reverse_iterator& operator += (int n){
	p_+=n;
	return *this;
      }

      inline const_reverse_iterator& operator -= (int n){
	p_-=n;
	return *this;
      }

      inline void operator++(){
	++(this->p_);
      }

      inline void operator++(int){
	(this->p_)++;
      }

      inline void operator--(){
	--(this->p_);
      }

      inline void operator--(int){
	(this->p_)--;
      }
      
      inline bool operator==(const Container::const_reverse_iterator<T1>& src) const {
	return this->p_==src.getIterator();
      }
      
      inline bool operator!=(const Container::const_reverse_iterator<T1>& src) const{
	return this->p_!=src.getIterator();
      }

      inline bool operator==(const Container::reverse_iterator<T1>& src) const {
	return this->p_==src.getIterator();
      }
      
      inline bool operator!=(const Container::reverse_iterator<T1>& src) const{
	return p_!=src.getIterator();
      }
      
      inline const_pointer operator->(void) const{
	return Transform<T1>::to_pointer(*(this->p_));
      }

      inline const_reference operator*(void) const{
	return Transform<T1>::reverse_transform(*(this->p_));
      }

      typename container<typename Transform<T1>::type,
			 typename Allocator::template rebind<T1>::other>::const_reverse_iterator
      getIterator()
      {
	return this->p_;
      }

      typename container<typename Transform<T1>::type,
			 typename Allocator::template rebind<T1>::other>::const_reverse_iterator
      getIterator() const
      {
	return this->p_;
      }

    private:

      typename container<typename Transform<T1>::type,
			 typename Allocator::template rebind<T1>::other>::const_reverse_iterator p_;

      TFEL_STATIC_ASSERT((tfel::meta::TLCountNbrOfT<T1,T>::value==1));

    };

  }

}

#endif /* _LIB_TFEL_MULTIPLEDEQUEVECTORITERATORS_IXX */
