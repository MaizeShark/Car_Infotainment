#ifndef SETTINGSPROVIDER_H
#define SETTINGSPROVIDER_H

#include <QObject>
#include <QStringList>
#include <QProcess>

// OpenAuto Configuration Includes
#include <f1x/openauto/autoapp/Configuration/IConfiguration.hpp>
#include <aasdk/proto/messages.pb.h>

// Namespace aliases to make code readable
namespace autoapp = f1x::openauto::autoapp;
namespace aap_msg = aap_protobuf::service::media::sink::message;

// Helper struct to hold temporary settings before saving
struct SettingsState {
    bool is60FPS;
    int resolution; // 0=480p, 1=720p, 2=1080p
    int dpi;
    int omxLayer;
    int marginWidth;
    int marginHeight;
    bool speechChannel;
    bool musicChannel;
    bool isRtAudio; // true = RT, false = Qt
    QString outputDevice;
    QString inputDevice;
};

class SettingsProvider : public QObject
{
    Q_OBJECT

    // --- Properties for QML ---
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

    Q_PROPERTY(QStringList inputDevices READ inputDevices NOTIFY inputDevicesChanged)
    Q_PROPERTY(QStringList outputDevices READ outputDevices NOTIFY outputDevicesChanged)

public:
    // Constructor now takes the Configuration Pointer
    explicit SettingsProvider(std::shared_ptr<autoapp::configuration::IConfiguration> configuration, QObject *parent = nullptr);

    // Actions
    Q_INVOKABLE void save();
    Q_INVOKABLE void load();   // Also acts as Cancel
    Q_INVOKABLE void onAudioTestClicked();
    Q_INVOKABLE void refreshAudioDevices();

    // Getters
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

    // Setters
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

private:
    std::shared_ptr<autoapp::configuration::IConfiguration> configuration_;

    SettingsState m_state; // Holds the "Live" values shown in QML

    QStringList m_inputDevicesList;
    QStringList m_outputDevicesList;

    QStringList fetchPulseAudioDevices(const QString& deviceType);
};

#endif // SETTINGSPROVIDER_H
