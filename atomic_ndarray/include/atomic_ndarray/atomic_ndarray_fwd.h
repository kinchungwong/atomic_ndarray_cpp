#ifndef ATOMIC_NDARRAY_FWD_H
#define ATOMIC_NDARRAY_FWD_H

#include "atomic_ndarray/atomic_ndarray_traits.h"

/**
 * @brief A class template for a multi-dimensional array of atomic variables.
 *
 * @tparam ValueType The type of the values to be stored in the last level array.
 * 
 * @tparam LogTwoSize The 2-logarithm of the size of each level of the array.
 *         In other words, the number of bits needed to represent the an index
 *         in each level of the array. The size of each level is thus
 *         ```(1 << LogTwoSize)```.
 *
 * @tparam Levels The number of levels in the multi-dimensional array.
 * 
 * @tparam (NonLeaf is automatically deduced. Do not specify.)
 * 
 * @tparam (IsValid is automatically deduced. Do not specify.)
 * 
 * @details Usage: atomic_ndarray<ValueType, LogTwoSize, Levels>
 *
 * @details Each level of the array is represented by a separate atomic_ndarray
 *          object, which is stored in an atomic pointer. The number of levels
 *          and the number of bits per level are specified as template parameters.
 *
 * @details Each intermediate array is only allocated when it is accessed for the
 *          first time. This allows for efficient memory usage, as only the
 *          necessary levels of the array are allocated.
 *
 * @details Once allocated, all intermediate arrays are never deleted.
 */
template <
    typename ValueType,
    std::size_t LogTwoSize,
    std::size_t Levels,
    bool NonLeaf = (Levels >= 2u),
    bool = atomic_ndarray_is_valid<ValueType, LogTwoSize, Levels>::value>
class atomic_ndarray;

#endif // ATOMIC_NDARRAY_FWD_H
