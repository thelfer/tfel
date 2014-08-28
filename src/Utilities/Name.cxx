/*!
 * \file   src/Utilities/Name.cxx
 * \brief    
 * \author Helfer Thomas
 * \date   17 avr 2008
 */

#include"TFEL/Utilities/Name.hxx"
#include"TFEL/Math/General/Complex.hxx"

/*!
 * \def    TFEL_UTILITIES_NAME
 * \brief  An helper macro which help specialisation of Name for standard types
 * \author Helfer Thomas
 * \date   17 Jul 2006
 */
#define TFEL_UTILITIES_NAME_IMPLEMENTATION(X)                \
    std::string                                              \
    Name< X >::getName(void){                                \
      return std::string(#X);                                \
    }                                                        \
    std::string                                              \
    Name<X*>::getName(void){                                 \
      return std::string("pointer to type "#X);              \
    }                                                        \
    std::string                                              \
    Name<const X * const>::getName(void){                    \
      return std::string("const pointer to const type "#X);  \
    }                                                        \
    std::string                                              \
    Name<const X *>::getName(void){                          \
      return std::string("pointer to const type "#X);        \
    }                                                        \
    std::string                                              \
    Name<X &>::getName(void){                                \
      return std::string("reference to type "#X);            \
    }                                                        \
    std::string                                              \
    Name<const X &>::getName(void){                          \
      return std::string("const reference to type "#X);      \
    }

namespace tfel
{

  namespace utilities
  {

    std::string
    Name<void>::getName(void){
      return std::string("void");
    }

    std::string
    Name<void*>::getName(void){
      return std::string("pointer to type void");
    }

    std::string
    Name<const void* const>::getName(void){
      return std::string("const pointer to const type void");
    }

    std::string
    Name<const void *>::getName(void){
      return std::string("pointer to const type void");
    }

    TFEL_UTILITIES_NAME_IMPLEMENTATION(char)
    TFEL_UTILITIES_NAME_IMPLEMENTATION(bool)
    TFEL_UTILITIES_NAME_IMPLEMENTATION(unsigned short)
    TFEL_UTILITIES_NAME_IMPLEMENTATION(unsigned int)
    TFEL_UTILITIES_NAME_IMPLEMENTATION(long unsigned int)
    TFEL_UTILITIES_NAME_IMPLEMENTATION(short)
    TFEL_UTILITIES_NAME_IMPLEMENTATION(int)
    TFEL_UTILITIES_NAME_IMPLEMENTATION(long int)
    TFEL_UTILITIES_NAME_IMPLEMENTATION(float)
    TFEL_UTILITIES_NAME_IMPLEMENTATION(double)
    TFEL_UTILITIES_NAME_IMPLEMENTATION(long double)
    TFEL_UTILITIES_NAME_IMPLEMENTATION(tfel::math::Complex<unsigned short>)
    TFEL_UTILITIES_NAME_IMPLEMENTATION(tfel::math::Complex<unsigned int>)
    TFEL_UTILITIES_NAME_IMPLEMENTATION(tfel::math::Complex<long unsigned int>)
    TFEL_UTILITIES_NAME_IMPLEMENTATION(tfel::math::Complex<short>)
    TFEL_UTILITIES_NAME_IMPLEMENTATION(tfel::math::Complex<int>)
    TFEL_UTILITIES_NAME_IMPLEMENTATION(tfel::math::Complex<long int>)
    TFEL_UTILITIES_NAME_IMPLEMENTATION(tfel::math::Complex<float>)
    TFEL_UTILITIES_NAME_IMPLEMENTATION(tfel::math::Complex<double>)
    TFEL_UTILITIES_NAME_IMPLEMENTATION(tfel::math::Complex<long double>)
    TFEL_UTILITIES_NAME_IMPLEMENTATION(std::string)

    std::string
    Name<tfel::meta::TLE>::getName(void)
    {
      return "TLE";
    } // end of Name<tfel::meta::TLE>::getName

    std::string
    Name<tfel::math::NoUnit>::getName(void){
      return std::string("[]");
    }

    std::string
    Name<tfel::math::Mass>::getName(void){
      return std::string("[kg]");
    }

    std::string
    Name<tfel::math::Length>::getName(void){
      return std::string("[m]");
    }

    std::string Name<tfel::math::Time>::getName(void){
      return std::string("[s]");
    }

    std::string
    Name<tfel::math::Ampere>::getName(void){
      return std::string("[A]");
    }

    std::string
    Name<tfel::math::Temperature>::getName(void){
      return std::string("[K]");
    }

    std::string
    Name<tfel::math::Candela>::getName(void){
      return std::string("[cd]");
    }

    std::string
    Name<tfel::math::Mole>::getName(void){
      return std::string("[mol]");
    }

    std::string
    Name<tfel::math::Velocity>::getName(void){
      return std::string("[Velocity]");
    }

    std::string 
    Name<tfel::math::Acceleration>::getName(void){
      return std::string("[Acceleration]");
    }

    std::string
    Name<tfel::math::Momentum>::getName(void){
      return std::string("[Momentum]");
    }

    std::string
    Name<tfel::math::Force>::getName(void){
      return std::string("[Force]");
    }

  } // end of namespace utilities


  namespace meta{

    namespace internals {

      std::string
      TLGetNames_<TLE>::exe()
      {
	return "}";
      }

    } // end of namespace internals

  } // end of namespace meta

} // end of namespace tfel
