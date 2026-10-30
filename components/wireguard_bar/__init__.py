"""WireGuard in the top bar: whether the screen's own WireGuard tunnel is up.

The tunnel is the screen's `wireguard:` (in its own YAML); its status is a binary sensor of plugin.yaml, which this
component reads for the top bar's item (wireguard_bar.cpp). register_plugin() takes the id, version and the screen's
texts from the manifest and translations/ beside this folder.
"""
import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import binary_sensor, smart_display
from esphome.const import CONF_ID

DEPENDENCIES = ["smart_display", "wireguard", "wifi"]

CONF_STATUS = "status"

wireguard_bar_ns = cg.esphome_ns.namespace("wireguard_bar")
WireguardBar = wireguard_bar_ns.class_("WireguardBar", cg.Component)

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(WireguardBar),
        cv.Required(CONF_STATUS): cv.use_id(binary_sensor.BinarySensor),
    }
).extend(cv.COMPONENT_SCHEMA)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await smart_display.register_plugin(var, __file__)
    cg.add(var.set_status(await cg.get_variable(config[CONF_STATUS])))
