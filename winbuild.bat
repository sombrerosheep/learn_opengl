@echo off

SET CLEANBUILD=0
SET SETUPENV=0

IF "%1"=="CLEAN" SET CLEANBUILD=1
IF "%2"=="CLEAN" SET CLEANBUILD=1

IF "%CLEANBUILD%"=="1" (
  echo "cleaning build dir"
  DEL /S .\build\* /q
)

IF "%1"=="SETENV" SET SETUPENV=1
IF "%2"=="SETENV" SET SETUPENV=1

IF "%SETUPENV%"=="1" (
  echo "init vs env"
  @call "%vs150comntools%VsDevCmd.bat"
)
mkdir build
pushd .\build
cmake ..\ -G "Visual Studio 15 2017 Win64"
devenv learn-opengl.sln /build

popd
