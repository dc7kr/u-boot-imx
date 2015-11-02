#include <common.h>

#include <asm/arch/clock.h>
#include <asm/arch/mx6-pins.h>
#include <asm/imx-common/boot_mode.h>
#include <asm/imx-common/iomux-v3.h>
#include <asm/gpio.h>
#include <fsl_esdhc.h>
#include <netdev.h>
#include <serial.h>

#ifdef CONFIG_FASTBOOT
#include <fastboot.h>
#ifdef CONFIG_ANDROID_RECOVERY
#include <recovery.h>
#endif /* CONFIG_ANDROID_RECOVERY */
#endif /* CONFIG_FASTBOOT */

#define IOMUXC_GPR1_USB_OTG_ID_SEL	13

void bcm89810_reset_en(int en);
void bcm89810_set_pads(void);
void ecspi_init(void);
void tobyl2_reset_en(int en);
void tobyl2_set_pads(void);
void wibear11n_reset(void);

DECLARE_GLOBAL_DATA_PTR;

#ifdef CONFIG_CMD_BMODE
/* For description of these values see IMX6DQRM.pdf page 404 ff. */
const struct boot_mode boot_modes[] = {
        /*
	 * cfg1: uSDHC, eMMC, normal boot, high speed, boot ack enabled, reset
	 *	enabled
	 * cfg2: 8-bit bus, uSDHC-3, boot ROM defaults, boot ack enabled, use
	 *	default pad settings
	 */
	{"emmc",	MAKE_CFGVAL(0x60, 0x10, 0x00, 0x00)},
        /*
	 * cfg1: uSDHC, SD, normal boot, normal/high speed, boot ack enabled, no
	 *	power cycle
	 * cfg2: 1 delay cell, 4-bit bus, uSDHC-4, boot ROM defaults, use
	 *	default pad settings
	 */
	{"sd",		MAKE_CFGVAL(0x50, 0x38, 0x00, 0x00)},
	{NULL,		0},
};
#endif /* CONFIG_CMD_BMODE */

#define UART_PAD_CTRL	0x1b0b1
const iomux_v3_cfg_t const uart2_pads[] = {
	MX6_PAD_EIM_D26__UART2_TX_DATA	| MUX_PAD_CTRL(UART_PAD_CTRL),
	MX6_PAD_EIM_D27__UART2_RX_DATA	| MUX_PAD_CTRL(UART_PAD_CTRL),
};
const iomux_v3_cfg_t const uart3_pads[] = {
	MX6_PAD_EIM_D24__UART3_TX_DATA	| MUX_PAD_CTRL(UART_PAD_CTRL),
	MX6_PAD_EIM_D25__UART3_RX_DATA	| MUX_PAD_CTRL(UART_PAD_CTRL),
};

struct fsl_esdhc_cfg usdhc_cfg[] = {
        {
		.esdhc_base	= USDHC3_BASE_ADDR,
		.max_bus_width	= 8,
	},
        {
		.esdhc_base	= USDHC4_BASE_ADDR,
		.max_bus_width	= 4,
	},
};


#define USDHC_CLK_PAD_CTRL	0x10059
#define USDHC_PAD_CTRL		0x17059
const iomux_v3_cfg_t const usdhc3_pads[] = {
	MX6_PAD_SD3_CLK__SD3_CLK	| MUX_PAD_CTRL(USDHC_CLK_PAD_CTRL),
	MX6_PAD_SD3_CMD__SD3_CMD	| MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD3_DAT0__SD3_DATA0	| MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD3_DAT1__SD3_DATA1	| MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD3_DAT2__SD3_DATA2	| MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD3_DAT3__SD3_DATA3	| MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD3_DAT4__SD3_DATA4	| MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD3_DAT5__SD3_DATA5	| MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD3_DAT6__SD3_DATA6	| MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD3_DAT7__SD3_DATA7	| MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD3_RST__SD3_RESET	| MUX_PAD_CTRL(USDHC_PAD_CTRL),
};

const iomux_v3_cfg_t const usdhc4_pads[] = {
	MX6_PAD_SD4_CLK__SD4_CLK	| MUX_PAD_CTRL(USDHC_CLK_PAD_CTRL),
	MX6_PAD_SD4_CMD__SD4_CMD	| MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD4_DAT0__SD4_DATA0	| MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD4_DAT1__SD4_DATA1	| MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD4_DAT2__SD4_DATA2	| MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD4_DAT3__SD4_DATA3	| MUX_PAD_CTRL(USDHC_PAD_CTRL),
};

