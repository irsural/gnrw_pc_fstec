#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <irsdefs.h>

#include <QMainWindow>
#include <QTimer>
#include <QMovie>
#include <QLabel>

#include "setdeviceipdialog.h"
#include "settings.h"
#include "gnrw.h"
#include "gnrw_scanner.h"

#include <irsfinal.h>

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();
private slots:
  void tick();

  void on_etherPowerLevelSpinBox_valueChangedUser(int arg1);
  void on_linePowerLevelSpinBox_valueChangedUser(int arg1);
  void on_settingsAction_triggered();
  void on_setDeviceIPAction_triggered();
  void on_volumeSpinBox_valueChanged(int arg1);
  void on_showPowerIntervalSpinBox_valueChanged(int arg1);
  void on_showTimeIntervalSpinBox_valueChanged(int arg1);
  void on_resetPushButton_clicked();
  void on_brightnessSpinBox_valueChanged(int arg1);
  void on_boostCheckBox_clicked(bool checked);
  void on_onPushButton_clicked();
  void on_offPushButton_clicked();
  void on_detect_devices_action_triggered();

  void new_gnrw_found(const QString &a_factory_number, const QHostAddress& a_ip);
private:
  QString timeToStr(double a_t, bool a_show_seconds = false);
  void updateStatus(bool a_force = false);
  void updateDetectorsStatus(bool a_force = false);
  void updateDetectorStatus(bool a_new_status, bool* ap_status,
    QLabel* ap_label);
  void updateStatus(bool a_new_status, bool* ap_status,
    QLabel* ap_label);
  void setDetectorStatus(bool a_status, QLabel* label);
  void setStatus(bool a_status, QLabel* label);
  void updateId();
  void updateManagement();
  void updateTotalWorkTimeLabels();
  void updateWorkTimes();
  Ui::MainWindow *ui;
  QTimer m_timer;
  irs::loop_timer_t m_update_timer;

  enum class status_t {
    disconnected,
    connected,
    error
  };

  settings_t* mp_settings;
  gnrw_link_t m_gnrw_link;
  gnrw_t m_gnrw;
  status_t m_status;
  bool m_detector_1_status;
  bool m_detector_2_status;
  bool m_detector_3_status;
  bool m_detector_4_status;
  bool m_radio_status;
  bool m_line_status;
  bool m_connected;
  QMovie* mp_movie;
  bool m_connect_to_device_with_new_address;
  bool m_set_power_after_boost_reset;

  gnrw_scanner_t m_gnrw_scanner;

  uint32_t m_target_factory_number;
  irs::timer_t m_find_gnrw_timer;

  static constexpr uint32_t m_find_gnrw_try_count = 3;
  uint32_t m_current_find_gnrw_try;

  bool m_target_gnrw_found;
};

#endif // MAINWINDOW_H
