/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SynthAudioProcessorEditor::SynthAudioProcessorEditor (SynthAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p),
adsr_gui(std::make_shared<ADSR_GUI>(audioProcessor.treeState.getParameter("AttackTime"),
                                    audioProcessor.treeState.getParameter("AttackValue"),
                                    audioProcessor.treeState.getParameter("DecayTime"),
                                    audioProcessor.treeState.getParameter("DecayValue"),
                                    audioProcessor.treeState.getParameter("Sustain"),
                                    audioProcessor.treeState.getParameter("SustainTime"),
                                    audioProcessor.treeState.getParameter("Release")
                                    )),
wave_gui1(std::make_unique<WaveFormPopUpMenu>(audioProcessor.treeState.getParameter("WaveTable1"),juce::Colours::pink)),
wave_gui2(std::make_unique<WaveFormPopUpMenu>(audioProcessor.treeState.getParameter("WaveTable2"),juce::Colours::lightblue)),
wave_gui3(std::make_unique<WaveFormPopUpMenu>(audioProcessor.treeState.getParameter("WaveTable3"),juce::Colours::lightgreen)),
pitchKnob1(std::make_shared<juce::Custom_Knob>()),
pitchKnob2(std::make_shared<juce::Custom_Knob>()),
pitchKnob3(std::make_shared<juce::Custom_Knob>()),
semitones1(audioProcessor.treeState, "Semitones1", *pitchKnob1),
semitones2(audioProcessor.treeState, "Semitones2", *pitchKnob2),
semitones3(audioProcessor.treeState, "Semitones3", *pitchKnob3)

{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setResizable(true, true);
    
    addAndMakeVisible(*adsr_gui);
    addAndMakeVisible(*wave_gui1);
    addAndMakeVisible(*wave_gui2);
    addAndMakeVisible(*wave_gui3);
    pitchKnob1->setStyles(juce::Custom_Knob::ZenStyles::blueStyle);
    addAndMakeVisible(*pitchKnob1);
    pitchKnob2->setStyles(juce::Custom_Knob::ZenStyles::blueStyle);
    addAndMakeVisible(*pitchKnob2);
    pitchKnob3->setStyles(juce::Custom_Knob::ZenStyles::blueStyle);
    addAndMakeVisible(*pitchKnob3);
    setSize (800, 600);
    startTimerHz(60);
}

SynthAudioProcessorEditor::~SynthAudioProcessorEditor()
{
}

//==============================================================================
void SynthAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (juce::Colours::black);
}

void SynthAudioProcessorEditor::resized()
{
    const int offsetx = getWidth() / 100 ;
  
    const int offsety = getHeight() / 10;
    const int adsrWidth = getWidth() /2 / 1.1;
    const int adsrHeight = getHeight() /2 / 2;
    const int wave_guiHeight = adsrHeight/2;
    const int wave_guiWidth = adsrWidth/2;
    const int offset_wavetable = offsetx * 5 + wave_guiWidth ;
    const int pitchwidth = wave_guiWidth;
    const int pitchHeight = wave_guiHeight * 2;
    wave_gui1->setBounds(offsetx, offsety , wave_guiWidth, wave_guiHeight);
    wave_gui2->setBounds(offset_wavetable, offsety , wave_guiWidth, wave_guiHeight);
    wave_gui3->setBounds(offset_wavetable * 2, offsety , wave_guiWidth, wave_guiHeight);
    pitchKnob1->setBounds(wave_gui1->getBounds().getBottomLeft().getX(), wave_gui1->getBounds().getBottomLeft().getY(), pitchwidth, pitchHeight);
    pitchKnob2->setBounds(wave_gui2->getBounds().getBottomLeft().getX(), wave_gui2->getBounds().getBottomLeft().getY(), pitchwidth, pitchHeight);
    pitchKnob3->setBounds(wave_gui3->getBounds().getBottomLeft().getX(), wave_gui3->getBounds().getBottomLeft().getY(), pitchwidth, pitchHeight);
    
    
    adsr_gui->setBounds(offsetx, offsety + getHeight() / 2, adsrWidth, adsrHeight);
  
}

void SynthAudioProcessorEditor::timerCallback()
{
    repaint();
}
