#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/printk.h>
#include <linux/sched.h>

void dfs(struct task_struct *tsk, struct list_head *ptr){
	//Struct used for holding current task
	struct task_struct *task;

	//For each children of the tsk task
	list_for_each(ptr, &tsk->children){ 
			
		//Gets a pointer to the next element
		task = list_entry(ptr, struct task_struct, sibling); 

		//Prints information about the task
		printk(KERN_INFO "%d %s\n", task->pid, task->comm);

		//Call resurcivly on the current task
		dfs(task,ptr);
	}
}

/* This function is called when the module is loaded. */
int tasklist_init(void)
{
	//Printing to the kernel info output that we're loading the module
	printk(KERN_INFO "Loading Module\n");

	//Defining a list_head struct to use as a loop cursor
	struct list_head *ptr;

	//Call the DFS algorithm on the init_task (The mother of all tasks)
	dfs(&init_task, ptr);		

	return 0;
}


/* This function is called when the module is removed. */
void tasklist_exit(void) {
	printk(KERN_INFO "Removing Module\n");
	
}

/* Macros for registering module entry and exit points. */
module_init( tasklist_init );
module_exit( tasklist_exit );

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Tasklist Module");
MODULE_AUTHOR("SGG");

