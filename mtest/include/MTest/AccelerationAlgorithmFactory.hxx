/*! 
 * \file  mfront/include/MTest/AccelerationAlgorithmFactory.hxx
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

#ifndef LIB_MFRONT_MTESTACCELERATIONALGORITHMFACTORY_H_
#define LIB_MFRONT_MTESTACCELERATIONALGORITHMFACTORY_H_ 

#include<map>
#include<string>

#include<memory>

#include"MTest/Config.hxx"
#include"MTest/AccelerationAlgorithm.hxx"

namespace mtest
{
  
  /*!
   * register the acceleration algorithms
   */
  struct AccelerationAlgorithmFactory
  {
    //! a simple alias
    typedef std::shared_ptr<AccelerationAlgorithm> (* constructor)(void);
    /*!
     * \return the uniq instance of the acceleration algorithm
     */
    static AccelerationAlgorithmFactory&
    getAccelerationAlgorithmFactory();
    /*!
     * \return the requested algorithm
     * \param[in] a : algorithm name
     */
    std::shared_ptr<AccelerationAlgorithm>
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
    AccelerationAlgorithmFactory();
    /*!
     * copy constructor (disabled)
     */
    AccelerationAlgorithmFactory(const AccelerationAlgorithmFactory&);
    /*!
     * assignement operator (disabled)
     */
    AccelerationAlgorithmFactory&
    operator=(const AccelerationAlgorithmFactory&);
    //! all registred constructors
    std::map<std::string,constructor> constructors;
  };

} // end of namespace mtest

#endif /* LIB_MFRONT_MTESTACCELERATIONALGORITHMFACTORY_H_ */

