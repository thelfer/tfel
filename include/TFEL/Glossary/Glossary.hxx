/*!
 *\file   Glossary.hxx
 *\author Helfer Thomas
 *\date   09/06/14
 */

#ifndef __TFEL_GLOSSARY_GLOSSARY_HH__
#define __TFEL_GLOSSARY_GLOSSARY_HH__

#include"TFEL/Config/TFELConfig.hxx"
#include<set>
#include<string>

#include"TFEL/Glossary/GlossaryEntry.hxx"

namespace tfel
{

  namespace glossary
  {

    /*!
     * \brief Structure in charge of handling the TFEL Glossary
     */
    struct TFELGLOSSARY_VISIBILITY_EXPORT Glossary
    {

      /*!
       * \return the uniq instance of this class (singleton pattern)
       */
      static Glossary&
      getGlossary();

      /*!
       * \return true if the glossary contains the given name
       * \param[in] n : name
       */
      bool
      contains(const std::string&) const;

      static const GlossaryEntry IrradiationTemperature;
      static const GlossaryEntry Temperature;
      static const GlossaryEntry KelvinTemperature;

    private :

      /*!
       * \brief insert a new entry
       */
      void
      insert(const GlossaryEntry&);

      Glossary();

      Glossary(const Glossary&);

      Glossary&
	operator=(const Glossary&);

      std::set<GlossaryEntry> entries;

    }; // end of struct Glossary

  } // end of namespace glossary

} // end of namespace tfel

#endif /* __TFEL_GLOSSARY_GLOSSARY_HH__ */
