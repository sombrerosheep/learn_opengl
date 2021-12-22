@echo off

REM Clear all args to 0
SET CLEANBUILD=0
SET SETUPENV=0
SET REBUILD=0

REM Process args
:ARGS
IF "%1"=="" GOTO EXEC
IF "%1"=="CLEAN" SET CLEANBUILD=1
IF "%1"=="SETUP" SET SETUPENV=1
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

IF %SETUPENV%==1 (
  echo Initializing visual studio environment
  SET foundvs=0

  for %%a in (
    "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvarsall.bat"
    "C:\Program Files (x86)\Microsoft Visual Studio\2017\Professional\VC\Auxiliary\Build\vcvarsall.bat"
  ) do (
    echo Looking for visual studio install at: %%a

    IF EXIST "%%a" (
      echo Found visual studio at: %%a

      IF NOT defined DevEnvDir (
        @call %%a x64
      ) ELSE (
        echo DevEnvDir already set. skipping call to vcvarsall.
      )

      SET foundvs=1
      GOTO EXITVSLOOP
    )
  )

  :EXITVSLOOP
  IF "%foundvs%"=="0" (
    echo ERROR - Couldn't find visual studio installation
    echo ERROR - Failure setting up environment!
  ) ELSE (
    echo VS Setup complete
  )
)


IF NOT EXIST .\build (
  mkdir build
)

pushd .\build

IF "%REBUILD%"=="1" (
  echo "rebuilding cmake project"
  cmake ..\ -G "Visual Studio 15 2017 Win64"
)

IF NOT defined DevEnvDir (
  echo ERROR - DevEnvDir was not defined. Please re-run with the SETUP arg.
) ELSE (
  echo Building solution...
  devenv learn-opengl.sln /build
)

popd
