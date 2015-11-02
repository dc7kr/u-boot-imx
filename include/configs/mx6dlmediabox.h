#ifndef _MX6DLMEDIABOX_CONFIG_H
#define _MX6DLMEDIABOX_CONFIG_H

#define CONFIG_MX6

/* uncomment for PLUGIN mode support */
/* #define CONFIG_USE_PLUGIN */

/* uncomment for SECURE mode support */
/* #define CONFIG_SECURE_BOOT */

#include "mx6_common.h"
#include <linux/sizes.h>

#define CONFIG_SYS_GENERIC_BOARD
#define CONFIG_DISPLAY_CPUINFO
#define CONFIG_DISPLAY_BOARDINFO

#include <asm/arch/imx-regs.h>
#include <asm/imx-common/gpio.h>

#define CONFIG_CMDLINE_TAG
#define CONFIG_SETUP_MEMORY_TAGS
#define CONFIG_INITRD_TAG
#define CONFIG_REVISION_TAG
#define CONFIG_SERIAL_TAG

/* Size of malloc() pool */
#define CONFIG_SYS_MALLOC_LEN		(10 * SZ_1M)

#define CONFIG_BOARD_EARLY_INIT_F
#define CONFIG_BOARD_LATE_INIT
#define CONFIG_MXC_GPIO

#define CONFIG_MXC_UART

#define CONFIG_CMD_FUSE
#ifdef CONFIG_CMD_FUSE
#define CONFIG_MXC_OCOTP
#endif

/* MMC Configs */
#define CONFIG_BOUNCE_BUFFER
#define CONFIG_CMD_EXT2
#define CONFIG_CMD_FAT
#define CONFIG_CMD_MMC
#define CONFIG_DOS_PARTITION
#define CONFIG_FSL_ESDHC
#define CONFIG_FSL_USDHC
#define CONFIG_GENERIC_MMC
#define CONFIG_MMC
//#define CONFIG_SUPPORT_EMMC_BOOT /* eMMC specific */
#define CONFIG_SYS_FSL_ESDHC_ADDR		0
//#define CONFIG_SYS_MMC_ENV_DEV			0 /* SDHC3 */
//#define CONFIG_SYS_MMC_ENV_PART			2 /* user partition */

/* allow to overwrite serial and ethaddr */
#define CONFIG_ENV_OVERWRITE
#define CONFIG_CONS_INDEX              1
#define CONFIG_BAUDRATE                        115200

/* Command definition */
#include <config_cmd_default.h>

#define CONFIG_CMD_BMODE
#define CONFIG_CMD_BOOTZ
#define CONFIG_CMD_SETEXPR
#undef CONFIG_CMD_IMLS

#define CONFIG_BOOTDELAY               0
#define CONFIG_ZERO_BOOTDELAY_CHECK

#define CONFIG_LOADADDR                        0x14008000
#define CONFIG_SYS_TEXT_BASE           0x17800000

//#define CONFIG_PREBOOT "run loadbootenv"
#define CONFIG_EXTRA_ENV_SETTINGS \
	"bootcmd_android=run env_load; " \
		"booti mmc${boot_dev}\0" \
	"bootcmd_android_recovery=run env_load; " \
		"booti mmc${boot_dev} recovery\0" \
	"bootcmd_linux=run env_load; " \
		"run linux_load; " \
		"bootz ${loadaddr} - ${fdt_addr}\0" \
	"bootcmd_mfg=bootz ${loadaddr} ${initrd_addr} ${fdt_addr}\0" \
	"env_load=if ext2load mmc ${env_dev}:${env_part} ${loadaddr} ${env_path}; then " \
		"env import -t ${loadaddr} ${filesize}; fi\0" \
	"env_path=/etc/uEnv.txt\0" \
	"fdt_addr=0x14f00000\0" \
	"fdt_high=0xffffffff\0" \
	"initrd_addr=0x20000000\0" \
	"initrd_high=0xffffffff\0" \
	"linux_fdt=/boot/imx6dl-mediabox.dtb\0" \
	"linux_kernel=/boot/zImage\0" \
	"linux_load=ext2load mmc ${boot_dev}:${boot_part} ${loadaddr} ${linux_kernel}; " \
		"ext2load mmc ${boot_dev}:${boot_part} ${fdt_addr} ${linux_fdt}\0"
#define CONFIG_BOOTCOMMAND

/* Miscellaneous configurable options */
#define CONFIG_SYS_LONGHELP
#define CONFIG_SYS_HUSH_PARSER
#define CONFIG_SYS_PROMPT_HUSH_PS2     "> "
#define CONFIG_AUTO_COMPLETE
#define CONFIG_SYS_CBSIZE              1024

/* Print Buffer Size */
#define CONFIG_SYS_PBSIZE (CONFIG_SYS_CBSIZE + sizeof(CONFIG_SYS_PROMPT) + 16)
#define CONFIG_SYS_MAXARGS             16
#define CONFIG_SYS_BARGSIZE CONFIG_SYS_CBSIZE

