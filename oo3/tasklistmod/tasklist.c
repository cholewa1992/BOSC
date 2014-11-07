#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/printk.h>
#include <linux/sched.h>

void dfs(struct task_struct *tsk, struct list_head *list){
	struct task_struct *task;
	list_for_each(list, &tsk->children){
		task = list_entry(list, struct task_struct, sibling);
		printk(KERN_INFO "%d %s\n", task->pid, task->comm);
		dfs(task,list);
	}
}

/* This function is called when the module is loaded. */
int simple_init(void)
{
		printk(KERN_INFO "Loading Module\n");
		struct list_head *list;
		dfs(&init_task, list);		

	return 0;
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

