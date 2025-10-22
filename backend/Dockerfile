FROM ubuntu:22.04

# 设置时区和语言环境
ENV TZ=Asia/Shanghai
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

# 使用国内镜像源替换apt源
RUN sed -i 's/archive.ubuntu.com/mirrors.aliyun.com/g' /etc/apt/sources.list && \
    sed -i 's/security.ubuntu.com/mirrors.aliyun.com/g' /etc/apt/sources.list

# 安装依赖包
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    libmysqlcppconn-dev \
    libboost-all-dev \
    libcpprest-dev \
    pkg-config \
    libssl-dev \
    ca-certificates \
    && rm -rf /var/lib/apt/lists/*

# 设置工作目录
WORKDIR /app

# 复制源代码
COPY . .

# 创建构建目录并构建项目
RUN rm -rf build && mkdir build && cd build && \
    cmake .. && \
    make -j$(nproc)

# 暴露端口
EXPOSE 8080
EXPOSE 8888

# 运行应用程序
CMD ["./build/myapp"]