int board_early_init_f(void)
{
#if (CONFIG_MXC_UART_BASE == UART2_BASE)
	imx_iomux_v3_setup_multiple_pads(uart2_pads, ARRAY_SIZE(uart2_pads));
#elif (CONFIG_MXC_UART_BASE == UART3_BASE)
	imx_iomux_v3_setup_multiple_pads(uart3_pads, ARRAY_SIZE(uart3_pads));
#endif /* CONFIG_MXC_UART_BASE */
#if defined(CONFIG_MXC_SPI)
	ecspi_init();
#endif
#ifdef CONFIG_DIM_BCM89810
	bcm89810_reset_en(1);
	bcm89810_set_pads();
#endif /* CONFIG_DIM_BCM89810 */
#ifdef CONFIG_DIM_TOBYL2
	tobyl2_reset_en(1);
	tobyl2_set_pads();
#endif /* CONFIG_DIM_TOBYL2 */

	return 0;
}

#ifdef CONFIG_FEC_MXC
#define ETH_PAD_CTRL		0x1b0b0
#define ETH_CLK_PAD_CTRL	0x1b0a8

const iomux_v3_cfg_t const eth_pads[] = {
	MX6_PAD_ENET_MDIO__ENET_MDIO		| MUX_PAD_CTRL(ETH_PAD_CTRL),
	MX6_PAD_ENET_MDC__ENET_MDC		| MUX_PAD_CTRL(ETH_PAD_CTRL),
	MX6_PAD_RGMII_TXC__RGMII_TXC		| MUX_PAD_CTRL(ETH_PAD_CTRL),
	MX6_PAD_RGMII_TD0__RGMII_TD0		| MUX_PAD_CTRL(ETH_PAD_CTRL),
	MX6_PAD_RGMII_TD1__RGMII_TD1		| MUX_PAD_CTRL(ETH_PAD_CTRL),
	MX6_PAD_RGMII_TD2__RGMII_TD2		| MUX_PAD_CTRL(ETH_PAD_CTRL),
	MX6_PAD_RGMII_TD3__RGMII_TD3		| MUX_PAD_CTRL(ETH_PAD_CTRL),
	MX6_PAD_RGMII_TX_CTL__RGMII_TX_CTL	| MUX_PAD_CTRL(ETH_PAD_CTRL),
	MX6_PAD_ENET_REF_CLK__ENET_TX_CLK	| MUX_PAD_CTRL(ETH_PAD_CTRL),
	MX6_PAD_RGMII_RXC__RGMII_RXC		| MUX_PAD_CTRL(ETH_PAD_CTRL),
	MX6_PAD_RGMII_RD0__RGMII_RD0		| MUX_PAD_CTRL(ETH_PAD_CTRL),
	MX6_PAD_RGMII_RD1__RGMII_RD1		| MUX_PAD_CTRL(ETH_PAD_CTRL),
	MX6_PAD_RGMII_RD2__RGMII_RD2		| MUX_PAD_CTRL(ETH_PAD_CTRL),
	MX6_PAD_RGMII_RD3__RGMII_RD3		| MUX_PAD_CTRL(ETH_PAD_CTRL),
	MX6_PAD_RGMII_RX_CTL__RGMII_RX_CTL	| MUX_PAD_CTRL(ETH_PAD_CTRL),
	MX6_PAD_GPIO_16__ENET_REF_CLK		| MUX_PAD_CTRL(ETH_CLK_PAD_CTRL),
};

int board_eth_init(bd_t *bis)
{
	int rv;

	rv = enable_fec_anatop_clock(0, ENET_25MHz);
	if (rv) {
		printf("[net] unable to enable ethernet clock: %d\n", rv);
		goto out;
	}

	imx_iomux_v3_setup_multiple_pads(eth_pads, ARRAY_SIZE(eth_pads));
	/* use 25M anatop REF_CLK1 for ENET1, set gpr1[21] */
	mxc_iomux_set_gpr_register(1, IOMUXC_GPR1_ENET_CLK_SEL_MASK, 1, 1);
	/* set RGMII driving stength to 2V5 IOMUXC_SW_PATH_CTL_GRP_DDR_TYPE_RGMII */
	writel(0x000c0000, 0x20e0790);

#ifdef CONFIG_DIM_BCM89810
	/* take BCM89810 BroadR-Reach ethernet PHY out of reset*/
	udelay(15000);
	bcm89810_reset_en(0);
	udelay(25);
#endif /* CONFIG_DIM_BCM89810 */

	/*
	 * do *not* initialize the ethernet MAC since phylib cannot handle
	 * BroadR well
	 */
	rv = cpu_eth_init(bis);
	if (rv)
		printf("[net] unable to initialize ethernet MAC: %d", rv);

out:
	return rv;
}
#endif /* CONFIG_FEC_MXC */

