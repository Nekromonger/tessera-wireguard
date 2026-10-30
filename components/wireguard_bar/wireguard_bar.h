#pragma once
#include "esphome/core/component.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/components/smart_display/plugin_api.h"

namespace esphome::wireguard_bar {

// The plugin: an ESPHome component and a tessera::Plugin. It registers its top bar items in setup().
class WireguardBar : public Component, public tessera::Plugin {
 public:
  void setup() override;
  float get_setup_priority() const override { return setup_priority::DATA; }
  void set_status(binary_sensor::BinarySensor *status) { status_ = status; }

 private:
  tessera::BarItem bar() const;
  tessera::BarItem signal() const;
  binary_sensor::BinarySensor *status_{};
};

}  // namespace esphome::wireguard_bar
