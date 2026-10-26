/*
 * Pantalla de estado propia: igual que la de ZMK, pero al encender
 * muestra unos segundos el logo "01" de Hatsune Miku.
 *
 * SPDX-License-Identifier: MIT
 */

#include <zmk/display/widgets/output_status.h>
#include <zmk/display/widgets/peripheral_status.h>
#include <zmk/display/widgets/battery_status.h>
#include <zmk/display/widgets/layer_status.h>
#include <zmk/display/status_screen.h>

#include <zephyr/logging/log.h>
LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

#define LOGO_MS 3000

extern lv_img_dsc_t logo_01;

#if IS_ENABLED(CONFIG_ZMK_WIDGET_BATTERY_STATUS)
static struct zmk_widget_battery_status battery_status_widget;
#endif

#if IS_ENABLED(CONFIG_ZMK_WIDGET_OUTPUT_STATUS)
static struct zmk_widget_output_status output_status_widget;
#endif

#if IS_ENABLED(CONFIG_ZMK_WIDGET_PERIPHERAL_STATUS)
static struct zmk_widget_peripheral_status peripheral_status_widget;
#endif

#if IS_ENABLED(CONFIG_ZMK_WIDGET_LAYER_STATUS)
static struct zmk_widget_layer_status layer_status_widget;
#endif

static void hide_logo(lv_timer_t *timer) { lv_obj_del((lv_obj_t *)timer->user_data); }

static void show_logo(lv_obj_t *screen) {
    lv_color_t bg = lv_obj_get_style_bg_color(screen, LV_PART_MAIN);
    lv_color_t fg = lv_obj_get_style_text_color(screen, LV_PART_MAIN);
    lv_img_buf_set_palette(&logo_01, 0, bg);
    lv_img_buf_set_palette(&logo_01, 1, fg);

    // Capa que tapa todo lo demás mientras se ve el logo
    lv_obj_t *splash = lv_obj_create(screen);
    lv_obj_remove_style_all(splash);
    lv_obj_set_size(splash, LV_PCT(100), LV_PCT(100));
    lv_obj_set_style_bg_color(splash, bg, LV_PART_MAIN);
    lv_obj_set_style_bg_opa(splash, LV_OPA_COVER, LV_PART_MAIN);

    lv_obj_t *img = lv_img_create(splash);
    lv_img_set_src(img, &logo_01);
    lv_obj_center(img);

    lv_timer_t *timer = lv_timer_create(hide_logo, LOGO_MS, splash);
    lv_timer_set_repeat_count(timer, 1);
}

lv_obj_t *zmk_display_status_screen() {
    lv_obj_t *screen;
    screen = lv_obj_create(NULL);

#if IS_ENABLED(CONFIG_ZMK_WIDGET_BATTERY_STATUS)
    zmk_widget_battery_status_init(&battery_status_widget, screen);
    lv_obj_align(zmk_widget_battery_status_obj(&battery_status_widget), LV_ALIGN_TOP_RIGHT, 0, 0);
#endif

#if IS_ENABLED(CONFIG_ZMK_WIDGET_OUTPUT_STATUS)
    zmk_widget_output_status_init(&output_status_widget, screen);
    lv_obj_align(zmk_widget_output_status_obj(&output_status_widget), LV_ALIGN_TOP_LEFT, 0, 0);
#endif

#if IS_ENABLED(CONFIG_ZMK_WIDGET_PERIPHERAL_STATUS)
    zmk_widget_peripheral_status_init(&peripheral_status_widget, screen);
    lv_obj_align(zmk_widget_peripheral_status_obj(&peripheral_status_widget), LV_ALIGN_TOP_LEFT, 0,
                 0);
#endif

#if IS_ENABLED(CONFIG_ZMK_WIDGET_LAYER_STATUS)
    zmk_widget_layer_status_init(&layer_status_widget, screen);
    lv_obj_set_style_text_font(zmk_widget_layer_status_obj(&layer_status_widget),
                               lv_theme_get_font_small(screen), LV_PART_MAIN);
    lv_obj_align(zmk_widget_layer_status_obj(&layer_status_widget), LV_ALIGN_BOTTOM_LEFT, 0, 0);
#endif

    show_logo(screen);
    return screen;
}
