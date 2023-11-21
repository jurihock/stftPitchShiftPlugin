#pragma once

#define CHRONOMETRY

#if defined(CHRONOMETRY)

  #include <StftPitchShift/Timer.h>

  static stftpitchshift::Timer<std::chrono::milliseconds> chronometry;

  static std::string chronolog()
  {
    const std::string str = chronometry.str();
    chronometry.cls();
    return str;
  }

  #define TIC(...) do { chronometry.tic(); } while (false)
  #define TOC(...) do { chronometry.toc(); } while (false)
  #define LAP(...) (chronometry.lap() > 10e+3)

  #define CHRONOLOG(...) chronolog()

#else

  #define TIC(...)
  #define TOC(...)
  #define LAP(...) false

  #define CHRONOLOG(...) ""

#endif
