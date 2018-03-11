
static const Caedes_MaxWealth = 200;
static const Caedes_RoundTime = 4320; // 36 * 60 * 2

static Caedes_ViewPoints;
static Caedes_IsBombPlanted;
static Caedes_BombingTeam;

static Caedes_player_kills;
static Caedes_player_deaths;
static Caedes_player_score;

static Caedes_player_round_damage;
static Caedes_player_round_score;

static Caedes_team_score;
static Caedes_team_kills;
static Caedes_team_deaths;

static Caedes_smoke_count;

// time in seconds
global func GetMaximumRoundTime(){return 60 * 7;}

global func CdsTeamID(int team)
{
	return 1000 + 10 * team;
}

global func InitCaedes()
{
	Caedes_player_kills = [];
	Caedes_player_deaths = [];
	Caedes_player_score = [];
	
	Caedes_player_round_damage = [];
	Caedes_player_round_score = [];
	
	Caedes_team_kills = [];
	Caedes_team_deaths = [];
	Caedes_team_score = [];
	
	Caedes_BombingTeam = 1 + Random(2);
	
	//AddEffect("GlobalVisibilityCheck", 0, 1, 15, 0);
	if(!ObjectCount(Find_ID(CallbackRule)))
		CreateObject(CallbackRule);
	AddEffect("SchedNewRound", nil, 1, 60);
	
	StopGameCounter(true); // create game counter, stopped
	
	Caedes_ViewPoints = [];
	SetAmbientBrightness(0);
	
	Scoreboard->Init(
		[
		 {key = "team", title = "", sorted = true, priority = 200, default = ""},
		 {key = "score", title = "Score", priority = 100, default = ""},
		 {key = "player_dead", title = "-", priority = 90, default = ""},
		 {key = "kills", title = Sword, sorted = true, desc = true, priority = 75, default = 0},
		 {key = "deaths", title = Clonk, sorted = true, desc = false, priority = 50, default = 0, conditional = Scoreboard_Death.ScoreboardCondition}]
		);
	
	for(var i = 1; i <= 2; ++i)
	{
		var team_id = CdsTeamID(i);
		Scoreboard->NewEntry(team_id, GetTaggedTeamName(i));
		Scoreboard->SetData(team_id, "team", "", team_id);
		Scoreboard->SetData(team_id, "score", 0);
	}
	
	Z4LeagueSystemInitScoreboard();
}

global func AddSmokeScreen(){++Caedes_smoke_count;}
global func RemoveSmokeScreen(){--Caedes_smoke_count;}

global func CaedesDoWealth(plr, amount)
{
	SetWealth(plr, Min(Caedes_MaxWealth, GetWealth(plr) + amount));
}

global func DoPlrScore(int plr, int amount)
{
	Caedes_player_round_score[plr] += amount;
	
	CaedesDoWealth(plr, amount);
	Sound("UI::Cash", false, 50, plr);
	Caedes_player_score[plr] += amount;
	Scoreboard->SetPlayerData(plr, "score", Format("<c 999955>%d</c>", Caedes_player_score[plr]), Caedes_player_score[plr]);
	
	{ // scope message
		var x = -15;
		var y = 100;
		var flm = CreateObject(FloatingMessage, x - GetX(), y - GetY(), plr);
		flm->SetMessage(Format("+%0d", amount));
		flm->SetColor(255, 255, 0);
		flm->FadeOut(5, 20);
		flm->SetYDir(-2);
		flm->SetCategory(flm->GetCategory() | C4D_Parallax | C4D_IgnoreFoW);
		flm.Parallaxity = [0, 0];
		flm.Visibility = VIS_Owner;
	}
}

global func GetPlrScore(int plr){return Caedes_player_score[plr];}

global func AddViewPoint()
{
	if(!this) return;
	PushBack(Caedes_ViewPoints, this);
	AddEffect("RemoveFromViewPoints", this, 1, 0, this);
}

func FxRemoveFromViewPointsStop(target, effect, reason, temp)
{
	if(temp) return;
	var i = GetIndexOf(Caedes_ViewPoints, this);
	if(i != -1)
	{
		RemoveArrayIndex(Caedes_ViewPoints, i, true);
	}
}

global func FxSchedNewRoundTimer()
{
	RoundHelper->NewRound();
	return -1;
}

