/**
	Silencer
	You're a ninja.
*/

local Name = "$Name$";
local Description = "$Description$";
public func GetDescID() { return nil; }

func IsPerk(){return true;}

func Apply(clnk)
{
	if(clnk.silencer == nil)
		clnk.silencer = 0;
	clnk.silencer++;
}

func Unapply(clnk)
{
	clnk.silencer = Max(clnk.silencer -1, 0);
}
