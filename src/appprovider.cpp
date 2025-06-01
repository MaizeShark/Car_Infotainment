// src/appprovider.cpp
#include "appprovider.h"
#include <QVariantMap>
#include <QDebug>

AppProvider::AppProvider(QObject *parent)
    : QObject(parent)
{
}

QVariantList AppProvider::appItems() const
{
    return m_appItems;
}

void AppProvider::addApp(const QString &name, const QString &iconSource, const QString &colorCode)
{
    // Prevent duplicates by name (optional, but good practice)
    for (const QVariant &item : m_appItems) {
        if (item.toMap().value("name").toString() == name) {
            qWarning() << "AppProvider: App with name" << name << "already exists.";
            return;
        }
    }

    QVariantMap appMap;
    appMap["name"] = name;
    appMap["iconSource"] = iconSource;
    appMap["colorCode"] = colorCode; // Used by AppMenuGridView for placeholder

    m_appItems.append(appMap);
    qDebug() << "AppProvider: Added app" << name;
    emit appItemsChanged();
}

bool AppProvider::removeAppByName(const QString &name)
{
    for (int i = 0; i < m_appItems.size(); ++i) {
        if (m_appItems.at(i).toMap().value("name").toString() == name) {
            m_appItems.removeAt(i);
            qDebug() << "AppProvider: Removed app" << name;
            emit appItemsChanged();
            return true;
        }
    }
    qWarning() << "AppProvider: App with name" << name << "not found for removal.";
    return false;
}

void AppProvider::clearApps()
{
    if (!m_appItems.isEmpty()) {
        m_appItems.clear();
        qDebug() << "AppProvider: Cleared all apps.";
        emit appItemsChanged();
    }
}

void AppProvider::moveItem(int from, int to)
{
    if (from < 0 || from >= m_appItems.size() || to < 0 || to >= m_appItems.size() || from == to)
        return;
    m_appItems.move(from, to);
    emit appItemsChanged();
}