/**
	SmokeGrenade
	Used to annoy the fire brigade.

*/

local Name = "$Name$";
local Description = "$Description$";

func TitleDef(){return SmokeGrenade_Title;}
func IsWeapon(){return true;}
public func IsSecondaryWeapon(){return true;}
public func GetDescID() { return nil; }


func Departure(obj)
{
	if(!obj) return;
	if(!obj->~IsClonk()) return;
	//ScheduleCall(this, "SpeedAdjust", 1);
	
	this.Visibility = obj.Visibility;
}


func SpeedAdjust()
{
	SetSpeed(GetXDir(100) / 2, GetYDir(100) / 2, 100);
}

func Hit(x, y)
{
	if(GBackSolid(0, 5))
	{
		SetSpeed();
		SetCategory(C4D_StaticBack);
		var smoke = CreateObject(SmokeGrenade_SmokeScreen, 0, 2, NO_OWNER);
		smoke->SetPosition(GetX(), GetY() - 32);
		smoke->Init();
		ScheduleCall(this, "Remove", 35*10);
		return;
	}
	
	if(GBackSolid(4, 0) || GBackSolid(-4, 0)) SetXDir(-x/2, 100);
}

func Remove()
{
	RemoveObject();
}
