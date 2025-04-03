// include guard for source file is only needed for single-translation-unit build mode.
#ifndef ATOMIC_NDARRAY_UTILS_CPP
#define ATOMIC_NDARRAY_UTILS_CPP

#include <stdexcept>
#include <string>
#include <cstdio>
#include <iostream>
#include "atomic_ndarray/atomic_ndarray_utils.h"

/*member_static*/
const void* atomic_ndarray_utils::record_last_bad_alloc(const void* p_this)
{
    static const void* last_bad_alloc_this = nullptr;
    if (p_this)
    {
        last_bad_alloc_this = p_this;
    }
    return last_bad_alloc_this;
}

/*member_static*/
void atomic_ndarray_utils::throw_bad_index(std::size_t index, std::size_t size, const void* p_this)
{
    static constexpr std::size_t buf_size = 96u;
    char buf[buf_size] = {};
    std::snprintf(buf, sizeof(buf), "Index out of range: %zu >= %zu, this=%p", index, size, p_this);
    buf[buf_size - 1u] = 0;
    throw std::out_of_range(buf);
}

/*member_static*/
void atomic_ndarray_utils::print_err(const char* msg)
{
    const char* msg2 = msg ? msg : "(msg is nullptr)";
    std::cerr << msg2 << std::endl;
}

#endif // ATOMIC_NDARRAY_UTILS_CPP
