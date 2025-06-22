/*!
 * \file  mfront/include/MFront/ModelDSL.hxx
 * \brief
 * \author Thomas Helfer
 * \brief 11 jun 2010
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONTMODELPARSER_HXX
#define LIB_MFRONTMODELPARSER_HXX

#include "MFront/MFrontConfig.hxx"
#include "MFront/ModelDSLBase.hxx"
#include "MFront/ModelDescription.hxx"

namespace mfront {

  /*!
   * \brief default class for analysing models
   */
  struct MFRONT_VISIBILITY_EXPORT ModelDSL : public ModelDSLBase<ModelDSL> {
    //! \return the name of the dsl
    static std::string getName();
    //! \return the description of the dsl
    static std::string getDescription();
    /*!
     * \brief add interfaces used for generating output files
     * \param[in] i: list of interfaces
     */
    virtual void setInterfaces(const std::set<std::string>&) override;
    /*!
     * \return a copy of the description of the model treated by the
     * dsl.
     * \note This method shall be called after the `analyseFile`
     * method.
     */
    virtual ModelDescription getModelDescription() const;
  };  // end of ModelDSL

}  // end of namespace mfront

#endif /* LIB_MFRONTMODELPARSER_HXX */
