/*
  ==============================================================================

    Saw_gui.h
    Created: 27 Apr 2023 12:06:28am
    Author:  Onez

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
class Saw_gui : public juce::Component
{
public:
    Saw_gui(juce::Colour& colorToPaint) :
    color(colorToPaint.darker()),
    paintColour(colorToPaint.darker())
    {
        addMouseListener(this,true);
        setRepaintsOnMouseActivity(true);
       
    };
    ~Saw_gui()
    {
        removeMouseListener(this);
    };
    
    void paint(juce::Graphics& g) override
    {
        g.fillAll (juce::Colours::black);
        g.setColour(paintColour);
        g.drawRect(0, 0, getLocalBounds().getWidth(), getLocalBounds().getHeight());
        auto topX = reducedSize.getX();
        auto topY = reducedSize.getY();
        
        auto bottomXR = reducedSize.getBottomRight().getX();
        auto bottomYR = reducedSize.getBottomRight().getY();
        
        auto bottomXL = reducedSize.getBottomLeft().getX();
        auto bottomYL = reducedSize.getBottomLeft().getY();
    
        g.drawLine(topX, topY, bottomXR, bottomYR);
        g.drawLine(topX, topY, bottomXL, bottomYL);
        
    }
    
    void resized() override
    {
        size_of = juce::Rectangle<int>(0,0,getWidth(), getHeight());
        reducedSize = juce::Rectangle<int>(0,0,getWidth(), getHeight() * 0.75);
        reducedSize.removeFromTop(reducedSize.getHeight() * 0.1);
        reducedSize.removeFromBottom(reducedSize.getHeight() * 0.1);
        reducedSize.removeFromLeft(reducedSize.getWidth() * 0.1);
        reducedSize.removeFromRight(reducedSize.getWidth() * 0.1);
    }
    
 
        
    void setColor(juce::Colour newValue)
    {
        paintColour = newValue;
    }
   
    void getPosition(juce::Point<int> point)
    {
        pointXY = point;
    }
    
    void getMenu(juce::Rectangle<int> rect)
    {
        parentRect = rect;
       
    }
   
    void mouseEnter(const juce::MouseEvent& e) override
        {
            juce::Rectangle<int> rect(getScreenX(), getScreenY(), getLocalBounds().getWidth(), getLocalBounds().getHeight());
            if(getLocalBounds().contains(e.getPosition()))
            {
                paintColour = color.brighter().brighter();
                isMouseOverItem = true;
            }
            if(!rect.contains(e.getScreenPosition()))
            {
                paintColour = color.darker().darker();
                isMouseOverItem = false;
            }
        }

        void mouseMove(const juce::MouseEvent& e) override
        {
            juce::Rectangle<int> rect(getScreenX(), getScreenY(), getLocalBounds().getWidth(), getLocalBounds().getHeight());

            if(getLocalBounds().contains(e.getPosition()))
            {
                paintColour = color.brighter().brighter();
                isMouseOverItem = true;
            }
            if(!rect.contains(e.getScreenPosition()))
            {
                paintColour = color.darker().darker();
                isMouseOverItem = false;
            }
        }

        void mouseExit(const juce::MouseEvent& e) override
        {
                if (!isMouseOverItem)
                {
                    paintColour = color.darker().darker();
                }
                else
                {
                    paintColour = color.brighter().brighter();
                }
        }



   
    
private:
    int size = 10;
    juce::Rectangle<int> size_of;
    juce::Rectangle<int> parentRect;
    juce::Rectangle<int> reducedSize;
    juce::Colour color;
    juce::Colour paintColour;
    bool isMouseOverItem = false;
    juce::Point<int> pointXY;
};
