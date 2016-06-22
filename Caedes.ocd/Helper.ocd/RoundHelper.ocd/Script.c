/**
	RoundHelper
	Helps with stuff

	@author <Insert Author>
*/

local Name = "$Name$";
local Description = "$Description$";

static const CAEDES_ViewRange = 600;

static Caedes_Halftime_announced;
static Caedes_LastRound_announced;

static const Caedes_ShoppingTime = 175;

local ActMap = {
Attach = {
	Prototype = Action,
	Name = "Attach",
	Procedure = DFA_ATTACH,
	Length = 1,
	Delay = 0,
	FacetBase=1,
}
};

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Round tester dummy stuff

global func GetRoundHelper()
{
	var helper = FindObject(Find_ID(RoundHelper));
	if (!helper) helper = CreateObject(RoundHelper);
	return helper;
}

public func OnRoundReset(int counter)
{
	Log("[%d] RoundHelper preparing for round %d", FrameCounter(), counter);
	RoundManager()->RegisterRoundStartBlocker(this);

	DoRoundCountdown();
}

public func TriggerRoundStart()
{
	Log("[%d] +60 Removing myself from the blocker list", FrameCounter());
	RoundManager()->RemoveRoundStartBlocker(this);
}

public func OnRoundStart(int counter)
{
	Log("[%d] RoundHelper: Round %d starts", FrameCounter(), counter);
	RoundManager()->RegisterRoundEndBlocker(this);

	// anything left from last round?
	ClearRoundEffects();

	// starting again!
	StopGameCounter(false);

    // end the game if one team is eliminated
    if (DoTeamEliminationCheck()) return;

	// some announcing
	if (!AnnounceNewRound()) return;

	// respawn players
	RespawnPlayers();

	// remove weapons
    RemoveWeapons();

    // place goal object
    PlaceGoal();


	AddEffect("CheckNextRound", this, 1, 30, this);
	AddEffect("DelayGoSound", nil, 1, Caedes_ShoppingTime, nil, RoundHelper);
}

public func TriggerRoundEnd()
{
	RoundManager()->RemoveRoundEndBlocker(this);
}

