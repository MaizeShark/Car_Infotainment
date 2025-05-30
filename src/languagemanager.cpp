#include "languagemanager.h"
#include <QDebug>
#include <QCoreApplication> // Für applicationDirPath

LanguageManager* LanguageManager::m_instance = nullptr; // Initialisierung für Singleton

LanguageManager::LanguageManager(QQmlEngine *engine, QObject *parent)
    : QObject(parent), m_qmlEngine(engine), m_currentLanguage("en") // Standard ist Englisch
{
    Q_ASSERT(m_qmlEngine); // Sicherstellen, dass der Engine gesetzt ist
    if (!m_instance) {
        m_instance = this;
    }
    // Initial keine Übersetzung laden, da "en" der Basiswert ist
}

LanguageManager* LanguageManager::instance() {
    return m_instance;
}

QString LanguageManager::currentLanguage() const
{
    return m_currentLanguage;
}

void LanguageManager::setCurrentLanguage(const QString &languageCode)
{
    if (m_currentLanguage == languageCode && (languageCode == "en" || !m_translator.isEmpty())) {
        // Sprache ist bereits gesetzt und entweder "en" oder Translator ist geladen
        return;
    }

    qDebug() << "Trying to set language to:" << languageCode;

    // Vorherigen Translator entfernen, falls einer aktiv war
    if (!m_translator.isEmpty()) {
        QGuiApplication::removeTranslator(&m_translator);
        m_translator.load(""); // Entladen der aktuellen Übersetzungsdatei
        qDebug() << "Removed previous translator.";
    }

    m_currentLanguage = languageCode;

    // "en" ist unsere Basissprache, dafür brauchen wir keinen Translator (es sei denn, Ihre Quelltexte sind nicht Englisch)
    if (languageCode != "en") {
        // Pfad zu den Übersetzungsdateien (z.B. im Build-Ordner/translations oder via qrc)
        // Hier als Beispiel: im Unterordner "translations" neben der Executable
        QString translationsPath = QCoreApplication::applicationDirPath() + "/";
        QLocale locale(languageCode);
        QString fullFileName = translationsPath + "app_" + languageCode + ".qm";
        qDebug() << "Attempting to load specific file:" << fullFileName;
        if (m_translator.load(fullFileName)) { // Nur zwei Argumente: kompletter Pfad zur Datei
            QGuiApplication::installTranslator(&m_translator);
            qDebug() << "Loaded and installed translation file:" << fullFileName;
        } else {
            qDebug() << "Failed to load translation file" << fullFileName;
            // Versuche es mit der Locale-basierten Suche als Fallback, falls die obige fehlschlägt
            qDebug() << "Trying QLocale-based load: locale:" << languageCode << "path:" << translationsPath << "prefix: app_";
            if (m_translator.load(QLocale(languageCode), "app", ".qm", translationsPath)) {
                QGuiApplication::installTranslator(&m_translator);
                qDebug() << "Loaded and installed translation file using QLocale:" << "app_" + languageCode + ".qm";
            } else {
                qDebug() << "QLocale-based load also failed for" << languageCode
                         << "from path" << translationsPath << "with prefix app_";
                m_currentLanguage = "en";
            }
        }
    } else {
        qDebug() << "Set to English (base). No translator loaded.";
    }

    if (m_qmlEngine) {
        m_qmlEngine->retranslate(); // QML-Engine anweisen, alle qsTr neu auszuwerten
        qDebug() << "QML engine retranslated.";
    }

    emit currentLanguageChanged(m_currentLanguage);
}
