/*--
	
--*/


#include Library_Goal

local fulfilled;

protected func Initialize()
{
	fulfilled = false;
	return inherited(...);
}

public func IsFulfilled()
{
	return fulfilled;
}

public func Fulfill()
{
	fulfilled = true;
	return;
}

func QueryRejectNewRound(int winning_team)
{
	// look for planted bomb
	var bomb = FindObject(Find_ID(BigBomb));
	if(!bomb) return;
	
	// no objection if the bomb is not even planted
	if(!GetEffect("IsDefusable", bomb)) return;
	
	// if the bomb belongs to the winning team, no objections
	if(bomb.team == winning_team) return;
	
	if(!bomb.already_announced)
	{
		bomb.already_announced = true;
		
		for(var i = 0; i < GetPlayerCount(); ++i)
		{
			var plr = GetPlayerByIndex(i);
			if(GetPlayerTeam(plr) == bomb.team) continue;
			Symbol_InfoDefuseTheBomb->ShowFor(plr);
		}
	}
	
	// BOMB ENEMY; DEFUSE!
	return true;
	
	return;
}

/*-- Proplist --*/
local Name = "$Name$";
