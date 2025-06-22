/*!
 * \file   Provider.hxx
 * \brief
 * \author Thomas Helfer
 * \date   25 juin 2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 * <!-- Local IspellDict: english -->
 */

#ifndef LIB_MFRONT_BEHAVIOURBRICK_REQUIREMENTPROVIDER_HXX
#define LIB_MFRONT_BEHAVIOURBRICK_REQUIREMENTPROVIDER_HXX

#include <string>

#include "MFront/MFrontConfig.hxx"
#include "MFront/BehaviourBrick/ProviderIdentifier.hxx"

namespace mfront {

  // forward declaration
  struct VariableDescription;
  // forward declaration
  struct StaticVariableDescription;

  namespace bbrick {

    // forward declaration
    struct Requirement;

    /*!
     * This structure describes a provider of a requirement of a
     * behaviour brick.  This structure is meant to have only a limited
     * number of child, each associated to a value of the
     * Requirement::VariableProvider enum.
     */
    struct MFRONT_VISIBILITY_EXPORT Provider {
      //! \return the type of the variable that can be produced
      virtual std::string getVariableType() const = 0;
      //! \return the external name of the variable that can be produced
      virtual std::string getExternalName() const = 0;
      //! \return the array size of the variable
      virtual unsigned short getArraySize() const = 0;
      /*!
       * \return true if the given requirement can be handled by the provider
       * \param[in] r : requirement
       * \param[in] b : check if units match
       */
      virtual bool handleRequirement(const Requirement&, const bool) const = 0;
      //! \return the identifier of the provider
      virtual ProviderIdentifier getIdentifier() const = 0;
      //! destructor
      virtual ~Provider();
    };  // end of struct Provider

    /*!
     * An auxiliary structure implementing part of the Provider interface.
     */
    struct MFRONT_VISIBILITY_EXPORT ProviderBase : public Provider {
      /*!
       * \return true if the given requirement can be handled by the provider
       * \param[in] r : requirement
       * \param[in] b : check if units match
       */
      bool handleRequirement(const Requirement&, const bool) const override;
      //! destructor
      ~ProviderBase() override;
    };  // end of struct ProviderBase

    /*!
     * A base class for provider standing for a variable declared by
     * the user
     */
    struct MFRONT_VISIBILITY_EXPORT StandardProvider : public ProviderBase {
      /*!
       * constructor
       * \param[in] t: variable type
       * \param[in] n: variable name
       * \param[in] e: external name
       * \param[in] s: array size
       * \param[in] b: check if variable type is suppported (See the
       * SupportedType class for details)
       */
      StandardProvider(const std::string&,
                       const std::string&,
                       const std::string&,
                       const unsigned short,
                       const bool);
      /*!
       * constructor
       * \param[in] v : variable description
       * \param[in] e : external name
       * \param[in] b: check if variable type is suppported (See the
       * SupportedType class for details)
       */
      StandardProvider(const mfront::VariableDescription&,
                       const std::string&,
                       const bool);
      //! \return the type of the variable that can be produced
      std::string getVariableType() const override final;
      //! \return the external name of the variable that can be produced
      std::string getExternalName() const override final;
      //! \return the array size of the variable
      unsigned short getArraySize() const override final;
      //! variable type
      const std::string type;
      //! variable name
      const std::string name;
      //! external variable name
      const std::string ename;
      //! array size
      const unsigned short asize;
      //! destructor
      ~StandardProvider() override;

     private:
      StandardProvider& operator=(StandardProvider&&) = delete;
      StandardProvider& operator=(const StandardProvider&) = delete;
    };  // end of struct StandardProvider

    /*!
     * Provider standing for a material property
     */
    struct MFRONT_VISIBILITY_EXPORT MaterialPropertyProvider
        : public StandardProvider {
      /*!
       * constructor
       * \param[in] t : variable type
       * \param[in] n : variable name
       * \param[in] e : external name
       * \param[in] s : array size
       */
      MaterialPropertyProvider(const std::string&,
                               const std::string&,
                               const std::string&,
                               const unsigned short);
      /*!
       * constructor
       * \param[in] v : variable description
       * \param[in] e : external name
       */
      MaterialPropertyProvider(const mfront::VariableDescription&,
                               const std::string&);
      //! \return the identifier of the provider
      ProviderIdentifier getIdentifier() const override final;
      //! destructor
      ~MaterialPropertyProvider() override;
    };  // end of struct MaterialPropertyProvider

    /*!
     * Provider standing for an auxiliary state variable
     */
    struct MFRONT_VISIBILITY_EXPORT AuxiliaryStateVariableProvider
        : public StandardProvider {
      /*!
       * constructor
       * \param[in] t : variable type
       * \param[in] n : variable name
       * \param[in] e : external name
       * \param[in] s : array size
       */
      AuxiliaryStateVariableProvider(const std::string&,
                                     const std::string&,
                                     const std::string&,
                                     const unsigned short);
      /*!
       * constructor
       * \param[in] v : variable description
       * \param[in] e : external name
       */
      AuxiliaryStateVariableProvider(const mfront::VariableDescription&,
                                     const std::string&);
      //! \return the identifier of the provider
      ProviderIdentifier getIdentifier() const override final;
      //! destructor
      ~AuxiliaryStateVariableProvider() override;
    };  // end of struct AuxiliaryStateVariableProvider

