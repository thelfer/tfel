/*! 
 * \file  mfront/include/MFront/MTestAccelerationAlgorithmFactory.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 10 juin 2014
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_MFRONT_MTESTACCELERATIONALGORITHMFACTORY_H_
#define _LIB_MFRONT_MTESTACCELERATIONALGORITHMFACTORY_H_ 

#include<map>
#include<string>

#include"TFEL/Utilities/SmartPtr.hxx"

#include"MFront/MTestConfig.hxx"
#include"MFront/MTestAccelerationAlgorithm.hxx"

namespace mfront
{
  
  /*!
   * register the acceleration algorithms
   */
  struct MTestAccelerationAlgorithmFactory
  {
    //! a simple alias
    typedef tfel::utilities::shared_ptr<MTestAccelerationAlgorithm> (* constructor)(void);
    /*!
     * \return the uniq instance of the acceleration algorithm
     */
    static MTestAccelerationAlgorithmFactory&
    getMTestAccelerationAlgorithmFactory();
    /*!
     * \return the requested algorithm
     * \param[in] a : algorithm name
     */
    tfel::utilities::shared_ptr<MTestAccelerationAlgorithm>
    getAlgorithm(const std::string&) const;
    /*!
     * \param[in] a : algorithm name 
     * \param[in] c : algorithm constructor 
     */
    void
    registerAlgorithm(const std::string&,
		      const constructor);
  private:
    /*!
     * default constructor
     */
    MTestAccelerationAlgorithmFactory();
    /*!
     * copy constructor (disabled)
     */
    MTestAccelerationAlgorithmFactory(const MTestAccelerationAlgorithmFactory&);
    /*!
     * assignement operator (disabled)
     */
    MTestAccelerationAlgorithmFactory&
    operator=(const MTestAccelerationAlgorithmFactory&);
    //! all registred constructors
    std::map<std::string,constructor> constructors;
  };

} // end of namespace mfront

#endif /* _LIB_MFRONT_MTESTACCELERATIONALGORITHMFACTORY_H */

