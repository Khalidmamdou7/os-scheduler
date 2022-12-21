build:
	mkdir -p build
	gcc process_generator.c ./DataStructures/Queue.c ./ipc/MsgQueue.c -o ./build/process_generator.out
	gcc clk.c -o ./build/clk.out
	gcc scheduler.c ./ipc/MsgQueue.c ./ProcessStructs/PCB.c ./DataStructures/Queue.c -o ./build/scheduler.out
	gcc process.c -o ./build/process.out
	gcc test_generator.c -o ./build/test_generator.out

clean:
	rm -f *.out  processes.txt build/*

all: clean build

run:
	./build/test_generator.out
	./build/process_generator.out processes.txt -sch 1