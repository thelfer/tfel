/*!
 * \file   mfront/include/MFront/StaticVariableDescription.hxx
 * \brief  This file declares the StaticVariableDescription class
 * \author Thomas Helfer
 * \date   17 Jan 2007
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_STATICVARIABLEDESCRIPTION_HXX
#define LIB_MFRONT_STATICVARIABLEDESCRIPTION_HXX

#include <string>
#include <vector>

#include "MFront/MFrontConfig.hxx"
#include "MFront/VariableDescriptionBase.hxx"

namespace mfront {

  //! \brief class representing a static variable
  struct MFRONT_VISIBILITY_EXPORT StaticVariableDescription
      : public VariableDescriptionBase {
#if defined _WIN32 || defined _WIN64 || defined __CYGWIN__
    typedef double StaticVariableValueType;
#else
    typedef long double StaticVariableValueType;
#endif /* LIB_MFRONT_STATICVARIABLEDESCRIPTION_HXX */
    /*!
     * Constructor
     * \param[in] type_       : variable type
     * \param[in] name_       : variable name
     * \param[in] lineNumber_ : line number
     * \param[in] value_      : value
     */
    StaticVariableDescription(const std::string&,
                              const std::string&,
                              const unsigned int,
                              const StaticVariableValueType);
    /*!
     * Constructor
     * \param[in] type_          : variable type
     * \param[in] symbolic_name_ : symbolic variable name
     * \param[in] name_          : variable name
     * \param[in] lineNumber_    : line number
     * \param[in] value_         : value
     */
    StaticVariableDescription(const std::string&,
                              const std::string&,
                              const std::string&,
                              const unsigned int,
                              const StaticVariableValueType);
    //! \brief move constructor
    StaticVariableDescription(StaticVariableDescription&&);
    //! \brief copy constructor
    StaticVariableDescription(const StaticVariableDescription&);
    //! \brief move assignement
    StaticVariableDescription& operator=(StaticVariableDescription&&);
    //! \brief standard assignement
    StaticVariableDescription& operator=(const StaticVariableDescription&);
    //! \brief destructor
    ~StaticVariableDescription();
    //! value of the static variable
    StaticVariableValueType value;
  };  // end of struct MFrontDSLBase<Child>::StaticVariableDescription

  //! a simple alias for backward compatibility
  typedef StaticVariableDescription StaticVarHandler;

  //! a simple wrapper around the std::vector class
  struct StaticVariableDescriptionContainer
      : private std::vector<StaticVariableDescription> {
    using std::vector<StaticVariableDescription>::value_type;
    using std::vector<StaticVariableDescription>::reference;
    using std::vector<StaticVariableDescription>::const_reference;
    using std::vector<StaticVariableDescription>::size_type;
    using std::vector<StaticVariableDescription>::iterator;
    using std::vector<StaticVariableDescription>::const_iterator;
    using std::vector<StaticVariableDescription>::push_back;
    using std::vector<StaticVariableDescription>::begin;
    using std::vector<StaticVariableDescription>::end;
    using std::vector<StaticVariableDescription>::empty;
    using std::vector<StaticVariableDescription>::front;
    using std::vector<StaticVariableDescription>::back;
    using std::vector<StaticVariableDescription>::size;
    using std::vector<StaticVariableDescription>::operator[];
    /*!
     * \param[in] n : variable name
     * \return true if a variable with the given name exists
     */
    bool contains(const std::string&) const;
    /*!
     * \param[in] n : variable name
     * \return the variable description with the given name
     */
    const StaticVariableDescription& get(const std::string&) const;
  };

  //! a simple alias for backward compatibility
  typedef StaticVariableDescriptionContainer StaticVarContainer;

}  // end of namespace mfront

#endif /* LIB_MFRONT_STATICVARIABLEDESCRIPTION_HXX */
