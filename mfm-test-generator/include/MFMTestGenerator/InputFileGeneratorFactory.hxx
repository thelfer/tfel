/*!
 * \file   InputFileGeneratorFactory.hxx
 * \brief
 * \author Thomas Helfer
 * \date   17/09/2019
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFM_TEST_INPUTFILEGENERATORFACTORY_HXX
#define LIB_MFM_TEST_INPUTFILEGENERATORFACTORY_HXX

#include <map>
#include <string>
#include <memory>
#include <functional>
#include "MFMTestGenerator/Config.hxx"

namespace mfmtg {

  // forward declaration
  struct AbstractTestCase;

  //! \brief an abstract factory for test cases.
  struct MFMTG_VISIBILITY_EXPORT InputFileGeneratorFactory {
    //! a simple alias
    using generator = std::function<void(const AbstractTestCase&)>;
    //! \return the uniq instance of this class
    static InputFileGeneratorFactory& get();
    /*!
     * \brief register a new input file generator for the given test case.
     * \param[in] n: name of the test case
     * \param[in] t: name of the targeted code
     * \param[in] g: input file generator
     */
    void add(const std::string&, const std::string&, const generator);
    /*!
     * \return a input file generator for the given test case and the given
     * targeted code \param[in] n: name of the test case \param[in] i: name of
     * the targeted code
     */
    generator get(const std::string&, const std::string&) const;

   private:
    //! \brief default constructor
    InputFileGeneratorFactory();
    //! \brief move constructor (disabled)
    InputFileGeneratorFactory(InputFileGeneratorFactory&&) = delete;
    //! \brief copy constructor (disabled)
    InputFileGeneratorFactory(const InputFileGeneratorFactory&) = delete;
    //! \brief move assignement (disabled)
    InputFileGeneratorFactory& operator=(InputFileGeneratorFactory&&) = delete;
    //! \brief standard assignement (disabled)
    InputFileGeneratorFactory& operator=(const InputFileGeneratorFactory&) =
        delete;
    //! destructor
    ~InputFileGeneratorFactory();
    //! list of registred generators
    std::map<std::string,           // test case
             std::map<std::string,  // interface
                      generator>>
        generators;
  };  // end of struct InputFileGeneratorFactory

}  // end of namespace mfmtg

#endif /* LIB_MFM_TEST_INPUTFILEGENERATORFACTORY_HXX */
