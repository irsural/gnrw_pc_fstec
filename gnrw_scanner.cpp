#include "gnrw_scanner.h"

#include <QNetworkInterface>
#include <QNetworkDatagram>

gnrw_scanner_t::gnrw_scanner_t() :
  m_sockets(),
  m_answer_address(),
  m_answer_port(0)
{
  const QHostAddress &localhost = QHostAddress(QHostAddress::LocalHost);
  for (auto& interface_: QNetworkInterface::allInterfaces()) {
    for (QNetworkAddressEntry &address : interface_.addressEntries()) {
      if (address.ip().protocol() == QAbstractSocket::IPv4Protocol && !address.netmask().isNull() &&
          address.ip() != localhost)
      {
        QHostAddress bcast = QHostAddress(address.ip().toIPv4Address() | ~address.netmask().toIPv4Address());

        auto socket = std::make_shared<QUdpSocket>(new QUdpSocket(this));
        socket->bind(address.ip());

        m_sockets[bcast.toString()] = socket;
        connect(socket.get(), &QUdpSocket::readyRead, this, &gnrw_scanner_t::read_ip_from_socket);
      }
    }
  }
}

void gnrw_scanner_t::scan()
{
  std::map<QString, std::shared_ptr<QUdpSocket>>::iterator it;
  for (it = m_sockets.begin(); it != m_sockets.end(); ++it) {
    it->second->writeDatagram(m_ip_request_str, QHostAddress(it->first), m_broadcast_port);
  }
}

void gnrw_scanner_t::read_ip_from_socket()
{
  std::map<QString, std::shared_ptr<QUdpSocket>>::iterator it;
  for (it = m_sockets.begin(); it != m_sockets.end(); ++it) {
    while (it->second->hasPendingDatagrams()) {
      QNetworkDatagram datagram = it->second->receiveDatagram();

      QString data = QString::fromStdString(datagram.data().toStdString());
      QStringList factory_ip = data.split(';');

      if (factory_ip.size() == static_cast<size_t>(recv_data_order_t::count)) {
        QHostAddress device_ip(factory_ip[static_cast<size_t>(recv_data_order_t::ip)]);
        if (QAbstractSocket::IPv4Protocol == device_ip.protocol()) {
          QString factory_number = factory_ip[static_cast<size_t>(recv_data_order_t::factory_number)];
          emit new_gnrw_found(factory_number, device_ip);
        }
      }
    }
  }
}
