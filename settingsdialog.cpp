/**
 * @file settingsdialog.cpp
 * @brief Implements the in-game Settings dialog for toggling music and sound effects.
 * 
 * Provides checkboxes for enabling/disabling background music and sound effects.
 * Changes are only applied after pressing the "Apply" button. Also includes a cancel option
 * to discard changes. Integrates sound feedback using MainWindow's sound system.
 * 
 * 
 * @author Krish Bhavin Patel
 */

#include "settingsdialog.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSoundEffect>
#include "mainwindow.h"

/**
 * @brief Constructs the Settings dialog.
 * @param initialMusicState Whether background music is initially enabled.
 * @param initialSoundEffectsState Whether sound effects are initially enabled.
 * @param parent Parent widget (usually the main window).
 */
SettingsDialog::SettingsDialog(bool initialMusicState, bool initialSoundEffectsState, QWidget *parent)
    : QDialog(parent),
      currentMusicState(initialMusicState),
      currentSoundEffectsState(initialSoundEffectsState)
{
    setWindowTitle("Settings");

    // Match game's main theme color
    this->setStyleSheet("background-color: #003153;");

    // Music toggle
    musicCheckBox = new QCheckBox("Enable Background Music", this);
    musicCheckBox->setChecked(currentMusicState);  // Reflect initial state
    musicCheckBox->setStyleSheet("color: white; font-size: 14px;");
    connect(musicCheckBox, &QCheckBox::stateChanged, this, &SettingsDialog::onMusicCheckboxStateChanged);

    // Sound effects toggle
    soundEffectsCheckBox = new QCheckBox("Enable Sound Effects", this);
    soundEffectsCheckBox->setChecked(currentSoundEffectsState); // Reflect initial state
    soundEffectsCheckBox->setStyleSheet("color: white; font-size: 14px;");
    connect(soundEffectsCheckBox, &QCheckBox::stateChanged, this, &SettingsDialog::onSoundEffectsCheckboxStateChanged);

    // Apply button setup
    applyButton = new QPushButton("Apply", this);
    applyButton->setStyleSheet(
        "font-size: 14px;"
        "font-weight: bold;"
        "border: 2px solid #8B4513;"
        "border-radius: 5px;"
        "background-color: #D2B48C;"
        "color: #8B4513;"
        "padding: 5px;"
    );
    connect(applyButton, &QPushButton::clicked, this, [=]() {
        auto mainWindow = qobject_cast<MainWindow*>(this->window());
        if (mainWindow) mainWindow->playSelectSound(); // Click sound
        applySettings(); // Apply changes
    });

    // Cancel button setup
    cancelButton = new QPushButton("Cancel", this);
    cancelButton->setStyleSheet(
        "font-size: 14px;"
        "font-weight: bold;"
        "border: 2px solid #8B4513;"
        "border-radius: 5px;"
        "background-color: #D2B48C;"
        "color: #8B4513;"
        "padding: 5px;"
    );
    connect(cancelButton, &QPushButton::clicked, this, [=]() {
        auto mainWindow = qobject_cast<MainWindow*>(this->window());
        if (mainWindow) mainWindow->playSelectSound(); // Click sound
        reject(); // Close dialog without applying
    });

    // Button layout (horizontal)
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(applyButton);
    buttonLayout->addWidget(cancelButton);

    // Main vertical layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(musicCheckBox);
    mainLayout->addWidget(soundEffectsCheckBox);
    mainLayout->addLayout(buttonLayout);

    setLayout(mainLayout); // Set final layout
}

/**
 * @brief Slot triggered when the music checkbox is toggled.
 * Updates internal state but does not apply until "Apply" is pressed.
 */
void SettingsDialog::onMusicCheckboxStateChanged(int state)
{
    currentMusicState = (state == Qt::Checked); // Store new state
}

/**
 * @brief Slot triggered when the sound effects checkbox is toggled.
 * Updates internal state but does not apply until "Apply" is pressed.
 */
void SettingsDialog::onSoundEffectsCheckboxStateChanged(int state)
{
    currentSoundEffectsState = (state == Qt::Checked); // Store new state
}

/**
 * @brief Applies and emits the toggled settings, then closes the dialog.
 */
void SettingsDialog::applySettings()
{
    emit musicToggled(currentMusicState);         // Notify main window
    emit soundEffectsToggled(currentSoundEffectsState); // Notify main window
    accept(); // Close the dialog with success
}