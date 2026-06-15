#include "tasmota_api.h"

#include "esphome/components/web_server_base/web_server_base.h"

class TasmotaHandler : public esphome::web_server_idf::AsyncWebHandler {
 public:
  tasmota_api::TasmotaAPI *parent;

  TasmotaHandler(tasmota_api::TasmotaAPI *p) : parent(p) {}

  bool canHandle(esphome::web_server_idf::AsyncWebServerRequest *request) const override {
    char url_buf[esphome::web_server_idf::AsyncWebServerRequest::URL_BUF_SIZE];
    auto url = request->url_to(url_buf);
    return url.compare("/cm") == 0;
  }

  void handleRequest(esphome::web_server_idf::AsyncWebServerRequest *request) override {
    if (!request->hasParam("cmnd")) {
      request->send(400, "application/json", "{\"Error\":\"Missing cmnd\"}");
      return;
    }

    auto *param = request->getParam("cmnd");
    std::string cmd = param->value().c_str();

    // ✅ Read current values safely
    float power = parent->power_ ? parent->power_->state : 0.0f;
    bool relay_on = parent->relay_ ? parent->relay_->state : false;

    // ✅ --- STATUS 10 (ENERGY) ---
    if (strcasecmp(cmd.c_str(), "Status 10") == 0) {

      // ESP-IDF time handling
      time_t now = ::time(nullptr);
      struct tm timeinfo;
      localtime_r(&now, &timeinfo);

      char time_str[25];
      strftime(time_str, sizeof(time_str), "%Y-%m-%dT%H:%M:%S", &timeinfo);

      char json[768];

      snprintf(json, sizeof(json),
        "{"
        "\"StatusSNS\":{"
          "\"Time\":\"%s\","
          "\"ENERGY\":{"
            "\"TotalStartTime\":\"1970-01-01T00:00:00\","
            "\"Total\":0.000,"
            "\"Yesterday\":0.000,"
            "\"Today\":0.000,"
            "\"Power\":%.0f,"
            "\"ApparentPower\":%.0f,"
            "\"ReactivePower\":0,"
            "\"Factor\":1.00,"
            "\"Voltage\":230,"
            "\"Current\":%.3f"
          "}"
        "}"
        "}",
        time_str,
        power,
        power,
        power > 0 ? power / 230.0f : 0.0f
      );

      request->send(200, "application/json", json);
    }

    // ✅ --- POWER STATE ---
    else if (strcasecmp(cmd.c_str(), "Power") == 0) {
      request->send(200, "application/json",
        relay_on ? "{\"POWER\":\"ON\"}" : "{\"POWER\":\"OFF\"}");
    }

    // ✅ --- POWER ON ---
    else if (strcasecmp(cmd.c_str(), "Power On") == 0) {
      if (parent->relay_) parent->relay_->turn_on();
      request->send(200, "application/json", "{\"POWER\":\"ON\"}");
    }

    // ✅ --- POWER OFF ---
    else if (strcasecmp(cmd.c_str(), "Power Off") == 0) {
      if (parent->relay_) parent->relay_->turn_off();
      request->send(200, "application/json", "{\"POWER\":\"OFF\"}");
    }

    // ✅ --- STATUS 0 (CRITICAL for SolarManager) ---
    else if (strcasecmp(cmd.c_str(), "Status 0") == 0) {

      char json[512];

      snprintf(json, sizeof(json),
        "{"
        "\"Status\":{\"Module\":0,\"FriendlyName\":[\"Tasmota\"]},"
        "\"StatusSTS\":{\"POWER\":\"%s\"},"
        "\"StatusSNS\":{\"ENERGY\":{\"Power\":%.0f}}"
        "}",
        relay_on ? "ON" : "OFF",
        power
      );

      request->send(200, "application/json", json);
    }

    else {
      request->send(200, "application/json",
        "{\"Error\":\"Unsupported command\"}");
    }
  }

  bool isRequestHandlerTrivial() const override { return false; }
};

namespace tasmota_api {

void TasmotaAPI::setup() {
  esphome::web_server_base::global_web_server_base->add_handler(
      new TasmotaHandler(this));
}

}  // namespace tasmota_api
