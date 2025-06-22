/*!
 * \file   include/TFEL/FSAlgorithm/copy.hxx
 * \brief  this file implements the copy class.
 * \author Helfer Thomas
 * \date   30 Jun 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_COPY_HXX_
#define LIB_TFEL_COPY_HXX_

#include "TFEL/Config/TFELConfig.hxx"
#include <type_traits>
#include "TFEL/TypeTraits/IsRandomAccessIterator.hxx"

namespace tfel {

  namespace fsalgo {

    /*!
     * \class copy
     * \brief copy copies N elements of a container to another.
     * \param N number of element to be copied.
     *
     * This documentation is mostly inspired from:
     * http://www.sgi.com/tech/stl/copy.html
     *
     * \author Helfer Thomas
     * \date   30 Jun 2006
     */
    template <unsigned int N>
    struct copy {
      /*!
       * copy the element pointed to by p to the element pointed to by q and the
       * call copy<N-1>::exe. Since calls are inlined, this recursion has no
       * cost at runtime.
       *
       * \param InputIterator iterator to the element to be copied
       * \param OutputIterator iterator to the place where this element will be
       * copied
       *
       * \return OutputIterator iterator after the last element copied
       *
       * \pre
       * - InputIterator is a model of Input Iterator.
       * - OutputIterator is a model of Output Iterator.
       * - InputIterator's value type is convertible to a type in
       * OutputIterator's set of value types.
       *
       * \author Helfer Thomas
       * \date   30 Jun 2006
       */
      template <typename InputIterator, typename OutputIterator>
      static TFEL_FSALGORITHM_INLINE OutputIterator exe(InputIterator p,
                                                        OutputIterator q) {
        *q = *p;
        return copy<N - 1>::exe(++p, ++q);
      }
    };

    /*!
     * \brief partial specialisation of struct copy to end recursion.
     *
     * \author Helfer Thomas
     * \date   30 Jun 2006
     */
    template <>
    struct copy<0u> {
      /*!
       * \return the ouptut iterator to the end the recursion
       * \sa copy<N>::exe()
       *
       * \author Helfer Thomas
       * \date   30 Jun 2006
       */
      template <typename InputIterator, typename OutputIterator>
      static TFEL_FSALGORITHM_INLINE OutputIterator exe(InputIterator,
                                                        OutputIterator q) {
        return q;
      }
    };

