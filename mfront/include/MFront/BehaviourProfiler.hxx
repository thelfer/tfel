/*!
 * \file  mfront/include/MFront/BehaviourProfiler.hxx
 * \brief
 * \author Thomas Helfer
 * \brief 15 mai 2014
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_MFRONTBEHAVIOURPROFILER_HXX
#define LIB_MFRONT_MFRONTBEHAVIOURPROFILER_HXX

#include "TFEL/Config/TFELConfig.hxx"
#if defined _MSC_VER
#define MFRONTBEHAVIOURPROFILER_CONST_QUALIFIER const
#else
#define MFRONTBEHAVIOURPROFILER_CONST_QUALIFIER constexpr const
#include <time.h>
#endif

#include "MFront/MFrontConfig.hxx"

#include <array>
#include <atomic>
#include <string>

namespace mfront {

  /*!
   * structure in charge of performance measurements in mechanical
   * behaviour
   */
  struct MFRONTPROFILING_VISIBILITY_EXPORT BehaviourProfiler {
    //! a simple alias
    using index_type = unsigned short;
    /*!
     * a timer for a specicied code block.
     * This descructor will increase the time count for the code block.
     */
    struct MFRONTPROFILING_VISIBILITY_EXPORT Timer {
      /*!
       * \param[in,out] t  : global timer
       * \param[in]     cn : code block
       */
      Timer(BehaviourProfiler&, const index_type);
      //! destructor
      ~Timer();

     protected:
      Timer(const Timer&) = default;
      Timer(Timer&&) = default;
      Timer& operator=(const Timer&) = delete;
      Timer& operator=(Timer&&) = delete;
      //! timer to which the results are reported
      BehaviourProfiler& gtimer;
      //! code block associated with the timer
      const index_type c;
#if !(defined _WIN32 || defined _WIN64 || defined __APPLE__)
      //! start
      timespec start;
      //! end
      timespec end;
#endif
    };  // end of struct Timer
    /*!
     * constructor
     * \param[in] n : behaviour name
     */
    explicit BehaviourProfiler(const std::string&);
    /*
     * \see MechanialBehaviourData for a description
     */
    //! code block index in the measures array
    static MFRONTBEHAVIOURPROFILER_CONST_QUALIFIER index_type FLOWRULE = 0;
    //! code block index in the measures array
    static MFRONTBEHAVIOURPROFILER_CONST_QUALIFIER index_type
        BEFOREINITIALIZELOCALVARIABLES = 1;
    //! code block index in the measures array
    static MFRONTBEHAVIOURPROFILER_CONST_QUALIFIER index_type
        INITIALIZELOCALVARIABLES = 2;
    //! code block index in the measures array
    static MFRONTBEHAVIOURPROFILER_CONST_QUALIFIER index_type
        AFTERINITIALIZELOCALVARIABLES = 3;
    //! code block index in the measures array
    static MFRONTBEHAVIOURPROFILER_CONST_QUALIFIER index_type COMPUTEPREDICTOR =
        4;
    //! code block index in the measures array
    static MFRONTBEHAVIOURPROFILER_CONST_QUALIFIER index_type
        COMPUTEPREDICTIONOPERATOR = 5;
    //! code block index in the measures array
    static MFRONTBEHAVIOURPROFILER_CONST_QUALIFIER index_type INTEGRATOR = 6;
    //! code block index in the measures array
    static MFRONTBEHAVIOURPROFILER_CONST_QUALIFIER index_type COMPUTESTRESS = 7;
    //! code block index in the measures array
    static MFRONTBEHAVIOURPROFILER_CONST_QUALIFIER index_type
        COMPUTEDERIVATIVE = 8;
    //! code block index in the measures array
    static MFRONTBEHAVIOURPROFILER_CONST_QUALIFIER index_type
        INITIALIZEJACOBIAN = 9;
    //! code block index in the measures array
    static MFRONTBEHAVIOURPROFILER_CONST_QUALIFIER index_type COMPUTEFDF = 10;
    //! code block index in the measures array
    static MFRONTBEHAVIOURPROFILER_CONST_QUALIFIER index_type TINYMATRIXSOLVE =
        11;
    //! code block index in the measures array
    static MFRONTBEHAVIOURPROFILER_CONST_QUALIFIER index_type
        COMPUTEFINALSTRESS = 12;
    //! code block index in the measures array
    static MFRONTBEHAVIOURPROFILER_CONST_QUALIFIER index_type
        COMPUTETANGENTOPERATOR = 13;
    //! code block index in the measures array
    static MFRONTBEHAVIOURPROFILER_CONST_QUALIFIER index_type
        UPDATEAUXILIARYSTATEVARIABLES = 14;
    //! code block index in the measures array
    static MFRONTBEHAVIOURPROFILER_CONST_QUALIFIER index_type
        FINITESTRAINPREPROCESSING = 15;
    //! code block index in the measures array
    static MFRONTBEHAVIOURPROFILER_CONST_QUALIFIER index_type
        FINITESTRAINPOSTPROCESSING = 16;
    //! code block index in the measures array
    static MFRONTBEHAVIOURPROFILER_CONST_QUALIFIER index_type USERDEFINEDCODE1 =
        17;
    //! code block index in the measures array
    static MFRONTBEHAVIOURPROFILER_CONST_QUALIFIER index_type USERDEFINEDCODE2 =
        18;
    //! code block index in the measures array
    static MFRONTBEHAVIOURPROFILER_CONST_QUALIFIER index_type
        ADDITIONALCONVERGENCECHECKS = 19;
    static MFRONTBEHAVIOURPROFILER_CONST_QUALIFIER index_type
        APRIORITIMESTEPSCALINGFACTOR = 20;
    static MFRONTBEHAVIOURPROFILER_CONST_QUALIFIER index_type
        APOSTERIORITIMESTEPSCALINGFACTOR = 21;
    //! code block index in the measures array
    static MFRONTBEHAVIOURPROFILER_CONST_QUALIFIER index_type TOTALTIME = 22;
    //! destructor
    ~BehaviourProfiler();

   protected:
    //! name of the behaviour
    const std::string name;
    //! time spend in each code block (nanoseconds)
    std::array<std::atomic<intmax_t>, 23> measures;
  };  // end of BehaviourProfiler

}  // end of namespace mfront

#endif /* LIB_MFRONT_MFRONTBEHAVIOURPROFILER_HXX */
