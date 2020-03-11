target = proc_read proc_pread proc_write \
		 proc_pwrite thread_write thread_pwrite \
		 thread_write_after thread_read thread_pread \
		 proc_read_single proc_read_twice

all: clean
	g++ proc_read.cpp -o proc_read
	g++ proc_pread.cpp -o proc_pread
	g++ proc_write.cpp -o proc_write
	g++ proc_pwrite.cpp -o proc_pwrite
	g++ proc_read_single.cpp -o proc_read_single
	g++ proc_pread_single.cpp -o proc_pread_single
	g++ proc_read_twice.cpp -o proc_read_twice
	g++ thread_read.cpp -o thread_read -lpthread
	g++ thread_pread.cpp -o thread_pread -lpthread
	g++ thread_write.cpp -o thread_write -lpthread
	g++ thread_pwrite.cpp -o thread_pwrite -lpthread
	g++ thread_write_after.cpp -o thread_write_after -lpthread

.PHONY:
	clean

clean:
	-rm ${target}

