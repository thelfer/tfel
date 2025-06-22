/*!
 * \file   Data.hxx
 * \brief
 * \author THOMAS HELFER
 * \date   12 sept. 2016
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_UTILITIES_DATA_HXX
#define LIB_TFEL_UTILITIES_DATA_HXX

#include <map>
#include <string>
#include <vector>
#include <utility>
#include <functional>
#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Utilities/GenTypeBase.hxx"
#include "TFEL/Utilities/CxxTokenizer.hxx"

namespace tfel {

  namespace utilities {

    // forward declaration
    struct Data;
    //! list of all type handled by the Data structure
    using DataTypes =
        tfel::meta::GenerateTypeList<bool,
                                     int,
                                     double,
                                     std::string,
                                     std::vector<Data>,
                                     std::map<std::string, Data>>::type;
    /*!
     * Data extracted from json like structure
     */
    struct TFELUTILITIES_VISIBILITY_EXPORT Data
        : public GenTypeBase<DataTypes> {
      //! a simple alias
      using CallBack = std::function<void(const Data&)>;
      //! constructor from a value
      template <typename T1,
                typename std::enable_if<
                    tfel::meta::TLCountNbrOfT<typename std::decay<T1>::type,
                                              DataTypes>::value == 1,
                    bool>::type = true>
      TFEL_INLINE Data(T1&& v) : GenTypeBase<DataTypes>(std::forward<T1>(v)) {}
      /*!
       * \brief read a JSON-like structure
       * \return the values read
       * \param[in,out] p:  current position in the tokens stream
       * \param[in]     pe: past-the-end iterator
       */
      static Data read(CxxTokenizer::const_iterator&,
                       const CxxTokenizer::const_iterator);
      /*!
       * \brief read a map. For each read key, a callback must
       * available to treat the associated data. The result of the
       * treatment is stored in the returned value.
       * \return treated values
       * \param[in,out] p:  current position in the tokens stream
       * \param[in]     pe: past-the-end iterator
       * \param[in]     callbacks: list of call-backs
       */
      static void parse(CxxTokenizer::const_iterator&,
                        const CxxTokenizer::const_iterator,
                        const std::map<std::string, CallBack>&);
      /*!
       * \brief read a standard value (string,double,int)
       * \return the values read
       * \param[in,out] p:  current position in the tokens stream
       * \param[in]     pe: past-the-end iterator
       */
      static Data read_value(CxxTokenizer::const_iterator&,
                             const CxxTokenizer::const_iterator);
      /*!
       * \brief read a map
       * \return the values read
       * \param[in,out] p:  current position in the tokens stream
       * \param[in]     pe: past-the-end iterator
       */
      static Data read_vector(CxxTokenizer::const_iterator&,
                              const CxxTokenizer::const_iterator);
      /*!
       * \brief read a map
       * \return the values read
       * \param[in,out] p:  current position in the tokens stream
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
      Data& operator=(Data&&);
      //! assignement
      Data& operator=(const Data&);
      //! destructor
      ~Data();
    };

    //! comparison operator
    TFELUTILITIES_VISIBILITY_EXPORT bool operator==(const Data&, const Data&);

    /*!
     * \brief extract a value of the given type from a Data structure
     * \param[in] d: data
     * \return the extracted value
     */
    template <typename T>
    T extract(const Data&);
    /*!
     * \param[in] d: data extract a vector of string from a Data
     *               structure.
     * \return the extracted value.
     * \pre The Data must hold a vector of Data and each Data of the
     *      vector must be a string
     */
    template <>
    TFELUTILITIES_VISIBILITY_EXPORT std::vector<std::string>
    extract<std::vector<std::string>>(const Data&);

  }  // end of namespace utilities

}  // end of namespace tfel

#endif /* LIB_TFEL_UTILITIES_DATA_HXX_ */
