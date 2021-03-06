#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "beej_pack.h"
#include "btc_message.h"

ptl_msg * unpack_resp_buf(ptl_resp_buf *resp_buf)
{
    unsigned char *bptr;
    ptl_msg *msg = malloc(sizeof(ptl_msg));
    ptl_payload *pd = malloc(sizeof(ptl_payload));

    bptr = resp_buf -> body;
    beej_unpack(bptr, "<L", &(msg -> magic));
    bptr += 4;

    memcpy(msg -> cmd, bptr, 12);
    bptr += 12;

    beej_unpack(bptr, "<L", &(msg -> len));
    bptr += 4;

    memcpy(msg -> checksum, bptr, 4);
    bptr += 4;

    pd -> len = msg -> len;
    memcpy(pd -> buf, bptr, pd -> len);

    msg -> pld_ptr = pd;
    

    return msg;
}
