#!/bin/bash

sudo apt update

# build essentials
sudo apt install --yes build-essential cmake pkg-config

# https://github.com/juce-framework/JUCE/blob/master/docs/Linux%20Dependencies.md
sudo apt install --yes libasound2-dev libjack-jackd2-dev ladspa-sdk libcurl4-openssl-dev libfreetype6-dev libx11-dev libxcomposite-dev libxcursor-dev libxcursor-dev libxext-dev libxinerama-dev libxrandr-dev libxrender-dev libwebkit2gtk-4.0-dev libglu1-mesa-dev mesa-common-dev
