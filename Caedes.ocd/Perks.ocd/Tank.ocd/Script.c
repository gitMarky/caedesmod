/**
	Tank
	Your HP increase.
*/

local Name = "$Name$";
local Description = "$Description$";
public func GetDescID() { return nil; }

func IsPerk(){return true;}

func Apply(clonk)
{
	clonk.MaxEnergy += 25000;
	clonk->DoEnergy(25);
}

func Unapply(clonk)
{
	clonk.MaxEnergy -= 25000;
	clonk->DoEnergy(-25);
}
