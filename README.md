# Official JUCE plugin for stftPitchShift

The *stftPitchShiftPlugin* features poly pitch and timbre shifting simultaneously and in real-time.
It's completely based on the [stftPitchShift](https://github.com/jurihock/stftPitchShift) library,
which also provides a Python interface and useful command line tools.

Key features:

- poly pitch shifting with or without formant preservation
- timbre changing with or without pitch shifting
- low latency capability depending on the preset block size
- optional output normalization to match the input loudness level

These are the current options, which can be changed on the fly:

<img src="assets/screenshot.png" width="500">

This project is currently at an early development stage,
so expect more highlights to come.

Stay tuned!

## License

*stftPitchShiftPlugin* is licensed under the terms of the GPL license.
For details please refer to the accompanying [LICENSE](LICENSE) file distributed with *stftPitchShiftPlugin*.
