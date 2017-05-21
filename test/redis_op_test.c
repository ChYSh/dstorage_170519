#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

#include "redis_op.h"
#include "make_log.h"

#define ROP_MODULE "test"
#define ROP_PROC "redis_op_test"

int main(int argc, const char* argv[])
{
    int ret = 0;
    
    redisContext* conn = NULL;
    conn = rop_connectdb_nopwd("127.0.0.1", "6379");
    if(conn == NULL){
        LOG(ROP_MODULE, ROP_PROC, "conn db error");
        ret = -1;
        goto END;
    }


    ret = rop_list_push(conn, "myapilist", "hellocpp");
    if(ret == -1){
        LOG(ROP_MODULE, ROP_PROC, "listpush error");
        printf("lpush error\n");
        goto END;
    }

   ret = rop_hset_hashes(conn,"myhashes", "ha1", "nihaoworld");
   if(ret != 0){
        LOG(ROP_MODULE, ROP_PROC, "hset error");
        printf("hset error\n");
        goto END;
   }
    char hvalue[1024] = {0};
    ret =  rop_hget_hashes(conn,"myhashes", "ha1",  hvalue);
    if(ret != 0){
        LOG(ROP_MODULE, ROP_PROC, "hget error");
        printf("hget error\n");
        goto END; 
    }
    printf("hashes = %s\n", hvalue);
    if(conn != NULL){
         rop_disconnect(conn);
    }
END:
    return ret;
}
