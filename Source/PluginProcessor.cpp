/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SynthAudioProcessor::SynthAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
,treeState(*this, nullptr, "PARAMETERS", createParameterLayout())
#endif
{
    attackTime = treeState.getRawParameterValue("AttackTime");
    attackValue = treeState.getRawParameterValue("AttackValue");
    decayTime = treeState.getRawParameterValue("DecayTime");
    decayValue = treeState.getRawParameterValue("DecayValue");
    sustain = treeState.getRawParameterValue("Sustain");
    sustainTime = treeState.getRawParameterValue("SustainTime");
    release = treeState.getRawParameterValue("Release");
    waveTabletoUse = treeState.getRawParameterValue("WaveTable1");
    waveTabletoUse2 = treeState.getRawParameterValue("WaveTable2");
    waveTabletoUse3 = treeState.getRawParameterValue("WaveTable3");
    gainOsc1 = treeState.getRawParameterValue("GainOsc1");
    gainOsc2 = treeState.getRawParameterValue("GainOsc2");
    gainOsc3 = treeState.getRawParameterValue("GainOsc3");
    
    midiSynth = std::make_unique<MidiSynth>();
    treeState.addParameterListener("AttackTime", this);
    treeState.addParameterListener("AttackValue", this);
    treeState.addParameterListener("DecayTime", this);
    treeState.addParameterListener("DecayValue", this);
    treeState.addParameterListener("Sustain", this);
    treeState.addParameterListener("SustainTime", this);
    treeState.addParameterListener("Release", this);
    treeState.addParameterListener("WaveTable1", this);
    treeState.addParameterListener("WaveTable2", this);
    treeState.addParameterListener("WaveTable3", this);
    treeState.addParameterListener("Semitones1", this);
    treeState.addParameterListener("Semitones2", this);
    treeState.addParameterListener("Semitones3", this);
    
    treeState.addParameterListener("GainOsc1", this);
    treeState.addParameterListener("GainOsc2", this);
    treeState.addParameterListener("GainOsc3", this);
    
    
}

SynthAudioProcessor::~SynthAudioProcessor()
{
    treeState.removeParameterListener("AttackTime", this);
    treeState.removeParameterListener("AttackValue", this);
    treeState.removeParameterListener("DecayTime", this);
    treeState.removeParameterListener("DecayValue", this);
    treeState.removeParameterListener("Sustain", this);
    treeState.removeParameterListener("SustainTime", this);
    treeState.removeParameterListener("Release", this);
    treeState.removeParameterListener("WaveTable1", this);
    treeState.removeParameterListener("WaveTable2", this);
    treeState.removeParameterListener("WaveTable3", this);
    treeState.removeParameterListener("Semitones1", this);
    treeState.removeParameterListener("Semitones2", this);
    treeState.removeParameterListener("Semitones3", this);
    
    treeState.removeParameterListener("GainOsc1", this);
    treeState.removeParameterListener("GainOsc2", this);
    treeState.removeParameterListener("GainOsc3", this);
}

