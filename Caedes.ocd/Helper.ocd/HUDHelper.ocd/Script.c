/**
	HUDHelper
	Helps with HUD

*/

local Name = "$Name$";
local Description = "$Description$";

public func Initialize()
{
	this.Visibility = (VIS_Owner);
	this.Parallaxity = [0, 0];
	SetPosition(50, -10);
	return true;
}