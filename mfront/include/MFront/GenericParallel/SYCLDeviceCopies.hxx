/*!
 * \file   mfront/include/MFront/GenericParallel/SYCLDeviceCopies.hxx
 * \brief  This file declares the copies in device memory of the arrays which
 * are not accessible by the device associated with a SYCL queue, used by
 * the `generic-parallel` interface for material properties.
 * \date   08/10/2026
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_GENERICPARALLEL_SYCLDEVICECOPIES_HXX
#define LIB_MFRONT_GENERICPARALLEL_SYCLDEVICECOPIES_HXX

#include <cstddef>
#include <cstring>
#include <optional>
#include <type_traits>
#include <vector>
#include <sycl/sycl.hpp>
#include "TFEL/Raise.hxx"

namespace mfront::generic_parallel::sycl {

  //! \brief a simple alias
  using size_type = std::size_t;

  //! \brief policy for the transfer of the data of a batch or of a set of
  //! points
  enum struct DataTransferPolicy {
    /*!
     * \brief the arrays which are not accessible by the device associated
     * with the queue (host memory not allocated by SYCL) are copied in
     * device memory, and the outputs are copied back
     */
    AUTOMATIC,
    //! \brief all the arrays are assumed to be accessible by the device
    NONE
  };

  /*!
   * \brief strided array: the `j`-th component of the `i`-th point is
   * stored at `values[i * point_stride + j * component_stride]`
   */
  template <typename ValueType>
  struct StridedArray {
    //! \brief pointer to the first value
    ValueType* values;
    //! \brief stride between two points
    size_type point_stride;
    //! \brief stride between two components
    size_type component_stride;
  };

  /*!
   * \brief copies in device memory of the fields which are not accessible
   * by the device associated with a queue. The outputs must be copied back
   * by calling the `copyOutputs` method.
   */
  struct DeviceCopies {
    /*!
     * \brief constructor
     * \param[in] q: queue
     * \param[in] n: number of points of the fields
     * \param[in] p: policy for the transfer of the fields. With the `NONE`
     * policy, no copy is made.
     */
    DeviceCopies(::sycl::queue& q,
                 const size_type n,
                 const DataTransferPolicy p)
        : number_of_points(n),
          queue(q),
          enabled(p == DataTransferPolicy::AUTOMATIC) {}
    DeviceCopies(DeviceCopies&&) = delete;
    DeviceCopies(const DeviceCopies&) = delete;
    DeviceCopies& operator=(DeviceCopies&&) = delete;
    DeviceCopies& operator=(const DeviceCopies&) = delete;
    /*!
     * \brief copy a field in device memory if it is not accessible by the
     * device, and make the field refer to the copy. All the values covered
     * by the strides of the field are copied. For an output, only the
     * values of the field are copied back, so that the values of other
     * outputs sharing the same memory (for example, the thermodynamic
     * forces and the internal state variables of a point stored
     * contiguously) are not overwritten.
     * \param[in,out] f: field
     * \param[in] size: number of components
     * \param[in] output: if true, the field is an output
     */
    template <typename FieldType>
    void transfer(FieldType& f, const size_type size, const bool output) {
      const auto n = this->number_of_points;
      if ((!this->enabled) || (f.values == nullptr) || (size == 0) ||
          (n == 0) || (this->isAccessible(f.values))) {
        return;
      }
      const auto extent =
          (n - 1) * f.point_stride + (size - 1) * f.component_stride + 1;
      using value_type =
          std::remove_const_t<std::remove_pointer_t<decltype(f.values)>>;
      const auto l = Layout{n, size, f.point_stride, f.component_stride};
      f.values = static_cast<value_type*>(
          this->copy(f.values, extent * sizeof(value_type), sizeof(value_type),
                     output, extent == n * size ? std::optional<Layout>{} : l));
    }
    /*!
     * \brief copy an array in device memory if it is not accessible by the
     * device, and make the pointer refer to the copy.
     * \param[in,out] p: pointer to the array
     * \param[in] size: number of values
     * \param[in] output: if true, the array is an output
     */
    template <typename ValueType>
    void transfer(ValueType*& p, const size_type size, const bool output) {
      if ((!this->enabled) || (p == nullptr) || (size == 0) ||
          (this->isAccessible(p))) {
        return;
      }
      using value_type = std::remove_const_t<ValueType>;
      p = static_cast<value_type*>(this->copy(p, size * sizeof(value_type),
                                              sizeof(value_type), output,
                                              std::optional<Layout>{}));
    }
    /*!
     * \brief copy the outputs back to their original locations, once the
     * computations submitted to the queue are completed
     */
    void copyOutputs() {
      if (this->copies.empty()) {
        return;
      }
      this->queue.wait_and_throw();
      for (const auto& c : this->copies) {
        if (!c.output) {
          continue;
        }
        if (!c.layout.has_value()) {
          this->queue.memcpy(c.host, c.device, c.size).wait_and_throw();
          continue;
        }
        auto values = std::vector<std::byte>(c.size);
        this->queue.memcpy(values.data(), c.device, c.size).wait_and_throw();
        const auto& l = *(c.layout);
        auto* const host = static_cast<std::byte*>(c.host);
        for (size_type i = 0; i != l.number_of_points; ++i) {
          for (size_type k = 0; k != l.number_of_components; ++k) {
            const auto o =
                (i * l.point_stride + k * l.component_stride) * c.value_size;
            std::memcpy(host + o, values.data() + o, c.value_size);
          }
        }
      }
    }
    /*!
     * \return if the given pointer is accessible by the device: any pointer
     * if the device is a CPU, the memory allocated by SYCL (unified shared
     * memory) otherwise
     * \param[in] p: pointer
     */
    [[nodiscard]] bool isAccessible(const void* const p) const {
      if (this->queue.get_device().is_cpu()) {
        return true;
      }
      return ::sycl::get_pointer_type(p, this->queue.get_context()) !=
             ::sycl::usm::alloc::unknown;
    }
    //! \return if the arrays which are not accessible are copied
    [[nodiscard]] bool isEnabled() const noexcept { return this->enabled; }
    //! \return if some arrays have been copied
    [[nodiscard]] bool holdsCopies() const noexcept {
      return !this->copies.empty();
    }
    //! \brief destructor
    ~DeviceCopies() {
      for (const auto& c : this->copies) {
        ::sycl::free(c.device, this->queue);
      }
    }

   private:
    //! \brief positions of the values of a strided field
    struct Layout {
      size_type number_of_points;
      size_type number_of_components;
      size_type point_stride;
      size_type component_stride;
    };
    //! \brief description of a copy in device memory
    struct Copy {
      //! \brief original location
      void* host;
      //! \brief copy
      void* device;
      //! \brief size in bytes
      std::size_t size;
      //! \brief size of a value in bytes
      std::size_t value_size;
      //! \brief if true, the copy must be copied back
      bool output;
      /*!
       * \brief positions of the values of the field, if the copy also
       * covers values which do not belong to the field
       */
      std::optional<Layout> layout;
    };
    /*!
     * \brief copy an array in device memory
     * \return the copy
     * \param[in] p: array
     * \param[in] size: size in bytes
     * \param[in] value_size: size of a value in bytes
     * \param[in] output: if true, the copy must be copied back
     * \param[in] layout: positions of the values to be copied back, if
     * not all the values of the array
     */
    void* copy(const void* const p,
               const std::size_t size,
               const std::size_t value_size,
               const bool output,
               const std::optional<Layout>& layout) {
      // the storage of the description is reserved before the allocation,
      // and the description is stored before the transfer, so that the copy
      // is freed by the destructor if the transfer throws
      this->copies.reserve(this->copies.size() + 1);
      auto* const c = ::sycl::malloc_device(size, this->queue);
      if (c == nullptr) {
        tfel::raise("memory allocation failed on the SYCL device");
      }
      this->copies.push_back(
          Copy{const_cast<void*>(p), c, size, value_size, output, layout});
      this->queue.memcpy(c, p, size).wait_and_throw();
      return c;
    }
    //! \brief number of points of the fields
    const size_type number_of_points;
    //! \brief copies
    std::vector<Copy> copies;
    //! \brief queue
    ::sycl::queue& queue;
    //! \brief if false, no copy is made
    const bool enabled;
  };

}  // end of namespace mfront::generic_parallel::sycl

#endif /* LIB_MFRONT_GENERICPARALLEL_SYCLDEVICECOPIES_HXX */
