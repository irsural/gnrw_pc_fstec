#include <algorithm>

#include <irsstring.h>

#include <QMessageBox>

#include "gnrw.h"

#include <irsfinal.h>

// class gnrw_t
gnrw_t::gnrw_t():
  mp_data(NULL),
  m_network_variables(),
  m_sync_timer(irs::make_cnt_s(2)),
  m_apply_ip_timer(irs::make_cnt_s(2)),
  m_wait_apply_ip_timer(irs::make_cnt_s(2))
  #ifdef GNRWPC_POWER_ONOFF_FSTEC_OLD
  ,
  m_power_index(1),
  m_wait_power_change_timer(irs::make_cnt_ms(500)),
  m_ether_power_on(m_power_max),
  m_line_power_on(m_power_max),
  m_ether_power_time_list(),
  m_line_power_time_list(),
  m_is_on_power_finded(false),
  m_is_on_power_find_perform(false)
  #endif //GNRWPC_POWER_ONOFF_FSTEC_OLD
{
}

void gnrw_t::connect(irs::mxdata_t* ap_data)
{
  mp_data = ap_data;
  m_network_variables.connect(mp_data);
}

bool gnrw_t::connected() const
{
  if (mp_data) {
    if (mp_data->connected()) {
      return true;
    }
  }
  return false;
}

bool gnrw_t::get_boost() const
{
  if (connected()) {
    return false; //m_network_variables.boost_bit;
  }
  return false;
}

void gnrw_t::set_boost(bool)
{
  if (!connected()) {
     return;
  }
  //m_network_variables.boost_bit = a_enable;
  m_sync_timer.start();
}

bool gnrw_t::get_error_status() const
{
  if (connected()) {
    return m_network_variables.fail_bit;
  }
  return false;
}

bool gnrw_t::get_detector_1_status() const
{
  if (!connected()) {
    return 0;
  }
  return m_network_variables.work1_bit;
}

bool gnrw_t::get_detector_2_status() const
{
  if (!connected()) {
    return 0;
  }
  return m_network_variables.work2_bit;
}

bool gnrw_t::get_detector_3_status() const
{
  if (!connected()) {
    return 0;
  }
  return m_network_variables.work3_bit;
}

bool gnrw_t::get_detector_4_status() const
{
  if (!connected()) {
    return 0;
  }
  return m_network_variables.work4_bit;
}

bool gnrw_t::get_work_radio_status() const
{
  if (!connected()) {
    return 0;
  }
  return m_network_variables.work_radio_bit;
}

bool gnrw_t::get_work_line_status() const
{
  if (!connected()) {
    return 0;
  }
  return m_network_variables.work_line_bit;
}

size_t gnrw_t::get_id() const
{
  if (!connected()) {
    return 0;
  }
  return m_network_variables.id;
}

double gnrw_t::get_work_time() const
{
  if (connected()) {
    return m_network_variables.work_time/10.;
  }
  return 0;
}

double gnrw_t::get_ether_work_time_for_current_level() const
{
  if (connected()) {
    return m_network_variables.ether_work_time_for_current_level*0.1*60*60;
  }
  return 0;
}

double gnrw_t::get_line_work_time_for_current_level() const
{
  if (connected()) {
    return m_network_variables.line_work_time_for_current_level*0.1*60*60;
  }
  return 0;
}

size_t gnrw_t::get_ether_power() const
{
  if (connected()) {
    return m_network_variables.ether_power;
  }
  return 0;
}

void gnrw_t::set_ether_power(size_t a_level)
{
  if (connected()) {
    m_network_variables.ether_power = a_level;
    m_sync_timer.start();
  }
}

size_t gnrw_t::get_line_power() const
{
  if (connected()) {
    return m_network_variables.line_power;
  }
  return 0;
}

void gnrw_t::set_line_power(size_t a_level)
{
  if (connected()) {
    m_network_variables.line_power = a_level;
    m_sync_timer.start();
  }
}

void gnrw_t::get_network_address(string_type* ap_ip, string_type* ap_mask)
{
  ap_ip->clear();
  ap_mask->clear();

  if (connected()) {
    mxip_t ip;
    ip.val[0] = m_network_variables.ip1;
    ip.val[1] = m_network_variables.ip2;
    ip.val[2] = m_network_variables.ip3;
    ip.val[3] = m_network_variables.ip4;
    *ap_ip = mxip_to_str(ip);

    mxip_t mask;
    mask.val[0] = m_network_variables.mask1;
    mask.val[1] = m_network_variables.mask2;
    mask.val[2] = m_network_variables.mask3;
    mask.val[3] = m_network_variables.mask4;
    *ap_mask = mxip_to_str(mask);
  }
}

