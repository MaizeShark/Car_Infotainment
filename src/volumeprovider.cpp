#include "volumeprovider.h"

VolumeProvider::VolumeProvider(QObject *parent)
    : QObject(parent)
{
}

int VolumeProvider::volume() const
{
    return m_volume;
}

void VolumeProvider::setVolume(int newVolume)
{
    if (m_volume == newVolume)
        return;

    m_volume = newVolume;
    emit volumeChanged();
}

bool VolumeProvider::muted() const
{
    return m_muted;
}

void VolumeProvider::setMuted(bool newMuted)
{
    if (m_muted == newMuted)
        return;

    m_muted = newMuted;
    emit mutedChanged();
}
