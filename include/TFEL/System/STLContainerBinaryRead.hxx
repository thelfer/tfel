/*! 
 * \file  STLContainerBinaryReade.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 27 avr 2009
 */

#ifndef _LIB_TFEL_SYSTEM_STLCONTAINERBINARYREADE_H_
#define _LIB_TFEL_SYSTEM_STLCONTAINERBINARYREADE_H_ 

#include<map>
#include<set>
#include<list>
#include<vector>
#include<string>

#include"TFEL/System/BinaryRead.hxx"

namespace tfel
{

  namespace system
  {

    template<typename T1,
	     typename T2>
    struct BinaryReader<std::pair<T1,T2> >
    {
      static void exe(const int f,std::pair<T1,T2>& res)
      {
	using namespace std;
	binary_read(f,res.first);
	binary_read(f,res.second);
      }
    };
    
    template<typename T,
	 typename Allocator>
    struct BinaryReader<std::vector<T,Allocator> >
    {
      static void exe(const int f,std::vector<T,Allocator>& res)
      {
	using namespace std;
	typename vector<T,Allocator>::iterator p;
	typename vector<T,Allocator>::size_type s = binary_read<typename vector<T,Allocator>::size_type>(f);
	res.clear();
	res.resize(s);
	for(p=res.begin();p!=res.end();++p){
	  binary_read(f,*p);
	}
      }
    };

    template<typename T,
	     typename Allocator>
    struct BinaryReader<std::list<T,Allocator> >
    {
      static void exe(const int f,std::list<T,Allocator>& res)
      {
	using namespace std;
	typename list<T,Allocator>::iterator p;
	typename list<T,Allocator>::size_type s = binary_read<typename list<T,Allocator>::size_type>(f);
	res.clear();
	res.resize(s);
	for(p=res.begin();p!=res.end();++p){
	  binary_read(f,*p);
	}
      }
    };

    template<typename T,
	     typename Allocator>
    struct BinaryReader<std::set<T,Allocator> >
    {
      static void exe(const int f,std::set<T,Allocator>& res)
      {
	using namespace std;
	typename set<T,Allocator>::iterator p;
	typename set<T,Allocator>::size_type s = binary_read<typename set<T,Allocator>::size_type>(f);
	res.clear();
	res.resize(s);
	for(p=res.begin();p!=res.end();++p){
	  binary_read(f,*p);
	}
      }
    };

    template<typename charT,
	     typename traits,
	     typename Alloc>
    struct BinaryReader<std::basic_string<charT,traits,Alloc> >
    {
      static void exe(const int f,std::basic_string<charT,traits,Alloc>& res)
      {
	using namespace std;
	typedef typename basic_string<charT,traits,Alloc>::size_type  size_type;
	typedef typename basic_string<charT,traits,Alloc>::value_type value_type;
	size_type s = binary_read<size_type>(f);
	res.clear();
	res.resize(s);
	::read(f,&res[0],s*sizeof(value_type));
      }
    };

    template<typename Key,
	     typename Data,
	     typename Compare,
	     typename Alloc>
    struct BinaryReader<std::map<Key,Data,Compare,Alloc> >
    {
      static void exe(const int f,
		      std::map<Key,Data,Compare,Alloc>& res)
      {
	using namespace std;
	typedef typename map<Key,Data,Compare,Alloc>::value_type MVType;
	typename map<Key,Data,Compare,Alloc>::size_type s;
	typename map<Key,Data,Compare,Alloc>::size_type i;
	binary_read(f,s);
	res.clear();
	for(i=0;i!=s;++i){
	  Key  key;
	  Data data;
	  binary_read(f,key);
	  binary_read(f,data);
	  res.insert(MVType(key,data));
	}
      }
    }; // end of BinaryReader<std::map...>

  } // end of namespace system

} // end of namespace tfel

#endif /* _LIB_TFEL_SYSTEM_STLCONTAINERBINARYREADER_H */

