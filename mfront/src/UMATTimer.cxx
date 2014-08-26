/*! 
 * \file  UMATTimer.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 14 mai 2014
 */

#include<iostream>
#include<atomic>

#if !(defined _WIN32 || defined _WIN64 ||defined __CYGWIN__)
#include<time.h>
#endif


#include"MFront/UMAT/UMATTimer.hxx"

namespace umat
{

#if (defined _WIN32 || defined _WIN64 ||defined __CYGWIN__)
  typedef intmax_t timespec;
#endif

  /*!
   * add a new measure
   * t     : measure to which the new measure is added
   * start : start of the measure
   * end   : end of the measure
   */
  static inline void
  add_measure(std::atomic<intmax_t>& t,
	      const timespec& start,
	      const timespec& end)
  {
    /* http://www.guyrutenberg.com/2007/09/22/profiling-code-using-clock_gettime */
    timespec temp;
    if ((end.tv_nsec-start.tv_nsec)<0) {
      temp.tv_sec = end.tv_sec-start.tv_sec-1;
      temp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
    } else {
      temp.tv_sec = end.tv_sec-start.tv_sec;
      temp.tv_nsec = end.tv_nsec-start.tv_nsec;
    }
    t += 1000000000*temp.tv_sec+temp.tv_nsec;
  } // end of add_measure

  /*!
   * print a time to the specified stream
   */
  static void print_time(std::ostream& os,
			 const intmax_t time)
  {
    constexpr intmax_t musec_d = 1000;
    constexpr intmax_t msec_d  = 1000*musec_d;
    constexpr intmax_t sec_d   = msec_d*1000;
    constexpr intmax_t min_d   = sec_d*60;
    constexpr intmax_t hour_d  = min_d*60;
    constexpr intmax_t days_d  = hour_d*24;
    intmax_t t = time;
    const intmax_t ndays  = t/days_d;
    t -= ndays*days_d;
    const intmax_t nhours = t/hour_d;
    t -= nhours*hour_d;
    const intmax_t nmins  = t/min_d;
    t -= nmins*min_d;
    const intmax_t nsecs   = t/sec_d;
    t -= nsecs*sec_d;
    const intmax_t nmsecs   = t/msec_d;
    t -= nmsecs*msec_d;
    const intmax_t nmusecs   = t/musec_d;
    t -= nmusecs*musec_d;
    if(ndays>0){
      os << ndays << "days ";
    }
    if(nhours>0){
      os << nhours << "hours ";
    }
    if(nmins>0){
      os << nmins << "mins ";
    }
    if(nsecs>0){
      os << nsecs << "secs ";
    }
    if(nmsecs>0){
      os << nmsecs << "msecs ";
    }
    if(nmusecs>0){
      os << nmusecs << "musecs ";
    }
    os << t << "nsecs";
  } // end pf print

  UMATTimer::UMATTimer(const char* const n)
    : name{n}
  {} // end of UMATTimer

  void UMATTimer::addTotalTimeMeasure(const timespec& start,
				      const timespec& end)
  {
    umat::add_measure(this->nnsec,start,end);
    this->ncalls += 1;
  }

  void UMATTimer::addFiniteStrainPreProcessingTimeMeasure(const timespec& start,
							  const timespec& end)
  {
    umat::add_measure(this->fs1_nsec,start,end);
  }

  void UMATTimer::addFiniteStrainPostProcessingTimeMeasure(const timespec& start,
							   const timespec& end)
  {
    umat::add_measure(this->fs2_nsec,start,end);
  }

  UMATTimer::~UMATTimer()
  {
    using namespace std;
    cout << this->name << ", number of calls  : " << this->ncalls << endl;
    cout << this->name << ", total time spend : " ;
    print_time(cout,this->nnsec);
    cout << endl;
    if(this->ncalls>0){
      cout << this->name << ", mean time : ";
      print_time(cout,this->nnsec/this->ncalls);
      cout << endl;
    }
    if(this->fs1_nsec>0){
      cout << this->name << ", time spend in finite strain pre-processing : " ;
      print_time(cout,this->nnsec);
      cout << endl;
    }
    if(this->fs2_nsec>0){
      cout << this->name << ", time spend in finite strain post-processing : " ;
      print_time(cout,this->nnsec);
      cout << endl;
    }
  }

} // end of namespace umat
