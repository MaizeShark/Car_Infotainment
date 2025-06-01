#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "languagemanager.h"
#include "volumeprovider.h"
#include "climateprovider.h"
#include "appprovider.h"

// Platzhalter für deine C++ Provider, falls du welche hast und sie so registrierst.
// Wenn climateProvider und volumeProvider reine QML-Singletons sind (z.B. über qmldir definiert),
// dann brauchst du hier keine explizite Registrierung.
// #include "climateprovider.h" // Beispiel
// #include "volumeprovider.h"  // Beispiel

int main(int argc, char *argv[])
{
    qDebug() << "Application starting..."; // NEU
    QGuiApplication app(argc, argv);

    QCoreApplication::setOrganizationName("MaizeShark"); // Ersetze das
    QCoreApplication::setOrganizationDomain("maizeshark.com"); // Ersetze das
    QCoreApplication::setApplicationName("CarInfotainmentApp"); // Ersetze das
    qDebug() << "App info set."; // NEU

    QQmlApplicationEngine engine;
    qDebug() << "QML Engine created."; // NEU

    engine.addImportPath("/home/niclas/Qt/Tools/QtDesignStudio/qt6_design_studio_reduced_version/qml");

    // LanguageManager auf dem Heap erstellen, app als Parent
    LanguageManager* languageManager = new LanguageManager(&engine, &app); // &app als Parent
    engine.rootContext()->setContextProperty("languageManager", languageManager);

    // Beispiel: Wenn deine Provider C++ Klassen sind, die du als Kontext-Properties registrieren willst:
    ClimateProvider* climateProvider = new ClimateProvider(&app); // &app als Parent (ggf. Konstruktor anpassen)
    engine.rootContext()->setContextProperty("climateProvider", climateProvider);

    VolumeProvider* volumeProvider = new VolumeProvider(&app); // &app als Parent (ggf. Konstruktor anpassen)
    engine.rootContext()->setContextProperty("volumeProvider", volumeProvider);

    AppProvider* appProvider = new AppProvider(&app);
    engine.rootContext()->setContextProperty("appProvider", appProvider);

    appProvider->addApp("Map", "qrc:/Car_infotainmentContent/icons/map.png", "red");
    appProvider->addApp("Spotify", "qrc:/Car_infotainmentContent/icons/Spotify.png", "limegreen");

    languageManager->setCurrentLanguage("de");


    // WICHTIG: Der Pfad zu deiner Haupt-QML-Datei im Ressourcensystem.
    // Wähle EINE der folgenden Optionen, je nachdem, wie du es in resources.qrc definierst:

    // Option A: Wenn Main.qml direkt unter dem Root im QRC liegt (durch Alias oder direkten Pfad)
    // const QUrl url(QStringLiteral("qrc:/Main.qml"));

    // Option B: Wenn Main.qml unter einem Pfad im QRC liegt (empfohlen für Klarheit)
    const QUrl url(QStringLiteral("qrc:/Car_infotainmentContent/Main.qml"));

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);

    engine.load(url);

    if (engine.rootObjects().isEmpty()) {
        qDebug() << "Fehler: Keine Root-Objekte in QML geladen!";
        return -1;
    }

    // Optional: Startsprache aus C++ setzen (Standard ist "en" im LanguageManager Konstruktor)
    // languageManager.setCurrentLanguage("de");

    return app.exec();
}


