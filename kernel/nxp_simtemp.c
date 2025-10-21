// SPDX-License-Identifier: GPL-2.0
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/jiffies.h>
#include <linux/sysfs.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include "nxp_simtemp.h"

struct simtemp_data *simtemp;

/* Timer callback */
static void simtemp_timer_callback(struct timer_list *t)
{
    simtemp->current_temp = 40000 + (prandom_u32() % 10000);
    mod_timer(&simtemp->timer, jiffies + msecs_to_jiffies(simtemp->sampling_ms));
}

/* Sysfs show/set */
static ssize_t sampling_ms_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
    return sprintf(buf, "%d\n", simtemp->sampling_ms);
}

static ssize_t sampling_ms_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{
    int val;
    if (kstrtoint(buf, 10, &val) == 0 && val > 0)
        simtemp->sampling_ms = val;
    return count;
}

static struct kobj_attribute sampling_ms_attr = __ATTR(sampling_ms, 0664, sampling_ms_show, sampling_ms_store);

/* File ops */
static ssize_t simtemp_read(struct file *file, char __user *buf, size_t count, loff_t *ppos)
{
    return -ENOSYS;
}

static const struct file_operations simtemp_fops = {
    .owner = THIS_MODULE,
    .read = simtemp_read,
};

/* Probe */
static int simtemp_probe(struct platform_device *pdev)
{
    int ret;

    simtemp = kzalloc(sizeof(*simtemp), GFP_KERNEL);
    if (!simtemp)
        return -ENOMEM;

    simtemp->sampling_ms = 100;
    simtemp->threshold_mC = 45000;

    simtemp->miscdev.minor = MISC_DYNAMIC_MINOR;
    simtemp->miscdev.name = DEVICE_NAME;
    simtemp->miscdev.fops = &simtemp_fops;

    ret = misc_register(&simtemp->miscdev);
    if (ret) goto err_misc;

    simtemp->kobj = kobject_create_and_add("simtemp", kernel_kobj);
    if (!simtemp->kobj) goto err_sysfs;

    ret = sysfs_create_file(simtemp->kobj, &sampling_ms_attr.attr);
    if (ret) goto err_sysfs;

    timer_setup(&simtemp->timer, simtemp_timer_callback, 0);
    mod_timer(&simtemp->timer, jiffies + msecs_to_jiffies(simtemp->sampling_ms));

    dev_info(&pdev->dev, "simtemp driver loaded\n");
    return 0;

err_sysfs:
    misc_deregister(&simtemp->miscdev);
err_misc:
    kfree(simtemp);
    return ret;
}

/* Remove */
static int simtemp_remove(struct platform_device *pdev)
{
    del_timer_sync(&simtemp->timer);
    sysfs_remove_file(simtemp->kobj, &sampling_ms_attr.attr);
    kobject_put(simtemp->kobj);
    misc_deregister(&simtemp->miscdev);
    kfree(simtemp);
    return 0;
}

/* Device tree match */
static const struct of_device_id simtemp_of_match[] = {
    { .compatible = "nxp,simtemp" },
    {},
};
MODULE_DEVICE_TABLE(of, simtemp_of_match);

static struct platform_driver simtemp_driver = {
    .driver = {
        .name = DRIVER_NAME,
        .of_match_table = simtemp_of_match,
    },
    .probe = simtemp_probe,
    .remove = simtemp_remove,
};

module_platform_driver(simtemp_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ricardo Saldaña");
MODULE_DESCRIPTION("NXP Simulated Temperature Sensor");
