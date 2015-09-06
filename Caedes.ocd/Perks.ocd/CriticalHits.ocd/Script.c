/**
	CriticalHits
	Your shots have a chance to hit stronger.

	@author <Insert Author>
*/

local Name = "$Name$";
local Description = "$Description$";
public func GetDescID() { return "CriticalHitsDesc"; }

func IsPerk(){return true;}

func Apply(clnk)
{
	if(clnk.crits == nil)
		clnk.crits = 0;
	clnk.crits++;
}

func Unapply(clnk)
{
	clnk.crits = Max(clnk.crits -1, 0);
}
