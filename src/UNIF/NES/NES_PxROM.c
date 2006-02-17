#include "..\..\DLL\d_UNIF.h"
#include "..\..\Hardware\h_MMC2.h"

static	void	Sync_PNROM (void)
{
	MMC2_SyncPRG(0xF,0);
	MMC2_SyncCHR();
	MMC2_SyncMirror();
}

static	void	_MAPINT	Shutdown (void)
{
	MMC2_Destroy();
}

static	void	_MAPINT	Reset_PNROM (RESET_TYPE ResetType)
{
	MMC2_Init(ResetType,Sync_PNROM);
}

CTMapperInfo	MapperInfo_NES_PNROM =
{
	"NES-PNROM",
	"MMC2",
	COMPAT_FULL,
	Reset_PNROM,
	Shutdown,
	NULL,
	NULL,
	MMC2_SaveLoad,
	NULL,
	NULL
};
