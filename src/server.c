#include "config.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <openssl/err.h>
#include <openssl/ssl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>


#define BUFFER_SIZE 4096

void handle_client(int client_sock, SSL* ssl)
{
    char buffer[BUFFER_SIZE];
    int bytes = USE_TLS ? SSL_read(ssl, buffer, sizeof(buffer) - 1)
                        : read(client_sock, buffer, sizeof(buffer) - 1);
    if (bytes <= 0) {
        perror("Read error");
        return;
    }

    buffer[bytes] = 0;
    printf("Received request:\n%s\n", buffer);

    // 构造 HTML 响应，将请求内容显示在页面上
    char html[BUFFER_SIZE + 256];
    snprintf(html, sizeof(html),
        "<html><body><h2>Request Info</h2><pre>%s</pre></body></html>", buffer);

    char response[BUFFER_SIZE + 512];
    int content_length = strlen(html);
    snprintf(response, sizeof(response),
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html; charset=utf-8\r\n"
        "Content-Length: %d\r\n"
        "\r\n"
        "%s",
        content_length, html);

    if (USE_TLS) {
        SSL_write(ssl, response, strlen(response));
    } else {
        write(client_sock, response, strlen(response));
    }
}

int run_server()
{
#if USE_TLS
    SSL_library_init();
    SSL_load_error_strings();
    OpenSSL_add_ssl_algorithms();
    SSL_CTX* ctx = SSL_CTX_new(TLS_server_method());
    SSL_CTX_set_min_proto_version(ctx, TLS1_2_VERSION);
    // // 设置 keylog 回调
    // void keylog_callback(const SSL* ssl, const char* line)
    // {
    //     const char* keylog_path = getenv("SSLKEYLOGFILE");
    //     if (!keylog_path)
    //         keylog_path = "tls_keylog.txt";
    //     FILE* keylog = fopen(keylog_path, "a");
    //     if (keylog) {
    //         fprintf(keylog, "%s\n", line);
    //         fclose(keylog);
    //     }
    // }
    // SSL_CTX_set_keylog_callback(ctx, keylog_callback);
    // printf("[TLS] Keylog callback set. Keylog file: %s\n", getenv("SSLKEYLOGFILE") ? getenv("SSLKEYLOGFILE") : "tls_keylog.txt");

    if (SSL_CTX_use_certificate_file(ctx, CERT_FILE, SSL_FILETYPE_PEM) <= 0 || SSL_CTX_use_PrivateKey_file(ctx, KEY_FILE, SSL_FILETYPE_PEM) <= 0) {
        ERR_print_errors_fp(stderr);
        return 1;
    }
#endif

    int server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0) {
        perror("socket()");
        return 1;
    }

    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_port = htons(SERVER_PORT),
        .sin_addr.s_addr = INADDR_ANY
    };

    if (bind(server_sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("bind()");
        return 1;
    }

    listen(server_sock, 10);
    printf("Server listening on port %d...\n", SERVER_PORT);

    while (1) {
        struct sockaddr_in client_addr;
        socklen_t len = sizeof(client_addr);
        int client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &len);

        if (client_sock < 0)
            continue;

#if USE_TLS
        SSL* ssl = SSL_new(ctx);
        SSL_set_fd(ssl, client_sock);
        if (SSL_accept(ssl) <= 0) {
            ERR_print_errors_fp(stderr);
            SSL_free(ssl);
            close(client_sock);
            continue;
        }
        handle_client(client_sock, ssl);
        SSL_shutdown(ssl);
        SSL_free(ssl);
#else
        handle_client(client_sock, NULL);
#endif
        close(client_sock);
    }

#if USE_TLS
    SSL_CTX_free(ctx);
    EVP_cleanup();
#endif

    return 0;
}
