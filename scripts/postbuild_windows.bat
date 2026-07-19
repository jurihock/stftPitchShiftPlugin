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

rem Execute pluginval.
"%PLUGINVAL%" %ARGS% "%ARTEFACTS%\VST3\StftPitchShiftPlugin.vst3" || exit /b

rem Delete intermediate files.
del /q "%ARTEFACTS%\StftPitchShiftPlugin_SharedCode.lib"
del /q "%ARTEFACTS%\VST3\StftPitchShiftPlugin.exp"
del /q "%ARTEFACTS%\VST3\StftPitchShiftPlugin.lib"

rem # Zip windows binaries in the same way as in macos and ubuntu builds.
pushd "%ARTEFACTS%"
tar -caf "..\StftPitchShiftPlugin.zip" -C "%ARTEFACTS%" . || exit /b
popd
rmdir /s /q "%ARTEFACTS%" || exit /b
mkdir "%ARTEFACTS%" || exit /b
pushd "%ARTEFACTS%"
move "..\StftPitchShiftPlugin.zip" . || exit /b
popd
