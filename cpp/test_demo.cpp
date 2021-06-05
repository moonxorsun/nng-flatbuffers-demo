#include <stdio.h>
#include <stdlib.h>
#include <nng/nng.h>
#include <nng/protocol/pubsub0/pub.h>
#include <nng/protocol/pubsub0/sub.h>
#include <nng/transport/ipc/ipc.h>
#include <nng/supplemental/util/platform.h>
#include <unistd.h>
#include <time.h>

nng_socket pubs;
nng_socket subs;

void subloop(void *arg)
{
    int rv;
    for (;;) {
        char *buf;
        size_t sz;
        time_t now;
        rv = nng_recv(subs, &buf, &sz, NNG_FLAG_NONBLOCK | NNG_FLAG_ALLOC);

        switch (rv) {
        case NNG_EAGAIN:
            now = time(NULL);
            printf("NNG_EAGAIN %s", ctime(&now));
            printf("GOT ERROR %s\n", nng_strerror(rv));
            break;
        case 0:
            printf("GOT message\n");
            nng_free(buf, sz);
            break;
        default:
            printf("GOT ERROR %s\n", nng_strerror(rv));
            return;
        }
        usleep(3);
    }
}

int main(int argc, char **argv)
{
    nng_thread *tid;

    if ((nng_sub0_open(&subs) != 0) ||
        (nng_pub0_open(&pubs) != 0)) {
        exit(1);
    }


    nng_setopt_int(subs, NNG_OPT_RECVBUF, 100);
    nng_setopt_ms(subs, NNG_OPT_RECONNMINT, 1000);
    nng_setopt_ms(subs, NNG_OPT_RECONNMAXT, 0);
    
    nng_setopt(subs, NNG_OPT_SUB_SUBSCRIBE, "", 0);

    nng_listen(pubs, "ipc:///tmp/ipctest", NULL, 0);
    nng_dial(subs, "ipc:///tmp/ipctest", NULL, 0);

    nng_thread_create(&tid, subloop, NULL);

    sleep(60);

    nng_close(subs);
}