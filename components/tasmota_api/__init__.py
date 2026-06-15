import esphome.codegen as cg
import esphome.config_validation as cv

from esphome.const import CONF_ID
from esphome.components import sensor, switch

AUTO_LOAD = ["web_server_base"]

tasmota_api_ns = cg.esphome_ns.namespace("tasmota_api")

TasmotaAPI = tasmota_api_ns.class_("TasmotaAPI", cg.Component)

CONF_POWER = "power"
CONF_RELAY = "relay"

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(TasmotaAPI),

    cv.Required(CONF_POWER): cv.use_id(sensor.Sensor),
    cv.Required(CONF_RELAY): cv.use_id(switch.Switch),

}).extend(cv.COMPONENT_SCHEMA)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)

    power = await cg.get_variable(config[CONF_POWER])
    relay = await cg.get_variable(config[CONF_RELAY])

    cg.add(var.set_power(power))
    cg.add(var.set_relay(relay))