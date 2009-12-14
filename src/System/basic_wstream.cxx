/*!
 * \file   basic_wstream.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   14 nov 2007
 */

#include<cerrno>
#include<limits>
#include<unistd.h>

#include"System/basic_wstream.hxx"
#include"System/System.hxx"

namespace tfel
{

  namespace system
  {

    void
    BlockingStreamWriter::write(int fd,const void *const buf,const size_t count)
    {
      using namespace std;
      const char * start = static_cast<const char *>(buf); //< some strange
							   //  warning of gcc
      size_t  r = count;       //< remaining block to be written
      ssize_t w = 0;           //< number of written blocks
      if(count>static_cast<size_t>(numeric_limits<ssize_t>::max())){
	throw(SystemError("BlockingStreamReader::write : number of bytes to write too high"));
      }
      while(r>0){
	while((w=::write(fd,start,r))==-1){
	  if(errno==EINTR)
	    continue;
	  if(errno!=EAGAIN)
	    systemCall::throwSystemError("BlockingStreamWriter::write : write failed",errno);
	  sleep(1);
	}
	r     -= w;
	start += w;
      }

    } // end of BlockingStreamWriter::write
    
    void
    NonBlockingStreamWriter::write(int fd,const void *const buf,const size_t count)
    {
      using namespace std;
      ssize_t rwrite;
      if(count>static_cast<size_t>(numeric_limits<ssize_t>::max())){
	throw(SystemError("NonBlockingStreamReader::write : number of bytes to write too high"));
      }
      rwrite=::write(fd,buf,count);
      if(rwrite==-1){
	systemCall::throwSystemError("NonBlockingStreamWriter::write : write failed",errno);
      }
    } // end of NonBlockingStreamWriter::write

  } // end of namespace System  

} // end of namespace tfel  