int board_init(void)
{
	char *display;

	gd->bd->bi_boot_params = PHYS_SDRAM + 0x100;

	return 0;
}

int board_late_init(void)
{
	block_dev_desc_t *mmc_dev;
	disk_partition_t mmc_part;
	enum boot_device boot_dev;
	int mmc_dev_num;
	int rv;
	struct mmc *mmc;

#ifdef CONFIG_CMD_BMODE
	add_board_boot_modes(boot_modes);
#endif /* CONFIG_CMD_BMODE */
#ifdef CONFIG_DIM_WIBEAR11N
	wibear11n_reset();
#endif /* CONFIG_DIM_WIBEAR11N */
#if defined(CONFIG_DIM_BCM89810) && !defined(CONFIG_FEX_MXC)
	/* take BCM89810 out of reset since nobody else did it */
	bcm89810_reset_en(0);
#endif /* defined(CONFIG_DIM_BCM89810) && !defined(CONFIG_FEX_MXC) */
#if defined(CONFIG_DIM_TOBYL2) && !defined(CONFIG_IMX_UDC) \
	&& !defined(CONFIG_USB_EHCI_MX6)
	/* take TOBY-L2 out of reset since nobody else did it */
	mdelay(20);
	tobyl2_reset_en(0);
#endif /* defined(CONFIG_DIM_TOBYL2) && !defined(CONFIG_IMX_UDC) ... */

	boot_dev = get_boot_device();
	switch (boot_dev) {
	case SD3_BOOT:
	case MMC3_BOOT:
		setenv("boot_dev", "0");
		setenv("env_dev", "0");
		mmc_dev_num = 0;
		break;
	case SD4_BOOT:
	case MMC4_BOOT:
		setenv("boot_dev", "1");
		setenv("env_dev", "1");
		mmc_dev_num = 1;
		break;
	case SPI_NOR_BOOT:
		setenv("boot_dev", "1");
		setenv("env_dev", "1");
		mmc_dev_num = 1;
		break;
	default:
		printf("unsupported boot device: %d\n", boot_dev);
		rv = -1;
		goto out;
	}

	mmc = find_mmc_device(mmc_dev_num);
	if (NULL == mmc) {
		printf("(E) unable to find mmc device %d\n", mmc_dev_num);
		rv = -1;
		goto out;
	}
	rv = mmc_init(mmc);
	if (0 != rv) {
		printf("(E) unable initialize mmc device %d (%d)\n",
			mmc_dev_num, rv);
		goto out;
	}
	mmc_dev = mmc_get_dev(mmc_dev_num);
	if (NULL == mmc_dev) {
		printf("(E) unable to get mmc device %d\n", mmc_dev_num);
		rv = -1;
		goto out;
	}
	if (0 == get_partition_info(mmc_dev, 7, &mmc_part)) {
		printf("partition 7 (vendor) found, assuming android\n");
		setenv("env_part", "5");
		if (!getenv("bootcmd")) {
			setenv("env_file", "/etc/uEnv.txt");
			setenv("bootcmd", "run bootcmd_android");
		}
	} else {
		printf("partition 7 *not* found, assuming linux\n");
		setenv("env_part", "1");
		if (!getenv("bootcmd")) {
			setenv("env_file", "/uEnv.txt");
			setenv("bootcmd", "run bootcmd_linux");
		}
	}

	rv = 0;

out:
	return 0;
}

int board_mmc_getcd(struct mmc *mmc)
{
	int rv;
	struct fsl_esdhc_cfg *cfg;

	cfg = (struct fsl_esdhc_cfg *)mmc->priv;
	switch (cfg->esdhc_base) {
	case USDHC3_BASE_ADDR:
	case USDHC4_BASE_ADDR:
		/*
		 * The DIM board does not have capabilities to detect presence
		 * of eMMC or microSD. So we simply assume that both are
		 * present.
		 */
		rv = 1;
		break;
	default:
		printf("[sdhc] controller at 0x%x not supported\n",
			cfg->esdhc_base);
		rv = 0;
		break;
	}

	return rv;
}

