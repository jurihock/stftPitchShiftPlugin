.PHONY: help build clean app plug unplug log kill

GENERATOR = Ninja
CONFIG    = Release
PLATFORM  = -DCMAKE_OSX_ARCHITECTURES=arm64
OPTIONS   = -DLOGGING=ON -DCHRONO=ON

PLUGIN    = StftPitchShiftPlugin
INPUT     = .
OUTPUT    = ./build
ARTEFACTS = $(OUTPUT)/$(PLUGIN)_artefacts/$(CONFIG)

help:
	@echo build
	@echo clean
	@echo app
	@echo plug
	@echo unplug
	@echo log
	@echo kill

build:
	@cmake -G$(GENERATOR) -DCMAKE_BUILD_TYPE=$(CONFIG) $(PLATFORM) $(OPTIONS) -S $(INPUT) -B $(OUTPUT)
	@cmake --build $(OUTPUT)

clean:
	@rm -rf $(OUTPUT)

app:
	@open -n $(ARTEFACTS)/Standalone/$(PLUGIN).app

plug: unplug
	@cp -rf $(ARTEFACTS)/AU/$(PLUGIN).component ~/Library/Audio/Plug-Ins/Components

unplug:
	@rm -rf ~/Library/Audio/Plug-Ins/Components/$(PLUGIN).component

log:
	@tail -F ~/Library/Logs/$(PLUGIN)/$(PLUGIN).log

kill:
	@sudo killall -9 AudioComponentRegistrar
