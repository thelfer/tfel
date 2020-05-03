/*!
 * \file   UserDefinedPostProcessing.hxx
 * \brief    
 * \author Thomas Helfer
 * \date   27/02/2018
 */

#ifndef LIB_MTEST_USERDEFINEDPOSTPROCESSING_HXX
#define LIB_MTEST_USERDEFINEDPOSTPROCESSING_HXX

#include <memory>
#include <vector>
#include <fstream>
#include "TFEL/Math/Evaluator.hxx"

namespace mtest{

  // forward declaration
  struct Behaviour;

  //! \brief class handling a user defined postprocessing
  struct UserDefinedPostProcessing {
    /*!
     * \brief constructor
     * \param[in] b: behaviour
     * \param[in] e: evolution manager
     * \param[in] f: file name
     * \param[in] ps: list of postprocessings
     */
    UserDefinedPostProcessing(const Behaviour&,
                              const EvolutionManager&,
                              const std::string&,
                              const std::vector<std::string>&);
    /*!
     * \param[in] s: current material state
     * \param[in] t  : time
     * \param[in] dt : time increment
     */
    void exe(const CurrentState&, const real, const real);
    //! destructor
    ~UserDefinedPostProcessing();

   private:
    //! list of post-processings
    std::vector<std::shared_ptr<tfel::math::Evaluator>> postprocessings;
    //! list of function object used to retrieve values from the current state
    std::map<std::string, std::function<real(const CurrentState&)>> extractors;
    //! evolution manager
    const EvolutionManager& evm;
    //! output file
    std::ofstream out;
  };  // end of UserDefinedPostProcessing
  
}  // end of namespace mtest

#endif /* LIB_MTEST_USERDEFINEDPOSTPROCESSING_HXX */
