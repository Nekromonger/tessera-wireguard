#include "wireguard_bar.h"
#include "esphome/components/wifi/wifi_component.h"
#include <algorithm>
#include <string>

namespace esphome::wireguard_bar {

static constexpr uint32_t SHIELD_LOCK = 0xF099D;  // shield-lock: the tunnel is up
static constexpr uint32_t SHIELD_OFF = 0xF099E;   // shield-off: no handshake with the peer
// wifi-strength-off-outline, then wifi-strength-1 .. 4: the same glyphs as the core's own Wi-Fi item.
static constexpr uint32_t WIFI_GLYPHS[5] = {0xF092E, 0xF091F, 0xF0922, 0xF0925, 0xF0928};

void WireguardBar::setup() {
  add_bar_item("tunnel", [this]() { return bar(); });
  add_bar_item("signal", [this]() { return signal(); });
}

// "VPN" with a green shield while the peer answers, "VPN off" with a red one while it does not.
tessera::BarItem WireguardBar::bar() const {
  tessera::BarItem item;
  if (!status_ || !status_->has_state()) return item;
  const bool up = status_->state;
  item.shown = true;
  item.icon = up ? SHIELD_LOCK : SHIELD_OFF;
  item.text = text(up ? "bar_up" : "bar_down");
  item.tone = up ? tessera::Tone::GOOD : tessera::Tone::ALERT;
  return item;
}

// The Wi-Fi signal in bars as the core draws it, coloured: green from -70 dBm, amber down to -78, red below (where the
// core calls a signal weak) and while the screen holds no network.
tessera::BarItem WireguardBar::signal() const {
  tessera::BarItem item;
  auto *wifi = wifi::global_wifi_component;
  if (!wifi) return item;
  const int rssi = wifi->is_connected() ? wifi->wifi_rssi() : 0;
  item.shown = true;
  if (rssi >= 0) {  // no network, or no reading (ESPHome says 0 or 31 then)
    item.icon = WIFI_GLYPHS[0];
    item.tone = tessera::Tone::ALERT;
    return item;
  }
  const int bars = rssi >= -60 ? 4 : rssi >= -70 ? 3 : rssi >= -78 ? 2 : 1;
  item.icon = WIFI_GLYPHS[bars];
  item.tone = bars >= 3 ? tessera::Tone::GOOD : bars == 2 ? tessera::Tone::BUSY : tessera::Tone::ALERT;
  item.text = std::to_string(std::clamp(2 * (rssi + 100), 0, 100)) + " %";
  return item;
}

}  // namespace esphome::wireguard_bar
