# WireGuard status

> The screen needs a WireGuard tunnel of its own first: a `wireguard:` block in its YAML (for a Tessera screen, in
> `<screen>.local.yaml`). Without it the build stops.

A shield in the top bar of the screen: green with "VPN" while the screen's WireGuard tunnel to its peer is up, red with
"VPN off" while it is down. Handy for a screen that reaches Home Assistant from another house through a tunnel.

## Set up

1. Give the screen its tunnel: a `wireguard:` block in `<screen>.local.yaml` in the ESPHome folder, with an `sntp` time
   source (the tunnel needs the time before Home Assistant is reachable). Build the screen once and check that it runs.
2. In Tessera, open **Plugins**, choose **Add with a link**, paste `https://github.com/Nekromonger/tessera-wireguard`,
   tick the screen and let it build.
3. In **Layout**, open the page's top bar, choose **Add**, **From plugins**, and add **WireGuard tunnel**.
4. Optional: add **Wi-Fi signal in colour** the same way, for the screen's Wi-Fi in green, amber or red.

## Good to know

- The screen knows itself whether its tunnel is up (the latest handshake with the peer); Home Assistant is not asked.
- Home Assistant gets the same as a binary sensor, "WireGuard status", for an automation that tells you when the screen
  loses its tunnel.
- The Wi-Fi item is green from -70 dBm (three or four bars), amber down to -78 dBm (two bars) and red below that or
  without a network. Choose "Icon only" in the editor to leave out the percentage.
- Works on every board that has a WireGuard tunnel.
