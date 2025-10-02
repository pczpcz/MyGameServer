FROM ubuntu:22.04

# Install dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    libmysqlcppconn-dev \
    libboost-all-dev \
    libcpprest-dev \
    pkg-config \
    libssl-dev \
    && rm -rf /var/lib/apt/lists/*

# Set working directory
WORKDIR /app

# Copy source code
COPY . .

# Create build directory and build the project
RUN mkdir build && cd build && \
    cmake .. && \
    make

# Expose port
EXPOSE 8080

# Run the application
CMD ["./build/myapp"]
