openssl req -x509 -nodes -days 365 -newkey rsa:2048 \
  -keyout server.key -out server.crt \
  -subj "/C=CN/ST=Test/L=Test/O=Test/OU=Test/CN=localhost"

