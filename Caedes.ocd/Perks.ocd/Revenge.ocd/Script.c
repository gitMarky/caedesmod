/**
	Revenge
*/

local Name = "$Name$";
local Description = "$Description$";
public func GetDescID() { return nil; }

func IsPerk(){return true;}

func Apply(clnk)
{
	if(!GetEffect("Revenge", clnk))
	{
		var e = AddEffect("Revenge", clnk, 50, 0, nil);
		e.done = false;
	}
	AddEffect("RevengeCallback", clnk, 1, 1, nil);
}

func Unapply(clnk)
{
	RemoveEffect("Revenge", clnk);
}

global func FxRevengeDamage(target, effect, damage, cause)
{
	if(!GetEffect("RevengeCallback", target))
		AddEffect("RevengeCallback", target, 1, 1, nil);
	return damage;
}

global func FxRevengeStop(target, effect, reason, temp)
{
	if(temp) return;
	if(effect.done)
		target.crits = Max(target.crits - 1, 0);

	if(GetEffect("RevengeCallback", target))
		RemoveEffect("RevengeCallback", target);
}

global func FxRevengeCallbackTimer(target, effect)
{
	Perk_Revenge->Check(target);
	return -1;
}

func Check(obj)
{
	var e;
	if(!(e = GetEffect("Revenge", obj))) return;

	var max = obj.MaxEnergy / 1000;
	var cur = obj->GetEnergy();

	if(cur < max/3)
	{
		 if(!e.done)
		 {
		 	if(obj.crits == nil)
				obj.crits = 0;
			obj.crits++;
			e.done = true;
		}
	}
	else 
		if(e.done)
		{
			obj.crits = Max(obj.crits - 1, 0);
			e.done = false;
		}
	
	
}
