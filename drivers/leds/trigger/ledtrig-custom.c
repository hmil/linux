
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/leds.h>
#include <linux/timer.h>
#include "../leds.h"

struct custom_data {
	struct timer_list timer;
};

static void led_custom_function(unsigned long data)
{

}

static void custom_trig_activate(struct led_classdev *led_cdev)
{
	struct custom_data *data;
	data =  kzalloc(sizeof(*data), GFP_KERNEL);
	if (!data)
		return;

	led_cdev->trigger_data = data;
	setup_timer(&data->timer, led_custom_function, (unsigned long) led_cdev);
	led_cdev->activated = true;
}

static void custom_trig_deactivate(struct led_classdev *led_cdev)
{
	struct custom_data *data = led_cdev->trigger_data;

	if (led_cdev->activated) {
		del_timer_sync(&data->timer);
		kfree(data);
		led_cdev->trigger_data = NULL;
		led_cdev->activated = false;
	}
}

static struct led_trigger custom_led_trigger = {
	.name     = "custom",
	.activate = custom_trig_activate,
	.deactivate = custom_trig_deactivate,
};

static int __init custom_trig_init(void)
{
	return led_trigger_register(&custom_led_trigger);
}

static void __exit custom_trig_exit(void)
{
	led_trigger_unregister(&custom_led_trigger);
}

module_init(custom_trig_init);
module_exit(custom_trig_exit);

MODULE_AUTHOR("Hadrien Milano <hadrien.milano@gmail.com>");
MODULE_DESCRIPTION("Custom LED trigger");
MODULE_LICENSE("GPL");
