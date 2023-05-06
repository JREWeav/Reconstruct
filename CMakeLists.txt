cmake_minimum_required(VERSION 3.16)

set (TargetName Reconstruct)
set (CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

project(RECONSTRUCT 
        VERSION 0.5.1
        LANGUAGES CXX
        DESCRIPTION "Reconstruct is a plugin for Granular Synthesis")

add_subdirectory(JUCE)                    

juce_add_plugin(${TargetName}                                 
    # ICON_BIG ...
    # ICON_SMALL ...
    COMPANY_NAME "Justin Weaver"
    IS_SYNTH TRUE
    NEEDS_MIDI_INPUT TRUE
    NEEDS_MIDI_OUTPUT FALSE                
    EDITOR_WANTS_KEYBOARD_FOCUS TRUE
    COPY_PLUGIN_AFTER_BUILD TRUE
    PLUGIN_MANUFACTURER_CODE JuWe
    PLUGIN_CODE Rec0
                          
    FORMATS VST3 Standalone AU           
    PRODUCT_NAME "Reconstruct")       

juce_generate_juce_header(${TargetName})

target_sources(${TargetName}
    PRIVATE
        ./src/AudioProcessing/Grain.cpp
        ./src/AudioProcessing/GranularEngine.cpp
        ./src/AudioProcessing/PluginProcessor.cpp
        ./src/AudioProcessing/EnvelopeGenerator.cpp

        ./src/GUI/LoadingComponent.cpp
        ./src/GUI/MainGUI.cpp
        ./src/GUI/PluginEditor.cpp
        ./src/GUI/Components/AudioWaveform.cpp
        ./src/GUI/Components/EnvelopeGUI.cpp
        ./src/GUI/Components/RandomnessDial.cpp

        ./src/LookAndFeel/ReconLookAndFeel.cpp
        )

target_compile_definitions(${TargetName}
    PUBLIC
        # JUCE_WEB_BROWSER and JUCE_USE_CURL would be on by default, but you might not need them.
        JUCE_WEB_BROWSER=0  # If you remove this, add `NEEDS_WEB_BROWSER TRUE` to the `juce_add_plugin` call
        JUCE_USE_CURL=0     # If you remove this, add `NEEDS_CURL TRUE` to the `juce_add_plugin` call
        JUCE_VST3_CAN_REPLACE_VST2=0
        JUCE_MODAL_LOOPS_PERMITTED=1)

target_link_libraries(${TargetName}
    PRIVATE
        # AudioPluginData           # If we'd created a binary data target, we'd link to it here
        juce::juce_audio_utils
        juce::juce_audio_processors
        juce::juce_dsp
        juce::juce_audio_basics
        juce::juce_audio_devices
        juce::juce_audio_formats
        juce::juce_audio_plugin_client
        juce::juce_audio_processors
        juce::juce_core
        juce::juce_data_structures
        juce::juce_events
        juce::juce_graphics
        juce::juce_gui_basics
        juce::juce_gui_extra
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)