int board_mmc_init(bd_t *bis) {
	int i;
	int rv;

        /*
         * mmc0	-> eMMC (on CPU board)
         * mmc1 -> microSD (on CPU board)
         */
	rv = 0;
	for (i = 0; i < ARRAY_SIZE(usdhc_cfg); i++) {
		switch (i) {
		case 0:
			imx_iomux_v3_setup_multiple_pads(usdhc3_pads,
				ARRAY_SIZE(usdhc3_pads));
			usdhc_cfg[i].sdhc_clk = mxc_get_clock(MXC_ESDHC3_CLK);
			break;
		case 1:
			imx_iomux_v3_setup_multiple_pads(usdhc4_pads,
				ARRAY_SIZE(usdhc4_pads));
			usdhc_cfg[i].sdhc_clk = mxc_get_clock(MXC_ESDHC4_CLK);
			break;
		default:
			printf("[sdhc] controller %d not supported\n", i);
			rv = -1;
			goto out;
		}

		rv |= fsl_esdhc_initialize(bis, &usdhc_cfg[i]);
	}

out:
	return rv;
}

int checkboard(void)
{
	puts("Board: i.MX6 Quad DIM\n");

        return 0;
}

int dram_init(void)
{
	gd->ram_size = ((phys_size_t)CONFIG_DDR_MB * SZ_1M);

	return 0;
}

#ifdef CONFIG_FASTBOOT
void board_fastboot_setup(void)
{
	enum boot_device dev;

	dev = get_boot_device();
	switch (dev) {
#if defined(CONFIG_FASTBOOT_STORAGE_MMC)
	case SD3_BOOT:
	case MMC3_BOOT:
		if (!getenv("fastboot_dev"))
			setenv("fastboot_dev", "mmc0");
		break;
	case SD4_BOOT:
	case MMC4_BOOT:
		if (!getenv("fastboot_dev"))
			setenv("fastboot_dev", "mmc1");
		break;
#endif /*CONFIG_FASTBOOT_STORAGE_MMC*/
	case SPI_NOR_BOOT:
		printf("SPI_NOR_BOOT is detected/n");
		if (!getenv("fastboot_dev"))
			setenv("fastboot_dev", "mmc1");
		break;
	default:
		printf("unsupported boot device: %d\n", dev);
		break;
	}
}

#ifdef CONFIG_ANDROID_RECOVERY
void board_recovery_setup(void)
{
	setenv("bootcmd", "run bootcmd_android_recovery");
}

int check_recovery_cmd_file(void)
{
	return recovery_check_and_clean_flag();
}
#endif /* CONFIG_ANDROID_RECOVERY */
#endif /* CONFIG_FASTBOOT */

#if defined(CONFIG_MXC_SPI)
#define SPI_PAD_CTRL		0x100b1
#define DIM_SPI1_CS1_GPIO	IMX_GPIO_NR(3, 19)

iomux_v3_cfg_t const ecspi1_pads[] = {
	MX6_PAD_EIM_D16__ECSPI1_SCLK | MUX_PAD_CTRL(SPI_PAD_CTRL),
	MX6_PAD_EIM_D17__ECSPI1_MISO | MUX_PAD_CTRL(SPI_PAD_CTRL),
	MX6_PAD_EIM_D18__ECSPI1_MOSI | MUX_PAD_CTRL(SPI_PAD_CTRL),
	MX6_PAD_EIM_D19__ECSPI1_SS1  | MUX_PAD_CTRL(NO_PAD_CTRL),
};

void ecspi_init(void)
{
	imx_iomux_v3_setup_multiple_pads(ecspi1_pads, ARRAY_SIZE(ecspi1_pads));
	//gpio_direction_output(DIM_SPI1_CS1_GPIO, 0);
}
#endif /* CONFIG_MXC_SPI */


#if defined(CONFIG_IMX_UDC) || defined(CONFIG_USB_EHCI_MX6)
#define DIM_USB_H1_PWR_EN_GPIO	IMX_GPIO_NR(3, 31)
#define DIM_USB_OTG_PWR_EN_GPIO	IMX_GPIO_NR(3, 22)

const iomux_v3_cfg_t const usb_h1_pads[] = {
	MX6_PAD_EIM_D31__GPIO3_IO31	| MUX_PAD_CTRL(NO_PAD_CTRL),
};
const iomux_v3_cfg_t const usb_otg_pads[] = {
	MX6_PAD_EIM_D22__GPIO3_IO22	| MUX_PAD_CTRL(NO_PAD_CTRL),
	MX6_PAD_GPIO_1__USB_OTG_ID	| MUX_PAD_CTRL(NO_PAD_CTRL),
};

