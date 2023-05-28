/*
  ==============================================================================

    ADSR_gui.h
    Created: 24 Apr 2023 11:51:40pm
    Author:  Onez

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"

class ADSR_GUI : public juce::Component
{
public:
    ADSR_GUI
    (
        juce::RangedAudioParameter* attackTime,
        juce::RangedAudioParameter* attackValue,
        juce::RangedAudioParameter* decayTime,
        juce::RangedAudioParameter* decayValue,
        juce::RangedAudioParameter* sustainValue,
        juce::RangedAudioParameter* sustainTime,
        juce::RangedAudioParameter* releaseTime
     ) :
    _attackTime(*attackTime),
    _attackValue(*attackValue),
    _decayTime(*decayTime),
    _decayValue(*decayValue),
    _sustainValue(*sustainValue),
    _sustainTime(*sustainTime),
    _releaseTime(*releaseTime),
    a_attackTime(_attackTime,[this](float){repaint();},nullptr),
    a_attackValue(_attackValue,[this](float){repaint();},nullptr),
    a_decayTime(_decayTime,[this](float){repaint();},nullptr),
    a_decayValue(_decayValue, [this](float){repaint();},nullptr),
    a_sustainValue(_sustainValue,[this](float){repaint();},nullptr),
    a_sustainTime(_sustainTime,[this](float){repaint();},nullptr),
    a_releaseTime(_releaseTime,[this](float){repaint();},nullptr)
    {
        a_attackTime.sendInitialUpdate();
        a_attackValue.sendInitialUpdate();
        a_decayTime.sendInitialUpdate();
        a_decayValue.sendInitialUpdate();
        a_sustainValue.sendInitialUpdate();
        a_releaseTime.sendInitialUpdate();
    };
    ~ADSR_GUI() {};
    
    void paint (juce::Graphics& g) override
    {
        g.fillAll(juce::Colours::black);
        g.setColour(juce::Colours::white);
        /*
        g.drawRect(attackArea);
        g.drawRect(decayArea);
        g.drawRect(sustainArea);
        g.drawRect(releaseArea);
        */
        g.setColour(juce::Colours::white);
        const int attackPosX = juce::jlimit(10, attackArea.getWidth() - 10, int((_attackTime.getValue()) * offset));
        const int attackPosY  = juce::jlimit(10, attackArea.getHeight() - 10, int( (1.0 -_attackValue.getValue()) * offset));
    
        const int decayPosX = juce::jlimit(decayArea.getX() + 10, offset *2,int(_decayTime.getValue() * offset * 2.0) - 10);
        const int decayPosY  = juce::jlimit(10, decayArea.getHeight() - 10,int( (1.0 -_decayValue.getValue()) * decayArea.getHeight()));
        
        const int sustainPosX = juce::jlimit(sustainArea.getX() + 10, offset*3,int(_sustainTime.getValue() * offset * 3.0) - 10);
        sustainPosY = juce::jlimit(10, sustainArea.getHeight() - 10, int(( 1.0 - _sustainValue.getValue()) * sustainArea.getHeight()));
      
        const int releasePosX = juce::jlimit(releaseArea.getX() + 10, offset*4, int(_releaseTime.getValue() * offset * 4.0) - 10);
      //  const int releasePosY  = juce::jlimit(0, releaseArea.getHeight(), releasePoint.getY());
        
        g.drawLine(attackArea.getBottomLeft().getX(),attackArea.getBottomLeft().getY(),attackPosX + size/2, attackPosY + size/2);
        g.drawLine(attackPosX + size/2, attackPosY + size/2, decayPosX + size/2, decayPosY + size/2);
        g.drawLine( decayPosX + size/2, decayPosY + size/2, sustainPosX + size/2, sustainPosY + size/2);
        g.drawLine(offset*3,sustainPosY, releasePosX + size,  releaseArea.getBottomRight().getY() - size);
       // g.drawLine(offset*4,  sustainPosY + size / 2, releasePosX + size / 2, releaseArea.getBottomRight().getY());
       
        
        g.drawRect(0,0,reducedSize.getWidth(), reducedSize.getHeight());
        
        
        
        g.setColour(attackColor);
       
        g.drawEllipse(attackPosX, attackPosY, size, size, size);
        
        g.setColour(decayColor);
       
        g.drawEllipse(decayPosX, decayPosY, size, size, size);
        
        
        g.setColour(sustainColor);
        g.drawEllipse(sustainPosX,sustainPosY,size,size,size);
        g.drawLine(sustainPosX+ size / 2, sustainPosY + size / 2, offset*3, sustainPosY);
    
        
        g.setColour(releaseColor);
        
        g.drawEllipse(releasePosX,releaseArea.getBottomLeft().getY() - size * 2, size, size, size);
        
        
        

    }
    
    void resized() override
    {
        reducedSize = getLocalBounds().reduced(size * 2);
        offset = reducedSize.getWidth() /4;
        attackArea.setBounds(0, 0,           offset, reducedSize.getHeight());
        decayArea.setBounds(offset,   0, offset, reducedSize.getHeight());
        sustainArea.setBounds(offset * 2, 0, offset, reducedSize.getHeight());
        releaseArea.setBounds(offset * 3, 0, offset, reducedSize.getHeight());
    }
    
    void mouseDown(const juce::MouseEvent &event) override
    {
        if(attackArea.contains(event.getPosition()))
        {
            attackPoint = event.getPosition();
            a_attackTime.beginGesture();
            a_attackValue.beginGesture();
            const float time = attackPoint.getX() / offset;
            const float value = attackPoint.getY() / reducedSize.getHeight();
            _attackTime.setValueNotifyingHost(time);
            _attackValue.setValueNotifyingHost(1.0 - value);
            a_attackTime.endGesture();
            a_attackValue.endGesture();
           // a_attackTime.setValueNotifyingHost():
        }
        if(decayArea.contains(event.getPosition()))
        {
            decayPoint = event.getPosition();
            decayColor = juce::Colours::red;
            a_decayTime.beginGesture();
            a_decayValue.beginGesture();
            const float time = decayPoint.toFloat().getX() / (offset * 2.0);
            const float value = decayPoint.toFloat().getY() / reducedSize.getHeight();
            _decayTime.setValueNotifyingHost(time);
            _decayValue.setValueNotifyingHost(1.0 - value);
            a_decayTime.endGesture();
            a_decayValue.endGesture();
            
            
            
        }
        if(sustainArea.contains(event.getPosition()))
        {
            sustainPoint = event.getPosition();
            a_sustainValue.beginGesture();
            a_sustainTime.beginGesture();
            const float value = sustainPoint.toFloat().getY() / reducedSize.getHeight();
            const float time = sustainPoint.toFloat().getX() / (offset * 3.0);
            _sustainValue.setValueNotifyingHost( 1.0 - value);
            _sustainTime.setValueNotifyingHost(time);
            a_sustainValue.endGesture();
            a_sustainTime.endGesture();
        }
        if(releaseArea.contains(event.getPosition()))
        {
            releasePoint = event.getPosition();
            releaseColor = juce::Colours::red;
            a_releaseTime.beginGesture();
            const float value = releasePoint.toFloat().getX() / (offset * 4.0);
            _releaseTime.setValueNotifyingHost(value);
            a_releaseTime.endGesture();
        }
        
        
        
    }
    
    
    
    void mouseDrag (const juce::MouseEvent &event) override
    {
       
        if(attackArea.contains(event.getPosition()))
        {
            attackPoint = event.getPosition();
            attackColor = juce::Colours::red;
            a_attackTime.beginGesture();
            a_attackValue.beginGesture();
            const float time = attackPoint.toFloat().getX() / offset;
            const float value = attackPoint.toFloat().getY() / reducedSize.getHeight();
            _attackTime.setValueNotifyingHost(time);
            _attackValue.setValueNotifyingHost(1.0 - value);
            a_attackTime.endGesture();
            a_attackValue.endGesture();
           
        }
        else
        {
            attackColor = juce::Colours::white;
           
        }
    
        if(decayArea.contains(event.getPosition()))
        {
            decayPoint = event.getPosition();
            decayColor = juce::Colours::red;
            a_decayTime.beginGesture();
            a_decayValue.beginGesture();
           
            const float time = decayPoint.toFloat().getX() / (offset * 2.0);
            const float value = decayPoint.toFloat().getY() / decayArea.getHeight();
            _decayTime.setValueNotifyingHost(time);
            _decayValue.setValueNotifyingHost(1.0 - value);
            a_decayTime.endGesture();
            a_decayValue.endGesture();
        } else
        {
            decayColor = juce::Colours::white;
        }
        if(sustainArea.contains(event.getPosition()))
        {
            sustainPoint = event.getPosition();
            sustainColor = juce::Colours::red;
            a_sustainValue.beginGesture();
            a_sustainTime.beginGesture();
            const float value = sustainPoint.toFloat().getY() / reducedSize.getHeight();
            const float time = sustainPoint.toFloat().getX() / (offset * 3.0);
            _sustainValue.setValueNotifyingHost(1.0 - value);
            _sustainTime.setValueNotifyingHost(time);
            a_sustainValue.endGesture();
            a_sustainTime.endGesture();
        } else
        {
            sustainColor = juce::Colours::white;
        }
        if(releaseArea.contains(event.getPosition()))
        {
            releasePoint = event.getPosition();
            releaseColor = juce::Colours::red;
            a_releaseTime.beginGesture();
            const float time = releasePoint.toFloat().getX() / (offset * 4.0);
            _releaseTime.setValueNotifyingHost(time);
            a_releaseTime.endGesture();
            
        } else
        {
            releaseColor = juce::Colours::white;
        }
        
    }
    
    void mouseEnter (const juce::MouseEvent &event) override
    {
        
        if(attackArea.contains(event.getPosition()))
        {
            attackColor = juce::Colours::red;
        }
        else
        {
            attackColor = juce::Colours::white;
        }
        if(decayArea.contains(event.getPosition()))
        {
            decayColor = juce::Colours::red;
        } else
        {
            decayColor = juce::Colours::white;
        }
        if(sustainArea.contains(event.getPosition()))
        {
            sustainColor = juce::Colours::red;
        } else
        {
            sustainColor = juce::Colours::white;
        }
        if(releaseArea.contains(event.getPosition()))
        {
            releaseColor = juce::Colours::red;
        } else
        {
            releaseColor = juce::Colours::white;
        }
        
        
    }
    
    void mouseExit(const juce::MouseEvent &event) override
    {
        if(!reducedSize.contains(event.getPosition()))
        {
            attackColor = juce::Colours::white;
            decayColor = juce::Colours::white;
            sustainColor = juce::Colours::white;
            releaseColor = juce::Colours::white;
        }
    }
    
    void mouseMove (const juce::MouseEvent &event) override
    {
        if(attackArea.contains(event.getPosition()))
        {
            attackColor = juce::Colours::red;
         //   dragattack = true;
        }
        else
        {
            attackColor = juce::Colours::white;
         //   dragattack = false;
        }
        if(decayArea.contains(event.getPosition()))
        {
            decayColor = juce::Colours::red;
        } else
        {
            decayColor = juce::Colours::white;
        }
        if(sustainArea.contains(event.getPosition()))
        {
            sustainColor = juce::Colours::red;
        } else
        {
            sustainColor = juce::Colours::white;
        }
        if(releaseArea.contains(event.getPosition()))
        {
            releaseColor = juce::Colours::red;
        } else
        {
            releaseColor = juce::Colours::white;
        }
    }
    
    void mouseUp(const juce::MouseEvent &event) override
    {
    }
    
   
    
