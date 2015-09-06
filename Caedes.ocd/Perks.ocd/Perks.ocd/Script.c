
static Perks_player_perks;

global func InitPerks(plr)
{
	if(GetType(Perks_player_perks) != C4V_Array)
		Perks_player_perks = [];
		
	if(GetType(Perks_player_perks[plr]) != C4V_Array)
		Perks_player_perks[plr] = [];
}

global func HasPerk(plr, ID)
{
	InitPerks(plr);
	
	return IsValueInArray(Perks_player_perks[plr], ID);
}

global func GetPlayerPerks(plr)
{
	InitPerks(plr);
	
	return Perks_player_perks[plr];
}

global func PerkChangeDenied(plr)
{
	var c = GetCursor(plr);
	if(!c) return false;
	if(c->Contained()) return false;
	if(c->GetEnergy() == c.MaxEnergy/1000) return false;
	if(GetEffect("NewRound", c)) return false;
	
	return true;
}

global func AddPerk(plr, ID)
{
	InitPerks(plr);
	// currently exclusive
	for(var p in Perks_player_perks[plr])
		RemovePerk(plr, p);
	if(!PerkChangeDenied(plr))
		ID->Apply(GetCrew(plr));
	PushBack(Perks_player_perks[plr], ID);
	
	var c = GetCursor(plr);
	if(c) c->~UpdateHUD();
}

global func RemovePerk(plr, ID)
{
	InitPerks(plr);
	if(IsValueInArray(Perks_player_perks[plr], ID))
		if(!PerkChangeDenied(plr))
			ID->Unapply(GetCrew(plr));
	RemoveArrayValue(Perks_player_perks[plr], ID, true);
	
	var c = GetCursor(plr);
	if(c) c->~UpdateHUD();
}

global func ApplyPerks(plr)
{
	InitPerks(plr);
	for(var p in Perks_player_perks[plr])
		p->Apply(GetCrew(plr));
	
	var c = GetCursor(plr);
	if(c) c->~UpdateHUD();
}

global func ApplyObjectPerks(int plr, object obj)
{
	InitPerks(plr);
	for(var p in Perks_player_perks[plr])
		p->~ApplyToObject(obj);
}

global func UnapplyObjectPerks(int plr, object obj)
{
	InitPerks(plr);
	for(var p in Perks_player_perks[plr])
		p->~UnapplyToObject(obj);
}