cmake_minimum_required(VERSION 3.22)
set(CMAKE_OSX_DEPLOYMENT_TARGET "12.0" CACHE STRING "")
project(AudioRoutingDevice VERSION 0.1.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_subdirectory(JUCE)

juce_add_gui_app(AudioRoutingDevice
    PRODUCT_NAME "Audio Routing Device"
    MICROPHONE_PERMISSION_ENABLED TRUE
    MICROPHONE_PERMISSION_TEXT "Audio Routing Device needs microphone access to receive audio input."
)

target_sources(AudioRoutingDevice PRIVATE
    Source/Main.cpp
    Source/MainComponent.cpp
)

target_compile_definitions(AudioRoutingDevice PRIVATE
    JUCE_USE_CURL=0
    JUCE_WEB_BROWSER=0
    JUCE_DISPLAY_SPLASH_SCREEN=0
    JUCE_REPORT_APP_USAGE=0
)

target_link_libraries(AudioRoutingDevice PRIVATE
    juce::juce_audio_devices
    juce::juce_audio_utils
    juce::juce_dsp
    juce::juce_gui_basics
    PUBLIC
    juce::juce_recommended_config_flags
    juce::juce_recommended_warning_flags
)
