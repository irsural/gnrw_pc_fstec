#include "settingsdialog.h"
#include "ui_settingsdialog.h"

#include <irsfinal.h>

SettingsDialog::SettingsDialog(QWidget *parent, settings_t* ap_settings) :
  QDialog(parent),
  ui(new Ui::SettingsDialog),
  mp_settings(ap_settings)
{
  ui->setupUi(this);

  this->setFixedSize(this->sizeHint());
}

SettingsDialog::~SettingsDialog()
{
  delete ui;
}

void SettingsDialog::reset()
{
  writeSettingsToInterface();
}

void SettingsDialog::writeSettingsToInterface()
{
  ui->ipLineEdit->setText(
    irs::str_conv<QString>(mp_settings->gnrw_settings.ip));
}

void SettingsDialog::readSettingsFromInterface()
{
  mp_settings->gnrw_settings.ip =
    irs::str_conv<irs::string_t>(ui->ipLineEdit->text());
}

void SettingsDialog::on_pushButton_clicked()
{
  readSettingsFromInterface();
  mp_settings->save();
  accept();
}

void SettingsDialog::on_pushButton_2_clicked()
{
  reject();
}
