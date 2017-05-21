#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

#include "hiredis.h"

int main(int argc, const char* argv[])
{
    redisContext *conn = redisConnect("127.0.0.1", 6379);
    if (conn == NULL || conn->err) {
        if (conn) {
            printf("Error: %s\n", conn->errstr);
        } else {
            printf("Can't allocate redis context\n");            
        }

    }
    redisReply *reply = NULL;
    reply = redisCommand(conn, "SET foo100 %s", "nihaoshijie");
    if(reply == NULL){
        printf("set error\n");
        goto END;
    }
    printf("set foo100 success\n");
    freeReplyObject(reply);
    
    reply = redisCommand(conn, "GET foo100");
    if(reply == NULL){
        printf("set error\n");
        goto END;
    }
    printf("len = %d, string = %s\n", (int)reply->len, reply->str);
    freeReplyObject(reply);
    
    char *list_key = "my_test_list";
    int list_len = 0;
    reply = redisCommand(conn, "lpush %s %s", list_key, "cpp");
    if(reply == NULL){
        printf("lpush error\n");
        goto END;
    }
    
    reply = redisCommand(conn, "lpush %s %s", list_key, "python");
    if(reply == NULL){
        printf("lpush error\n");
        goto END;
    }
    list_len = reply->integer;
    printf("list_len = %d\n", list_len);

    

END:
    if(conn != NULL){
        redisFree(conn);
    }
    return 0;
}
