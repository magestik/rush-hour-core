#include "pre.h"

/*explicit*/ CGameBlock::CGameBlock(t_position pos, short s, EAxis a)
: position(pos)
, size(s)
, axis(a)
{

}

/*virtual*/ CGameBlock::~CGameBlock(void)
{

}
