.PHONY: help build run clean

help:
	@echo build
	@echo run
	@echo clean

build:
	@cmake -S . -B build
	@cmake --build build

run:
	@open -n ./build/StftPitchShiftPlugin_artefacts/Standalone/StftPitchShiftPlugin.app

clean:
	@rm -rf build
