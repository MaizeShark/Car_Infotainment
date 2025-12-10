#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QStringList>
#include <QRect>

// Helper struct to store settings data
struct SettingsData {
    bool is60FPS = false;
    int resolution = 0; // 0=480p, 1=720p, 2=1080p
    int dpi = 160;
    int omxLayer = 1;
    int marginWidth = 0;
    int marginHeight = 0;
    bool speechChannel = true;
    bool musicChannel = true;
    bool isRtAudio = true; // true=RT, false=Qt
    QString outputDevice = "Default";
    QString inputDevice = "Default";
};

class SettingsProvider : public QObject
{
    Q_OBJECT

    // --- Properties for QML Binding ---
    Q_PROPERTY(bool is60FPS READ is60FPS WRITE setIs60FPS NOTIFY is60FPSChanged)
    Q_PROPERTY(int resolution READ resolution WRITE setResolution NOTIFY resolutionChanged)
    Q_PROPERTY(int dpi READ dpi WRITE setDpi NOTIFY dpiChanged)
    Q_PROPERTY(int omxLayer READ omxLayer WRITE setOmxLayer NOTIFY omxLayerChanged)
    Q_PROPERTY(int marginWidth READ marginWidth WRITE setMarginWidth NOTIFY marginWidthChanged)
    Q_PROPERTY(int marginHeight READ marginHeight WRITE setMarginHeight NOTIFY marginHeightChanged)

    Q_PROPERTY(bool speechChannel READ speechChannel WRITE setSpeechChannel NOTIFY speechChannelChanged)
    Q_PROPERTY(bool musicChannel READ musicChannel WRITE setMusicChannel NOTIFY musicChannelChanged)
    Q_PROPERTY(bool isRtAudio READ isRtAudio WRITE setIsRtAudio NOTIFY isRtAudioChanged)

    Q_PROPERTY(QString currentOutputDevice READ currentOutputDevice WRITE setCurrentOutputDevice NOTIFY currentOutputDeviceChanged)
    Q_PROPERTY(QString currentInputDevice READ currentInputDevice WRITE setCurrentInputDevice NOTIFY currentInputDeviceChanged)

    // Device Lists (Read-only for QML models)
    Q_PROPERTY(QStringList inputDevices READ inputDevices NOTIFY inputDevicesChanged)
    Q_PROPERTY(QStringList outputDevices READ outputDevices NOTIFY outputDevicesChanged)

public:
    explicit SettingsProvider(QObject *parent = nullptr);

    // --- Actions ---
    Q_INVOKABLE void save();
    Q_INVOKABLE void load();   // Reload from disk/saved state
    Q_INVOKABLE void cancel(); // Revert changes
    Q_INVOKABLE void onAudioTestClicked();
    Q_INVOKABLE void refreshAudioDevices();

    // --- Getters ---
    bool is60FPS() const;
    int resolution() const;
    int dpi() const;
    int omxLayer() const;
    int marginWidth() const;
    int marginHeight() const;
    bool speechChannel() const;
    bool musicChannel() const;
    bool isRtAudio() const;
    QString currentOutputDevice() const;
    QString currentInputDevice() const;
    QStringList inputDevices() const;
    QStringList outputDevices() const;

    // --- Setters ---
    void setIs60FPS(bool v);
    void setResolution(int v);
    void setDpi(int v);
    void setOmxLayer(int v);
    void setMarginWidth(int v);
    void setMarginHeight(int v);
    void setSpeechChannel(bool v);
    void setMusicChannel(bool v);
    void setIsRtAudio(bool v);
    void setCurrentOutputDevice(const QString &v);
    void setCurrentInputDevice(const QString &v);

signals:
    // Change Notifications
    void is60FPSChanged();
    void resolutionChanged();
    void dpiChanged();
    void omxLayerChanged();
    void marginWidthChanged();
    void marginHeightChanged();
    void speechChannelChanged();
    void musicChannelChanged();
    void isRtAudioChanged();
    void currentOutputDeviceChanged();
    void currentInputDeviceChanged();

    void inputDevicesChanged();
    void outputDevicesChanged();

    // Signal to tell QML to close the tab or show a toast
    void settingsSaved();
    void settingsCancelled();

private:
    // Internal Helper
    QStringList fetchPulseAudioDevices(const QString& deviceType);
    void updatePropertiesFromStruct(const SettingsData& data);

    // Data Storage
    SettingsData m_current; // What is shown on screen
    SettingsData m_saved;   // What is on disk

    QStringList m_inputDevicesList;
    QStringList m_outputDevicesList;
};

#endif // SETTINGS_H
