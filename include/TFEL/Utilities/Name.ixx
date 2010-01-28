/*!
 * \file   Name.ixx
 * 
 * \brief  Specialise Name for the most used standard types.  
 * \author Helfer Thomas
 * \date   17 Jul 2006
 */

#ifndef _LIB_TFEL_NAME_I_
#define _LIB_TFEL_NAME_I_ 

#include <string>
#include"TFEL/Math/General/Complex.hxx"

/*!
 * \def    TFEL_UTILITIES_NAME
 * \brief  An helper macro which help specialisation of Name for standard types
 * \author Helfer Thomas
 * \date   17 Jul 2006
 */
#define TFEL_UTILITIES_NAME(X)                               \
    /*!                                                      \
     * Partial specialisation for X                          \
     */                                                      \
    template<>                                               \
    struct Name< X >                                         \
    {                                                        \
      /*!                                                    \
       * \brief returns the Name of X.                       \
       * \param  void                                        \
       * \return std::string, the name of X.                 \
       */                                                    \
      static                                                 \
      std::string				             \
      getName(void);                                         \
    };                                                       \
    /*!                                                      \
     * Partial specialisation for X*                         \
     */                                                      \
    template<>                                               \
    struct Name<X*>                                          \
    {                                                        \
      /*!                                                    \
       * \brief returns the Name of X.                       \
       * \param  void                                        \
       * \return std::string, the name of X.                 \
       */                                                    \
      static                                                 \
      std::string                                            \
      getName(void);                                         \
    };                                                       \
    /*!                                                      \
     * Partial specialisation for const X * const            \
     */                                                      \
    template<>                                               \
    struct Name<const X * const>                             \
    {                                                        \
      /*!                                                    \
       * \brief returns the Name of X.                       \
       * \param  void                                        \
       * \return std::string, the name of X.                 \
       */                                                    \
      static                                                 \
      std::string		   	  	             \
      getName(void);                                         \
    };                                                       \
    /*!                                                      \
     * Partial specialisation for const X*                   \
     */                                                      \
    template<>                                               \
    struct Name<const X *>                                   \
    {                                                        \
      /*!                                                    \
       * \brief returns the Name of X.                       \
       * \param  void                                        \
       * \return std::string, the name of X.                 \
       */                                                    \
      static                                                 \
      std::string                                            \
      getName(void);                                         \
    };                                                       \
    /*!                                                      \
     * Partial specialisation for  X&                        \
     */                                                      \
    template<>                                               \
    struct Name<X &>                                         \
    {                                                        \
      /*!                                                    \
       * \brief returns the Name of X.                       \
       * \param  void                                        \
       * \return std::string, the name of X.                 \
       */                                                    \
      static                                                 \
      std::string                                            \
      getName(void);                                         \
    };                                                       \
    /*!                                                      \
     * Partial specialisation for const X&                   \
     */                                                      \
    template<>                                               \
    struct Name<const X &>                                   \
    {                                                        \
      /*!                                                    \
       * \brief returns the Name of X.                       \
       * \param  void                                        \
       * \return std::string, the name of X.                 \
       */                                                    \
      static                                                 \
      std::string        				     \
      getName(void);                                         \
    }

#define TFEL_UTILITIES_NAME_VOID                                \
    /*!                                                         \
     * Partial specialisation for void                          \
     */                                                         \
    template<>                                                  \
    struct Name<void>                                           \
    {                                                           \
      /*!                                                       \
       * \brief returns the Name of X.                          \
       * \param  void                                           \
       * \return std::string, the name of X.                    \
       */                                                       \
      static                                                    \
      std::string                                               \
      getName(void);                                            \
    };                                                          \
    /*!                                                         \
     * Partial specialisation for void*                         \
     */                                                         \
    template<>                                                  \
    struct Name<void*>                                          \
    {                                                           \
      /*!                                                       \
       * \brief returns the Name of X.                          \
       * \param  void                                           \
       * \return std::string, the name of X.                    \
       */                                                       \
      static                                                    \
      std::string                                               \
      getName(void);                                            \
    };                                                          \
    /*!                                                         \
     * Partial specialisation for const void* const             \
     */                                                         \
    template<>                                                  \
    struct Name<const void* const>                              \
    {                                                           \
      /*!                                                       \
       * \brief returns the Name of X.                          \
       * \param  void                                           \
       * \return std::string, the name of X.                    \
       */                                                       \
      static                                                    \
      std::string                                               \
      getName(void);                                            \
    };                                                          \
    /*!                                                         \
     * Partial specialisation for const void*                   \
     */                                                         \
    template<>                                                  \
    struct Name<const void *>                                   \
    {                                                           \
      /*!                                                       \
       * \brief returns the Name of X.                          \
       * \param  void                                           \
       * \return std::string, the name of X.                    \
       */                                                       \
      static                                                    \
      std::string                                               \
      getName(void);                                            \
    }                                                       

namespace tfel
{
  
  namespace utilities
  {

    /*!
     * Partial specialisation for some pointer functions.
     */
    template<typename T>
    struct Name<T (*)(T)>
    {
      /*!
       * \brief returns the Name of the pointer functions.
       * \param  void
       * \return std::string.
       */
      static
      std::string 
      getName(void){
	return Name<T>::getName()+std::string("(*)(")+Name<T>::getName()+std::string(")");
      }
    };

