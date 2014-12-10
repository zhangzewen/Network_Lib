DIR=$(shell pwd)
UTIL_DIR=$(DIR)/util
REACTOR_DIR=$(DIR)/Reactor

.PHONY: all clean

all:
	@for dir in `ls`;\
	do if [ -d $${dir} ];\
	then make -C $${dir};\
	fi;\
	done;

clean:
	@for dir in `ls`;\
	do if [ -d $${dir} ];\
	then make clean -C $${dir} --no-print-directory;\
	fi;\
	done;

