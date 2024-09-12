
#pragma once

#include <JuceHeader.h>

const juce::String PLUGIN_NAME{ "saturator" };

namespace Colours
{
	const juce::Colour background{ 0xffcd9999 };
	const juce::Colour boxOutline{ 0xff956B6B };
	const juce::Colour textColour{ juce::Colours::white };
	const juce::Colour midOutline{ juce::Colours::darkgrey };

	namespace Knob
	{
		const juce::Colour trackBackground{juce::Colours::lightblue.withBrightness(0.4)  };
		const juce::Colour trackActive{juce::Colours::white };
		const juce::Colour outline{ juce::Colours::darkgrey };
		const juce::Colour gradientTop{ 250, 245, 240 };
		const juce::Colour gradientBottom{ juce::Colours::darkgrey };
		const juce::Colour dial{ juce::Colours::darkgrey }; ///0xFFD69999
		const juce::Colour dropShadow{ 195, 190, 185 };
	}
}

class RotaryKnobLookAndFeel : public juce::LookAndFeel_V4
{
public:
	RotaryKnobLookAndFeel();

	static RotaryKnobLookAndFeel* get()
	{
		static RotaryKnobLookAndFeel instance;
		return &instance;
	}

	void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
		float sliderPos, float rotaryStartAngle,
		float rotaryEndAngle, juce::Slider& slider) override;

private:
	// juce::DropShadow dropShadow{ Colours::Knob::dropShadow, 6 /*radius*/, {0,3} /*offset*/ };
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RotaryKnobLookAndFeel)
};


class MainLookAndFeel : public juce::LookAndFeel_V4
{
public:
	MainLookAndFeel() {};
private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainLookAndFeel)
};