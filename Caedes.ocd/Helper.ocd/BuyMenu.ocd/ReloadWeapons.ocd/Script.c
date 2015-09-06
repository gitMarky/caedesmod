/**
	CriticalHits
	Your shots have a chance to hit stronger.

*/

local Name = "$Name$";
local Description = "$Description$";
public func GetDescID() { return nil; }

public func IsSecondaryWeapon(){return true;}
func IsSpecial(){return true;}

func TryReload(clnk, c)
{
	var plr = clnk->GetOwner();
	if(!c) return 0;
	if(!c->IsWeapon()) return 0;
	if(!c.weapon_properties) return 0;
	
	if(!c.weapon_properties.max_ammo) return 0;
	if(c.total_ammo >= c.weapon_properties.max_ammo) return 0;
		
	var p = (c.total_ammo * 100) / c.weapon_properties.max_ammo;
	//var cost = (10 * p) / 100;
	var cost = 10 - (p/10);
	if(cost <= 0) cost = 1;
		
	if(GetWealth(plr) < cost)
	{
		Sound("Prrrrb", false, nil, plr);
		return -1;
	}
		
	c.total_ammo = c.weapon_properties.max_ammo;
	if(!GetEffect("Deviate", c))
		c.rounds = c.weapon_properties.clip;
	DoWealth(plr, -cost);
	clnk->~UpdateHUD();
	return 1;
}	

func Apply(clnk)
{
	var found = false;
	var plr = clnk->GetOwner();
	for(var i = 0; i < clnk->ContentsCount(); ++i)
	{
		var c = clnk->Contents(i);
		if(TryReload(clnk, c) > 0) found = true;
	}
	
	if(!found)
		Sound("Nope", false, nil, plr);
	else Sound("Cash", false, nil, plr);
}

