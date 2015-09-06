
static team_flags;

protected func Initialize()
{
	// Goal.
	//CreateObject(Goal_Melee, 0, 0, NO_OWNER);
	CreateObject(Rule_KillLogs);
	CreateObject(Goal_Destruction);
	CreateObject(Rule_SharkPit);
	
	PlaceBombZone(LandscapeWidth()/2, 325);
	//PlaceBombZone(LandscapeWidth()/2, 370);
	
	InitCaedes();
	
	team_flags = [];
	var pos = [{x = 50, y = 250 + 35}, {x = LandscapeWidth() - 50, y = 250 + 35}];
	for(var t = 1; t <= 2; ++t)
	{
		var flag = CreateObject(Flagpole, pos[t-1].x, pos[t-1].y, -1);
		team_flags[t] = flag;
		flag->SetFlagRadius(60);
		flag.Team = t;
	}
	return;
}

func InitializePlayer(plr, x, y, base, team)
{
	if(team_flags[team]->GetOwner() == NO_OWNER)
	{
		team_flags[team]->SetOwner(plr);
		team_flags[team]->~OnOwnerChange(-1);
	}
}


global func GetTeamSpawnPosition(int team)
{
	var pos = [{x = 50, y = 300}, {x = LandscapeWidth() - 50, y = 300}];
	pos = pos[team - 1];
	return pos;
}

global func GetPlayerRespawnPosition(plr)
{
	
	var pos = GetTeamSpawnPosition(GetPlayerTeam(plr));
	pos.x += RandomX(-30, 30);
	return pos;
}
