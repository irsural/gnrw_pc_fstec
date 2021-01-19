#ifndef GNRW_SCANNER_T_H
#define GNRW_SCANNER_T_H

#include <QUdpSocket>
#include <memory>

class gnrw_scanner_t : public QObject
{
  Q_OBJECT

public:
  gnrw_scanner_t();
  virtual ~gnrw_scanner_t() {};

  void scan();

signals:
  void new_gnrw_found(const QString &factory_number, const QHostAddress& a_ip);

private:
  enum class recv_data_order_t {
    ip = 0,
    factory_number = 1,
    count = factory_number + 1
  };

  // const чтобы компилятор не ругался
  static constexpr const char* m_ip_request_str = "pokrov_get_ip";
  static constexpr size_t m_broadcast_port = 5007;

  static constexpr const char* m_zero_network_broadcast_address = "255.255.255.255";

  std::map<QString, std::shared_ptr<QUdpSocket>> m_sockets;
  QHostAddress m_answer_address;
  uint16_t m_answer_port;

  void read_ip_from_socket();
};

#endif // GNRW_SCANNER_T_H
