#pragma once

/*
 * Called by the bt_clear behavior to reset a profile's stored base layer to
 * the default (index 0 in base-layers, e.g. WINDOWS_L). Also schedules a
 * settings save if CONFIG_BT_SWITCH_PERSIST_BASE_LAYER is enabled.
 */
void bt_switch_reset_profile_layer(int profile);
