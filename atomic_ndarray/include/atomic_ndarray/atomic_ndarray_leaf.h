#ifndef ATOMIC_NDARRAY_LEAF_H
#define ATOMIC_NDARRAY_LEAF_H

#include <atomic>
#include <array>
#include "atomic_ndarray/atomic_ndarray_fwd.h"
#include "atomic_ndarray/atomic_ndarray_utils.h"

/**
 * @brief Leaf specialization of atomic_ndarray.
 */
template <
    typename T, 
    std::size_t BitsPerLevel>
class atomic_ndarray<T, BitsPerLevel, /*Levels*/ 1u, /*NonLeaf*/ false>
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
    static constexpr std::size_t _levels = 1u;
    static constexpr bool _is_leaf = true;

    /**
     * @note The following static_asserts are redundant, already guaranteed by
     *       template. Still needed to satisfy the compiler.
     */
    static_assert(_bits >= 1u && _bits <= 31u, "");
    static_assert(_levels == 1u, ""); // (_is_leaf == true)
    static_assert(_bits * _levels <= 63u, "");

public:
    static constexpr std::size_t _size = std::size_t(1u) << _bits;
    using value_type = T;
    using self_type = atomic_ndarray<value_type, _bits, _levels>;
    using self_array_type = std::array<std::atomic<T>, _size>;

private:
    self_array_type _values;

public:
    atomic_ndarray()
        : _values()
    {
        const value_type init_value{};
        for (std::atomic<value_type>& ref_atomic_val: _values)
        {
            ref_atomic_val.store(init_value, std::memory_order::memory_order_release);
        }
    }

    ~atomic_ndarray()
    {
        /**
         * @note The leaf specialization has no memory allocations of its own;
         *       the std::array<std::atomic<T>> is part of the class itself.
         */
    }

    std::atomic<T>& operator[] (std::size_t index)
    {
        if (index >= _size)
        {
            throw_bad_index(index, _size, this);
        }
        return static_cast<std::atomic<T>&>(_values[index]);
    }
};

#endif // ATOMIC_NDARRAY_LEAF_H
