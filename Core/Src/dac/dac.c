//
// Created by Artyom on 3/17/2025.
//

#include "dac.h"
#include "../cli/cli.h"
#include "../cli/cli_string_literals.h"
#include <stdio.h>
#include <ctype.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

extern UART_HandleTypeDef huart1;
extern DAC_HandleTypeDef hdac;

uint8_t exec_dac(cmd_t * const cmd) {

	const char * const option = cmd->argv[0];

	if (!strcmp(option, "write")) {
		return dac_write(cmd);
	}

	printf("dac: %s", CLI_INVALID_OPTIONS);
	return BSP_INVALID_OPTIONS;
}

uint8_t dac_write(cmd_t * const cmd) {

	if (cmd->argc != 3) {
		printf("dac: write: %s", CLI_INVALID_OPTIONS);
		return EINVAL;
	}

	const uint8_t dac_id = satoi(cmd->argv[1]).val;

	if (dac_id != 1) {
		printf(CLI_DAC_INVALID_ID);
		return EINVAL;
	}

	const double_optional_t res = satof(cmd->argv[2]);

	const double value = res.val;

	if (!res.has_val) {
		printf(CLI_DAC_INVALID_OPTION);
		return EINVAL;
	} else if (value < 0 || value > DAC_MAX_VALUE) {
		printf(CLI_DAC_INVALID_VALUE);
		return EINVAL;
	}

	const uint16_t dac_value = (value * 4095);

	HAL_DAC_SetValue(cmd->bsp->hdacx, DAC_CHANNEL_2, DAC_ALIGN_12B_R, dac_value);
	printf("dac: write: success!\r\n");
	printf("output set to %d\r\n", dac_value);

	return 0;
}

