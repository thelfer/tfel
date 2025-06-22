/*!
 * \file  mfront/include/MFront/DataInterpolationUtilities.hxx
 * \brief This file declares a set of classes and methods useful
 * to handle data interpolation.
 * \author Thomas Helfer
 * \brief 17/06/2024
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <map>
#include <string>
#include "TFEL/Utilities/Data.hxx"
#include "MFront/MFrontConfig.hxx"

namespace mfront {

  /*!
   * \brief a structure representing the interpolation of data
   * of a single variable
   */
  struct MFRONT_VISIBILITY_EXPORT SingleVariableInterpolatedData {
    //! \brief arguments passed to the writeLinearInterpolationValues function
    struct WriteLinearInterpolationValuesArguments {
      std::string abscissae_name;
      std::string abscissae_type;
      std::string ordinates_name;
      std::string ordinates_type;
    };
    //! \brief arguments passed to the writeCollocationPoints function
    struct WriteCollocationPointsArguments {
      std::string collocation_points_name;
      std::string abscissae_type;
      std::string ordinates_type;
      std::string ordinates_derivatives_type;
    };
    /*!
     * \return an object built from the given data map
     * \param[in] d: data map
     */
    static SingleVariableInterpolatedData extract(
        const tfel::utilities::DataMap&);
    /*!
     * \brief interpolation algorithm
     */
    enum InterpolationType {
      LINEAR_INTERPOLATION,
      CUBIC_SPLINE_INTERPOLATION
    } itype = LINEAR_INTERPOLATION;
    /*!
     * \brief extrapolation type
     *
     * If true, extrapolation is allowed and based on the interpolation
     * selected. If false, no extrapolation is performed and the value is kept
     * constant, egal to the value of the closest point.
     */
    bool etype = true;
    /*!
     * \brief the values of the yield radius as a function of the equivalent
     * plastic strain.
     */
    std::map<double, double> values;
  };

  /*!
   * \brief write the declarations of two constexpr arrays containing the
   * abscissaes and the ordinates of interpolation data
   * \param[in] idata: interpolated data
   * \param[in] args: additional information
   */
  MFRONT_VISIBILITY_EXPORT std::string writeLinearInterpolationValues(
      const SingleVariableInterpolatedData&,
      const SingleVariableInterpolatedData::
          WriteLinearInterpolationValuesArguments&);
  /*!
   * \brief write the collocations points computed from
   * abscissaes and the ordinates of interpolation data
   * \param[in] idata: interpolated data
   * \param[in] args: additional information
   */
  MFRONT_VISIBILITY_EXPORT std::string writeCollocationPoints(
      const SingleVariableInterpolatedData&,
      const SingleVariableInterpolatedData::WriteCollocationPointsArguments&);

}  // end of namespace mfront