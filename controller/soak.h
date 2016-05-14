#include <Arduino.h>

void lights_update();
void light_on(unsigned int num);
void light_off(unsigned int num);
void light_toggle(unsigned int num);
void light_blink(unsigned int num, unsigned int milliseconds);
int light_is_on(unsigned int num);
int light_is_blinking(unsigned int num);

void buttons_update();
int button_press(unsigned int num);
int button_release(unsigned int num);
int button_down(unsigned int num);

