/*!
 * \file   PipeElementBase.hxx
 * \brief
 * \author Thomas Helfer
 * \date   10 déc. 2015
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MTEST_PIPEELEMENTBASE_HXX
#define LIB_MTEST_PIPEELEMENTBASE_HXX

#include "MTest/PipeElement.hxx"

namespace mtest {

  // forward declaration
  struct Behaviour;
  // forward declaration
  struct PipeMesh;

  /*!
   * \brief structure describing a Hybrid High Order element for pipes
   */
  struct PipeElementBase : PipeElement {
    /*!
     * \brief constructor
     * \param[in] m: mesh
     * \param[in] b: behaviour
     * \param[in] n: element number
     */
    PipeElementBase(const PipeMesh&, const Behaviour&, const size_type);
    //! destructor
    virtual ~PipeElementBase() noexcept;

   protected:
    //! inner radius
    const real inner_radius;
    //! outer radius
    const real outer_radius;
    //! \brief behaviour
    const Behaviour& behaviour;
    //! index of the element
    const size_t index;
  };  // end of struct PipeElementBase

}  // end of namespace mtest

#endif /* LIB_MTEST_PIPEELEMENTBASE_HXX */
