#ifndef ATOMIC_NDARRAY_NONLEAF_H
#define ATOMIC_NDARRAY_NONLEAF_H

#include <atomic>
#include <array>
#include "atomic_ndarray/atomic_ndarray_fwd.h"
#include "atomic_ndarray/atomic_ndarray_utils.h"

/**
 * @brief Non-leaf specialization of atomic_ndarray.
 */
template <
    typename T, 
    std::size_t BitsPerLevel, 
    std::size_t Levels>
class atomic_ndarray<T, BitsPerLevel, Levels, true>
    : private atomic_ndarray_utils
{
private:
    /**
     * @note The following static_asserts are specified for the primary template
     *       of std::atomic<T>, replicated here for clarity.
     * @ref https://en.cppreference.com/w/cpp/atomic/atomic
     */
    static_assert(std::is_trivially_copyable<T>::value, "T must be trivially copyable");
    static_assert(std::is_copy_constructible<T>::value, "T must be copy constructible");
    static_assert(std::is_move_constructible<T>::value, "T must be move constructible");
    static_assert(std::is_copy_assignable<T>::value, "T must be copy assignable");
    static_assert(std::is_move_assignable<T>::value, "T must be move assignable");
    static_assert(std::is_same<T, typename std::remove_cv<T>::type>::value, "T must not be const or volatile");

public:
    static constexpr std::size_t _bits = BitsPerLevel;
    static constexpr std::size_t _levels = Levels;
    static constexpr bool _is_leaf = false;

    /**
     * @note The following static_asserts are redundant, already guaranteed by
     *       template. Still needed to satisfy the compiler.
     */
    static_assert(_bits >= 1u && _bits <= 31u, "");
    static_assert(_levels >= 2u && _levels <= 63u, ""); // (_is_leaf == false)
    static_assert(_bits * _levels <= 63u, "");

public:
    static constexpr std::size_t _size = std::size_t(1u) << _bits;
    using value_type = T;
    using self_type = atomic_ndarray<value_type, _bits, _levels>;
    using child_type = atomic_ndarray<value_type, _bits, _levels - 1u>;
    using self_array_type = std::array<std::atomic<void*>, _size>;

private:
    self_array_type _child_ptrs;

public:
    atomic_ndarray()
        : _child_ptrs()
    {
        for (std::atomic<void*>& ref_atomic: _child_ptrs)
        {
            ref_atomic.store(nullptr, std::memory_order::memory_order_release);
        }
    }

    ~atomic_ndarray()
    {
        /**
         * @note Destructor is empty because the child objects are never deleted
         *       (leaked on purpose).
         */
    }

    child_type& operator[] (std::size_t index)
    {
        if (index >= _size)
        {
            throw_bad_index(index, _size, this);
        }
        std::atomic<void*>& ref_atomic = _child_ptrs.operator[](index);
        /**
         * @note "pv" is just a static_cast<void*> of "pchild", and vice versa.
         * @note We use std::atomic<void*> because its existence is guaranteed
         *       by a primary template in C++11, whereas std::atomic<child_type*>
         *       may have non-trivial specializations (such as requiring a lock).
         */
        void* pv = ref_atomic.load(std::memory_order::memory_order_relaxed);
        child_type* pchild = nullptr;
        if (pv)
        {   
            pchild = static_cast<child_type*>(pv);
            return *pchild;
        }
        /**
         * @note Provisionally allocate a new child object, then try to install it.
         * @note If installation fails, delete the child object, and reload the 
         *       already-installed one instead.
         */
        try
        {
            pchild = call_new<child_type>();
        }
        catch (const std::bad_alloc& e)
        {
            /**
             * @note Record the this pointer for debuggers. See record_last_bad_alloc().
             * @note Insert exception-intercepting code here for debuggers.
             * @note No string formatting here due to low memory.
             * @note Remember that there is no recovery from bad_alloc:
             *       program must be allowed to terminate (no cleanup).
             */
            record_last_bad_alloc(this);
            print_err(e.what());
            throw;
        }
        pv = static_cast<void*>(pchild);
        void* pv_expected_actual = nullptr;
        bool has_installed = ref_atomic.compare_exchange_strong(
            pv_expected_actual, pv, std::memory_order::memory_order_seq_cst);
        if (has_installed)
        {
            /**
             * @note Installed; use pchild.
             */
            return *pchild;
        }
        else
        {
            /**
             * @note Not installed; delete pchild, then static_cast from the
             *       already-installed pv into child_type.
             */
            delete pchild;
            pchild = static_cast<child_type*>(pv_expected_actual);
            return *pchild;
        }
    }
};

#endif // ATOMIC_NDARRAY_NONLEAF_H
