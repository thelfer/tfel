/*!
 * \file   mfront/include/MFront/VariableDescription.hxx
 * \brief  This file declares the VariableDescription class
 * \author Helfer Thomas
 * \date   17 Jan 2007
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_MFRONT_VARDESCRIPTION_H_
#define _LIB_MFRONT_VARDESCRIPTION_H_ 

#include<map>
#include<vector>
#include<string>

#include"TFEL/Config/TFELConfig.hxx"

namespace mfront
{

  /*!
   * structure standing for a variable
   * The support of fixed-sized arrays has been added lately.
   * This variable is considered as an array if arraySize is greater than 1.
   */
  struct TFEL_VISIBILITY_EXPORT VariableDescription
  {
    VariableDescription(const VariableDescription&) = default;
    VariableDescription(VariableDescription&&) = default;
    VariableDescription&
    operator=(VariableDescription&&) = default;
    VariableDescription&
    operator=(const VariableDescription&) = default;
    /*!
     * Constructor
     * \param[in] t : variable type
     * \param[in] n : variable name
     * \param[in] s : if greater than 1, this variable will
     * be considered as an array
     * \param[in] l : line number
     */
    VariableDescription(const std::string&,
			const std::string&,
			const unsigned short,
			const unsigned int);
    /*!
     * \return the name used to call the variable from outside
     * (pleiades application, mtest, etc..)
     * \param[in] glossaryNames
     * \param[in] entryNames
     */
    const std::string&
    getExternalName(const std::map<std::string,std::string>&,
		    const std::map<std::string,std::string>&) const;
    //! destructor
    ~VariableDescription();
    //! type of the variable
    //  If the variable has been declared as an array (see below),
    //  this field holds the type contained by the array.
    std::string type;
    //! name of the variable
    std::string name;
    //! description
    std::string description;
    //! if the variable has been declared as an array, this field
    //  contains a value greater than 1
    unsigned short arraySize;
    //! line at wich the variable has been declared
    unsigned int lineNumber;
  }; // end of struct VariableDescription

  //! a simple alias for backward compatibility
  typedef VariableDescription  VarHandler;

  //! a simple wrapper around the std::vector class
  struct VariableDescriptionContainer
    : private std::vector<VariableDescription>
  {
    VariableDescriptionContainer() = default;
    VariableDescriptionContainer(VariableDescriptionContainer&&) = default;
    VariableDescriptionContainer(const VariableDescriptionContainer&) = default;
    VariableDescriptionContainer& operator=(VariableDescriptionContainer&&) = default;
    VariableDescriptionContainer& operator=(const VariableDescriptionContainer&) = default;
    // resusing method from std::vector
    using std::vector<VariableDescription>::size_type;
    using std::vector<VariableDescription>::value_type;
    using std::vector<VariableDescription>::reference;
    using std::vector<VariableDescription>::const_reference;
    using std::vector<VariableDescription>::iterator;
    using std::vector<VariableDescription>::const_iterator;
    using std::vector<VariableDescription>::push_back;
    using std::vector<VariableDescription>::begin;
    using std::vector<VariableDescription>::end;
    using std::vector<VariableDescription>::empty;
    using std::vector<VariableDescription>::insert;
    using std::vector<VariableDescription>::front;
    using std::vector<VariableDescription>::back;
    using std::vector<VariableDescription>::clear;
    using std::vector<VariableDescription>::size;
    using std::vector<VariableDescription>::operator[];
    /*!
     * \param[in] n : variable name
     * \return true if a variable with the given name exists
     */
    bool contains(const std::string&) const;
    /*!
     * \return the list of external names associated with this
     * container.
     * \see VariableDescription::getExternalName
     * \param[in] glossaryNames : glossary names
     * \param[in] entryNames    : entry names
     */
    std::vector<std::string>
    getExternalNames(const std::map<std::string,std::string>&,
		     const std::map<std::string,std::string>&) const;
    /*!
     * get the external names associated with this container.
     * \see VariableDescription::getExternalName
     * \param[out] names         : glossary names of the function
     * \param[in]  glossaryNames : glossary names
     * \param[in]  entryNames    : entry names
     */
    void
    getExternalNames(std::vector<std::string>&,
		     const std::map<std::string,std::string>&,
		     const std::map<std::string,std::string>&) const;
    /*!
     * append the list of external names associated with this
     * container at the end of the given list.
     * \see VariableDescription::getExternalName
     * \param[in] names         : list of names
     * \param[in] glossaryNames : glossary names
     * \param[in] entryNames    : entry names
     */
    void
    appendExternalNames(std::vector<std::string>&,
			const std::map<std::string,std::string>&,
			const std::map<std::string,std::string>&) const;
    /*!
     * \return the variable with the given name
     * \param[in] n : name
     */
    const VariableDescription&
    getVariable(const std::string&);
    //! destrocut
    ~VariableDescriptionContainer();
  }; // end of struct VariableDescription

  //! a simple alias for backward compatibility
  typedef VariableDescriptionContainer      VarContainer;  

} // end of namespace mfront

#endif /* _LIB_MFRONT_VARDESCRIPTION_H */

