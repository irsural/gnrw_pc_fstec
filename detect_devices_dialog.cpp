#include "detect_devices_dialog.h"
#include "ui_detect_devices_dialog.h"

#include <QNetworkInterface>
#include <QNetworkDatagram>
#include <QMessageBox>


detect_devices_dialog::detect_devices_dialog(QWidget *parent, settings_t *ap_settings) :
  QDialog(parent),
  ui(new Ui::detect_devices_dialog),
  mp_settings(ap_settings),
  m_sockets(),
  m_answer_address(),
  m_answer_port(0)
{
  ui->setupUi(this);

  connect(ui->cancel_button, &QAbstractButton::clicked, this, &detect_devices_dialog::reject);

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
        connect(socket.get(), &QUdpSocket::readyRead, this, &detect_devices_dialog::read_ip_from_socket);
      }
    }
  }

  on_detect_devices_button_clicked();
}

detect_devices_dialog::~detect_devices_dialog()
{
  delete ui;
}

void detect_devices_dialog::read_ip_from_socket()
{
  for (auto& [_, socket]: m_sockets) {
    while (socket->hasPendingDatagrams()) {
      QNetworkDatagram datagram = socket->receiveDatagram();

      QString data = QString::fromStdString(datagram.data().toStdString());
      QStringList factory_ip = data.split(';');

      if (factory_ip.size() == static_cast<size_t>(recv_data_order_t::count)) {
        QHostAddress device_ip(factory_ip[static_cast<size_t>(recv_data_order_t::ip)]);
        if (QAbstractSocket::IPv4Protocol == device_ip.protocol()) {
          QString factory_number = factory_ip[static_cast<size_t>(recv_data_order_t::factory_number)];
          append_ip_to_table(factory_number, device_ip);
        }
      }
    }
  }
}

void detect_devices_dialog::append_ip_to_table(const QString& factory_number, const QHostAddress& a_ip)
{
  size_t row = ui->devices_table->rowCount();
  ui->devices_table->insertRow(row);

  auto factory_number_item = new QTableWidgetItem(QString(factory_number));
  factory_number_item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
  ui->devices_table->setItem(row, static_cast<int>(devices_table_column_t::factory_number), factory_number_item);

  auto ip_item = new QTableWidgetItem(a_ip.toString());
  ip_item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
  ui->devices_table->setItem(row, static_cast<int>(devices_table_column_t::ip), ip_item);
}

bool detect_devices_dialog::set_selected_ip()
{
  auto selected_items = ui->devices_table->selectedItems();
  if (!selected_items.empty()) {
    QString ip_str = selected_items[static_cast<size_t>(devices_table_column_t::ip)]->text();

    mp_settings->gnrw_settings.ip = irs::str_conv<irs::string_t>(ip_str);
    return true;
  } else {
    QMessageBox::warning(this, tr("Ошибка"), tr("IP-адрес не выбран"),
      QMessageBox::Ok, QMessageBox::Ok);
    return false;
  }
}

void detect_devices_dialog::on_detect_devices_button_clicked()
{
  ui->devices_table->setRowCount(0);

  for (auto& [addr, socket]: m_sockets) {
    socket->writeDatagram(m_ip_request_str, QHostAddress(addr), m_broadcast_port);
  }
}

void detect_devices_dialog::on_connect_button_clicked()
{
  if (set_selected_ip()) {
    mp_settings->save();
    accept();
  }
}

void detect_devices_dialog::on_devices_table_itemDoubleClicked(QTableWidgetItem* /*a_item*/)
{
  if (set_selected_ip()) {
    mp_settings->save();
    accept();
  }
}
