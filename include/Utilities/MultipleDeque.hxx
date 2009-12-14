#ifndef _LIB_TFEL_MULTIPLEDEQUE_H_
#define _LIB_TFEL_MULTIPLEDEQUE_H_ 1

#include <string>
#include <deque>

#include "Utilities/Name.hxx"

#include "Utilities/MultipleDequeVector/TypeTransform.hxx"
#include "Utilities/MultipleDequeVector/MultipleDequeVector.hxx"

namespace tfel{

  namespace utilities{
    
    /*!
     * \class  MultipleDeque
     * \brief  The MultipleDeque enables storage of every elements in a TypeList.
     * \param  T, TypeList.
     * \param  Transform, tells how elements are handled.
     * \see    TypeTransform.hxx
     * \author Helfer Thomas
     * \date   28 Aug 2006
     */
    template<typename T,template<class> class Transform=tfel::utilities::IdTransform>
    class  MultipleDeque
      : public tfel::utilities::MultipleDequeVectorContainer<T,Transform,std::deque>
    {
      /*
       * Return the name of the class.
       * \param  void.
       * \return std::string, the name of the class.
       * \see    Name.
       */
      static
      const std::string
      getName(void){
	using namespace std;
	return string("MultipleDeque<")+
	Name<T>::getName()+string(",")+
	Transform<T>::getName()+string(">");
      }
    };
 
  } // end of namespace utilities

} // end of namespace tfel

#endif /* _LIB_TFEL_MULTIPLEDEQUE_H_ */
