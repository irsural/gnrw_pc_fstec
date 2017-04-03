#ifndef SETDEVICEIPDIALOG_H
#define SETDEVICEIPDIALOG_H

#include <irsdefs.h>

#include <QDialog>

#include <irsstrconv.h>

#include <irsfinal.h>

namespace Ui {
  class SetDeviceIPDialog;
}

struct address_t
{
  typedef irs::string_t string_type;
  string_type ip;
  string_type mask;
  bool operator==(const address_t& a_address) const
  {
    return (ip == a_address.ip) && (mask == a_address.mask);
  }
  bool operator!=(const address_t& a_address) const
  {
    return !operator==(a_address);
  }
};

class SetDeviceIPDialog : public QDialog
{
  Q_OBJECT

public:
  explicit SetDeviceIPDialog(QWidget *parent = 0);
  ~SetDeviceIPDialog();

  address_t getAddress() const;
  void setAddress(const address_t& a_address);
  bool connectToDeviceWithNewAddress() const;
private slots:
  void on_okPushButton_clicked();

  void on_pushButton_clicked();

private:
  typedef irs::string_t string_type;
  Ui::SetDeviceIPDialog *ui;

  address_t m_address;
};

#endif // SETDEVICEIPDIALOG_H
