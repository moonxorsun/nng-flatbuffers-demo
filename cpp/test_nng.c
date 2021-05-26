#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "nng/nng.h" 
#include "nng/protocol/pubsub0/pub.h"

void test_nng() {
    nng_socket sid;
    int ret = nng_pub_open(&sid);
    if (0 != ret) {
        printf("[test] nng_pub_open ret error! ret: %d\n", ret);
        return;
    }
    ret = nng_listen(sid, "tcp://*:5678", NULL, 0);
    if (0 != ret) {
        printf("[test] nng_listen ret error! ret: %d\n", ret);
        return;
    }
    char buf[8];
    size_t len = sizeof(buf);
    while (1) {
        ret = nng_recv(sid, buf, &len, 0);
        if (0 != ret) {
            printf("[test] nng_recv ret error! ret: %d\n", ret);
            return;
        } else {
            char str[9] = { '\0' };
            memcpy(str, buf, len);
            printf("[test] nng recv str: %s\n", str);
        }
    }
}

int main() {
    printf("hello nng\n");
    return 0;
}
