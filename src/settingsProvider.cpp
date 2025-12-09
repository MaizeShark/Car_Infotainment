#include "settingsProvider.h"
#include <QDebug>
#include <QProcess>

SettingsProvider::SettingsProvider(QObject *parent)
    : QObject(parent)
{
    // 1. Simulate loading from Configuration on startup
    // In real code: m_saved.is60FPS = configuration_->getVideoFPS() == ...
    m_saved.is60FPS = false;
    m_saved.resolution = 0; // 480p
    m_saved.dpi = 140;
    m_saved.omxLayer = 1;
    m_saved.marginWidth = 0;
    m_saved.marginHeight = 0;
    m_saved.speechChannel = true;
    m_saved.musicChannel = true;
    m_saved.isRtAudio = true;
    m_saved.outputDevice = "Default";
    m_saved.inputDevice = "Default";

    // 2. Load devices
    refreshAudioDevices();

    // 3. Sync Current state with Saved state
    cancel();
}

// --- Action Methods ---

void SettingsProvider::save()
{
    qDebug() << "Saving settings to configuration...";

    // Commit current state to saved state
    m_saved = m_current;

    /* YOUR CONFIGURATION CODE GOES HERE:
       configuration_->setVideoFPS(m_saved.is60FPS ? ... : ...);
       configuration_->setVideoResolution(... based on m_saved.resolution ...);
       configuration_->setScreenDPI(m_saved.dpi);
       ...
       configuration_->save();
    */

    emit settingsSaved();
}

void SettingsProvider::cancel()
{
    qDebug() << "Reverting changes...";
    // Reset current state to the last saved state
    updatePropertiesFromStruct(m_saved);
    emit settingsCancelled();
}

void SettingsProvider::load()
{
    // This function would re-read from the physical disk/config object
    // For now, it behaves like cancel (reverting to known saved state)
    cancel();
}

void SettingsProvider::updatePropertiesFromStruct(const SettingsData& data)
{
    setIs60FPS(data.is60FPS);
    setResolution(data.resolution);
    setDpi(data.dpi);
    setOmxLayer(data.omxLayer);
    setMarginWidth(data.marginWidth);
    setMarginHeight(data.marginHeight);
    setSpeechChannel(data.speechChannel);
    setMusicChannel(data.musicChannel);
    setIsRtAudio(data.isRtAudio);
    setCurrentOutputDevice(data.outputDevice);
    setCurrentInputDevice(data.inputDevice);
}

// --- Audio Logic ---

void SettingsProvider::onAudioTestClicked()
{
    qDebug() << "Starting Audio Test (crankshaft)...";
    QProcess::startDetached("/usr/local/bin/crankshaft", QStringList() << "audio" << "test");
}

void SettingsProvider::refreshAudioDevices()
{
    QStringList newOutputs = fetchPulseAudioDevices("sinks");
    if (m_outputDevicesList != newOutputs) {
        m_outputDevicesList = newOutputs;
        emit outputDevicesChanged();
    }

    QStringList newInputs = fetchPulseAudioDevices("sources");
    if (m_inputDevicesList != newInputs) {
        m_inputDevicesList = newInputs;
        emit inputDevicesChanged();
    }
}

QStringList SettingsProvider::fetchPulseAudioDevices(const QString& deviceType)
{
    QStringList devices;
    QProcess process;
    process.start("pactl", QStringList() << "list" << "short" << deviceType);
    if (!process.waitForFinished(2000)) return QStringList() << "Default";

    QString output = process.readAllStandardOutput();
    QStringList lines = output.split('\n', Qt::SkipEmptyParts);
    for (const QString &line : lines) {
        QStringList parts = line.split('\t', Qt::SkipEmptyParts);
        if (parts.size() >= 2) devices.append(parts[1]);
    }
    if (devices.isEmpty()) devices.append("Default");
    return devices;
}

// --- Setters (Update QML & m_current) ---

void SettingsProvider::setIs60FPS(bool v) {
    if (m_current.is60FPS == v) return;
    m_current.is60FPS = v;
    emit is60FPSChanged();
}
void SettingsProvider::setResolution(int v) {
    if (m_current.resolution == v) return;
    m_current.resolution = v;
    emit resolutionChanged();
}
void SettingsProvider::setDpi(int v) {
    if (m_current.dpi == v) return;
    m_current.dpi = v;
    emit dpiChanged();
}
void SettingsProvider::setOmxLayer(int v) {
    if (m_current.omxLayer == v) return;
    m_current.omxLayer = v;
    emit omxLayerChanged();
}
void SettingsProvider::setMarginWidth(int v) {
    if (m_current.marginWidth == v) return;
    m_current.marginWidth = v;
    emit marginWidthChanged();
}
void SettingsProvider::setMarginHeight(int v) {
    if (m_current.marginHeight == v) return;
    m_current.marginHeight = v;
    emit marginHeightChanged();
}
void SettingsProvider::setSpeechChannel(bool v) {
    if (m_current.speechChannel == v) return;
    m_current.speechChannel = v;
    emit speechChannelChanged();
}
void SettingsProvider::setMusicChannel(bool v) {
    if (m_current.musicChannel == v) return;
    m_current.musicChannel = v;
    emit musicChannelChanged();
}
void SettingsProvider::setIsRtAudio(bool v) {
    if (m_current.isRtAudio == v) return;
    m_current.isRtAudio = v;
    emit isRtAudioChanged();
}
void SettingsProvider::setCurrentOutputDevice(const QString &v) {
    if (m_current.outputDevice == v) return;
    m_current.outputDevice = v;
    emit currentOutputDeviceChanged();
}
void SettingsProvider::setCurrentInputDevice(const QString &v) {
    if (m_current.inputDevice == v) return;
    m_current.inputDevice = v;
    emit currentInputDeviceChanged();
}

// --- Getters ---
bool SettingsProvider::is60FPS() const { return m_current.is60FPS; }
int SettingsProvider::resolution() const { return m_current.resolution; }
int SettingsProvider::dpi() const { return m_current.dpi; }
int SettingsProvider::omxLayer() const { return m_current.omxLayer; }
int SettingsProvider::marginWidth() const { return m_current.marginWidth; }
int SettingsProvider::marginHeight() const { return m_current.marginHeight; }
bool SettingsProvider::speechChannel() const { return m_current.speechChannel; }
bool SettingsProvider::musicChannel() const { return m_current.musicChannel; }
bool SettingsProvider::isRtAudio() const { return m_current.isRtAudio; }
QString SettingsProvider::currentOutputDevice() const { return m_current.outputDevice; }
QString SettingsProvider::currentInputDevice() const { return m_current.inputDevice; }
QStringList SettingsProvider::inputDevices() const { return m_inputDevicesList; }
QStringList SettingsProvider::outputDevices() const { return m_outputDevicesList; }
