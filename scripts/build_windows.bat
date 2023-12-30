@echo off

pushd "%~dp0"
set BASE=%CD%
popd

pushd "%BASE%\.."
set ROOT=%CD%
popd

set ARGS=-DCMAKE_GENERATOR_PLATFORM=x64
set CONFIG=Release
set PLUGIN=StftPitchShiftPlugin
set INPUT=%ROOT%
set OUTPUT=%ROOT%\build

set ARTEFACTS=%OUTPUT%\%PLUGIN%_artefacts\%CONFIG%

cmake %ARGS% -S "%INPUT%" -B "%OUTPUT%" || exit /b
cmake --build "%OUTPUT%" --config %CONFIG% || exit /b

echo DONE!
dir /b "%ARTEFACTS%"
