#include <Arduino.h>

void lights_update();
void light_on(unsigned int num);
void light_off(unsigned int num);
int light_toggle(unsigned int num);
void light_blink(unsigned int num, unsigned int milliseconds);
int light_blink_toggle(unsigned int num, unsigned int milliseconds);
int light_is_on(unsigned int num);
int light_is_blinking(unsigned int num);

void buttons_update();
int button_press(unsigned int num);
int button_release(unsigned int num);
int button_down(unsigned int num);

void leds_update();
void led_color(unsigned int num, unsigned int rgb);
void led_color(unsigned int num, unsigned char red, unsigned char green, unsigned char blue);
#define OFF    0x000000
#define RED    0xFF0000
#define GREEN  0x00FF00
#define BLUE   0x0000FF
#define YELLOW 0xFFFF00
#define PINK   0xFF1088
#define ORANGE 0xE05800
#define WHITE  0xFFFFFF



