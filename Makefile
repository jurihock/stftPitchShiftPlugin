.PHONY: help build clean app log

help:
	@echo build
	@echo clean
	@echo app
	@echo log

build:
	@cmake -DCMAKE_BUILD_TYPE=Release -S . -B build
	@cmake --build build

clean:
	@rm -rf build

app:
	@open -n ./build/StftPitchShiftPlugin_artefacts/Release/Standalone/StftPitchShiftPlugin.app

log:
	@tail -F ~/Library/Logs/StftPitchShiftPlugin/StftPitchShiftPlugin.log
