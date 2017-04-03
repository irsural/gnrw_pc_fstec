#include "setdeviceipdialog.h"
#include "ui_setdeviceipdialog.h"

#include <irssysutils.h>

#include <irsfinal.h>

SetDeviceIPDialog::SetDeviceIPDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::SetDeviceIPDialog),
  m_address()
{
  ui->setupUi(this);

  this->setFixedSize(this->sizeHint());
}

SetDeviceIPDialog::~SetDeviceIPDialog()
{
  delete ui;
}

address_t SetDeviceIPDialog::getAddress() const
{
  return m_address;
}

void SetDeviceIPDialog::setAddress(const address_t& a_address)
{
  m_address = a_address;

  ui->ipLineEdit->setText(irs::str_conv<QString>(a_address.ip));
  ui->maskLineEdit->setText(irs::str_conv<QString>(a_address.mask));
}

bool SetDeviceIPDialog::connectToDeviceWithNewAddress() const
{
  return ui->connectToDeviceWithNewAddressCheckBox->isChecked();
}

void SetDeviceIPDialog::on_okPushButton_clicked()
{
  m_address.ip = irs::str_conv<irs::string_t>(ui->ipLineEdit->text());
  m_address.mask = irs::str_conv<irs::string_t>(ui->maskLineEdit->text());

  accept();
}

void SetDeviceIPDialog::on_pushButton_clicked()
{
  reject();
}