global func FxGlobalVisibilityCheckTimer(t, effect, time)
{
	for(var i = GetLength(Caedes_ViewPoints)-1; i >= 0; --i)
	{
		if(!Caedes_ViewPoints[i])
			RemoveArrayIndex(Caedes_ViewPoints, i, true);
	}
	var unmarked = [];
	var all = Caedes_ViewPoints[:];
	
	for(var o in all)
	{
		o.Visibility = (VIS_Owner | VIS_Allies | VIS_God);
	}
	
	var marked = [];
	for(var i = 0; i < GetPlayerCount(); ++i)
	{
		var p = GetPlayerByIndex(i);
		var c = GetCursor(p);
		if(!c) continue;
		PushBack(all, c);
		c.Visibility = (VIS_Owner | VIS_Allies | VIS_God);
		
		if(GetEffect("*Visible*", c))
			PushBack(marked, c);
	}
	unmarked = all[:];
	
	
	
	while(GetLength(unmarked) > 0)
	{
		var c = PopBack(unmarked);
		for(var obj in all)
		{
			if(IsAllied(c->GetOwner(), obj->GetOwner())) continue;
			if(!PathFree(c->GetX(), c->GetY(), obj->GetX(), obj->GetY())) continue;
			
			if(Caedes_smoke_count > 0)
			{
				if(FindObject(Find_OnLine(c->GetX(), c->GetY(), obj->GetX(), obj->GetY()), Find_ID(SmokeGrenade_SmokeScreen))) continue;
			}
			
			if(!IsValueInArray(marked, obj)) PushBack(marked, obj);
			if(!IsValueInArray(marked, c)) PushBack(marked, c);
			RemoveArrayValue(unmarked, obj);
			RemoveArrayValue(unmarked, c);
			break;
		}
	}
	
	for(var obj in marked)
	{
		obj.Visibility = VIS_All;
	}
}

global func CaedesSetBombingTeam(int to)
{
	Caedes_BombingTeam = to;
}

global func CaedesSetBombPlanted(bool planted)
{
	Caedes_IsBombPlanted = planted;
	
	if(Caedes_IsBombPlanted)
	{
		CreateObject(Symbol_BombPlanted, 250 - GetX(), 300 - GetY(), NO_OWNER);
		RemoveAll(Find_ID(HUD_CaedesTimer));
	}
	
	for(var i = 0; i < GetPlayerCount(); ++i)
	{
		var p = GetPlayerByIndex(i);
		var c = GetCursor(p);
		if(!c) continue;
		c->~UpdateHUD();	
	}
}

global func CaedesBombExploded(bool exploded)
{
	CaedesSetBombPlanted(false);
	
	if(exploded)
		RoundHelper->TeamWonRound(Caedes_BombingTeam);
	else 
	{
		CreateObject(Symbol_BombDefused, 250 - GetX(), 300 - GetY(), NO_OWNER);
		RoundHelper->TeamWonRound(3 - Caedes_BombingTeam);
	}
	
}

global func CaedesTimeRanOut()
{
	var team = 3 - Caedes_BombingTeam;
		
	for(var i = 0; i < GetPlayerCount(); ++i)
	{
		var plr = GetPlayerByIndex(i);
		if(GetPlayerTeam(plr) == team) Symbol_InfoDefendedTheObjects->ShowFor(plr);
		else Symbol_InfoFailedToPlant->ShowFor(plr);
	}
	
	RoundHelper->TeamWonRound(team);
}

global func FxCaedesGameCounterStart(_, effect, temp)
{
	if(temp) return;
	effect.counter = 0;
	effect.active = false;
}

global func FxCaedesGameCounterTimer(_, effect, time)
{
	if(!effect.active) return 1;
	++effect.counter;
	return 1;
}

global func StopGameCounter(bool do_stop)
{
	var e = GetEffect("CaedesGameCounter", nil);
	if(!e) // first start
	{
		e = AddEffect("CaedesGameCounter", nil, 1, 36, nil);
	}
	e.active = !do_stop;
	return 1;
}

global func GetGameCounter() // in seconds
{
	var e = GetEffect("CaedesGameCounter", nil);
	if(!e) // first start
	{
		return 0;
	}
	return e.counter;
}

