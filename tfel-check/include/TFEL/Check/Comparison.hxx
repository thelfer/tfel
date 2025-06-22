/*!
 * \file Comparison.hxx
 *
 *  \date 27 mai 2013
 *  \author Remy Petkantchin
 *
 *  \class Comparison
 *  \brief Abstract mother class for Test comparison criterias. Does the
 * comparison for two given columns. \copyright Copyright (C) 2006-2018 CEA/DEN,
 * EDF R&D. All rights reserved. This project is publicly released under either
 * the GNU GPL Licence with linking exception or the CECILL-A licence. A copy of
 * thoses licences are delivered with the sources of TFEL. CEA or EDF may also
 * distribute this project under specific licensing conditions.
 */

#ifndef LIB_TFELCHECK_COMPARISON_HXX
#define LIB_TFELCHECK_COMPARISON_HXX

#include <cmath>
#include <string>

#include "TFEL/Check/TFELCheckConfig.hxx"
#include "TFEL/Check/Column.hxx"
#include "TFEL/Check/Interpolation.hxx"

namespace tfel {

  namespace check {

    struct TFELCHECK_VISIBILITY_EXPORT Comparison {
      Comparison();
      Comparison(const std::shared_ptr<Column> c1,
                 const std::shared_ptr<Column> c2,
                 const double prec,
                 const double precision2,
                 const std::shared_ptr<Column> ci,
                 const std::string interpolationType,
                 const bool interpolationIsConform,
                 const std::shared_ptr<Column> colIntegralInterpolated,
                 const std::shared_ptr<Interpolation> integralInterpolation);
      /*!
       * \brief returns true if Comparison has succeed
       */
      bool hasSucceed() const;
      /*!
       * \brief returns the log message with comparison info
       */
      std::string getMsgLog() const;
      /*!
       * \brief executes the comparison between the values
       */
      virtual void compare() = 0;
      /*!
       * \brief sets the parameters for the comparison
       */
      void setParameters(
          const std::shared_ptr<Column> c1,
          const std::shared_ptr<Column> c2,
          const double prec,
          const double precision2,
          const std::shared_ptr<Column> ci,
          const std::string interpolationType,
          const bool interpolationIsConform,
          const std::shared_ptr<Column> colIntegralInterpolated,
          const std::shared_ptr<Interpolation> integralInterpolation);

      const std::string& getName() const;
      //! destructor
      virtual ~Comparison();

     protected:
      bool success; /**!< current Comparison success indicator */
      std::string msgLog;
      std::shared_ptr<Column>
          c1; /**!< the first column on which the comparison will be made */
      std::shared_ptr<Column>
          c2; /**!< the second column on which the comparison will be made */
      double prec; /**!< the precision of the Comparison */
      double
          precision2; /**!< the optional second precision of the Comparison */
      std::shared_ptr<Column> ci; /**!< the column that has been interpolated */
      std::string
          interpolationType;       /**!< the interpolation that has been done */
      bool interpolationIsConform; /**!< true if the interpolation isConform */
      std::string name;            /**!< the name of the comparison */
      /**!< only for AreaComparison */
      std::shared_ptr<Column>
          colIntegralInterpolated; /**!< the column used for integration
                                      interpolation (abscissa)*/
      std::shared_ptr<Interpolation>
          integralInterpolation; /**!< the interpolation chosen for the
                                    integration*/
    };

  }  // end of namespace check

}  // end of namespace tfel

#endif /* LIB_TFELCHECK_COMPARISON_HXX */
