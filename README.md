# Full-Stack Web Application

A complete full-stack web application with:
- **Backend**: C++ REST API using cpprestsdk
- **Frontend**: Next.js React application with TypeScript
- **Database**: MySQL with C++ MySQL Connector
- **Web Server**: Nginx reverse proxy
- **Containerization**: Docker with Docker Compose

## Project Structure

```
├── backend/                 # C++ backend application
│   ├── src/                # Source code
│   │   ├── main.cpp        # Main application entry point
│   │   ├── Database.h/cpp  # Database connection and operations
│   │   ├── User.h/cpp      # User model
│   │   ├── UserService.h/cpp # Business logic
│   │   └── UserController.h/cpp # HTTP request handlers
│   ├── CMakeLists.txt      # CMake build configuration
│   └── Dockerfile          # Backend Docker configuration
├── frontend/               # Next.js frontend application
│   ├── app/                # Next.js app directory
│   │   ├── layout.tsx      # Root layout
│   │   ├── page.tsx        # Main page
│   │   └── globals.css     # Global styles
│   ├── next.config.js      # Next.js configuration
│   ├── package.json        # Node.js dependencies
│   └── Dockerfile          # Frontend Docker configuration
├── nginx/
│   └── nginx.conf          # Nginx reverse proxy configuration
├── mysql/
│   └── init.sql            # Database initialization script
├── docker-compose.yml      # Multi-container orchestration
├── build.sh                # Linux/macOS build script
├── build.bat               # Windows build script
└── README.md
```

## Quick Start

### Prerequisites
- Docker and Docker Compose installed
- Git (for cloning the repository)

### Build and Run

**Option 1: Using build scripts**
```bash
# Linux/macOS
./build.sh

# Windows
build.bat
```

**Option 2: Using Docker Compose directly**
```bash
docker-compose up --build
```

### Access the Application

After successful build, access the application at:
- **Frontend**: http://localhost:3000
- **Backend API**: http://localhost:8080
- **Nginx**: http://localhost:80
- **MySQL**: localhost:3306

## API Endpoints

The C++ backend provides the following REST API endpoints:

- `GET /api/users` - Get all users
- `GET /api/users/{id}` - Get user by ID
- `POST /api/users` - Create new user
- `PUT /api/users/{id}` - Update user
- `DELETE /api/users/{id}` - Delete user
- `GET /api/` - Health check

## Services Architecture

### Backend (C++)
- **Port**: 8080
- **Framework**: cpprestsdk (Casablanca)
- **Database**: MySQL Connector/C++
- **Features**: REST API, JSON serialization, database operations

### Frontend (Next.js)
- **Port**: 3000
- **Framework**: Next.js 14 with App Router
- **Language**: TypeScript
- **Features**: React hooks, API integration, responsive design

### Database (MySQL)
- **Port**: 3306
- **Database**: myapp
- **User**: appuser/apppassword
- **Root**: root/rootpassword

### Web Server (Nginx)
- **Port**: 80
- **Role**: Reverse proxy and load balancer
- **Features**: SSL termination, static file serving, CORS support

## Development

### Backend Development
```bash
cd backend
# The backend will be built automatically in Docker
# For local development, install:
# - build-essential, cmake
# - libmysqlcppconn-dev
# - libcpprest-dev
```

### Frontend Development
```bash
cd frontend
npm install
npm run dev
```

### Database Management
```bash
# Connect to MySQL
docker exec -it mysql_db mysql -u appuser -papppassword myapp

# View logs
docker-compose logs mysql
```

## Useful Commands

```bash
# Start services
docker-compose up -d

# Stop services
docker-compose down

# View logs
docker-compose logs -f

# Rebuild specific service
docker-compose build backend

# Access container shell
docker exec -it cpp_backend bash
```

## Features

- ✅ Complete CRUD operations for users
- ✅ RESTful API design
- ✅ MySQL database integration
- ✅ Docker containerization
- ✅ Nginx reverse proxy
- ✅ CORS support
- ✅ Error handling
- ✅ Sample data initialization
- ✅ Health check endpoints

## Technology Stack

- **Backend**: C++17, cpprestsdk, MySQL Connector/C++, CMake
- **Frontend**: Next.js 14, React 18, TypeScript, CSS3
- **Database**: MySQL 8.0
- **Infrastructure**: Docker, Docker Compose, Nginx
- **Development**: Git, VS Code

## Troubleshooting

1. **Port conflicts**: Ensure ports 3000, 8080, 80, and 3306 are available
2. **Docker not running**: Start Docker Desktop or Docker service
3. **Build failures**: Check Docker logs with `docker-compose logs`
4. **Database connection**: Wait for MySQL to initialize (first startup takes longer)

## License

This project is for educational and demonstration purposes.
