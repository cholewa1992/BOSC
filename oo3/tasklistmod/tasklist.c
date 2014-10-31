#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/printk.h>
#include <linux/sched.h>


struct task_struct *task;

/* This function is called when the module is loaded. */
int simple_init(void)
{
		printk(KERN_INFO "Loading Module\n");
		
			
		
		struct task_struct *task;
		struct list_head *list;
		struct task_struct init_task;
		
		task = &init_task;
		
		dfs:
		printk(KERN_INFO "%d %s\n", task->pid, task->comm);
		list_for_each(list, task->children) {
		task = list_entry(list, struct task_struct, sibling);
		if(task != null){
			goto dfs;
		}
		if(sibling != null){
			task = sibling;
			goto dfs;
		}
	}
		
		
		
		return 0;
}

void dfs(task_struct *task, task_struct *list){


}

/* This function is called when the module is removed. */
void simple_exit(void) {
	printk(KERN_INFO "Removing Module\n");
	
}

/* Macros for registering module entry and exit points. */
module_init( simple_init );
module_exit( simple_exit );

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple Module");
MODULE_AUTHOR("SGG");

