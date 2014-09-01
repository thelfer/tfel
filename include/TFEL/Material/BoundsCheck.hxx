/*!
 * \file   include/TFEL/Material/BoundsCheck.hxx
 * \brief  This file declares the  class
 * \author Helfer Thomas
 * \date   06 Mar 2007
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_TFEL_BOUNDSCHECK_H_
#define _LIB_TFEL_BOUNDSCHECK_H_ 

#include<iostream>

#include"TFEL/Math/qt.hxx"
#include"TFEL/Math/stensor.hxx"
#include"TFEL/TypeTraits/BaseType.hxx"
#include"TFEL/Material/MaterialException.hxx"
#include"TFEL/Material/OutOfBoundsPolicy.hxx"

namespace tfel{
  
  namespace material{
    
    template<unsigned short N>
    struct BoundsCheck;

    template<>
    struct BoundsCheck<1u>
    {
      template<typename T>
      static inline
      void
      lowerBoundCheck(const std::string& name,
		      const T value,
		      const T lBound,
		      const OutOfBoundsPolicy p = Strict)
      {
	using namespace std;
	if(value<lBound){
	  if(p==None){
	    return;
	  }
	  ostringstream msg;
	  msg << name << " is out of bound (";
	  msg << value << " < " << lBound << ")";
	  switch(p){
	  case Strict:
	    throw(OutOfBoundsException(msg.str()));
	    break;
	  case Warning:
	    cerr << msg.str() << endl;
	    break;
	  case None:
	  default:
	    break;
	  }
	}
      }
      template<typename T,typename Unit>
      static inline
      void
      lowerBoundCheck(const std::string& name,
		      const tfel::math::qt<Unit,T> value,
		      const T lBound,
		      const OutOfBoundsPolicy p = Strict)
      {
	using namespace std;
	if(value.getValue()<lBound){
	  if(p==None){
	    return;
	  }
	  ostringstream msg;
	  msg << name << " is out of bound (";
	  msg << value << " < " << lBound << ")";
	  switch(p){
	  case Strict:
	    throw(OutOfBoundsException(msg.str()));
	    break;
	  case Warning:
	    cerr << msg.str() << endl;
	    break;
	  case None:
	  default:
	    break;
	  }
	}
      }
      template<typename T>
      static inline
      void
      upperBoundCheck(const std::string& name,
		      const T value,
		      const T uBound,
		      const OutOfBoundsPolicy p = Strict)
      {
	using namespace std;
	if(value>uBound){
	  if(p==None){
	    return;
	  }
	  ostringstream msg;
	  msg << name << " is out of bound (";
	  msg << value << " > " << uBound << ")";
	  switch(p){
	  case Strict:
	    throw(OutOfBoundsException(msg.str()));
	    break;
	  case Warning:
	    cerr << msg.str() << endl;
	    break;
	  case None:
	  default:
	    break;
	  }
	}
      }
      template<typename T,typename Unit>
      static inline
      void
      upperBoundCheck(const std::string& name,
		      const tfel::math::qt<Unit,T> value,
		      const T uBound,
		      const OutOfBoundsPolicy p = Strict)
      {
	using namespace std;
	if(value.getValue()>uBound){
	  if(p==None){
	    return;
	  }
	  ostringstream msg;
	  msg << name << " is out of bound (";
	  msg << value << " > " << uBound << ")";
	  switch(p){
	  case Strict:
	    throw(OutOfBoundsException(msg.str()));
	    break;
	  case Warning:
	    cerr << msg.str() << endl;
	    break;
	  case None:
	  default:
	    break;
	  }
	}
      }
      template<typename T>
      static inline
      void
      lowerAndUpperBoundsChecks(const std::string& name,
				const T value,
				const T lBound,
				const T uBound,
				const OutOfBoundsPolicy p = Strict)
      {
	using namespace std;
	if((value<lBound)||(value>uBound)){
	  if(p==None){
	    return;
	  }
	  ostringstream msg;
	  msg << name << " is out of bound ((";
	  msg << value << " < " << lBound << ")||(";
	  msg << value << " > " << uBound << "))";
	  switch(p){
	  case Strict:
	    throw(OutOfBoundsException(msg.str()));
	    break;
	  case Warning:
	    cerr << msg.str() << endl;
	    break;
	  case None:
	  default:
	    break;
	  }
	}
      }
      template<typename T,typename Unit>
      static inline
      void
      lowerAndUpperBoundsChecks(const std::string& name,
				const tfel::math::qt<Unit,T> value,
				const T lBound,
				const T uBound,
				const OutOfBoundsPolicy p = Strict)
      {
	using namespace std;
	if((value.getValue()<lBound)||(value.getValue()>uBound)){
	  if(p==None){
	    return;
	  }
	  ostringstream msg;
	  msg << name << " is out of bound ((";
	  msg << value << " < " << lBound << ")||(";
	  msg << value << " > " << uBound << "))";
	  switch(p){
	  case Strict:
	    throw(OutOfBoundsException(msg.str()));
	    break;
	  case Warning:
	    cerr << msg.str() << endl;
	    break;
	  case None:
	  default:
	    break;
	  }
	}
      }
      template<typename T>
      static void
      lowerBoundCheck(const std::string& name,
		      const tfel::math::stensor<1u,T>& s,
		      const T lBound,
		      const OutOfBoundsPolicy p = Strict)
      {
	BoundsCheck::lowerBoundCheck(name+"(0)",s(0),lBound,p);
	BoundsCheck::lowerBoundCheck(name+"(1)",s(1),lBound,p);
	BoundsCheck::lowerBoundCheck(name+"(2)",s(2),lBound,p);
      }
      template<typename T>
      static void
      upperBoundCheck(const std::string& name,
		      const tfel::math::stensor<1u,T>& s,
		      const T uBound,
		      const OutOfBoundsPolicy p = Strict)
      {
	BoundsCheck::upperBoundCheck(name+"(0)",s(0),uBound,p);
	BoundsCheck::upperBoundCheck(name+"(1)",s(1),uBound,p);
	BoundsCheck::upperBoundCheck(name+"(2)",s(2),uBound,p);
      }
      template<typename T>
      static void
      lowerAndUpperBoundsChecks(const std::string& name,
				const tfel::math::stensor<1u,T>& s,
				const T lBound,
				const T uBound,
				const OutOfBoundsPolicy p = Strict)
      {
	BoundsCheck::lowerAndUpperBoundsChecks(name+"(0)",s(0),lBound,uBound,p);
	BoundsCheck::lowerAndUpperBoundsChecks(name+"(1)",s(1),lBound,uBound,p);
	BoundsCheck::lowerAndUpperBoundsChecks(name+"(2)",s(2),lBound,uBound,p);
      }
      template<typename T,typename Unit>
      static void
      lowerBoundCheck(const std::string& name,
		      const tfel::math::stensor<1u,tfel::math::qt<Unit,T> >& s,
		      const T lBound,
		      const OutOfBoundsPolicy p = Strict)
      {
	BoundsCheck::lowerBoundCheck(name+"(0)",s(0).getValue(),lBound,p);
	BoundsCheck::lowerBoundCheck(name+"(1)",s(1).getValue(),lBound,p);
	BoundsCheck::lowerBoundCheck(name+"(2)",s(2).getValue(),lBound,p);
      }
      template<typename T,typename Unit>
      static void
      upperBoundCheck(const std::string& name,
		      const tfel::math::stensor<1u,tfel::math::qt<Unit,T> >& s,
		      const T uBound,
		      const OutOfBoundsPolicy p = Strict)
      {
	BoundsCheck::upperBoundCheck(name+"(0)",s(0).getValue(),uBound,p);
	BoundsCheck::upperBoundCheck(name+"(1)",s(1).getValue(),uBound,p);
	BoundsCheck::upperBoundCheck(name+"(2)",s(2).getValue(),uBound,p);
      }
      template<typename T,typename Unit>
      static void
      lowerAndUpperBoundsChecks(const std::string& name,
				const tfel::math::stensor<1u,tfel::math::qt<Unit,T> >& s,
				const T lBound,
				const T uBound,
				const OutOfBoundsPolicy p = Strict)
      {
	BoundsCheck::lowerAndUpperBoundsChecks(name+"(0)",s(0).getValue(),lBound,uBound,p);
	BoundsCheck::lowerAndUpperBoundsChecks(name+"(1)",s(1).getValue(),lBound,uBound,p);
	BoundsCheck::lowerAndUpperBoundsChecks(name+"(2)",s(2).getValue(),lBound,uBound,p);
      }
    }; // end of struct BoundsCheck<1u>;

    template<>
    struct BoundsCheck<2u>
    {
      template<typename T>
      static inline
      void
      lowerBoundCheck(const std::string& name,
		      const T value,
		      const T lBound,
		      const OutOfBoundsPolicy p = Strict)
      {
	using namespace std;
	if(value<lBound){
	  if(p==None){
	    return;
	  }
	  ostringstream msg;
	  msg << name << " is out of bound (";
	  msg << value << " < " << lBound << ")";
	  switch(p){
	  case Strict:
	    throw(OutOfBoundsException(msg.str()));
	    break;
	  case Warning:
	    cerr << msg.str() << endl;
	    break;
	  case None:
	  default:
	    break;
	  }
	}
      }
      template<typename T,typename Unit>
      static inline
      void
      lowerBoundCheck(const std::string& name,
		      const tfel::math::qt<Unit,T> value,
		      const T lBound,
		      const OutOfBoundsPolicy p = Strict)
      {
	using namespace std;
	if(value.getValue()<lBound){
	  if(p==None){
	    return;
	  }
	  ostringstream msg;
	  msg << name << " is out of bound (";
	  msg << value << " < " << lBound << ")";
	  switch(p){
	  case Strict:
	    throw(OutOfBoundsException(msg.str()));
	    break;
	  case Warning:
	    cerr << msg.str() << endl;
	    break;
	  case None:
	  default:
	    break;
	  }
	}
      }
      template<typename T>
      static inline
      void
      upperBoundCheck(const std::string& name,
		      const T value,
		      const T uBound,
		      const OutOfBoundsPolicy p = Strict)
      {
	using namespace std;
	if(value>uBound){
	  if(p==None){
	    return;
	  }
	  ostringstream msg;
	  msg << name << " is out of bound (";
	  msg << value << " > " << uBound << ")";
	  switch(p){
	  case Strict:
	    throw(OutOfBoundsException(msg.str()));
	    break;
	  case Warning:
	    cerr << msg.str() << endl;
	    break;
	  case None:
	  default:
	    break;
	  }
	}
      }
      template<typename T,typename Unit>
      static inline
      void
      upperBoundCheck(const std::string& name,
		      const tfel::math::qt<Unit,T> value,
		      const T uBound,
		      const OutOfBoundsPolicy p = Strict)
      {
	using namespace std;
	if(value.getValue()>uBound){
	  if(p==None){
	    return;
	  }
	  ostringstream msg;
	  msg << name << " is out of bound (";
	  msg << value << " > " << uBound << ")";
	  switch(p){
	  case Strict:
	    throw(OutOfBoundsException(msg.str()));
	    break;
	  case Warning:
	    cerr << msg.str() << endl;
	    break;
	  case None:
	  default:
	    break;
	  }
	}
      }
      template<typename T>
      static inline
      void
      lowerAndUpperBoundsChecks(const std::string& name,
				const T value,
				const T lBound,
				const T uBound,
				const OutOfBoundsPolicy p = Strict)
      {
	using namespace std;
	if((value<lBound)||(value>uBound)){
	  if(p==None){
	    return;
	  }
	  ostringstream msg;
	  msg << name << " is out of bound ((";
	  msg << value << " < " << lBound << ")||(";
	  msg << value << " > " << uBound << "))";
	  switch(p){
	  case Strict:
	    throw(OutOfBoundsException(msg.str()));
	    break;
	  case Warning:
	    cerr << msg.str() << endl;
	    break;
	  case None:
	  default:
	    break;
	  }
	}
      }
      template<typename T,typename Unit>
      static inline
      void
      lowerAndUpperBoundsChecks(const std::string& name,
				const tfel::math::qt<Unit,T> value,
				const T lBound,
				const T uBound,
				const OutOfBoundsPolicy p = Strict)
      {
	using namespace std;
	BoundsCheck::lowerBoundCheck(name,value.getValue(),lBound,p);
	BoundsCheck::upperBoundCheck(name,value.getValue(),uBound,p);
      }
      template<typename T>
      static void
      lowerBoundCheck(const std::string& name,
		      const tfel::math::stensor<2u,T>& s,
		      const T lBound,
		      const OutOfBoundsPolicy p = Strict)
      {
	BoundsCheck::lowerBoundCheck(name+"(0)",s(0),lBound,p);
	BoundsCheck::lowerBoundCheck(name+"(1)",s(1),lBound,p);
	BoundsCheck::lowerBoundCheck(name+"(2)",s(2),lBound,p);
	BoundsCheck::lowerBoundCheck(name+"(3)",s(3),lBound,p);
      }
      template<typename T>
      static void
      upperBoundCheck(const std::string& name,
		      const tfel::math::stensor<2u,T>& s,
		      const T uBound,
		      const OutOfBoundsPolicy p = Strict)
      {
	BoundsCheck::upperBoundCheck(name+"(0)",s(0),uBound,p);
	BoundsCheck::upperBoundCheck(name+"(1)",s(1),uBound,p);
	BoundsCheck::upperBoundCheck(name+"(2)",s(2),uBound,p);
	BoundsCheck::upperBoundCheck(name+"(3)",s(3),uBound,p);
      }
      template<typename T>
      static void
      lowerAndUpperBoundsChecks(const std::string& name,
				const tfel::math::stensor<2u,T>& s,
				const T lBound,
				const T uBound,
				const OutOfBoundsPolicy p = Strict)
      {
	BoundsCheck::lowerAndUpperBoundsChecks(name+"(0)",s(0),lBound,uBound,p);
	BoundsCheck::lowerAndUpperBoundsChecks(name+"(1)",s(1),lBound,uBound,p);
	BoundsCheck::lowerAndUpperBoundsChecks(name+"(2)",s(2),lBound,uBound,p);
	BoundsCheck::lowerAndUpperBoundsChecks(name+"(3)",s(3),lBound,uBound,p);
      }
      template<typename T,typename Unit>
      static void
      lowerBoundCheck(const std::string& name,
		      const tfel::math::stensor<2u,tfel::math::qt<Unit,T> >& s,
		      const T lBound,
		      const OutOfBoundsPolicy p = Strict)
      {
	BoundsCheck::lowerBoundCheck(name+"(0)",s(0).getValue(),lBound,p);
	BoundsCheck::lowerBoundCheck(name+"(1)",s(1).getValue(),lBound,p);
	BoundsCheck::lowerBoundCheck(name+"(2)",s(2).getValue(),lBound,p);
	BoundsCheck::lowerBoundCheck(name+"(3)",s(3).getValue(),lBound,p);
      }
      template<typename T,typename Unit>
      static void
      upperBoundCheck(const std::string& name,
		      const tfel::math::stensor<2u,tfel::math::qt<Unit,T> >& s,
		      const T uBound,
		      const OutOfBoundsPolicy p = Strict)
      {
	BoundsCheck::upperBoundCheck(name+"(0)",s(0).getValue(),uBound,p);
	BoundsCheck::upperBoundCheck(name+"(1)",s(1).getValue(),uBound,p);
	BoundsCheck::upperBoundCheck(name+"(2)",s(2).getValue(),uBound,p);
	BoundsCheck::upperBoundCheck(name+"(3)",s(3).getValue(),uBound,p);
      }
      template<typename T,typename Unit>
      static void
      lowerAndUpperBoundsChecks(const std::string& name,
				const tfel::math::stensor<2u,tfel::math::qt<Unit,T> >& s,
				const T lBound,
				const T uBound,
				const OutOfBoundsPolicy p = Strict)
      {
	BoundsCheck::lowerAndUpperBoundsChecks(name+"(0)",s(0).getValue(),lBound,uBound,p);
	BoundsCheck::lowerAndUpperBoundsChecks(name+"(1)",s(1).getValue(),lBound,uBound,p);
	BoundsCheck::lowerAndUpperBoundsChecks(name+"(2)",s(2).getValue(),lBound,uBound,p);
	BoundsCheck::lowerAndUpperBoundsChecks(name+"(3)",s(3).getValue(),lBound,uBound,p);
      }
    }; // end of struct BoundsCheck<2u>;

    template<>
    struct BoundsCheck<3u>
    {
      template<typename T>
      static inline
      void
      lowerBoundCheck(const std::string& name,
		      const T value,
		      const T lBound,
		      const OutOfBoundsPolicy p = Strict)
      {
	using namespace std;
	if(value<lBound){
	  if(p==None){
	    return;
	  }
	  ostringstream msg;
	  msg << name << " is out of bound (";
	  msg << value << " < " << lBound << ")";
	  switch(p){
	  case Strict:
	    throw(OutOfBoundsException(msg.str()));
	    break;
	  case Warning:
	    cerr << msg.str() << endl;
	    break;
	  case None:
	  default:
	    break;
	  }
	}
      }
      template<typename T,typename Unit>
      static inline
      void
      lowerBoundCheck(const std::string& name,
		      const tfel::math::qt<Unit,T> value,
		      const T lBound,
		      const OutOfBoundsPolicy p = Strict)
      {
	using namespace std;
	if(value.getValue()<lBound){
	  if(p==None){
	    return;
	  }
	  ostringstream msg;
	  msg << name << " is out of bound (";
	  msg << value << " < " << lBound << ")";
	  switch(p){
	  case Strict:
	    throw(OutOfBoundsException(msg.str()));
	    break;
	  case Warning:
	    cerr << msg.str() << endl;
	    break;
	  case None:
	  default:
	    break;
	  }
	}
      }
      template<typename T>
      static inline
      void
      upperBoundCheck(const std::string& name,
		      const T value,
		      const T uBound,
		      const OutOfBoundsPolicy p = Strict)
      {
	using namespace std;
	if(value>uBound){
	  if(p==None){
	    return;
	  }
	  ostringstream msg;
	  msg << name << " is out of bound (";
	  msg << value << " > " << uBound << ")";
	  switch(p){
	  case Strict:
	    throw(OutOfBoundsException(msg.str()));
	    break;
	  case Warning:
	    cerr << msg.str() << endl;
	    break;
	  case None:
	  default:
	    break;
	  }
	}
      }
      template<typename T,typename Unit>
      static inline
      void
      upperBoundCheck(const std::string& name,
		      const tfel::math::qt<Unit,T> value,
		      const T uBound,
		      const OutOfBoundsPolicy p = Strict)
      {
	using namespace std;
	if(value.getValue()>uBound){
	  if(p==None){
	    return;
	  }
	  ostringstream msg;
	  msg << name << " is out of bound (";
	  msg << value << " > " << uBound << ")";
	  switch(p){
	  case Strict:
	    throw(OutOfBoundsException(msg.str()));
	    break;
	  case Warning:
	    cerr << msg.str() << endl;
	    break;
	  case None:
	  default:
	    break;
	  }
	}
      }
      template<typename T>
      static inline
      void
      lowerAndUpperBoundsChecks(const std::string& name,
				const T value,
				const T lBound,
				const T uBound,
				const OutOfBoundsPolicy p = Strict)
      {
	using namespace std;
	if((value<lBound)||(value>uBound)){
	  if(p==None){
	    return;
	  }
	  ostringstream msg;
	  msg << name << " is out of bound ((";
	  msg << value << " < " << lBound << ")||(";
	  msg << value << " > " << uBound << "))";
	  switch(p){
	  case Strict:
	    throw(OutOfBoundsException(msg.str()));
	    break;
	  case Warning:
	    cerr << msg.str() << endl;
	    break;
	  case None:
	  default:
	    break;
	  }
	}
      }
      template<typename T,typename Unit>
      static inline
      void
      lowerAndUpperBoundsChecks(const std::string& name,
				const tfel::math::qt<Unit,T> value,
				const T lBound,
				const T uBound,
				const OutOfBoundsPolicy p = Strict)
      {
	using namespace std;
	if((value.getValue()<lBound)||(value.getValue()>uBound)){
	  if(p==None){
	    return;
	  }
	  ostringstream msg;
	  msg << name << " is out of bound ((";
	  msg << value << " < " << lBound << ")||(";
	  msg << value << " > " << uBound << "))";
	  switch(p){
	  case Strict:
	    throw(OutOfBoundsException(msg.str()));
	    break;
	  case Warning:
	    cerr << msg.str() << endl;
	    break;
	  case None:
	  default:
	    break;
	  }
	}
      }
      template<typename T>
      static void
      lowerBoundCheck(const std::string& name,
		      const tfel::math::stensor<3u,T>& s,
		      const T lBound,
		      const OutOfBoundsPolicy p = Strict)
      {
	BoundsCheck::lowerBoundCheck(name+"(0)",s(0),lBound,p);
	BoundsCheck::lowerBoundCheck(name+"(1)",s(1),lBound,p);
	BoundsCheck::lowerBoundCheck(name+"(2)",s(2),lBound,p);
	BoundsCheck::lowerBoundCheck(name+"(3)",s(3),lBound,p);
	BoundsCheck::lowerBoundCheck(name+"(4)",s(4),lBound,p);
	BoundsCheck::lowerBoundCheck(name+"(5)",s(5),lBound,p);
      }
      template<typename T>
      static void
      upperBoundCheck(const std::string& name,
		      const tfel::math::stensor<3u,T>& s,
		      const T uBound,
		      const OutOfBoundsPolicy p = Strict)
      {
	BoundsCheck::upperBoundCheck(name+"(0)",s(0),uBound,p);
	BoundsCheck::upperBoundCheck(name+"(1)",s(1),uBound,p);
	BoundsCheck::upperBoundCheck(name+"(2)",s(2),uBound,p);
	BoundsCheck::upperBoundCheck(name+"(3)",s(3),uBound,p);
	BoundsCheck::upperBoundCheck(name+"(4)",s(4),uBound,p);
	BoundsCheck::upperBoundCheck(name+"(5)",s(5),uBound,p);
      }
      template<typename T>
      static void
      lowerAndUpperBoundsChecks(const std::string& name,
				const tfel::math::stensor<3u,T>& s,
				const T lBound,
				const T uBound,
				const OutOfBoundsPolicy p = Strict)
      {
	BoundsCheck::lowerAndUpperBoundsChecks(name+"(0)",s(0),lBound,uBound,p);
	BoundsCheck::lowerAndUpperBoundsChecks(name+"(1)",s(1),lBound,uBound,p);
	BoundsCheck::lowerAndUpperBoundsChecks(name+"(2)",s(2),lBound,uBound,p);
	BoundsCheck::lowerAndUpperBoundsChecks(name+"(3)",s(3),lBound,uBound,p);
	BoundsCheck::lowerAndUpperBoundsChecks(name+"(4)",s(4),lBound,uBound,p);
	BoundsCheck::lowerAndUpperBoundsChecks(name+"(5)",s(5),lBound,uBound,p);
      }
      template<typename T,typename Unit>
      static void
      lowerBoundCheck(const std::string& name,
		      const tfel::math::stensor<3u,tfel::math::qt<Unit,T> >& s,
		      const T lBound,
		      const OutOfBoundsPolicy p = Strict)
      {
	BoundsCheck::lowerBoundCheck(name+"(0)",s(0).getValue(),lBound,p);
	BoundsCheck::lowerBoundCheck(name+"(1)",s(1).getValue(),lBound,p);
	BoundsCheck::lowerBoundCheck(name+"(2)",s(2).getValue(),lBound,p);
	BoundsCheck::lowerBoundCheck(name+"(3)",s(3).getValue(),lBound,p);
	BoundsCheck::lowerBoundCheck(name+"(4)",s(4).getValue(),lBound,p);
	BoundsCheck::lowerBoundCheck(name+"(5)",s(5).getValue(),lBound,p);
      }
      template<typename T,typename Unit>
      static void
      upperBoundCheck(const std::string& name,
		      const tfel::math::stensor<3u,tfel::math::qt<Unit,T> >& s,
		      const T uBound,
		      const OutOfBoundsPolicy p = Strict)
      {
	BoundsCheck::upperBoundCheck(name+"(0)",s(0).getValue(),uBound,p);
	BoundsCheck::upperBoundCheck(name+"(1)",s(1).getValue(),uBound,p);
	BoundsCheck::upperBoundCheck(name+"(2)",s(2).getValue(),uBound,p);
	BoundsCheck::upperBoundCheck(name+"(3)",s(3).getValue(),uBound,p);
	BoundsCheck::upperBoundCheck(name+"(4)",s(4).getValue(),uBound,p);
	BoundsCheck::upperBoundCheck(name+"(5)",s(5).getValue(),uBound,p);
      }
      template<typename T,typename Unit>
      static void
      lowerAndUpperBoundsChecks(const std::string& name,
				const tfel::math::stensor<3u,tfel::math::qt<Unit,T> >& s,
				const T lBound,
				const T uBound,
				const OutOfBoundsPolicy p = Strict)
      {
	BoundsCheck::lowerAndUpperBoundsChecks(name+"(0)",s(0).getValue(),lBound,uBound,p);
	BoundsCheck::lowerAndUpperBoundsChecks(name+"(1)",s(1).getValue(),lBound,uBound,p);
	BoundsCheck::lowerAndUpperBoundsChecks(name+"(2)",s(2).getValue(),lBound,uBound,p);
	BoundsCheck::lowerAndUpperBoundsChecks(name+"(3)",s(3).getValue(),lBound,uBound,p);
	BoundsCheck::lowerAndUpperBoundsChecks(name+"(4)",s(4).getValue(),lBound,uBound,p);
	BoundsCheck::lowerAndUpperBoundsChecks(name+"(5)",s(5).getValue(),lBound,uBound,p);
      }
    }; // end of struct BoundsCheck<3u>;

  } // end of namespace material
    
} // end of namespace tfel

#endif /* _LIB_TFEL_BOUNDSCHECK_H */

