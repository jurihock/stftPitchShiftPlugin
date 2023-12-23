.PHONY: help build clean app log plug unplug reset test

GENERATOR = Ninja
CONFIG    = Release
PLATFORM  = -DCMAKE_OSX_ARCHITECTURES=arm64
OPTIONS   = -DLOGGING=ON -DCHRONO=ON

PLUGIN    = StftPitchShiftPlugin
INPUT     = .
OUTPUT    = ./build
ARTEFACTS = $(OUTPUT)/$(PLUGIN)_artefacts/$(CONFIG)
PLUGINVAL = $(OUTPUT)/_deps/pluginval-src/Contents/MacOS/pluginval

help:
	@echo build
	@echo clean
	@echo app
	@echo log
	@echo plug
	@echo unplug
	@echo reset
	@echo test

build:
	@cmake -G $(GENERATOR) -DCMAKE_BUILD_TYPE=$(CONFIG) $(PLATFORM) $(OPTIONS) -S $(INPUT) -B $(OUTPUT)
	@cmake --build $(OUTPUT)

clean:
	@rm -rf $(OUTPUT)

app:
	@open -n $(ARTEFACTS)/Standalone/$(PLUGIN).app

log:
	@tail -F ~/Library/Logs/$(PLUGIN)/$(PLUGIN).log

plug: unplug
	@cp -rf $(ARTEFACTS)/AU/$(PLUGIN).component ~/Library/Audio/Plug-Ins/Components

unplug:
	@rm -rf ~/Library/Audio/Plug-Ins/Components/$(PLUGIN).component

reset: unplug
	@rm -rf ~/Library/Application\ Support/Audacity/plugin*.cfg
	@rm -rf ~/Library/Caches/AudioUnitCache
	@sudo killall -9 AudioComponentRegistrar
	@auval -a

test: plug
	@$(PLUGINVAL) --strictness-level 5 --validate ~/Library/Audio/Plug-Ins/Components/$(PLUGIN).component
