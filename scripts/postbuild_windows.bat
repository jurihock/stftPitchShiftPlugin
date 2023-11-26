@echo off

pushd "%~dp0"
set BASE=%CD%
popd

pushd "%BASE%\.."
set ROOT=%CD%
popd

set ARGS=--strictness-level 5 --validate-in-process --validate
set CONFIG=Release
set INPUT=%ROOT%
set OUTPUT=%ROOT%\build

set ARTEFACTS=%OUTPUT%\StftPitchShiftPlugin_artefacts\%CONFIG%
set PLUGINVAL=%OUTPUT%\_deps\pluginval-src\pluginval.exe

"%PLUGINVAL%" %ARGS% "%ARTEFACTS%\VST3\StftPitchShiftPlugin.vst3" || exit /b
