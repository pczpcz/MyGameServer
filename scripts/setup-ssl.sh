#!/bin/bash

# Let's Encrypt SSL证书自动配置脚本
# 使用方法: ./setup-ssl.sh your-domain.com

set -e  # 遇到错误立即退出

# 颜色输出
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# 日志函数
log_info() {
    echo -e "${GREEN}[INFO]${NC} $1"
}

log_warn() {
    echo -e "${YELLOW}[WARN]${NC} $1"
}

log_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# 检查参数
if [ $# -eq 0 ]; then
    log_error "请提供域名参数"
    echo "使用方法: $0 your-domain.com"
    exit 1
fi

DOMAIN=$1
SSL_DIR="./nginx/ssl"
PROJECT_ROOT=$(pwd)

log_info "开始为域名 $DOMAIN 配置SSL证书..."

# 检查是否以root权限运行
if [ "$EUID" -ne 0 ]; then
    log_warn "建议使用sudo运行此脚本以获得最佳效果"
fi

# 1. 安装certbot
log_info "检查并安装certbot..."
if ! command -v certbot &> /dev/null; then
    log_info "安装certbot..."
    apt update
    apt install -y certbot
else
    log_info "certbot已安装"
fi

# 2. 停止Nginx容器（如果正在运行）
log_info "检查并停止Nginx容器..."
if docker ps | grep -q nginx_proxy; then
    log_info "停止Nginx容器..."
    docker stop nginx_proxy
fi

# 3. 获取SSL证书
log_info "获取SSL证书..."
certbot certonly --standalone \
    -d "$DOMAIN" \
    --non-interactive \
    --agree-tos \
    --email admin@$DOMAIN \
    --preferred-challenges http

# 4. 创建SSL目录
log_info "创建SSL目录..."
mkdir -p "$SSL_DIR"

# 5. 复制证书文件到项目目录
log_info "复制证书文件..."
CERT_PATH="/etc/letsencrypt/live/$DOMAIN"
if [ -d "$CERT_PATH" ]; then
    cp "$CERT_PATH/fullchain.pem" "$SSL_DIR/cert.pem"
    cp "$CERT_PATH/privkey.pem" "$SSL_DIR/key.pem"
    
    # 设置正确的文件权限
    chmod 600 "$SSL_DIR/key.pem"
    chmod 644 "$SSL_DIR/cert.pem"
    
    log_info "证书文件已复制到 $SSL_DIR/"
else
    log_error "证书目录不存在: $CERT_PATH"
    exit 1
fi

# 6. 创建自动续期脚本
log_info "创建自动续期脚本..."
cat > /usr/local/bin/renew-ssl.sh << 'EOF'
#!/bin/bash
# SSL证书自动续期脚本

set -e

DOMAIN="$1"
PROJECT_DIR="/path/to/your/project"  # 需要根据实际情况修改

log_info() {
    echo "[$(date)] [INFO] $1"
}

log_error() {
    echo "[$(date)] [ERROR] $1"
}

if [ -z "$DOMAIN" ]; then
    log_error "请提供域名参数"
    exit 1
fi

log_info "开始续期SSL证书: $DOMAIN"

# 停止Nginx容器
docker stop nginx_proxy

# 续期证书
certbot renew --quiet

# 复制新证书
CERT_PATH="/etc/letsencrypt/live/$DOMAIN"
if [ -d "$CERT_PATH" ]; then
    cp "$CERT_PATH/fullchain.pem" "$PROJECT_DIR/nginx/ssl/cert.pem"
    cp "$CERT_PATH/privkey.pem" "$PROJECT_DIR/nginx/ssl/key.pem"
    
    # 设置权限
    chmod 600 "$PROJECT_DIR/nginx/ssl/key.pem"
    chmod 644 "$PROJECT_DIR/nginx/ssl/cert.pem"
    
    log_info "证书已更新"
else
    log_error "证书目录不存在"
    exit 1
fi

# 重新启动Nginx容器
cd "$PROJECT_DIR" && docker-compose up -d nginx

log_info "SSL证书续期完成"
EOF

# 设置续期脚本权限
chmod +x /usr/local/bin/renew-ssl.sh

# 7. 添加crontab自动续期
log_info "设置自动续期..."
(crontab -l 2>/dev/null | grep -v "renew-ssl.sh"; echo "0 3 * * * /usr/local/bin/renew-ssl.sh $DOMAIN") | crontab -

# 8. 验证证书
log_info "验证证书文件..."
if [ -f "$SSL_DIR/cert.pem" ] && [ -f "$SSL_DIR/key.pem" ]; then
    log_info "证书验证成功:"
    echo "证书文件: $SSL_DIR/cert.pem"
    echo "私钥文件: $SSL_DIR/key.pem"
    
    # 显示证书信息
    openssl x509 -in "$SSL_DIR/cert.pem" -noout -subject -dates
else
    log_error "证书文件验证失败"
    exit 1
fi

# 9. 重新启动服务
log_info "重新启动Docker服务..."
cd "$PROJECT_ROOT"
docker-compose up -d

log_info "SSL证书配置完成!"
log_info "证书将在每天凌晨3点自动续期"
log_info "现在可以通过 https://$DOMAIN 访问您的服务"

# 10. 显示部署状态
echo ""
log_info "部署状态检查:"
docker ps --format "table {{.Names}}\t{{.Status}}\t{{.Ports}}"
