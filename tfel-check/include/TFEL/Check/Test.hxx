/*!
 * \file  tfel-check/include/TFEL/Check/Test.hxx
 * \brief Represents a single test between two columns
 * \date 29 mai 2013
 * \author Remy Petkantchin
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFELCHECK_TEST_HXX
#define LIB_TFELCHECK_TEST_HXX

#include <string>
#include "TFEL/Check/TFELCheckConfig.hxx"
#include "TFEL/Check/Column.hxx"
#include "TFEL/Check/Interpolation.hxx"
#include "TFEL/Check/Comparison.hxx"

namespace tfel {

  namespace check {

    struct TFELCHECK_VISIBILITY_EXPORT Test {
      Test();
      Test(Test&&);
      Test(const Test&);
      Test& operator=(Test&&);
      Test& operator=(const Test&);
      virtual ~Test();

      /*!
       * \brief Does the comparison between the two columns, based on
       * comparison_
       */
      void compare();

      /*!
       * \brief Returns true if the current test succeeded
       */
      bool hasSucceed() const;

      /*!
       * \brief Returns the message to be added in the log
       */
      std::string getMsgLog();

      /*!
       * \brief Returns comparison_
       */
      const std::shared_ptr<Comparison> getComparison() const;

      /*!
       * \brief Sets the value of comparison_
       */
      void setComparison(const std::shared_ptr<Comparison>&);

      /*!
       * \brief Returns true if user allowed less values for the results in the
       * test
       */
      bool isAllowLessResults() const;

      /*!
       * \brief Sets the value of allowLessResults_
       */
      void setAllowLessResults(const bool);

      /*!
       * \brief Sets the first column to compare
       */
      void setColA(const std::shared_ptr<Column>&);

      /*!
       * \brief Sets the second column to compare
       */
      void setColB(const std::shared_ptr<Column>&);

      /*!
       * \brief Sets the column to be interpolated
       */
      void setColInterpolated(const std::shared_ptr<Column>& ci);

      /*!
       * \brief Returns the name of the file containing the references
       */
      const std::string& getFileA() const;

      /*!
       * \brief Sets the name of the file containing the references
       */
      void setFileA(const std::string& f1);

      /*!
       * \brief Returns the name of the file containing the results
       */
      const std::string& getFileB() const;

      /*!
       * \brief Sets the name of the file containing the results
       */
      void setFileB(const std::string& f2);

      /*!
       * \brief Returns the interpolation used for this test
       */
      const std::shared_ptr<Interpolation>& getInterpolation() const;

      /*!
       * \brief Sets the interpolation used for this test
       */
      void setInterpolation(
          const std::shared_ptr<Interpolation>& interpolation);

      /*!
       * \brief Returns the first precision
       */
      double getPrec() const;

      /*!
       * \brief Sets the first precision
       */
      void setPrec(double prec);

      /*!
       * \brief Returns the second precision
       */
      double getPrecision2() const;

      /*!
       * \brief Sets the second precision
       */
      void setPrecision2(double precision2);

      /*!
       * \brief Applies the interpolation
       */
      void applyInterpolation();

      /*!
       * \brief Gets the column to be interpolated for the integration
       */
      const std::shared_ptr<Column>& getColIntegralInterpolated() const;

      /*!
       * \brief Sets the column to be interpolated for the integration
       */
      void setColIntegralInterpolated(
          const std::shared_ptr<Column>& colIntegralInterpolated);

      /*!
       * \brief Gets the interpolation for the integration
       */
      const std::shared_ptr<Interpolation>& getIntegralInterpolation() const;

      /*!
       * \brief Sets the interpolation for the integration
       */
      void setIntegralInterpolation(
          const std::shared_ptr<Interpolation>& integralInterpolation);

      /*!
       * \brief Gets the column of values of the first file
       */
      const std::shared_ptr<Column>& getColA() const;

      /*!
       * \brief Gets the column of values of the second file
       */
      const std::shared_ptr<Column>& getColB() const;

     private:
      double prec = 0;       /**!< the first precision given */
      double precision2 = 0; /**!< the second optional precision given */
      bool allowLessResults =
          false;      /**!< true if user allows to interpolate ref */
      std::string f1; /**!< first file : .ref */
      std::string f2; /**!< second file : .res */
      std::shared_ptr<Column> c1; /**!< column used in first file **/
      std::shared_ptr<Column> c2; /**!< column used in second file **/
      std::shared_ptr<Interpolation>
          interpolation; /**!< the interpolation chosen */
      std::shared_ptr<Column>
          ci; /**!< the column used for interpolation (abscissa) */
      std::shared_ptr<Interpolation>
          integralInterpolation; /**!< the interpolation chosen for the
                                    integration*/
      std::shared_ptr<Column>
          colIntegralInterpolated; /**!< the column used for integration
                                      interpolation (abscissa)*/
      std::shared_ptr<Comparison>
          comparison;     /**!< the type of the test (absolute, relative...) */
      std::string msgLog; /**!< message log */
    };
    // end of struct Test

  }  // end of namespace check

}  // end of namespace tfel

#endif /* LIB_TFELCHECK_TEST_HXX */
