# SSL证书配置说明

## 证书文件要求

在生产环境中部署前，需要将SSL证书文件放置在此目录：

### 必需文件：
- `cert.pem` - SSL证书文件
- `key.pem` - 私钥文件

### 获取SSL证书的方式：

#### 1. 使用Let's Encrypt（推荐）
```bash
# 安装certbot
sudo apt install certbot

# 获取证书（需要域名指向服务器IP）
sudo certbot certonly --standalone -d your-domain.com

# 证书位置：/etc/letsencrypt/live/your-domain.com/
# 复制到项目目录：
cp /etc/letsencrypt/live/your-domain.com/fullchain.pem ./nginx/ssl/cert.pem
cp /etc/letsencrypt/live/your-domain.com/privkey.pem ./nginx/ssl/key.pem
```

#### 2. 商业SSL证书
从证书颁发机构（CA）购买证书，将提供的文件重命名为：
- 证书文件 → `cert.pem`
- 私钥文件 → `key.pem`

#### 3. 自签名证书（仅用于测试）
```bash
# 生成自签名证书
openssl req -x509 -nodes -days 365 -newkey rsa:2048 \
  -keyout nginx/ssl/key.pem \
  -out nginx/ssl/cert.pem \
  -subj "/C=US/ST=State/L=City/O=Organization/CN=localhost"
```

## 文件权限
确保私钥文件权限正确：
```bash
chmod 600 nginx/ssl/key.pem
```

## 自动续期（Let's Encrypt）
Let's Encrypt证书有效期为90天，建议设置自动续期：
```bash
# 添加到crontab
0 12 * * * /usr/bin/certbot renew --quiet
```

## 部署注意事项
- 确保`nginx/ssl`目录在部署时传输到服务器
- 证书文件路径在`nginx/nginx.prod.conf`中配置为`/etc/nginx/ssl/`
- 生产环境建议使用有效的SSL证书
