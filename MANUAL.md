# stftPitchShiftPlugin User Manual

This audio effect modifies the frequency content of the input signal in several ways in real-time.

## Parameters

Following customizations are currently available:

<img src="assets/screenshot.png" width="500">

| # | Parameter | Function |
|:-:| ------- | -------- |
| `1` | Bypass | Enable to skip the signal processing and pass the input signal directly to the output. |
| `2` | Normalize | Enable to match the processed output loudness level to the input level. |
| `3` | Timbre quefrency | Cutoff _quefrency_ of the [cepstral](https://en.wikipedia.org/wiki/Cepstrum) low-pass _lifter_ in milliseconds used for timbre extraction. If you set it to zero, the pitch independent timbre processing disappears. |
| `4` | Timbre shift | Number of semitones to shift the timbre independent of the pitch when `3` is enabled. |
| <nobr>`5`-`9`</nobr> | Pitch shift | Number of semitones to repeatedly shift the pitch. Depending on `3` it will have an equal effect on the timbre. |
| `10` | Pitch stages | Total amount of pitch shifts that will be applied. |
| `11` | STFT overlap | Degree of the internal audio segment [overlap](https://api.semanticscholar.org/CorpusID:15841658). |
| `12` | DFT size | Number of frequency bins to be used for spectral processing. |
| `13` | Low latency | Reduce the output latency to the lowest possible value based on the preset block size. |

## Automation

- All parameters are adjustable at runtime, the changes take effect immediately.
- Manually adjusting parameters `11`, `12`, and `13` causes a complete reset of the internal signal processing. Permanently changing these two parameters at runtime is not recommended.
- Additionally, parameters `11` and `12` are automatically adapted for extremely small and large block sizes. This usually happens only once at startup or on each manual change of `11` and `12`.

## Routing

Formally, the _stftPitchShiftPlugin_ supports any mono/stereo combination of input and output.
However, only the first input channel (usually _left_) will be captured and passed to all connected output channels.

## Latency

The output latency _generally_ depends on the preset block size and the DFT size `12`.
In low latency mode `13`, smaller blocks can also affect the overall quality of the signal processing.

Furthermore, the low latency mode `13` requires a constant block size. Some plugin hosts may vary the block size erratically during the processing time. In this case, this feature is not applicable.

## Performance

The plugin performance is _most_ affected by parameters `3`, `11` and `12`.
If you're experiencing dropouts, consider disabling `3` or reducing `11` and `12`.
