/*!
 * \file   include/TFEL/System/ExternalLibraryManager.hxx
 * \brief
 * \author Helfer Thomas
 * \date   31 Oct 2007
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_EXTERNALLIBRARYMANAGER_H_
#define LIB_EXTERNALLIBRARYMANAGER_H_

#include <map>
#include <vector>
#include <string>

#if (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__)
#include <windows.h>
#ifdef small
#undef small
#endif /* small */
#endif /* (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__) */

#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Material/OutOfBoundsPolicy.hxx"
#include "TFEL/System/ExternalFunctionsPrototypes.hxx"

namespace tfel {

  namespace system {

    /*!
     * \brief Structure in charge of loading external function and
     * retrieving information from shared libraries.
     */
    struct TFELSYSTEM_VISIBILITY_EXPORT ExternalLibraryManager {
      //! \return the uniq instance of this class
      static ExternalLibraryManager& getExternalLibraryManager();
      /*!
       * \brief a wrapper around the ::dlopen system call
       * \param[in] name : name of the library
       * \param[in] b : boolean allowing ::dlopen to fail. If ::dlopen
       * fails, a null pointer is returned. This library is *not*
       * registred.
       * \return a pointer to the library
       * \note on success, the pointer is registred in a map using its
       * name as a key. This name is used in the methods of this call
       */
#if (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__)
      HINSTANCE__* loadLibrary(const std::string&, const bool = false);
#else
      void* loadLibrary(const std::string&, const bool = false);
#endif /* LIB_EXTERNALLIBRARYMANAGER_H_ */

