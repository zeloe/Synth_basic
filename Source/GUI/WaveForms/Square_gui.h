/*
  ==============================================================================

    Square_Gui.h
    Created: 27 Apr 2023 12:06:10am
    Author:  Onez

  ==============================================================================
*/

#pragma once
class Square_Gui : public juce::Component
{
public:
    Square_Gui(juce::Colour colorToPaint) :
    color(colorToPaint),
    paintColour(colorToPaint)
    {
        setRepaintsOnMouseActivity(true);
    };
    ~Square_Gui(){};
    
    void paint(juce::Graphics& g) override
    {
        g.fillAll (juce::Colours::black);
        g.setColour(paintColour);
        g.drawRect(0, 0, getLocalBounds().getWidth(), getLocalBounds().getHeight());
        auto mid = reducedSize.getWidth() / 2;
        auto offsety = reducedSize.getHeight() * 0.15;
        auto offsetx =  reducedSize.getWidth() * 0.15;
        auto bottom = reducedSize.getHeight();
        auto bottomRight = reducedSize.getBottomRight().getX() - offsetx;
        g.drawLine(offsetx, offsety, mid, offsety);
        g.drawLine(mid,offsety,mid, bottom);
        g.drawLine(mid,bottom,bottomRight, bottom);
        
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
    int size = 10;
    juce::Rectangle<int> reducedSize;
    juce::Colour color;
    juce::Colour paintColour;
    
};
