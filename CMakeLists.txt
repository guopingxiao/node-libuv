cmake_minimum_required(VERSION 3.2)

project(LibuvDemo)

include_directories(/Users/xiaopingguo/Libuv/libuv/include)
include_directories(/Users/xiaopingguo/Libuv/libuv)
include_directories(SYSTEM "/foo/bar")

link_directories(
        /Users/xiaopingguo/Libuv/libuv/out/Debug
)
link_libraries(
        uv
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -pthread")

set(HELLO_FILE
        ./src/hello_libuv.c)
set(IDLE_FILE
        ./src/idle.c)
set(FS_FILE
        ./src/fs.c)
set(TCP_FILE2
        ./src/tcpres.c)
set(TCP_FILE
        ./src/tcpserver.c)
set(UDP_FILE
        ./src/udpserver.c)
set(PROCESS_FILE
        ./src/process.c)
set(THREAD_COND
        ./src/thread_cond.c)
set(THREAD_POOL
        ./src/thread_pool.c)
set(THREAD_FILE
        ./src/thread.c)
set(DNS_FILE
        ./src/dns.c)
set(PIPE_FILE
        ./src/pipe/pipe.c)
set(WORKER_FILE
        ./src/pipe/worker.c)
set(EVENT_LOOP
        src/event_loop.cc)
add_executable(HelloUv ${HELLO_FILE})
add_executable(IdleHandle ${IDLE_FILE})
add_executable(FsHandle ${FS_FILE})
add_executable(TcpHandle ${TCP_FILE})
add_executable(TcpHandle2 ${TCP_FILE2})
add_executable(UdpHandle ${UDP_FILE})
add_executable(ProcessHandle ${PROCESS_FILE})
add_executable(ThreadHandle ${THREAD_FILE})
add_executable(ThreadPool ${THREAD_POOL})
add_executable(ThreadCond ${THREAD_COND})
add_executable(DNSHandle ${DNS_FILE})
add_executable(PipeHandle ${PIPE_FILE})
add_executable(WorkerHandle ${WORKER_FILE})
add_executable(EventLoop ${EVENT_LOOP})