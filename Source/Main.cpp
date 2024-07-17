#include "MainComponent.h"
#include "MainWindow.h"

// #include "juce_gui_basics/juce_gui_basics.h"
#include <JuceHeader.h> // ProjectInfo exists only here

//==============================================================================
class VoidCustomApp : public juce::JUCEApplication {
public:
  //==============================================================================
  VoidCustomApp() = default;

  // TODO: Theese two are from Projucer era. Should be replaced.
  const juce::String getApplicationName() override {
    return ProjectInfo::projectName;
  }
  const juce::String getApplicationVersion() override {
    return ProjectInfo::versionString;
  }
  bool moreThanOneInstanceAllowed() override { return false; }

  //==============================================================================
  void initialise(const juce::String &commandLine) override {
    // This method is where you should put your application's initialisation
    // code..
    juce::ignoreUnused(commandLine);

    // Initialize ApplicationProperties
    juce::PropertiesFile::Options options;
    options.applicationName = "PipoOnset";
    options.filenameSuffix = ".settings";
    options.osxLibrarySubFolder = "Application Support";
    options.folderName = "VOID9";
    options.storageFormat = juce::PropertiesFile::storeAsXML;

    properties.setStorageParameters(options);

    main_window_ = std::make_unique<MainWindow>(properties.getUserSettings());
  }

  void shutdown() override {
    main_window_ = nullptr; // (deletes our window)
  }

  //==============================================================================
  void systemRequestedQuit() override {
    // This is called when the app is being asked to quit: you can ignore this
    // request and let the app carry on running, or call quit() to allow the app
    // to close.
    quit();
  }

  void anotherInstanceStarted(const juce::String &commandLine) override {
    // When another instance of the app is launched while this one is running,
    // this method is invoked, and the commandLine parameter tells you what
    // the other instance's command-line arguments were.
  }

private:
  std::unique_ptr<MainWindow> main_window_;
  juce::ApplicationProperties properties;
};

//==============================================================================
// This macro generates the main() routine that launches the app.
START_JUCE_APPLICATION(VoidCustomApp)