public func OnRoundEnd(int counter)
{
	Log("[%d] RoundHelper: Round %d ends", FrameCounter(), counter);
	ClearRoundEffects();
	
	// stop timer!
	StopGameCounter(true);

	// round over message after score gain
	if(!GameCall("RejectRoundOverMessage"))
		ShowRoundOverMessage();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Caedes original scripts

func ContainedLeft(who)
{
	AttachTo(who, 0, GetX());
	return true;
}

func ContainedRight(who)
{
	AttachTo(who, GetX(), LandscapeWidth());
	return true;
}

func AttachTargetLost()
{
	if(!GetEffect("AutoAttach", this))
		AddEffect("AutoAttach", this, 1, 25, this);
}

func AttachTo(who, x, xr)
{
	if(!who) return false;
	if(GetEffect("AutoAttach", this))
		RemoveEffect("AutoAttach", this);
	
	var p = who->GetOwner();
	var obj = FindObject(Find_InRect(x - GetX(), -GetY(), xr - x, LandscapeHeight()), Find_OCF(OCF_CrewMember | OCF_Alive), Find_Allied(p), Find_NoContainer(), Sort_Distance());
	if(!obj)
	{
		obj = FindObject(Find_OCF(OCF_CrewMember | OCF_Alive), Find_Allied(p), Find_NoContainer(), Sort_Reverse(Sort_Distance()));
		if(!obj)
			return false;
	}
	SetAction("Attach", obj);
	return true;
}	

func FxAutoAttachTimer(target, effect, time)
{
	if(!Contents(0)) return -1;
	if(Random(2))
		ContainedRight(Contents(0));
	else ContainedLeft(Contents(0));
	return -1;
}

global func RespawnPlayer(int plr, object clnk)
{
	if(GameCall("QueryRejectRespawnPlayer", plr, clnk)) return;
	
	Sound("BallonPop", nil, nil, plr);
	if(clnk) clnk->Message("");
	GetRoundHelper()->CreateNewClonk(plr);
	GetRoundHelper()->InitPlayer(plr, clnk);
}

func CreateNewClonk(plr)
{
	var clnk = CreateObjectAbove(Clonk, 0 - GetX(), 0 - GetY(), plr);
	clnk->MakeCrewMember(plr);
	AddEffect("LogDamage", clnk, 1, 70, nil,  nil);
	SetCursor(plr, clnk, true);
}

func InitPlayer(plr, old)
{
	SetPlayerZoomByViewRange(plr, CAEDES_ViewRange / 2, CAEDES_ViewRange / 2, PLRZOOM_LimitMax);
	var x, y;
	if(old)
	{
		x = old->GetX();
		y = old->GetY();
	}
	else
	{
		x = LandscapeWidth() / 2;
		y = LandscapeHeight() / 2;
	}
	
	var clnk = GetCrew(plr);
	if(!clnk) {Log("Init player with no valid Clonk called"); return;} // wat
	clnk->SetLightRange(400, 10);
	ApplyPerks(plr);
	
	// TODO: replace with respawn container!
	//var obj = CreateObject(RoundHelper, x - GetX(), y - GetX());
	//clnk->Enter(obj);
	//if(old)
	//	AddEffect("AutoAttach", obj, 1, 40, obj);
}

func Ejection()
{
	// remove one frame later so that Clonk->Departure does not whine about "removed object used"..
	if(!ContentsCount())
		return ScheduleCall(this, "Remove", 1, 0);
}

func Remove() { return RemoveObject(); }

global func GetPlayerRespawnPosition(){return _inherited(...);}
global func GetTeamSpawnPosition(){return _inherited(...);}

func ClearRoundEffects()
{
	RemoveEffect("CheckNextRound", this);
	RemoveAll(Find_ID(HUD_CaedesTimer));
}

func TeamWonRound(int team)
{
    // TODO: this should not be possible anyway...
	// already over
	// if (IsRoundCountdown()) return;

	var players_winlose = [[],[]];

	// give moneos to winner team
	for(var i = 0; i < GetPlayerCount(); ++i)
	{
		var p = GetPlayerByIndex(i);
		if(GetPlayerTeam(p) != team) {PushBack(players_winlose[1], p); continue;}
		PushBack(players_winlose[0], p);
		DoPlrScore(p, 20);
	}

	// inc score
	Scoreboard->SetData(CdsTeamID(team), "score", ++Caedes_team_score[team]);

    // some stuff for the league!
	Z4PlayersWonRound(players_winlose[0], players_winlose[1]);
}

		c.MaxEnergy = 100 * 1000;
		AddEffect("HoldPlayersInPlace", c, 1, Caedes_ShoppingTime, nil, RoundHelper);
		fx.max_time = Caedes_ShoppingTime;
		var bombing_team = Scenario->~GetBombingTeam();
		if (bombing_team == nil)
		{
		}
		else
		{
			Caedes_BombingTeam = bombing_team;
		}
protected func FxCheckNextRoundEffect(string new_name, object target, proplist effect, var1, var2, var3, var4)
{
	if(new_name == "CheckNextRound") return -1;
}

protected func FxCheckNextRoundTimer(target, effect, time)
{
	var teams = [];
	// count all players that are not respawning
	for(var i = 0; i < GetPlayerCount(); ++i)
	{
		var player = GetPlayerByIndex(i);
		var crew = GetCrew(player);
		var team_index = GetPlayerTeam(player);
		if(!team_index) continue;
		if(!crew) continue;
		
		if(!crew->Contained())
			teams[team_index] = team_index;
	}

    // as long as both teams are alive do not start a new round
	var cnt = 0;
	var found_team = nil;
	for(var team in teams)
		if(team) {cnt += 1; found_team = team;}
	if(cnt >= 2) return 1;
	// in single player, if he has a team, prevent a new round from starting
	if((GetPlayerCount() <= 1) && found_team) return 1;
	
	// if anyone else does not want a new round to start, then prevent that
	// TODO: these objects should register themselves to the round manager instead
	if(GameCall("QueryRejectNewRound")) return 1;

	// ask goals if they prevent the start of a new round
	// TODO: these objects should register themselves to the round manager instead
	for(var obj in FindObjects(Find_Category(C4D_Goal)))
	{
		if(obj->~QueryRejectNewRound(found_team)) return 1;
	}
	
	// let the surviving team win
	if(found_team != nil)
		TeamWonRound(found_team);

	// actually start the new round.
    RoundManager()->RemoveRoundEndBlocker(this);
	
	return -1;
}

protected func FxTickNewRoundStart(object target, proplist effect, temp)
{
	if (temp) return;
	// Create progress bars for all players.
	effect.bars = [];
	effect.max_time = 4 * 35;
	for (var i = 0; i < GetPlayerCount(); ++i)
	{
		var plr = GetPlayerByIndex(i);
		var bar = CreateProgressBar(GUI_HUDProgressBar, effect.max_time, effect.max_time, nil, plr, nil, VIS_Owner, {color = RGB(100, 0, 100), text = "$NewRoundShort$"});
		PushBack(effect.bars, bar);
	}
}

protected func FxTickNewRoundEffect (string new_name, object target, proplist effect, var1, var2, var3, var4)
{
	if(new_name == "TickNewRound") return -1;
}

protected func FxTickNewRoundTimer(target, effect, time)
{
	var value = effect.max_time - time;

	if(Caedes_LastRound_announced || value <= 0)
	{
		target->TriggerRoundStart();
		return -1;
	}
		
	for (var bar in effect.bars)
	{
		bar->SetValue(value);
	}
		
	return 1;
}

protected func FxTickNewRoundStop(object target, proplist effect, reason, temp)
{
	if (temp) return;

	for (var bar in effect.bars)
		bar->Close();
}

func FxHoldPlayersInPlaceStart(target, fx, temp)
{
	if(temp) return;
	fx.X = target->GetX();
}

func FxHoldPlayersInPlaceTimer(target, fx, time)
{
	if (time > fx.max_time) return FX_Execute_Kill;
	target->SetPosition(fx.X, target->GetY());
	return FX_OK;
}

func FxDelayGoSoundTimer()
{
	Sound("Go", nil, 50);
	return -1;
}


private func DoTeamEliminationCheck()
{
	// first check whether only one team remaining (time out etc)
	var a_teams = [0, 0, 0, 0, 0];
	for (var pcnt = 0; pcnt < GetPlayerCount(); ++pcnt)
	{
		var plr = GetPlayerByIndex(pcnt);

		// also reset per-round data
		Caedes_player_round_score[plr] = 0;
		Caedes_player_round_damage[plr] = 0;

		var team;
		if(team = GetPlayerTeam(plr))
			++a_teams[team];
	}

	var team_count = 0;
	for (var ti = 0; ti < GetLength(a_teams); ++ti)
	{
		if(a_teams[ti]) ++team_count;
	}

	if (team_count == 1 // exactly one team remaining
    && (GetPlayerCount() < GetLength(Caedes_player_score))) // at least one player left
	{
		// end game instantly!
		EndGame();
		return true;
	}
	// end elim check
    return false;
}


private func AnnounceNewRound()
{
	if(GetMaximumGameTime())
	{
		var t = GetMaximumGameTime() - GetGameCounter();
		if(t < 0) t = 0;


		if((t < GetMaximumGameTime() / 2) && !Caedes_Halftime_announced)
		{
			Caedes_Halftime_announced = true;
			ExecuteHalftime();
		}

		if(t == 0)
		{
			// already announced?
			if(Caedes_LastRound_announced)
			{
				EndGame();
				return false;
			}

			Caedes_LastRound_announced = true;
			AnnounceLastRound();
		}
		CustomMessage(Format("$NewRound$", t / 60, t % 60), nil, nil, 0, 0, 0, 0, 0, 0);
	}
	else CustomMessage("$NewRoundShort$");
	
	return true;
}

private func RespawnPlayers()
{
	for(var i = 0; i < GetPlayerCount(); ++i)
	{
		var p = GetPlayerByIndex(i);
		CaedesDoWealth(p, 75);
		var c = GetCrew(p);
		if(!c) continue; // wat
		if(c->Contained()) c->Exit();

		AddEffect("NewRound", c, 1, 35*5, nil);

		var pos = GetPlayerRespawnPosition(p);
		c->SetPosition(pos.x, pos.y);
		c->DoEnergy(100);

		SetCursor(p, c);

		AddEffect("HoldPlayersInPlace", c, 1, Caedes_ShoppingTime, nil, RoundHelper);

		var obj;
		for(var t = c->ContentsCount(); obj = c->Contents(--t);)
		{
			obj->~OnNewRound();
		}


		SetPlayerZoomByViewRange(p, CAEDES_ViewRange, CAEDES_ViewRange, PLRZOOM_LimitMax);
		SetPlayerZoomByViewRange(p, CAEDES_ViewRange, CAEDES_ViewRange);

		c->~UpdateHUD();
		//AddEffect("LogDamage", c, 1, 70, nil,  nil);
		Scoreboard->SetPlayerData(p, "player_dead", "");
	
		var m = c->~GetMenu();
		if(m) m->~Close();
	}
}

private func RemoveWeapons()
{
	for(var obj in FindObjects(Find_NoContainer()))
	{
		if(obj->~IsWeapon() || obj->GetDefCoreVal("Projectile", "DefCore") == 1)
			obj->RemoveObject();
	}
}

private func PlaceGoal()
{
	// if bombing goal, place bomb
	if(ObjectCount(Find_ID(Goal_Destruction)))
	{
		Caedes_BombingTeam = 3 - Caedes_BombingTeam;

		// find random player of team
		var pos = GetTeamSpawnPosition(Caedes_BombingTeam);
		var bomb = CreateObject(BigBomb, pos.x, pos.y, NO_OWNER);
		bomb.team = Caedes_BombingTeam;

		// announce for planting team
		for(var i = 0; i < GetPlayerCount(); ++i)
		{
			var plr = GetPlayerByIndex(i);
			if(GetPlayerTeam(plr) != Caedes_BombingTeam) continue;
			Symbol_InfoYourTeamHasBomb->ShowFor(plr);
		}

		// timer
		CreateObject(HUD_CaedesTimer);
	}
}


private func DoRoundCountdown()
{
	if (!IsRoundCountdown()) AddEffect("TickNewRound", this, 1, 5, this);
}

public func IsRoundCountdown()
{
    return GetEffect("TickNewRound", this);
}
