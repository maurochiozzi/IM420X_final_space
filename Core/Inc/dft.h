/*
 * Discrete Fourier transform (C)
 * by Project Nayuki, 2021. Public domain.
 * https://www.nayuki.io/page/how-to-implement-the-discrete-fourier-transform
 */


/*
 * Computes the discrete Fourier transform (DFT) of the given complex vector.
 * All the array arguments must be non-NULL and have a length equal to n.
 */

#include <math.h>
#include <stddef.h>

#ifndef __DFT_ALGORITHM__
#define __DFT_ALGORITHM__

void compute_dft_real_pair(const double inreal[restrict], const double inimag[restrict], double outreal[restrict], double outimag[restrict], size_t n);

#endif
