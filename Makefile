MultiThread: MultiThread.c
	gcc MultiThread.c -o NoSync -pthread
	gcc MultiThread.c -o Sync -D SYNC -pthread
		
