/*
 * Copyright 2023 The Hafnium Authors.
 *
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file or at
 * https://opensource.org/licenses/BSD-3-Clause.
 */

#include "hf/arch/vm/power_mgmt.h"

#include "hf/io.h"
#include "hf/mm.h"

#include "test/hftest.h"

#define QEMU_SECURE_GPIO_BASE ((uintpaddr_t)0x090b0000)
#define QEMU_SECURE_GPIO_DIR_OFF ((uintpaddr_t)0x400)
#define QEMU_SECURE_GPIO_PWR_OFF UINT32_C(0)

noreturn void hftest_device_reboot(void)
{
	arch_reboot();
}

void hftest_device_exit_test_environment(void)
{
	/* Map the secure PL061 GPIO controller. */
	hftest_mm_identity_map((void *)QEMU_SECURE_GPIO_BASE, PAGE_SIZE,
			       MM_MODE_R | MM_MODE_W | MM_MODE_D);

	/*
	 * Refer to [1] for the qemu system off sequence:
	 *
	 * [1]
	 * https://git.trustedfirmware.org/TF-A/trusted-firmware-a.git/tree/plat/qemu/common/qemu_pm.c#n210
	 */

	/* Set power off gpio direction to out. */
	io_write8(io8_c(QEMU_SECURE_GPIO_BASE, QEMU_SECURE_GPIO_DIR_OFF), 1);

	/* Set power off gpio output value to low. */
	io_write8(io8_c(QEMU_SECURE_GPIO_BASE,
			(1ULL << (QEMU_SECURE_GPIO_PWR_OFF + 2))),
		  0);

	/*
	 * Set power off gpio output value to high.
	 * This triggers a system off event.
	 */
	io_write8(io8_c(QEMU_SECURE_GPIO_BASE,
			(1ULL << (QEMU_SECURE_GPIO_PWR_OFF + 2))),
		  (1ULL << QEMU_SECURE_GPIO_PWR_OFF));
}
