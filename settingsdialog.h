/**
 * @file settingsdialog.h
 * @brief Implements the in-game Settings dialog for toggling music and sound effects.
 * 
 * Provides checkboxes for enabling/disabling background music and sound effects.
 * Changes are only applied after pressing the "Apply" button. Also includes a cancel option
 * to discard changes. Integrates sound feedback using MainWindow's sound system.
 * 
 * 
 * @author Krish Bhavin
 */

#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QPushButton>

/**
 * @class SettingsDialog
 * @brief Dialog for toggling background music and sound effects.
 * 
 * This dialog allows the player to enable or disable audio features.
 */
class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief Constructor for the settings dialog.
     * @param initialMusicState Initial state of background music toggle.
     * @param initialSoundEffectsState Initial state of sound effects toggle.
     * @param parent Parent widget (usually MainWindow).
     */
    explicit SettingsDialog(bool initialMusicState, bool initialSoundEffectsState, QWidget *parent = nullptr);

signals:
    /**
     * @brief Signal emitted when music setting is toggled.
     * @param enabled True if music should be enabled.
     */
    void musicToggled(bool enabled);

    /**
     * @brief Signal emitted when sound effects setting is toggled.
     * @param enabled True if sound effects should be enabled.
     */
    void soundEffectsToggled(bool enabled);

private slots:
    /**
     * @brief Slot for handling music checkbox state change.
     * @param state New checkbox state (Qt::Checked or Qt::Unchecked).
     */
    void onMusicCheckboxStateChanged(int state);

    /**
     * @brief Slot for handling sound effects checkbox state change.
     * @param state New checkbox state (Qt::Checked or Qt::Unchecked).
     */
    void onSoundEffectsCheckboxStateChanged(int state);

    /**
     * @brief Applies the user's selected settings and closes the dialog.
     */
    void applySettings();

private:
    QCheckBox *musicCheckBox;             // Checkbox for background music
    QCheckBox *soundEffectsCheckBox;      // Checkbox for sound effects
    QPushButton *applyButton;             // Button to confirm and apply settings
    QPushButton *cancelButton;            // Button to cancel and discard changes
    bool currentMusicState;               // Stores music toggle state
    bool currentSoundEffectsState;        // Stores sound effects toggle state
};

#endif // SETTINGSDIALOG_H