global func EndGame()
{
	StopGameCounter(true); // if still running
	
	AddEffect("IsGameOver", nil, 1, 0, nil);
	
	// eliminate losing team!
	var winner = 0;
	if(Caedes_team_score[1] > Caedes_team_score[2]) winner = 1;
	else if(Caedes_team_score[2] > Caedes_team_score[1]) winner = 2;
	var msg = "";
	if(winner == 0) msg = "@$Draw$|";
	else msg = Format("@$TeamWon$|", winner, GetTaggedTeamName(winner));
	
	// get all winning players and sort them by score, eliminate rest
	var winning = [];
	var max_name_length = 1;
	var plr_cnt = GetPlayerCount();
	for(var i = plr_cnt - 1; i >= 0; --i)
	{
		var p = GetPlayerByIndex(i);
		
		// add evaluation data to scoreboard
		AddEvaluationData(Z4GetPlrLeagueRank(p), GetPlayerID(p));
		
		if(winner)
		{
			if(GetPlayerTeam(p) != winner) {EliminatePlayer(p); continue;}
		}
		var name_length = GetLength(GetTaggedPlayerName(p));
		if(name_length > max_name_length) max_name_length = name_length;
		PushBack(winning, {plr = p, score = Caedes_player_score[p], kills = Caedes_player_kills[p], deaths = Caedes_player_deaths[p]});
	}
	
	// sort array - insertion sort
	var sorted = [];
	var l = GetLength(winning);
	for(var t = l - 1; t >= 0; --t)
	{
		var min = nil, min_i = nil;
		for(var i = l - 1; i >= 0; --i)
		{
			if(!winning[i]) continue;
			if((min != nil) && (winning[i].score <= min)) continue;

			min = winning[i].score;
			min_i = i;
		}
		if(min == nil) break;
		PushBack(sorted, winning[min_i]);
		winning[min_i] = nil;
	}	
	
	// print
	for(var p in sorted)
	{
		var name = GetTaggedPlayerName(p.plr);
		var len = GetLength(name);
		// fill with spaces
		while(len < max_name_length) {name = Format("%s ", name); ++len;}
		msg = Format("%s|%s ~ <c eeee66>%3d</c> [%2d:%2d]", msg, name, p.score, p.kills, p.deaths);
	}
	
	CustomMessage(msg, nil, nil, 0, 200, 0xffffff, GUI_MenuDeco, CaedesLogo, MSG_HCenter | MSG_NoLinebreak);
	
	// make scoreboard visible
	DoScoreboardShow(true, nil);
	
	Schedule(nil, "GameOver()", 35*3);
}

global func GetTitleDef(obj)
{
	if(obj->~TitleDef()) return obj->~TitleDef();
	return obj;
}

global func ExecuteHalftime()
{
	var switch_sides = GameCall("SwitchSides");
	CreateObject(Symbol_Halftime, 500, 300, NO_OWNER);
	Sound("Halftime");
	if(switch_sides) AddEffect("AnnounceSideSwitch", nil, 1, (36 * 3) / 2, nil);
}

global func FxAnnounceSideSwitchTimer()
{
	CreateObject(Symbol_SwitchingSides, 500, 400, NO_OWNER);
	Sound("SwitchingSides");
	return -1;
}

global func AnnounceLastRound()
{
	CreateObject(Symbol_LastRound, 500, 400, NO_OWNER);
	Sound("LastRound");
	return -1;
}

global func ShowRoundOverMessage()
{

	var msg = "";
	msg = Format("$RoundStats$");
	
	// get round-scores of all players and put in array
	var scores = [];
	var max_name_length = 1;
	var plr_cnt = GetPlayerCount();
	for(var i = plr_cnt - 1; i >= 0; --i)
	{
		var p = GetPlayerByIndex(i);
		var name_length = GetLength(GetTaggedPlayerName(p));
		if(name_length > max_name_length) max_name_length = name_length;
		
		PushBack(scores, {plr = p, score = Caedes_player_round_score[p], damage = Caedes_player_round_damage[p]});
	}
	
	// sort array - insertion sort
	var sorted = [];
	var l = GetLength(scores);
	for(var t = l - 1; t >= 0; --t)
	{
		var min = nil, min_i = nil;
		for(var i = l - 1; i >= 0; --i)
		{
			if(!scores[i]) continue;
			if((min != nil) && (scores[i].score <= min)) continue;

			min = scores[i].score;
			min_i = i;
		}
		if(min == nil) break;
		PushBack(sorted, scores[min_i]);
		scores[min_i] = nil;
	}	
	
	// print
	for(var p in sorted)
	{
		var name = GetTaggedPlayerName(p.plr);
		var len = GetLength(name);
		// fill with spaces
		while(len < max_name_length) {name = Format("%s ", name); ++len;}
		msg = Format("%s|%s ~ <c eeee66>$%3d</c> [DMG %2d]", msg, name, p.score, p.damage/1000);
	}
	
	if(GetMaximumRoundTime())
	{
		var time = Max(0, GetMaximumRoundTime() - GetGameCounter());
		msg = Format("%s||$TimeLeft$", msg, time/60, time%60);
	}
	CustomMessage(msg, nil, nil, 0, 200, 0xffffff, GUI_MenuDeco, CaedesLogo, MSG_HCenter | MSG_NoLinebreak);
	Schedule(nil, "CustomMessage(\"\", nil, nil, 0, 200, 0xffffff, GUI_MenuDeco, CaedesLogo, MSG_HCenter | MSG_NoLinebreak)", 35 * 4);
}