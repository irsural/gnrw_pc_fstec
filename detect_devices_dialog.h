#ifndef DETECT_DEVICES_DIALOG_H
#define DETECT_DEVICES_DIALOG_H

#include <QDialog>
#include <QTableWidgetItem>
#include <QHostAddress>
#include <map>

#include "settings.h"
#include "gnrw_scanner.h"

namespace Ui {
class detect_devices_dialog;
}

class detect_devices_dialog : public QDialog
{
  Q_OBJECT

public:
  explicit detect_devices_dialog(QWidget *parent, settings_t* ap_settings, gnrw_scanner_t *ap_gnrw_scanner);
  ~detect_devices_dialog();

private slots:
  void on_detect_devices_button_clicked();
  void on_connect_button_clicked();
  void on_devices_table_itemDoubleClicked(QTableWidgetItem *item);

  void append_ip_to_table(const QString &factory_number, const QHostAddress& a_ip);

private:
  enum class devices_table_column_t {
    factory_number,
    ip
  };

  Ui::detect_devices_dialog *ui;
  settings_t* mp_settings;

  gnrw_scanner_t* mp_gnrw_scanner;

  bool set_selected_ip();
};

#endif // DETECT_DEVICES_DIALOG_H
