/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "GUI/ADSR_gui.h"
#include "GUI/WaveFormPopUpMenu.h"
#include "GUI/LookAndFeel/Knobs.h"
//==============================================================================
/**
*/
class SynthAudioProcessorEditor  : public juce::AudioProcessorEditor,
public juce::Timer
{
public:
    SynthAudioProcessorEditor (SynthAudioProcessor&);
    ~SynthAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
  
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SynthAudioProcessor& audioProcessor;
    std::shared_ptr<ADSR_GUI> adsr_gui;
    std::unique_ptr<WaveFormPopUpMenu> wave_gui1;
    std::unique_ptr<WaveFormPopUpMenu> wave_gui2;
    std::unique_ptr<WaveFormPopUpMenu> wave_gui3;
    std::shared_ptr<juce::Custom_Knob> pitchKnob1;
    std::shared_ptr<juce::Custom_Knob> pitchKnob2;
    std::shared_ptr<juce::Custom_Knob> pitchKnob3;
    juce::AudioProcessorValueTreeState::SliderAttachment semitones1, semitones2, semitones3;
    void timerCallback() override;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SynthAudioProcessorEditor)
};
