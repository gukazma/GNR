@echo off

:: 创建环境
call conda env create -f environment.yml --prefix ./env
if %ERRORLEVEL% NEQ 0 (
    echo Conda environment creation failed.
    exit /B %ERRORLEVEL%
)

:: 激活环境
call conda activate ./env
if %ERRORLEVEL% NEQ 0 (
    echo Failed to activate conda environment.
    exit /B %ERRORLEVEL%
)

:: 创建并进入 build 目录
mkdir "build"
cd "build"

:: 运行 cmake
cmake .. %*
if %ERRORLEVEL% NEQ 0 (
    echo CMake configuration failed.
    exit /B %ERRORLEVEL%
)

echo Build setup completed successfully.