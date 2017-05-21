#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>

#include "redis_op.h"
#include "fdfs_api.h"
#include "make_log.h"
#define ROP_MODULE "test"
#define ROP_PROC "gotofdfd_redis"

int main(int argc, const char* argv[])
{
    
    char fileid[1024] = {0};
    int ret = 0;
    //const char *filename = "testfile.txt";
    char filename[1024] = {"testfile.txt"};
    ret = fdfs_upload_by_filename("testfile.txt", fileid);
    if(ret != 0){
        printf("upload error\n");
        return 0;
    }
    printf("fileid = %s\n", fileid);
    
    struct stat buf_st;
    ret = stat(filename, &buf_st);
    if(ret == -1){
        perror("stat");
        exit(1);
    }
    printf("%s  %d  %d  %d\n", filename, (int)buf_st.st_ctime, (int)buf_st.st_uid, 1);
    char crtime[64] = {0};
    char uid[64] = {0};
    char downnum[10] = {0};
    sprintf(crtime, "%d", buf_st.st_ctime);
    sprintf(uid, "%d", buf_st.st_uid);
    sprintf(downnum, "%d", 1);
    struct tm *p = localtime(&buf_st.st_ctime);
    char timebuf[64] = {0};
    sprintf(timebuf,"%d-%d-%d %d:%d:%d", p->tm_year, p->tm_mon, p->tm_mday, p->tm_hour,p->tm_min, p->tm_sec);
    
    
    redisContext* conn = NULL;
    conn = rop_connectdb_nopwd("127.0.0.1", "6379");
    if(conn == NULL){
        LOG(ROP_MODULE, ROP_PROC, "conn db error");
        ret = -1;
        goto END;
    }


   ret = rop_hset_hashes(conn,"myhashes1", "filename", filename);
   if(ret != 0){
       // LOG(ROP_MODULE, ROP_PROC, "hset error");
        printf("hset error\n");
        goto END;
   }
   ret = rop_hset_hashes(conn,"myhashes1", "filecrtime", timebuf);
   ret = rop_hset_hashes(conn,"myhashes1", "fileuid", uid);
   ret = rop_hset_hashes(conn,"myhashes1", "fdown", downnum);

     ret = rop_list_push(conn, "myapilistha", "myhashes1");
    if(ret == -1){
        //LOG(ROP_MODULE, ROP_PROC, "listpush error");
        printf("lpush error\n");
        goto END;
    }
END: 
    return ret;
}
