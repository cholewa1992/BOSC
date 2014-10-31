#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/types.h>
#include <linux/printk.h>
#include <linux/slab.h>


typedef struct birthday {
	int day;
	int month;
	int year;
	struct list_head list;
} Birthday;

//struct list_head birthday_list;
static LIST_HEAD(birthday_list);

/* This function is called when the module is loaded. */
int simple_init(void)
{
		printk(KERN_INFO "Loading Module\n");
		
		

		int i;
		
		for (i = 1; i <= 5; i++){
			Birthday *person;
			person = kmalloc(sizeof(*person), GFP_KERNEL);
			person->day = i;
			person->month= 8;
			person->year = 1995;
			INIT_LIST_HEAD(&person->list);
			list_add_tail(&person->list, &birthday_list);
			printk(KERN_INFO "added\n");
		}
		
		// for each loop over linked list
		Birthday *p;
		list_for_each_entry(p, &birthday_list, list) {
			printk(KERN_INFO "%d-%d-%d\n", p->day, p->month, p->year);
		}
		
	   
		return 0;
}

/* This function is called when the module is removed. */
void simple_exit(void) {
	printk(KERN_INFO "Removing Module\n");
	
	Birthday *ptr, *next;
	list_for_each_entry_safe(ptr,next,&birthday_list,list) {
		/* on each iteration ptr points */
		/* to the next birthday struct */
		list_del(&ptr->list);
		printk(KERN_INFO "removed elm %d\n", ptr->day);
		kfree(ptr);
	}
}

/* Macros for registering module entry and exit points. */
module_init( simple_init );
module_exit( simple_exit );

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple Module");
MODULE_AUTHOR("SGG");

