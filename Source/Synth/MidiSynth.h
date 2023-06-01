/*
  ==============================================================================

    MidiSynth.h
    Created: 23 Apr 2023 9:10:36pm
    Author:  Onez

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "../WaveTables/WaveTableOscillator.h"
class MidiSynth
{
public:
    MidiSynth()
    {};
    ~MidiSynth()
    {};
    void prepare(double sampleRate, int blockSize)
    {
        //prepare Funktion
        for (int i = 0; i < waveTable.size(); ++i)
        {
            waveTable[i].prepare(sampleRate,blockSize);
            waveTable[i].adsr->prepare(sampleRate, blockSize);
        }
        // Size für die temporären Buffer
        tempBuffer.setSize(2, blockSize);
        subBuffer.setSize(2,blockSize);
    }
    
    void handleMidiEvent(const juce::MidiMessage& midiMessages)
    {
        // Falls midi message is NoteON
        if(midiMessages.isNoteOn())
        {
            for(int i = 0; i < waveTable.size(); ++i)
            {
                // Schau ob die Stimme gerade was macht
                if(waveTable[i].adsr->isActive() == false)
                {
                    //falls nicht berechne die Frequenz
                    const auto frequency = juce::MidiMessage::getMidiNoteInHertz(midiMessages.getNoteNumber());
                    // Merke dir die Midi Note
                    waveTable[i].getMidiNote(midiMessages.getNoteNumber());
                    //übergib die Frequenz den Wavetables
                    waveTable[i].setFrequency(shiftedFrequency(frequency,semitones_1));
                    waveTable[i].setFrequency2(shiftedFrequency(frequency,semitones_2));
                    waveTable[i].setFrequency3(shiftedFrequency(frequency,semitones_3));
                    // normalisier die Velocity
                    waveTable[i].getVelocity(float(midiMessages.getVelocity()) / 127.0);
                    // berechne die ADSR coeffizienten
                    waveTable[i].adsr->calcCoeffs();
                    // Trigger den NoteOn
                    waveTable[i].adsr->noteOn();
                    // Exit loop
                    break;
                }
            }
            
            
        }
        else if(midiMessages.isNoteOff())
        {
            // falls du einen Note off bekommst
            for(int i = 0; i < waveTable.size(); ++i)
            {
                // Falls die ADSR  aktiv ist
                if(waveTable[i].adsr->isActive() == true)
                {
                    // check ob die Midi note einem der Stimmen entspricht
                    if(midiMessages.getNoteNumber() == waveTable[i].returnMidiNote())
                    {
                        // trigger den NoteOff (fade out)
                        waveTable[i].adsr->noteOff();
                        // falls der State der ADSR Idle ist
                        if(waveTable[i].adsr->state == CustomADSR::State::Idle)
                        {
                            // Setz die Note zurück
                            waveTable[i].getMidiNote(0);
                            break;
                        }
                        // all notes off look
                    }
                }
                
            }
        }
    }
        


inline float shiftedFrequency(float frequency, float semitones)
{
    // berechung des Semitone Offsets
    float shiftedFrequency = 0;
    if (semitones == 0)
    {
        shiftedFrequency = frequency;
    }
    else if (semitones > 0)
    {
        shiftedFrequency = frequency * pow(2,semitones / 12);
    }
    else if (semitones < 0)
    {
        shiftedFrequency =  frequency / pow(2, -1.0*(semitones) / 12);
    }
    return shiftedFrequency;
}
    
    
    void getSemitones1(float semitones)
    {
        // Bekomme den Offset von der GUI
        semitones_1 = semitones;
    }
    void getSemitones2(float semitones)
    {
        // Bekomme den Offset von der GUI
        semitones_2 = semitones;
    }
    
    void getSemitones3(float semitones)
    {
        // Bekomme den Offset von der GUI
        semitones_3 = semitones;
    }
    
    
    
    void getAttack(float attack)
    {
        // Bekomme den Attack von der GUI
        m_Attack = attack;
    }
    void getDecay(float decay)
    {
        // Bekomme den Decay von der GUI
        m_Decay = decay;
    }
    
    void getSustain(float sustain)
    {
        // Bekomme den Sustain von der GUI
        m_Sustain = sustain;
    }
    void getRelease(float release)
    {
        // Bekomme den Release von der GUI
        m_Release = release;
    }
    
    void getWaveTable(int index)
    {
            for(int i = 0; i < waveTable.size(); ++i)
            {
                waveTable[i].swapTable(index);
            }
    }
    
    void getWaveTable2(int index)
    {
            for(int i = 0; i < waveTable.size(); ++i)
            {
                waveTable[i].swapTable2(index);
            }
    }
    
    void getWaveTable3(int index)
    {
            for(int i = 0; i < waveTable.size(); ++i)
            {
                waveTable[i].swapTable3(index);
            }
    }
    
    
    
    
    
    void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
    {
        auto currentSample = 0;
        // unsigned int currentSample = 0;
        for (const auto midiMetadata : midiMessages)
        {
            const auto message = midiMetadata.getMessage();
            const auto messagePosition = static_cast<int>(message.getTimeStamp());

            render(buffer, currentSample, messagePosition);
            currentSample = messagePosition;
            handleMidiEvent(message);
        }

        render(buffer, currentSample, buffer.getNumSamples());
        
    }
    
    
    void render(juce::AudioBuffer<float>& buffer, int beginSample, int endSample)
    {
        //überprüfen !!
        subBuffer.makeCopyOf(buffer);
        tempBuffer.makeCopyOf(subBuffer);
        
        auto* firstChannel = tempBuffer.getWritePointer(0);
        // look at if adsr is active look it up
        for(int i = 0; i < waveTable.size(); ++i)
            {
               // if (waveTable[i].adsr->isActive() == true)
              //  {
                    for (auto sample = beginSample; sample < endSample; ++sample)
                    {
                        firstChannel[sample] += waveTable[i].getAllTables();
                    }
             //   }
            }

            for (int channel = 1; channel < buffer.getNumChannels(); ++channel)
            {
                auto* channelData = tempBuffer.getWritePointer(channel);
                std::copy(firstChannel + beginSample, firstChannel + endSample, channelData + beginSample);
            }
        subBuffer.makeCopyOf(tempBuffer);
        buffer.makeCopyOf(subBuffer);
    }
    
    
    std::array<WaveTableOscillator , 16>  waveTable;
   
private:
    
    float m_Attack, m_Sustain, m_Decay, m_Release = 0;
    juce::AudioBuffer<float> tempBuffer;
    juce::AudioBuffer<float> subBuffer;
    int previousIndex = 5;
    float semitones_1, semitones_2, semitones_3;
};
