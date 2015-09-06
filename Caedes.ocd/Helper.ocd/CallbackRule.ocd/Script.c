
func Initialize()
{
	SetPosition(0, 0);
}

func OnClonkDeath(object clonk, int killed_by)
{
	var plr = clonk->GetOwner();
	
	// no handling of invalid players
	if(!GetPlayerName(plr))
	{
		return;
	}
	
	// announce death
	if(plr != NO_OWNER)
	{ // scope
		var x = -25;
		var other = FindObject(Find_InRect(x - 10 - GetX(), 0 - GetY(), 20, 500), Find_ID(FloatingMessage), Sort_Distance(x - GetX(), 500));
		var y = 300;
		if(other) y = Max(y, other->GetY() + 25);
		var flm = CreateObject(FloatingMessage, x - GetX(), y - GetY(), NO_OWNER);
		var ws = "->";
		if(clonk.last_damaging_weapon)
			ws = Format("{{%i}}", GetTitleDef(clonk.last_damaging_weapon));
		var killer_name = GetTaggedPlayerName(killed_by);
		if(!killer_name) killer_name = "Gaia";
		flm->SetMessage(Format("%s %s %s", killer_name, ws, GetTaggedPlayerName(plr)));
		flm->FadeOut(10, 20);
		flm->SetYDir(-1);
		flm->SetCategory(flm->GetCategory() | C4D_Parallax | C4D_IgnoreFoW);
		flm.Parallaxity = [0, 0];
	}
	
	// respawn instantly
	RespawnPlayer(plr, clonk);
		
	// Only log for existing players and clonks.
	if (plr == NO_OWNER || !GetPlayerName(plr) || !clonk) 
		return;
	Scoreboard->SetPlayerData(plr, "player_dead", Symbol_Skull);
	ScheduleCall(this, "OnClonkDeathEx", 1, 0, clonk, plr, killed_by);
	return _inherited(clonk, killed_by, ...);
}

func OnClonkDeathEx(object clonk, int plr, int killed_by)
{
	if(!GetPlayerName(plr)) return;
	
	
	if(Hostile(killed_by, plr))
	{
		var bounty = clonk.kill_bounty ?? 0;
		DoPlrScore(killed_by, 15 + bounty);
		// update kills
		var enemy_team = GetPlayerTeam(killed_by);
		Scoreboard->SetPlayerData(killed_by, "kills", ++Caedes_player_kills[killed_by]);
		Scoreboard->SetData(CdsTeamID(enemy_team), "kills", ++Caedes_team_kills[enemy_team]);
	}
	
	// updates deaths
	var team = GetPlayerTeam(plr);
	Scoreboard->SetPlayerData(plr, "deaths", ++Caedes_player_deaths[plr]);
	Scoreboard->SetData(CdsTeamID(team), "deaths", ++Caedes_team_deaths[team]);
	
	if(clonk)
		clonk->CastBlood();
	
	return _inherited(clonk, plr, killed_by, ...);
}

func GetAdditionalPlayerRelaunchString(clonk, owner, killed_by)
{
	if(!clonk) return nil;
	if(clonk.last_damaging_weapon)
	{
		var wep = clonk.last_damaging_weapon;
		return Format("[{{%i}} ~ %s]", GetTitleDef(wep), wep->GetName());
	}
	return nil;
}

func RemovePlayer(plr, team)
{
	if(!GetEffect("IsGameOver", nil))
	{
		Scoreboard->SetPlayerData(plr, "deaths", -1);
		AddEvaluationData(Z4GetPlrLeagueRank(plr), GetPlayerID(plr));
	}
}

func InitializePlayer(int newplr, int x, int y, object base, int team)
{
	Caedes_player_kills[newplr] = 0;
	Caedes_player_deaths[newplr] = 0;
	Caedes_player_score[newplr] = 0;
	
	Scoreboard->NewPlayerEntry(newplr);
	Scoreboard->SetPlayerData(newplr, "team", "", CdsTeamID(team) + 1);
	
	Z4LeagueSystemRefreshScoreboard(newplr);
	SetPlayerViewLock(newplr, true);
	RoundHelper->InitPlayer(newplr, nil);
}