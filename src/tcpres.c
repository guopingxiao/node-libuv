/**
基于TCP的回显程序，即客户端传输什么内容，服务器就返回什么内容。

*/
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <uv.h>

#define CHECK(ret, msg) if (ret) \
{                                                       \
  printf("%s: [%s(%d): %s]\n", msg, uv_err_name((ret)), (int) ret, uv_strerror((ret))); \
  exit(1);                                                                           \
}

const static char* HOST  = "0.0.0.0"; // 使用4个0即可，不需要写具体的IP
const static int   PORT  = 12345;
const static int   NBUFS = 1; /* number of buffers we write at once */

// tcp连接句实例
static uv_tcp_t tcpServer;

typedef struct
{
    uv_write_t req;
    uv_buf_t buf;
} write_req_t;

static void OnClose(uv_handle_t* client)
{
    free(client);
    printf("Closed connection\n");
}

// 断开连接时会调用到此函数
// 注意，并不是退出服务端程序
static void OnShutdown(uv_shutdown_t* req, int status)
{
    printf("On Shutdown...\n");
    uv_close((uv_handle_t*)req->handle, OnClose);
    free(req);
}

static void OnAlloc(uv_handle_t *handle, size_t size, uv_buf_t *buf)
{
    /* libuv suggests a buffer size but leaves it up to us to create one of any size we see fit */
    buf->base = malloc(size);
    buf->len = size;
    if (buf->base == NULL)
    {
        printf("OnAlloc buffer didn't properly initialize\n");
    }
}

// 注：OnAfterWrite仅是做一些释放工作，真正发送数据是在uv_write完成的
static void OnAfterWrite(uv_write_t *req, int status)
{
    CHECK(status, "OnAfterWrite");
    
    printf("Replied to client\n");
    
    /* Since the req is the first field inside the wrapper write_req, we can just cast to it */
    /* Basically we are telling C to include a bit more data starting at the same memory location, which in this case is our buf */
    write_req_t *write_req = (write_req_t*)req;
    free(write_req->buf.base);
    free(write_req);
}

static void OnRead(uv_stream_t* client, ssize_t nread, const uv_buf_t* buf)
{
    int ret = 0;
    uv_shutdown_t *shutdown_req;
    
    printf("OnRead: nread: %ld\n", nread);
    /* Errors or EOF */
    if (nread < 0)
    {
        if (nread != UV_EOF)
            CHECK(nread, "OnRead");
        
        /* Client signaled that all data has been sent, so we can close the connection and are done */
        free(buf->base);
        
        shutdown_req = malloc(sizeof(uv_shutdown_t));
        ret = uv_shutdown(shutdown_req, client, OnShutdown);
        CHECK(ret, "uv_shutdown");
        return;
    }
    
    if (nread == 0)
    {
        /* Everything OK, but nothing read and thus we don't write anything */
        free(buf->base);
        return;
    }
    
    /* Check if we should quit the server which the client signals by sending "QUIT" */
    if (!strncmp("QUIT", buf->base, fmin(nread, 4)))
    {
        printf("Closing the server\n");
        free(buf->base);
        /* Before exiting we need to properly close the server via uv_close */
        /* We can do this synchronously */
        uv_close((uv_handle_t*) &tcpServer, NULL);
        printf("Closed server, exiting\n");
        exit(0);
    }
    
    /* 6. Write same data back to client since we are an *echo* server and thus can reuse the buffer used to read*/
    /*    We wrap the write req and buf here in order to be able to clean them both later */
    write_req_t *write_req = malloc(sizeof(write_req_t));
    write_req->buf = uv_buf_init(buf->base, nread);
    ret = uv_write(&write_req->req, client, &write_req->buf, NBUFS, OnAfterWrite);

    CHECK(ret, "uv_write");
}

static void ShowSockname(struct sockaddr* addr)
{
    struct sockaddr_in* check_addr = (struct sockaddr_in*)addr;
    char check_ip[17];
    int r;

    r = uv_ip4_name(check_addr, (char*) check_ip, sizeof check_ip);

    printf("accepted from: %s:%d\n", check_ip, ntohs(check_addr->sin_port));
}

static void OnConnection(uv_stream_t* server, int status)
{
    static int cnt = 1;
    int ret = 0;
    uv_shutdown_t* shutdown_req;

    struct sockaddr sockname, peername;
    int namelen = sizeof(struct sockaddr);

    CHECK(status, "OnConnection");

    /* 4. Accept client connection */
    printf("Accepting Connection\n");
    
    /* 4.1. Init client connection using `server->loop`, passing the client handle */
    uv_tcp_t *client = malloc(sizeof(uv_tcp_t));
    ret = uv_tcp_init(server->loop, client);

    CHECK(ret, "uv_tcp_init");
    
    /* 4.2. Accept the now initialized client connection */
    ret = uv_accept(server, (uv_stream_t*)client);
    if (ret)
    {
      printf("trying to accept connection %d\n", ret);
    
      shutdown_req = malloc(sizeof(uv_shutdown_t));
      ret = uv_shutdown(shutdown_req, (uv_stream_t*) client, OnShutdown);
      CHECK(ret, "uv_shutdown");
    }
    
    // 获取对端IP地址，并打印之
    ret = uv_tcp_getpeername(client, &peername, &namelen);
    if (ret != 0)
    {
        printf("uv_tcp_getpeername failed.\n");
        return;
    }

    ShowSockname(&peername);

    printf("Connection accepted, cnt: %d\n", cnt++);

    /* 5. Start reading data from client */
    ret = uv_read_start((uv_stream_t *)client, OnAlloc, OnRead);
    CHECK(ret, "uv_read_start");
}

int main(void)
{
    int ret = 0;
    
    // 创建loop实例
    uv_loop_t *loop = uv_default_loop();
    
    // 1、初始化TCP
    ret = uv_tcp_init(loop, &tcpServer);
    CHECK(ret, "uv_tcp_init");
    
    // 2、绑定IP地址和端口
    struct sockaddr_in addr;
    ret = uv_ip4_addr(HOST, PORT, &addr);
    CHECK(ret, "uv_ip4_addr");
    
    ret = uv_tcp_bind(&tcpServer, (struct sockaddr*)&addr, AF_INET);
    CHECK(ret, "uv_tcp_bind");
    
    // 3、监听端口
    // 注：uv_tcp_t继承于uv_stream_t（前面的结构成员完全一样），可以直接强制转换
    // 内部调用uv_tcp_listen->uv__handle_start
    // SOMAXCONN可设置其它值，默认128
    ret = uv_listen((uv_stream_t*) &tcpServer, SOMAXCONN, OnConnection);
    CHECK(ret, "uv_listen");
    
    printf("Listening on %s:%d\n", HOST, PORT);
    
    // 4、运行循环体，内部使用for死循环，根据epoll轮询，
    // 直到在异常退出
    uv_run(loop, UV_RUN_DEFAULT);

    return 0;
}