#include "MainWindow.h"

MainWindow::MainWindow(juce::PropertiesFile *propertiesFile)
    : DocumentWindow(
          "PipoOnset",
          juce::Desktop::getInstance().getDefaultLookAndFeel().findColour(
              juce::ResizableWindow::backgroundColourId),
          DocumentWindow::allButtons),
      propertiesFile_(propertiesFile) {

  // Restore previous window position
  const auto window_state = propertiesFile_->getValue("mainWindowState");
  const auto isBigScreenView = propertiesFile_->getValue("isBigScreenView");
  DBG("isBigScreenView: " + isBigScreenView);

  if (!isBigScreenView.isEmpty()) {
    is_full_device_mode_ = isBigScreenView.getIntValue();
    alt_mode_windowsize_ = window_state;
  }

  main_component_ = std::make_unique<MainComponent>(is_full_device_mode_);
  setUsingNativeTitleBar(true);
  setContentNonOwned(main_component_.get(), true);

#if JUCE_IOS || JUCE_ANDROID
  setFullScreen(true);
#else
  og_width_ = getWidth();
  og_height_ = getHeight();
#if 0
  setResizable(false, true);
  // const double ratio = static_cast<double>(getWidth()) / static_cast<double>(getHeight());
  // getConstrainer()->setFixedAspectRatio(ratio);
  centreWithSize(og_width_, og_height_);
#else
  resizeMainWindow(is_full_device_mode_);
#endif
#endif

// TODO
#if 0
  if(!window_state.isEmpty())
    restoreWindowStateFromString(window_state);
#endif

  Component::setVisible(true);
}

MainWindow::~MainWindow() {
  // Save previous window position
  const auto window_state = getWindowStateAsString();
  propertiesFile_->setValue("mainWindowState", window_state);
  propertiesFile_->setValue("isBigScreenView", is_full_device_mode_);
}

void MainWindow::resizeMainWindow(bool val) {
  is_full_device_mode_ = val;

  if (is_full_device_mode_) {
    alt_mode_windowsize_ = getWindowStateAsString();
    centreWithSize(og_width_, og_height_);
    setResizable(false, true);
  } else {
    if (alt_mode_windowsize_ != juce::String("")) {
      restoreWindowStateFromString(alt_mode_windowsize_);
    } else {
      centreWithSize(400, 600);
    }

    setResizable(true, true);
  }
}

void MainWindow::closeButtonPressed() {
  juce::JUCEApplication::getInstance()->systemRequestedQuit();
}

void MainWindow::activeWindowStatusChanged() {
  if (main_component_ == nullptr)
    return;

  if (main_component_->monai_device.keyboard.isShowing())
    main_component_->monai_device.keyboard.grabKeyboardFocus();
}
