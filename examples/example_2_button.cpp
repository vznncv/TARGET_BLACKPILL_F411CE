/**
 * BlackPill button demo.
 */
#include "mbed.h"

DigitalOut user_led(LED1, 1);
InterruptIn user_button(BUTTON1, PullUp);

void user_button_fall_callback()
{
    printf("Button is pressed\n");
}

void user_button_rise_callback()
{
    printf("Button is released\n");
    for (int i = 0; i < 10; i++) {
        user_led = !user_led;
        ThisThread::sleep_for(50ms);
    }
}

auto user_button_rise_event = mbed_event_queue()->make_user_allocated_event(user_button_rise_callback);
auto user_button_fall_event = mbed_event_queue()->make_user_allocated_event(user_button_fall_callback);

int main()
{
    user_button.rise(Callback<void()>(&user_button_rise_event, &decltype(user_button_rise_event)::try_call));
    user_button.fall(Callback<void()>(&user_button_fall_event, &decltype(user_button_fall_event)::try_call));
    printf("====== Start ======\n");

    while (true) {
        ThisThread::sleep_for(1s);
    }
}
