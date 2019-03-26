CFLAGS = -g -Wall -I./include

OBJS:= src/main.o src/parser.o src/descriptors.o src/os.o src/tasks.o
OBJS_FIFO:= ${OBJS} src/sched_fifo.o
OBJS_PRIO:= ${OBJS} src/sched_prio.o
OBJS_RR:= ${OBJS} src/sched_rr.o

all: schedsim_fifo schedsim_prio schedsim_rr

./lib/libjsmn.a: ./lib/jsmn.o
	ar rc $@ $^

schedsim_fifo: ${OBJS_FIFO} ./lib/libjsmn.a
	gcc ${CFLAGS} -o schedsim_fifo ${OBJS_FIFO} -L./lib -ljsmn

schedsim_prio: ${OBJS_PRIO} ./lib/libjsmn.a
	gcc ${CFLAGS} -o schedsim_prio ${OBJS_PRIO} -L./lib -ljsmn

schedsim_rr: ${OBJS_RR} ./lib/libjsmn.a
	gcc ${CFLAGS} -o schedsim_rr ${OBJS_RR} -L./lib -ljsmn

clean:
	@rm -rf ${OBJS_FIFO} ${OBJS_PRIO} ${OBJS_RR} ./lib/libjsmn.a ./lib/jsmn.o
	@rm -rf schedsim_fifo schedsim_prio schedsim_rr
