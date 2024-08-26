/*
  ==============================================================================

    Distortion.cpp
    Created: 1 Aug 2024 10:54:08am
    Author:  Rudro Hasan

  ==============================================================================
*/

#include "Distortion.h"

Distortion::Distortion() {


}

void Distortion::setParam(float dist, float tone, float volume, bool osEnabled) {
    if (osEnabled) {
        T = 1. / (4. * sampleRate);
    }
    Rd = std::powf(10., 2. * dist - 2.) * 100000.;
    Rt = 1500. + std::powf(10., 1. * tone - 1) * 100000.;
    Volume = volume;
    //Constants Calculation
    gc2 = C2 / T;
    gc1 = T / (2. * C1);
    gr2 = 1. / R2;
    gc5 = T / (2. * C5);
    gc6 = T / (2. * C6);
    gc4 = C4 / T;
    grd = 1. / Rd;
    gc9 = T / (2. * C9);
    gc7 = T / (2. * C7);
    gr8 = 1. / R8;
    gc8 = C8 / T;

}

void Distortion::prepare(double SampleRate, int SamplePerBlock) {
    samplePerBlock = SamplePerBlock;
    sampleRate = SampleRate;
    T = 1. / sampleRate;
    setParam(0.5, 0.5, 0.5, false);
    V2_prev = 4.50002888780806;
    Vc1_prev = -4.500028886551527;
    ic1_prev = 2.763001796157738e-11;
    V3_prev = 4.5;
    Vc5_prev = 4.500029018261253;
    ic5_prev = -2.7755998301170726e-09;
    Vc6_prev = 4.500030442143965;
    ic6_prev = -2.8026668132752496e-09;

    V3_diff_prev = 0.;
    V4_prev = 0.;

    Vc9_prev = -0.2704307402724265;
    Vtone_prev = -0.2610136203230552;
    ic9_prev = 9.417119949371302e-09;
    Vc7_prev = 4.5567605036041465;
    ic7_prev = -1.8954021159105192e-06;
    Vout_prev = 0.00;
}

