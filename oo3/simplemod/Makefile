obj-m += simple.o
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	sudo rmmod simple
	sudo insmod simple.ko
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
