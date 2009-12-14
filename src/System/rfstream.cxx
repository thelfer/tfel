/*!
 * \file   rfstream.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   14 nov 2007
 */

#include<iostream>

#include"System/System.hxx"
#include"System/rfstream.hxx"

namespace tfel
{

  namespace system
  {
    
    rfstream::rfstream()
      : basic_rstream<rfstream,stream_traits<rfstream> >(),
	tfel::utilities::SmartPtr<int>(-1)
    {} // end of rfstream::rfstream

    rfstream::rfstream(const rfstream& src)
      : basic_rstream<rfstream,stream_traits<rfstream> >(src),
	tfel::utilities::SmartPtr<int>(src)
    {} // end of rfstream::rfstream

    rfstream &
    rfstream::operator=(const rfstream& src)
    {
      using namespace tfel::utilities;
      if(this==&src){
	return *this;
      }
      this->close();
      basic_rstream<rfstream,stream_traits<rfstream> >::operator=(src);
      SmartPtr<int>::operator=(src);
      return *this;
    } // end of rfstream::operator

    rfstream::rfstream(const std::string& name, const int flags)
      : basic_rstream<rfstream,stream_traits<rfstream> >(),
	tfel::utilities::SmartPtr<int>(-1)
    {
      this->open(name,flags);
    } // end of rfstream::rfstream

    void
    rfstream::open(const std::string& name,
		   const int flags)
    {
      using namespace std;
      using namespace tfel::utilities;
      int fd;
      if(*(this->p)!=-1){
	// closing the previous file
	this->close();
      }
      fd=::open(name.c_str(),flags);
      if(fd==-1){
	string msg("rfstream::open : ");
	msg += "failed to open file "+name+".";
	systemCall::throwSystemError(msg,errno);
      }
      SmartPtr<int>::operator=(SmartPtr<int>(fd));
    } // end of rfstream::open

    void
    rfstream::close(void)
    {
      using namespace std;
      using namespace tfel::utilities;
      if(*(this->p)==-1){
	return;
      }
      if(this->count()==1){
	if(::close(*(this->p))==-1){
	  string msg("rfstream::close : ");
	  msg += "failed to close file.";
	  systemCall::throwSystemError(msg,errno);
	}
      }
      SmartPtr<int>::operator=(SmartPtr<int>(-1));
    } // end of rfstream::close

    int
    rfstream::getFileDescriptor(void) const
    {
      return *(this->p);
    } // end of rfstream::getFileDescriptor

    rfstream::~rfstream()
    {
      this->close();
    } // end of rfstream::~rfstream
    
  } // end of namespace System  
  
} // end of namespace tfel  
