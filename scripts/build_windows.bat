@echo off

pushd "%~dp0"
set BASE=%CD%
popd

pushd "%BASE%\.."
set ROOT=%CD%
popd

set ARGS=-DCMAKE_GENERATOR_PLATFORM=x64
set CONFIG=Release
set INPUT=%ROOT%
set OUTPUT=%ROOT%\build

set ARTEFACTS=%OUTPUT%\StftPitchShiftPlugin_artefacts\%CONFIG%

cmake %ARGS% -S "%INPUT%" -B "%OUTPUT%" || exit /b
cmake --build "%OUTPUT%" --config %CONFIG% || exit /b

if not exist "%ARTEFACTS%\Standalone\StftPitchShiftPlugin.exe" (
  exit /b 1
)

echo DONE!
dir /b "%ARTEFACTS%"
