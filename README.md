# OpenSSL TLS Demo

本项目是一个基于 OpenSSL 静态库的简单 TLS 服务器 Demo，监听 8080 端口，适合学习 OpenSSL 在 C 语言项目中的集成与使用。

## 目录结构

```
.
├── CMakeLists.txt      # CMake 构建配置
├── include/            # OpenSSL 头文件
├── lib/                # OpenSSL 静态库
├── src/
│   ├── main.c
│   └── server.c
├── certs/              # 证书和密钥
└── openssl_src/        # OpenSSL 源码及构建脚本
```

## 编译流程

1. **构建 OpenSSL 静态库和头文件**

   `openssl_src/` 目录下包含两个版本的 OpenSSL 源码。[自行下载](https://github.com/openssl/openssl),进入对应版本目录，执行 `build.sh` 脚本，会自动编译 OpenSSL，并将生成的静态库和头文件自动复制到此项目的 `lib/` 和 `include/` 目录下。

   ```sh
   cd openssl_src/openssl-master   # 或 openssl-1_1_1-stable
   ./build.sh
   ```

2. **编译 Demo 程序**

   只有在完成 OpenSSL 静态库和头文件的构建后，才能在项目根目录下执行 `build.sh` 脚本来编译 Demo 程序：

   ```sh
   ./build.sh
   ```

   生成的可执行文件为 `openssl_tls_demo`。

## 运行方法

1. 确保有服务器证书和私钥（如 `certs/server.crt` 和 `certs/server.key`）。可执行 crt_gen.sh 来生成
2. 运行程序：

   ```sh
   ./openssl_tls_demo
   ```

3. 用浏览器或 curl 访问：

   ```
   https://localhost:8080
   ```

   或

   ```sh
   curl -k https://localhost:8080
   ```

## 依赖

- CMake >= 3.10
- gcc
- OpenSSL 静态库（libssl.a, libcrypto.a）

## 说明

- 本项目仅用于学习和演示 TLS 通信，未做生产环境安全加固。
- 如需重新生成证书，请使用 OpenSSL 工具或相关脚本