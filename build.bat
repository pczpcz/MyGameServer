@echo off
REM Full-Stack Application Build Script for Windows
REM This script builds all components of the application

echo 🚀 Building Full-Stack Application...

REM Check if Docker is running
docker info >nul 2>&1
if %errorlevel% neq 0 (
    echo ❌ Docker is not running. Please start Docker and try again.
    exit /b 1
)

REM Build and start all services
echo 📦 Building and starting all services...
docker-compose up --build -d

if %errorlevel% equ 0 (
    echo ✅ Build completed successfully!
    echo.
    echo 📋 Services are now running:
    echo    • Frontend: http://localhost:3000
    echo    • Backend API: http://localhost:8080
    echo    • Nginx: http://localhost:80
    echo    • MySQL: localhost:3306
    echo.
    echo 🔍 To view logs: docker-compose logs -f
    echo 🛑 To stop: docker-compose down
    echo 🔄 To rebuild: build.bat
) else (
    echo ❌ Build failed. Check the logs above for details.
)
