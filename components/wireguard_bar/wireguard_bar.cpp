#include "wireguard_bar.h"

namespace esphome::wireguard_bar {

static constexpr uint32_t SHIELD_LOCK = 0xF099D;  // shield-lock: the tunnel is up
static constexpr uint32_t SHIELD_OFF = 0xF099E;   // shield-off: no handshake with the peer

void WireguardBar::setup() {
  add_bar_item("tunnel", [this]() { return bar(); });
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

}  // namespace esphome::wireguard_bar
