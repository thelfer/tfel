/*!
 * \file Column.hxx
 *
 *  \date 23 mai 2013
 *  \author Remy Petkantchin
 *
 *  \class Column
 *  \brief Treats the columns in files
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFELCHECK_COLUMN_HXX
#define LIB_TFELCHECK_COLUMN_HXX

#include <string>
#include <vector>
#include <memory>

#include "TFEL/Check/TFELCheckConfig.hxx"
#include "TFEL/Utilities/CxxTokenizer.hxx"
#include "TFEL/Utilities/TextData.hxx"
#include "TFEL/Check/PCLogger.hxx"

namespace tfel {

  namespace check {

    struct TFELCHECK_VISIBILITY_EXPORT Column {
      Column(int);
      Column(std::string);
      Column(Column&&);
      Column(const Column&);
      Column& operator=(Column&&);
      Column& operator=(const Column&);
      /*!
       * \brief returns the name of the column used in file. If name
       *        hasn't been given, returns number instead
       */
      std::string getName() const;
      /*!
       * \brief returns the column values
       */
      const std::vector<double>& getValues();
      /*!
       * \brief clears the values vector
       */
      void clearValues();
      /*!
       * \brief resize the values vector
       */
      void resizeValues(std::vector<double>::size_type size);
      /*!
       * \brief set the value at position i in the values vector
       */
      void setValue(unsigned pos, double value);
      /*!
       * \brief returns the filename
       */
      void setFilename(std::string);
      /*!
       * \brief returns the filename
       */
      const std::string& getFilename() const;
      /*!
       * \brief returns the TextData used to create column
       */
      const std::shared_ptr<tfel::utilities::TextData> getData() const;
      //! destructor
      virtual ~Column();

     private:
      //! the data from the column
      std::shared_ptr<tfel::utilities::TextData> data;
      //! the values contained in the column
      std::vector<double> values;
      //! the name of the file which the column was built with
      std::string f;
      /*!
       * the name of the column in the file. It's
       * he number if the name wasn't given at
       * construction
       */
      std::string name;
      //! column number
      unsigned short num;
      //!
      bool byName;
    };

  }  // end of namespace check

}  // end of namespace tfel

#endif /* LIB_TFELCHECK_COLUMN_HXX */
