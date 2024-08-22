/*
  ==============================================================================

    Distortion.h
    Created: 1 Aug 2024 10:54:08am
    Author:  Rudro Hasan

  ==============================================================================
*/

#pragma once
#include<JuceHeader.h>

class Distortion {
public:
    Distortion();
    void prepare(double SampleRate, int SamplePerBlock);
    void setParam(float dist, float tone, float volume, bool osEnabled);
    void process(juce::dsp::AudioBlock<float>& block);


private:
    float solve(float Vin);
    double sampleRate;
    int samplePerBlock;
    float T;
    float R2 = 1000000.;
    float R3 = 1000.;
    float C1 = 22. / 1000000000.;
    float C2 = 1. / 1000000000.;

    float Rd = 3000.;
    float R4 = 47.;
    float C5 = 2.2 / 1000000.;
    float R5 = 560.;
    float C6 = 4.7 / 1000000.;
    float C4 = 100. / 1000000000000.;


    float C7 = 4.7 / 1000000.;
    float R6 = 1000.;
    float Rt = 1500. + 100000.;
    float C8 = 3.3 / 1000000000.;
    float C9 = 22. / 1000000000.;
    float R8 = 1000000.;

    float Vref = 4.5;
    float Ids = 2.52 * std::pow(10., -9.);
    float Vt = 26. / 1000.;

    float Volume = 1.;
    float gc2, gc1, gr2, gc5, gc6, gc4, grd, gc9, gc7, gr8, gc8;
    float V4, gd, Id, Ideq, x, y;
    float V2_prev, Vc1_prev, ic1_prev, V3_prev, Vc5_prev, ic5_prev, Vc6_prev, ic6_prev, V3_diff_prev, V4_prev, Vc9_prev, Vtone_prev, ic9_prev, Vc7_prev, ic7_prev, Vout_prev;
    float maxVoltage = 1.0; // for V4


};