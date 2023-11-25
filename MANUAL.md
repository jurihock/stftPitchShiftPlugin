# stftPitchShiftPlugin User Manual

This audio effect modifies the frequency content of the input signal in several ways in real-time.

## Parameters

Currently, the following customizations are available, which can be adjusted at runtime:

<img src="assets/screenshot.png" width="500">

| # | Feature | Function |
| - | ------- | -------- |
| 1 | Bypass | Enable to skip the signal processing and pass the input signal directly to the output. |
| 2 | Normalize | Enable to match the processed output loudness level to the input level. |
| 3 | Timbre quefrency | Cutoff _quefrency_ of the low-pass _lifter_ in milliseconds used for timbre extraction. If you set it to zero, the pitch independent timbre processing disappears. |
| 4 | Timbre shift | Number of semitones to shift the timbre independent of the pitch when `3` is enabled. |
| 5-9 | Pitch shift | Number of semitones to repeatedly shift the pitch. Depending on `3` it will have an equal effect on the timbre. |
| 10 | Pitch stages | Total amount of pitch shifts that will be applied. |
| 11 | STFT overlap | Degree of the internal audio segment overlap. |
| 12 | DFT size | Size of the signal analysis window. |

Adjusting of parameters `11` and `12` results in a total reset of the internal signal processing routine.
Therefore, it is not recommended to change these two parameters at runtime.

## Routing

Formally, the _stftPitchShiftPlugin_ supports any mono/stereo combination of input and output.
However, only the first input channel (usually _left_) will be captured and passed to all connected output channels.

## Latency

The output latency generally depends on the preset block size and the DFT window size `12`.
Smaller blocks may have impact on the overal quality of the signal processing.

## Performance

The plugin performance is most affected by parameters `3`, `11` and `12`.
If you are experiencing dropouts, consider disabling `3` or reducing `11` and `12`.
