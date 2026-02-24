/*!
 * \file  BehaviourDocumentationGenerator.hxx
 * \brief
 * \author Thomas Helfer
 * \date   04 mars 2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_BEHAVIOURDOCUMENTATIONGENERATOR_H
#define LIB_MFRONT_BEHAVIOURDOCUMENTATIONGENERATOR_H

#include <string>
#include <vector>
#include <memory>
#include <utility>
#include <functional>

#include "MFront/DocumentationGeneratorBase.hxx"

namespace mfront {

  /*!
   * Class used by the mfront-query tool to extract information from
   * behaviour implementation
   */
  struct BehaviourDocumentationGenerator : public DocumentationGeneratorBase {
    /*!
     * build a BehaviourDocumentationGenerator object based on command line
     * arguments
     * \param[in] argc : number of command line arguments
     * \param[in] argv : command line arguments
     * \param[in] d    : behaviour domain specific language
     * \param[in] f    : behaviour domain specific language
     */
    BehaviourDocumentationGenerator(const int,
                                    const char *const *const,
                                    std::shared_ptr<AbstractBehaviourDSL>,
                                    const std::string &);
    //! treat the requests
    void exe() const override;
    //! destructor
    ~BehaviourDocumentationGenerator() override;

    struct VariableInformation
        : DocumentationGeneratorBase::VariableInformationBase {
      using Hypothesis = tfel::material::ModellingHypothesis::Hypothesis;
      VariableInformation();
      VariableInformation(VariableInformation &&) noexcept;
      VariableInformation(const VariableInformation &);
      VariableInformation &operator=(VariableInformation &&);
      VariableInformation &operator=(const VariableInformation &);
      ~VariableInformation() noexcept;
      std::map<Hypothesis, std::string> descriptions;
      std::vector<Hypothesis> hypotheses;
    };

   private:
    void writeWebOutput(std::ostream &,
                        const BehaviourDescription &,
                        const FileDescription &) const;
    void writeFullOutput(std::ostream &,
                         const BehaviourDescription &,
                         const FileDescription &) const;
    //! abstract behaviour dsl
    std::shared_ptr<AbstractBehaviourDSL> dsl;
  };  // end of struct BehaviourDocumentationGenerator

}  // end of namespace mfront

#endif /* LIB_MFRONT_BEHAVIOURDOCUMENTATIONGENERATOR_H */
