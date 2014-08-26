/*! 
 * \file  GlossaryEntry.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 31 mars 2014
 */

#ifndef _LIB_TFEL_GLOSSARY_GLOSSARYENTRY_H_
#define _LIB_TFEL_GLOSSARY_GLOSSARYENTRY_H_ 

#include<set>
#include<string>
#include<vector>

#include"TFEL/Config/TFELConfig.hxx"

namespace tfel
{

  namespace glossary
  {
  
    /*!
     * structure describing an entry of the glossary
     */
    struct TFELGLOSSARY_VISIBILITY_EXPORT GlossaryEntry
    {
      /*!
       * constructor
       * \param[in] k  : key used to stor 
       * \param[in] n  : name
       * \param[in] u  : unit
       * \param[in] t  : type
       * \param[in] d  : description
       * \param[in] l  : latex description
       * \param[in] no : note
       */
      GlossaryEntry(const std::string&,
		    const std::string&,
		    const std::string&,
		    const std::string&,
		    const std::string&,
		    const std::vector<std::string>&,
		    const std::vector<std::string>&,
		    const std::vector<std::string>&);
      /*!
       * constructor
       * \param[in] k  : key used to stor 
       * \param[in] n  : name
       * \param[in] u  : unit
       * \param[in] t  : type
       * \param[in] d  : description
       * \param[in] l  : latex description
       * \param[in] no : note
       */
      GlossaryEntry(const std::string&,
		    const std::string&,
		    const std::string&,
		    const std::string&,
		    const std::string&,
		    const std::string&,
		    const std::string&,
		    const std::string&);
      /*!
       * \return the key
       */
      const std::string&
      getKey(void) const;
      /*!
       * \return the name
       */
      const std::string&
      getName(void) const;
      /*!
       * \return the unit
       */
      const std::string&
      getUnit(void) const;
      /*!
       * \return the type
       */
      const std::string&
      getType(void) const;
      /*!
       * \return the short description
       */
      const std::string&
      getShortDescription(void) const;
      /*!
       * \return the description
       */
      const std::vector<std::string>&
      getDescription(void) const;
      /*!
       * \return the latex description
       */
      const std::vector<std::string>&
      getLaTeXDescription(void) const;
      /*!
       * \return the notes
       */
      const std::vector<std::string>&
      getNotes(void) const;
      /*!
       * cast operator to string
       */
      operator const std::string& () const;
      /*!
       * comparison operator
       */
      bool operator<(const GlossaryEntry&) const;
    private:
      /*!
       * key used to store this entry in the glossary
       */
      const std::string key;
      /*!
       * name used in input file or return by the convertion to string
       * operator
       */
      const std::string name;
      /*!
       * unit of the entry
       */
      const std::string unit;
      /*!
       * type of the entry
       */
      const std::string type;
      /*!
       * short description of the entry
       */
      const std::string short_description;
      /*!
       * description
       */
      const std::vector<std::string> description;
      /*!
       * latex description
       */
      const std::vector<std::string> latex_description;
      /*!
       * note
       */
      const std::vector<std::string> notes;
    }; // end of struct GlossaryEntry

  } // end of namespace glossary

} // end of namespace tfel

#endif /* _LIB_TFEL_GLOSSARY_GLOSSARYENTRY_H */
