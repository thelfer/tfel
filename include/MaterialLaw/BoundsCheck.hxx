/*!
 * \file   BoundsCheck.hxx
 * \brief  This file declares the  class
 * \author Helfer Thomas
 * \date   06 Mar 2007
 */

#ifndef _LIB_TFEL_BOUNDSCHECK_H_
#define _LIB_TFEL_BOUNDSCHECK_H_ 

#include"Math/qt.hxx"
#include"Math/stensor.hxx"
#include"TypeTraits/BaseType.hxx"
#include"MaterialLaw/MaterialLawException.hxx"

namespace tfel{
  
  namespace materiallaw{
    
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
		      const T lBound)
      {
	using namespace std;
	if(value<lBound){
	  ostringstream msg;
	  msg << name << " is out of bound (";
	  msg << value << " < " << lBound << ")";
	  throw(OutOfBoundsException(msg.str()));
	}
      }
      template<typename T,typename Unit>
      static inline
      void
      lowerBoundCheck(const std::string& name,
		      const tfel::math::qt<Unit,T> value,
		      const T lBound)
      {
	using namespace std;
	if(value.getValue()<lBound){
	  ostringstream msg;
	  msg << name << " is out of bound (";
	  msg << value << " < " << lBound << ")";
	  throw(OutOfBoundsException(msg.str()));
	}
      }
      template<typename T>
      static inline
      void
      upperBoundCheck(const std::string& name,
		      const T value,
		      const T uBound)
      {
	using namespace std;
	if(value>uBound){
	  ostringstream msg;
	  msg << name << " is out of bound (";
	  msg << value << " > " << uBound << ")";
	  throw(OutOfBoundsException(msg.str()));
	}
      }
      template<typename T,typename Unit>
      static inline
      void
      upperBoundCheck(const std::string& name,
		      const tfel::math::qt<Unit,T> value,
		      const T uBound)
      {
	using namespace std;
	if(value.getValue()>uBound){
	  ostringstream msg;
	  msg << name << " is out of bound (";
	  msg << value << " > " << uBound << ")";
	  throw(OutOfBoundsException(msg.str()));
	}
      }
      template<typename T>
      static inline
      void
      lowerAndUpperBoundCheck(const std::string& name,
			      const T value,
			      const T lBound,
			      const T uBound)
      {
	using namespace std;
	if((value<lBound)||(value>uBound)){
	  ostringstream msg;
	  msg << name << " is out of bound ((";
	  msg << value << " < " << lBound << ")||(";
	  msg << value << " > " << uBound << "))";
	  throw(OutOfBoundsException(msg.str()));
	}
      }
      template<typename T,typename Unit>
      static inline
      void
      lowerAndUpperBoundCheck(const std::string& name,
			      const tfel::math::qt<Unit,T> value,
			      const T lBound,
			      const T uBound)
      {
	using namespace std;
	if((value.getValue()<lBound)||(value.getValue()>uBound)){
	  ostringstream msg;
	  msg << name << " is out of bound ((";
	  msg << value << " < " << lBound << ")||(";
	  msg << value << " > " << uBound << "))";
	  throw(OutOfBoundsException(msg.str()));
	}
      }
      template<typename T>
      static void
      lowerBoundCheck(const std::string& name,
		      const tfel::math::stensor<1u,T>& s,
		      const T lBound)
      {
	BoundsCheck::lowerBoundCheck(name+"(0)",s(0),lBound);
	BoundsCheck::lowerBoundCheck(name+"(1)",s(1),lBound);
	BoundsCheck::lowerBoundCheck(name+"(2)",s(2),lBound);
      }
      template<typename T>
      static void
      upperBoundCheck(const std::string& name,
		      const tfel::math::stensor<1u,T>& s,
		      const T uBound)
      {
	BoundsCheck::upperBoundCheck(name+"(0)",s(0),uBound);
	BoundsCheck::upperBoundCheck(name+"(1)",s(1),uBound);
	BoundsCheck::upperBoundCheck(name+"(2)",s(2),uBound);
      }
      template<typename T>
      static void
      lowerAndUpperBoundCheck(const std::string& name,
			      const tfel::math::stensor<1u,T>& s,
			      const T lBound,
			      const T uBound)
      {
	BoundsCheck::lowerAndUpperBoundCheck(name+"(0)",s(0),lBound,uBound);
	BoundsCheck::lowerAndUpperBoundCheck(name+"(1)",s(1),lBound,uBound);
	BoundsCheck::lowerAndUpperBoundCheck(name+"(2)",s(2),lBound,uBound);
      }
      template<typename T,typename Unit>
      static void
      lowerBoundCheck(const std::string& name,
		      const tfel::math::stensor<1u,tfel::math::qt<Unit,T> >& s,
		      const T lBound)
      {
	BoundsCheck::lowerBoundCheck(name+"(0)",s(0).getValue(),lBound);
	BoundsCheck::lowerBoundCheck(name+"(1)",s(1).getValue(),lBound);
	BoundsCheck::lowerBoundCheck(name+"(2)",s(2).getValue(),lBound);
      }
      template<typename T,typename Unit>
      static void
      upperBoundCheck(const std::string& name,
		      const tfel::math::stensor<1u,tfel::math::qt<Unit,T> >& s,
		      const T uBound)
      {
	BoundsCheck::upperBoundCheck(name+"(0)",s(0).getValue(),uBound);
	BoundsCheck::upperBoundCheck(name+"(1)",s(1).getValue(),uBound);
	BoundsCheck::upperBoundCheck(name+"(2)",s(2).getValue(),uBound);
      }
      template<typename T,typename Unit>
      static void
      lowerAndUpperBoundCheck(const std::string& name,
			      const tfel::math::stensor<1u,tfel::math::qt<Unit,T> >& s,
			      const T lBound,
			      const T uBound)
      {
	BoundsCheck::lowerAndUpperBoundCheck(name+"(0)",s(0).getValue(),lBound,uBound);
	BoundsCheck::lowerAndUpperBoundCheck(name+"(1)",s(1).getValue(),lBound,uBound);
	BoundsCheck::lowerAndUpperBoundCheck(name+"(2)",s(2).getValue(),lBound,uBound);
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
		      const T lBound)
      {
	using namespace std;
	if(value<lBound){
	  ostringstream msg;
	  msg << name << " is out of bound (";
	  msg << value << " < " << lBound << ")";
	  throw(OutOfBoundsException(msg.str()));
	}
      }
      template<typename T,typename Unit>
      static inline
      void
      lowerBoundCheck(const std::string& name,
		      const tfel::math::qt<Unit,T> value,
		      const T lBound)
      {
	using namespace std;
	if(value.getValue()<lBound){
	  ostringstream msg;
	  msg << name << " is out of bound (";
	  msg << value << " < " << lBound << ")";
	  throw(OutOfBoundsException(msg.str()));
	}
      }
      template<typename T>
      static inline
      void
      upperBoundCheck(const std::string& name,
		      const T value,
		      const T uBound)
      {
	using namespace std;
	if(value>uBound){
	  ostringstream msg;
	  msg << name << " is out of bound (";
	  msg << value << " > " << uBound << ")";
	  throw(OutOfBoundsException(msg.str()));
	}
      }
      template<typename T,typename Unit>
      static inline
      void
      upperBoundCheck(const std::string& name,
		      const tfel::math::qt<Unit,T> value,
		      const T uBound)
      {
	using namespace std;
	if(value.getValue()>uBound){
	  ostringstream msg;
	  msg << name << " is out of bound (";
	  msg << value << " > " << uBound << ")";
	  throw(OutOfBoundsException(msg.str()));
	}
      }
      template<typename T>
      static inline
      void
      lowerAndUpperBoundCheck(const std::string& name,
			      const T value,
			      const T lBound,
			      const T uBound)
      {
	using namespace std;
	if((value<lBound)||(value>uBound)){
	  ostringstream msg;
	  msg << name << " is out of bound ((";
	  msg << value << " < " << lBound << ")||(";
	  msg << value << " > " << uBound << "))";
	  throw(OutOfBoundsException(msg.str()));
	}
      }
      template<typename T,typename Unit>
      static inline
      void
      lowerAndUpperBoundCheck(const std::string& name,
			      const tfel::math::qt<Unit,T> value,
			      const T lBound,
			      const T uBound)
      {
	using namespace std;
	if((value.getValue()<lBound)||(value.getValue()>uBound)){
	  ostringstream msg;
	  msg << name << " is out of bound ((";
	  msg << value << " < " << lBound << ")||(";
	  msg << value << " > " << uBound << "))";
	  throw(OutOfBoundsException(msg.str()));
	}
      }
      template<typename T>
      static void
      lowerBoundCheck(const std::string& name,
		      const tfel::math::stensor<2u,T>& s,
		      const T lBound)
      {
	BoundsCheck::lowerBoundCheck(name+"(0)",s(0),lBound);
	BoundsCheck::lowerBoundCheck(name+"(1)",s(1),lBound);
	BoundsCheck::lowerBoundCheck(name+"(2)",s(2),lBound);
	BoundsCheck::lowerBoundCheck(name+"(3)",s(3),lBound);
      }
      template<typename T>
      static void
      upperBoundCheck(const std::string& name,
		      const tfel::math::stensor<2u,T>& s,
		      const T uBound)
      {
	BoundsCheck::upperBoundCheck(name+"(0)",s(0),uBound);
	BoundsCheck::upperBoundCheck(name+"(1)",s(1),uBound);
	BoundsCheck::upperBoundCheck(name+"(2)",s(2),uBound);
	BoundsCheck::upperBoundCheck(name+"(3)",s(3),uBound);
      }
      template<typename T>
      static void
      lowerAndUpperBoundCheck(const std::string& name,
			      const tfel::math::stensor<2u,T>& s,
			      const T lBound,
			      const T uBound)
      {
	BoundsCheck::lowerAndUpperBoundCheck(name+"(0)",s(0),lBound,uBound);
	BoundsCheck::lowerAndUpperBoundCheck(name+"(1)",s(1),lBound,uBound);
	BoundsCheck::lowerAndUpperBoundCheck(name+"(2)",s(2),lBound,uBound);
	BoundsCheck::lowerAndUpperBoundCheck(name+"(3)",s(3),lBound,uBound);
      }
      template<typename T,typename Unit>
      static void
      lowerBoundCheck(const std::string& name,
		      const tfel::math::stensor<2u,tfel::math::qt<Unit,T> >& s,
		      const T lBound)
      {
	BoundsCheck::lowerBoundCheck(name+"(0)",s(0).getValue(),lBound);
	BoundsCheck::lowerBoundCheck(name+"(1)",s(1).getValue(),lBound);
	BoundsCheck::lowerBoundCheck(name+"(2)",s(2).getValue(),lBound);
	BoundsCheck::lowerBoundCheck(name+"(3)",s(3).getValue(),lBound);
      }
      template<typename T,typename Unit>
      static void
      upperBoundCheck(const std::string& name,
		      const tfel::math::stensor<2u,tfel::math::qt<Unit,T> >& s,
		      const T uBound)
      {
	BoundsCheck::upperBoundCheck(name+"(0)",s(0).getValue(),uBound);
	BoundsCheck::upperBoundCheck(name+"(1)",s(1).getValue(),uBound);
	BoundsCheck::upperBoundCheck(name+"(2)",s(2).getValue(),uBound);
	BoundsCheck::upperBoundCheck(name+"(3)",s(3).getValue(),uBound);
      }
      template<typename T,typename Unit>
      static void
      lowerAndUpperBoundCheck(const std::string& name,
			      const tfel::math::stensor<2u,tfel::math::qt<Unit,T> >& s,
			      const T lBound,
			      const T uBound)
      {
	BoundsCheck::lowerAndUpperBoundCheck(name+"(0)",s(0).getValue(),lBound,uBound);
	BoundsCheck::lowerAndUpperBoundCheck(name+"(1)",s(1).getValue(),lBound,uBound);
	BoundsCheck::lowerAndUpperBoundCheck(name+"(2)",s(2).getValue(),lBound,uBound);
	BoundsCheck::lowerAndUpperBoundCheck(name+"(3)",s(3).getValue(),lBound,uBound);
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
		      const T lBound)
      {
	using namespace std;
	if(value<lBound){
	  ostringstream msg;
	  msg << name << " is out of bound (";
	  msg << value << " < " << lBound << ")";
	  throw(OutOfBoundsException(msg.str()));
	}
      }
      template<typename T,typename Unit>
      static inline
      void
      lowerBoundCheck(const std::string& name,
		      const tfel::math::qt<Unit,T> value,
		      const T lBound)
      {
	using namespace std;
	if(value.getValue()<lBound){
	  ostringstream msg;
	  msg << name << " is out of bound (";
	  msg << value << " < " << lBound << ")";
	  throw(OutOfBoundsException(msg.str()));
	}
      }
      template<typename T>
      static inline
      void
      upperBoundCheck(const std::string& name,
		      const T value,
		      const T uBound)
      {
	using namespace std;
	if(value>uBound){
	  ostringstream msg;
	  msg << name << " is out of bound (";
	  msg << value << " > " << uBound << ")";
	  throw(OutOfBoundsException(msg.str()));
	}
      }
      template<typename T,typename Unit>
      static inline
      void
      upperBoundCheck(const std::string& name,
		      const tfel::math::qt<Unit,T> value,
		      const T uBound)
      {
	using namespace std;
	if(value.getValue()>uBound){
	  ostringstream msg;
	  msg << name << " is out of bound (";
	  msg << value << " > " << uBound << ")";
	  throw(OutOfBoundsException(msg.str()));
	}
      }
      template<typename T>
      static inline
      void
      lowerAndUpperBoundCheck(const std::string& name,
			      const T value,
			      const T lBound,
			      const T uBound)
      {
	using namespace std;
	if((value<lBound)||(value>uBound)){
	  ostringstream msg;
	  msg << name << " is out of bound ((";
	  msg << value << " < " << lBound << ")||(";
	  msg << value << " > " << uBound << "))";
	  throw(OutOfBoundsException(msg.str()));
	}
      }
      template<typename T,typename Unit>
      static inline
      void
      lowerAndUpperBoundCheck(const std::string& name,
			      const tfel::math::qt<Unit,T> value,
			      const T lBound,
			      const T uBound)
      {
	using namespace std;
	if((value.getValue()<lBound)||(value.getValue()>uBound)){
	  ostringstream msg;
	  msg << name << " is out of bound ((";
	  msg << value << " < " << lBound << ")||(";
	  msg << value << " > " << uBound << "))";
	  throw(OutOfBoundsException(msg.str()));
	}
      }
      template<typename T>
      static void
      lowerBoundCheck(const std::string& name,
		      const tfel::math::stensor<3u,T>& s,
		      const T lBound)
      {
	BoundsCheck::lowerBoundCheck(name+"(0)",s(0),lBound);
	BoundsCheck::lowerBoundCheck(name+"(1)",s(1),lBound);
	BoundsCheck::lowerBoundCheck(name+"(2)",s(2),lBound);
	BoundsCheck::lowerBoundCheck(name+"(3)",s(3),lBound);
	BoundsCheck::lowerBoundCheck(name+"(4)",s(4),lBound);
	BoundsCheck::lowerBoundCheck(name+"(5)",s(5),lBound);
      }
      template<typename T>
      static void
      upperBoundCheck(const std::string& name,
		      const tfel::math::stensor<3u,T>& s,
		      const T uBound)
      {
	BoundsCheck::upperBoundCheck(name+"(0)",s(0),uBound);
	BoundsCheck::upperBoundCheck(name+"(1)",s(1),uBound);
	BoundsCheck::upperBoundCheck(name+"(2)",s(2),uBound);
	BoundsCheck::upperBoundCheck(name+"(3)",s(3),uBound);
	BoundsCheck::upperBoundCheck(name+"(4)",s(4),uBound);
	BoundsCheck::upperBoundCheck(name+"(5)",s(5),uBound);
      }
      template<typename T>
      static void
      lowerAndUpperBoundCheck(const std::string& name,
			      const tfel::math::stensor<3u,T>& s,
			      const T lBound,
			      const T uBound)
      {
	BoundsCheck::lowerAndUpperBoundCheck(name+"(0)",s(0),lBound,uBound);
	BoundsCheck::lowerAndUpperBoundCheck(name+"(1)",s(1),lBound,uBound);
	BoundsCheck::lowerAndUpperBoundCheck(name+"(2)",s(2),lBound,uBound);
	BoundsCheck::lowerAndUpperBoundCheck(name+"(3)",s(3),lBound,uBound);
	BoundsCheck::lowerAndUpperBoundCheck(name+"(4)",s(4),lBound,uBound);
	BoundsCheck::lowerAndUpperBoundCheck(name+"(5)",s(5),lBound,uBound);
      }
      template<typename T,typename Unit>
      static void
      lowerBoundCheck(const std::string& name,
		      const tfel::math::stensor<3u,tfel::math::qt<Unit,T> >& s,
		      const T lBound)
      {
	BoundsCheck::lowerBoundCheck(name+"(0)",s(0).getValue(),lBound);
	BoundsCheck::lowerBoundCheck(name+"(1)",s(1).getValue(),lBound);
	BoundsCheck::lowerBoundCheck(name+"(2)",s(2).getValue(),lBound);
	BoundsCheck::lowerBoundCheck(name+"(3)",s(3).getValue(),lBound);
	BoundsCheck::lowerBoundCheck(name+"(4)",s(4).getValue(),lBound);
	BoundsCheck::lowerBoundCheck(name+"(5)",s(5).getValue(),lBound);
      }
      template<typename T,typename Unit>
      static void
      upperBoundCheck(const std::string& name,
		      const tfel::math::stensor<3u,tfel::math::qt<Unit,T> >& s,
		      const T uBound)
      {
	BoundsCheck::upperBoundCheck(name+"(0)",s(0).getValue(),uBound);
	BoundsCheck::upperBoundCheck(name+"(1)",s(1).getValue(),uBound);
	BoundsCheck::upperBoundCheck(name+"(2)",s(2).getValue(),uBound);
	BoundsCheck::upperBoundCheck(name+"(3)",s(3).getValue(),uBound);
	BoundsCheck::upperBoundCheck(name+"(4)",s(4).getValue(),uBound);
	BoundsCheck::upperBoundCheck(name+"(5)",s(5).getValue(),uBound);
      }
      template<typename T,typename Unit>
      static void
      lowerAndUpperBoundCheck(const std::string& name,
			      const tfel::math::stensor<3u,tfel::math::qt<Unit,T> >& s,
			      const T lBound,
			      const T uBound)
      {
	BoundsCheck::lowerAndUpperBoundCheck(name+"(0)",s(0).getValue(),lBound,uBound);
	BoundsCheck::lowerAndUpperBoundCheck(name+"(1)",s(1).getValue(),lBound,uBound);
	BoundsCheck::lowerAndUpperBoundCheck(name+"(2)",s(2).getValue(),lBound,uBound);
	BoundsCheck::lowerAndUpperBoundCheck(name+"(3)",s(3).getValue(),lBound,uBound);
	BoundsCheck::lowerAndUpperBoundCheck(name+"(4)",s(4).getValue(),lBound,uBound);
	BoundsCheck::lowerAndUpperBoundCheck(name+"(5)",s(5).getValue(),lBound,uBound);
      }
    }; // end of struct BoundsCheck<3u>;

  } // end of namespace materiallaw
    
} // end of namespace tfel

#endif /* _LIB_TFEL_BOUNDSCHECK_H */

