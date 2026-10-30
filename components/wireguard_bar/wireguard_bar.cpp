#include "wireguard_bar.h"
#include "esphome/components/wifi/wifi_component.h"
#include <algorithm>
#include <string>

namespace esphome::wireguard_bar {

static constexpr uint32_t SHIELD_LOCK = 0xF099D;  // shield-lock: the tunnel is up
static constexpr uint32_t SHIELD_OFF = 0xF099E;   // shield-off: no handshake with the peer
// wifi: the one Wi-Fi glyph of the tile icons, which a plugin's item draws with (the core's own Wi-Fi bars are in a font
// of the top bar's that plugin items do not use); the colour says how good the signal is.
static constexpr uint32_t WIFI = 0xF05A9;

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

// The Wi-Fi signal, coloured: green from -70 dBm, amber down to -78, red below (where the
// core calls a signal weak) and while the screen holds no network.
tessera::BarItem WireguardBar::signal() const {
  tessera::BarItem item;
  auto *wifi = wifi::global_wifi_component;
  if (!wifi) return item;
  const int rssi = wifi->is_connected() ? wifi->wifi_rssi() : 0;
  item.shown = true;
  if (rssi >= 0) {  // no network, or no reading (ESPHome says 0 or 31 then)
    item.icon = WIFI;
    item.tone = tessera::Tone::ALERT;
    return item;
  }
  const int bars = rssi >= -60 ? 4 : rssi >= -70 ? 3 : rssi >= -78 ? 2 : 1;
  item.icon = WIFI;
  item.tone = bars >= 3 ? tessera::Tone::GOOD : bars == 2 ? tessera::Tone::BUSY : tessera::Tone::ALERT;
  item.text = std::to_string(std::clamp(2 * (rssi + 100), 0, 100)) + " %";
  return item;
}

}  // namespace esphome::wireguard_bar
