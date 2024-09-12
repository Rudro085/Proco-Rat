
#include "LookAndFeel.h"

RotaryKnobLookAndFeel::RotaryKnobLookAndFeel()
{
	setColour(juce::Label::textColourId, Colours::textColour);
	setColour(juce::Slider::textBoxTextColourId, Colours::textColour);
	setColour(juce::Slider::rotarySliderFillColourId, Colours::Knob::trackActive);
	setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
}

void RotaryKnobLookAndFeel::drawRotarySlider(
	juce::Graphics& g,
	int x, int y, int width, [[maybe_unused]] int height,
	float sliderPos,
	float rotaryStartAngle, float rotaryEndAngle,
	juce::Slider& slider)
{

	setColour(juce::Slider::rotarySliderFillColourId, Colours::Knob::trackActive);

	auto bounds = juce::Rectangle<int>(x, y, width, width).toFloat();
	auto knobRect = bounds.reduced(20.0f, 20.0f);

	// drawing outer light circle with drop shadow
	auto path = juce::Path();
	path.addEllipse(knobRect);
	//dropShadow.drawForPath(g, path);

	g.setColour(Colours::Knob::outline);
	g.fillEllipse(knobRect);

	// drawing inner darker circle 
	auto innerRect = knobRect.reduced(2.0f, 2.0f);
	auto gradient = juce::ColourGradient(
		Colours::Knob::gradientTop, 0.0f, innerRect.getY(),
		Colours::Knob::gradientBottom, 0.0f, innerRect.getBottom(), false);

	g.setGradientFill(gradient);
	g.fillEllipse(innerRect);

	// draw the slider track
	auto center = bounds.getCentre();
	auto radius = bounds.getWidth() / 2.6f;
	auto lineWidth = 3.0f;
	auto arcRadius = radius - lineWidth / 3.0f;
	juce::Path backgroundArc;
	backgroundArc.addCentredArc(center.x,
		center.y,
		arcRadius,
		arcRadius,
		0.0f,
		rotaryStartAngle,
		rotaryEndAngle,
		true);
	auto strokeType = juce::PathStrokeType(
		lineWidth, juce::PathStrokeType::curved, juce::PathStrokeType::rounded);
	g.setColour(Colours::Knob::trackBackground);
	g.strokePath(backgroundArc, strokeType);

	// Draw the dial
	auto dialRadius = innerRect.getHeight() / 2.0f - lineWidth / 2.0f;
	auto toAngle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
	juce::Point<float> dialStart(center.x + 10.0f * std::sin(toAngle), center.y - 10.0f * std::cos(toAngle));
	juce::Point<float> dialEnd(center.x + dialRadius * std::sin(toAngle),
		center.y - dialRadius * std::cos(toAngle));
	juce::Path dialPath;
	dialPath.startNewSubPath(dialStart);
	dialPath.lineTo(dialEnd);
	g.setColour(Colours::Knob::dial);
	g.strokePath(dialPath, strokeType);

	// draw a filled path in the active portion of the track (from rotaryStartAngle ---> toAngle)
	if (slider.isEnabled()) {

		bool isGainKnob = slider.getProperties()["drawsFromMid"];

		if (isGainKnob || slider.getValue() != slider.getMinimum())
		{
			// gain knob slider will start at middle of ellipse 
			float fromAngle = (isGainKnob) ? rotaryStartAngle + (rotaryEndAngle - rotaryStartAngle) / 2.0f : rotaryStartAngle;
			juce::Path valueArc;
			valueArc.addCentredArc(center.x,
				center.y,
				arcRadius,
				arcRadius,
				0.0f,
				fromAngle,
				toAngle,
				true);
			g.setColour(slider.findColour(juce::Slider::rotarySliderFillColourId));
			g.strokePath(valueArc, strokeType);
		}
	}

}