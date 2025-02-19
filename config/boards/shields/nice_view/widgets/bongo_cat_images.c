#include <zephyr.h>
#include <drivers/display.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys/printk.h>

LOG_MODULE_REGISTER(bongo_cat, LOG_LEVEL_DBG);

// Placeholder image data (a simple checkerboard pattern)
static const uint8_t placeholder_img_map[] = {
    0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, // Row 1
    0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, // Row 2
    0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, // Row 3
    0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, // Row 4
};

// Image descriptor for the placeholder image
static const struct display_image placeholder_img = {
    .data = placeholder_img_map,
    .width = 64,   // Width in pixels
    .height = 32,  // Height in pixels
    .color_format = DISPLAY_COLOR_FORMAT_MONO // Monochrome color format
};

// Image data for idle, fast, and slow images (to be replaced with actual data)
static const uint8_t idle_img1_map[] = { /* Insert actual image data here */ };
static const uint8_t fast_img1_map[] = { /* Insert actual image data here */ };
static const uint8_t slow_img_map[] = { /* Insert actual image data here */ };

// Image descriptors for animations
static const struct display_image idle_img = {
    .data = idle_img1_map,
    .width = 128,
    .height = 40,
    .color_format = DISPLAY_COLOR_FORMAT_MONO
};

static const struct display_image fast_img = {
    .data = fast_img1_map,
    .width = 128,
    .height = 40,
    .color_format = DISPLAY_COLOR_FORMAT_MONO
};

static const struct display_image slow_img = {
    .data = slow_img_map,
    .width = 128,
    .height = 40,
    .color_format = DISPLAY_COLOR_FORMAT_MONO
};

// Function to update the display with an image
void update_display(const struct display_image *img) {
    struct device *disp_dev = device_get_binding(CONFIG_DISPLAY_DEVICE_NAME);
    if (!disp_dev) {
        LOG_ERR("Display device not found!");
        return;
    }

    // Draw the image on the display
    int ret = display_blit(disp_dev, 0, 0, img->width, img->height, img->data);
    if (ret) {
        LOG_ERR("Failed to update display: %d", ret);
    }
}

// Function to select the appropriate animation based on WPM
void update_bongo_cat_wpm(int wpm) {
    LOG_DBG("Updating bongo cat animation, WPM: %d", wpm);

    if (wpm < CONFIG_ZMK_WIDGET_BONGO_CAT_IDLE_LIMIT) {
        LOG_DBG("Set source to idle image");
        update_display(&idle_img);
    } else if (wpm < CONFIG_ZMK_WIDGET_BONGO_CAT_SLOW_LIMIT) {
        LOG_DBG("Set source to slow image");
        update_display(&slow_img);
    } else {
        LOG_DBG("Set source to fast image");
        update_display(&fast_img);
    }
}

// Function to set the display to a placeholder image
void set_placeholder_image() {
    LOG_DBG("Setting source to placeholder image");
    update_display(&placeholder_img);
}

int bongo_cat_listener(const zmk_event_t *eh) {
    struct zmk_wpm_state_changed *ev = as_zmk_wpm_state_changed(eh);

    // Call the update function whenever WPM changes
    update_bongo_cat_wpm(ev->state);

    return ZMK_EV_EVENT_BUBBLE;
}

// Register the listener
ZMK_LISTENER(zmk_widget_bongo_cat, bongo_cat_listener)
ZMK_SUBSCRIPTION(zmk_widget_bongo_cat, zmk_wpm_state_changed);

