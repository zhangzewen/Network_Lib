

all:
	
	@ for dir in $(shell ls) do if [ -d $(dir) ];then make -C ${dir} if done


