#pragma once

#define ENABLE_PLUGIN_CHRONOMETER

#if defined(ENABLE_PLUGIN_CHRONOMETER)

  #include <StftPitchShift/Timer.h>

  static stftpitchshift::Timer<std::chrono::milliseconds> chronometer;

  static std::string chronometry()
  {
    const std::string str = chronometer.str();
    chronometer.cls();
    return str;
  }

  #define TIC(...) do { chronometer.tic(); } while (false)
  #define TOC(...) do { chronometer.toc(); } while (false)
  #define LAP(...) (chronometer.lap() > 10e+3)

  #define CHRONOMETRY(...) chronometry()

#else

  #define TIC(...)
  #define TOC(...)
  #define LAP(...) false

  #define CHRONOMETRY(...) ""

#endif
