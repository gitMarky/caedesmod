/**
	Bandoleer
*/

local Name = "$Name$";
local Description = "$Description$";

func IsPerk(){return true;}
public func GetDescID() { return nil; }

func Apply(clnk)
{
	if(clnk.faster_reload == nil)
		clnk.faster_reload = 0;
	clnk.faster_reload++;
	
	for(var i = clnk->ContentsCount()-1; i >= 0; --i)
	{
		var obj = clnk->Contents(i);
		if(obj)
			ApplyToObject(obj);
	}
}

func Unapply(clnk)
{
	clnk.faster_reload = Max(clnk.faster_reload -1, 0);
	
	for(var i = clnk->ContentsCount()-1; i >= 0; --i)
	{
		var obj = clnk->Contents(i);
		if(obj)
			UnapplyToObject(obj);
	}
}

func ApplyToObject(object obj, int plr)
{
	if(GetEffect("HasBandoleer", obj)) return;
	if(obj.weapon_properties == nil) return;
	if(!obj.weapon_properties.max_ammo) return;
	if(!obj.weapon_properties.clip) return;
	
	AddEffect("HasBandoleer", obj, 1, 0, nil, Perk_Bandoleer);
}

func UnapplyToObject(object obj, int plr)
{
	if(GetEffect("HasBandoleer", obj))
		RemoveEffect("HasBandoleer", obj);
}

func FxHasBandoleerStart(target, effect, temp)
{
	if(temp) return true;
	var magazine = (target.total_ammo * 100) / target.weapon_properties.max_ammo;
	target.weapon_properties.max_ammo = target.weapon_properties.max_ammo + 2 * target.weapon_properties.clip;
	
	target.total_ammo = Max(target.total_ammo, (magazine * target.weapon_properties.max_ammo) / 100);
}

func FxHasBandoleerStop(target, effect, reason, temp)
{
	if(temp) return true;
	if(!target) return true;
	var magazine = (target.total_ammo * 100) / target.weapon_properties.max_ammo;
	target.weapon_properties.max_ammo = target.weapon_properties.max_ammo - 2 * target.weapon_properties.clip;
	target.total_ammo = Min(target.total_ammo, (magazine * target.weapon_properties.max_ammo) / 100);
}