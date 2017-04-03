#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <irsdefs.h>

#include <QDialog>

#include "settings.h"

#include <irsfinal.h>

namespace Ui {
  class SettingsDialog;
}

class SettingsDialog : public QDialog
{
  Q_OBJECT

public:
  typedef std::size_t size_type;
  typedef irs::string_t string_type;

  explicit SettingsDialog(QWidget *parent,
    settings_t* ap_settings);
  ~SettingsDialog();

  void reset();
private slots:

  void on_pushButton_clicked();

  void on_pushButton_2_clicked();

private:
  void writeSettingsToInterface();
  void readSettingsFromInterface();
  Ui::SettingsDialog *ui;

  settings_t* mp_settings;
};

#endif // SETTINGSDIALOG_H
