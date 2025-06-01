// src/appprovider.h
#ifndef APPPROVIDER_H
#define APPPROVIDER_H

#include <QObject>
#include <QVariantList> // To hold a list of app data
#include <QString>

class AppProvider : public QObject
{
    Q_OBJECT
    // This property will hold the list of apps. Each app will be a QVariantMap.
    Q_PROPERTY(QVariantList appItems READ appItems NOTIFY appItemsChanged)

public:
    explicit AppProvider(QObject *parent = nullptr);

    QVariantList appItems() const;

    // Methods to manipulate the app list, callable from QML
    Q_INVOKABLE void addApp(const QString &name, const QString &iconSource, const QString &colorCode);
    Q_INVOKABLE bool removeAppByName(const QString &name);
    Q_INVOKABLE void clearApps(); // Optional: to remove all apps
    Q_INVOKABLE void moveItem(int from, int to);

signals:
    void appItemsChanged(); // Emitted when the list changes

private:
    QVariantList m_appItems; // Stores QVariantMap for each app
};

#endif // APPPROVIDER_H
