/*
  ==============================================================================

    WaveFormPopUpMenu.h
    Created: 27 Apr 2023 12:08:49am
    Author:  Onez

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "WaveForms/Saw_gui.h"
#include "WaveForms/Sine_gui.h"
#include "WaveForms/Square_gui.h"
#include "WaveForms/Tri_gui.h"
class WaveFormPopUpMenu :public juce::Component
{
public:
    WaveFormPopUpMenu(juce::RangedAudioParameter* _waveForm, juce::Colour colorToPaint) :
    waveForm(*_waveForm),
    a_waveForm(waveForm,[this](int){repaint();},nullptr),
    paintColor(colorToPaint)
    {
        a_waveForm.sendInitialUpdate();
        popUpMenu = std::make_unique<juce::PopupMenu>();
        sine = std::make_unique<Sine_Gui>(paintColor);
        saw = std::make_unique<Saw_gui>(paintColor);
        square = std::make_unique<Square_Gui>(paintColor);
        tri = std::make_unique<Tri_Gui>(paintColor);
    };
    ~WaveFormPopUpMenu()
    {
    };
    
    void resized() override
    {
        reducedSize = getLocalBounds().reduced(size);
        menuSize = juce::Rectangle<int>(0,0,getWidth() * 0.5, getHeight() * 0.75);
        menu1 = juce::Rectangle<int>(0,menuSize.getHeight(),getWidth() * 0.5, getHeight() * 0.75);
       
      
        
    } ;
    
    void paint(juce::Graphics& g) override
    {
        setWaveform((waveForm.getValue() / 100 * 300), g);
    };
    
    

    
    void mouseDown (const juce::MouseEvent& e) override
    {
       
        popUpMenu->clear();
        popUpMenu->addCustomItem(1,*(sine.get()), menuSize.getWidth(),  menuSize.getHeight(), true, nullptr, " ");
        popUpMenu->addCustomItem(2,*(saw.get()),menuSize.getWidth(),  menuSize.getHeight(), true, nullptr, " ");
        popUpMenu->addCustomItem(3,*(square.get()), menuSize.getWidth(),  menuSize.getHeight(), true, nullptr, " ");
        popUpMenu->addCustomItem(4,*(tri.get()), menuSize.getWidth(),  menuSize.getHeight(), true, nullptr, " ");
        
        
        saw->getPosition(e.getPosition());
        saw->getMenu(saw->getLocalBounds());
        popUpMenu->showMenuAsync(juce::PopupMenu::Options(),
                                 [this](int result)
                                 {
            
            
            if (result == 0)
            {
            }
            else if(result == 1)
            {
               
                //  a_waveForm.beginGesture();
                a_waveForm.setValueAsCompleteGesture(0);
                //a_waveForm.endGesture();
                
            }
            else if(result == 2)
            {
               
                //   a_waveForm.beginGesture();
                a_waveForm.setValueAsCompleteGesture(1);
                //    a_waveForm.endGesture();
            }
            else if(result == 3)
            {
                //   a_waveForm.beginGesture();
                a_waveForm.setValueAsCompleteGesture(2);
                //   a_waveForm.endGesture();
            }
            else if(result == 4)
            {
                //   a_waveForm.beginGesture();
                a_waveForm.setValueAsCompleteGesture(3);
                //   a_waveForm.endGesture();
            }
            
            repaint();
            
        });
    }
    
    
   
    void setWaveform(int index, juce::Graphics& g)
    {
        switch(index)
        {
            case 0:
                paintSine(g);
                break;
                
            case 1:
                paintSawtooth(g);
                break;
                
            case 2:
                paintSquare(g);
                break;
                
            case 3:
                paintTriangle(g);
                break;
                
        }
        
    }
    
    
    void paintSine(juce::Graphics& g)
    {
        g.setColour(paintColor);
        g.drawRect(0,0,getLocalBounds().getWidth(), getLocalBounds().getHeight());
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
    void paintSawtooth(juce::Graphics& g)
    {
        g.setColour(paintColor);
        g.drawRect(0,0,getLocalBounds().getWidth(), getLocalBounds().getHeight());
        auto topX = reducedSize.getX();
        auto topY = reducedSize.getY();
        
        auto bottomXR = reducedSize.getBottomRight().getX();
        auto bottomYR = reducedSize.getBottomRight().getY();
        
        auto bottomXL = reducedSize.getBottomLeft().getX();
        auto bottomYL = reducedSize.getBottomLeft().getY();
    
        g.drawLine(topX, topY, bottomXR, bottomYR);
        g.drawLine(topX, topY, bottomXL, bottomYL);
    }
    
    void paintTriangle(juce::Graphics& g)
    {
        g.setColour(paintColor);
        g.drawRect(0,0,getLocalBounds().getWidth(), getLocalBounds().getHeight());
        auto mid = getLocalBounds().getWidth() / 2;
        auto midy = 10;
        auto bottomXL = reducedSize.getBottomLeft().getX();
        auto bottomYL = reducedSize.getBottomLeft().getY();
        auto bottomXR = reducedSize.getBottomRight().getX();
        auto bottomYR = reducedSize.getBottomRight().getY();
        
        
        g.drawLine(bottomXL, bottomYL, mid, midy);
        g.drawLine(mid,midy, bottomXR, bottomYR  );
    }
    
    void paintSquare(juce::Graphics& g)
    {
        g.setColour(paintColor);
        g.drawRect(0,0,getLocalBounds().getWidth(), getLocalBounds().getHeight());
        auto mid = reducedSize.getWidth() / 2;
        auto offsety = reducedSize.getHeight() * 0.15;
        auto offsetx =  reducedSize.getWidth() * 0.15;
        auto bottom = reducedSize.getHeight();
        auto bottomRight = reducedSize.getBottomRight().getX() - offsetx;
        g.drawLine(offsetx, offsety, mid, offsety);
        g.drawLine(mid,offsety,mid, bottom);
        g.drawLine(mid,bottom,bottomRight, bottom);
    }
    
    
   
    
private:
    std::unique_ptr<juce::PopupMenu> popUpMenu;
    std::unique_ptr<Sine_Gui> sine;
    std::unique_ptr<Saw_gui> saw;
    std::unique_ptr<Square_Gui> square;
    std::unique_ptr<Tri_Gui> tri;
    juce::Rectangle<int> reducedSize;
    juce::Rectangle<int> menuSize;
    juce::Rectangle<int> menu1;
    juce::Rectangle<int> menu2;
    juce::Rectangle<int> menu3;
    juce::Rectangle<int> menu4;
    int size = 10;
    int index = 1;
    
    juce::RangedAudioParameter &waveForm;
    juce::ParameterAttachment a_waveForm;
    juce::Colour paintColor;
};
