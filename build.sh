#!/bin/bash
set -e

PROJECT_ROOT="$(cd "$(dirname "$0")" && pwd)"
BUILD_DIR="$PROJECT_ROOT/build"
CERTS_SRC="$PROJECT_ROOT/certs"
CERTS_DST="$BUILD_DIR/certs"
EXECUTABLE="$BUILD_DIR/openssl_tls_demo"

echo "[*] 清理旧编译文件..."
rm -rf "$BUILD_DIR"

echo "[*] 创建构建目录：$BUILD_DIR"
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

echo "[*] 运行 cmake 生成构建文件..."
cmake ..

echo "[*] 编译项目..."
cmake --build . -- -j$(nproc)

echo "[*] 拷贝 certs 到构建目录..."
if [ -d "$CERTS_SRC" ]; then
    mkdir -p "$CERTS_DST"
    cp -r "$CERTS_SRC/"* "$CERTS_DST/" 2>/dev/null || echo "(warn) certs 目录为空"
else
    echo "(warn) certs 目录不存在"
fi

echo "[*] 执行程序：$EXECUTABLE"

# unset SSLKEYLOGFILE

export SSLKEYLOGFILE=/root/ssl_out.log
# openssl s_client -connect 172.16.0.201:8080
# GET /
echo "[*] 已设置 SSLKEYLOGFILE=$SSLKEYLOGFILE"

if [ -f "$EXECUTABLE" ]; then
    chmod +x "$EXECUTABLE"
    "$EXECUTABLE"
else
    echo "[!] 可执行文件不存在：$EXECUTABLE"
    exit 1
fi

echo "[✓] 构建完成！"

