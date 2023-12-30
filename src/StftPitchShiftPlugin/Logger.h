#pragma once

#if defined(ENABLE_PLUGIN_LOGGER)

  #include <JuceHeader.h>

  static std::shared_ptr<juce::FileLogger> plugin_file_logger =
    std::shared_ptr<juce::FileLogger>(
      juce::FileLogger::createDefaultAppLogger(
        juce::String(ProjectInfo::projectName),
        juce::String(ProjectInfo::projectName) + ".log",
        juce::String("")));

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
