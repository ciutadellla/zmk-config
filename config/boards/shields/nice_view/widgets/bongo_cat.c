#include <zmk/event_manager.h>
#include <zmk/events/wpm_state_changed.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys/printk.h>
#include <zephyr/kernel.h>
#include <drivers/display.h>

LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

#include "bongo_cat.h"

#if IS_ENABLED(CONFIG_ZMK_DISPLAY_WORK_QUEUE_DEDICATED)
#error "Bongo cat only works with the system work queue currently"
#endif

static sys_slist_t widgets = SYS_SLIST_STATIC_INIT(&widgets);

enum anim_state {
    anim_state_none,
    anim_state_idle,
    anim_state_slow,
    anim_state_fast
} current_anim_state;

const void **images;
uint8_t images_len;

// Placeholder for the image data; you'd define these or use an appropriate display library.
static const uint8_t *idle_images[] = {
    // Add actual image data here
};

static const uint8_t *fast_images[] = {
    // Add actual image data here
};

static const uint8_t *slow_img = NULL;  // Define slow image here

void update_bongo_cat_wpm(struct zmk_widget_bongo_cat *widget, int wpm) {
    LOG_DBG("anim state %d", current_anim_state);

    if (wpm < CONFIG_ZMK_WIDGET_BONGO_CAT_IDLE_LIMIT) {
        if (current_anim_state != anim_state_idle) {
            LOG_DBG("Set source to idle images!");

            // Set widget display to idle images
            images = idle_images;
            current_anim_state = anim_state_idle;
            // Call a function that updates the widget's display based on the images array
            update_widget_display(widget);
        }
    } else if (wpm < CONFIG_ZMK_WIDGET_BONGO_CAT_SLOW_LIMIT) {
        if (current_anim_state != anim_state_slow) {
            LOG_DBG("Set source to slow image!");
            // Set widget display to slow image
            current_anim_state = anim_state_slow;
            update_widget_display(widget);  // Use slow_img here
        }
    } else {
        if (current_anim_state != anim_state_fast) {
            LOG_DBG("Set source to fast images!");

            // Set widget display to fast images
            images = fast_images;
            current_anim_state = anim_state_fast;
            update_widget_display(widget);  // Update widget with fast images
        }
    }
}

// This function will update the widget's display with the selected image based on the current WPM state
void update_widget_display(struct zmk_widget_bongo_cat *widget) {
    // Add logic here to update the widget display, e.g., using Zephyr's display driver API.
    struct device *disp_dev = device_get_binding(CONFIG_DISPLAY_DEVICE_NAME);

    if (disp_dev) {
        // You would need to render the image to the screen here.
        // Replace this logic with your actual method of displaying an image.
        printk("Updating display with selected image...\n");

        // For example, if using Zephyr's framebuffer:
        display_blit(disp_dev, &widget->obj);  // Replace with actual drawing logic
    } else {
        LOG_ERR("Display device not found.");
    }
}

int zmk_widget_bongo_cat_init(struct zmk_widget_bongo_cat *widget, lv_obj_t *parent) {
    // No LVGL widget creation here.
    // Instead, you'll initialize a display or buffer for your widget to render on.
    widget->obj = NULL;  // Set a reference to the actual display buffer or widget object here.
    update_bongo_cat_wpm(widget, 0);

    sys_slist_append(&widgets, &widget->node);

    return 0;
}

int bongo_cat_listener(const zmk_event_t *eh) {
    struct zmk_widget_bongo_cat *widget;
    struct zmk_wpm_state_changed *ev = as_zmk_wpm_state_changed(eh);

    SYS_SLIST_FOR_EACH_CONTAINER(&widgets, widget, node) {
        LOG_DBG("Set the WPM %d", ev->state);
        update_bongo_cat_wpm(widget, ev->state);
    }

    return ZMK_EV_EVENT_BUBBLE;
}

ZMK_LISTENER(zmk_widget_bongo_cat, bongo_cat_listener)
ZMK_SUBSCRIPTION(zmk_widget_bongo_cat, zmk_wpm_state_changed);