juce::AudioProcessorValueTreeState::ParameterLayout
SynthAudioProcessor::createParameterLayout()
{
    std::vector <std::unique_ptr<juce::RangedAudioParameter>> params;
    
    auto pAttackTime = std::make_unique<juce::AudioParameterFloat>("AttackTime","AttackTime",0.01f,1.0f,0.5f);
    
    auto pAttackValue = std::make_unique<juce::AudioParameterFloat>("AttackValue","AttackValue",0.01f,1.0f,0.5f);
    
    auto pDecayTime =  std::make_unique<juce::AudioParameterFloat>("DecayTime","DecayTime",0.01f,1.0f,0.25f);
    
    auto pDecayValue =  std::make_unique<juce::AudioParameterFloat>("DecayValue","DecayValue",0.01f,1.0f,0.25f);
    
    auto pSustain = std::make_unique<juce::AudioParameterFloat>("Sustain","Sustain",0.01f,1.0f,0.5f);
    
    auto pSustainTime = std::make_unique<juce::AudioParameterFloat>("SustainTime","SustainTime",0.01f,1.0f,0.5f);
    
    auto pRelease = std::make_unique<juce::AudioParameterFloat>("Release","Release",0.01f,1.0f,0.24f);
    
    auto pWavetable1 = std::make_unique<juce::AudioParameterInt>("WaveTable1", "WaveTable1", 0,3, 1);
    
    auto pWavetable2 = std::make_unique<juce::AudioParameterInt>("WaveTable2", "WaveTable2", 0,3, 1);
    
    auto pWavetable3 = std::make_unique<juce::AudioParameterInt>("WaveTable3", "WaveTable3", 0,3, 1);
    
    
    auto pSemitones1 = std::make_unique<juce::AudioParameterInt>("Semitones1", "Semitones1", -24,24, 0);
    
    auto pSemitones2 = std::make_unique<juce::AudioParameterInt>("Semitones2", "Semitones2",-24,24, 0);
    
    auto pSemitones3 = std::make_unique<juce::AudioParameterInt>("Semitones3", "Semitones3",-24,24, 0);
     
    auto pgainOsc1 = std::make_unique<juce::AudioParameterFloat>("GainOsc1","GainOsc1",0.00f,1.0f,0.24f);
    
    auto pgainOsc2 = std::make_unique<juce::AudioParameterFloat>("GainOsc2","GainOsc2",0.00f,1.0f,0.24f);
    
    auto pgainOsc3 = std::make_unique<juce::AudioParameterFloat>("GainOsc3","GainOsc3",0.00f,1.0f,0.24f);
    
    params.push_back(std::move(pAttackTime));
    params.push_back(std::move(pAttackValue));
    params.push_back(std::move(pDecayTime));
    params.push_back(std::move(pDecayValue));
    
    params.push_back(std::move(pSustain));
    params.push_back(std::move(pSustainTime));
    
    params.push_back(std::move(pRelease));
    
    params.push_back(std::move(pWavetable1));
    params.push_back(std::move(pWavetable2));
    params.push_back(std::move(pWavetable3));
    
    params.push_back(std::move(pSemitones1));
    params.push_back(std::move(pSemitones2));
    params.push_back(std::move(pSemitones3));
    
    
    params.push_back(std::move(pgainOsc1));
    params.push_back(std::move(pgainOsc2));
    params.push_back(std::move(pgainOsc3));
    
    return {params.begin(),params.end()};
}
void SynthAudioProcessor::parameterChanged(const juce::String &paramterID, float newValue)
{
    if(paramterID == "AttackTime")
    {
        for(int i = 0; i < midiSynth->waveTable.size(); ++i)
        {
            midiSynth->waveTable[i].adsr->getAttackTime(newValue);
        }
    }
    if(paramterID == "AttackValue")
    {
        for(int i = 0; i < midiSynth->waveTable.size(); ++i)
        {
            midiSynth->waveTable[i].adsr->getAttackValue(newValue);
        }
    }
    
    if(paramterID == "DecayTime")
    {
        for(int i = 0; i < midiSynth->waveTable.size(); ++i)
        {
            midiSynth->waveTable[i].adsr->getDecayTime(newValue);
        }
    }
    
    if(paramterID == "DecayValue")
    {
        for(int i = 0; i < midiSynth->waveTable.size(); ++i)
        {
            midiSynth->waveTable[i].adsr->getDecayValue(newValue);
        }
    }
    
    if(paramterID == "Sustain")
    {
        for(int i = 0; i < midiSynth->waveTable.size(); ++i)
        {
            midiSynth->waveTable[i].adsr->getSustainValue(newValue);
        }
    }
    
    if(paramterID == "SustainTime")
    {
        for(int i = 0; i < midiSynth->waveTable.size(); ++i)
        {
            midiSynth->waveTable[i].adsr->getSustainTime(newValue);
        }
    }
    
    if(paramterID == "Release")
    {
        for(int i = 0; i < midiSynth->waveTable.size(); ++i)
        {
            midiSynth->waveTable[i].adsr->getReleaseTime(newValue);
        }
    }
    if(paramterID == "WaveTable1")
    {
        for(int i = 0; i < midiSynth->waveTable.size(); ++i)
        {
            midiSynth->getWaveTable(newValue);
        }
    }
    
    if(paramterID == "WaveTable2")
    {
        for(int i = 0; i < midiSynth->waveTable.size(); ++i)
        {
            midiSynth->getWaveTable2(newValue);
        }
    }
    
    if(paramterID == "WaveTable3")
    {
        for(int i = 0; i < midiSynth->waveTable.size(); ++i)
        {
            midiSynth->getWaveTable3(newValue);
        }
    }
    
    if(paramterID == "Semitones1")
    {
        for(int i = 0; i < midiSynth->waveTable.size(); ++i)
        {
            midiSynth->getSemitones1(newValue);
        }
    }
    
    if(paramterID == "Semitones2")
    {
        for(int i = 0; i < midiSynth->waveTable.size(); ++i)
        {
            midiSynth->getSemitones2(newValue);
        }
    }
    
    if(paramterID == "Semitones3")
    {
        for(int i = 0; i < midiSynth->waveTable.size(); ++i)
        {
            midiSynth->getSemitones3(newValue);
        }
    }
    
    if(paramterID == "GainOsc1")
    {
        for(int i = 0; i < midiSynth->waveTable.size(); ++i)
        {
            midiSynth->waveTable[i].getgainOsc1(newValue);
        }
    }
    
    if(paramterID == "GainOsc2")
    {
        for(int i = 0; i < midiSynth->waveTable.size(); ++i)
        {
            midiSynth->waveTable[i].getgainOsc2(newValue);
        }
    }
    
    if(paramterID == "GainOsc3")
    {
        for(int i = 0; i < midiSynth->waveTable.size(); ++i)
        {
            midiSynth->waveTable[i].getgainOsc3(newValue);
        }
    }
}

