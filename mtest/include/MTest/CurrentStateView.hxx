/*!
 * \file   mtest/include/MTest/CurrentStateView.hxx
 * \brief
 * \author Thomas Helfer
 * \date   21/03/2020
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MTEST_CURRENTSTATEVIEW_HXX
#define LIB_MTEST_CURRENTSTATEVIEW_HXX

#include <map>
#include <string>
#include <vector>
#include <type_traits>

#include "TFEL/Utilities/Span.hxx"
#include "TFEL/Math/tmatrix.hxx"
#include "MTest/Config.hxx"
#include "MTest/Types.hxx"
#include "MTest/PackagingInformation.hxx"

namespace mtest {

  // forward declaration
  struct CurrentState;

  /*!
   * \brief structure containing a view of the state of the material.
   */
  struct MTEST_VISIBILITY_EXPORT CurrentStateView {
    //! \brief default constructor
    CurrentStateView(CurrentState&) noexcept;
    //! \brief copy constructor
    CurrentStateView(const CurrentStateView&) noexcept;
    //! \partial copy constructor
    CurrentStateView(const CurrentStateView& src,
                     std::vector<real>& s1,
                     std::vector<real>& iv1,
                     real& se1,
                     real& de1) noexcept;
    //! \brief move constructor
    CurrentStateView(CurrentStateView&&) noexcept;
    //! \brief assignement
    CurrentStateView& operator=(const CurrentStateView&) noexcept;
    //! \brief move assignement
    CurrentStateView& operator=(CurrentStateView&&) noexcept;
    //! \brief destructor
    ~CurrentStateView() noexcept;
    //! \brief thermodynamic forces at the beginning of the time step
    tfel::utilities::ConstSpan<real> s0;
    //! \brief thermodynamic forces at the end of the time step
    tfel::utilities::Span<real> s1;
    //! \brief driving variables at the beginning of the time step
    tfel::utilities::ConstSpan<real> e0;
    //! \brief driving variables at the end of the time step
    tfel::utilities::ConstSpan<real> e1;
    //! \brief thermal strain at the beginning of the time step
    tfel::utilities::ConstSpan<real> e_th0;
    //! \brief thermal strain at the end of the time step
    tfel::utilities::ConstSpan<real> e_th1;
    //! \brief material properties at the end of the time step
    tfel::utilities::ConstSpan<real> mprops1;
    //! \brief internal variables at the beginning of the time step
    tfel::utilities::ConstSpan<real> iv0;
    //! \brief internal variables at the end of the time step
    tfel::utilities::Span<real> iv1;
    //! \brief external variables at the beginning of the time step
    tfel::utilities::ConstSpan<real> esv0;
    //! \brief external variables increments
    tfel::utilities::ConstSpan<real> desv;
    //! \brief stored energy at the beginning of the time step
    const real& se0;
    //! \brief stored energy at the end of the time step
    real& se1;
    //! \brief dissipated energy at the beginning of the time step
    const real& de0;
    //! \brief dissipated energy at the end of the time step
    real& de1;
    //! \brief rotation matrix
    const tfel::math::tmatrix<3u, 3u>& r;
    //! \brief information collected during the packaging stage
    std::map<std::string, PackagingInformation>& packaging_info;
  };  // end of struct CurrentStateView

  /*!
   * \brief An helper structure which is used to turned a constant
   * CurrentStateView into a mutable one.
   */
  struct MutableCurrentStateViewCopy {
    //! \brief default constructor
    MutableCurrentStateViewCopy() noexcept;
    //! \brief default constructor
    MutableCurrentStateViewCopy(const CurrentStateView&) noexcept;
    //! \brief move constructor
    MutableCurrentStateViewCopy(MutableCurrentStateViewCopy&&) noexcept;
    //! \brief copy constructor
    MutableCurrentStateViewCopy(const MutableCurrentStateViewCopy&) noexcept;
    //! \brief move assignement
    MutableCurrentStateViewCopy& operator=(
        MutableCurrentStateViewCopy&&) noexcept;
    //! \brief copy assignement
    MutableCurrentStateViewCopy& operator=(
        const MutableCurrentStateViewCopy&) noexcept;
    //! \return a mutable view
    CurrentStateView getView();

   private:
    //! \brief reference to the original state view
    const CurrentStateView& v;
    //! \brief local copy of thermodynamic forces at the end of the time step
    std::vector<real> s1;
    //! \brief local copy of the internal variables at the end of the time step
    std::vector<real> iv1;
    //! \brief local copy of the stored energy at the end of the time step
    real se1;
    //! \brief local copy of the dissipated energy at the end of the time step
    real de1;
  };  // end of struct MutableCurrentStateViewCopy

  /*!
   * \brief a simple utility function
   * \param[in] s: a state
   */
  CurrentStateView makeView(CurrentState&);
  /*!
   * \brief a simple utility function
   * \param[in] s: a state view
   */
  MutableCurrentStateViewCopy makeMutableView(const CurrentStateView&);

}  // end of namespace mtest

#endif /* LIB_MTEST_CURRENTSTATEVIEW_HXX */
