#define SINGLE_TRANSLATION_UNIT_BUILD_MODE
#include "atomic_ndarray/atomic_ndarray.h"

int main()
{
    atomic_ndarray<int, 1, 1> i_1_1;
    atomic_ndarray<int, 1, 2> i_1_2;
    atomic_ndarray<int, 1, 3> i_1_3;
    atomic_ndarray<int, 1, 16> i_1_16;
    atomic_ndarray<int, 2, 16> i_2_16;
    atomic_ndarray<int, 3, 16> i_3_16;
    atomic_ndarray<int, 16, 3> i_16_3;
    atomic_ndarray<int, 1, 48> i_1_48;
    atomic_ndarray<int, 24, 2> i_24_2;

#if 0
    /**
     * @note The following line should not compile, as the product of LogTwoSize
     *       and Levels exceeds 63.
     */
    atomic_ndarray<int, 8, 8> i_8_8;
#endif

    return 0;
}
