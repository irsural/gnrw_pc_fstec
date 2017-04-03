#include "settings.h"

#include <irsfinal.h>

settings_t* settings_t::mp_instance = 0;

settings_t::settings_t():
  gnrw_settings(),
  m_file_name("gnrwsettings.ini")
{
  mp_instance = this;
}

settings_t* settings_t::instance()
{
  return mp_instance ? mp_instance : new settings_t();
}

void settings_t::save()
{
  QSettings settings(m_file_name, QSettings::IniFormat);
  settings.beginGroup("connection");
  settings.setValue("ip",
    irs::str_conv<QString>(mp_instance->gnrw_settings.ip));
  settings.setValue("port",
    irs::str_conv<QString>(mp_instance->gnrw_settings.port));
  settings.endGroup();
}

void settings_t::load()
{
  QSettings settings(m_file_name, QSettings::IniFormat);
  settings.beginGroup("connection");
  mp_instance->gnrw_settings.ip = irs::str_conv<string_type>(
    settings.value("ip", "192.168.0.208").toString());
  mp_instance->gnrw_settings.port = irs::str_conv<string_type>(
    settings.value("port", "5006").toString());
  settings.endGroup();
}
