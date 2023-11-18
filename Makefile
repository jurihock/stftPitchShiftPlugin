.PHONY: help build run clean

help:
	@echo build
	@echo run
	@echo clean

build:
	@cmake -DCMAKE_BUILD_TYPE=Release -S . -B build
	@cmake --build build

run:
	@open -n ./build/StftPitchShiftPlugin_artefacts/Release/Standalone/StftPitchShiftPlugin.app

clean:
	@rm -rf build
