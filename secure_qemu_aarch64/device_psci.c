/*
 * Copyright 2022 The Hafnium Authors.
 *
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file or at
 * https://opensource.org/licenses/BSD-3-Clause.
 */

#include "hf/arch/vm/power_mgmt.h"

#include "hf/io.h"

#include "test/hftest.h"

noreturn void hftest_device_reboot(void)
{
	arch_reboot();
}

void hftest_device_exit_test_environment(void)
{
	/*
	 * PL061 is qemu platform specific.
	 * SECURE_GPIO_BASE 0x090b0000
	 * gpio_set_direction(SECURE_GPIO_POWEROFF, GPIO_DIR_OUT);
	 * gpio_set_value(SECURE_GPIO_POWEROFF, GPIO_LEVEL_LOW);
	 * gpio_set_value(SECURE_GPIO_POWEROFF, GPIO_LEVEL_HIGH);
	 */
	io_write8(io8_c((uintpaddr_t)0x090b0000, 0), 0);
	io_write8(io8_c((uintpaddr_t)0x090b0000, 2), 2);
}
