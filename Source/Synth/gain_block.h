
#include "JuceHeader.h"
//not really sure about using this approach
class Gain_Block
{
public:
    Gain_Block():
    gainSmooth(std::make_unique<juce::LinearSmoothedValue<float>>())
    
    {};
    ~Gain_Block() {};
    
    void prepare(float sampleRate)
    {
        gainSmooth->reset(sampleRate, 0.01);
    }
    
    void setGain(float gain)
    {
        gainSmooth->setTargetValue(gain);
    }
    
    float process() noexcept
    {
        return gainSmooth->getNextValue();
    }
    
private:
    std::unique_ptr<juce::LinearSmoothedValue<float>> gainSmooth;
    float temp_gain;
    float current_gain;
    float gain_inc = 0;
    float result = 0;
    size_t bs = 0;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Gain_Block);
};
