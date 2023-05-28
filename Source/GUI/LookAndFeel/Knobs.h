/*
  ==============================================================================

    Knobs.h
    Created: 30 Apr 2023 10:53:40am
    Author:  Onez

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"

// check this video https://www.youtube.com/watch?v=SLpeh-q3oIE how to create our own
namespace juce
{
class Zen_BlueKnob : public LookAndFeel_V4
{
public:
    Zen_BlueKnob() {};
    ~Zen_BlueKnob() {};
    
    
    void drawRotarySlider (Graphics& g, int x, int y, int width, int height, float sliderPos,
                           const float rotaryStartAngle, const float rotaryEndAngle, Slider& slider) override
    {
       // g.setColour(juce::Colours::white);
       // g.drawRect(0,0,width,height);
        auto color =  juce::Colours::blue;
        auto bounds = Rectangle<int> (x, y, width, height).toFloat().reduced (10);
        
        auto radius = jmin (bounds.getWidth(), bounds.getHeight()) / 2.0f;
        auto toAngle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
        auto lineW = jmin (8.0f, radius * 0.5f);
        auto arcRadius = radius - lineW * 0.5f;
        
        Path backgroundArc;
        backgroundArc.addCentredArc (bounds.getCentreX(),
                                     bounds.getCentreY(),
                                     arcRadius,
                                     arcRadius,
                                     0.0f,
                                     rotaryStartAngle,
                                     rotaryEndAngle,
                                     true);
        
        g.setColour (color);
        g.strokePath (backgroundArc, PathStrokeType (lineW, PathStrokeType::curved, PathStrokeType::rounded));
        
        if (slider.isEnabled())
        {
            Path valueArc;
            valueArc.addCentredArc (bounds.getCentreX(),
                                    bounds.getCentreY(),
                                    arcRadius,
                                    arcRadius,
                                    0.0f,
                                    rotaryStartAngle,
                                    toAngle,
                                    true);
            
            g.setColour (color);
            g.strokePath (valueArc, PathStrokeType (lineW, PathStrokeType::curved, PathStrokeType::rounded));
        }
        
        auto thumbWidth = lineW * 2.0f;
        Point<float> thumbPoint (bounds.getCentreX()  + 0.8 *arcRadius * std::cos (toAngle - MathConstants<float>::halfPi),
                                 bounds.getCentreY() + 0.8* arcRadius * std::sin (toAngle - MathConstants<float>::halfPi));
        Line<float> thumbCenter (bounds.getCentreX()  + 0.8 *arcRadius * std::cos (toAngle - MathConstants<float>::halfPi),
                                 bounds.getCentreY() + 0.8* arcRadius * std::sin (toAngle - MathConstants<float>::halfPi),
                                 bounds.getCentreX(), bounds.getCentreY());
        
        g.setColour (color);
        g.fillEllipse (Rectangle<float> (thumbWidth, thumbWidth).withCentre (thumbPoint));
        Path sliderInternal;
        sliderInternal.addLineSegment(thumbCenter, lineW);
        g.fillPath(sliderInternal);
        
        
        
        
        
        
    }
};
}

namespace juce {

class Custom_Knob : public Slider
{
public:
    Custom_Knob()
    {
        setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
        setTextBoxStyle(TextBoxBelow, false, 100, 50);
    };
    ~Custom_Knob()  override
    {
        setLookAndFeel(nullptr);
    }
    enum class ZenStyles
    {
        blueStyle
    };
    void setStyles(ZenStyles styles)
    {
        switch(styles)
        {
            case ZenStyles::blueStyle:
                setLookAndFeel(&blueStyle);
                break;
                
        }
        
    }
private:
    
    void mouseEnter(const MouseEvent& event) override
    {
        
        setColour(ColourIds::textBoxTextColourId, findColour(Slider::ColourIds::textBoxTextColourId).withAlpha(1.0f));
        
    };
    void mouseExit(const MouseEvent& event) override
    {
        
        setColour(ColourIds::textBoxTextColourId, findColour(Slider::ColourIds::textBoxTextColourId).withAlpha(0.5f));
        
    };
    
    
   
    Zen_BlueKnob blueStyle;
    
};
}



