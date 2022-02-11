#ifndef INC_MCP8024_DEF_H_
#define INC_MCP8024_DEF_H_

#define MCP8024_GET_DAC_CURRENT_REFERENCE_VALUE(ref)	(((ref)-0.991f)/0.01377f)

typedef enum {
	MCP8024_CMD_SET_CFG_0 = 0x81,
	MCP8024_CMD_GET_CFG_0 = 0x82,
	MCP8024_CMD_STATUS_0 = 0x85,
	MCP8024_CMD_STATUS_1 = 0x86,
	MCP8024_CMD_SET_CFG_1 = 0x83,
	MCP8024_CMD_GET_CFG_1 = 0x84,
	MCP8024_CMD_SET_CFG_2 = 0x87,
	MCP8024_CMD_GET_CFG_2 = 0x88,
} MCP8024_Command_t;

typedef enum {
	MCP8024_CONFIG_DISCONNECTION_OF_30K_LEVEL_TRANSLATOR_PULLUP_WHEN_CE_0_DISABLE = 0x00,
	MCP8024_CONFIG_DISCONNECTION_OF_30K_LEVEL_TRANSLATOR_PULLUP_WHEN_CE_0_ENABLE = 0x01
} MCP8024_30K_Pullup_Disconnection_t;

typedef enum {
	MCP8024_CONFIG_UNDERVOLTAGE_LOCKOUT_ENABLED = 0x00,
	MCP8024_CONFIG_UNDERVOLTAGE_LOCKOUT_DISABLED = 0x01
} MCP8024_Undervoltage_Lockout_t;

typedef enum {
	MCP8024_CONFIG_EXT_MOSFET_OVERCURRENT_DETECTION_ENABLED = 0x00,
	MCP8024_CONFIG_EXT_MOSFET_OVERCURRENT_DETECTION_DISABLED = 0x01
} MCP8024_External_MOSFET_Overcurrent_Detection_t;

typedef enum {
	MCP8024_CONFIG_EXT_MOSFET_OVERCURRENT_LIMIT_0_250_V = 0x00,
	MCP8024_CONFIG_EXT_MOSFET_OVERCURRENT_LIMIT_0_500_V = 0x01,
	MCP8024_CONFIG_EXT_MOSFET_OVERCURRENT_LIMIT_0_750_V = 0x02,
	MCP8024_CONFIG_EXT_MOSFET_OVERCURRENT_LIMIT_1_000_V = 0x03
} MCP8024_External_MOSFET_Overcurrent_Limit_t;

typedef enum {
	MCP8024_CONFIG_DRIVER_DEAD_TIME_2_US = 0x00,
	MCP8024_CONFIG_DRIVER_DEAD_TIME_1_US = 0x01,
	MCP8024_CONFIG_DRIVER_DEAD_TIME_500_NS = 0x02,
	MCP8024_CONFIG_DRIVER_DEAD_TIME_250_NS = 0x03,
} MCP8024_Driver_Dead_Time_t;

typedef enum {
	MCP8024_CONFIG_DRIVER_BLANKING_TIME_4_US = 0x00,
	MCP8024_CONFIG_DRIVER_BLANKING_TIME_2_US = 0x01,
	MCP8024_CONFIG_DRIVER_BLANKING_TIME_1_US = 0x02,
	MCP8024_CONFIG_DRIVER_BLANKING_TIME_500_NS = 0x03,
} MCP8024_Driver_Blanking_Time_t;

typedef struct {
	uint8_t cmd;
	uint8_t data;
} MCP8024_Message_t;

typedef struct {
	MCP8024_External_MOSFET_Overcurrent_Limit_t ext_mosfet_overcurrent_limit : 2;
	MCP8024_External_MOSFET_Overcurrent_Detection_t ext_mosfet_overcurrent_detection : 1;
	MCP8024_Undervoltage_Lockout_t undervoltage_lockout : 1;
	uint8_t reserved : 1;
	uint8_t unused1 : 1;
	MCP8024_30K_Pullup_Disconnection_t disable_30k_pullup : 1;
	uint8_t unused0 : 1;
} MCP8024_Config_0_t;

typedef struct {
	uint8_t dac_current_reference : 8;
} MCP8024_Config_1_t;

typedef struct {
	MCP8024_Driver_Blanking_Time_t driver_blanking_time : 2;
	MCP8024_Driver_Dead_Time_t driver_dead_time : 2;
	uint8_t unused : 4;
} MCP8024_Config_2_t;

typedef struct {uint8_t temperature_warning : 1;
	uint8_t over_temperature : 1;
	uint8_t input_undervoltage : 1;
	uint8_t reserved : 1;
	uint8_t input_overvoltage : 1;
	uint8_t buck_regulator_overcurrent : 1;
	uint8_t buck_regulator_output_undervoltage_warning : 1;
	uint8_t buck_regulator_output_undervoltage : 1;
} MCP8024_Status_0_t;

typedef struct {
	uint8_t ldo_5v_overcurrent : 1;
	uint8_t ldo_12v_overcurrent : 1;
	uint8_t ext_mosfet_undervoltage_lockout : 1;
	uint8_t ext_mosfet_overcurrent_detection : 1;
	uint8_t brown_out_reset_config_lost : 1;
	uint8_t not_used : 3;
} MCP8024_Status_1_t;

#endif
