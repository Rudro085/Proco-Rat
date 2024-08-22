/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ProcoRatAudioProcessorEditor::ProcoRatAudioProcessorEditor (ProcoRatAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(400, 300);
    distortionSlider.setSliderStyle(juce::Slider::Rotary);
    distortionSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    addAndMakeVisible(distortionSlider);

    toneSlider.setSliderStyle(juce::Slider::Rotary);
    toneSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    addAndMakeVisible(toneSlider);

    volumeSlider.setSliderStyle(juce::Slider::Rotary);
    volumeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    addAndMakeVisible(volumeSlider);

    // Add labels
    distortionLabel.setText("Distortion", juce::dontSendNotification);
    distortionLabel.attachToComponent(&distortionSlider, false);
    addAndMakeVisible(distortionLabel);

    toneLabel.setText("Tone", juce::dontSendNotification);
    toneLabel.attachToComponent(&toneSlider, false);
    addAndMakeVisible(toneLabel);

    volumeLabel.setText("Volume", juce::dontSendNotification);
    volumeLabel.attachToComponent(&volumeSlider, false);
    addAndMakeVisible(volumeLabel);

    //oversampleButton.setButtonText("Enable Oversampling");
    //oversampleButton.onClick = [this] { buttonClicked(&oversampleButton); };
    //addAndMakeVisible(oversampleButton);

    // Attach sliders to parameters
    distortionAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.getAPVTS(), "distortion", distortionSlider));
    toneAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.getAPVTS(), "tone", toneSlider));
    volumeAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.getAPVTS(), "volume", volumeSlider));
}

ProcoRatAudioProcessorEditor::~ProcoRatAudioProcessorEditor()
{
}

//==============================================================================
void ProcoRatAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void ProcoRatAudioProcessorEditor::resized()
{
    distortionSlider.setBounds(50, 100, 100, 100);
    toneSlider.setBounds(150, 100, 100, 100);
    volumeSlider.setBounds(250, 100, 100, 100);
}
