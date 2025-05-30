#include "climateprovider.h"

ClimateProvider::ClimateProvider(QObject *parent)
    : QObject(parent)
{
}

int ClimateProvider::temperature() const
{
    return m_temperature;
}

void ClimateProvider::setTemperature(int temp)
{
    if (m_temperature != temp) {
        m_temperature = temp;
        emit temperatureChanged();
    }
}
