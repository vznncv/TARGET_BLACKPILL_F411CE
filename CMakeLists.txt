add_library(mbed-blackpill-f411ce INTERFACE)

target_sources(mbed-blackpill-f411ce
    INTERFACE
        PeripheralPins.c
        system_clock.c
)

target_include_directories(mbed-blackpill-f411ce
    INTERFACE
        .
)

target_link_libraries(mbed-blackpill-f411ce INTERFACE mbed-stm32f411xe)
