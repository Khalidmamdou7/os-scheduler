build:
	mkdir -p build
	mkdir -p Logs
	rm -f *.out build/* Logs/* *.log *.perf
	gcc process_generator.c headers.c ./DataStructures/Queue.c ./ipc/MsgQueue.c -o ./build/process_generator.out
	gcc clk.c headers.c -o ./build/clk.out
	gcc scheduler.c headers.c ./ipc/MsgQueue.c ./ProcessStructs/PCB.c ./DataStructures/Queue.c -o ./build/scheduler.out
	gcc process.c headers.c -o ./build/process.out
	gcc test_generator.c -o ./build/test_generator.out
	gcc ./schedulers/scheduler-sjf.c headers.c ./ipc/MsgQueue.c ./DataStructures/PriorityQueue.c ./ProcessStructs/PCB.c ./DataStructures/Tree.c ./memory/memory.c ./schedulers/scheduler-utils.c ./utils/Logger.c -o ./build/scheduler-sjf.out
	gcc ./schedulers/scheduler-hpf.c headers.c ./ipc/MsgQueue.c ./DataStructures/PriorityQueue.c ./ProcessStructs/PCB.c ./DataStructures/Tree.c ./memory/memory.c ./schedulers/scheduler-utils.c ./utils/Logger.c -o ./build/scheduler-hpf.out
	gcc ./schedulers/scheduler-rr.c headers.c ./ipc/MsgQueue.c ./DataStructures/Queue.c ./ProcessStructs/PCB.c ./DataStructures/Tree.c ./memory/memory.c ./schedulers/scheduler-utils.c ./utils/Logger.c -o ./build/scheduler-rr.out
	gcc ./schedulers/scheduler-mlf.c headers.c ./ipc/MsgQueue.c ./DataStructures/Queue.c ./ProcessStructs/PCB.c ./DataStructures/Tree.c ./memory/memory.c ./schedulers/scheduler-utils.c -o ./build/scheduler-mlf.out

clean:
	rm -f *.out  processes.txt build/* Logs/* *.log *.perf

all: clean build

run:
	./build/test_generator.out
	./build/process_generator.out processes.txt -sch 1