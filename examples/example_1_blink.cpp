/**
 * Simple led blink and printf usage demo.
 */
#include "mbed.h"

DigitalOut user_led(LED1, 1);

int main()
{
    int blink_count = 0;

    printf("====== Start ======\n");
    while (true) {
        for (int i = 0; i < 8; i++) {
            user_led = !user_led;
            ThisThread::sleep_for(25ms);
        }
        blink_count++;

        printf("Blink count = %4i\n", blink_count);

        ThisThread::sleep_for(1800ms);
    }
}
