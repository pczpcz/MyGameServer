# GitHub Action 自动化部署指南

## 概述

本项目使用 GitHub Actions 实现自动化部署流程。当代码推送到 `main` 分支时，会自动构建 Docker 镜像并推送到 Docker Hub。

## 工作流配置

### 触发条件
- 推送到 `main` 分支
- 针对 `main` 分支的 Pull Request

### 工作流程步骤

1. **构建和测试阶段**
   - 检出代码
   - 设置 Docker Buildx
   - 登录 Docker Hub
   - 分别构建并推送后端、前端、Nginx 镜像
   - 上传 MySQL 初始化脚本

2. **部署阶段** (仅限 main 分支)
   - 下载 MySQL 初始化脚本
   - 执行部署命令

## 所需环境变量

在 GitHub 仓库的 Settings → Secrets and variables → Actions 中配置以下密钥：

- `DOCKERHUB_USERNAME`: Docker Hub 用户名
- `DOCKERHUB_TOKEN`: Docker Hub 访问令牌

## 镜像命名规则

- 后端镜像: `docker.io/<username>/<repository>-backend:latest`
- 前端镜像: `docker.io/<username>/<repository>-frontend:latest`
- Nginx 镜像: `docker.io/<username>/<repository>-nginx:latest`

## MySQL 配置

MySQL 使用官方 `mysql:8.0` 镜像，无需构建自定义镜像。工作流会自动：
- 上传 MySQL 初始化脚本 (`mysql/init.sql`) 作为构建产物
- 在部署阶段下载初始化脚本
- 拉取官方 MySQL 8.0 镜像
- 将初始化脚本传输到目标服务器
- 确保 MySQL 容器启动时执行初始化脚本

## 部署配置

### 当前部署步骤
目前部署阶段仅输出构建成功的镜像信息。您需要根据实际部署环境配置具体的部署命令。

### 可能的部署方式

#### 1. SSH 部署到服务器
```yaml
- name: Deploy via SSH
  uses: appleboy/ssh-action@v1.0.3
  with:
    host: ${{ secrets.SERVER_HOST }}
    username: ${{ secrets.SERVER_USERNAME }}
    key: ${{ secrets.SERVER_SSH_KEY }}
    script: |
      cd /path/to/your/project
      docker-compose pull
      docker-compose up -d
```

#### 2. Kubernetes 部署
```yaml
- name: Deploy to Kubernetes
  uses: azure/k8s-deploy@v4
  with:
    namespace: default
    manifests: k8s/
    images: |
      ${{ env.REGISTRY }}/${{ env.IMAGE_NAME }}-backend:latest
      ${{ env.REGISTRY }}/${{ env.IMAGE_NAME }}-frontend:latest
      ${{ env.REGISTRY }}/${{ env.IMAGE_NAME }}-nginx:latest
```

#### 3. 使用 docker-compose 部署
```yaml
- name: Deploy with docker-compose
  run: |
    scp docker-compose.yml ${{ secrets.SERVER_USER }}@${{ secrets.SERVER_HOST }}:/path/to/project/
    ssh ${{ secrets.SERVER_USER }}@${{ secrets.SERVER_HOST }} "cd /path/to/project && docker-compose pull && docker-compose up -d"
```

## 自定义配置

### 修改触发分支
要修改触发部署的分支，编辑 `.github/workflows/deploy.yml` 中的：
```yaml
on:
  push:
    branches: [ main ]  # 修改为您想要的分支
```

### 添加环境变量
要添加更多环境变量，在 `env` 部分添加：
```yaml
env:
  REGISTRY: docker.io
  IMAGE_NAME: ${{ github.repository }}
  YOUR_VARIABLE: your_value
```

### 添加构建缓存
工作流已配置构建缓存以提高构建速度。缓存会自动存储在 GitHub Actions 缓存中。

## 故障排除

1. **构建失败**
   - 检查 Dockerfile 语法
   - 验证依赖项是否正确安装
   - 检查网络连接

2. **推送镜像失败**
   - 验证 Docker Hub 凭据
   - 检查镜像名称格式
   - 确认仓库权限

3. **部署失败**
   - 验证服务器连接信息
   - 检查服务器上的 Docker 服务状态
   - 确认部署脚本权限

## 监控和日志

- 在 GitHub 仓库的 Actions 标签页查看工作流执行状态
- 点击具体的工作流运行查看详细日志
- 设置通知以便在部署失败时收到提醒
