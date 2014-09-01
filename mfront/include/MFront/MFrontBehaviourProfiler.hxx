/*! 
 * \file  mfront/include/MFront/MFrontBehaviourProfiler.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 15 mai 2014
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_MFRONT_MFRONTBEHAVIOURPROFILER_H_
#define _LIB_MFRONT_MFRONTBEHAVIOURPROFILER_H_ 

#if !(defined _WIN32 || defined _WIN64 ||defined __CYGWIN__)
#include<time.h>
#endif

#include"MFront/MFrontConfig.hxx"

#include<array>
#include<atomic>
#include<string>

namespace mfront{

  /*!
   * structure in charge of performance measurements in mechanical
   * behaviour
   */
  struct MFRONTTIMER_VISIBILITY_EXPORT MFrontBehaviourProfiler
  {
    /*!
     * a timer for a specicied code block.
     * This descructor will increase the time count for the code block.
     */
    struct Timer
    {
      /*!
       * \param[in,out] t  : global timer
       * \param[in]     cn : code block 
       */
      Timer(MFrontBehaviourProfiler&,
	    const unsigned short);
      //! destructor
      ~Timer();
    protected:
      //! timer to which the results are reported
      MFrontBehaviourProfiler& gtimer;
      //! code block associated with the timer
      const unsigned short c;
#if !(defined _WIN32 || defined _WIN64 ||defined __CYGWIN__)
      //! start
      timespec start;
      //! end
      timespec end;
#endif
    }; // end of struct Timer
    /*!
     * constructor
     * \param[in] n : behaviour name
     */
    MFrontBehaviourProfiler(const std::string&);
    /*
     * \see MechanialBehaviourData for a description
     */
    //! code block index in the measures array
    static constexpr unsigned short FLOWRULE = 0;
    //! code block index in the measures array
    static constexpr unsigned short BEFOREINITIALIZELOCALVARIABLES = 1;
    //! code block index in the measures array
    static constexpr unsigned short INITIALIZELOCALVARIABLES = 2;
    //! code block index in the measures array
    static constexpr unsigned short AFTERINITIALIZELOCALVARIABLES = 3;
    //! code block index in the measures array
    static constexpr unsigned short COMPUTEPREDICTOR = 4;
    //! code block index in the measures array
    static constexpr unsigned short COMPUTEPREDICTIONOPERATOR = 5;
    //! code block index in the measures array
    static constexpr unsigned short INTEGRATOR = 6;
    //! code block index in the measures array
    static constexpr unsigned short COMPUTESTRESS = 7;
    //! code block index in the measures array
    static constexpr unsigned short COMPUTEDERIVATIVE = 8;
    //! code block index in the measures array
    static constexpr unsigned short INITIALIZEJACOBIAN = 9;
    //! code block index in the measures array
    static constexpr unsigned short COMPUTEFDF = 10;
    //! code block index in the measures array
    static constexpr unsigned short TINYMATRIXSOLVE = 11;
    //! code block index in the measures array
    static constexpr unsigned short COMPUTEFINALSTRESS = 12;
    //! code block index in the measures array
    static constexpr unsigned short COMPUTETANGENTOPERATOR = 13;
    //! code block index in the measures array
    static constexpr unsigned short UPDATEAUXILIARYSTATEVARIABLES = 14;
    //! code block index in the measures array
    static constexpr unsigned short FINITESTRAINPREPROCESSING  = 15;
    //! code block index in the measures array
    static constexpr unsigned short FINITESTRAINPOSTPROCESSING = 16;
    //! code block index in the measures array
    static constexpr unsigned short USERDEFINEDCODE1 = 17;
    //! code block index in the measures array
    static constexpr unsigned short USERDEFINEDCODE2 = 18;
    //! code block index in the measures array
    static constexpr unsigned short TOTALTIME  = 19;
    //! destructor
    ~MFrontBehaviourProfiler();
  protected:
    //! name of the behaviour
    const std::string name;
    //! time spend in each code block (nanoseconds)
    std::array<std::atomic<intmax_t>,20> measures;
  }; // end of MFrontBehaviourProfiler

} // end of namespace mfront

#endif /* _LIB_MFRONT_MFRONTBEHAVIOURPROFILER_H */

