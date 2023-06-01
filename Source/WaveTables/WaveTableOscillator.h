/*
  ==============================================================================

    WaveTableOscillator.h
    Created: 22 Apr 2023 3:15:11pm
    Author:  Onez

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "Sine.h"
#include "Square.h"
#include "Sawtooth.h"
#include "Triangle.h"
#include "../Synth/CustomADSR.h"
#include "../Synth/gain_block.h"
class WaveTableOscillator
{
public:
    WaveTableOscillator() :
    //pointer ownership
    adsr(std::make_unique<CustomADSR>()),
    sineTable(std::make_unique<Sine>()),
    squareTable(std::make_unique<Square>()),
    sawtoothTable(std::make_unique<Sawtooth>()),
    triangleTable(std::make_unique<Triangle>()),
    gainOsc1(std::make_unique<Gain_Block>()),
    gainOsc2(std::make_unique<Gain_Block>()),
    gainOsc3(std::make_unique<Gain_Block>()),
    tableSize(sineTable->waveTable.getNumSamples() - 1),
    wavetable(&sineTable->waveTable),
    wavetable2(&sineTable->waveTable),
    wavetable3(&sineTable->waveTable)
    {};
    
    ~WaveTableOscillator()
    {} ;
    
    void prepare(float sampleRate, int blocksize)
    {
        //prepare funktion die vor dem ausführen gebarucht wird
        m_sampleRate = sampleRate;
        gainOsc1->prepare(sampleRate);
        gainOsc2->prepare(sampleRate);
        gainOsc3->prepare(sampleRate);
        
    }
    
    void getMidiNote(int midiNote)
    {
        //um zu wissen welche midi note gebraucht wird
        m_Note = midiNote;
    }
    
    int returnMidiNote()
    {
        // um die spielende note zu bekommen
        return m_Note ;
    }
    
    void getVelocity(float velocity)
    {
        // um die velocity zu bekommen
        m_velocity = velocity;
    }
    
    void getgainOsc1(float gain)
    {
        gainOsc1->setGain(gain);
    }
    
    void getgainOsc2(float gain)
    {
        gainOsc2->setGain(gain);
    }
    
    void getgainOsc3(float gain)
    {
        gainOsc3->setGain(gain);
    }
    
    
    
    void setFrequency (float frequency)
    {
        // funktion um beim NoteOn die Frequenz zu bekommen für 1tn OSC
        auto tableSizeOverSampleRate = (float) tableSize / m_sampleRate;
        tableDelta = frequency * tableSizeOverSampleRate;
    }
    
    void setFrequency2 (float frequency)
    {
        // funktion um beim NoteOn die Frequenz zu bekommen für 2tn OSC
        auto tableSizeOverSampleRate = (float) tableSize / m_sampleRate;
        tableDelta2 = frequency * tableSizeOverSampleRate;
    }
    
    void setFrequency3 (float frequency)
    {
        // funktion um beim NoteOn die Frequenz zu bekommen für 3tn OSC
        auto tableSizeOverSampleRate = (float) tableSize / m_sampleRate;
        tableDelta3 = frequency * tableSizeOverSampleRate;
    }
    
    float getNextSample() noexcept
    {
        // Funktion für ausführen der Wavetable für OSC 1
        auto index0 = (unsigned int) currentIndex;
        auto index1 = index0 + 1;

        auto frac = currentIndex - (float) index0;

        auto* table = wavetable->getReadPointer (0);
        auto value0 = table[index0];
        auto value1 = table[index1];

        auto currentSample = (value0 + frac * (value1 - value0)) * adsr->process() * m_velocity;

        if ((currentIndex += tableDelta) > (float) tableSize)
            currentIndex -= (float) tableSize;
        return currentSample;
    }
    
    float getNextSample2() noexcept
    {
        // Funktion für ausführen der Wavetable für OSC 2
        auto index0 = (unsigned int) currentIndex2;
        auto index1 = index0 + 1;

        auto frac = currentIndex2 - (float) index0;

        auto* table = wavetable2->getReadPointer (0);
        auto value0 = table[index0];
        auto value1 = table[index1];

        auto currentSample = (value0 + frac * (value1 - value0)) * adsr->process() * m_velocity;

        if ((currentIndex2 += tableDelta2) > (float) tableSize)
            currentIndex2 -= (float) tableSize;
        return currentSample;
    }
    
    float getNextSample3() noexcept
    {
        // Funktion für ausführen der Wavetable für OSC 3
        auto index0 = (unsigned int) currentIndex3;
        auto index1 = index0 + 1;

        auto frac = currentIndex3 - (float) index0;

        auto* table = wavetable3->getReadPointer (0);
        auto value0 = table[index0];
        auto value1 = table[index1];

        auto currentSample = (value0 + frac * (value1 - value0)) * adsr->process() * m_velocity;

        if ((currentIndex3 += tableDelta3) > (float) tableSize)
            currentIndex3 -= (float) tableSize;
        return currentSample;
    }
    
    
    float getAllTables()
    {
        // return Funktion für alle WaveTables
        return gainOsc1->process() * getNextSample() + gainOsc2->process() * getNextSample2() + gainOsc3->process() * getNextSample3();
    }
    
    void swapTable(unsigned int index)
    {
        // Funktion um wavetable zu ändern für OSC 1
            switch(index)
            {
                case(0):
                    wavetable = &sineTable->waveTable;
                    
                    break;
                case(1):
                    wavetable = &squareTable->waveTable;
                 
                    break;
                case(2):
                    wavetable = &sawtoothTable->waveTable;
                    break;
                case(3):
                    wavetable = &triangleTable->waveTable;
                    break;
            }
    }
    void swapTable2(unsigned int index)
    {
        // Funktion um wavetable zu ändern für OSC 2
            
            switch(index)
            {
                case(0):
                    wavetable2 = &sineTable->waveTable;
                    break;
                case(1):
                    wavetable2 = &squareTable->waveTable;
                    break;
                case(2):
                    wavetable2 = &sawtoothTable->waveTable;
                    break;
                case(3):
                    wavetable2 = &triangleTable->waveTable;
                    break;
            }
        
    }
    void swapTable3(unsigned int index)
    {
        // Funktion um wavetable zu ändern für OSC 3
            
            switch(index)
            {
                case(0):
                    wavetable3 = &sineTable->waveTable;
                    break;
                case(1):
                    wavetable3 = &squareTable->waveTable;
                    break;
                case(2):
                    wavetable3 = &sawtoothTable->waveTable;
                    break;
                case(3):
                    wavetable3 = &triangleTable->waveTable;
                    break;
          
            }
        
    }
    
    // Public für die CustomADSR pointer
    std::unique_ptr<CustomADSR> adsr;
    
    
private:
    //private Variablen
    int m_Note = -1;
    float m_sampleRate = 0;
    float currentIndex = 0.0f, tableDelta = 0.0f;
    float currentIndex2 = 0.0f, tableDelta2 = 0.0f;
    float currentIndex3 = 0.0f, tableDelta3 = 0.0f;
    std::unique_ptr<Sine> sineTable;
    std::unique_ptr<Square> squareTable;
    std::unique_ptr<Sawtooth> sawtoothTable;
    std::unique_ptr<Triangle> triangleTable;
    
    std::unique_ptr<Gain_Block> gainOsc1;
    std::unique_ptr<Gain_Block> gainOsc2;
    std::unique_ptr<Gain_Block> gainOsc3;
    const int tableSize;
    juce::AudioSampleBuffer* wavetable;
    juce::AudioSampleBuffer* wavetable2;
    juce::AudioSampleBuffer* wavetable3;
    float m_velocity;
    // check last index for fade in and out
    unsigned int lastIndex = 0;
};
