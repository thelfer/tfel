/*!
 * \file   TestLauncher.hxx
 * \brief  This file declares the TestLauncher class
 * \author Helfer Thomas
 * \date   28 Jan 2008
 */

#ifndef _LIB_TFEL_UTILITIES_TESTLAUNCHER_H_
#define _LIB_TFEL_UTILITIES_TESTLAUNCHER_H_ 

#include<ostream>
#include<fstream>
#include<vector>
#include<string>
#include<map>
#include<set>

#include <sys/times.h> 

#include"Utilities/CxxTokenizer.hxx"
#include"Utilities/TextData.hxx"

namespace tfel
{
  namespace utilities
  {

    struct TestLauncher
      : public tfel::utilities::CxxTokenizer
    {
      TestLauncher(const std::string&,
		   std::ostream&);
      bool
      execute(void);
      ~TestLauncher(void);
    private:
      typedef void (TestLauncher::*  CallBack)(void);
      enum TestType {
	Absolute,
	Relative
      }; // end of enum TestType
      struct Test
      {
	std::string    name;
	unsigned short column;
	bool byName;
	double prec;
	TestType type;
      }; // end of struct Test
      //! list of all tokens of the file, sorted by line
      std::vector<std::vector<tfel::utilities::Token> > tokens;
      std::vector<std::vector<tfel::utilities::Token> >::const_iterator line;
      std::vector<tfel::utilities::Token>::const_iterator current;
      //! list of files comparison to be made
      std::map<std::string,std::map<std::string,std::vector<Test> > > comparisons;
      std::map<std::string,CallBack> callBacks;
      std::string logfile;
      std::ofstream log;	/**< logfile output stream */
      std::string file;		/**< file containing tfel-check commands */
      std::vector<std::string> commands;
      std::vector<std::string> meshCommands;
      std::string XMLResult;	/**< string containing testcase result, Junit formatted. */
      double prec;
      TestType testType;
      tms timeStart, timeStop;	/**< tms struct used by ClockAction */
      void registerCallBack(const std::string&,const CallBack&);
      //! analyse the input file
      void
      analyseInputFile(void);
      void
      treatTest(void);
      void
      treatPrecision(void);
      void
      treatCommand(void);
      void
      treatMeshCommand(void);
      void
      treatTestType(void);

      void addTestcaseXML(const std::string& name , 
			  const float time, 
			  const bool success, 
			  const std::string& message);

      void addTestcaseXML(const std::string& name , 
			  const std::string& command, 
			  const float time, 
			  const bool success, 
			  const std::string& message);

      void writeXML(void); 

      std::string logToString (void);

      enum ClockEventType { START, STOP, GET };	/**< ClockAction supported event. */
      double ClockAction(ClockEventType clockevent);

      /*!
       * global log (given by tfel check)
       */
      std::ostream& glog;

    }; // end of struct TestLauncher

  } // end of namespace utilities

} // end of namespace tfel

#endif /* _LIB_TFEL_UTILITIES_TESTLAUNCHER_H */

