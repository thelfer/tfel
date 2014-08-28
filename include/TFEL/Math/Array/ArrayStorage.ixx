/*!
 * \file   include/TFEL/Math/Array/ArrayStorage.ixx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   02 jui 2006
 */

#ifndef _LIB_TFEL_ARRAY_STORAGE_IXX_
#define _LIB_TFEL_ARRAY_STORAGE_IXX_ 

#include <string>

#include"TFEL/Config/TFELConfig.hxx"

namespace tfel{
  
  namespace math {
    
    template<typename T,typename Ordering,typename TAllocator>
    class ArrayStorage_
      : public Ordering
    {
      
      ArrayStorage_(const ArrayStorage_&);
      
    protected:

      T* v;

      ~ArrayStorage_()
      {
	if(this->v!=0){
	  TAllocator::template free<T>(this->v);
	}
      };

    public:

      typedef typename Ordering::RunTimeProperties RunTimeProperties;

      ArrayStorage_()
	: v(0)
      {}

      ArrayStorage_(const RunTimeProperties& src)
	: Ordering(src)
      {
	this->v = TAllocator::template alloc<T>(this->size);
      }

      void resize(const RunTimeProperties& src)
      {
	Ordering::operator=(src);
	this->v = TAllocator::template alloc<T>(this->v,this->size);
      }

      static const bool storage_is_dynamic  = true;
      static const bool storage_is_static   = false;
      static const bool storage_is_external = false;
    };

    template<typename T,typename Ordering,typename TAllocator>
    struct ArrayStorage<1u,T,Ordering,TAllocator>
      : public ArrayStorage_<T,Ordering,TAllocator>
    {

      typedef typename ArrayStorage_<T,Ordering,TAllocator>::RunTimeProperties RunTimeProperties;
      
      ArrayStorage(const RunTimeProperties& src)
	: ArrayStorage_<T,Ordering,TAllocator>(src)
      {};

      TFEL_MATH_INLINE T operator()(const unsigned int i) const
      {
	return *(this->v+i);
      }
      TFEL_MATH_INLINE T& operator()(const unsigned int i)
      {
	return *(this->v+i);
      }
    };

    template<typename T,typename Ordering,typename TAllocator>
    struct ArrayStorage<2u,T,Ordering,TAllocator>
      : public ArrayStorage_<T,Ordering,TAllocator>
    {

      typedef typename ArrayStorage_<T,Ordering,TAllocator>::RunTimeProperties RunTimeProperties;

      ArrayStorage(const RunTimeProperties& src)
	: ArrayStorage_<T,Ordering,TAllocator>(src)
      {};

      TFEL_MATH_INLINE T operator()(const unsigned int i,const unsigned int j) const
      {
	return *(this->v+this->getOffset(i,j));
      }
      TFEL_MATH_INLINE T& operator()(const unsigned int i,const unsigned int j)
      {
	return *(this->v+this->getOffset(i,j));
      }
    };

    template<typename T,typename Ordering,typename TAllocator>
    struct ArrayStorage<3u,T,Ordering,TAllocator>
      : public ArrayStorage_<T,Ordering,TAllocator>
    {

      typedef typename ArrayStorage_<T,Ordering,TAllocator>::RunTimeProperties RunTimeProperties;

      ArrayStorage(const RunTimeProperties& src)
	: ArrayStorage_<T,Ordering,TAllocator>(src)
      {};

      TFEL_MATH_INLINE T operator()(const unsigned int i,const unsigned int j,const unsigned int k) const
      {
	return *(this->v+this->getOffset(i,j,k));
      }
      TFEL_MATH_INLINE T& operator()(const unsigned int i,const unsigned int j,const unsigned int k)
      {
	return *(this->v+this->getOffset(i,j,k));
      }
    };
    
  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_ARRAY_STORAGE_IXX */

