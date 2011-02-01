/*! 
 * \file  STLContainerBinaryWrite.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 27 avr 2009
 */

#ifndef _LIB_TFEL_SYSTEM_STLCONTAINERBINARYWRITE_H_
#define _LIB_TFEL_SYSTEM_STLCONTAINERBINARYWRITE_H_ 

#include<map>
#include<set>
#include<list>
#include<vector>
#include<string>

#include"TFEL/System/BinaryWrite.hxx"

namespace tfel
{

  namespace system
  {

    template<typename T1,
	     typename T2>
    struct BinaryWriter<std::pair<T1,T2> >
    {
      static void exe(const int f,
		      const std::pair<T1,T2>& v)
      {
	using namespace std;
	binary_write(f,v.first);
	binary_write(f,v.second);
      }
    }; // end of BinaryWriter<std::vector..>

    template<typename T,
	     typename Allocator>
    struct BinaryWriter<std::vector<T,Allocator> >
    {
      static void exe(const int f,
		      const std::vector<T,Allocator>& v)
      {
	using namespace std;
	typename vector<T,Allocator>::const_iterator p;
	typename vector<T,Allocator>::size_type s = v.size();
	binary_write(f,s);
	for(p=v.begin();p!=v.end();++p){
	  binary_write(f,*p);
	}
      }
    }; // end of BinaryWriter<std::vector..>

    template<typename charT,
	     typename traits,
	     typename Alloc>
    struct BinaryWriter<std::basic_string<charT,traits,Alloc> >
    {
      static void exe(const int f,const std::basic_string<charT,traits,Alloc>& v)
      {
	using namespace std;
	typedef typename basic_string<charT,traits,Alloc>::size_type  size_type;
	typedef typename basic_string<charT,traits,Alloc>::value_type value_type;
	binary_write(f,v.size());
	systemCall::write(f,&v[0],v.size()*sizeof(value_type));
      }
    }; // end of struct BinaryWriter<std::basic_string<charT,traits,Alloc> >

    template<typename T,
	     typename Allocator>
    struct BinaryWriter<std::list<T,Allocator> >
    {
      static void exe(const int f,
		      const std::list<T,Allocator>& v)
      {
	using namespace std;
	typename list<T,Allocator>::const_iterator p;
	typename list<T,Allocator>::size_type s = v.size();
	binary_write(f,s);
	for(p=v.begin();p!=v.end();++p){
	  binary_write(f,*p);
	}
      }
    }; // end of BinaryWriter

    template<typename T,
	     typename Allocator>
    struct BinaryWriter<std::set<T,Allocator> >
    {
      static void exe(const int f,
		      const std::set<T,Allocator>& v)
      {
	using namespace std;
	typename set<T,Allocator>::const_iterator p;
	typename set<T,Allocator>::size_type s = v.size();
	binary_write(f,s);
	for(p=v.begin();p!=v.end();++p){
	  binary_write(f,*p);
	}
      }
    }; // end of BinaryWriter

    template<typename Key,
	     typename Data,
	     typename Compare,
	     typename Alloc>
    struct BinaryWriter<std::map<Key,Data,Compare,Alloc> >
    {
      static void exe(const int f,
		      const std::map<Key,Data,Compare,Alloc>& v)
      {
	using namespace std;
	typename map<Key,Data,Compare,Alloc>::const_iterator p;
	typename map<Key,Data,Compare,Alloc>::size_type s = v.size();
	binary_write(f,s);
	for(p=v.begin();p!=v.end();++p){
	  binary_write(f,p->first);
	  binary_write(f,p->second);
	}
      }
    }; // end of BinaryWriter<std::map...>

  } // end of namespace system

} // end of namespace tfel

#endif /* _LIB_TFEL_SYSTEM_STLCONTAINERBINARYWRITE_H */

