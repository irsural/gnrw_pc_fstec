#include "gnrw.h"

#include <irsfinal.h>

// class gnrw_t
gnrw_t::gnrw_t():
  mp_data(NULL),
  m_network_variables(),
  m_sync_timer(irs::make_cnt_s(2)),
  m_apply_ip_timer(irs::make_cnt_s(2)),
  m_wait_apply_ip_timer(irs::make_cnt_s(2))
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
    return m_network_variables.boost_bit;
  }
  return false;
}

void gnrw_t::set_boost(bool a_enable)
{
  if (!connected()) {
     return;
  }
  m_network_variables.boost_bit = a_enable;
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
  const string_type& a_mask)
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

bool gnrw_t::ip_change_success_check()
{
  return m_wait_apply_ip_timer.check();
}

void gnrw_t::reset()
{
  m_network_variables.boost_bit = 1;
  m_network_variables.ether_power = 9;
  m_network_variables.line_power = 9;
  m_network_variables.volume = 1;
  m_network_variables.show_power_interval = 1;
  m_network_variables.show_time_interval = 5;
  m_network_variables.bright = 2;

  m_sync_timer.start();
}

bool gnrw_t::synchronizes()
{
  m_sync_timer.check();
  return !m_sync_timer.stopped();
}

void gnrw_t::tick()
{
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
  boost_bit.connect(ap_data, 0, 2);
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
