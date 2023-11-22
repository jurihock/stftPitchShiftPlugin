.PHONY: help build clean app plug unplug log

CONFIG  = Release
OPTIONS = -DLOGGING=ON -DCHRONO=ON

help:
	@echo build
	@echo clean
	@echo app
	@echo plug
	@echo unplug
	@echo log

build:
	@cmake $(OPTIONS) -DCMAKE_BUILD_TYPE=$(CONFIG) -GNinja -S . -B build
	@cmake --build build

clean:
	@rm -rf build

app:
	@open -n ./build/StftPitchShiftPlugin_artefacts/$(CONFIG)/Standalone/StftPitchShiftPlugin.app

plug: unplug
	@cp -r build/StftPitchShiftPlugin_artefacts/$(CONFIG)/AU/StftPitchShiftPlugin.component ~/Library/Audio/Plug-Ins/Components

unplug:
	@rm -rf ~/Library/Audio/Plug-Ins/Components/StftPitchShiftPlugin.component

log:
	@tail -F ~/Library/Logs/StftPitchShiftPlugin/StftPitchShiftPlugin.log
