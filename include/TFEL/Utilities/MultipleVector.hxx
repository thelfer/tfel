#ifndef _LIB_TFEL_MULTIPLEVECTOR_H_
#define _LIB_TFEL_MULTIPLEVECTOR_H_ 1

#include <string>
#include <vector>

#include"TFEL/Utilities/Name.hxx"

#include"TFEL/Utilities/MultipleDequeVector/TypeTransform.hxx"
#include"TFEL/Utilities/MultipleDequeVector/MultipleDequeVector.hxx"

namespace tfel{
  
  namespace utilities{

    /*!
     * \class  MultipleVector
     * \brief  The MultipleVector enables storage of every elements in a TypeList.
     * \param  T, TypeList.
     * \param  Transform, tells how elements are handled.
     * \see    TypeTransform.hxx
     * \author Helfer Thomas
     * \date   28 Aug 2006
     */
    template<typename T, template<class> class Transform=tfel::utilities::IdTransform>
    class  MultipleVector
      : public tfel::utilities::MultipleDequeVectorContainer<T,Transform,std::vector>
    {
      /*
       * Return the name of the class.
       * \param  void.
       * \return std::string, the name of the class.
       * \see    Name.
       */
      static
      const 
      std::string 
      getName(void){
	using namespace std;
	return string("MultipleVector<")+
	Name<T>::getName()+string(",")+
	Transform<T>::getName()+string(">");
      }
    };
    
  } // end of namespace utilities

} // end of namespace tfel

#endif /* _LIB_TFEL_MULTIPLEVECTOR_H_ */
