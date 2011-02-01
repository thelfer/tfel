#ifndef _ST2TOST2_CONCEPT_LIB_
#define _ST2TOST2_CONCEPT_LIB_ 1

#include <ostream>
#include <string>

#include"TFEL/Config/TFELConfig.hxx"

#include"TFEL/Utilities/Name.hxx"

#include"TFEL/Metaprogramming/EnableIf.hxx"
#include"TFEL/Metaprogramming/Implements.hxx"
#include"TFEL/Metaprogramming/InvalidType.hxx"
#include"TFEL/Metaprogramming/IF.hxx"
#include"TFEL/TypeTraits/IsTemporary.hxx"

namespace tfel{

  namespace math{

    template<class T>
    struct ST2toST2Traits{
      typedef tfel::meta::InvalidType NumType;
      static const unsigned short dime = 0u;
    };

    /*!
     * \class St2toSt2Tag
     * \brief Helper class to characterise st2tost2.
     */ 
    struct St2toSt2Tag
    {
      /*!
       * \brief  Return the name of the class.
       * \param  void.
       * \return const std::string, the name of the class.
       * \see    Name.
       */
      static 
      const std::string 
      getName(void);
    };

    template<class T>
    class ST2toST2Concept 
    {
      
      typedef ST2toST2Traits<T> traits;
      static const bool isTemporary = tfel::typetraits::IsTemporary<T>::cond;
      typedef typename tfel::meta::IF<isTemporary,
				      typename traits::NumType,
				      const typename traits::NumType&>::type ValueType;

    protected:
      ~ST2toST2Concept(){};
      
    public:
      
      typedef St2toSt2Tag ConceptTag;

      static
      std::string 
      getName(void);

      ValueType
      operator()(const unsigned short,
		 const unsigned short) const;
      
    };

    // Serialisation operator
    template<typename T>
    std::ostream &
    operator << (std::ostream &,
		 const ST2toST2Concept<T>&);

    template<typename T>
    struct ST2toST2Type{
      typedef T type;
    };

  } // end of namespace math

} // end of namespace tfel

#include"TFEL/Math/ST2toST2/ST2toST2Concept.ixx"

#endif /* _ST2TOST2_CONCEPT_LIB_ */
