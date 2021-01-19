#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTranslator>
#include <QMessageBox>

#include <irs_codecvt.h>
#include <irslocale.h>

#include "settingsdialog.h"
#include "detect_devices_dialog.h"

#include <irsfinal.h>

#define TMP_DETECT_DEVICES_ENABLE


MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow),
  m_timer(),
  m_update_timer(irs::make_cnt_s(0.1)),
  mp_settings(settings_t::instance()),
  m_gnrw_link(gnrw_settings_t()),
  m_gnrw(),
  m_status(status_t::disconnected),
  m_detector_1_status(false),
  m_detector_2_status(false),
  m_detector_3_status(false),
  m_detector_4_status(false),
  m_radio_status(false),
  m_line_status(false),
  m_connected(false),
  mp_movie(),
  m_connect_to_device_with_new_address(false),
  m_set_power_after_boost_reset(false),
  m_gnrw_scanner(),
  m_target_factory_number(0),
  m_find_gnrw_timer(irs::make_cnt_ms(1000)),
  m_current_find_gnrw_try(0),
  m_target_gnrw_found(false)
{
  ui->setupUi(this);

  connect(&m_timer, SIGNAL(timeout()), this, SLOT(tick()));

  connect(ui->etherPowerLevelSpinBox, SIGNAL(valueChanged(int)),
    this, SLOT(on_etherPowerLevelSpinBox_valueChangedUser(int)));

  connect(ui->linePowerLevelSpinBox, SIGNAL(valueChanged(int)),
    this, SLOT(on_linePowerLevelSpinBox_valueChangedUser(int)));


  connect(ui->etherPowerLevelSpinBox, SIGNAL(valueChanged(int)),
    ui->etherPowerLevelHorizontalSlider, SLOT(setValue(int)));
  connect(ui->etherPowerLevelHorizontalSlider, SIGNAL(valueChanged(int)),
    ui->etherPowerLevelSpinBox, SLOT(setValue(int)));

  connect(ui->linePowerLevelSpinBox, SIGNAL(valueChanged(int)),
    ui->linePowerLevelHorizontalSlider, SLOT(setValue(int)));
  connect(ui->linePowerLevelHorizontalSlider, SIGNAL(valueChanged(int)),
    ui->linePowerLevelSpinBox, SLOT(setValue(int)));

  connect(ui->volumeSpinBox, SIGNAL(valueChanged(int)),
    ui->volumeHorizontalSlider, SLOT(setValue(int)));
  connect(ui->volumeHorizontalSlider, SIGNAL(valueChanged(int)),
    ui->volumeSpinBox, SLOT(setValue(int)));

  connect(ui->brightnessSpinBox, SIGNAL(valueChanged(int)),
    ui->brightnessHorizontalSlider, SLOT(setValue(int)));
  connect(ui->brightnessHorizontalSlider, SIGNAL(valueChanged(int)),
    ui->brightnessSpinBox, SLOT(setValue(int)));

  connect(ui->showPowerIntervalSpinBox, SIGNAL(valueChanged(int)),
    ui->showPowerIntervalHorizontalSlider, SLOT(setValue(int)));
  connect(ui->showPowerIntervalHorizontalSlider, SIGNAL(valueChanged(int)),
    ui->showPowerIntervalSpinBox, SLOT(setValue(int)));

  connect(ui->showTimeIntervalSpinBox, SIGNAL(valueChanged(int)),
    ui->showTimeIntervalHorizontalSlider, SLOT(setValue(int)));
  connect(ui->showTimeIntervalHorizontalSlider, SIGNAL(valueChanged(int)),
    ui->showTimeIntervalSpinBox, SLOT(setValue(int)));

  setWindowTitle(QString::fromLocal8Bit(PROGNAME_STR) + " " +
    QString::fromLocal8Bit(VERSION_STR));

  irs::loc();
  setlocale(LC_ALL, "Russian_Russia.1251");

  m_timer.start(10);

  this->setFixedSize(this->sizeHint());

  mp_settings->load();

  m_gnrw_link.set_settings(mp_settings->gnrw_settings);
  m_gnrw_link.gnrw(&m_gnrw);

  m_gnrw_link.enabled(true);

  mp_movie = new QMovie(this);

  mp_movie->setFileName(":/Images/Images/wait.gif");
  mp_movie->setScaledSize(QSize(20, 20));
  mp_movie->start();

  ui->statusIconLabel->setAttribute(Qt::WA_NoSystemBackground);
  updateStatus(true);

  ui->detector1IconLabel->setAttribute(Qt::WA_NoSystemBackground);
  ui->detector2IconLabel->setAttribute(Qt::WA_NoSystemBackground);
  ui->detector3IconLabel->setAttribute(Qt::WA_NoSystemBackground);
  ui->detector4IconLabel->setAttribute(Qt::WA_NoSystemBackground);
  updateDetectorsStatus(true);

  // �������� ����� "������������ ��������"
  ui->boostCheckBox->setVisible(false);

  #ifndef GNRWPC_FSB
  ui->label_2->setVisible(false);
  ui->etherPowerLevelSpinBox->setVisible(false);
  ui->etherPowerLevelHorizontalSlider->setVisible(false);
  ui->label->setVisible(false);
  ui->linePowerLevelSpinBox->setVisible(false);
  ui->linePowerLevelHorizontalSlider->setVisible(false);
  ui->showPowerIntervalLabel->setVisible(false);
  ui->showPowerIntervalSpinBox->setVisible(false);
  ui->showPowerIntervalHorizontalSlider->setVisible(false);
  ui->showTimeIntervalLabel->setVisible(false);
  ui->showTimeIntervalSpinBox->setVisible(false);
  ui->showTimeIntervalHorizontalSlider->setVisible(false);
  #endif //GNRWPC_FSB

  #ifdef TMP_DETECT_DEVICES_ENABLE
  ui->menu->addAction(ui->detect_devices_action);
  #endif
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::tick()
{
  m_gnrw_link.tick();
  m_gnrw.tick();

  static bool gnrw_connected_prev = false;
  if (m_gnrw.connected() != gnrw_connected_prev) {
    if (m_gnrw.on()) {
      ui->onPushButton->setChecked(true);
      ui->offPushButton->setChecked(false);
    } else {
      ui->onPushButton->setChecked(false);
      ui->offPushButton->setChecked(true);
    }
  }
  gnrw_connected_prev = m_gnrw.connected();

  if (m_update_timer.check()) {
    updateStatus();
    updateDetectorsStatus();

    ui->setDeviceIPAction->setEnabled(m_gnrw.connected());

    updateId();
    updateManagement();
    updateTotalWorkTimeLabels();
    updateWorkTimes();
  }

  if (m_connect_to_device_with_new_address) {
    if (m_gnrw.ip_change_success_check()) {
      connect(&m_gnrw_scanner, &gnrw_scanner_t::new_gnrw_found, this, &MainWindow::new_gnrw_found);

      m_current_find_gnrw_try = 0;
      m_target_gnrw_found = false;
      m_gnrw_scanner.scan();
      m_find_gnrw_timer.start();
    }

    if (m_target_gnrw_found) {
      m_connect_to_device_with_new_address = false;

    } else if (m_find_gnrw_timer.check()) {
      m_current_find_gnrw_try++;

      if (m_current_find_gnrw_try < m_find_gnrw_try_count) {
        m_gnrw_scanner.scan();
        m_find_gnrw_timer.start();
      } else {
        m_connect_to_device_with_new_address = false;
        disconnect(&m_gnrw_scanner, &gnrw_scanner_t::new_gnrw_found, this, &MainWindow::new_gnrw_found);
      }
    }
  }
}

void MainWindow::updateStatus(bool a_force)
{
  status_t new_status = status_t::disconnected;
  if (m_gnrw.connected()) {
    if (m_gnrw.get_error_status()) {
      new_status = status_t::error;
    } else {
      new_status = status_t::connected;
    }
  } else {
    new_status = status_t::disconnected;
  }

  if ((m_status != new_status) || a_force) {
    m_status = new_status;
    std::wstring status;
    switch (m_status) {
      case status_t::disconnected: {
        status = L"������� ����������� � �����������";
        ui->statusIconLabel->setMovie(mp_movie);
      } break;
      case status_t::connected: {
        status = L"���������� �����������";
        QPixmap pixmap(":/Images/Images/success.png");
        ui->statusIconLabel->setPixmap(pixmap.scaled(QSize(20, 20),
          Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
      } break;
      case status_t::error: {
        status = L"������, ����������� ���������";
        QPixmap pixmap(":/Images/Images/error.png");
        ui->statusIconLabel->setPixmap(pixmap.scaled(QSize(20, 20),
          Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
      } break;
    }
    ui->statusLabel->setText(irs::str_conv<QString>(status));
  }
}

void MainWindow::updateDetectorsStatus(bool a_force)
{
  if (a_force) {
    m_detector_1_status = m_gnrw.get_detector_1_status();
    m_detector_2_status = m_gnrw.get_detector_2_status();
    m_detector_3_status = m_gnrw.get_detector_3_status();
    m_detector_4_status = m_gnrw.get_detector_4_status();
    m_radio_status = m_gnrw.get_work_radio_status();
    m_line_status = m_gnrw.get_work_line_status();
    setDetectorStatus(m_detector_1_status, ui->detector1IconLabel);
    setDetectorStatus(m_detector_2_status, ui->detector2IconLabel);
    setDetectorStatus(m_detector_3_status, ui->detector3IconLabel);
    setDetectorStatus(m_detector_4_status, ui->detector4IconLabel);
    setStatus(m_radio_status, ui->radioStatusIconLabel);
    setStatus(m_line_status, ui->lineStatusIconLabel);
    return;
  }

  updateDetectorStatus(m_gnrw.get_detector_1_status(), &m_detector_1_status,
    ui->detector1IconLabel);
  updateDetectorStatus(m_gnrw.get_detector_2_status(), &m_detector_2_status,
    ui->detector2IconLabel);
  updateDetectorStatus(m_gnrw.get_detector_3_status(), &m_detector_3_status,
    ui->detector3IconLabel);
  updateDetectorStatus(m_gnrw.get_detector_4_status(), &m_detector_4_status,
    ui->detector4IconLabel);
  updateStatus(m_gnrw.get_work_radio_status(), &m_radio_status,
    ui->radioStatusIconLabel);
  updateStatus(m_gnrw.get_work_line_status(), &m_line_status,
    ui->lineStatusIconLabel);

  ui->groupBox->setEnabled(m_gnrw.connected());
}

void MainWindow::updateDetectorStatus(bool a_new_status, bool* ap_status,
  QLabel* ap_label)
{
  if (*ap_status != a_new_status) {
    setDetectorStatus(a_new_status, ap_label);
  }
  *ap_status = a_new_status;
}

void MainWindow::updateStatus(bool a_new_status, bool* ap_status,
  QLabel* ap_label)
{
  if (*ap_status != a_new_status) {
    setStatus(a_new_status, ap_label);
  }
  *ap_status = a_new_status;
}

void MainWindow::setDetectorStatus(bool a_status, QLabel* ap_label)
{
  QPixmap pixmap;
  if (a_status) {
    pixmap.load(":/Images/Images/greenStone.png");
  } else {
    pixmap.load(":/Images/Images/yellowStone.png");
  }
  ap_label->setPixmap(pixmap.scaled(QSize(20, 20),
    Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
}

void MainWindow::setStatus(bool a_status, QLabel* ap_label)
{
  QPixmap pixmap;
  if (a_status) {
    pixmap.load(":/Images/Images/greenStone.png");
  } else {
    pixmap.load(":/Images/Images/redStone.png");
  }
  ap_label->setPixmap(pixmap.scaled(QSize(20, 20),
    Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
}

void MainWindow::updateId()
{
  ui->idLabel->setText(QString::number(m_gnrw.get_id()));
}

void MainWindow::updateManagement()
{
  if (m_gnrw.connected()) {

    if (m_gnrw.synchronizes()) {
      ui->managementGroupBox->setTitle(
        irs::str_conv<QString>(std::wstring(L"���������� (�������������)")));
    } else {
      ui->managementGroupBox->setTitle(
        irs::str_conv<QString>(std::wstring(L"����������")));
    }

    if (!m_gnrw.synchronizes()) {

      ui->boostCheckBox->setChecked(m_gnrw.get_boost());

      // ���� ��� ����� ����� "������������ ��������" ����� ���������
      // ������ ��������, �� ���������� ������ �������� � ����������
      if (m_set_power_after_boost_reset) {
        m_set_power_after_boost_reset = false;
        m_gnrw.set_ether_power(ui->etherPowerLevelSpinBox->value());
        m_gnrw.set_line_power(ui->linePowerLevelSpinBox->value());
      } else {
        const int ether_power = static_cast<int>(m_gnrw.get_ether_power());
        if (ui->etherPowerLevelSpinBox->value() != ether_power) {
          // ��������� ���� ����� ���������� ��������, ����� ����� ����������
          disconnect(ui->etherPowerLevelSpinBox, SIGNAL(valueChanged(int)),
            this, SLOT(on_etherPowerLevelSpinBox_valueChangedUser(int)));

          ui->etherPowerLevelSpinBox->setValue(ether_power);

          connect(ui->etherPowerLevelSpinBox, SIGNAL(valueChanged(int)),
            this, SLOT(on_etherPowerLevelSpinBox_valueChangedUser(int)));
        }

        const int line_power = static_cast<int>(m_gnrw.get_line_power());
        if (ui->linePowerLevelSpinBox->value() != line_power) {
          // ��������� ���� ����� ���������� ��������, ����� ����� ����������
          disconnect(ui->linePowerLevelSpinBox, SIGNAL(valueChanged(int)),
            this, SLOT(on_linePowerLevelSpinBox_valueChangedUser(int)));

          ui->linePowerLevelSpinBox->setValue(line_power);

          connect(ui->linePowerLevelSpinBox, SIGNAL(valueChanged(int)),
            this, SLOT(on_linePowerLevelSpinBox_valueChangedUser(int)));
        }
      }

      ui->volumeSpinBox->setValue(m_gnrw.get_volume());

      ui->showPowerIntervalSpinBox->setValue(m_gnrw.get_show_power_interval());

      ui->showTimeIntervalSpinBox->setValue(m_gnrw.get_show_time_interva());

      ui->brightnessSpinBox->setValue(m_gnrw.get_brightness());
    }

    if (!m_connected) {
      ui->managementGroupBox->setEnabled(true);
    }
    m_connected = true;
  } else {
    m_connected = false;
    ui->managementGroupBox->setEnabled(false);
  }
}

void MainWindow::updateTotalWorkTimeLabels()
{
  std::wstringstream ostr;
  ostr << L"����� ����� ������ ���� (������� " <<
    ui->etherPowerLevelSpinBox->value()
    << L")";
  ui->etherTotalWorkTimeForCurrentLevelNameLabel->setText(
    irs::str_conv<QString>(ostr.str()));

  ostr.str(std::wstring());

  ostr << L"����� ����� ������ ���� ������� (������� " <<
    ui->linePowerLevelSpinBox->value()
    << L")";
  ui->lineTotalWorkTimeForCurrentLevelNameLabel->setText(
    irs::str_conv<QString>(ostr.str()));
}

void MainWindow::updateWorkTimes()
{
  const double wt = m_gnrw.get_work_time();
  ui->workTimeLabel->setText(timeToStr(wt, true));

  double ewt = m_gnrw.get_ether_work_time_for_current_level();
  ui->etherTotalWorkTimeForCurrentLevelLabel->setText(timeToStr(ewt));

  double lwt = m_gnrw.get_line_work_time_for_current_level();
  ui->lineTotalWorkTimeForCurrentLevelLabel->setText(timeToStr(lwt));
}

QString MainWindow::timeToStr(double a_t, bool a_show_seconds)
{
  size_t sec = static_cast<size_t>(a_t);

  size_t years = sec/(365*24*60*60);

  size_t days = (sec%(365*24*60*60))/(24*60*60);
  days = irs::range<size_t>(days, 0, 365);

  size_t hours = (sec%(24*60*60))/(60*60);
  hours = irs::range<size_t>(hours, 0, 24);

  size_t minutes = (sec%(60*60))/60;
  irs::range<size_t>(minutes, 0, 59);

  size_t seconds = sec%60;

  QString text;
  if (years > 0) {
    text = text +
      QString::number(years) + irs::str_conv<QString>(std::wstring(L" �. "));
  }

  if ((years > 0) || (days > 0)) {
    text = text +
      QString::number(days) + irs::str_conv<QString>(std::wstring(L" �. "));
  }

  if ((years > 0) || (days > 0)|| (hours > 0)) {
    text = text +
      QString::number(hours) + irs::str_conv<QString>(std::wstring(L" �. "));
  }

  text = text +
    QString::number(minutes) + irs::str_conv<QString>(std::wstring(L" ���."));

  if (a_show_seconds) {
    text = text + irs::str_conv<QString>(std::wstring(L" ")) +
      QString::number(seconds) + irs::str_conv<QString>(std::wstring(L" c��."));
  }

  return text;
}

void MainWindow::on_etherPowerLevelSpinBox_valueChangedUser(int arg1)
{
  // ���� ���� ������ ���������� ������ ��� ��������� �������� �������������


  // ���� ������� ����� "������������ ��������", �� ������� ����������
  // ���� �����, ���� ���������� � ����� ���������� ������ ��������
  if (ui->boostCheckBox->isChecked()) {
    m_gnrw.set_boost(false);
    m_set_power_after_boost_reset = true;
  } else {
    m_gnrw.set_ether_power(arg1);
  }
}

void MainWindow::on_linePowerLevelSpinBox_valueChangedUser(int arg1)
{
  // ���� ���� ������ ���������� ������ ��� ��������� �������� �������������


  // ���� ������� ����� "������������ ��������", �� ������� ����������
  // ���� �����, ���� ���������� � ����� ���������� ������ ��������
  if (ui->boostCheckBox->isChecked()) {
    m_gnrw.set_boost(false);
    m_set_power_after_boost_reset = true;
  } else {
    m_gnrw.set_line_power(arg1);
  }
}

void MainWindow::on_settingsAction_triggered()
{
  SettingsDialog dialog(0, mp_settings);
  dialog.reset();
  const int res = dialog.exec();
  if (res == QDialog::Accepted) {
    if (m_gnrw_link.get_settings() != mp_settings->gnrw_settings) {
      m_gnrw_link.enabled(false);
      m_gnrw_link.set_settings(mp_settings->gnrw_settings);
      m_gnrw_link.enabled(true);
    }
    mp_settings->save();
  }
}

void MainWindow::on_setDeviceIPAction_triggered()
{
  SetDeviceIPDialog dialog;
  address_t address;
  m_gnrw.get_network_address(&address.ip, &address.mask);

  dialog.setAddress(address);
  if (dialog.exec() == QDialog::Accepted) {
    address = dialog.getAddress();
    bool dhcp_enabled = dialog.is_dhcp_enabled();
    m_target_factory_number = m_gnrw.get_id();

    if (m_gnrw.set_network_address(address.ip, address.mask, dhcp_enabled)) {
      m_connect_to_device_with_new_address = true;
      m_target_gnrw_found = false;
      m_find_gnrw_timer.stop();
    } else {
      QMessageBox msgBox;
      msgBox.setWindowTitle(irs::str_conv<QString>(std::wstring(L"������")));
      QString text = irs::str_conv<QString>(
        std::wstring(L"�� ������� ������ ���������� ����� �����"));
      msgBox.setText(text);
      msgBox.setStandardButtons(QMessageBox::Ok);
      msgBox.setDefaultButton(QMessageBox::Ok);
      msgBox.setIcon(QMessageBox::Information);

      QFont fond = msgBox.font();
      fond.setPointSize(font().pointSize());
      msgBox.setFont(fond);
      msgBox.exec();
    }
  }
}

void MainWindow::new_gnrw_found(const QString &a_factory_number, const QHostAddress& a_ip)
{
  if (a_factory_number.toUInt() == m_target_factory_number) {
    mp_settings->gnrw_settings.ip = irs::str_conv<settings_t::string_type>(a_ip.toString());
    // ���������������
    m_gnrw_link.enabled(false);
    m_gnrw_link.set_settings(mp_settings->gnrw_settings);
    m_gnrw_link.enabled(true);
    mp_settings->save();

    m_target_gnrw_found = true;
    disconnect(&m_gnrw_scanner, &gnrw_scanner_t::new_gnrw_found, this, &MainWindow::new_gnrw_found);
  }
}

void MainWindow::on_volumeSpinBox_valueChanged(int arg1)
{
  m_gnrw.set_volume(arg1);
}

void MainWindow::on_showPowerIntervalSpinBox_valueChanged(int arg1)
{
  m_gnrw.set_show_power_interval(arg1);
}

void MainWindow::on_showTimeIntervalSpinBox_valueChanged(int arg1)
{
  m_gnrw.set_show_time_interval(arg1);
}

void MainWindow::on_resetPushButton_clicked()
{
  QMessageBox msgBox;
  msgBox.setWindowTitle(irs::str_conv<QString>(std::wstring(L"������")));
  QString text = irs::str_conv<QString>(
    std::wstring(L"�� ������������� ������ �������� ���������?"));
  msgBox.setText(text);
  msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
  msgBox.setDefaultButton(QMessageBox::Cancel);
  msgBox.setIcon(QMessageBox::Question);

  QFont fond = msgBox.font();
  fond.setPointSize(font().pointSize());
  msgBox.setFont(fond);

  if (msgBox.exec() == QMessageBox::Ok) {
    m_gnrw.reset();
  }
}

void MainWindow::on_brightnessSpinBox_valueChanged(int arg1)
{
  m_gnrw.set_brightness(arg1);
}

void MainWindow::on_boostCheckBox_clicked(bool checked)
{
  m_gnrw.set_boost(checked);
}

void MainWindow::on_onPushButton_clicked()
{
  m_gnrw.on(true);
  ui->onPushButton->setChecked(true);
  ui->offPushButton->setChecked(false);
}

void MainWindow::on_offPushButton_clicked()
{
  m_gnrw.on(false);
  ui->onPushButton->setChecked(false);
  ui->offPushButton->setChecked(true);
}

void MainWindow::on_detect_devices_action_triggered()
{
  detect_devices_dialog dialog(this, mp_settings, &m_gnrw_scanner);
  const int res = dialog.exec();
  if (res == QDialog::Accepted) {
    if (m_gnrw_link.get_settings() != mp_settings->gnrw_settings) {
      m_gnrw_link.enabled(false);
      m_gnrw_link.set_settings(mp_settings->gnrw_settings);
      m_gnrw_link.enabled(true);
    }
    mp_settings->save();
  }
}
