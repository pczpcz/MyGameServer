# HTTPS 已禁用说明

## 配置变更

本项目已禁用HTTPS并关闭SSL配置，具体变更包括：

### 1. nginx.prod.conf
- 移除了HTTP到HTTPS的重定向服务器块
- 移除了HTTPS服务器配置（SSL证书、HTTP/2、SSL安全设置等）
- 移除了HSTS（HTTP Strict Transport Security）头
- 现在只监听80端口

### 2. docker-compose.prod.yml
- 移除了443端口的暴露
- 移除了SSL证书目录的挂载

### 3. 开发环境
- nginx.conf 和 docker-compose.yml 已配置为只使用HTTP

## 当前状态
- 应用现在只通过HTTP协议访问（端口80）
- 不再需要SSL证书
- 不再强制重定向到HTTPS

## 注意事项
- 如果需要重新启用HTTPS，需要：
  1. 恢复nginx.prod.conf中的HTTPS配置
  2. 在docker-compose.prod.yml中重新暴露443端口
  3. 配置SSL证书
  4. 运行 `scripts/setup-ssl.sh` 脚本

## 安全考虑
在禁用HTTPS的情况下，建议：
- 在内部网络环境中使用
- 避免传输敏感信息
- 考虑使用VPN或其他安全措施
