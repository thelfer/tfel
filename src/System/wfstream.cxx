/*!
 * \file   src/System/wfstream.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   14 nov 2007
 */

#include"TFEL/System/System.hxx"
#include"TFEL/System/wfstream.hxx"

namespace tfel
{

  namespace system
  {
    
    wfstream::wfstream()
      : basic_wstream<wfstream,stream_traits<wfstream> >(),
	tfel::utilities::shared_ptr<int>(new int(-1))
    {} // end of wfstream::wfstream

    wfstream::wfstream(const wfstream& src)
      : basic_wstream<wfstream,stream_traits<wfstream> >(src),
	tfel::utilities::shared_ptr<int>(src)
    {} // end of wfstream::wfstream

    wfstream &
    wfstream::operator=(const wfstream& src)
    {
      using namespace tfel::utilities;
      if(this==&src){
	return *this;
      }
      this->close();
      basic_wstream<wfstream,stream_traits<wfstream> >::operator=(src);
      shared_ptr<int>::operator=(src);
      return *this;
    } // end of wfstream::operator

    wfstream::wfstream(const std::string& name, const int flags,const mode_t mode)
      : basic_wstream<wfstream,stream_traits<wfstream> >(),
	tfel::utilities::shared_ptr<int>(new int(-1))
    {
      this->open(name,flags,mode);
    } // end of wfstream::wfstream

    void
    wfstream::open(const std::string& name,
		   const int flags,
		   const mode_t mode)
    {
      using namespace std;
      using namespace tfel::utilities;
      int fd;
      if(*(this->get())!=-1){
	// closing the previous file
	this->close();
      }
      fd=::open(name.c_str(),flags,mode);
      if(fd==-1){
	string msg("wfstream::open : ");
	msg += "failed to open file "+name+".";
	systemCall::throwSystemError(msg,errno);
      }
      shared_ptr<int>::operator=(shared_ptr<int>(new int(fd)));
    } // end of wfstream::open

    void
    wfstream::close(void)
    {
      using namespace std;
      using namespace tfel::utilities;
      if(*(this->get())==-1){
	return;
      }
      if(this->unique()){
	if(::close(*(this->get()))==-1){
	  string msg("wfstream::close : ");
	  msg += "failed to close file.";
	  systemCall::throwSystemError(msg,errno);
	}
      }
      shared_ptr<int>::operator=(shared_ptr<int>(new int(-1)));
    } // end of wfstream::close

    int
    wfstream::getFileDescriptor(void) const
    {
      return *(this->get());
    } // end of wfstream::getFileDescriptor

    wfstream::~wfstream()
    {
      this->close();
    } // end of wfstream::~wfstream
    
  } // end of namespace System  
  
} // end of namespace tfel  