private:
    // parameters
    juce::RangedAudioParameter &_attackTime, &_attackValue;
    juce::RangedAudioParameter &_decayTime;
    juce::RangedAudioParameter &_decayValue;
    juce::RangedAudioParameter &_sustainValue;
    juce::RangedAudioParameter &_sustainTime;
    juce::RangedAudioParameter &_releaseTime;
        // attachments
    juce::ParameterAttachment a_attackTime,a_attackValue;
    juce::ParameterAttachment a_decayTime;
    juce::ParameterAttachment a_decayValue;
    juce::ParameterAttachment a_sustainValue;
    juce::ParameterAttachment a_sustainTime;
    juce::ParameterAttachment a_releaseTime;
    
    
    
    juce::Point<int> attackPoint;
    juce::Point<int> decayPoint;
    juce::Point<int> sustainPoint;
    juce::Point<int> releasePoint;
    juce::Rectangle<int> reducedSize;
    juce::Rectangle<int> attackArea;
    juce::Rectangle<int> decayArea;
    juce::Rectangle<int> sustainArea;
    juce::Rectangle<int> releaseArea;
    juce::Colour attackColor = juce::Colours::white;
    juce::Colour decayColor = juce::Colours::white;
    juce::Colour sustainColor = juce::Colours::white;
    juce::Colour releaseColor = juce::Colours::white;
    int offset = 0;
    float size = 5;
    int sustainPosY = 0;
};