#define CONFIG_CMD_MEMTEST
#define CONFIG_SYS_MEMTEST_START       0x10000000
#define CONFIG_SYS_MEMTEST_END         0x10010000
#define CONFIG_SYS_MEMTEST_SCRATCH     0x10800000

#define CONFIG_SYS_LOAD_ADDR           CONFIG_LOADADDR

#define CONFIG_CMDLINE_EDITING
#define CONFIG_STACKSIZE               (128 * 1024)

/* Physical Memory Map */
#define CONFIG_NR_DRAM_BANKS           1
#define PHYS_SDRAM                     MMDC0_ARB_BASE_ADDR

#define CONFIG_SYS_SDRAM_BASE          PHYS_SDRAM
#define CONFIG_SYS_INIT_RAM_ADDR       IRAM_BASE_ADDR
#define CONFIG_SYS_INIT_RAM_SIZE       IRAM_SIZE

#define CONFIG_SYS_INIT_SP_OFFSET \
	(CONFIG_SYS_INIT_RAM_SIZE - GENERATED_GBL_DATA_SIZE)
#define CONFIG_SYS_INIT_SP_ADDR \
	(CONFIG_SYS_INIT_RAM_ADDR + CONFIG_SYS_INIT_SP_OFFSET)

/* FLASH and environment organization */
#define CONFIG_SYS_NO_FLASH

#define CONFIG_ENV_SIZE			(8 * 1024)

#ifndef CONFIG_SYS_NOSMP
#define CONFIG_SYS_NOSMP
#endif

//#define CONFIG_ENV_IS_IN_MMC
#define CONFIG_ENV_IS_NOWHERE

#define CONFIG_ENV_OFFSET		(8 * 64 * 1024)

#define CONFIG_OF_LIBFDT

#ifndef CONFIG_SYS_DCACHE_OFF
#define CONFIG_CMD_CACHE
#endif

/*
 * I2C configs
 */
/*
#define CONFIG_CMD_I2C
#define CONFIG_SYS_I2C
#define CONFIG_SYS_I2C_MXC
#define CONFIG_SYS_I2C_SPEED		100000
*/

/*
#if defined(CONFIG_ANDROID_SUPPORT)
#include "mx6sabreandroid_common.h"
#endif
*/

#define CONFIG_MACH_TYPE	4412
#ifndef CONFIG_MXC_UART_BASE
#define CONFIG_MXC_UART_BASE		UART2_BASE
#endif

/* USB Configs */
#define CONFIG_CMD_USB
#define CONFIG_EHCI_HCD_INIT_AFTER_RESET
#define CONFIG_IMX_UDC
#define CONFIG_MXC_USB_FLAGS			0
#define CONFIG_MXC_USB_PORTSC			(PORT_PTS_UTMI | PORT_PTS_PTW)
#define CONFIG_USB_DEVICE
#define CONFIG_USB_EHCI
#define CONFIG_USB_EHCI_MX6
#define CONFIG_USB_MAX_CONTROLLER_COUNT		2

#undef CONFIG_LDO_BYPASS_CHECK

/* Android fastboot and recovery features */
#define CONFIG_ANDROID_RECOVERY
#define CONFIG_ANDROID_BOOT_PARTITION_MMC	1
#define CONFIG_ANDROID_RECOVERY_PARTITION_MMC	2
#define CONFIG_ANDROID_SYSTEM_PARTITION_MMC	5
#define CONFIG_CMD_BOOTI
#define CONFIG_FASTBOOT
#define CONFIG_FASTBOOT_BCD_DEVICE		0x311
#define CONFIG_FASTBOOT_CONFIGURATION_STR	"Android fastboot"
#define CONFIG_FASTBOOT_INTERFACE_STR		"Android fastboot"
#define CONFIG_FASTBOOT_MANUFACTURER_STR	"Germaneers"
#define CONFIG_FASTBOOT_PRODUCT_ID		0x0d02
#define CONFIG_FASTBOOT_PRODUCT_NAME_STR	"DIM"
#define CONFIG_FASTBOOT_SERIAL_NUM		"0123456789"
#define CONFIG_FASTBOOT_STORAGE_MMC
#define CONFIG_FASTBOOT_VENDOR_ID		0x18d1
#define CONFIG_FASTBOOT_TRANSFER_BUF		0x2c000000
#define CONFIG_FASTBOOT_TRANSFER_BUF_SIZE	0x19000000

#if defined(CONFIG_GCU)
//#define CONFIG_DIM_TOBYL2
//#define CONFIG_DIM_WIBEAR11N
#define CONFIG_FEC_MXC
#define CONFIG_FEC_MXC_NO_ANEG
#define CONFIG_FEC_MXC_PHYADDR			0
#define CONFIG_FEC_XCV_TYPE			RGMII
#define CONFIG_MII
#define IMX_FEC_BASE				ENET_BASE_ADDR
#elif defined(CONFIG_MEDIABOX)
#define CONFIG_DIM_WIBEAR11N
#endif

#endif /* _MX6DLMEDIABOX_CONFIG_H */
