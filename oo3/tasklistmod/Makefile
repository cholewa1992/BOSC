obj-m += tasklist.o
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
install:	
	sudo rmmod tasklist
	sudo insmod tasklist.ko
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
