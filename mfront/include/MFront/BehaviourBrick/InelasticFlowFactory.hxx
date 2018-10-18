/*!
 * \file   include/MFront/BehaviourBrick/InelasticFlowFactory.hxx
 * \brief
 * \author Thomas Helfer
 * \date   15/03/2018
 */

#ifndef LIB_MFRONT_BEHAVIOURBRICK_INELASTICFLOWFACTORY_HXX
#define LIB_MFRONT_BEHAVIOURBRICK_INELASTICFLOWFACTORY_HXX

#include <map>
#include <string>
#include <memory>
#include <functional>

namespace mfront {

  namespace bbrick {

    // forward declaration
    struct InelasticFlow;

    /*!
     * \brief abstract factory for inelastic Flows.
     */
    struct InelasticFlowFactory {
      //! a simple alias
      using Generator = std::function<std::shared_ptr<InelasticFlow>()>;
      //! \return the uniq instance of the class
      static InelasticFlowFactory& getFactory();
      /*!
       * \brief add a new generator
       * \param[in] n: name of the generator
       * \param[in] g: generator
       */
      void addGenerator(const std::string&, const Generator&);
      //! \return the list of available inelastic flows
      std::vector<std::string> getRegistredInelasticFlows() const;
      /*!
       * \brief generate a new inelastic flow
       * \param[in] n: name of the inelastic flow
       */
      std::shared_ptr<InelasticFlow> generate(const std::string&) const;

     private:
      //! default constructor
      InelasticFlowFactory();
      //! move constructor (deleted)
      InelasticFlowFactory(InelasticFlowFactory&&) = delete;
      //! copy constructor (deleted)
      InelasticFlowFactory(const InelasticFlowFactory&) = delete;
      //! move assignement (deleted)
      InelasticFlowFactory& operator=(InelasticFlowFactory&&) =
          delete;
      //! standard assignement(deleted)
      InelasticFlowFactory& operator=(const InelasticFlowFactory&) =
          delete;
      //! \brief destructor
      ~InelasticFlowFactory();
      //! \brief generators
      std::map<std::string, Generator> generators;
    };  // end of struct InelasticFlowFactory

  }  // end of namespace bbrick

}  // end of namespace mfront

#endif /* LIB_MFRONT_BEHAVIOURBRICK_INELASTICFLOWFACTORY_HXX */