bool gnrw_t::set_network_address(const string_type& a_ip,
  const string_type& a_mask, bool a_dhcp_enable)
{
  mxip_t ip;
  if (!str_to_mxip(a_ip, &ip)) {
    return false;
  }
  mxip_t mask;
  if (!str_to_mxip(a_mask, &mask)) {
    return false;
  }
  if (!connected()) {
    return false;
  }
  m_network_variables.ip1 = ip.val[0];
  m_network_variables.ip2 = ip.val[1];
  m_network_variables.ip3 = ip.val[2];
  m_network_variables.ip4 = ip.val[3];

  m_network_variables.mask1 = mask.val[0];
  m_network_variables.mask2 = mask.val[1];
  m_network_variables.mask3 = mask.val[2];
  m_network_variables.mask4 = mask.val[3];

  m_network_variables.dhcp_bit = a_dhcp_enable;

  m_apply_ip_timer.start();

  return true;
}

size_t gnrw_t::get_volume() const
{
  if (!connected()) {
     return 0;
  }
  return m_network_variables.volume;
}

void gnrw_t::set_volume(size_t a_level)
{
  if (!connected()) {
     return;
  }
  m_network_variables.volume = a_level;
  m_sync_timer.start();
}

size_t gnrw_t::get_brightness() const
{
  if (!connected()) {
    return 0;
  }
  return m_network_variables.bright;
}

void gnrw_t::set_brightness(size_t a_value)
{
  if (!connected()) {
     return;
  }
  m_network_variables.bright = a_value;
  m_sync_timer.start();
}

size_t gnrw_t::get_show_power_interval() const
{
  if (!connected()) {
    return 0;
  }
  return m_network_variables.show_power_interval;
}

void gnrw_t::set_show_power_interval(size_t a_time)
{
  if (!connected()) {
    return;
  }
  m_network_variables.show_power_interval = a_time;
  m_sync_timer.start();
}

size_t gnrw_t::get_show_time_interva() const
{
  if (!connected()) {
    return 0;
  }
  return m_network_variables.show_time_interval;
}

void gnrw_t::set_show_time_interval(size_t a_time)
{
  if (!connected()) {
    return;
  }
  m_network_variables.show_time_interval = a_time;
  m_sync_timer.start();
}

void gnrw_t::on(bool a_on)
{
  #ifdef GNRWPC_POWER_ONOFF_FSTEC_OLD
  if (!connected()) {
    return;
  }
  if (a_on) {
    if (m_is_on_power_finded) {
      m_network_variables.ether_power = m_ether_power_on;
      m_network_variables.line_power = m_line_power_on;
    } else {
      on_power_find_start();
    }
  } else {
    m_network_variables.ether_power = 0;
    m_network_variables.line_power = 0;
  }
  #else //GNRWPC_POWER_ONOFF_FSTEC_OLD
  if (a_on) {
    m_network_variables.on_bit = 1;
  } else {
    m_network_variables.on_bit = 0;
  }
  #endif //GNRWPC_POWER_ONOFF_FSTEC_OLD
}

bool gnrw_t::on()
{
  if (!connected()) {
    return false;
  }

  #ifdef GNRWPC_POWER_ONOFF_FSTEC_OLD
  return (get_ether_power() > 0) || (get_line_power() > 0);
  #else //GNRWPC_POWER_ONOFF_FSTEC_OLD
  return m_network_variables.on_bit == 1;
  #endif //GNRWPC_POWER_ONOFF_FSTEC_OLD
}

bool gnrw_t::ip_change_success_check()
{
  return m_wait_apply_ip_timer.check();
}

void gnrw_t::reset()
{
  //m_network_variables.boost_bit = 1;
  #ifdef GNRWPC_FSB
  m_network_variables.ether_power = 9;
  m_network_variables.line_power = 9;
  m_network_variables.show_power_interval = 1;
  m_network_variables.show_time_interval = 5;
  #endif //GNRWPC_FSB
  m_network_variables.volume = 1;
  m_network_variables.bright = 2;
  m_sync_timer.start();
}

bool gnrw_t::synchronizes()
{
  m_sync_timer.check();
  return !m_sync_timer.stopped();
}

#ifdef GNRWPC_POWER_ONOFF_FSTEC_OLD
void gnrw_t::on_power_find_start()
{
  m_is_on_power_find_perform = true;
  m_power_index = 1;
  m_ether_power_time_list.clear();
  //m_ether_power_time_list.push_back(0);
  m_line_power_time_list.clear();
  //m_line_power_time_list.push_back(0);
}

