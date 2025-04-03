#ifndef ATOMIC_NDARRAY_TRAITS_H
#define ATOMIC_NDARRAY_TRAITS_H

#include <type_traits>

template <
    typename ValueType,
    std::size_t LogTwoSize,
    std::size_t Levels,
    bool ValueTypeValid = (
        std::is_trivially_copyable<ValueType>::value &&
        std::is_copy_constructible<ValueType>::value &&
        std::is_move_constructible<ValueType>::value &&
        std::is_copy_assignable<ValueType>::value &&
        std::is_move_assignable<ValueType>::value &&
        std::is_same<ValueType, typename std::remove_cv<ValueType>::type>::value
    ),
    bool LogTwoSizeValid = (
        LogTwoSize >= 1u && 
        LogTwoSize <= 31u
    ),
    bool LevelsValid = (
        Levels >= 1u && 
        Levels <= 63u
    ),
    bool LogTwoSizeTimesLevelsValid = (
        LogTwoSize * Levels <= 63u
    )>
class atomic_ndarray_is_valid
    : private std::false_type
{
    /**
     * @note The following static_asserts are redundant, but they provide
     *       useful compiler diagnostics.
     */
    static_assert(ValueTypeValid, "ValueType must be trivially copyable, copy/move constructible/assignable, and not const or volatile");
    static_assert(LogTwoSizeValid, "LogTwoSize must be between 1 and 31");
    static_assert(LevelsValid, "Levels must be between 1 and 63");
    static_assert(LogTwoSizeTimesLevelsValid, "(LogTwoSize * Levels) must be less than or equal to 63");
};

template <
    typename ValueType,
    std::size_t LogTwoSize,
    std::size_t Levels>
class atomic_ndarray_is_valid<ValueType, LogTwoSize, Levels, true, true, true, true>
    : public std::true_type
{
};

#endif // ATOMIC_NDARRAY_TRAITS_H
