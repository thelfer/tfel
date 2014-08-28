/*!
 * \file   include/TFEL/Math/T2toT2/T2toT2/T2toST2Concept.hxx  
 * \brief    
 * \author Helfer Thomas
 * \date   19 November 2013
 */

#ifndef _TFEL_MATH_T2TOST2_CONCEPT_LIB_
#define _TFEL_MATH_T2TOST2_CONCEPT_LIB_ 1

#include <ostream>
#include <string>

#include"TFEL/Config/TFELConfig.hxx"

#include"TFEL/Metaprogramming/EnableIf.hxx"
#include"TFEL/Metaprogramming/Implements.hxx"
#include"TFEL/Metaprogramming/InvalidType.hxx"
#include"TFEL/Metaprogramming/IF.hxx"
#include"TFEL/TypeTraits/IsTemporary.hxx"

#include"TFEL/Math/General/Abs.hxx"
#include"TFEL/Math/Forward/T2toST2Concept.hxx"

namespace tfel{

  namespace math{

    template<class T>
    struct T2toST2Traits{
      typedef tfel::meta::InvalidType NumType;
      static const unsigned short dime = 0u;
    };

    /*!
     * \class T2tost2Tag
     * \brief Helper class to characterise t2tost2.
     */ 
    struct T2tost2Tag
    {
      /*!
       * \brief  Return the name of the class.
       * \param  void.
       * \return const std::string, the name of the class.
       * \see    Name.
       */
      static std::string 
      getName(void);
    };

    template<class T>
    struct T2toST2Concept 
    {

    private:

      typedef T2toST2Traits<T> traits;
      static const bool isTemporary = tfel::typetraits::IsTemporary<T>::cond;
      typedef typename tfel::meta::IF<isTemporary,
				      typename traits::NumType,
				      const typename traits::NumType&>::type ValueType;

    protected:
      ~T2toST2Concept(){};
      
    public:
      
      typedef T2tost2Tag ConceptTag;

      ValueType
      operator()(const unsigned short,
		 const unsigned short) const;
      
    };

    // Serialisation operator
    template<typename T>
    std::ostream &
    operator << (std::ostream &,
		 const T2toST2Concept<T>&);

    template<typename T>
    struct T2toST2Type{
      typedef T type;
    };

    template<typename T2toST2Type>
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<T2toST2Type,T2toST2Concept>::cond,
      typename tfel::typetraits::AbsType<typename T2toST2Traits<T2toST2Type>::NumType>::type
    >::type
    abs(const T2toST2Type&);

  } // end of namespace math

} // end of namespace tfel

#include"TFEL/Math/T2toST2/T2toST2Concept.ixx"

#endif /* _LIB_TFEL_MATH_T2TOST2_CONCEPT_LIB_ */
