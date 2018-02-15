/* Include all the necessary libraries here. */
#include <linux/init.h>       /* Contains macros such as __init and __exit. */
#include <linux/module.h>     /* Contains headers for the loadable kernel modules. */
#include <linux/kernel.h>     /* Contains types, macro and function of kernel. */
#include <linux/fs.h>         /* file operations data structure is defined*/
#include  <linux/time.h>
#include  <linux/device.h>
#include  <linux/string.h>
#include  <asm/uaccess.h>
#include <linux/slab.h>
#define NAME_OF_DEVICE "mytime"
#define CLASS_OF_DEVICE "CHARDEV"
MODULE_LICENSE("GPL");
MODULE_AUTHOR("SUJIT"); 
MODULE_DESCRIPTION("A char device to print time");
MODULE_VERSION("1.0"); 
int deviceNumber;
static struct class*  MyTimeClass  = NULL; 
static struct device* MyTimeDevice = NULL;  
static struct timespec xtime;
static struct timespec *gtime;
//static char   userTimeBuffer[500] = {0}; // the result is wriiten here by the module
//static int  messageBufferSize; 
static int noOfUsersOnFile;
static size_t byteCounter;
char timeStringReturn[500];
//struct timespec gtime; 
static ssize_t device_read(struct file *file_pointer, char *buffer_pointer, size_t length_to_read, loff_t *offset_pointer);
static int device_open(struct inode *inodep, struct file *file_pointer);

static struct file_operations file_op =
{
   .open = device_open,
   .read = device_read,
   //.release = device_close,
};

// called when module is installed
static int __init mytime_init(void)
{
	
	//timeStringReturn = kmalloc(100 * sizeof(*char), GFP_KERNEL);
   //gtime = kmalloc(sizeof(struct timespec),GFP_KERNEL);

	deviceNumber=register_chrdev(0, NAME_OF_DEVICE, &file_op);
	if (deviceNumber<0){
		printk(KERN_ALERT "device number went wrong");
	} else {
	}	
	MyTimeClass = class_create(THIS_MODULE, CLASS_OF_DEVICE);
	if (IS_ERR(MyTimeClass)){ 
		printk(KERN_ALERT "class went wrong");
		return PTR_ERR(MyTimeClass);
	} else {
	}
	MyTimeDevice = device_create(MyTimeClass, NULL, MKDEV(deviceNumber, 0), NULL, NAME_OF_DEVICE);
	if (IS_ERR(MyTimeDevice)){
		printk(KERN_ALERT "Failed to create the device\n");
		class_destroy(MyTimeClass);
      		unregister_chrdev(deviceNumber, NAME_OF_DEVICE);      		
      		return PTR_ERR(MyTimeDevice);
	}
	return 0;
}



// called when module is removed
static void __exit mytime_exit(void)
{
	device_destroy(MyTimeClass, MKDEV(deviceNumber, 0));    
   	class_unregister(MyTimeClass);                        
   	class_destroy(MyTimeClass);        
   	unregister_chrdev(deviceNumber, NAME_OF_DEVICE);
      kfree(gtime);
	printk(KERN_ALERT "module ends\n");
}

static ssize_t device_read(struct file *file_pointer, char *buffer_pointer, size_t length_to_read, loff_t *offset_pointer){
   printk(KERN_ALERT "ENTERING-1\n");
   byteCounter=0;
   printk(KERN_ALERT "ENTERING-2\n");
   xtime = current_kernel_time();
   printk(KERN_ALERT "ENTERING-3,\n");
   gtime = kmalloc(sizeof(struct timespec),GFP_KERNEL);
   getnstimeofday(gtime);
   printk(KERN_ALERT "ENTERING-4,\n");
   sprintf(timeStringReturn,"%s %ld %ld %s %s %ld  %ld %s","current_kernel_time: ", xtime.tv_sec, xtime.tv_nsec, "\n", "getnstimeofday: ", gtime->tv_sec, gtime->tv_nsec, "\n");
   printk(KERN_ALERT "\ncurrent_kernel_time: %s", timeStringReturn);
   // copy_to_user format - ( * to, *from, size) and returns 0 on success
   byteCounter = copy_to_user(buffer_pointer, timeStringReturn, strlen(timeStringReturn));
   if (byteCounter != 0) {
   	printk(KERN_ALERT "Failed to write %zu bytes into user space\n", byteCounter);
   	return -EFAULT;
   } else {
   	printk(KERN_ALERT "Copying to user space has succeeded");
   }
   return byteCounter;
}

static int device_open(struct inode *inodep, struct file *file_pointer){
   noOfUsersOnFile++;
   printk(KERN_ALERT "MyTime Open: Device has been opened by %d users\n", noOfUsersOnFile);
   //return 0;
   return 0;
}
//static int device_close(struct inode *inodep, struct file *file_pointer){
  // noOfUsersOnFile--;
   //printk(KERN_ALERT "MyTime Release: The device has been opened by %d users\n", noOfUsersOnFile);
   //return 0;
   //return 0;
//}

module_init(mytime_init);
module_exit(mytime_exit);
