#include "MainComponent.h"
#include "MainWindow.h" // Include MainWindow header

//==============================================================================
MainComponent::MainComponent(
    bool isFullDeviceMode) /* : juce::AudioAppComponent(audioDevManager)*/
{
  // setWantsKeyboardFocus(true);
  setOpaque(true);
  setSize(static_cast<int>((1032 + device_margin_ * 2) * scale_),
          static_cast<int>(500 * scale_));

  audio_engine_ = std::make_unique<MonaiAudioEngine>();

  // deviceManager.chooseBestSampleRate(44100.f);
  // deviceManager.chooseBestBufferSize(64);

  // Some platforms require permissions to open input channels so request that
  // here
  if (juce::RuntimePermissions::isRequired(
          juce::RuntimePermissions::recordAudio) &&
      !juce::RuntimePermissions::isGranted(
          juce::RuntimePermissions::recordAudio)) {
    juce::RuntimePermissions::request(
        juce::RuntimePermissions::recordAudio,
        [&](bool granted) { setAudioChannels(granted ? 2 : 0, 2); });
  } else {
    // Specify the number of input and output channels that we want to open
    setAudioChannels(2, 2);
  }

  // Inspector Button
  inspectorBtn_.onClick = [this]() {
    if (!inspector_) {
      inspector_ = std::make_unique<melatonin::Inspector>(*this);
      inspector_->onClose = [this]() { inspector_.reset(); };
    }

    inspector_->setVisible(true);
  };
  addAndMakeVisible(inspectorBtn_);
}

MainComponent::~MainComponent() {
  // This shuts down the audio device and clears the audio source.
  shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay(int samplesPerBlockExpected,
                                  double sampleRate) {
  // This function will be called when the audio device is started, or when
  // its settings (i.e. sample rate, block size, etc) are changed.

  // You can use this function to initialise any resources you might need,
  // but be careful - it will be called on the audio thread, not the GUI thread.

  // For more details, see the help for AudioProcessor::prepareToPlay()

  static bool is_first_load = true;

  if (is_first_load) {
    is_first_load = false;
    //

    auto audio_setup = deviceManager.getAudioDeviceSetup();

    printf("1st Sample rate: %f; buffer size: %i;\r\n", audio_setup.sampleRate,
           audio_setup.bufferSize);

    audio_setup.bufferSize = 64;
    audio_setup.sampleRate = 44100.f;
    // audio_setup.sampleRate = 48000.f;

    auto is_err = deviceManager.setAudioDeviceSetup(audio_setup, false);

    if (!is_err.isEmpty()) {
      DBG("Error setting up audio device: " + is_err);
    }

    auto *device = deviceManager.getCurrentAudioDevice();
    const auto activeInputChannels =
        device->getActiveInputChannels().toInteger();
    const auto activeOutputChannels =
        device->getActiveOutputChannels().toInteger();

    if (activeInputChannels == 1) {
      audio_engine_->SetInputIsMono(true);
    } else {
      audio_engine_->SetInputIsMono(false);
    }

    audio_setup = deviceManager.getAudioDeviceSetup();
    printf("2nd Sample rate: %f; buffer size: %i;\r\n", audio_setup.sampleRate,
           audio_setup.bufferSize);
    printf("2nd Active inputs: %i; Active outputs: %i;\r\n",
           activeInputChannels, activeOutputChannels);
  }

  double bufferSampleRate = 44100.0; // Fixed sample rate of the buffer
  double resamplingRatio = sampleRate / bufferSampleRate;

  DBG("Resampling ratio:");
  DBG(resamplingRatio);
  DBG("Expected block size:");
  DBG(samplesPerBlockExpected);

  // Do something useful
}

void MainComponent::getNextAudioBlock(
    const juce::AudioSourceChannelInfo &bufferToFill) {

  // audio_engine_->getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources() {
  // This will be called when the audio device stops, or when it is being
  // restarted due to a setting change.
  // For more details, see the help for AudioProcessor::releaseResources()
}

//==============================================================================
void MainComponent::paint(juce::Graphics &g) {
  // (Our component is opaque, so we must completely fill the background with a
  // solid colour) g.fillAll (getLookAndFeel().findColour
  // (juce::ResizableWindow::backgroundColourId));

  const auto bg_color = juce::Colour::fromFloatRGBA(0.2f, 0.2f, 0.2f, 1.0f);
  // const auto bg_color  = juce::Colour::fromFloatRGBA(0.43f,
  // 0.83f, 1.0f, 1.0f);

  g.fillAll(bg_color);
}

void MainComponent::resized() {
  inspectorBtn_.setBounds(10, getHeight() - 100, 100, 20);
}