float Distortion::solve(float Vin) {
    //input filter
    float V2 = (R3 * V2_prev * gc1 * gc2 * gr2 + R3 * V2_prev * gc2 + V2_prev * gc1 * gc2 - Vc1_prev + Vin + Vref * gc1 * gr2 - gc1 * ic1_prev) / (R3 * gc1 * gc2 * gr2 + R3 * gc2 + gc1 * gc2 + gc1 * gr2 + 1.);

    //input filter memory element update
    Vc1_prev = Vc1_prev + gc1 * (gc2 * (V2 - V2_prev) + gr2 * (R3 * gc2 * (V2 - V2_prev) + V2 - Vref) + ic1_prev);
    ic1_prev = gc2 * (V2 - V2_prev) + gr2 * (R3 * gc2 * (V2 - V2_prev) + V2 - Vref);
    V2_prev = V2;

    //op-amp section
    float V3 = (R4 * R5 * V2 * gc4 + R4 * R5 * V2 * grd - R4 * R5 * V2_prev * gc4 + R4 * R5 * V3_prev * gc4 + R4 * V2 * gc4 * gc6 + R4 * V2 * gc6 * grd + R4 * V2 - R4 * V2_prev * gc4 * gc6 + R4 * V3_prev * gc4 * gc6 - R4 * Vc6_prev - R4 * gc6 * ic6_prev + R5 * V2 * gc5 * gc4 + R5 * V2 * gc5 * grd + R5 * V2 - R5 * V2_prev * gc5 * gc4 + R5 * V3_prev * gc5 * gc4 - R5 * Vc5_prev - R5 * gc5 * ic5_prev + V2 * gc5 * gc4 * gc6 + V2 * gc5 * gc6 * grd + V2 * gc5 + V2 * gc6 - V2_prev * gc5 * gc4 * gc6 + V3_prev * gc5 * gc4 * gc6 - Vc5_prev * gc6 - Vc6_prev * gc5 - gc5 * gc6 * ic5_prev - gc5 * gc6 * ic6_prev) / ((R4 + gc5) * (R5 + gc6) * (gc4 + grd));
    if (V3 >= 9.0)  V3 = 9.0;
    else if (V3 <= 0.)  V3 = 0.;

    //op-amp section memory element update
    ic5_prev = (V2 - Vc5_prev - gc5 * ic5_prev) / (R4 + gc5);
    Vc5_prev = V2 - ic5_prev * R4;
    ic6_prev = (V2 - Vc6_prev - gc6 * ic6_prev) / (R5 + gc6);
    Vc6_prev = V2 - ic5_prev * R5;

    float V4 = V3;

    //clipping section
    float Vout = Vout_prev;
    int j = 0;

    //Newton-Rapson loop
    while (j <= 60) {
        j++;
        float Vd = Rt * (Vout * gr8 + gc8 * (Vc9_prev + Vout - Vtone_prev + gc9 * (Vout * gr8 + ic9_prev))) + Vc9_prev + Vout + gc9 * (Vout * gr8 + ic9_prev);
        if (abs(Vd) >= maxVoltage) Vd = maxVoltage * Vd / abs(Vd);


        Id = Ids * sinh(Vd / (2. * Vt));
        gd = (Ids / (Vt)) * cosh(Vd / (2. * Vt));
        Ideq = Id - gd * Vd;
        Vout = -(Ideq * R6 + Ideq * gc7 + R6 * Rt * Vc9_prev * gc8 * gd - R6 * Rt * Vtone_prev * gc8 * gd + R6 * Rt * gc8 * gc9 * gd * ic9_prev + R6 * Vc9_prev * gc8 + R6 * Vc9_prev * gd - R6 * Vtone_prev * gc8 + R6 * gc8 * gc9 * ic9_prev + R6 * gc9 * gd * ic9_prev + Rt * Vc9_prev * gc7 * gc8 * gd + Rt * Vc9_prev * gc8 - Rt * Vtone_prev * gc7 * gc8 * gd - Rt * Vtone_prev * gc8 + Rt * gc7 * gc8 * gc9 * gd * ic9_prev + Rt * gc8 * gc9 * ic9_prev - V4 + Vc7_prev + Vc9_prev * gc7 * gc8 + Vc9_prev * gc7 * gd + Vc9_prev - Vtone_prev * gc7 * gc8 + gc7 * gc8 * gc9 * ic9_prev + gc7 * gc9 * gd * ic9_prev + gc7 * ic7_prev + gc9 * ic9_prev) / (R6 * Rt * gc8 * gc9 * gd * gr8 + R6 * Rt * gc8 * gd + R6 * Rt * gd * gr8 + R6 * gc8 * gc9 * gr8 + R6 * gc8 + R6 * gc9 * gd * gr8 + R6 * gd + R6 * gr8 + Rt * gc7 * gc8 * gc9 * gd * gr8 + Rt * gc7 * gc8 * gd + Rt * gc7 * gd * gr8 + Rt * gc8 * gc9 * gr8 + Rt * gc8 + Rt * gr8 + gc7 * gc8 * gc9 * gr8 + gc7 * gc8 + gc7 * gc9 * gd * gr8 + gc7 * gd + gc7 * gr8 + gc9 * gr8 + 1.);
        //if (abs(Vout) >= maxVoltage) Vout = maxVoltage * Vout / abs(Vout);
        if (abs(Vout - Vout_prev) <= 0.000000001)  break;
        else Vout_prev = Vout;
    }
    //clipping section memory element update
    float ic9 = gr8 * Vout;
    Vc9_prev = gc9 * (ic9 + ic9_prev) + Vc9_prev;
    ic9_prev = ic9;
    float ic8 = gc8 * (Vout + Vc9_prev - Vtone_prev);
    Vtone_prev = Vout + Vc9_prev;
    float ic7 = ic8 + ic9 + Id;
    Vc7_prev = gc7 * (ic7 + ic7_prev) + Vc7_prev;
    ic7_prev = ic7;
    Vout_prev = Vout;
    return Vout * Volume;

}

void Distortion::process(juce::dsp::AudioBlock<float>& block) {

    for (int sample = 0;sample < block.getNumSamples();++sample) {
        x = (block.getSample(0, sample) + block.getSample(1, sample)) / 2.0;
        y = solve(x);
        block.setSample(0, sample, y);
        block.setSample(1, sample, y);
    }

}