    /*!
     * Partial specialisation for some pointer functions.
     */
    template<typename T1,typename T2>
    struct Name<T1 (*)(T2)>
    {
      /*!
       * \brief returns the Name of the pointer function.
       * \param  void
       * \return std::string.
       */
      static
      std::string 
      getName(void){
	return Name<T1>::getName()+std::string("(*)(")+Name<T2>::getName()+std::string(")");
      }
    };

    /*!
     * Partial specialisation for some pointer functions.
     */
    template<typename T1,typename T2,typename T3>
    struct Name<T1 (*)(T2,T3)>
    {
      /*!
       * \brief returns the Name of the pointer function.
       * \param  void
       * \return std::string.
       */
      static
      std::string
      getName(void){
	return Name<T1>::getName()+std::string("(*)(")+Name<T2>::getName()+std::string(",")+Name<T3>::getName()+std::string(")");
      }
    };

    /*!
     * Partial specialisation for some pointer functions.
     */
    template<typename T1,typename T2,typename T3,typename T4>
    struct Name<T1 (*)(T2,T3,T4)>
    {
      /*!
       * \brief returns the Name of the pointer function.
       * \param  void
       * \return std::string.
       */
      static
      std::string
      getName(void){
	return Name<T1>::getName()+std::string("(*)(")+Name<T2>::getName()+std::string(",")+Name<T3>::getName()+std::string(",")+Name<T4>::getName()+std::string(")");
      }
    };

    /*!
     * Partial specialisation for const variable.
     */
    template<typename T>
    struct Name<const T>
    {
      /*!
       * \brief returns the Name of the pointer function.
       * \param  void
       * \return std::string.
       */
      static
      std::string
      getName(void){
	return std::string("const ")+Name<T>::getName();
      }
    };

    /*!
     * Partial specialisation for pointers.
     */
    template<typename T>
    struct Name<T*>
    {
      /*!
       * \brief returns the Name of the pointer.
       * \param  void
       * \return std::string.
       */
      static
      std::string getName(void){
	return std::string("pointer to type ")+Name<T>::getName();
      }
    };

    /*!
     * Partial specialisation for pointers to const object.
     */
    template<typename T>
    struct Name<const T*>
    {
      /*!
       * \brief returns the Name of the pointer to const.
       * \param  void
       * \return std::string.
       */
      static
      std::string
      getName(void){
	return std::string("pointer to const type ")+Name<T>::getName();
      }
    };

    /*!
     * Partial specialisation for const pointers to object.
     */
    template<typename T>
    struct Name<T*const>
    {
      /*!
       * \brief returns the Name of the const pointer.
       * \param  void
       * \return std::string.
       */
      static
      std::string
      getName(void){
	return std::string("const pointer to type ")+Name<T>::getName();
      }
    };

    /*!
     * Partial specialisation for const pointers to const object.
     */
    template<typename T>
    struct Name<const T* const>
    {
      /*!
       * \brief returns the Name of the const pointer to const.
       * \param  void
       * \return std::string.
       */
      static
      std::string
      getName(void){
	return std::string("const pointer to const type ")+Name<T>::getName();
      }
    };

    /*!
     * Partial specialisation for reference to object.
     */
    template<typename T>
    struct Name<T&>
    {
      /*!
       * \brief returns the Name of the reference.
       * \param  void
       * \return std::string.
       */
      static
      std::string
      getName(void){
	return std::string("reference to type ")+Name<T>::getName();
      }
    };

    /*!
     * Partial specialisation for const reference to object.
     */
    template<typename T>
    struct Name<const T&>
    {
      /*!
       * \brief returns the Name of the const reference.
       * \param  void
       * \return std::string.
       */
      static
      std::string
      getName(void){
	return std::string("const reference to type ")+Name<T>::getName();
      }
    };

    TFEL_UTILITIES_NAME_VOID;
    TFEL_UTILITIES_NAME(char);
    TFEL_UTILITIES_NAME(unsigned short);
    TFEL_UTILITIES_NAME(unsigned int);
    TFEL_UTILITIES_NAME(long unsigned int);
    TFEL_UTILITIES_NAME(short);
    TFEL_UTILITIES_NAME(int);
    TFEL_UTILITIES_NAME(long int);
    TFEL_UTILITIES_NAME(float);
    TFEL_UTILITIES_NAME(double);
    TFEL_UTILITIES_NAME(long double);
    TFEL_UTILITIES_NAME(tfel::math::Complex<unsigned short>);
    TFEL_UTILITIES_NAME(tfel::math::Complex<unsigned int>);
    TFEL_UTILITIES_NAME(tfel::math::Complex<long unsigned int>);
    TFEL_UTILITIES_NAME(tfel::math::Complex<short>);
    TFEL_UTILITIES_NAME(tfel::math::Complex<int>);
    TFEL_UTILITIES_NAME(tfel::math::Complex<long int>);
    TFEL_UTILITIES_NAME(tfel::math::Complex<float>);
    TFEL_UTILITIES_NAME(tfel::math::Complex<double>);
    TFEL_UTILITIES_NAME(tfel::math::Complex<long double>);
    TFEL_UTILITIES_NAME(std::string);
    
    template<typename T>
    std::string
    name(const T&)
    {
      return Name<T>::getName();
    } // end of name

  } // end of namespace utilities

} // end of namespace tfel

#endif /* _LIB_TFEL_NAME_I_ */

