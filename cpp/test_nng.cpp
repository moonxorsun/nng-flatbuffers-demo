#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>

#include <atomic>
#include <thread>

#include "nng/nng.h" 
#include "nng/protocol/pubsub0/pub.h"
#include "nng/protocol/pubsub0/sub.h"
#include "nng/protocol/pair0/pair.h"

static std::atomic_bool sub_finished(false);
static std::atomic_bool pub_finished(false);

// static const char* kUrl = "inproc://test_nng";
static const char* kUrl = "tcp://127.0.0.1:4567";

void test_nng_sub() {
    nng_socket sid;
    int ret = nng_sub_open(&sid);
    do {
        if (0 != ret) {
            printf("[test] nng_sub_open ret error! ret: %d\n", ret);
            break;
        }
        ret = nng_dial(sid, kUrl, NULL, 0);
        if (0 != ret) {
            printf("[test] nng_dial ret error! ret: %d\n", ret);
            break;
        }

        char* buf = NULL;
        size_t len = 0;
        size_t count = 10;
        while (count--) {
            printf("[debug] nng sub\n");
            ret = nng_recv(sid, &buf, &len, NNG_FLAG_NONBLOCK | NNG_FLAG_ALLOC);
            if (0 != ret) {
                printf("[test] nng_recv ret error! ret: %d\n", ret);
                //return;
            } else {
                char str[20] = { '\0' };
                memcpy(str, buf, len);
                printf("[test] nng recv str: %s\n", str);
                nng_free(buf, len);
            }
            sleep(1);
        }
    } while(0);
    nng_close(sid);
    sub_finished = true;
}

void test_nng_pub() {
    nng_socket sid;
    int ret = nng_pub_open(&sid);
    do {
        if (0 != ret) {
            printf("[test] nng_pub_open ret error! ret: %d\n", ret);
            return;
        }
        ret = nng_listen(sid, kUrl, NULL, 0);
        if (0 != ret) {
            printf("[test] nng_listen ret error! ret: %d\n", ret);
            return;
        }

        const char* buf = "hello nng";
        size_t len = strlen(buf) + 1;
        size_t count = 10;
        while (count--) {
            printf("[debug] nng pub\n");
            ret = nng_send(sid, (void*)buf, len, 0);
            if (0 != ret) {
                printf("[test] nng_send ret error! ret: %d\n", ret);
                //return;
            }
            sleep(1);
        }
    } while(0);
    nng_close(sid);
    pub_finished = true;
}

int main() {
    printf("hello nng\n");
    // test_nng_sub();
    std::thread thread_pub(test_nng_pub);
    printf("start pub\n");
    thread_pub.detach();
    // sleep(5);
    printf("start sub\n");
    std::thread thread_sub(test_nng_sub);
    thread_sub.detach();
    
    while (!sub_finished || !pub_finished) {
        sleep(1);
    }

    return 0;
}
