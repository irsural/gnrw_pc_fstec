#include "detect_devices_dialog.h"
#include "ui_detect_devices_dialog.h"

#include <QMessageBox>


detect_devices_dialog::detect_devices_dialog(QWidget *parent, settings_t *ap_settings, gnrw_scanner_t* ap_gnrw_scanner) :
  QDialog(parent),
  ui(new Ui::detect_devices_dialog),
  mp_settings(ap_settings),
  mp_gnrw_scanner(ap_gnrw_scanner)
{
  ui->setupUi(this);

  connect(ui->cancel_button, &QAbstractButton::clicked, this, &detect_devices_dialog::reject);
  connect(mp_gnrw_scanner, &gnrw_scanner_t::new_gnrw_found, this, &detect_devices_dialog::append_ip_to_table);
  on_detect_devices_button_clicked();
}

detect_devices_dialog::~detect_devices_dialog()
{
  delete ui;
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
  mp_gnrw_scanner->scan();
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
