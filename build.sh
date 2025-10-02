#!/bin/bash

# Full-Stack Application Build Script
# This script builds all components of the application

set -e

echo "🚀 Building Full-Stack Application..."

# Check if Docker is running
if ! docker info > /dev/null 2>&1; then
    echo "❌ Docker is not running. Please start Docker and try again."
    exit 1
fi

# Build and start all services
echo "📦 Building and starting all services..."
docker-compose up --build -d

echo "✅ Build completed successfully!"
echo ""
echo "📋 Services are now running:"
echo "   • Frontend: http://localhost:3000"
echo "   • Backend API: http://localhost:8080"
echo "   • Nginx: http://localhost:80"
echo "   • MySQL: localhost:3306"
echo ""
echo "🔍 To view logs: docker-compose logs -f"
echo "🛑 To stop: docker-compose down"
echo "🔄 To rebuild: ./build.sh"
