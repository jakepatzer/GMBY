#include "Registers.h"

bool Registers::getFlag(byte flag)
{
	return (flag & f) == flag;
}

void Registers::setFlag(byte flag)
{
	f |= flag;
}

void Registers::resetFlag(byte flag)
{
	f &= ~flag;
}

void Registers::toggleFlag(byte flag)
{
	f ^= flag;
}
