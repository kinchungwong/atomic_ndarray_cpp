#ifndef ATOMIC_NDARRAY_UTILS_H
#define ATOMIC_NDARRAY_UTILS_H

#include <cstdint>

#define INLINE_NEVER __attribute__((noinline))
#define INLINE_ALWAYS __attribute__((always_inline))

/**
 * @brief Utility functions for atomic_ndarray, to allow separation of details
 *        and for runtime debugging (breakpoints and crash analysis).
 *
 * @note Uses non-standard formatting for function declarations due to
 *       compiler-specific keywords and attributes required to be in
 *       compiler-dependent order.
 */
class atomic_ndarray_utils
{
protected:
/**
 * @todo Export this function to debugger interface.
 */
static
const void*
INLINE_NEVER
record_last_bad_alloc(const void* p_this);

/**
 * @brief Throws an std::out_of_range exception with a message indicating
 *        the index and size of the array. The this pointer is included to
 *        aid debugging.
 */
[[noreturn]]
static
void
INLINE_NEVER
throw_bad_index(std::size_t index, std::size_t size, const void* p_this = nullptr);

/**
 * @brief Prints an error message to stderr without formatting.
 */
static
void
INLINE_NEVER
print_err(const char* msg);

/**
 * @note Handles allocations on behalf of atomic_ndarray, since all such allocations
 *       are intended to leak on purpose. (Once allocated, they are never deleted.)
 * @todo Add this function to the runtime memory leaker analyzer list.
 */
template <typename T>
static
T*
INLINE_NEVER
call_new()
{
    return new T();
}

}; // class atomic_ndarray_utils

#endif // ATOMIC_NDARRAY_UTILS_H
