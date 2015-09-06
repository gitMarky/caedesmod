/**
	HelmetPart
	Broken.

	@author <Insert Author>
*/

local Name = "$Name$";
local Description = "$Description$";

public func Initialize()
{
	SetXDir(RandomX(-5, 5));
	SetYDir(-30);
	var r = 30;
	if(Random(2)) r = -30;
	SetRDir(r);
	SetObjDrawTransform(900, 0, 0, 0, 900);
	return true;
}