void udc_pins_setting(void)
{
	imx_iomux_v3_setup_multiple_pads(usb_otg_pads,
		ARRAY_SIZE(usb_otg_pads));

	/* set USB_OTG_ID_SEL to GPIO_1 */
	mxc_iomux_set_gpr_register(1, IOMUXC_GPR1_USB_OTG_ID_SEL, 1, 1);
}

int board_ehci_hcd_init(int port)
{
	int rv;

	rv = 0;
	switch (port) {
	case 0:
		udc_pins_setting();
		break;
	case 1:
		imx_iomux_v3_setup_multiple_pads(usb_h1_pads,
			ARRAY_SIZE(usb_h1_pads));
		break;
	default:
		printf("[usb] port %d not supported\n", port);
		rv = 1;
	}

	return rv;
}

int board_ehci_power(int port, int on)
{
	switch (port) {
	case 0:
		if (on)
			gpio_direction_output(DIM_USB_OTG_PWR_EN_GPIO, 1);
		else
			gpio_direction_output(DIM_USB_OTG_PWR_EN_GPIO, 0);
		break;
	case 1:
		if (on) {
#ifdef CONFIG_DIM_TOBYL2
			gpio_direction_output(DIM_USB_H1_PWR_EN_GPIO, 0);
			tobyl2_reset_en(0);
#else
			gpio_direction_output(DIM_USB_H1_PWR_EN_GPIO, 1);
#endif /* CONFIG_DIM_TOBYL2 */
		} else {
			gpio_direction_output(DIM_USB_H1_PWR_EN_GPIO, 0);
		}
		break;
	default:
		printf("[usb] port %d not supported\n", port);
		return 1;
	}

	return 0;
}
#endif /* defined(CONFIG_IMX_UDC) || defined(CONFIG_USB_EHCI_MX6) */

#ifdef CONFIG_DIM_BCM89810
#define DIM_BCM89810_RST_GPIO	IMX_GPIO_NR(1, 25)

const iomux_v3_cfg_t const bcm89810_pads[] = {
	MX6_PAD_ENET_CRS_DV__GPIO1_IO25	| MUX_PAD_CTRL(0x108b0),
};

void bcm89810_reset_en(int en)
{
	// active low
	gpio_direction_output(DIM_BCM89810_RST_GPIO, 0 == en ? 1 : 0);
}

void bcm89810_set_pads(void)
{
	imx_iomux_v3_setup_multiple_pads(bcm89810_pads,
		ARRAY_SIZE(bcm89810_pads));
}
#endif /* CONFIG_DIM_BCM89810 */

#ifdef CONFIG_DIM_TOBYL2
#define DIM_TOBYL2_RST_GPIO	IMX_GPIO_NR(1, 17)

const iomux_v3_cfg_t const tobyl2_pads[] = {
	MX6_PAD_EIM_D31__GPIO3_IO31	| MUX_PAD_CTRL(NO_PAD_CTRL),
	MX6_PAD_SD1_DAT1__GPIO1_IO17	| MUX_PAD_CTRL(0x100b0),
};

void tobyl2_reset_en(int en)
{
	// active high
	gpio_direction_output(DIM_TOBYL2_RST_GPIO, 0 == en ? 0 : 1);
}

void tobyl2_set_pads(void)
{
	gpio_direction_output(DIM_USB_H1_PWR_EN_GPIO, 0);
	imx_iomux_v3_setup_multiple_pads(tobyl2_pads, ARRAY_SIZE(tobyl2_pads));
}
#endif /* CONFIG_DIM_TOBYL2 */

#ifdef CONFIG_DIM_WIBEAR11N
#define DIM_WIBEAR11N_RST_GPIO	IMX_GPIO_NR(5, 27)

const iomux_v3_cfg_t const wibear11n_pads[] = {
	MX6_PAD_CSI0_DAT9__GPIO5_IO27	| MUX_PAD_CTRL(NO_PAD_CTRL),
};

void wibear11n_reset(void)
{
	// active low
	printf("resetting WiBear 11n (20 ms)\n");
	gpio_direction_output(DIM_WIBEAR11N_RST_GPIO, 0);
	imx_iomux_v3_setup_multiple_pads(wibear11n_pads,
		ARRAY_SIZE(wibear11n_pads));
	udelay(20000);
	gpio_direction_output(DIM_WIBEAR11N_RST_GPIO, 1);
}
#endif /* CONFIG_DIM_WIBEAR11N */