    /*!
     * Provider standing for an external state variable
     */
    struct MFRONT_VISIBILITY_EXPORT ExternalStateVariableProvider
        : public StandardProvider {
      /*!
       * constructor
       * \param[in] t : variable type
       * \param[in] n : variable name
       * \param[in] e : external name
       * \param[in] s : array size
       */
      ExternalStateVariableProvider(const std::string&,
                                    const std::string&,
                                    const std::string&,
                                    const unsigned short);
      /*!
       * constructor
       * \param[in] v : variable description
       * \param[in] e : external name
       */
      ExternalStateVariableProvider(const mfront::VariableDescription&,
                                    const std::string&);
      //! \return the identifier of the provider
      ProviderIdentifier getIdentifier() const override final;
      //! destructor
      ~ExternalStateVariableProvider() override;
    };  // end of struct ExternalStateVariableProvider

    /*!
     * Provider standing for an integration variable
     */
    struct MFRONT_VISIBILITY_EXPORT IntegrationVariableProvider
        : public StandardProvider {
      /*!
       * constructor
       * \param[in] t : variable type
       * \param[in] n : variable name
       * \param[in] e : external name
       * \param[in] s : array size
       */
      IntegrationVariableProvider(const std::string&,
                                  const std::string&,
                                  const std::string&,
                                  const unsigned short);
      /*!
       * constructor
       * \param[in] v : variable description
       * \param[in] e : external name
       */
      IntegrationVariableProvider(const mfront::VariableDescription&,
                                  const std::string&);
      //! \return the identifier of the provider
      ProviderIdentifier getIdentifier() const override final;
      //! destructor
      ~IntegrationVariableProvider() override;
    };  // end of struct IntegrationVariableProvider

    /*!
     * Provider standing for a local variable
     */
    struct MFRONT_VISIBILITY_EXPORT LocalVariableProvider
        : public StandardProvider {
      /*!
       * constructor
       * \param[in] t : variable type
       * \param[in] n : variable name
       * \param[in] e : external name
       * \param[in] s : array size
       */
      LocalVariableProvider(const std::string&,
                            const std::string&,
                            const std::string&,
                            const unsigned short);
      /*!
       * constructor
       * \param[in] v : variable description
       * \param[in] e : external name
       */
      LocalVariableProvider(const mfront::VariableDescription&,
                            const std::string&);
      //! \return the identifier of the provider
      ProviderIdentifier getIdentifier() const override final;
      //! destructor
      ~LocalVariableProvider() override;
    };  // end of struct LocalVariableProvider

    /*!
     * Provider standing for a static variable
     */
    struct MFRONT_VISIBILITY_EXPORT StaticVariableProvider
        : public ProviderBase {
      /*!
       * constructor
       * \param[in] t : variable type
       * \param[in] n : variable name
       * \param[in] e : external name
       */
      StaticVariableProvider(const std::string&,
                             const std::string&,
                             const std::string&);
      /*!
       * constructor
       * \param[in] v : variable description
       * \param[in] e : external name
       */
      StaticVariableProvider(const mfront::StaticVariableDescription&,
                             const std::string&);
      //! \return the type of the variable that can be produced
      std::string getVariableType() const override final;
      //! \return the external name of the variable that can be produced
      std::string getExternalName() const override final;
      //! \return the array size of the variable
      unsigned short getArraySize() const override final;
      //! \return the identifier of the provider
      ProviderIdentifier getIdentifier() const override final;
      //! variable type
      const std::string type;
      //! variable name
      const std::string name;
      //! external variable name
      const std::string ename;
      //! destructor
      ~StaticVariableProvider() override;

     private:
      StaticVariableProvider& operator=(StaticVariableProvider&&) = delete;
      StaticVariableProvider& operator=(const StaticVariableProvider&) = delete;
    };  // end of struct StaticVariableProvider

    /*!
     * Provider standing for a parameter variable
     */
    struct MFRONT_VISIBILITY_EXPORT ParameterProvider : public ProviderBase {
      /*!
       * constructor
       * \param[in] t : variable type
       * \param[in] n : variable name
       * \param[in] e : external name
       */
      ParameterProvider(const std::string&,
                        const std::string&,
                        const std::string&);
      /*!
       * constructor
       * \param[in] v : variable description
       * \param[in] e : external name
       */
      ParameterProvider(const mfront::VariableDescription&, const std::string&);
      //! \return the type of the variable that can be produced
      std::string getVariableType() const override final;
      //! \return the external name of the variable that can be produced
      std::string getExternalName() const override final;
      //! \return the array size of the variable
      unsigned short getArraySize() const override final;
      //! \return the identifier of the provider
      ProviderIdentifier getIdentifier() const override final;
      //! variable type
      const std::string type;
      //! variable name
      const std::string name;
      //! external variable name
      const std::string ename;
      //! destructor
      ~ParameterProvider() override;

     private:
      ParameterProvider& operator=(ParameterProvider&&) = delete;
      ParameterProvider& operator=(const ParameterProvider&) = delete;
    };  // end of struct ParameterProvider

    // /*!
    //  * Provider standing for a material law
    //  */
    // struct MFRONT_VISIBILITY_EXPORT MaterialLawProvider
    //   : public ProviderBase
    // {
    //   //! constructor
    //   MaterialLawProvider();
    //   //! \return the type of the variable that can be produced
    //   virtual std::string getVariableType() const override final;
    //   //! \return the external name of the variable that can be produced
    //   virtual std::string getExternalName() const override final;
    //   //! \return the array size of the variable
    //   virtual unsigned short getArraySize() const override final;
    //   //! \return the identifier of the provider
    //   virtual ProviderIdentifier getIdentifier() const override final;
    //   //! destructor
    //   virtual ~MaterialLawProvider();
    // }; // end of struct MaterialLawProvider

  }  // end of namespace bbrick

}  // end of namespace mfront

#endif /* LIB_MFRONT_BEHAVIOURBRICK_REQUIREMENTPROVIDER_HXX */