      /*!
       * \param[in] l : name of the library
       * \param[in] f : name of function or mechanical behaviour
       * \param[in] p : out of bounds policy value
       */
      void setOutOfBoundsPolicy(const std::string&,
                                const std::string&,
                                const tfel::material::OutOfBoundsPolicy);
      /*!
       * \param[in] l : name of the library
       * \param[in] s : name of function or mechanical behaviour
       * \param[in] p : parameter name
       * \param[in] v : value
       */
      void setParameter(const std::string&,
                        const std::string&,
                        const std::string&,
                        const double);
      /*!
       * set the value of an integer parameter
       * \param[in] l : name of the library
       * \param[in] s : name of function or mechanical behaviour
       * \param[in] p : parameter name
       * \param[in] v : value
       */
      void setParameter(const std::string&,
                        const std::string&,
                        const std::string&,
                        const int);
      /*!
       * set the value of an unsigned short parameter
       * \param[in] l : name of the library
       * \param[in] s : name of function or mechanical behaviour
       * \param[in] p : parameter name
       * \param[in] v : value
       */
      void setParameter(const std::string&,
                        const std::string&,
                        const std::string&,
                        const unsigned short);
      /*!
       * \param[in] l : name of the library
       * \param[in] s : name of function or mechanical behaviour
       * \param[in] h : modelling hypothesis
       * \param[in] p : parameter name
       * \param[in] v : value
       */
      void setParameter(const std::string&,
                        const std::string&,
                        const std::string&,
                        const std::string&,
                        const double);
      /*!
       * set the value of an integer parameter
       * \param[in] l : name of the library
       * \param[in] s : name of function or mechanical behaviour
       * \param[in] h : modelling hypothesis
       * \param[in] p : parameter name
       * \param[in] v : value
       */
      void setParameter(const std::string&,
                        const std::string&,
                        const std::string&,
                        const std::string&,
                        const int);
      /*!
       * set the value of an unsigned short parameter
       * \param[in] l : name of the library
       * \param[in] s : name of function or mechanical behaviour
       * \param[in] h : modelling hypothesis
       * \param[in] p : parameter name
       * \param[in] v : value
       */
      void setParameter(const std::string&,
                        const std::string&,
                        const std::string&,
                        const std::string&,
                        const unsigned short);
      /*!
       * \return the src of the function or the mechanical behaviour
       * \param[in] l: name of the library
       * \param[in] s: name of function or mechanical behaviour
       * This function looks for the symbol s+'_src' in the library
       * and expect it to a pointer to characters.
       * If the symbol is not found, an empty string is returned.
       */
      std::string getSource(const std::string&, const std::string&);
      /*!
       * \return the interface of the function or the mechanical behaviour
       * \param[in] l: name of the library
       * \param[in] s: name of function or mechanical behaviour
       * This function looks for the symbol s+'_interface' in the library
       * and expect it to a pointer to characters.
       * If the symbol is not found, an empty string is returned.
       */
      std::string getInterface(const std::string&, const std::string&);
      /*!
       * \return the list of supported modelling hypotheses
       * \param[in] l : name of the library
       * \param[in] s : name of function or mechanical behaviour
       * This function looks for the symbol s+'_ModellingHypotheses'
       * in the library and expect it to a pointer to characters. If
       * the symbol is not found, an empty vector is returned.
       */
      std::vector<std::string> getSupportedModellingHypotheses(
          const std::string&, const std::string&);
      /*!
       * \param[in] l : name of the library
       * \param[in] f : law name
       */
      bool contains(const std::string&, const std::string&);
      /*!
       * \param[in] l : name of the library
       * \param[in] f : law name
       */
      CyranoFctPtr getCyranoFunction(const std::string&, const std::string&);
      /*!
       * \param[in] l : name of the library
       * \param[in] f : law name
       */
      AsterFctPtr getAsterFunction(const std::string&, const std::string&);
      /*!
       * \param[in] l : name of the library
       * \param[in] f : law name
       */
      EuroplexusFctPtr getEuroplexusFunction(const std::string&,
                                             const std::string&);
      /*!
       * \param[in] l : name of the library
       * \param[in] f : law name
       */
      CastemFctPtr getCastemExternalBehaviourFunction(const std::string&,
                                                      const std::string&);
      /*!
       * \param[in] l : name of the library
       * \param[in] f : law name
       */
      AbaqusFctPtr getAbaqusExternalBehaviourFunction(const std::string&,
                                                      const std::string&);
      /*!
       * \param[in] l : name of the library
       * \param[in] f : law name
       */
      AbaqusExplicitFctPtr getAbaqusExplicitExternalBehaviourFunction(
          const std::string&, const std::string&);
      /*!
       * \note the loadLibrary must have already been called with this name
       */
      bool isUMATBehaviourUsableInPurelyImplicitResolution(const std::string&,
                                                           const std::string&,
                                                           const std::string&);
      /*!
       * \return the type of the behaviour
       * \see MechanicalBehaviourBase::BehaviourType
       * The value returned are the following
       * 0 : general behaviour
       * 1 : small strain behaviour
       * 2 : finite strain behaviour
       * 3 : cohesive zone model
       * \param[in] l : name of the library
       * \param[in] f : law name
       */
      unsigned short getUMATBehaviourType(const std::string&,
                                          const std::string&);
      /*!
       * \return the kinematic assumption used by the behaviour
       * \see MechanicalBehaviourBase::Kinematic
       * The value returned are the following
       * 0 : undefined kinematic
       * 1 : standard small strain behaviour kinematic
       * 2 : cohesive zone model kinematic
       * 3 : standard finite strain kinematic (F-Cauchy)
       * 4 : ptest finite strain kinematic (eto-pk1)
       * \param[in] l : name of the library
       * \param[in] f : law name
       */
      unsigned short getUMATBehaviourKinematic(const std::string&,
                                               const std::string&);
      /*!
       * \return the symmetry of the behaviour (isotropic or
       * orthotropic) If the returned value is 0, the beahviour is
       * isotropic. If the returned value is 1, the beahviour is
       * orthotropic.
       * \param[in] l : name of the library
       * \param[in] f : law name
       */
      unsigned short getUMATSymmetryType(const std::string&,
                                         const std::string&);
      /*!
       * \return the symmetry of the elastic behaviour (isotropic or
       * orthotropic) If the returned value is 0, the beahviour is
       * isotropic. If the returned value is 1, the beahviour is
       * orthotropic.
       * \param[in] l : name of the library
       * \param[in] f : law name
       */
      unsigned short getUMATElasticSymmetryType(const std::string&,
                                                const std::string&);

      /*!
       * \return true if the umat behaviour uses the generic plane stress
       * algorithm \param[in] l : name of the library \param[in] f : law name
       */
      bool checkIfUMATBehaviourUsesGenericPlaneStressAlgorithm(
          const std::string&, const std::string&);
      /*!
       * \param[in] l : name of the library
       * \param[in] f : law name
       * \param[in] h : modelling hypothesis
       */
      std::vector<std::string> getUMATMaterialPropertiesNames(
          const std::string&, const std::string&, const std::string&);
      /*!
       * \param[in] l : name of the library
       * \param[in] f : law name
       * \param[in] h : modelling hypothesis
       */
      std::vector<std::string> getUMATInternalStateVariablesNames(
          const std::string&, const std::string&, const std::string&);

      /*!
       * \param[in] l : name of the library
       * \param[in] f : law name
       * \param[in] h : modelling hypothesis
       */
      std::vector<int> getUMATInternalStateVariablesTypes(const std::string&,
                                                          const std::string&,
                                                          const std::string&);

      /*!
       * \param[in] l : name of the library
       * \param[in] f : law name
       * \param[in] h : modelling hypothesis
       */
      std::vector<std::string> getUMATExternalStateVariablesNames(
          const std::string&, const std::string&, const std::string&);

