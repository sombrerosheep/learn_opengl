@echo off

REM Clear all args to 0
SET CLEANBUILD=0
SET SETUPENV=0
SET REBUILD=0

REM Process args
:ARGS
IF "%1"=="" GOTO EXEC
IF "%1"=="CLEAN" SET CLEANBUILD=1
IF "%1"=="SETENV" SET SETUPENV=1
IF "%1"=="REBUILD" SET REBUILD=1
SHIFT

GOTO ARGS


REM execute actions
:EXEC

IF "%CLEANBUILD%"=="1" (
  echo "cleaning build dir"
  DEL /S .\build\* /q
  SET REBUILD=1
)

IF "%SETUPENV%"=="1" (
  echo "init vs env"
  @call "%vs150comntools%VsDevCmd.bat"
)

IF NOT EXIST .\build (
  mkdir build
)

pushd .\build

IF "%REBUILD%"=="1" (
  echo "rebuilding cmake project"
  cmake ..\ -G "Visual Studio 15 2017 Win64"
)

devenv learn-opengl.sln /build

popd
