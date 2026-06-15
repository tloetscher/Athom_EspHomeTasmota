#pragma once
#include "esphome.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/switch/switch.h"

namespace tasmota_api {

class TasmotaAPI : public esphome::Component {
 public:
  void setup() override;

  // setters from Python
  void set_power(esphome::sensor::Sensor *power) { power_ = power; }
  void set_relay(esphome::switch_::Switch *relay) { relay_ = relay; }

  // ✅ NOW FULLY DEFINED
  esphome::sensor::Sensor *power_;
  esphome::switch_::Switch *relay_;
};

}  // namespace tasmota_api