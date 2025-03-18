@echo off
setlocal EnableDelayedExpansion

set /p PORT="Please enter the port number (default is 12345): "
if "!PORT!"=="" set PORT=12345

set "VALID=true"
for /f "delims=0123456789" %%i in ("!PORT!") do set "VALID=false"
if "!VALID!"=="false" (
    echo Invalid port number, using default 12345
    set PORT=12345
)
if !PORT! LSS 1 (
    echo Port must be greater than 0, using default 12345
    set PORT=12345
)
if !PORT! GTR 65535 (
    echo Port must be less than 65536, using default 12345
    set PORT=12345
)

echo Starting ChatServer and ChatClients on port: !PORT!

echo Starting ChatServer...
start "ChatServer" ChatServer.exe --port !PORT!

timeout /t 2 >nul

echo Starting ChatClient 1...
start "ChatClient 1" ChatClient.exe --port !PORT!

echo Starting ChatClient 2...
start "ChatClient 2" ChatClient.exe --port !PORT!

echo Starting ChatClient 3...
start "ChatClient 3" ChatClient.exe --port !PORT!

echo Demo started! You can now interact with the chat clients.
pause