@echo off
setlocal enabledelayedexpansion

where cloudflared >nul 2>&1
if %errorlevel% equ 0 (
    echo cloudflared is already installed
    goto :run_tunnel
)

echo cloudflared not found, installing...

set "TEMP_DIR=%TEMP%\cloudflared_install"
if not exist "%TEMP_DIR%" mkdir "%TEMP_DIR%"

echo Downloading cloudflared...
curl -L "https://github.com/cloudflare/cloudflared/releases/latest/download/cloudflared-windows-amd64.exe" -o "%TEMP_DIR%\cloudflared.exe"

if %errorlevel% neq 0 (
    echo Failed to download cloudflared
    pause
    exit /b 1
)

set "INSTALL_DIR=%LOCALAPPDATA%\cloudflared"
if not exist "%INSTALL_DIR%" mkdir "%INSTALL_DIR%"

move /y "%TEMP_DIR%\cloudflared.exe" "%INSTALL_DIR%\cloudflared.exe"

set "PATH=%INSTALL_DIR%;%PATH%"

setx PATH "%INSTALL_DIR%;%PATH%" >nul 2>&1

echo cloudflared installed successfully

:run_tunnel
echo Starting tunnel...
cloudflared tunnel --url http://localhost:8080

pause