      /*!
       * \return true if a behaviour generated throught the aster
       * interface requires a offset for the elastic properties
       * \param[in] l : name of the library
       * \param[in] f : law name
       * \param[in] h : modelling hypothesis
       */
      bool getUMATRequiresStiffnessTensor(const std::string&,
                                          const std::string&,
                                          const std::string&);
      /*!
       * \return true if a behaviour generated throught the aster
       * interface requires a offset for the elastic properties
       * \param[in] l : name of the library
       * \param[in] f : law name
       * \param[in] h : modelling hypothesis
       */
      bool getUMATRequiresThermalExpansionCoefficientTensor(const std::string&,
                                                            const std::string&,
                                                            const std::string&);
      /*!
       * \return true if a behaviour generated throught the aster
       * interface saves the tangent operator
       * \param[in] l : name of the library
       * \param[in] f : law name
       */
      bool checkIfAsterBehaviourSavesTangentOperator(const std::string&,
                                                     const std::string&);
      /*!
       * \return the orthotropy management policy for the Abaqus and
       * Abaqus/Explicit interface. See the associated documentation
       * for details.
       *
       * The return value has the following meaning:
       *
       * - 0: unspecified policy (equivalent to native)
       * - 1: native policy (equivalent to native)
       * - 2: mfront policy
       *
       * \param[in] l : name of the library
       * \param[in] f : law name
       */
      unsigned short getAbaqusOrthotropyManagementPolicy(const std::string&,
                                                         const std::string&);
      /*!
       * \param[in] l : name of the library
       * \param[in] f : function name
       */
      unsigned short getCastemFunctionNumberOfVariables(const std::string&,
                                                        const std::string&);

      /*!
       * \param[in] l : name of the library
       * \param[in] f : function name
       */
      std::vector<std::string> getCastemFunctionVariables(const std::string&,
                                                          const std::string&);

      /*!
       * \param[in] l : name of the library
       * \param[in] f : function name
       */
      void getCastemFunctionVariables(std::vector<std::string>&,
                                      const std::string&,
                                      const std::string&);

      /*!
       * load a function from a library
       * \param l : library  name
       * \param f : function name
       * \return the function pointer
       */
      CastemFunctionPtr getCastemFunction(const std::string&,
                                          const std::string&);

      /*!
       * load a function from a library
       * \param l : library  name
       * \param f : function name
       * \return the function pointer
       */
      CFunction0Ptr getCFunction0(const std::string&, const std::string&);

      /*!
       * load a function from a library
       * \param l : library  name
       * \param f : function name
       * \return the function pointer
       */
      CFunction1Ptr getCFunction1(const std::string&, const std::string&);

      /*!
       * load a function from a library
       * \param l : library  name
       * \param f : function name
       * \return the function pointer
       */
      CFunction2Ptr getCFunction2(const std::string&, const std::string&);

      /*!
       * load a function from a library
       * \param l : library  name
       * \param f : function name
       * \return the function pointer
       */
      CFunction3Ptr getCFunction3(const std::string&, const std::string&);

      /*!
       * load a function from a library
       * \param l : library  name
       * \param f : function name
       * \return the function pointer
       */
      CFunction4Ptr getCFunction4(const std::string&, const std::string&);

      /*!
       * load a function from a library
       * \param l : library  name
       * \param f : function name
       * \return the function pointer
       */
      CFunction5Ptr getCFunction5(const std::string&, const std::string&);

      /*!
       * load a function from a library
       * \param l : library  name
       * \param f : function name
       * \return the function pointer
       */
      CFunction6Ptr getCFunction6(const std::string&, const std::string&);

      /*!
       * load a function from a library
       * \param l : library  name
       * \param f : function name
       * \return the function pointer
       */
      CFunction7Ptr getCFunction7(const std::string&, const std::string&);

      /*!
       * load a function from a library
       * \param l : library  name
       * \param f : function name
       * \return the function pointer
       */
      CFunction8Ptr getCFunction8(const std::string&, const std::string&);

      /*!
       * load a function from a library
       * \param l : library  name
       * \param f : function name
       * \return the function pointer
       */
      CFunction9Ptr getCFunction9(const std::string&, const std::string&);

      /*!
       * load a function from a library
       * \param l : library  name
       * \param f : function name
       * \return the function pointer
       */
      CFunction10Ptr getCFunction10(const std::string&, const std::string&);

      /*!
       * load a function from a library
       * \param l : library  name
       * \param f : function name
       * \return the function pointer
       */
      CFunction11Ptr getCFunction11(const std::string&, const std::string&);

      /*!
       * load a function from a library
       * \param l : library  name
       * \param f : function name
       * \return the function pointer
       */
      CFunction12Ptr getCFunction12(const std::string&, const std::string&);

      /*!
       * load a function from a library
       * \param l : library  name
       * \param f : function name
       * \return the function pointer
       */
      CFunction13Ptr getCFunction13(const std::string&, const std::string&);