void SynthAudioProcessor::initParams()
{
    for(int i = 0; i < midiSynth->waveTable.size(); i++)
    {
        midiSynth->waveTable[i].adsr->getAttackTime(*attackTime);
        midiSynth->waveTable[i].adsr->getAttackValue(*attackValue);
        midiSynth->waveTable[i].adsr->getDecayTime(*decayValue);
        midiSynth->waveTable[i].adsr->getDecayValue(*decayTime);
        midiSynth->waveTable[i].adsr->getSustainValue(*sustain);
        midiSynth->waveTable[i].adsr->getSustainTime(*sustainTime);
        midiSynth->waveTable[i].adsr->getReleaseTime(*release);
        midiSynth->getWaveTable(*waveTabletoUse);
        midiSynth->getWaveTable(*waveTabletoUse2);
        midiSynth->getWaveTable(*waveTabletoUse3);
        midiSynth->waveTable[i].getgainOsc1(*gainOsc1);
        midiSynth->waveTable[i].getgainOsc2(*gainOsc2);
        midiSynth->waveTable[i].getgainOsc3(*gainOsc3);
    }
    
    
    
}

//==============================================================================
const juce::String SynthAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SynthAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SynthAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SynthAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SynthAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SynthAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SynthAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SynthAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String SynthAudioProcessor::getProgramName (int index)
{
    return {};
}

void SynthAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void SynthAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{

    midiSynth->prepare(sampleRate, samplesPerBlock);
    initParams();
}

void SynthAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SynthAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void SynthAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
                buffer.clear (i, 0, buffer.getNumSamples());
    
    
   
    
    midiSynth->processBlock(buffer, midiMessages);
    // under construction
    if(int(*waveTabletoUse) != lastIndex)
    {
       // midiSynth->getWaveTable(*waveTabletoUse);
        buffer.clear();
        
        for (int i = buffer.getNumChannels(); --i >= 0;)
        {
            buffer.applyGainRamp (i, 0, juce::jmin (512, buffer.getNumSamples()), 1.0f, 0.0f);
        //  buffer.applyGainRamp(<#int channel#>, <#int startSample#>, <#int numSamples#>, <#float startGain#>, <#float endGain#>)
        }

        lastIndex = int(*waveTabletoUse);
    }
}

//==============================================================================
bool SynthAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* SynthAudioProcessor::createEditor()
{
    return new SynthAudioProcessorEditor (*this);
}

//==============================================================================
void SynthAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    juce::MemoryOutputStream mos(destData, true);
        treeState.state.writeToStream(mos);
}

void SynthAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    auto tree = juce::ValueTree::readFromData(data, sizeInBytes);
        if (tree.isValid())
        {
            treeState.replaceState(tree);
        }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SynthAudioProcessor();
}
