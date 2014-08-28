/*!
 * \file   include/TFEL/Math/ST2toT2/ST2toT2Concept.hxx  
 * \brief    
 * \author Helfer Thomas
 * \date   19 November 2013
 */

#ifndef _TFEL_MATH_ST2TOT2_CONCEPT_LIB_
#define _TFEL_MATH_ST2TOT2_CONCEPT_LIB_ 1

#include <ostream>
#include <string>

#include"TFEL/Config/TFELConfig.hxx"

#include"TFEL/Metaprogramming/EnableIf.hxx"
#include"TFEL/Metaprogramming/Implements.hxx"
#include"TFEL/Metaprogramming/InvalidType.hxx"
#include"TFEL/Metaprogramming/IF.hxx"
#include"TFEL/TypeTraits/IsTemporary.hxx"

#include"TFEL/Math/General/Abs.hxx"
#include"TFEL/Math/Forward/ST2toT2Concept.hxx"

namespace tfel{

  namespace math{

    template<class T>
    struct ST2toT2Traits{
      typedef tfel::meta::InvalidType NumType;
      static const unsigned short dime = 0u;
    };

    /*!
     * \class ST2toT2Tag
     * \brief Helper class to characterise st2tot2.
     */ 
    struct ST2toT2Tag
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
    struct ST2toT2Concept 
    {

    private:

      typedef ST2toT2Traits<T> traits;
      static const bool isTemporary = tfel::typetraits::IsTemporary<T>::cond;
      typedef typename tfel::meta::IF<isTemporary,
				      typename traits::NumType,
				      const typename traits::NumType&>::type ValueType;

    protected:
      ~ST2toT2Concept(){};
      
    public:
      
      typedef ST2toT2Tag ConceptTag;

      ValueType
      operator()(const unsigned short,
		 const unsigned short) const;
      
    };

    // Serialisation operator
    template<typename T>
    std::ostream &
    operator << (std::ostream &,
		 const ST2toT2Concept<T>&);

    template<typename T>
    struct ST2toT2Type{
      typedef T type;
    };

    template<typename ST2toT2Type>
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<ST2toT2Type,ST2toT2Concept>::cond,
      typename tfel::typetraits::AbsType<typename ST2toT2Traits<ST2toT2Type>::NumType>::type
    >::type
    abs(const ST2toT2Type&);

  } // end of namespace math

} // end of namespace tfel

#include"TFEL/Math/ST2toT2/ST2toT2Concept.ixx"

#endif /* _LIB_TFEL_MATH_ST2TOT2_CONCEPT_LIB_ */
