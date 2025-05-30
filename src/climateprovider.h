#ifndef CLIMATEPROVIDER_H
#define CLIMATEPROVIDER_H

#include <QObject>

class ClimateProvider : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int temperature READ temperature WRITE setTemperature NOTIFY temperatureChanged)

public:
    explicit ClimateProvider(QObject *parent = nullptr);

    int temperature() const;
    void setTemperature(int temp);

signals:
    void temperatureChanged();

private:
    int m_temperature = 22; // Default room temperature
};

#endif // CLIMATEPROVIDER_H
