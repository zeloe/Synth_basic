/*
  ==============================================================================

    PopUpLook.h
    Created: 27 Apr 2023 2:57:34pm
    Author:  Onez

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
namespace juce
{
class PopUpLook : public juce::LookAndFeel_V4
{
public:
    PopUpLook() {};
    ~PopUpLook() {};
    void drawPopupMenuItem (Graphics& g, const Rectangle<int>& area,
                            const bool isSeparator, const bool isActive,
                            const bool isHighlighted, const bool isTicked,
                            const bool hasSubMenu, const String& text,
                            const String& shortcutKeyText,
                            const Drawable* icon, const Colour* const textColourToUse) override
    {
        if (isSeparator)
        {
            auto r  = area.reduced (5, 0);
            r.removeFromTop (roundToInt (((float) r.getHeight() * 0.5f) - 0.5f));
            
            g.setColour (juce::Colours::white.withAlpha (0.3f));
            g.fillRect (r.removeFromTop (1));
        }
        else
        {
            auto textColour = (textColourToUse == nullptr ? findColour (PopupMenu::textColourId)
                               : *textColourToUse);
            
            auto r  = area.reduced (1);
            
            if (isHighlighted && isActive)
            {
                g.setColour ((juce::Colours::grey));
                g.fillRect (r);
                
                g.setColour (findColour (PopupMenu::highlightedTextColourId));
            }
            else
            {
                g.setColour (textColour.withMultipliedAlpha (isActive ? 1.0f : 0.5f));
            }
            
            r.reduce (jmin (5, area.getWidth() / 20), 0);
            
            auto font = getPopupMenuFont();
            
            auto maxFontHeight = (float) r.getHeight() / 1.3f;
            
            if (font.getHeight() > maxFontHeight)
                font.setHeight (maxFontHeight);
            
            g.setFont (font);
            
            auto iconArea = r.removeFromLeft (roundToInt (maxFontHeight)).toFloat();
            
            if (icon != nullptr)
            {
                icon->drawWithin (g, iconArea, RectanglePlacement::centred | RectanglePlacement::onlyReduceInSize, 1.0f);
                r.removeFromLeft (roundToInt (maxFontHeight * 0.5f));
            }
            else if (isTicked)
            {
                auto tick = getTickShape (1.0f);
                g.fillPath (tick, tick.getTransformToScaleToFit (iconArea.reduced (iconArea.getWidth() / 5, 0).toFloat(), true));
            }
            
            if (hasSubMenu)
            {
                auto arrowH = 0.6f * getPopupMenuFont().getAscent();
                
                auto x = static_cast<float> (r.removeFromRight ((int) arrowH).getX());
                auto halfH = static_cast<float> (r.getCentreY());
                
                Path path;
                path.startNewSubPath (x, halfH - arrowH * 0.5f);
                path.lineTo (x + arrowH * 0.6f, halfH);
                path.lineTo (x, halfH + arrowH * 0.5f);
                
                g.strokePath (path, PathStrokeType (2.0f));
            }
            
            r.removeFromRight (3);
            g.drawFittedText (text, r, Justification::centredLeft, 1);
            
            if (shortcutKeyText.isNotEmpty())
            {
                auto f2 = font;
                f2.setHeight (f2.getHeight() * 0.75f);
                f2.setHorizontalScale (0.95f);
                g.setFont (f2);
                
                g.drawText (shortcutKeyText, r, Justification::centredRight, true);
            }
        }
    }
    
    
    
};
};
class popup : public juce::PopupMenu
{
public:
    popup()
    {
        setLookAndFeel(&popUpLook);
    };
    
    ~popup()  {
        setLookAndFeel(nullptr);
    };
    
private:
    
    juce::PopUpLook popUpLook;
};
