/*
  ==============================================================================

    CustomADSR.h
    Created: 19 Apr 2023 3:28:52pm
    Author:  Onez

  ==============================================================================
*/

#pragma once 
#include "JuceHeader.h"
class CustomADSR
{
public:
    CustomADSR(){};
    ~CustomADSR(){};
    
    void prepare(float sampleRate, int blocksize)
    {
        m_sampleRate = sampleRate;
        bs =blocksize;
    }
    void calcCoeffs()
    {
            
        //überprüfen
            auto getRate = [] (float distance, float timeInSeconds,double sr)
            {
                return timeInSeconds > 0.0f ? (float) (distance / (timeInSeconds * sr)) : -1.0f;
            };
        coefpoint1 = getRate(attackValue, attackTime, m_sampleRate);
        coefpoint2 = getRate(decayValue - attackValue,  decayTime, m_sampleRate);
        coefpoint3 = getRate(sustainValue - decayValue, sustainTime, m_sampleRate);
        coefpoint4 = getRate(sustainValue, releaseTime, m_sampleRate);
        stage1bool = attackValue < decayValue;
        stage2bool = decayValue < sustainValue;
        
    }
    
    void getAttackTime(float newValue)
    {
        attackTime = newValue;
    }

    void getAttackValue(float newValue)
    {
        attackValue = newValue;
    }
    
    void getDecayTime(float newValue)
    {
        decayTime = newValue;
    }
    
    void getDecayValue(float newValue)
    {
        decayValue = newValue;
    }
    
    void getSustainValue(float newValue)
    {
        sustainValue = newValue;
    }
    void getSustainTime(float newValue)
    {
        sustainTime = newValue;
    }
    
    void getReleaseTime(float newValue)
    {
        releaseTime = newValue;
    }
    
    
    
    float getRate (float distance, float timeInSeconds,double sr)
    {
        return timeInSeconds > 0.0f ? (float) (distance / (timeInSeconds * sr)) : -1.0f;
    };
    
    void noteOn ()
    {
        output = 0.0;
        state = State::Attack;
        // überprüfen
        sustainStage = false;
    }
    
    
    void noteOff()
    {
        if(state != State::Idle)
        {
            //überprüfen
            if(releaseTime > 0.0f)
            {
                  if(sustainStage == false)
                  {
                      coefpoint4 = getRate(output,releaseTime,m_sampleRate);
                  }
                state = State::Release;
            } else
            {
                reset();
            }
        }
        
    }
    
    bool isActive()
    {
        bool currentState = true;
        if (state == State::Idle)
        {
            currentState = false;
        }
        return currentState;
    }
    
    
    void reset()
    {
        state = State::Idle;
        output = 0.0f;
    }
    float process()
    {
        switch (state)
        {
            case State::Idle:
                output = 0.0;
                break;
                    
            case State::Attack:
                    output += coefpoint1;
                // falls der output > attackValue dann trigger decay
                    if(output >= attackValue)
                    {
                        output = attackValue;
                        state = State::Decay;
                    }
                break;
                    
            case State::Decay:
                if(stage1bool == true)
                {
                    output += coefpoint2;
                    if(output >= decayValue)
                    {
                        output = decayValue;
                        state = State::Sustain;
                    }
                }
                else
                {
                    output += coefpoint2;
                    if(output <= decayValue)
                    {
                        output = decayValue;
                        state = State::Sustain;
                    }
                }
                break;
                    
            case State::Sustain:
                if(stage2bool == true)
                {
                    output += coefpoint3;
                    if(output >= sustainValue)
                    {
                        sustainStage = true;
                        output =  sustainValue;
                    }
                }
                else
                {
                    output += coefpoint3;
                    if(output <= sustainValue)
                    {
                        sustainStage = true;
                        output = sustainValue;
                    }
                    
                }
                break;
            case State::Release:
                output -= coefpoint4;
                if(output <= 0.0)
                {
                    output = 0.0;
                    state = State::Idle;
                }
                break;
            default : State::Idle;
        }
        return output;
        
       
    }
    
    
    enum class State
        {
            Idle,
            Attack,
            Decay,
            Sustain,
            Release
        };
    State state;
private:
    float releaseVal = 0.0;
    float attackBase = 0;
    float decayBase = 0;
    float sustainBase = 0;
    float releaseBase = 0;
    float samp_Time;
    float m_sampleRate;
    bool stage1bool = false;
    bool stage2bool = false;
    bool stage3bool = false;
    bool sustainStage = false;
    float point1 = 0;
    float point2 = 0;
    float point3 = 0;
    float point4 = 0;
    float coefpoint1 = 0;
    float coefpoint2 = 0;
    float coefpoint3 = 0;
    float coefpoint4 = 0;
    float attackTime = 0;
    float attackValue = 0;
    float decayTime = 0;
    float decayValue = 0;
    float sustainTime = 0;
    float sustainValue = 0;
    float releaseTime = 0;
    float val1 = 0;
    float val2 = 0;
    float val3 = 0;
    float val4 = 0;
    float output = 0;
   
    float envOut = 0;
    float bs = 0;
    float bsrate = 0;
   
};
