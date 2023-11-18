.PHONY: help build clean

help:
	@echo build
	@echo clean

build:
	@cmake -S . -B build
	@cmake --build build

clean:
	@rm -rf build
