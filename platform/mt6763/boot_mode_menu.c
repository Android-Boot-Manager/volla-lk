/*This file implements MTK boot mode.*/

#include <sys/types.h>
#include <debug.h>
#include <err.h>
#include <reg.h>
#include <video.h>
#include <platform/mt_typedefs.h>
#include <platform/boot_mode.h>
#include <platform/mt_reg_base.h>
#include <platform/mtk_key.h>
#include <platform/mt_gpt.h>
#include <target/cust_key.h>
#include <platform/mtk_wdt.h>

#include <droidboot_platforms/lk/common_mtk/src/lk_mtk_common.h>

extern BOOL recovery_check_command_trigger(void);
#define MODULE_NAME "[BOOT_MENU]"
extern void cmdline_append(const char* append_string);
extern bool boot_ftrace;
bool g_boot_menu = false;

void boot_mode_menu_select()
{
    video_printf("Enter droidboot_mtk_show_boot_mode_menu");
	g_boot_mode=droidboot_mtk_show_boot_mode_menu();
}

BOOL boot_menu_key_trigger(void)
{
#if 1
	//wait
	ulong begin = get_timer(0);
	dprintf(INFO, "\n%s Check  boot menu\n",MODULE_NAME);
	dprintf(INFO, "%s Wait 50ms for special keys\n",MODULE_NAME);

	//let some case of recovery mode pass.
	if (unshield_recovery_detection()) {
		return TRUE;
	}

	while (get_timer(begin)<50) {
		if (mtk_detect_key(MT65XX_BOOT_MENU_KEY)) {
			mtk_wdt_disable();
			boot_mode_menu_select();
			mtk_wdt_init();
			return TRUE;
		}
	}
#endif
	return FALSE;
}

BOOL boot_menu_detection(void)
{
	return boot_menu_key_trigger();
}


int unshield_recovery_detection(void)
{
	//because recovery_check_command_trigger's BOOL is different from the BOOL in this file.
	//so use code like this type.
	return recovery_check_command_trigger()? TRUE:FALSE;
}

