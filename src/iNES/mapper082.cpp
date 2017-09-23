/* Nintendulator Mapper DLLs
 * Copyright (C) QMT Productions
 *
 * $URL$
 * $Id$
 */

#include	"..\DLL\d_iNES.h"

namespace
{
uint8 PRG[3], CHR[6];
uint8 Mirror;
FCPUWrite _Write7;

void	Sync (void)
{
	EMU->SetPRG_RAM8(0x6, 0);
	EMU->SetPRG_ROM8(0x8, PRG[0]);
	EMU->SetPRG_ROM8(0xA, PRG[1]);
	EMU->SetPRG_ROM8(0xC, PRG[2]);
	EMU->SetPRG_ROM8(0xE, -1);
	EMU->SetCHR_ROM2(0 ^ ((Mirror << 1) & 0x04), CHR[0]);
	EMU->SetCHR_ROM2(2 ^ ((Mirror << 1) & 0x04), CHR[1]);
	EMU->SetCHR_ROM1(4 ^ ((Mirror << 1) & 0x04), CHR[2]);
	EMU->SetCHR_ROM1(5 ^ ((Mirror << 1) & 0x04), CHR[3]);
	EMU->SetCHR_ROM1(6 ^ ((Mirror << 1) & 0x04), CHR[4]);
	EMU->SetCHR_ROM1(7 ^ ((Mirror << 1) & 0x04), CHR[5]);
	if (Mirror & 1)
		EMU->Mirror_V();
	else	EMU->Mirror_H();
}

int	MAPINT	SaveLoad (STATE_TYPE mode, int offset, unsigned char *data)
{
	SAVELOAD_BYTE(mode, offset, data, Mirror);
	for (int i = 0; i < 3; i++)
		SAVELOAD_BYTE(mode, offset, data, PRG[i]);
	for (int i = 0; i < 8; i++)
		SAVELOAD_BYTE(mode, offset, data, CHR[i]);
	if (mode == STATE_LOAD)
		Sync();
	return offset;
}

void	MAPINT	Write (int Bank, int Addr, int Val)
{
	_Write7(Bank, Addr, Val);
	switch (Addr)
	{
	case 0xEF0:	CHR[0] = Val >> 1;	break;
	case 0xEF1:	CHR[1] = Val >> 1;	break;
	case 0xEF2:	CHR[2] = Val;		break;
	case 0xEF3:	CHR[3] = Val;		break;
	case 0xEF4:	CHR[4] = Val;		break;
	case 0xEF5:	CHR[5] = Val;		break;
	case 0xEF6:	Mirror = Val;		break;
	case 0xEFA:	PRG[0] = Val >> 2;	break;
	case 0xEFB:	PRG[1] = Val >> 2;	break;
	case 0xEFC:	PRG[2] = Val >> 2;	break;
	}
	Sync();
}

BOOL	MAPINT	Load (void)
{
	iNES_SetSRAM();
	return TRUE;
}
void	MAPINT	Reset (RESET_TYPE ResetType)
{
	_Write7 = EMU->GetCPUWriteHandler(0x7);
	EMU->SetCPUWriteHandler(0x7, Write);

	if (ResetType == RESET_HARD)
	{
		Mirror = 0;
		for (int i = 0; i < 8; i++)
			CHR[i] = i;
		PRG[0] = 0;
		PRG[1] = 1;
		PRG[2] = 0xFE;
	}

	Sync();
}

uint16 MapperNum = 82;
} // namespace

const MapperInfo MapperInfo_082 =
{
	&MapperNum,
	_T("Mapper 82"),
	COMPAT_NEARLY,
	Load,
	Reset,
	NULL,
	NULL,
	NULL,
	SaveLoad,
	NULL,
	NULL
};
