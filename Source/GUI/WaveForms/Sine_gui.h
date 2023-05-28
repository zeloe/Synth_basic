/*
  ==============================================================================

    Sine_gui.h
    Created: 27 Apr 2023 12:05:59am
    Author:  Onez

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
class Sine_Gui : public juce::Component
{
public:
    Sine_Gui(juce::Colour colorToPaint) :
    color(colorToPaint),
    paintColour(colorToPaint)
   {
        setRepaintsOnMouseActivity(true);
    };
   ~Sine_Gui(){};
   
   void paint(juce::Graphics& g) override
   {
       g.fillAll (juce::Colours::black);
       g.setColour(paintColour);
       g.drawRect(0, 0, getLocalBounds().getWidth(), getLocalBounds().getHeight());
       auto area = reducedSize;
       float topY = (float) area.getY();
       float bottomY = (float) area.getBottom();
       float midY = (topY + bottomY) * 0.5f;
       float vscale = 0.4 * (bottomY - topY) / (float)area.getWidth();
       juce::RectangleList<float> waveform;
       float x = (float)area.getX();
       for (int w = area.getWidth(); --w >= 0;)
       {
               float res   = std::sin(2 * M_PI * float(w) / float(area.getWidth())) * float(area.getWidth());
               float top = juce::jmax (midY - res  * vscale - 0.3f, topY);
               float bottom = juce::jmin(midY - res * vscale + 0.3f, bottomY);
               waveform.addWithoutMerging (juce::Rectangle<float> (x, top, 1.0f,bottom - top));
       
           x += 1.0f;
       }

       g.fillRectList (waveform);
       
   }
   
   void resized() override
   {
       reducedSize = juce::Rectangle<int>(0,0,getWidth(), getHeight() * 0.75);
       reducedSize.removeFromTop(reducedSize.getHeight() * 0.1);
       reducedSize.removeFromBottom(reducedSize.getHeight() * 0.1);
       reducedSize.removeFromLeft(reducedSize.getWidth() * 0.1);
       reducedSize.removeFromRight(reducedSize.getWidth() * 0.1);
   }
    void mouseExit(const juce::MouseEvent &event) override
    {
        if(!getLocalBounds().contains(event.getPosition()))
            paintColour = color.darker();
    }
    void mouseEnter(const juce::MouseEvent &event) override
    {
        if(getLocalBounds().contains(event.getPosition()))
        {
            paintColour = color;
        }
    }
    void mouseMove (const juce::MouseEvent &event) override
    {
        if(getLocalBounds().contains(event.getPosition()))
            paintColour = color;
    }


   
   
private:
   int size = 10;
   juce::Rectangle<int> reducedSize;
    juce::Colour color;
    juce::Colour paintColour;
  
};
