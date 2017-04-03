#ifndef SETTINGS_H
#define SETTINGS_H

#include <irsdefs.h>

#include <QSettings>

#include "gnrw.h"

#include <irsfinal.h>

class settings_t
{
public:
  typedef std::size_t size_type;
  typedef irs::string_t string_type;
  static settings_t* instance();
  void save();
  void load();
  gnrw_settings_t gnrw_settings;
private:
  settings_t();
  const QString m_file_name;
  static settings_t* mp_instance;
};
#endif // SETTINGS_H
