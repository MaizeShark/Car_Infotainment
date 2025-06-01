cmake_minimum_required(VERSION 3.16) # Empfohlen für Qt6, für Qt5 geht auch älter (z.B. 3.10)

project(Car_Infotainment_App VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(DESIGN_STUDIO_QML_IMPORTS /home/niclas/Qt/Tools/QtDesignStudio/qt6_design_studio_reduced_version/qml/)
list(APPEND CMAKE_PREFIX_PATH ${DESIGN_STUDIO_QML_IMPORTS}/../..)

# --- Qt Setup ---
# Für Qt6 (empfohlen, wenn möglich)
find_package(Qt6 COMPONENTS Core Gui Qml Quick LinguistTools Core5Compat REQUIRED)
set(QT_VERSION_MAJOR 6)

# Für Qt5 (falls du noch Qt5 verwendest)
# find_package(Qt5 COMPONENTS Core Gui Qml Quick REQUIRED)
# set(QT_VERSION_MAJOR 5)

set(CMAKE_AUTOMOC ON) # Automatische Ausführung des Meta-Object Compilers
set(CMAKE_AUTORCC ON) # Automatische Verarbeitung von .qrc Dateien
set(CMAKE_AUTOUIC ON) # Automatische Verarbeitung von .ui Dateien (falls vorhanden)

# --- Projektquellen ---
# Alle C++ Quelldateien
set(PROJECT_SOURCES
    src/main.cpp
    src/languagemanager.cpp
    src/languagemanager.h # Header hier auflisten ist gute Praxis für IDEs
                          # oder separat als target_sources mit PUBLIC/PRIVATE definieren
)

# Die Ressourcendatei
# WICHTIG: Entferne die .qm-Dateien aus dieser QRC, wenn du qt_add_translations (Qt6) verwendest!
set(PROJECT_RESOURCES
    resources.qrc
)

# --- Erstelle das ausführbare Programm ---
add_executable(${PROJECT_NAME}
    ${PROJECT_SOURCES}
    ${PROJECT_RESOURCES}
    src/climateprovider.h
    src/climateprovider.cpp
    src/volumeprovider.h src/volumeprovider.cpp
    src/appprovider.h src/appprovider.cpp
)

# --- Verlinke Qt-Module ---
if(QT_VERSION_MAJOR EQUAL 6)
    target_link_libraries(${PROJECT_NAME} PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Qml
        Qt6::Quick
        Qt6::Core5Compat
    )
else() # Qt5
    target_link_libraries(${PROJECT_NAME} PRIVATE
        Qt5::Core
        Qt5::Gui
        Qt5::Qml
        Qt5::Quick
    )
endif()

# --- Internationalisierung (i18n) ---
set(TS_FILES # Liste deiner .ts Dateien
    translations/app_de.ts
    # translations/app_fr.ts # Füge hier weitere Sprachen hinzu
)

if(QT_VERSION_MAJOR EQUAL 6 AND Qt6_VERSION VERSION_GREATER_EQUAL "6.2")
    # Qt6.2+ bietet eine sehr komfortable Möglichkeit, Übersetzungen zu handhaben
    # Die .qm Dateien werden automatisch generiert und in eine QRC-Datei gepackt.
    # Der PREFIX sorgt dafür, dass sie unter "qrc:/translations/app_de.qm" etc. liegen.
    qt_add_translations(${PROJECT_NAME}
        PREFIX "translations" # Wichtig für den Pfad im LanguageManager
        TS_FILES ${TS_FILES}
    )
else()
    # Manuelle Methode für Qt5 oder ältere Qt6 Versionen
    # Stelle sicher, dass lupdate und lrelease im Systempfad sind oder gib den vollen Pfad an.
    # Finde die Qt Linguist Werkzeuge
    if(QT_VERSION_MAJOR EQUAL 6)
        find_package(Qt6 COMPONENTS LinguistTools REQUIRED)
        set(LUPDATE_EXECUTABLE Qt6::lupdate)
        set(LRELEASE_EXECUTABLE Qt6::lrelease)
    else() # Qt5
        find_package(Qt5 COMPONENTS LinguistTools REQUIRED)
        set(LUPDATE_EXECUTABLE Qt5::lupdate)
        set(LRELEASE_EXECUTABLE Qt5::lrelease)
    endif()

    # Target zum Aktualisieren der .ts Dateien (lupdate)
    add_custom_target(update_translations ALL # 'ALL' bedeutet, es wird immer versucht auszuführen
        COMMAND ${LUPDATE_EXECUTABLE}
            # Alle relevanten Quellordner und Dateien angeben
            ${CMAKE_SOURCE_DIR}/src
            ${CMAKE_SOURCE_DIR}/Car_infotainmentContent
            ${CMAKE_SOURCE_DIR}/Car_infotainment
            -ts ${TS_FILES}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Updating translation source files (.ts)..."
        VERBATIM
    )

    # Generiere .qm Dateien aus .ts Dateien (lrelease)
    # Diese müssen dann manuell zur resources.qrc hinzugefügt werden oder
    # in eine separate QRC, die dann zu PROJECT_RESOURCES hinzugefügt wird.
    # Diese manuelle Methode ist etwas umständlicher.
    # Hier wird angenommen, du fügst die generierten .qm Dateien manuell zu resources.qrc hinzu
    # und lässt CMAKE_AUTORCC den Rest erledigen.
    # Damit dies funktioniert, muss lrelease vor dem AUTORCC-Schritt laufen,
    # was kompliziert sein kann.
    #
    # BESSERE MANUELLE VARIANTE: Generiere QM-Dateien und füge sie einer *separaten*,
    # generierten QRC hinzu.
    # Hier ist ein vereinfachter Ansatz, der nur das lrelease-Kommando zeigt:
    # Du müsstest die .qm-Dateien in deiner `resources.qrc` auflisten.
    foreach(TS_FILE ${TS_FILES})
        get_filename_component(TS_NAME ${TS_FILE} NAME_WE) # z.B. app_de
        set(QM_FILE ${CMAKE_CURRENT_BINARY_DIR}/translations/${TS_NAME}.qm) # Output im Build-Ordner
        add_custom_command(
            OUTPUT ${QM_FILE}
            COMMAND ${LRELEASE_EXECUTABLE} -compress ${CMAKE_CURRENT_SOURCE_DIR}/${TS_FILE} -qm ${QM_FILE}
            DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${TS_FILE} # Abhängigkeit von der .ts Datei
            COMMENT "Releasing ${TS_FILE} to ${QM_FILE}"
            VERBATIM
        )
        # Damit diese QM-Dateien gefunden werden, musst du sie in deine resources.qrc aufnehmen
        # oder eine separate QRC-Datei dafür erstellen und zu `PROJECT_RESOURCES` hinzufügen.
        # Z.B. in resources.qrc: <file>pfad/zu/build/translations/app_de.qm</file> (was unschön ist)
        # Besser: Kopiere die .qm-Dateien in dein Source-translations-Verzeichnis und nimm sie von dort ins QRC.
    endforeach()
    # Die Qt6.2+ Methode ist DEUTLICH einfacher!
endif()


# --- Installationsregeln (optional, aber gut für Deployment) ---
# install(TARGETS ${PROJECT_NAME}
#     RUNTIME DESTINATION bin # Windows: bin, Linux: bin oder usr/bin
#     BUNDLE DESTINATION .    # macOS: Erstellt ein .app Bundle
# )

# Wenn dein QML-Modul "Car_infotainment" eine qmldir hat und über
# einen URI importiert wird (z.B. import Car_infotainment 1.0),
# stelle sicher, dass der QML-Engine den Pfad findet.
# Normalerweise reicht es, wenn die qmldir und die zugehörigen QML-Dateien
# über die resources.qrc verfügbar sind (z.B. qrc:/Car_infotainment/qmldir).
# target_qml_sources kann hier auch helfen (Qt6) oder explizite QML_IMPORT_PATH setzen.
# Da du alles in resources.qrc hast, sollte es funktionieren.
