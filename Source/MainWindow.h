#pragma once

#include "MainComponent.h"

#include "juce_gui_basics/juce_gui_basics.h"

class MainWindow : public juce::DocumentWindow {
public:
  MainWindow(juce::PropertiesFile *propertiesFile);
  ~MainWindow();

  void resizeMainWindow(bool val);
  void closeButtonPressed() override;
  void activeWindowStatusChanged() override;

private:
  std::unique_ptr<MainComponent> main_component_;
  juce::PropertiesFile *propertiesFile_;

  bool is_full_device_mode_ = true;
  int og_width_ = 0;
  int og_height_ = 0;

  juce::String alt_mode_windowsize_{juce::String("")};

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainWindow)
};
