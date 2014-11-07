obj-m += tasklist.o
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
remove:	
	sudo rmmod tasklist
install:
	sudo insmod tasklist.ko
reinstall:
	sudo rmmod tasklist
	sudo insmod tasklist.ko
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
