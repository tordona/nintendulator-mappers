/* Nintendulator Mapper DLLs
 * Copyright (C) QMT Productions
 *
 * $URL$
 * $Id$
 */

#include	"..\DLL\d_iNES.h"
#include	"..\Hardware\h_Latch.h"

namespace
{
void	Sync (void)
{
	EMU->SetPRG_ROM32(0x8, (Latch::Data >> 4) & 0xF);
	iNES_SetCHR_Auto8(0x0, (Latch::Data >> 0) & 0xF);
}

BOOL	MAPINT	Load (void)
{
	Latch::Load(Sync, FALSE);
	return TRUE;
}
void	MAPINT	Reset (RESET_TYPE ResetType)
{
	iNES_SetMirroring();
	EMU->SetCPUWriteHandler(0x6, Latch::Write);
	EMU->SetCPUWriteHandler(0x7, Latch::Write);
	Latch::Reset(ResetType);
}
void	MAPINT	Unload (void)
{
	Latch::Unload();
}

uint16 MapperNum = 66;
} // namespace

const MapperInfo MapperInfo_066 =
{
	&MapperNum,
	_T("GNROM/compatible"),
	COMPAT_FULL,
	Load,
	Reset,
	Unload,
	NULL,
	NULL,
	Latch::SaveLoad_D,
	NULL,
	NULL
};