void gnrw_t::on_power_find_tick()
{
  if (!connected()) {
    m_is_on_power_finded = false;
  }
  if (m_is_on_power_find_perform) {
    if ((m_power_index == 1) || (m_wait_power_change_timer.check())) {
      if (m_power_index > 1) {
        m_ether_power_time_list.push_back(
          m_network_variables.ether_work_time_for_current_level);
        m_line_power_time_list.push_back(
          m_network_variables.line_work_time_for_current_level);
      }
      if (m_power_index <= m_power_max) {
        set_ether_power(m_power_index);
        set_line_power(m_power_index);
        m_power_index++;
        m_wait_power_change_timer.start();
      } else {
        typedef vector<irs_u32>::reverse_iterator it_t;
        it_t ether_it = max_element(m_ether_power_time_list.rbegin(),
          m_ether_power_time_list.rend());
        m_ether_power_on = m_ether_power_time_list.rend() - ether_it;
        it_t line_it = max_element(m_line_power_time_list.rbegin(),
          m_line_power_time_list.rend());
        m_line_power_on = m_line_power_time_list.rend() - line_it;
        m_network_variables.ether_power = m_ether_power_on;
        m_network_variables.line_power = m_line_power_on;
        m_is_on_power_finded = true;

#ifdef NOP
        QMessageBox msgBox;
        msgBox.setWindowTitle(irs::str_conv<QString>(std::wstring(L"Покров")));
        QString text;
        text += irs::str_conv<QString>(std::wstring(L"Поле: "));
        for (size_t i = 0; i < m_ether_power_time_list.size(); i++) {
          text += irs::irsstr_from_number_current(char(),
            m_ether_power_time_list[i]*0.1).c_str();
          text += " ";
        }
        text += "\n";
        text += irs::str_conv<QString>(std::wstring(L"Сеть: "));
        for (size_t i = 0; i < m_line_power_time_list.size(); i++) {
          text += irs::irsstr_from_number_current(char(),
            m_line_power_time_list[i]*0.1).c_str();
          text += " ";
        }
        text += "\n";
        msgBox.setText(text);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Information);
        msgBox.exec();
#endif //NOP
      }
    }
  }
}
#endif //GNRWPC_POWER_ONOFF_FSTEC_OLD

void gnrw_t::tick()
{
  #ifdef GNRWPC_POWER_ONOFF_FSTEC_OLD
  on_power_find_tick();
  #endif //GNRWPC_POWER_ONOFF_FSTEC_OLD

  if (m_apply_ip_timer.check()) {
    if (connected()) {
      m_network_variables.apply_ip_mask_bit = 1;
      m_wait_apply_ip_timer.start();
    }
  }
}

void gnrw_t::network_variables_t::connect(irs::mxdata_t* ap_data)
{
  apply_ip_mask_bit.connect(ap_data, 0, 0);
  wdt_test_bit.connect(ap_data, 0, 1);
  //boost_bit.connect(ap_data, 0, 2);
  on_bit.connect(ap_data, 0, 4);
  fsb_bit.connect(ap_data, 0, 5);
  fsb_auto_bit.connect(ap_data, 0, 6);
  dhcp_bit.connect(ap_data, 0, 7);

  fail_bit.connect(ap_data, 2, 0);
  work1_bit.connect(ap_data, 2, 1);
  work2_bit.connect(ap_data, 2, 2);
  work3_bit.connect(ap_data, 2, 3);
  work4_bit.connect(ap_data, 2, 4);
  work_radio_bit.connect(ap_data, 2, 5);
  work_line_bit.connect(ap_data, 2, 6);

  work_time.connect(ap_data, 4);
  ether_power.connect(ap_data, 8);
  line_power.connect(ap_data, 9);

  id.connect(ap_data, 10);

  volume.connect(ap_data, 14);
  show_power_interval.connect(ap_data, 15);
  show_time_interval.connect(ap_data, 16);

  ip1.connect(ap_data, 17);
  ip2.connect(ap_data, 18);
  ip3.connect(ap_data, 19);
  ip4.connect(ap_data, 20);
  mask1.connect(ap_data, 21);
  mask2.connect(ap_data, 22);
  mask3.connect(ap_data, 23);
  mask4.connect(ap_data, 24);

  ether_work_time_for_current_level.connect(ap_data, 25);
  line_work_time_for_current_level.connect(ap_data, 29);

  bright.connect(ap_data, 33);
}

