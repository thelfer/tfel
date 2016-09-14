/*!
 * \file   Data.hxx
 * \brief    
 * \author THOMAS HELFER
 * \date   12 sept. 2016
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */


#ifndef LIB_TFEL_UTILITIES_DATA_HXX
#define LIB_TFEL_UTILITIES_DATA_HXX

#include<map>
#include<string>
#include<vector>
#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/Utilities/GenTypeBase.hxx"
#include"TFEL/Utilities/CxxTokenizer.hxx"

namespace tfel{

  namespace utilities{

    // forward declaration
    struct Data;
    //! list of all type handled by the Data structure
    using DataTypes = tfel::meta::GenerateTypeList<int,double,std::string,
						   std::vector<Data>,
						   std::map<std::string,Data>>::type;
    /*!
     * Data extracted from json like structure
     */
    struct TFELUTILITIES_VISIBILITY_EXPORT Data
      : public tfel::utilities::GenTypeBase<DataTypes>
    {
      using tfel::utilities::GenTypeBase<DataTypes>::GenTypeBase;
      /*!
       * \brief read a JSON-like structure
       * \return the values read
       * \param[in/out] p:  current position in the tokens stream
       * \param[in]     pe: past-the-end iterator
       */
      static Data read(CxxTokenizer::const_iterator&,
		       const CxxTokenizer::const_iterator);
      /*!
       * \brief read a standard value (string,double,int)
       * \return the values read
       * \param[in/out] p:  current position in the tokens stream
       * \param[in]     pe: past-the-end iterator
       */
      static Data read_value(CxxTokenizer::const_iterator&,
			     const CxxTokenizer::const_iterator);
      /*!
       * \brief read a map
       * \return the values read
       * \param[in/out] p:  current position in the tokens stream
       * \param[in]     pe: past-the-end iterator
       */
      static Data read_vector(CxxTokenizer::const_iterator&,
			      const CxxTokenizer::const_iterator);
      /*!
       * \brief read a map
       * \return the values read
       * \param[in/out] p:  current position in the tokens stream
       * \param[in]     pe: past-the-end iterator
       */
      static Data read_map(CxxTokenizer::const_iterator&,
			   const CxxTokenizer::const_iterator);
      //! default constructor
      Data();
      //! move constructor
      Data(Data&&);
      //! copy constructor
      Data(const Data&);
      //! move assignement
      Data& operator = (Data&&);
      //! assignement
      Data& operator = (const Data&);
      //! destructor
      ~Data();
    };

    //! comparison operator
    bool operator==(const Data&,const Data&);

    /*!
     * \brief extract a value of the given type from a Data structure
     * \param[in] d: data
     * \return the extracted value
     */
    template<typename T>
    T extract(const Data&);
    /*!
     * \param[in] d: data extract a vector of string from a Data
     *               structure.
     * \return the extracted value.
     * \pre The Data must hold a vector of Data and each Data of the
     *      vector must be a string
     */
    template<>
    std::vector<std::string>
    extract<std::vector<std::string>>(const Data&);
    
  } // end of namespace utilities

} // end of namespace tfel

#endif /* LIB_TFEL_UTILITIES_DATA_HXX_ */
