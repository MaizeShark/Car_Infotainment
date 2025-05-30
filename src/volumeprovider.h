#ifndef VOLUMEPROVIDER_H
#define VOLUMEPROVIDER_H

#include <QObject>

class VolumeProvider : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int volume READ volume WRITE setVolume NOTIFY volumeChanged)
    Q_PROPERTY(bool muted READ muted WRITE setMuted NOTIFY mutedChanged)

public:
    explicit VolumeProvider(QObject *parent = nullptr);

    int volume() const;
    void setVolume(int newVolume);

    bool muted() const;
    void setMuted(bool newMuted);

signals:
    void volumeChanged();
    void mutedChanged();

private:
    int m_volume = 75; // Standardlautstärke
    bool m_muted = false;
};

#endif // VOLUMEPROVIDER_H
