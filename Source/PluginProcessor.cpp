
#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ProcoRatAudioProcessor::ProcoRatAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
    apvts(*this, nullptr, "PARAMETERS",
        {
            std::make_unique<juce::AudioParameterFloat>("distortion", "Distortion", 0.0f, 1.0f, 0.5f),
            std::make_unique<juce::AudioParameterFloat>("tone", "Tone", 0.0f, 1.0f, 0.5f),
            std::make_unique<juce::AudioParameterFloat>("volume", "Volume", 0.0f, 1.0f, 0.5f)
        }),
    OsProcessor(2, 1, juce::dsp::Oversampling<float>::filterHalfBandPolyphaseIIR, true, false)
#endif
{
}

ProcoRatAudioProcessor::~ProcoRatAudioProcessor()
{
}

//==============================================================================
const juce::String ProcoRatAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool ProcoRatAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool ProcoRatAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool ProcoRatAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double ProcoRatAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int ProcoRatAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int ProcoRatAudioProcessor::getCurrentProgram()
{
    return 0;
}

void ProcoRatAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String ProcoRatAudioProcessor::getProgramName (int index)
{
    return {};
}

void ProcoRatAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void ProcoRatAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    Cat_processor.prepare(sampleRate, samplesPerBlock);
    OsProcessor.initProcessing(samplesPerBlock);
    OsProcessor.reset();
}

void ProcoRatAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ProcoRatAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void ProcoRatAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    // Parameters collection
    float distortionValue = *apvts.getRawParameterValue("distortion");
    float toneValue = *apvts.getRawParameterValue("tone");
    float volumeValue = *apvts.getRawParameterValue("volume");
    Cat_processor.setParam(distortionValue, toneValue, volumeValue, isOsEnabled);

    juce::dsp::AudioBlock<float> block(buffer);

    if (isBypassed == false) {
        if (isOsEnabled == false)
            Cat_processor.process(block);
        else {
            juce::dsp::AudioBlock<float> osBlock = OsProcessor.processSamplesUp(block);
            Cat_processor.process(osBlock);
            OsProcessor.processSamplesDown(block);

        }
    }


    for (int channel = 0; channel < block.getNumChannels(); ++channel)
    {
        auto* channelData = block.getChannelPointer(channel);
        for (int sample = 0;sample < block.getNumSamples();++sample) {
            channelData[sample] = block.getSample(channel, sample);
        }

        // ..do something to the data...
    }
}

//==============================================================================
bool ProcoRatAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* ProcoRatAudioProcessor::createEditor()
{
    return new ProcoRatAudioProcessorEditor (*this);
}

//==============================================================================
void ProcoRatAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void ProcoRatAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ProcoRatAudioProcessor();
}
