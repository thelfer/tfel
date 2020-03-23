/*!
 * \file   mtest/src/CurrentStateView.cxx
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

#include "MTest/CurrentState.hxx"
#include "MTest/CurrentStateView.hxx"

namespace mtest {

  CurrentStateView::CurrentStateView(CurrentState& src) noexcept
      : s0(src.s0),
        s1(src.s1),
        e0(src.e0),
        e1(src.e1),
        e_th0(src.e_th0),
        e_th1(src.e_th1),
        mprops1(src.mprops1),
        iv0(src.iv0),
        iv1(src.iv1),
        esv0(src.esv0),
        desv(src.desv),
        se0(src.se0),
        se1(src.se1),
        de0(src.de0),
        de1(src.de1),
        r(src.r),
        packaging_info(src.packaging_info) {
  }  // end of CurrentStateView::CurrentStateView

  CurrentStateView::CurrentStateView(const CurrentStateView&) noexcept = default;

  CurrentStateView::CurrentStateView(const CurrentStateView& src,
                                     std::vector<real>& ns1,
                                     std::vector<real>& niv1,
                                     real& nse1,
                                     real& nde1) noexcept
      : s0(src.s0),
        s1(ns1),
        e0(src.e0),
        e1(src.e1),
        e_th0(src.e_th0),
        e_th1(src.e_th1),
        mprops1(src.mprops1),
        iv0(src.iv0),
        iv1(niv1),
        esv0(src.esv0),
        desv(src.desv),
        se0(src.se0),
        se1(nse1),
        de0(src.de0),
        de1(nde1),
        r(src.r),
        packaging_info(src.packaging_info) {
  }  // end of CurrentStateView::CurrentStateView

  CurrentStateView::CurrentStateView(CurrentStateView&&) noexcept = default;

  CurrentStateView::~CurrentStateView() noexcept = default;

  MutableCurrentStateViewCopy::MutableCurrentStateViewCopy(
      const CurrentStateView& src) noexcept
      : v(src),
        s1(src.s1.begin(), src.s1.end()),
        iv1(src.iv1.begin(), src.iv1.end()),
        se1(src.se1),
        de1(src.de1) {
  }  // end of MutableCurrentStateViewCopy::MutableCurrentStateViewCopy

  MutableCurrentStateViewCopy::MutableCurrentStateViewCopy(
      MutableCurrentStateViewCopy&&) noexcept = default;

  CurrentStateView MutableCurrentStateViewCopy::getView() {
    return CurrentStateView(this->v, this->s1, this->iv1, this->se1, this->de1);
  }  // end of MutableCurrentStateViewCopy::getView

  CurrentStateView makeView(CurrentState& s) {
    return CurrentStateView(s);
  }  // end of makeView

  MutableCurrentStateViewCopy makeMutableView(const CurrentStateView& s){
    return MutableCurrentStateViewCopy(s);
  } // end of makeMutableView

}  // end of namespace mtest