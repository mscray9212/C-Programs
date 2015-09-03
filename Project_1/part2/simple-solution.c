#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/slab.h>

struct birthday *person,*ptr, *next;
static LIST_HEAD(birthday_list);

struct birthday 
{	
	int month;
	int day;
	int year;
    char *name;
    struct list_head list;
};

/**
 * The following defines and initializes a list_head object named birthday_list
 */

int simple_init(void)
{
   
    /* Create a linked list containing five struct birthday elements*/
    /* NOTE:THE NAME OF FIRST STRUCT BIRTHDAY SHOULD BE YOUR OWN NAME*/
    person = kmalloc(sizeof(*person), GFP_KERNEL);
	person->name="Michael Scray";
	person->month=9;
	person->day=19;
	person->year=1987;
	INIT_LIST_HEAD(&person->list);
	list_add_tail(&person->list, &birthday_list);

	person = kmalloc(sizeof(*person), GFP_KERNEL);
	person->name="Yuan Long";
	person->month=8;
	person->day=13;
	person->year=1995;
	INIT_LIST_HEAD(&person->list);
	list_add_tail(&person->list, &birthday_list);

	person = kmalloc(sizeof(*person), GFP_KERNEL);
	person->name="John";
	person->month=9;
	person->day=2;
	person->year=1998;
	INIT_LIST_HEAD(&person->list);
	list_add_tail(&person->list, &birthday_list);

	person = kmalloc(sizeof(*person), GFP_KERNEL);
	person->name="Kate";
	person->month=8;
	person->day=12;
	person->year=1963;
	INIT_LIST_HEAD(&person->list);
	list_add_tail(&person->list, &birthday_list);

	person = kmalloc(sizeof(*person), GFP_KERNEL);
	person->name="Lily";
	person->month=10;
	person->day=22;
	person->year=1963;
	INIT_LIST_HEAD(&person->list);
	list_add_tail(&person->list, &birthday_list);
     
     
    
    /* Traverse the linked list  */
    printk(KERN_INFO "***Loading Module***\n");
    /* Remove the elements from the linked list and return 
    the free memory back to the kernel */

    /* Go thru the list and free the memory. */
    list_for_each_entry_safe(ptr, next, &birthday_list, list)
    {
        /* on each iteration ptr points to the next birthday struct */
        printk(KERN_INFO "Name: %s, Birthday: Month %d, Day %d, Year %d\n",
        		ptr->name,
                ptr->month,
                ptr->day,
                ptr->year);
        kfree(ptr);
    }
    return 0;
}

void simple_exit(void) {
    
    printk(KERN_INFO "***Removing Module***\n");
    /* Remove the elements from the linked list and return 
    the free memory back to the kernel */

    /* Go thru the list and free the memory. */
    list_for_each_entry_safe(ptr, next, &birthday_list, list)
    {
        /* on each iteration ptr points to the next birthday struct */
        printk(KERN_INFO "Removing: %s, %d, %d, %d\n",
        		ptr->name,
                ptr->month,
                ptr->day,
                ptr->year);
        list_del(&ptr->list);
    }
    
}

module_init( simple_init );
module_exit( simple_exit );

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Kernel Data Structures");
MODULE_AUTHOR("SGG");