#ifndef NO_FSALGORITHM_SPECIALISATION

    /*!
     * \brief partial specialisation of struct copy to end recursion.
     *
     * This is disabled if NO_FSALGORITHM_SPECIALISATION is defined
     * \see NO_FSALGORITHM_SPECIALISATION
     *
     * \author Helfer Thomas
     * \date   30 Jun 2006
     */
    template <>
    struct copy<1u> {
      /*!
       * \return the ouptut iterator to the end the recursion
       * \sa copy<N>::exe()
       *
       * \author Helfer Thomas
       * \date   30 Jun 2006
       */
      template <typename InputIterator, typename OutputIterator>
      static TFEL_FSALGORITHM_INLINE OutputIterator exe(InputIterator p,
                                                        OutputIterator q) {
        *q = *p;
        return ++q;
      }
    };

    /*!
     * \brief partial specialisation of struct copy to end recursion.
     *
     * This is disabled if NO_FSALGORITHM_SPECIALISATION is defined
     * \see NO_FSALGORITHM_SPECIALISATION
     *
     * \author Helfer Thomas
     * \date   30 Jun 2006
     */
    template <>
    struct copy<2u> {
      /*!
       * This version of copy is used if iterators are Random Access. Test is
       * based on the tfel::typetraits::IsRandomAccessIterator class.
       *
       * \param InputIterator iterator to the element to be copied
       * \param OutputIterator iterator to the place where this element will be
       * copied
       *
       * \return OutputIterator iterator after the last element copied
       *
       * \pre
       * - InputIterator is a model of Random Access Iterator.
       * - OutputIterator is a model of Random Access Iterator.
       * - InputIterator's value type is convertible to a type in
       * OutputIterator's set of value types.
       *
       * \author Helfer Thomas
       * \date   30 Jun 2006
       */
      template <typename InputIterator, typename OutputIterator>
      static TFEL_FSALGORITHM_INLINE typename std::enable_if<
          tfel::typetraits::IsRandomAccessIterator<InputIterator>::cond &&
              tfel::typetraits::IsRandomAccessIterator<OutputIterator>::cond,
          OutputIterator>::type
      exe(InputIterator p, OutputIterator q) {
        q[0] = p[0];
        q[1] = p[1];
        return q + 2;
      }

      /*!
       * General version
       * \see copy<N>::exe for details
       */
      template <typename InputIterator, typename OutputIterator>
      static TFEL_FSALGORITHM_INLINE typename std::enable_if<
          !(tfel::typetraits::IsRandomAccessIterator<InputIterator>::cond &&
            tfel::typetraits::IsRandomAccessIterator<OutputIterator>::cond),
          OutputIterator>::type
      exe(InputIterator p, OutputIterator q) {
        *q = *p;
        return copy<1u>::exe(++p, ++q);
      }
    };

    /*!
     * \brief partial specialisation of struct copy to end recursion.
     *
     * This is disabled if NO_FSALGORITHM_SPECIALISATION is defined
     * \see NO_FSALGORITHM_SPECIALISATION
     *
     * \author Helfer Thomas
     * \date   30 Jun 2006
     */
    template <>
    struct copy<3u> {
      /*!
       * This version of copy is used if iterators are Random Access. Test is
       * based on the tfel::typetraits::IsRandomAccessIterator class. \see
       * std::enable_if, tfel::typetraits::IsRandomAccessIterator
       *
       * \param InputIterator iterator to the element to be copied
       * \param OutputIterator iterator to the place where this element will be
       * copied
       *
       * \return OutputIterator iterator after the last element copied
       *
       * \pre
       * - InputIterator is a model of Random Access Iterator.
       * - OutputIterator is a model of Random Access Iterator.
       * - InputIterator's value type is convertible to a type in
       * OutputIterator's set of value types.
       *
       * \author Helfer Thomas
       * \date   30 Jun 2006
       */
      template <typename InputIterator, typename OutputIterator>
      static TFEL_FSALGORITHM_INLINE typename std::enable_if<
          tfel::typetraits::IsRandomAccessIterator<InputIterator>::cond &&
              tfel::typetraits::IsRandomAccessIterator<OutputIterator>::cond,
          OutputIterator>::type
      exe(InputIterator p, OutputIterator q) {
        q[0] = p[0];
        q[1] = p[1];
        q[2] = p[2];
        return q + 3;
      }

      /*!
       * General version
       * \see copy<N>::exe for details
       */
      template <typename InputIterator, typename OutputIterator>
      static TFEL_FSALGORITHM_INLINE typename std::enable_if<
          !(tfel::typetraits::IsRandomAccessIterator<InputIterator>::cond &&
            tfel::typetraits::IsRandomAccessIterator<OutputIterator>::cond),
          OutputIterator>::type
      exe(InputIterator p, OutputIterator q) {
        *q = *p;
        return copy<2u>::exe(++p, ++q);
      }
    };

    /*!
     * \brief partial specialisation of struct copy to end recursion.
     *
     * This is disabled if NO_FSALGORITHM_SPECIALISATION is defined
     * \see NO_FSALGORITHM_SPECIALISATION
     *
     * \author Helfer Thomas
     * \date   30 Jun 2006
     */
    template <>
    struct copy<4u> {
      /*!
       * This version of copy is used if iterators are Random Access. Test is
       * based on the tfel::typetraits::IsRandomAccessIterator class.
       *
       * \param InputIterator iterator to the element to be copied
       * \param OutputIterator iterator to the place where this element will be
       * copied
       *
       * \return OutputIterator iterator after the last element copied
       *
       * \pre
       * - InputIterator is a model of Random Access Iterator.
       * - OutputIterator is a model of Random Access Iterator.
       * - InputIterator's value type is convertible to a type in
       * OutputIterator's set of value types.
       *
       * \author Helfer Thomas
       * \date   30 Jun 2006
       */
      template <typename InputIterator, typename OutputIterator>
      static TFEL_FSALGORITHM_INLINE typename std::enable_if<
          tfel::typetraits::IsRandomAccessIterator<InputIterator>::cond &&
              tfel::typetraits::IsRandomAccessIterator<OutputIterator>::cond,
          OutputIterator>::type
      exe(InputIterator p, OutputIterator q) {
        q[0] = p[0];
        q[1] = p[1];
        q[2] = p[2];
        q[3] = p[3];
        return q + 4;
      }

      /*!
       * General version
       * \see copy<N>::exe for details
       */
      template <typename InputIterator, typename OutputIterator>
      static TFEL_FSALGORITHM_INLINE typename std::enable_if<
          !(tfel::typetraits::IsRandomAccessIterator<InputIterator>::cond &&
            tfel::typetraits::IsRandomAccessIterator<OutputIterator>::cond),
          OutputIterator>::type
      exe(InputIterator p, OutputIterator q) {
        *q = *p;
        return copy<3u>::exe(++p, ++q);
      }
    };

    /*!
     * \brief partial specialisation of struct copy to end recursion.
     *
     * This is disabled if NO_FSALGORITHM_SPECIALISATION is defined
     * \see NO_FSALGORITHM_SPECIALISATION
     *
     * \author Helfer Thomas
     * \date   30 Jun 2006
     */
    template <>
    struct copy<5u> {
      /*!
       * This version of copy is used if iterators are Random Access. Test is
       * based on the tfel::typetraits::IsRandomAccessIterator class.
       *
       * \param InputIterator iterator to the element to be copied
       * \param OutputIterator iterator to the place where this element will be
       * copied
       *
       * \return OutputIterator iterator after the last element copied
       *
       * \pre
       * - InputIterator is a model of Random Access Iterator.
       * - OutputIterator is a model of Random Access Iterator.
       * - InputIterator's value type is convertible to a type in
       * OutputIterator's set of value types.
       *
       * \author Helfer Thomas
       * \date   30 Jun 2006
       */
      template <typename InputIterator, typename OutputIterator>
      static TFEL_FSALGORITHM_INLINE typename std::enable_if<
          tfel::typetraits::IsRandomAccessIterator<InputIterator>::cond &&
              tfel::typetraits::IsRandomAccessIterator<OutputIterator>::cond,
          OutputIterator>::type
      exe(InputIterator p, OutputIterator q) {
        q[0] = p[0];
        q[1] = p[1];
        q[2] = p[2];
        q[3] = p[3];
        q[4] = p[4];
        return q + 5;
      }

      /*!
       * General version
       * \see copy<N>::exe for details
       */
      template <typename InputIterator, typename OutputIterator>
      static TFEL_FSALGORITHM_INLINE typename std::enable_if<
          !(tfel::typetraits::IsRandomAccessIterator<InputIterator>::cond &&
            tfel::typetraits::IsRandomAccessIterator<OutputIterator>::cond),
          OutputIterator>::type
      exe(InputIterator p, OutputIterator q) {
        *q = *p;
        return copy<4u>::exe(++p, ++q);
      }
    };

    /*!
     * \brief partial specialisation of struct copy to end recursion.
     *
     * This is disabled if NO_FSALGORITHM_SPECIALISATION is defined
     * \see NO_FSALGORITHM_SPECIALISATION
     *
     * \author Helfer Thomas
     * \date   30 Jun 2006
     */
    template <>
    struct copy<6u> {
      /*!
       * This version of copy is used if iterators are Random Access. Test is
       * based on the tfel::typetraits::IsRandomAccessIterator class.
       *
       * \param InputIterator iterator to the element to be copied
       * \param OutputIterator iterator to the place where this element will be
       * copied
       *
       * \return OutputIterator iterator after the last element copied
       *
       * \pre
       * - InputIterator is a model of Random Access Iterator.
       * - OutputIterator is a model of Random Access Iterator.
       * - InputIterator's value type is convertible to a type in
       * OutputIterator's set of value types.
       *
       * \author Helfer Thomas
       * \date   30 Jun 2006
       */
      template <typename InputIterator, typename OutputIterator>
      static TFEL_FSALGORITHM_INLINE typename std::enable_if<
          tfel::typetraits::IsRandomAccessIterator<InputIterator>::cond &&
              tfel::typetraits::IsRandomAccessIterator<OutputIterator>::cond,
          OutputIterator>::type
      exe(InputIterator p, OutputIterator q) {
        q[0] = p[0];
        q[1] = p[1];
        q[2] = p[2];
        q[3] = p[3];
        q[4] = p[4];
        q[5] = p[5];
        return q + 6;
      }

      /*!
       * General version
       * \see copy<N>::exe for details
       */
      template <typename InputIterator, typename OutputIterator>
      static TFEL_FSALGORITHM_INLINE typename std::enable_if<
          !(tfel::typetraits::IsRandomAccessIterator<InputIterator>::cond &&
            tfel::typetraits::IsRandomAccessIterator<OutputIterator>::cond),
          OutputIterator>::type
      exe(InputIterator p, OutputIterator q) {
        *q = *p;
        return copy<5u>::exe(++p, ++q);
      }
    };

    /*!
     * \brief partial specialisation of struct copy to end recursion.
     *
     * This is disabled if NO_FSALGORITHM_SPECIALISATION is defined
     * \see NO_FSALGORITHM_SPECIALISATION
     *
     * \author Helfer Thomas
     * \date   30 Jun 2006
     */
    template <>
    struct copy<7u> {
      /*!
       * This version of copy is used if iterators are Random Access. Test is
       * based on the tfel::typetraits::IsRandomAccessIterator class.
       *
       * \param InputIterator iterator to the element to be copied
       * \param OutputIterator iterator to the place where this element will be
       * copied
       *
       * \return OutputIterator iterator after the last element copied
       *
       * \pre
       * - InputIterator is a model of Random Access Iterator.
       * - OutputIterator is a model of Random Access Iterator.
       * - InputIterator's value type is convertible to a type in
       * OutputIterator's set of value types.
       *
       * \author Helfer Thomas
       * \date   30 Jun 2006
       */
      template <typename InputIterator, typename OutputIterator>
      static TFEL_FSALGORITHM_INLINE typename std::enable_if<
          tfel::typetraits::IsRandomAccessIterator<InputIterator>::cond &&
              tfel::typetraits::IsRandomAccessIterator<OutputIterator>::cond,
          OutputIterator>::type
      exe(InputIterator p, OutputIterator q) {
        q[0] = p[0];
        q[1] = p[1];
        q[2] = p[2];
        q[3] = p[3];
        q[4] = p[4];
        q[5] = p[5];
        q[6] = p[6];
        return q + 7;
      }

      /*!
       * General version
       * \see copy<N>::exe for details
       */
      template <typename InputIterator, typename OutputIterator>
      static TFEL_FSALGORITHM_INLINE typename std::enable_if<
          !(tfel::typetraits::IsRandomAccessIterator<InputIterator>::cond &&
            tfel::typetraits::IsRandomAccessIterator<OutputIterator>::cond),
          OutputIterator>::type
      exe(InputIterator p, OutputIterator q) {
        *q = *p;
        return copy<6u>::exe(++p, ++q);
      }
    };

    /*!
     * \brief partial specialisation of struct copy to end recursion.
     *
     * This is disabled if NO_FSALGORITHM_SPECIALISATION is defined
     * \see NO_FSALGORITHM_SPECIALISATION
     *
     * \author Helfer Thomas
     * \date   30 Jun 2006
     */
    template <>
    struct copy<8u> {
      /*!
       * This version of copy is used if iterators are Random Access. Test is
       * based on the tfel::typetraits::IsRandomAccessIterator class.
       *
       * \param InputIterator iterator to the element to be copied
       * \param OutputIterator iterator to the place where this element will be
       * copied
       *
       * \return OutputIterator iterator after the last element copied
       *
       * \pre
       * - InputIterator is a model of Random Access Iterator.
       * - OutputIterator is a model of Random Access Iterator.
       * - InputIterator's value type is convertible to a type in
       * OutputIterator's set of value types.
       *
       * \author Helfer Thomas
       * \date   30 Jun 2006
       */
      template <typename InputIterator, typename OutputIterator>
      static TFEL_FSALGORITHM_INLINE typename std::enable_if<
          tfel::typetraits::IsRandomAccessIterator<InputIterator>::cond &&
              tfel::typetraits::IsRandomAccessIterator<OutputIterator>::cond,
          OutputIterator>::type
      exe(InputIterator p, OutputIterator q) {
        q[0] = p[0];
        q[1] = p[1];
        q[2] = p[2];
        q[3] = p[3];
        q[4] = p[4];
        q[5] = p[5];
        q[6] = p[6];
        q[7] = p[7];
        return q + 8;
      }

      /*!
       * General version
       * \see copy<N>::exe for details
       */
      template <typename InputIterator, typename OutputIterator>
      static TFEL_FSALGORITHM_INLINE typename std::enable_if<
          !(tfel::typetraits::IsRandomAccessIterator<InputIterator>::cond &&
            tfel::typetraits::IsRandomAccessIterator<OutputIterator>::cond),
          OutputIterator>::type
      exe(InputIterator p, OutputIterator q) {
        *q = *p;
        return copy<7u>::exe(++p, ++q);
      }
    };

    /*!
     * \brief partial specialisation of struct copy to end recursion.
     *
     * This is disabled if NO_FSALGORITHM_SPECIALISATION is defined
     * \see NO_FSALGORITHM_SPECIALISATION
     *
     * \author Helfer Thomas
     * \date   30 Jun 2006
     */
    template <>
    struct copy<9u> {
      /*!
       * This version of copy is used if iterators are Random Access. Test is
       * based on the tfel::typetraits::IsRandomAccessIterator class.
       *
       * \param InputIterator iterator to the element to be copied
       * \param OutputIterator iterator to the place where this element will be
       * copied
       *
       * \return OutputIterator iterator after the last element copied
       *
       * \pre
       * - InputIterator is a model of Random Access Iterator.
       * - OutputIterator is a model of Random Access Iterator.
       * - InputIterator's value type is convertible to a type in
       * OutputIterator's set of value types.
       *
       * \author Helfer Thomas
       * \date   30 Jun 2006
       */
      template <typename InputIterator, typename OutputIterator>
      static TFEL_FSALGORITHM_INLINE typename std::enable_if<
          tfel::typetraits::IsRandomAccessIterator<InputIterator>::cond &&
              tfel::typetraits::IsRandomAccessIterator<OutputIterator>::cond,
          OutputIterator>::type
      exe(InputIterator p, OutputIterator q) {
        q[0] = p[0];
        q[1] = p[1];
        q[2] = p[2];
        q[3] = p[3];
        q[4] = p[4];
        q[5] = p[5];
        q[6] = p[6];
        q[7] = p[7];
        q[8] = p[8];
        return q + 9;
      }

      /*!
       * General version
       * \see copy<N>::exe for details
       */
      template <typename InputIterator, typename OutputIterator>
      static TFEL_FSALGORITHM_INLINE typename std::enable_if<
          !(tfel::typetraits::IsRandomAccessIterator<InputIterator>::cond &&
            tfel::typetraits::IsRandomAccessIterator<OutputIterator>::cond),
          OutputIterator>::type
      exe(InputIterator p, OutputIterator q) {
        *q = *p;
        return copy<8u>::exe(++p, ++q);
      }
    };

    /*!
     * \brief partial specialisation of struct copy to end recursion.
     *
     * This is disabled if NO_FSALGORITHM_SPECIALISATION is defined
     * \see NO_FSALGORITHM_SPECIALISATION
     *
     * \author Helfer Thomas
     * \date   30 Jun 2006
     */
    template <>
    struct copy<10u> {
      /*!
       * This version of copy is used if iterators are Random Access. Test is
       * based on the tfel::typetraits::IsRandomAccessIterator class.
       *
       * \param InputIterator iterator to the element to be copied
       * \param OutputIterator iterator to the place where this element will be
       * copied
       *
       * \return OutputIterator iterator after the last element copied
       *
       * \pre
       * - InputIterator is a model of Random Access Iterator.
       * - OutputIterator is a model of Random Access Iterator.
       * - InputIterator's value type is convertible to a type in
       * OutputIterator's set of value types.
       *
       * \author Helfer Thomas
       * \date   30 Jun 2006
       */
      template <typename InputIterator, typename OutputIterator>
      static TFEL_FSALGORITHM_INLINE typename std::enable_if<
          tfel::typetraits::IsRandomAccessIterator<InputIterator>::cond &&
              tfel::typetraits::IsRandomAccessIterator<OutputIterator>::cond,
          OutputIterator>::type
      exe(InputIterator p, OutputIterator q) {
        q[0] = p[0];
        q[1] = p[1];
        q[2] = p[2];
        q[3] = p[3];
        q[4] = p[4];
        q[5] = p[5];
        q[6] = p[6];
        q[7] = p[7];
        q[8] = p[8];
        q[9] = p[9];
        return q + 10;
      }

      /*!
       * General version
       * \see copy<N>::exe for details
       */
      template <typename InputIterator, typename OutputIterator>
      static TFEL_FSALGORITHM_INLINE typename std::enable_if<
          !(tfel::typetraits::IsRandomAccessIterator<InputIterator>::cond &&
            tfel::typetraits::IsRandomAccessIterator<OutputIterator>::cond),
          OutputIterator>::type
      exe(InputIterator p, OutputIterator q) {
        *q = *p;
        return copy<9u>::exe(++p, ++q);
      }
    };
#endif /* LIB_TFEL_COPY_HXX_ */

  }  // end of namespace fsalgo

}  // end of namespace tfel

#endif /* LIB_TFEL_COPY_HXX_ */
