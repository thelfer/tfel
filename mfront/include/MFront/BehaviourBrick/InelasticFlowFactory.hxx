/*!
 * \file   include/MFront/BehaviourBrick/InelasticFlowFactory.hxx
 * \brief
 * \author Thomas Helfer
 * \date 15/03/2018
 */

#ifndef LIB_MFRONT_BEHAVIOURBRICK_INELASTICFLOWFACTORY_HXX
#define LIB_MFRONT_BEHAVIOURBRICK_INELASTICFLOWFACTORY_HXX

#include <map>
#include <string>
#include <memory>
#include <functional>
#include "MFront/MFrontConfig.hxx"

namespace mfront::bbrick {

  // forward declaration
  struct InelasticFlow;

  //! \brief abstract factory for inelastic Flows.
  struct MFRONT_VISIBILITY_EXPORT InelasticFlowFactory {
    //! \brief a simple alias
    using Generator = std::function<std::shared_ptr<InelasticFlow>()>;
    //! \return the uniq instance of the class
    [[nodiscard]] static InelasticFlowFactory& getFactory();
    //
    InelasticFlowFactory(InelasticFlowFactory&&) = delete;
    InelasticFlowFactory(const InelasticFlowFactory&) = delete;
    InelasticFlowFactory& operator=(InelasticFlowFactory&&) = delete;
    InelasticFlowFactory& operator=(const InelasticFlowFactory&) = delete;
    /*!
     * \brief add a new generator
     * \param[in] n: name of the generator
     * \param[in] g: generator
     */
    void addGenerator(const std::string&, const Generator&);
    //! \return the list of available inelastic flows
    [[nodiscard]] std::vector<std::string> getRegistredInelasticFlows() const;
    /*!
     * \brief generate a new inelastic flow
     * \param[in] n: name of the inelastic flow
     */
    [[nodiscard]] std::shared_ptr<InelasticFlow> generate(
        const std::string&) const;

   private:
    //! \brief default constructor
    InelasticFlowFactory();
    //! \brief destructor
    ~InelasticFlowFactory();
    //! \brief generators
    std::map<std::string, Generator> generators;
  };  // end of struct InelasticFlowFactory

}  // end of namespace mfront::bbrick

#endif /* LIB_MFRONT_BEHAVIOURBRICK_INELASTICFLOWFACTORY_HXX */
