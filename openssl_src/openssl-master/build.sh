# yum install -y perl-IPC-Cmd perl-ExtUtils-MakeMaker perl-core

#!/bin/bash
set -e

CUR_DIR="$(pwd)"

# 项目根目录，回退两级到 /root/fuck_dev/openssl_tls_demo
PROJECT_ROOT="$(dirname $(dirname "$CUR_DIR"))"

# OpenSSL 输出目录
OUTPUT_DIR="$CUR_DIR/output"

# openssl_tls_demo 目录
DEMO_DIR="$PROJECT_ROOT"

echo "[*] 开始编译 OpenSSL..."

# 检查 Perl IPC::Cmd 模块是否安装
if ! perl -MIPC::Cmd -e1 2>/dev/null; then
  echo "[!] 请先安装 Perl 模块 IPC::Cmd"
  exit 1
fi

# 配置 OpenSSL，静态库编译，输出到 output
./Configure linux-x86_64 no-shared enable-sslkeylog --prefix="$OUTPUT_DIR" --openssldir="$OUTPUT_DIR"
make clean
make -j$(nproc)
make install_sw

echo "[*] 编译完成，开始拷贝文件到项目 openssl_tls_demo ..."

# 目标目录：include 和 lib
INCLUDE_DIR="$DEMO_DIR/include"
LIB_DIR="$DEMO_DIR/lib"

# 清理旧文件
rm -rf "$INCLUDE_DIR/openssl"
rm -rf "$LIB_DIR/*"
# 拷贝头文件
cp -r "$OUTPUT_DIR/include/openssl" "$INCLUDE_DIR/"

# 拷贝静态库
cp "$OUTPUT_DIR/lib64/libssl.a" "$LIB_DIR/"
cp "$OUTPUT_DIR/lib64/libcrypto.a" "$LIB_DIR/"

echo "[✓] OpenSSL 静态库和头文件已成功拷贝到 $DEMO_DIR"

echo "[*] 你可以切换到 $DEMO_DIR，执行 make 编译你的 demo"


