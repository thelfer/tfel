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

#include <vector>
#include <type_traits>

#include "TFEL/Math/tmatrix.hxx"
#include "MTest/Config.hxx"
#include "MTest/Types.hxx"

namespace mtest {

  // forward declaration
  struct CurrentState;

  namespace internals {

    /*!
     * \brief a simple implementation of `std::span` (not available in C++-11)
     */
    template <typename T, bool const_view = false>
    struct VectorView {
      //! a simple alias
      using vector_type = typename std::conditional<const_view,
                                                    const std::vector<T>,
                                                    std::vector<T>>::type;
      //! a simple alias
      using value_type =
          typename std::conditional<const_view, const T, T>::type;
      //! a simple alias
      using size_type = typename vector_type::size_type;
      //! a simple alias
      using iterator_type = value_type*;
      //! a simple alias
      using const_iterator_type = const value_type*;
      //! \brief constructor
      VectorView(vector_type&) noexcept;
      //! \brief move constructor
      VectorView(VectorView&&) noexcept;
      //! \brief copy constructor
      VectorView(const VectorView&) noexcept;
      /*!
       * \return the pointer to the first element or nullptr is the
       * sequence is empty
       */
      value_type* begin() noexcept;
      /*!
       * \return a pointer after to last element or nullptr is the
       * sequence is empty
       */
      value_type* end() noexcept;
      //! \return the size of the buffer
      size_type size() const noexcept;
      //! \return true if the buffer has zero size
      bool empty() const noexcept;
      /*!
       * \return the ith element
       * \param[in] i: rank
       */
      value_type& operator[](const size_type) noexcept;
      /*!
       * \return the ith element
       * \param[in] i: rank
       */
      value_type& operator()(const size_type) noexcept;

     private:
      //! pointer to the beginning of the buffer
      value_type* const b;
      //! size of the memory buffer
      const size_type s;
    }; // end of VectorView

    //! \brief a simple alias
    template <typename T>
    using ConstVectorView = VectorView<T, true>;

  };  // end of namespace internals

  /*!
   * \brief structure containing a view of the state of the material.
   */
  struct MTEST_VISIBILITY_EXPORT CurrentStateView {
    //! \brief default constructor
    CurrentStateView(CurrentState&);
    //! \brief copy constructor
    CurrentStateView(const CurrentStateView&);
    //! \brief move constructor
    CurrentStateView(CurrentStateView&&);
    //! \brief assignement
    CurrentStateView& operator=(const CurrentStateView&);
    //! \brief move assignement
    CurrentStateView& operator=(CurrentStateView&&);
    //! \brief destructor
    ~CurrentStateView() noexcept;
    //! \brief thermodynamic forces at the beginning of the time step
    mtest::internals::ConstVectorView<real> s0;
    //! \brief thermodynamic forces at the end of the time step
    mtest::internals::VectorView<real> s1;
    //! \brief driving variables at the beginning of the time step
    mtest::internals::ConstVectorView<real> e0;
    //! \brief driving variables at the end of the time step
    mtest::internals::VectorView<real> e1;
    //! \brief thermal strain at the beginning of the time step
    mtest::internals::ConstVectorView<real> e_th0;
    //! \brief thermal strain at the end of the time step
    mtest::internals::VectorView<real> e_th1;
    //! \brief material properties at the end of the time step
    mtest::internals::ConstVectorView<real> mprops1;
    //! \brief internal variables at the beginning of the time step
    mtest::internals::ConstVectorView<real> iv0;
    //! \brief internal variables at the end of the time step
    mtest::internals::VectorView<real> iv1;
    //! \brief external variables at the beginning of the time step
    mtest::internals::ConstVectorView<real> esv0;
    //! \brief external variables increments
    mtest::internals::ConstVectorView<real> desv;
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
  };  // end of struct CurrentStateView

}  // end of namespace mtest

#include "MTest/CurrentStateView.ixx"

#endif /* LIB_MTEST_CURRENTSTATEVIEW_HXX */
