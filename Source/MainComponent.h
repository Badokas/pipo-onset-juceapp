#pragma once

#include "juce_audio_utils/juce_audio_utils.h"
#include "juce_gui_basics/juce_gui_basics.h"
#include "melatonin_inspector/melatonin_inspector.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent : public juce::AudioAppComponent {
public:
  //==============================================================================
  MainComponent(bool isFullDeviceMode);
  ~MainComponent() override;

  //==============================================================================
  void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
  void
  getNextAudioBlock(const juce::AudioSourceChannelInfo &bufferToFill) override;
  void releaseResources() override;

  //==============================================================================
  void paint(juce::Graphics &g) override;
  void resized() override;
  bool keyPressed(const juce::KeyPress &key) override;

  //
  //

private:
  static constexpr float scale_ = 1.25f;
  static constexpr float screen_scale_ = 0.29f;
  static constexpr float device_margin_ = 10;

private:
  juce::Colour blue = juce::Colour::fromFloatRGBA(0.43f, 0.83f, 1.0f, 1.0f);

  juce::TextButton inspectorBtn_{"Inspector"};
  std::unique_ptr<melatonin::Inspector> inspector_;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
