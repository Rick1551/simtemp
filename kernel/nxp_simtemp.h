#ifndef NXP_SIMTEMP_H
#define NXP_SIMTEMP_H

#include <linux/miscdevice.h>
#include <linux/timer.h>
#include <linux/kobject.h>

/* Nombre del driver y dispositivo */
#define DRIVER_NAME "nxp_simtemp"
#define DEVICE_NAME "simtemp"

/* IOCTL opcional para futuras extensiones */
#define SIMTEMP_IOCTL_GET_TEMP _IOR('s', 1, int)

/* Estructura principal del dispositivo */
struct simtemp_data {
    struct miscdevice miscdev;
    struct timer_list timer;
    struct kobject *kobj;
    int sampling_ms;
    int threshold_mC;
    int current_temp;
};

/* Extern para acceso global */
extern struct simtemp_data *simtemp;

#endif
