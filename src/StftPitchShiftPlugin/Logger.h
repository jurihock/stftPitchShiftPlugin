#pragma once

#define ENABLE_PLUGIN_FILE_LOGGER

#if defined(ENABLE_PLUGIN_FILE_LOGGER)

  #include <JuceHeader.h>

  #include <utility>

  static FileLogger* plugin_file_logger = FileLogger::createDefaultAppLogger(
    "StftPitchShiftPlugin", "StftPitchShiftPlugin.log", "");

  template<typename... Args>
  static void write_to_log_file(const juce::String& str, Args&&... args)
  {
    const auto message = juce::String::formatted(
      str, std::forward<Args>(args)...);

    plugin_file_logger->logMessage(message);
  }

  #define LOG(...) write_to_log_file(__VA_ARGS__)

#else

  #define LOG(...)

#endif
