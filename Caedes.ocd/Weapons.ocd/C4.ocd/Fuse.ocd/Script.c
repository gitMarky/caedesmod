/**
	Fuse
	Fuses a pack of C4.
*/

local Name = "$Name$";
local Description = "$Description$";

func IsWeapon(){return true;}
local target;

func OnNewRound()
{
	RemoveObject();
}

func Set(to)
{
	target = to;
}

func ControlUseStart(clonk)
{
	if(!target) return RemoveObject();
	
	target->BlowUpEx();
	RemoveObject();
	return 1;
}