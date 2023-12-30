@echo off

pushd "%~dp0"
set BASE=%CD%
popd

pushd "%BASE%\.."
set ROOT=%CD%
popd

set ARGS=--strictness-level 5 --validate-in-process --validate
set CONFIG=Release
set PLUGIN=StftPitchShiftPlugin
set INPUT=%ROOT%
set OUTPUT=%ROOT%\build

set ARTEFACTS=%OUTPUT%\%PLUGIN%_artefacts\%CONFIG%
set PLUGINVAL=%OUTPUT%\_deps\pluginval-src\pluginval.exe

"%PLUGINVAL%" %ARGS% "%ARTEFACTS%\VST3\%PLUGIN%.vst3" || exit /b

del /q "%ARTEFACTS%\%PLUGIN%_SharedCode.lib"
del /q "%ARTEFACTS%\VST3\%PLUGIN%.exp"
del /q "%ARTEFACTS%\VST3\%PLUGIN%.lib"
