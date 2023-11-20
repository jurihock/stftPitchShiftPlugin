.PHONY: help build clean app plug unplug log

help:
	@echo build
	@echo clean
	@echo app
	@echo plug
	@echo unplug
	@echo log

build:
	@cmake -DCMAKE_BUILD_TYPE=Release -GNinja -S . -B build
	@cmake --build build

clean:
	@rm -rf build

app:
	@open -n ./build/StftPitchShiftPlugin_artefacts/Release/Standalone/StftPitchShiftPlugin.app

plug: unplug
	@cp -r build/StftPitchShiftPlugin_artefacts/Release/AU/StftPitchShiftPlugin.component ~/Library/Audio/Plug-Ins/Components

unplug:
	@rm -rf ~/Library/Audio/Plug-Ins/Components/StftPitchShiftPlugin.component

log:
	@tail -F ~/Library/Logs/StftPitchShiftPlugin/StftPitchShiftPlugin.log
