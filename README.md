<div align="center">

# 🌐 webserv

**An HTTP/1.1 web server written in C++. From raw sockets to serving web pages.**

![C++](https://img.shields.io/badge/language-C%2B%2B98-orange?style=for-the-badge&logo=cplusplus)
![42](https://img.shields.io/badge/school-42-black?style=for-the-badge)
![Score](https://img.shields.io/badge/score-125%2F100-brightgreen?style=for-the-badge)

</div>

---

## 🧠 What is webserv?

`webserv` is a 42 project where you implement a fully functional **HTTP/1.1 web server** in C++98 — from socket creation and connection handling to request parsing, response generation, CGI execution, and file uploads. Inspired by nginx.

No frameworks. No HTTP libraries. Pure socket programming.

---

## ✅ Features

### Core HTTP
- `GET`, `POST`, `DELETE` methods
- HTTP/1.1 compliant responses with proper status codes
- Persistent connections (`Connection: keep-alive`)
- Custom error pages

### Configuration File (nginx-style)
```nginx
server {
    listen 8080;
    server_name localhost;
    root ./www;
    index index.html;

    location / {
        allowed_methods GET POST;
        autoindex on;
    }

    location /upload {
        allowed_methods POST;
        upload_store ./uploads;
        client_max_body_size 10M;
    }

    location /cgi-bin {
        cgi_pass .py;
        cgi_pass .php;
    }

    error_page 404 /errors/404.html;
}
```

### CGI Support
- Execute `.py`, `.php`, `.sh` scripts
- Environment variables passed correctly (`QUERY_STRING`, `REQUEST_METHOD`, etc.)
- Response piped back to client

### Multiplexing
- **Non-blocking I/O** with `select()` / `poll()` / `epoll()`
- Handles multiple simultaneous clients without threads

---

## 🚀 Usage

```bash
git clone https://github.com/Houdaifa1/webserv
cd webserv
make
./webserv config/default.conf
```

Test it:

```bash
curl http://localhost:8080/
curl -X POST http://localhost:8080/upload -F "file=@test.txt"
curl -X DELETE http://localhost:8080/files/test.txt
```

---

## 🔧 Build

```bash
make        # Compile
make clean  # Remove objects
make fclean # Full cleanup
make re     # Rebuild
```

---

## 📁 Architecture

```
webserv/
├── src/
│   ├── server/       # Socket setup, main event loop
│   ├── http/         # Request parser, response builder
│   ├── config/       # Configuration file parser
│   ├── cgi/          # CGI handler
│   └── utils/        # String utils, MIME types, etc.
├── config/           # Example config files
├── www/              # Default web root
└── Makefile
```

---

## 📊 HTTP Status Codes Handled

`200 OK` | `201 Created` | `204 No Content` | `301 Moved` | `400 Bad Request` | `403 Forbidden` | `404 Not Found` | `405 Method Not Allowed` | `413 Payload Too Large` | `500 Internal Server Error`

---

<div align="center">
<i>nginx didn't write itself. Now you know why that's impressive.</i>
</div>
