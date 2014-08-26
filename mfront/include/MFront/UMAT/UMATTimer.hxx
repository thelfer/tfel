/*! 
 * \file  UMATTimer.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 14 mai 2014
 */

#ifndef _LIB_MFRONT_UMAT_UMATTIMER_H_
#define _LIB_MFRONT_UMAT_UMATTIMER_H_ 

#include<atomic>

#include"MFront/UMAT/UMATConfig.hxx"

namespace umat
{
  /*!
   * structure in charge of measuring the execution time of a behaviour
   */
  struct MFRONT_UMATTIMER_VISIBILITY_EXPORT UMATTimer
  {
    /*!
     * constructor
     * \param[in] n : name of the behaviour
     */
    UMATTimer(const char* const);
    /*!
     * add a mesure to the total time spend in the behaviour integration
     * \param[in] start : beginning of the integration
     * \param[in] end   : end       of the integration
     */
    void addTotalTimeMeasure(const timespec&,
			     const timespec&);
    /*!
     * add a mesure to the time spend in the finite strain pre-processing
     * \param[in] start : beginning of the pre-processing
     * \param[in] end   : end       of the pre-processing
     */
    void addFiniteStrainPreProcessingTimeMeasure(const timespec&,
						 const timespec&);
    /*!
     * add a mesure to the time spend in the finite strain post-processing
     * \param[in] start : beginning of the post-processing
     * \param[in] end   : end       of the post-processing
     */
    void addFiniteStrainPostProcessingTimeMeasure(const timespec&,
						  const timespec&);
    /*!
     * destructor
     */
    ~UMATTimer();
  protected:
    //! name of the behaviour
    const char* const name;
    //! total time spend in the beahviour integration in nanoseconds
    std::atomic<intmax_t> nnsec{0};
    //! finite strain pre-processing
    std::atomic<intmax_t> fs1_nsec{0};
    //! finite strain post-processing
    std::atomic<intmax_t> fs2_nsec{0};
    //! number of calls
    std::atomic<intmax_t> ncalls{0};
  }; // end of struct UMATTimer

} // end of namespace umat

#endif /* _LIB_MFRONT_UMAT_UMATTIMER_H */