      /*!
       * load a function from a library
       * \param l : library  name
       * \param f : function name
       * \return the function pointer
       */
      CFunction14Ptr getCFunction14(const std::string&, const std::string&);

      /*!
       * load a function from a library
       * \param l : library  name
       * \param f : function name
       * \return the function pointer
       */
      CFunction15Ptr getCFunction15(const std::string&, const std::string&);

      /*!
       * load a function from a library
       * \param l : library  name
       * \param f : function name
       * \return the function pointer
       */
      FortranFunction0Ptr getFortranFunction0(const std::string&,
                                              const std::string&);

      /*!
       * load a function from a library
       * \param l : library  name
       * \param f : function name
       * \return the function pointer
       */
      FortranFunction1Ptr getFortranFunction1(const std::string&,
                                              const std::string&);

      /*!
       * load a function from a library
       * \param l : library  name
       * \param f : function name
       * \return the function pointer
       */
      FortranFunction2Ptr getFortranFunction2(const std::string&,
                                              const std::string&);

      /*!
       * load a function from a library
       * \param l : library  name
       * \param f : function name
       * \return the function pointer
       */
      FortranFunction3Ptr getFortranFunction3(const std::string&,
                                              const std::string&);

      /*!
       * load a function from a library
       * \param l : library  name
       * \param f : function name
       * \return the function pointer
       */
      FortranFunction4Ptr getFortranFunction4(const std::string&,
                                              const std::string&);

      /*!
       * load a function from a library
       * \param l : library  name
       * \param f : function name
       * \return the function pointer
       */
      FortranFunction5Ptr getFortranFunction5(const std::string&,
                                              const std::string&);

      /*!
       * load a function from a library
       * \param l : library  name
       * \param f : function name
       * \return the function pointer
       */
      FortranFunction6Ptr getFortranFunction6(const std::string&,
                                              const std::string&);

      /*!
       * load a function from a library
       * \param l : library  name
       * \param f : function name
       * \return the function pointer
       */
      FortranFunction7Ptr getFortranFunction7(const std::string&,
                                              const std::string&);

      /*!
       * load a function from a library
       * \param l : library  name
       * \param f : function name
       * \return the function pointer
       */
      FortranFunction8Ptr getFortranFunction8(const std::string&,
                                              const std::string&);

      /*!
       * load a function from a library
       * \param l : library  name
       * \param f : function name
       * \return the function pointer
       */
      FortranFunction9Ptr getFortranFunction9(const std::string&,
                                              const std::string&);

      /*!
       * load a function from a library
       * \param l : library  name
       * \param f : function name
       * \return the function pointer
       */
      FortranFunction10Ptr getFortranFunction10(const std::string&,
                                                const std::string&);

      /*!
       * load a function from a library
       * \param l : library  name
       * \param f : function name
       * \return the function pointer
       */
      FortranFunction11Ptr getFortranFunction11(const std::string&,
                                                const std::string&);

      /*!
       * load a function from a library
       * \param l : library  name
       * \param f : function name
       * \return the function pointer
       */
      FortranFunction12Ptr getFortranFunction12(const std::string&,
                                                const std::string&);

      /*!
       * load a function from a library
       * \param l : library  name
       * \param f : function name
       * \return the function pointer
       */
      FortranFunction13Ptr getFortranFunction13(const std::string&,
                                                const std::string&);

      /*!
       * load a function from a library
       * \param l : library  name
       * \param f : function name
       * \return the function pointer
       */
      FortranFunction14Ptr getFortranFunction14(const std::string&,
                                                const std::string&);

      /*!
       * load a function from a library
       * \param l : library  name
       * \param f : function name
       * \return the function pointer
       */
      FortranFunction15Ptr getFortranFunction15(const std::string&,
                                                const std::string&);

      ~ExternalLibraryManager();

     private:
      TFEL_VISIBILITY_LOCAL ExternalLibraryManager();

      TFEL_VISIBILITY_LOCAL ExternalLibraryManager(
          const ExternalLibraryManager&);

      TFEL_VISIBILITY_LOCAL ExternalLibraryManager& operator=(
          const ExternalLibraryManager&);

      TFEL_VISIBILITY_LOCAL void getUMATNames(std::vector<std::string>&,
                                              const std::string&,
                                              const std::string&,
                                              const std::string&,
                                              const std::string&);

#if (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__)
      std::map<std::string, HINSTANCE__*> librairies;
#else
      std::map<std::string, void*> librairies;
#endif /* LIB_EXTERNALLIBRARYMANAGER_H_ */

    };  // end of struct LibraryManager

  }  // end of namespace system

}  // end of namespace tfel

#endif /* LIB_EXTERNALLIBRARYMANAGER_H_ */
