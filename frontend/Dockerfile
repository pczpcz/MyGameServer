FROM node:18-alpine

# 设置时区
RUN apk add --no-cache tzdata
ENV TZ=Asia/Shanghai

# 设置工作目录
WORKDIR /app

# 复制package文件
COPY package*.json ./

# 使用国内npm镜像安装依赖
RUN npm config set registry https://registry.npmmirror.com && \
    npm install --no-optional

# 复制源代码
COPY . .

# 构建应用
RUN npm run build

# 暴露端口
EXPOSE 3000

# 启动应用
CMD ["npm", "start"]
