CC=gcc
CPPFLAGS= -I./include -I/usr/include/fastdfs -I/usr/include/fastcommon -I/usr/local/include/hiredis -I./include
CFLAGS=-Wall 
LIBS= -lfdfsclient -lfastcommon -lhiredis

main=./main
hiredis_test=./test/hiredis_test
redis_op_test=./test/redis_op_test
gotofdfs_redis=./test/gotofdfs_redis
target=$(main) $(hiredis_test) $(redis_op_test) $(gotofdfs_redis)


ALL:$(target)


#生成所有的.o文件
%.o:%.c
	$(CC) -c $< -o $@ $(CPPFLAGS) $(CFLAGS) 


#main程序
$(main):./main.o ./fdfs_api.o make_log.o
	$(CC) $^ -o $@ $(LIBS)
#test程序
$(hiredis_test):./test/hiredis_test.o 
	$(CC) $^ -o $@ $(LIBS)

$(redis_op_test):./test/redis_op_test.o redis_op.o make_log.o 
	$(CC) $^ -o $@ $(LIBS)
$(gotofdfs_redis):./test/gotofdfs_redis.o fdfs_api.o  redis_op.o make_log.o 
	$(CC) $^ -o $@ $(LIBS)
#clean指令

clean:
	-rm -rf ./*.o $(target) ./test/*.o

distclean:
	-rm -rf ./*.o $(target) ./test/*.o

#将clean目标 改成一个虚拟符号
.PHONY: clean ALL distclean