// class gnrw_link_t
irs::handle_t<irs::mxdata_t> gnrw_link_t::make_client(
  irs::handle_t<irs::hardflow_t> ap_hardflow,
  const modbus_settings_t& a_modbus_settings)
{
  return new irs::modbus_client_t(
    ap_hardflow.get(),
    irs::modbus_client_t::mode_refresh_auto,
    a_modbus_settings.discret_inputs,
    a_modbus_settings.coils,
    a_modbus_settings.holding_registers,
    a_modbus_settings.input_registers,
    irs::make_cnt_s(a_modbus_settings.update_period)
  );
}
irs::handle_t<irs::hardflow_t> gnrw_link_t::make_hardflow()
{
  irs::handle_t<irs::hardflow_t> hardflow_ret = IRS_NULL;

  hardflow_ret = irs::hardflow::make_udp_flow_client(
    m_settings.ip, m_settings.port);

  return hardflow_ret;
}

gnrw_link_t::gnrw_link_t(const gnrw_settings_t& a_gnrw_settings):
  m_settings(a_gnrw_settings),
  mp_tstlan4(NULL),
  mp_gnrw(NULL),
  m_enabled(false),
  mp_modbus_client_hardflow(NULL),
  mp_modbus_client(NULL),
  m_activated(false),
  m_activation_timer(irs::make_cnt_s(1))
{
  //mp_tstlan4->ini_name(m_conf_file_name);
}
gnrw_link_t::~gnrw_link_t()
{
}

void gnrw_link_t::add_error(const string_type& a_error)
{
  stringstream s;
  s << irs::sdatetime;
  string_type msg = irs::str_conv<string_type>(s.str()) + a_error;
  m_error_list.push_back(msg);
  if (m_error_list.size() > error_list_max_size) {
    m_error_list.pop_front();
  }
}

bool gnrw_link_t::enabled() const
{
  return m_enabled;
}
void gnrw_link_t::enabled(bool a_enabled)
{
  if (a_enabled == m_enabled) {
    return;
  }
  if (a_enabled) {
    try_create_modbus();
  } else {
    destroy_modbus();
  }
  m_enabled = a_enabled;
}

void gnrw_link_t::try_create_modbus()
{
  try {
    create_modbus();
  } catch (std::runtime_error& e) {
    const string_type s = irs::str_conv<string_type>(string(e.what()));
    add_error(s);
    destroy_modbus();
  } catch (...) {
    add_error(irst("Неизвестная ошибка"));
    destroy_modbus();
  }
}

void gnrw_link_t::create_modbus()
{
  mp_modbus_client_hardflow = make_hardflow();
  mp_modbus_client = make_client(mp_modbus_client_hardflow,
    m_settings.modbus_settings);
  //mp_tstlan4->connect(mp_modbus_client.get());
  mp_gnrw->connect(mp_modbus_client.get());
  m_activated = true;
}

void gnrw_link_t::destroy_modbus()
{
  //mp_tstlan4->connect(NULL);
  mp_gnrw->connect(NULL);
  mp_modbus_client.reset();
  mp_modbus_client_hardflow.reset();
  m_activated = false;
}

irs::mxdata_t* gnrw_link_t::mxdata()
{
  return mp_modbus_client.get();
}
void gnrw_link_t::tick()
{
  if (!mp_modbus_client.is_empty()) {
    mp_modbus_client->tick();
    const string_type error_string =
      mp_modbus_client_hardflow->param(irst("error_string"));
    if (!error_string.empty()) {
      add_error(error_string);
      destroy_modbus();
    }
  }
  if (m_enabled && !m_activated && m_activation_timer.check()) {
    try_create_modbus();
  }
}

void gnrw_link_t::set_settings(const gnrw_settings_t& a_settings)
{
  m_settings = a_settings;
}

const gnrw_settings_t& gnrw_link_t::get_settings() const
{
  return m_settings;
}

void gnrw_link_t::show_options()
{
}

void gnrw_link_t::tstlan4(irs::tstlan4_base_t* ap_tstlan4)
{
  mp_tstlan4 = ap_tstlan4;
}

void gnrw_link_t::gnrw(gnrw_t* ap_gnrw)
{
  mp_gnrw = ap_gnrw;
}

gnrw_link_t::status_t gnrw_link_t::get_status()
{
  if (m_activated) {
    if (mp_modbus_client->connected()) {
      return status_connected;
    }
  }
  if (m_enabled) {
    return status_busy;
  }
  return status_disabled;
}

gnrw_link_t::error_string_list_type
gnrw_link_t::get_last_error_string_list()
{
  error_string_list_type error_list = m_error_list;
  m_error_list.clear();
  return error_list;
}
