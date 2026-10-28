/*!
 * \file   mfront/include/MFront/GenericParallel/StatusBlockPool.hxx
 * \brief  This file declares a pool of blocks of memory holding the
 * statuses or the statistics of the computations performed on a device.
 * \date   08/10/2026
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_GENERICPARALLEL_STATUSBLOCKPOOL_HXX
#define LIB_MFRONT_GENERICPARALLEL_STATUSBLOCKPOOL_HXX

#include <mutex>
#include <vector>
#include <utility>

namespace mfront::generic_parallel {

  /*!
   * \brief pool of blocks of memory holding the statuses or the statistics
   * of synchronous computations performed on a device.
   *
   * The kernels only write in a block when an event occurs (failure at a
   * point, bound violated, etc.). When the block is accessible by the host
   * and by the device (managed or shared memory), the host initializes it
   * before the computations and reads it once they are completed: without
   * event, the device does not access the block and no data is transferred
   * between the host and the device.
   *
   * The blocks are allocated at their first use on a device and reused by
   * the following calls: no memory is allocated per call. A block is used
   * by one call at a time, so that concurrent calls (from several threads or
   * on several queues) are supported. As the copies of the shared data, the
   * blocks are never freed by the pool.
   *
   * \tparam T: type of the values of a block (a block may hold several
   * values, the allocation function giving the size of the blocks)
   * \tparam Device: type identifying a device, comparable with `==`
   */
  template <typename T, typename Device>
  class StatusBlockPool {
   public:
    //! \brief a block, given back to the pool at destruction
    class Block {
     public:
      //! \brief move constructor
      Block(Block&& src) noexcept
          : pool(std::exchange(src.pool, nullptr)),
            values(std::exchange(src.values, nullptr)) {}
      Block(const Block&) = delete;
      Block& operator=(Block&&) = delete;
      Block& operator=(const Block&) = delete;
      //! \return the values of the block, or null if the allocation failed
      T* get() const noexcept { return this->values; }
      //! \brief destructor
      ~Block() {
        if (this->pool != nullptr) {
          this->pool->release(this->values);
        }
      }

     private:
      friend class StatusBlockPool;
      Block(StatusBlockPool* const p, T* const v) noexcept
          : pool(p), values(v) {}
      //! \brief pool owning the block
      StatusBlockPool* pool;
      //! \brief values of the block
      T* values;
    };
    /*!
     * \return a block of the given device, not used by another call. The
     * block is null if the allocation failed.
     * \param[in] d: device
     * \param[in] allocate: function allocating a block on the device,
     * called if all the blocks of the device are used, and returning a null
     * pointer on failure
     */
    template <typename AllocationFunction>
    Block acquire(const Device& d,
                  const AllocationFunction& allocate) noexcept {
      const auto lock = std::lock_guard{this->mutex};
      for (auto& b : this->blocks) {
        if ((!b.used) && (b.device == d)) {
          b.used = true;
          return Block(this, b.values);
        }
      }
      try {
        this->blocks.reserve(this->blocks.size() + 1);
        auto* const v = allocate();
        if (v == nullptr) {
          return Block(nullptr, nullptr);
        }
        this->blocks.push_back({d, v, true});
        return Block(this, v);
      } catch (...) {
      }
      return Block(nullptr, nullptr);
    }  // end of acquire

   private:
    //! \brief give a block back to the pool
    void release(T* const v) noexcept {
      const auto lock = std::lock_guard{this->mutex};
      for (auto& b : this->blocks) {
        if (b.values == v) {
          b.used = false;
          return;
        }
      }
    }  // end of release
    //! \brief description of a block
    struct BlockDescription {
      //! \brief device
      Device device;
      //! \brief values
      T* values;
      //! \brief if true, the block is used by a call
      bool used;
    };
    //! \brief mutex protecting the blocks
    std::mutex mutex;
    //! \brief blocks
    std::vector<BlockDescription> blocks;
  };

}  // end of namespace mfront::generic_parallel

#endif /* LIB_MFRONT_GENERICPARALLEL_STATUSBLOCKPOOL_HXX */
