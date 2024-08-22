/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class ProcoRatAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    ProcoRatAudioProcessorEditor (ProcoRatAudioProcessor&);
    ~ProcoRatAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    ProcoRatAudioProcessor& audioProcessor;
    juce::Slider distortionSlider;
    juce::Slider toneSlider;
    juce::Slider volumeSlider;

    juce::Label distortionLabel;
    juce::Label toneLabel;
    juce::Label volumeLabel;
    //juce::TextButton oversampleButton;

    //void buttonClicked(juce::Button* button) override;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> distortionAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> toneAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> volumeAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ProcoRatAudioProcessorEditor)
};
