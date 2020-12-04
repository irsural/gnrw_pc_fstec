#ifndef DETECT_DEVICES_DIALOG_H
#define DETECT_DEVICES_DIALOG_H

#include <QDialog>
#include <QTableWidgetItem>
#include <QUdpSocket>
#include <map>

#include "settings.h"

namespace Ui {
class detect_devices_dialog;
}

class detect_devices_dialog : public QDialog
{
  Q_OBJECT

public:
  explicit detect_devices_dialog(QWidget *parent, settings_t* ap_settings);
  ~detect_devices_dialog();

private slots:
  void on_detect_devices_button_clicked();
  void on_connect_button_clicked();
  void on_devices_table_itemDoubleClicked(QTableWidgetItem *item);
  void read_ip_from_socket();

private:
  enum class devices_table_column_t {
    factory_number,
    ip
  };

  enum class recv_data_order_t {
    ip = 0,
    factory_number = 1,
    count = factory_number + 1
  };

  // const чтобы компилятор не ругался
  static constexpr const char* m_ip_request_str = "pokrov_get_ip";
  static constexpr size_t m_broadcast_port = 5007;

  static constexpr const char* m_zero_network_broadcast_address = "255.255.255.255";

  Ui::detect_devices_dialog *ui;
  settings_t* mp_settings;

  std::map<QString, std::shared_ptr<QUdpSocket>> m_sockets;

  QHostAddress m_answer_address;
  uint16_t m_answer_port;

  void append_ip_to_table(const QString &factory_number, const QHostAddress& a_ip);
  bool set_selected_ip();
};

#endif // DETECT_DEVICES_DIALOG_H
