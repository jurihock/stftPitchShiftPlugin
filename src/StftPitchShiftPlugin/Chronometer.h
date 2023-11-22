#pragma once

#if defined(ENABLE_PLUGIN_CHRONOMETER)

  #include <StftPitchShift/Timer.h>

  static stftpitchshift::Timer<std::chrono::milliseconds> chronometer;

  static std::string chronometry(const double samplerate, const int blocksize)
  {
    const double period = (samplerate > 0) ? (blocksize / samplerate) : 0;
    const std::string stats = chronometer.str();

    chronometer.cls();

    std::ostringstream result;

    result << stats << " | ";
    result << int(period * 1e+3)  << " ms ";
    result << "bs=" << blocksize  << " ";
    result << "sr=" << samplerate;

    return result.str();
  }

  #define TIC(...) do { chronometer.tic(); } while (false)
  #define TOC(...) do { chronometer.toc(); } while (false)
  #define LAP(...) (chronometer.lap() > 10e+3)

  #define CHRONOMETRY(...) chronometry(__VA_ARGS__)

#else

  #define TIC(...)
  #define TOC(...)
  #define LAP(...) false

  #define CHRONOMETRY(...) "=^..^="

#endif
