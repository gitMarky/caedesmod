/**
	C4
	Can be thrown and triggered later.

*/

local Name = "$Name$";
local Description = "$Description$";

func TitleDef(){return C4_Title;}
func IsWeapon(){return true;}
func IsSecondaryWeapon(){return true;}
public func GetDescID() { return "C4Desc"; }

local my_fuse;

func Departure(obj)
{
	if(!obj) return;
	if(!obj->~IsClonk()) return;
	ScheduleCall(this, "SpeedAdjust", 1);
	my_fuse = obj->CreateContents(C4_Fuse);
	my_fuse->Set(this);
	
	obj->SetHandItemPos(1, obj->GetItemPos(my_fuse));
	
	this.Visibility = obj.Visibility;
	if(this.Visibility != VIS_All)
		AddEffect("CheckVis", this, 1, 20, this);
	MakeDefusable(35*2, GetPlayerTeam(obj->GetOwner()));
}

func OnDefused()
{
	/*if(my_fuse)
		my_fuse->RemoveObject();*/
	Smoke(0, 0, 10, nil, true);
	RemoveObject();
}

func SpeedAdjust()
{
	SetSpeed(GetXDir(100) / 3, GetYDir(100) / 3, 100);
}

func FxCheckVisTimer(target, effect, time)
{
	var obj;
	if(obj = FindObject(Find_Distance(100), Find_OCF(OCF_Alive), Find_Hostile(GetController()), Sort_Distance()))
	{
		if(!PathFree(GetX(), GetY(), obj->GetX(), obj->GetY())) return 1;
		this.Visibility = VIS_All;
		return -1;
	}
	return 1;
}

func BlowUpEx()
{
	Sound("Click");
	ScheduleCall(this, "BlowUp", 10);
	this.Visibility = (VIS_All);
}
func BlowUp()
{
	ExplosionEffect(30);
	Sound("BlastC4");
	for(var obj in FindObjects(Find_Distance(60), Find_Or(Find_OCF(OCF_Alive), Find_Func("IsProjectileTarget", this)), Find_NoContainer(), Find_PathFree(this)))
	{
		DoDmg(60 - ObjectDistance(this, obj), nil, obj, nil, nil, nil, GetID());
	}
	RemoveObject();
}
