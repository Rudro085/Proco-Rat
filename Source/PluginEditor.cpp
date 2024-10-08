
#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "LookAndFeel.h"

//==============================================================================
ProcoRatAudioProcessorEditor::ProcoRatAudioProcessorEditor (ProcoRatAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(400, 300);
    distortionSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    distortionSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80,10);
    distortionSlider.setLookAndFeel(RotaryKnobLookAndFeel::get());
    addAndMakeVisible(distortionSlider);

    toneSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    toneSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 10);
   
    toneSlider.setLookAndFeel(RotaryKnobLookAndFeel::get());
    addAndMakeVisible(toneSlider);

    volumeSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    volumeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80,10);
    volumeSlider.setLookAndFeel(RotaryKnobLookAndFeel::get());
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

    addAndMakeVisible(ilevelL);
    addAndMakeVisible(ilevelR);
    addAndMakeVisible(olevel);

    //oversampleButton.setButtonText("Enable Oversampling");
    //oversampleButton.onClick = [this] { buttonClicked(&oversampleButton); };
    //addAndMakeVisible(oversampleButton);

    // Attach sliders to parameters
    distortionAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.getAPVTS(), "distortion", distortionSlider));
    toneAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.getAPVTS(), "tone", toneSlider));
    volumeAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.getAPVTS(), "volume", volumeSlider));

    startTimerHz(24);
}

ProcoRatAudioProcessorEditor::~ProcoRatAudioProcessorEditor()
{
    stopTimer();
}

//==============================================================================
void ProcoRatAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}
void ProcoRatAudioProcessorEditor::timerCallback() {
    ilevelL.setLevel(audioProcessor.getLevel(0));
    ilevelL.repaint();
    ilevelR.setLevel(audioProcessor.getLevel(1));
    ilevelR.repaint();
    olevel.setLevel(audioProcessor.getLevel(2));
    olevel.repaint();
}

void ProcoRatAudioProcessorEditor::resized()
{
    distortionSlider.setBounds(50, 50, 100, 100);
    toneSlider.setBounds(150, 50, 100, 100);
    volumeSlider.setBounds(250, 50, 100, 100);
    ilevelL.setBounds(5, 10 , 5 ,300-20);
    ilevelR.setBounds(12, 10, 5, 300 - 20);
    olevel.setBounds(400-10, 10, 5, 300 - 20);
}
