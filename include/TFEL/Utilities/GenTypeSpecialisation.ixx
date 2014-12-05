/*!
 * \file   include/TFEL/Utilities/GenTypeSpecialisation.ixx
 * \brief  This file implements some special support for specific classes.
 * \see    TL
 * \author Helfer Thomas
 * \date   19 Apr. 2007
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_TFEL_GENTYPESPECIALISATION_I_
#define _LIB_TFEL_GENTYPESPECIALISATION_I_ 

//! an helper macro to specialize GenTypeTraits for plain old data types.
#define TFEL_UTILITIES_GENTYPETRAITS(X)                \
      /*                                               \
       * \brief  specialisation for X                  \
       * \author Helfer Thomas                         \
       * \date   20 Apr. 2007                          \
       */                                              \
      template<>                                       \
      struct GenTypeTraits< X >                        \
      {                                                \
	static constexpr bool isFundamentalType = true;    \
      }

//! an helper macro to specialize GenTypeSpecializedAccessor.
#define TFEL_UTILITIES_GENTYPESPECIALIZEDACCESSOR(X,Y)              \
      /*                                                            \
       * \brief  partial specialisation for X                       \
       * \author Helfer Thomas                                      \
       * \date   20 Apr. 2007                                       \
       */                                                           \
      template<typename Child>                                      \
      struct GenTypeSpecializedAccessor<Child, X >                  \
      {                                                             \
        /*                                                          \
	 * set an object of type X.                                 \
	 * \param const X&, the source                              \
	 */                                                         \
	TFEL_INLINE                                                 \
	void                                                        \
	set##Y(const X& src)                                        \
	{                                                           \
          void (Child:: * s)(const X&) = &Child::template set<X>;   \
	  static_cast<Child *>(this)->*s(src);                      \
	}                                                           \
        /*                                                          \
	 * get an object of type X.                                 \
	 * \return X&                                               \
	 */                                                         \
	TFEL_INLINE                                                 \
	X&                                                          \
	get##Y(void)                                                \
	{                                                           \
	  return static_cast<Child *>(this)->template get<X>();     \
	}                                                           \
        /*                                                          \
	 * get an object of type X (const version).                 \
	 * \return const X&                                         \
 	 */                                                         \
	TFEL_INLINE                                                 \
	const X&                                                    \
	get##Y(void) const                                          \
	{                                                           \
	  return static_cast<Child *>(this)->template get<X>();     \
	}                                                           \
      }

namespace tfel{
  
  namespace utilities{

    namespace internals{
      
      TFEL_UTILITIES_GENTYPETRAITS(bool);
      TFEL_UTILITIES_GENTYPETRAITS(unsigned short);
      TFEL_UTILITIES_GENTYPETRAITS(unsigned int);
      TFEL_UTILITIES_GENTYPETRAITS(long unsigned int);
      TFEL_UTILITIES_GENTYPETRAITS(short);
      TFEL_UTILITIES_GENTYPETRAITS(int);
      TFEL_UTILITIES_GENTYPETRAITS(long int);
      TFEL_UTILITIES_GENTYPETRAITS(float);
      TFEL_UTILITIES_GENTYPETRAITS(double);
      TFEL_UTILITIES_GENTYPETRAITS(long double);

      TFEL_UTILITIES_GENTYPESPECIALIZEDACCESSOR(bool,Bool);
      TFEL_UTILITIES_GENTYPESPECIALIZEDACCESSOR(unsigned short,UShort);
      TFEL_UTILITIES_GENTYPESPECIALIZEDACCESSOR(unsigned int,UInt);
      TFEL_UTILITIES_GENTYPESPECIALIZEDACCESSOR(long unsigned int,LongUInt);
      TFEL_UTILITIES_GENTYPESPECIALIZEDACCESSOR(short,Short);
      TFEL_UTILITIES_GENTYPESPECIALIZEDACCESSOR(int,Int);
      TFEL_UTILITIES_GENTYPESPECIALIZEDACCESSOR(long int,LongInt);
      TFEL_UTILITIES_GENTYPESPECIALIZEDACCESSOR(float,Float);
      TFEL_UTILITIES_GENTYPESPECIALIZEDACCESSOR(double,Double);
      TFEL_UTILITIES_GENTYPESPECIALIZEDACCESSOR(long double,LongDouble);

      /*
       * \brief  partial specialisation for std::string.
       * \author Helfer Thomas
       * \date   20 Apr. 2007
       */
      template<typename Child>
      struct GenTypeSpecializedAccessor<Child,std::string>
      {
	/*
	 * Assignement for a C-style string.
	 * \param const char* const, the C-style string.
	 * \return Child&, a reference to this.
	 * \warning, this does not work yet.
	 */
	Child&
	operator=(const char* const  c)
	{
          void (Child:: * s)(const std::string&) = &Child::template set<std::string>;
	  static_cast<Child *>(this)->*s(c);
	  return static_cast<Child &>(*this);
	}
        /*
	 * set an object of type std::string.
	 * \param const std::string&, the source
	 */
	TFEL_INLINE
	void
	setString(const std::string& src)
	{
	  void (Child:: * s)(const std::string&) = &Child::template set<std::string>;
	  static_cast<Child *>(this)->*s(src);
	}
        /*
	 * get an object of type std::string.
	 * \return std::string&
	 */
	TFEL_INLINE
	std::string&
	getString(void)
	{
	  return static_cast<Child *>(this)->template get<std::string>();
	}
        /*
	 * get an object of type std::string (const version).
	 * \return const std::string&
 	 */
	TFEL_INLINE
	const std::string&
	getString(void) const
	{
	  return static_cast<Child *>(this)->template get<std::string>();
	}
      }; // end of class GenTypeSpecializedAccessor

    } // end of namespace internals

  } // end of namespace utilities

} // end of namespace tfel

#endif /* _LIB_TFEL_GENTYPESPECIALISATION_I */
