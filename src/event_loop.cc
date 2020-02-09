#include <iostream>
#include "uv.h"
using namespace std;

void idle_callback(uv_idle_t* idle);
void prepare_callback(uv_prepare_t* prepare);
void check_callback(uv_check_t* check);

#define RUN_HANDLE(type) \
do {    \
uv_##type##_t type;    \
uv_##type##_init(loop, &type);    \
uv_##type##_start(&type, type##_callback);    \
} while(0)

#define CALLBACK(type)  \
do {    \
cout << "Run " << #type << " handles" << endl;   \
uv_##type##_stop(type);    \
} while(0)

#define OPEN(PATH, callback) \
do {    \
uv_fs_t req;    \
uv_fs_open(loop, &req, PATH, O_RDONLY, 0, callback); \
uv_fs_req_cleanup(&req);    \
} while(0)

void idle_callback(uv_idle_t* idle) { CALLBACK(idle); }
void prepare_callback(uv_prepare_t* prepare) { CALLBACK(prepare); }
void check_callback(uv_check_t* check) { CALLBACK(check); }
void on_open(uv_fs_t* req) { cout << "poll for I/O" << endl; }

int main(int argc, const char * argv[]) {
    auto loop = uv_default_loop();

    RUN_HANDLE(check);
    RUN_HANDLE(prepare);
    RUN_HANDLE(idle);

    OPEN("/Users/feilongpang/workspace/i.js", on_open);

    uv_run(loop, UV_RUN_DEFAULT);
    uv_loop_close(loop);
    return 0;
}