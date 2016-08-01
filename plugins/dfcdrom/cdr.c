/*
 * Cdrom for Psemu Pro like Emulators
 *
 * By: linuzappz <linuzappz@hotmail.com>
 *
 */

#include "config.h"

#ifdef ENABLE_NLS
#include <libintl.h>
#include <locale.h>
#define _(x)  gettext(x)
#define N_(x) (x)
#else
#define _(x)  (x)
#define N_(x) (x)
#endif

#if defined (__linux__)
#include "cdr-linux.c"
#else
#include "cdr-null.c"
#endif

char *PSEgetLibName(void) {
	return _(LibName);
}

unsigned long PSEgetLibType(void) {
	return PSE_LT_CDR;
}

unsigned long PSEgetLibVersion(void) {
	return 1 << 16;
}

#define FUNC(n) { #n, n }

static const struct {
	const char *name;
	void *func;
} plugin_funcs[] = {
	/* CDR */
	FUNC(CDRinit),
	FUNC(CDRshutdown),
	FUNC(CDRopen),
	FUNC(CDRclose),
	FUNC(CDRgetTN),
	FUNC(CDRgetTD),
	FUNC(CDRreadTrack),
	FUNC(CDRgetBuffer),
	FUNC(CDRgetBufferSub),
	FUNC(CDRplay),
	FUNC(CDRstop),
	FUNC(CDRgetStatus),
};

void dfcdrom_set_fname(const char *fname)
{
	strcpy(CdromDev, fname);
}

void *dfcdrom_get_sym(const char *sym)
{
	int i;
	for (i = 0; i < sizeof(plugin_funcs) / sizeof(plugin_funcs[0]); i++)
		if (strcmp(plugin_funcs[i].name, sym) == 0)
			return plugin_funcs[i].func;
	return NULL;
}

#include <mntent.h>
int dfcdrom_path_is_cd(const char *fname)
{
	FILE *mounts = NULL;
	struct mntent *ent = NULL;
	int iscd = 0;

	if ((mounts = setmntent("/etc/mtab", "r")))
	{
		while ((ent = getmntent(mounts)))
		{
			if (/*!strcmp(ent->mnt_dir, fname)*/
				!strcmp(ent->mnt_fsname, fname) &&
				!strcmp(ent->mnt_type, "iso9660"))
			{
				iscd = 1;
				break;
			}
		}
		endmntent(mounts);
	}

	return iscd;
}

