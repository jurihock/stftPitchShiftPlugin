# Official JUCE plugin for stftPitchShift

The *stftPitchShiftPlugin* features poly pitch and timbre shifting simultaneously and in real-time.
It's completely based on the [stftPitchShift](https://github.com/jurihock/stftPitchShift) library,
which also provides a Python interface and useful command line tools.

## Features

- poly pitch shifting with or without formant preservation
- timbre changing with or without pitch shifting
- low latency capability depending on the preset block size
- optional output normalization to match the input loudness level

The *stftPitchShiftPlugin* is available in common VST3, AU, LV2 formats as well as a standalone app.
It also provides a basic UI to adjust effect options on the fly:

<img src="assets/screenshot.png" width="500">

Please refer to the [User Manual](MANUAL.md) for detailed explanations.

This project is currently at an early development stage,
so expect more highlights to come.

Stay tuned!

## License

*stftPitchShiftPlugin* is licensed under the terms of the GPL license.
For details please refer to the accompanying [LICENSE](LICENSE) file distributed with *stftPitchShiftPlugin*.
