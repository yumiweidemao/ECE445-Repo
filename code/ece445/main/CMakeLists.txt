# See the build system documentation in IDF programming guide
# for more information about component CMakeLists.txt files.

idf_component_register(
    SRCS main.c wifi_station.c mqtt_service.c rake_eventgroup.c motor.c odor.c weight.c HX711.c
    INCLUDE_DIRS include/
)
