#ifndef GNRV_H
#define GNRV_H

#include <irsdefs.h>

#include <hardflowg.h>
#include <irsmbus.h>
#include <irsdevices.h>

#include <irsfinal.h>

struct modbus_settings_t
{
  typedef irs_size_t size_type;
  size_type discret_inputs;
  size_type coils;
  size_type holding_registers;
  size_type input_registers;
  double update_period;
  modbus_settings_t():
    discret_inputs(0),
    coils(0),
    holding_registers(21),
    input_registers(0),
    update_period(0.1)
  {
  }
  bool operator==(const modbus_settings_t& a_ms) const
  {
    return (discret_inputs == a_ms.discret_inputs) &&
      (discret_inputs == a_ms.discret_inputs) &&
      (coils == a_ms.coils) &&
      (holding_registers == a_ms.holding_registers) &&
      (input_registers == a_ms.input_registers) &&
      (update_period == a_ms.update_period);
  }
};

struct gnrw_settings_t
{
  typedef irs_size_t size_type;
  typedef irs::string_t string_type;
  string_type ip;
  string_type port;
  modbus_settings_t modbus_settings;
  gnrw_settings_t():
    ip(irst("192.168.0.208")),
    port(irst("5006")),
    modbus_settings()
  {
  }
  bool operator==(const gnrw_settings_t& a_gnrw_settings) const
  {
    return (ip == a_gnrw_settings.ip) &&
      (port == a_gnrw_settings.port) &&
      (modbus_settings == a_gnrw_settings.modbus_settings);
  }

  bool operator!=(const gnrw_settings_t& a_gnrw_settings) const
  {
    return !operator==(a_gnrw_settings);
  }
};

class gnrw_t
{
public:
  typedef irs::string_t string_type;
  gnrw_t();
  void connect(irs::mxdata_t* ap_data);
  bool connected() const;
  bool get_boost() const;
  void set_boost(bool);
  bool get_error_status() const;
  bool get_detector_1_status() const;
  bool get_detector_2_status() const;
  bool get_detector_3_status() const;
  bool get_detector_4_status() const;
  bool get_work_radio_status() const;
  bool get_work_line_status() const;
  size_t get_id() const;
  double get_work_time() const;
  double get_ether_work_time_for_current_level() const;
  double get_line_work_time_for_current_level() const;
  size_t get_ether_power() const;
  void set_ether_power(size_t a_level);
  size_t get_line_power() const;
  void set_line_power(size_t a_level);
  void get_network_address(string_type* ap_ip, string_type* ap_mask);
  bool set_network_address(const string_type& a_ip, const string_type& a_mask,
    bool a_dhcp_enable);
  size_t get_volume() const;
  void set_volume(size_t a_level);
  size_t get_brightness() const;
  void set_brightness(size_t a_value);
  size_t get_show_power_interval() const;
  void set_show_power_interval(size_t a_time);
  size_t get_show_time_interva() const;
  void set_show_time_interval(size_t a_time);
  void on(bool a_on);
  bool on();

  bool ip_change_success_check();
  //generator_status_t get_status();
  void reset();
  bool synchronizes();
  void tick();
private:
  struct network_variables_t
  {
    void connect(irs::mxdata_t* ap_data);
    irs::bit_data_t apply_ip_mask_bit;
    irs::bit_data_t wdt_test_bit;
    //irs::bit_data_t boost_bit;
    irs::bit_data_t on_bit;
    irs::bit_data_t fsb_bit;
    irs::bit_data_t fsb_auto_bit;
    irs::bit_data_t dhcp_bit;
    irs::bit_data_t fail_bit;
    irs::bit_data_t work1_bit;
    irs::bit_data_t work2_bit;
    irs::bit_data_t work3_bit;
    irs::bit_data_t work4_bit;
    irs::bit_data_t work_radio_bit;
    irs::bit_data_t work_line_bit;
    irs::conn_data_t<irs_u32> work_time;
    irs::conn_data_t<irs_u8> ether_power;
    irs::conn_data_t<irs_u8> line_power;

    irs::conn_data_t<irs_u32> id;

    irs::conn_data_t<irs_u8> volume;
    irs::conn_data_t<irs_u8> show_power_interval;
    irs::conn_data_t<irs_u8> show_time_interval;

    irs::conn_data_t<irs_u8> ip1;
    irs::conn_data_t<irs_u8> ip2;
    irs::conn_data_t<irs_u8> ip3;
    irs::conn_data_t<irs_u8> ip4;
    irs::conn_data_t<irs_u8> mask1;
    irs::conn_data_t<irs_u8> mask2;
    irs::conn_data_t<irs_u8> mask3;
    irs::conn_data_t<irs_u8> mask4;

    irs::conn_data_t<irs_u32> ether_work_time_for_current_level;
    irs::conn_data_t<irs_u32> line_work_time_for_current_level;

    irs::conn_data_t<irs_u8> bright;
  };

  static const size_t m_power_max = 9;

  irs::mxdata_t* mp_data;
  network_variables_t m_network_variables;
  irs::timer_t m_sync_timer;
  irs::timer_t m_apply_ip_timer;
  irs::timer_t m_wait_apply_ip_timer;
  #ifdef GNRWPC_POWER_ONOFF_FSTEC_OLD
  size_t m_power_index;
  irs::timer_t m_wait_power_change_timer;
  size_t m_ether_power_on;
  size_t m_line_power_on;
  vector<irs_u32> m_ether_power_time_list;
  vector<irs_u32> m_line_power_time_list;
  bool m_is_on_power_finded;
  bool m_is_on_power_find_perform;
  #endif //GNRWPC_POWER_ONOFF_FSTEC_OLD

  #ifdef GNRWPC_POWER_ONOFF_FSTEC_OLD
  void on_power_find_start();
  void on_power_find_tick();
  #endif //GNRWPC_POWER_ONOFF_FSTEC_OLD
};

class gnrw_link_t: public irs::mxdata_assembly_t
{
public:
  typedef irs_size_t size_type;
  typedef irs::string_t string_type;

  gnrw_link_t(const gnrw_settings_t& a_gnrw_settings);
  virtual ~gnrw_link_t();
  virtual bool enabled() const;
  virtual void enabled(bool a_enabled);
  virtual irs::mxdata_t* mxdata();
  virtual void tick();
  void set_settings(const gnrw_settings_t& a_settings);
  const gnrw_settings_t& get_settings() const;
  virtual void show_options();
  virtual void tstlan4(irs::tstlan4_base_t* ap_tstlan4);
  void gnrw(gnrw_t* ap_gnrw);
  virtual status_t get_status();
  virtual error_string_list_type get_last_error_string_list();
private:
  void add_error(const string_type& a_error);
  gnrw_settings_t m_settings;
  //param_box_tune_t m_param_box_tune;
  irs::tstlan4_base_t* mp_tstlan4;
  gnrw_t* mp_gnrw;
  bool m_enabled;
  irs::handle_t<irs::hardflow_t> mp_modbus_client_hardflow;
  irs::handle_t<irs::mxdata_t> mp_modbus_client;
  bool m_activated;
  irs::loop_timer_t m_activation_timer;
  enum { error_list_max_size = 100 };
  error_string_list_type m_error_list;

  static irs::handle_t<irs::mxdata_t> make_client(
    irs::handle_t<irs::hardflow_t> ap_hardflow,
    const modbus_settings_t& a_modbus_settings);

  irs::handle_t<irs::hardflow_t> make_hardflow();
  void try_create_modbus();
  void create_modbus();
  void destroy_modbus();
};

#endif // GNRV_H
