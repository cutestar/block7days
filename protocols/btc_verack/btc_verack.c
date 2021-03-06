#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <openssl/sha.h>

#include "btc_message.h"
#include "kyk_sha.h"
#include "beej_pack.h"
#include "kyk_socket.h"

void build_btc_message(ptl_msg * msg, const char *cmd, ptl_payload *pld);
size_t print_hex(const unsigned char *buf, size_t len, int width, char *note);
void pack_btc_message(ptl_msg_buf *msg_buf, ptl_msg *msg);
void print_msg_buf(const ptl_msg_buf *msg_buf);

int main(void)
{
    ptl_msg msg;
    ptl_payload pld;
    ptl_msg *resp_msg;
    ptl_msg_buf msg_buf;
    ptl_resp_buf resp_buf;
    ptl_msg_buf resp_msg_buf;

    msg.len = 0;
    pld.len = 0;
    build_btc_message(&msg, "verack", &pld);
    pack_btc_message(&msg_buf, &msg);

    kyk_send_btc_msg_buf("seed.bitcoin.sipa.be", "8333", &msg_buf, &resp_buf);
    
    resp_msg = unpack_resp_buf(&resp_buf);
    pack_btc_message(&resp_msg_buf, resp_msg);

    printf("=======> Request Buf:\n");
    print_msg_buf(&msg_buf);
    printf("=======> Response Buf:\n");
    print_msg_buf(&resp_msg_buf);
	
}

void print_msg_buf(const ptl_msg_buf *msg_buf)
{
    const unsigned char *buf = msg_buf -> body;
    size_t len = 0;
    int wth = 36;

    len = print_hex(buf, sizeof(uint32_t), wth, "Start String: Mainnet");
    buf += len;
    len = print_hex(buf, 12, wth, "Command name");
    printf("Command name: %s\n", buf);
    buf += len;
    len = print_hex(buf, sizeof(uint32_t), wth, "Payload size");
    buf += len;

    len = print_hex(buf, 4, wth, "Checksum");
    buf += len;

    len = msg_buf -> len - 24;
    len = print_hex(buf, len, wth, "Payload");
    buf += len;

    for(int i=0; i < msg_buf -> len; i++){
	printf("%02x", msg_buf -> body[i]);
    }

    printf("\n");
}

size_t print_hex(const unsigned char *buf, size_t len, int width, char *note)
{
    for(int i=0; i < len; i++){
	printf("%02x", *buf++);
    }

    printf(" ");
    for(int j=len*2; j < width; j++){
	printf(".");
    }

    printf(" %s\n", note);

    return len;
}

void build_btc_message(ptl_msg * msg, const char *cmd, ptl_payload *pld)
{
    unsigned char *dg2;
    
    msg -> magic = NT_MAGIC_MAIN;
    strcpy(msg -> cmd, cmd);
    msg -> len = pld -> len;
    msg -> pld_ptr = pld;
    dg2 = kyk_dble_sha256((char *)pld -> buf, (size_t)pld -> len);
    memcpy(msg -> checksum, dg2, 4);
}

void pack_btc_message(ptl_msg_buf *msg_buf, ptl_msg *msg)
{
    unsigned char *buf = msg_buf -> body;
    size_t size=0;
    
    msg_buf -> len = 0;
    size = beej_pack(buf, "<L", msg -> magic);
    msg_buf -> len += size;
    buf += size;

    size = sizeof(msg -> cmd);
    memcpy(buf, msg -> cmd, size);
    msg_buf -> len += size;
    buf += size;
    
    size = beej_pack(buf, "<L", (msg -> pld_ptr) -> len);
    msg_buf -> len += size;
    buf += size;

    size = sizeof(msg -> checksum);
    memcpy(buf, msg -> checksum, size);
    msg_buf -> len += size;
    buf += size;

    size = msg -> pld_ptr -> len;
    memcpy(buf, msg -> pld_ptr -> buf, size);
    msg_buf -> len += size;
    buf += size;
    
}


