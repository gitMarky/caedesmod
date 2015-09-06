/**
	First Strike
*/

local Name = "$Name$";
local Description = "$Description$";
public func GetDescID() { return nil; }

func IsPerk(){return true;}

func Apply(clnk)
{
	if(!GetEffect("FirstStrike", clnk))
	{
		var e = AddEffect("FirstStrike", clnk, 50, 0, nil);
		e.done = false;
	}
	AddEffect("FirstStrikeCallback", clnk, 1, 1, nil);
}

func Unapply(clnk)
{
	RemoveEffect("FirstStrike", clnk);
}

global func FxFirstStrikeDamage(target, effect, damage, cause)
{
	if(!GetEffect("FirstStrikeCallback", target))
		AddEffect("FirstStrikeCallback", target, 1, 1, nil);
	return damage;
}

global func FxFirstStrikeStop(target, effect, reason, temp)
{
	if(temp) return;
	if(effect.done)
		target.crits = Max(target.crits - 1, 0);

	if(GetEffect("FirstStrikeCallback", target))
		RemoveEffect("FirstStrikeCallback", target);
}

global func FxFirstStrikeCallbackTimer(target, effect)
{
	Perk_FirstStrike->Check(target);
	return -1;
}

func Check(obj)
{
	var e;
	if(!(e = GetEffect("FirstStrike", obj))) return;

	var max = obj.MaxEnergy / 1000;
	var cur = obj->GetEnergy();

	if(cur == max)
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
