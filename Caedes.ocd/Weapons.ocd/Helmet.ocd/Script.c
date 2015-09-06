/**
	Helmet
	Prevents one deadly shot.
*/

local Name = "$Name$";
local Description = "$Description$";

func TitleDef(){return Helmet_Title;}

func NeedsNoSlot(){return true;} // for the buy menu
func IsWeapon(){return true;}
func IsGadget(){return true;}
public func GetDescID() { return "HelmetDesc"; }

func RejectPurchase(object to)
{
	return !!GetEffect("WearsHelmet", to);
}

func Entrance(obj)
{
	if(GetEffect("WearsHelmet", obj)) return true;
	AddEffect("WearsHelmet", obj, 9, 0, nil, Helmet);
	RemoveObject();
}

func FxWearsHelmetStart(target, effect, temp)
{
	if(temp) return;
	effect.attach = target->AttachMesh(Helmet, "skeleton_head", "main");
}

func FxWearsHelmetDamage(target, effect, dmg, cause)
{
	if(dmg >= 0) return dmg;
	
	if((target->GetEnergy() + dmg/1000) > 0) return dmg;
	
	// prevent shot!
	
	target->Sound("Catapult_Launch", false, nil, nil);
	Schedule(target, "Sound(\"Hurt*\")", 20);
	
	RemoveEffect(nil, target, effect);
	
	return 0;
}

func FxWearsHelmetStop(target, effect, reason, temp)
{
	if(temp) return;
	if(target)
	{
		target->DetachMesh(effect.attach);
		CreateObject(Helmet_Part, target->GetX(), target->GetY());
	}
}
