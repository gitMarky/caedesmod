/**
	FlakVest
	Lowers the damage of every shot
*/

local Name = "$Name$";
local Description = "$Description$";

static const FlakVest_MaxDamage = 50;

func TitleDef(){return FlakVest_Title;}

func NeedsNoSlot(){return true;} // for the buy menu
func IsWeapon(){return true;}
func IsGadget(){return true;}
public func GetDescID() { return FlakVest_Title; }

func RejectPurchase(object to)
{
	var e = GetEffect("WearsFlakVest", to);
	if(!e) return false;
	
	return !e.damage_taken;
}

func Entrance(obj)
{
	var e = GetEffect("WearsFlakVest", obj);
	if(!e) e = AddEffect("WearsFlakVest", obj, 10, 0, nil, FlakVest);
	e.damage_taken = 0;
	EffectCall(obj, e, "Show");
	RemoveObject();
}

func FxWearsFlakVestStart(target, effect, temp)
{
	if(temp) return;
	effect.attach = target->AttachMesh(FlakVest, "skeleton_body", "main");
}

func FxWearsFlakVestShow(target, effect)
{
	if(effect.bar) effect.bar->SetValue(effect.damage_taken);
	else
	{
		var o = FindObject(Find_ID(CallbackRule));
		effect.bar = o->CreateProgressBar(GUI_CustomImageProgressBar, FlakVest_MaxDamage, effect.damage_taken, 36, target->GetOwner(), {x = -100, y = -100}, VIS_Owner, {image = FlakVest_HUDImage});
		effect.bar->MakeHUDElement();
	}
}

func FxWearsFlakVestDamage(target, effect, dmg, cause)
{
	if(dmg >= 0) return dmg;
	
	// prevent shot!
	
	if(!Random(10))
		target->Sound("LightMetalHit*");
	
	// weaken damage
	if(dmg < -1000)
		dmg -= -1000;
	else dmg = Max(dmg, -500);
	
	effect.damage_taken += Max(1, Abs((dmg / 2000)));
	
	EffectCall(target, effect, "Show");
	
	if(effect.damage_taken > FlakVest_MaxDamage)
	{
		RemoveEffect(nil, target, effect);
		Sound("Clonk");
	}
	return dmg;
}

func FxWearsFlakVestStop(target, effect, reason, temp)
{
	if(temp) return;
	if(target)
		target->DetachMesh(effect.attach);
	
}
