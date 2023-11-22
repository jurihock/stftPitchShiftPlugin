#pragma once

#if defined(ENABLE_PLUGIN_LOGGER)

  #include <JuceHeader.h>

  static juce::FileLogger* plugin_file_logger = juce::FileLogger::createDefaultAppLogger(
    "StftPitchShiftPlugin", "StftPitchShiftPlugin.log", "");

  template<typename... Args>
  static void write_to_log_file(const juce::String& str, Args&&... args)
  {
    const auto message = juce::String::formatted(
      str, std::forward<Args>(args)...);

    plugin_file_logger->logMessage(message);
  }

  #define LOG(...) do { write_to_log_file(__VA_ARGS__); } while (false)

#else

  #define LOG(...)

#endif
