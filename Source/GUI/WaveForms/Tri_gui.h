/*
  ==============================================================================

    Tri_gui.h
    Created: 27 Apr 2023 12:06:49am
    Author:  Onez

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
class Tri_Gui : public juce::Component
{
public:
    Tri_Gui(juce::Colour colorToPaint) :
    color(colorToPaint),
    paintColour(colorToPaint)
   {
        setRepaintsOnMouseActivity(true);
    };
   ~Tri_Gui(){};
   
   void paint(juce::Graphics& g) override
   {
       g.fillAll (juce::Colours::black);
       g.setColour(paintColour);
       g.drawRect(0, 0, getLocalBounds().getWidth(), getLocalBounds().getHeight());
       auto mid = reducedSize.getWidth() / 2 - reducedSize.getWidth() * 0.01;
       auto midy = reducedSize.getHeight() * 0.1 - reducedSize.getHeight() * 0.01;
       auto bottomXL = reducedSize.getBottomLeft().getX();
       auto bottomYL = reducedSize.getBottomLeft().getY();
       auto bottomXR = reducedSize.getBottomRight().getX();
       auto bottomYR = reducedSize.getBottomRight().getY();
       
       
       g.drawLine(bottomXL, bottomYL, mid, midy);
       g.drawLine(mid,midy, bottomXR, bottomYR  );
       
   }
   
   void resized() override
   {
       reducedSize = juce::Rectangle<int>(0,0,getWidth(), getHeight() * 0.75);
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
   juce::Rectangle<int> reducedSize;
   
    juce::Colour color;
    juce::Colour paintColour;
};
