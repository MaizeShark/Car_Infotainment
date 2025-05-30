#ifndef LANGUAGEMANAGER_H
#define LANGUAGEMANAGER_H

#include <QObject>
#include <QTranslator>
#include <QQmlEngine> // Wichtig für retranslate
#include <QGuiApplication> // Für install/removeTranslator

class LanguageManager : public QObject
{
    Q_OBJECT
    // QML-Property, um die aktuelle Sprache abzufragen
    Q_PROPERTY(QString currentLanguage READ currentLanguage WRITE setCurrentLanguage NOTIFY currentLanguageChanged)

public:
    // Konstruktor nimmt den QML-Engine entgegen
    explicit LanguageManager(QQmlEngine *engine, QObject *parent = nullptr);

    // Statische Methode für einfachen Zugriff (optional, aber praktisch)
    static LanguageManager* instance();

    QString currentLanguage() const;

public slots:
    // Slot, um die Sprache zu ändern (kann von C++ oder QML aufgerufen werden)
    void setCurrentLanguage(const QString &languageCode);

signals:
    // Signal, das gesendet wird, wenn sich die Sprache ändert
    void currentLanguageChanged(const QString &languageCode);

private:
    QQmlEngine *m_qmlEngine;    // Referenz zum QML-Engine
    QTranslator m_translator;   // Der aktuelle Translator
    QString m_currentLanguage;  // z.B. "en", "de"
    QString translationsPath;

    static LanguageManager* m_instance; // Für Singleton-Zugriff
};

#endif // LANGUAGEMANAGER_H
