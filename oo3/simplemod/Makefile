obj-m += simple.o
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
remove:	
	sudo rmmod simple
install:
	sudo insmod simple.ko
reinstall:
	sudo rmmod simple
	sudo insmod simple.ko
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
