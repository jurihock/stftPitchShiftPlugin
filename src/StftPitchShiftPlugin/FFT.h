#pragma once

#if defined(ENABLE_POCKET_FFT)

  #include <StftPitchShiftPlugin/FFT/PocketFFT.h>

  typedef PocketFFT FFT;

#else

  #include <StftPitchShiftPlugin/FFT/RealFFT.h>

  typedef RealFFT FFT;

#endif
