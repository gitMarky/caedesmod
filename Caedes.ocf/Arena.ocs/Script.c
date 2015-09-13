
static team_flags;
static spawn_pos;

protected func Initialize()
{
	SetNextMission("Caedes.ocf/Arena.ocs","RESTART","$RestartDesc$");
	
	SmoothLandscape();
		
	// Goal.
	//CreateObject(Goal_Melee, 0, 0, NO_OWNER);
	CreateObject(Rule_KillLogs);
	CreateObject(Goal_Destruction);
	
	// spawn points	
	CreateSpawnPoint(LandscapeWidth()/2 + 50, 263)->SetRespawnTimer(SPAWNPOINT_Timer_Infinite)->SpawnDeco(ExplosiveBarrel);
	CreateSpawnPoint(LandscapeWidth()/2 + 50, 263)->SetRespawnTimer(SPAWNPOINT_Timer_Infinite)->SpawnDeco(ExplosiveBarrel);

	CreateSpawnPoint(383, 123 - 11)->SetRespawnTimer(SPAWNPOINT_Timer_Infinite)->SpawnDeco(PottedPlant);
	CreateSpawnPoint(419, 123 - 11)->SetRespawnTimer(SPAWNPOINT_Timer_Infinite)->SpawnDeco(PottedPlant, {width = -1000, height = 1000});

	PlaceBombZone(LandscapeWidth()/2, 115);
	PlaceBombZone(LandscapeWidth()/2, 315 + 8);
	
	InitCaedes();
	
	team_flags = [];
	spawn_pos = [{x = 75, y = 213}, {x = LandscapeWidth() - 75, y = 213}];
	
	var pos = [{x = 75, y = 213}, {x = LandscapeWidth() - 75, y = 213}];
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
	var pos = {Prototype = spawn_pos[team - 1]};
	return pos;
}

global func GetPlayerRespawnPosition(plr)
{
	
	var pos = GetTeamSpawnPosition(GetPlayerTeam(plr));
	pos.x += RandomX(-30, 30);
	return pos;
}


func SwitchSides()
{
	var m = spawn_pos[0], m2 = spawn_pos[1];
	spawn_pos[0] = m2;
	spawn_pos[1] = m;
	
	for(var flag in team_flags)
	{
		if(!flag) continue;
		
		var team = 3 - flag.Team;
		flag.Team = team;
		for(var i = 0; i < GetPlayerCount(); ++i)
		{
			var p = GetPlayerByIndex(i);
			if(GetPlayerTeam(p) == team){flag->SetOwner(p); flag->~OnOwnerChange(-1); break;}
		}
	}

	return true;
}