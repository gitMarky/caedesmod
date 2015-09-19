
static round_data;
static current_step;

static func_counter;
static goal;
static PLAYER;
static purchaseable;

static tutorial_paused;

static const STD_TIME = 90;

global func GetMaximumGameTime(){return 0;}

func RejectRoundOverMessage(){return true;}

func QueryRejectNewRound()
{
	var c = GetCrew(PLAYER);
	if(!c) return false;
	if(!c->Contained()) return true;
}

func QueryRejectRespawnPlayer(plr)
{
	return GetPlayerType(plr) == C4PT_Script;
}

protected func Initialize()
{
	SetTutorialMessagePos(MSG_HCenter, 0, 200, nil);
	
	SmoothLandscape();
	
	purchaseable = [];
	round_data = 
	[
	 {init = Scenario.WelcomeInit, start = Scenario.WelcomeBegin, check = Scenario.WelcomeCheck, end = Scenario.WelcomeEnd, txt = "$WelcomeInfo$"},	
	 {init = Scenario.Pistol1, start = Scenario.PistolBegin, check = Scenario.PistolCheck, end = Scenario.PistolEnd, txt = "$PistolInfo$"},	
	 {init = Scenario.Shotgun1, start = Scenario.ShotgunBegin, check = Scenario.ShotgunCheck, end = Scenario.ShotgunEnd, txt = "$ShotgunInfo$"},	
	 {init = Scenario.Sniper1, start = Scenario.SniperBegin, check = Scenario.SniperCheck, end = Scenario.SniperEnd, txt = "$SniperInfo$"},	
	 {init = Scenario.MGL1, start = Scenario.MGLBegin, check = Scenario.MGLCheck, end = Scenario.MGLEnd, txt = "$MGLInfo$"},	
	 {init = Scenario.Nades1, start = Scenario.NadesBegin, check = Scenario.NadesCheck, end = Scenario.NadesEnd, txt = "$NadesInfo$"},	
	 {init = Scenario.BigBomb1, start = Scenario.BigBombBegin, check = Scenario.BigBombCheck, end = Scenario.BigBombEnd, txt = "$BigBombInfo$"},	
	 {init = Scenario.FalconPunch1, start = Scenario.FalconPunchBegin, check = Scenario.FalconPunchCheck, end = Scenario.FalconPunchEnd, txt = "$FalconPunchInfo$"},	
	 {init = Scenario.EpilogueInit, check = Scenario.EpilogueCheck, txt = "$EpilogueInfo$"}
	];
	// Goal.
	//CreateObject(Goal_Melee, 0, 0, NO_OWNER);
	CreateObject(Rule_KillLogs);
	goal = CreateObject(Goal_Script);
	CreateObject(SignPool, 100, 242)->SetClrModulation(RGBa(255,255,255,150));
	InitCaedes();
	
	var flag = CreateObject(Flagpole, 150, 230, -1);
	flag->SetFlagRadius(60);
	flag.Team = 1;
	
	tutorial_paused = false;
	
	AddEffect("TutorialCheck", nil, 1, 35, nil);
	
	return;
}

func InitializePlayer(plr)
{
	PLAYER = plr;
	return _inherited(plr, ...);
}

global func GetTeamSpawnPosition(int team)
{
	var pos = [{x = 150, y = 250}, {x = LandscapeWidth() - 100, y = 180}];
	pos = pos[team - 1];
	return pos;
}

global func GetPlayerRespawnPosition(plr)
{
	
	var pos = GetTeamSpawnPosition(GetPlayerTeam(plr));
	pos.x += RandomX(-30, 30);
	return pos;
}

global func FxTutorialCheckTimer(target, effect, time)
{
	if(GetCursor(PLAYER)->Contained()) return 1;
	if(tutorial_paused) return 1;

	var data = round_data[current_step];
	if(data->check())
	{
		if(data.end) data->end();
		Next(nil);
		++ current_step;
		tutorial_paused = true;
		if (goal) RoundManager()->RemoveRoundEndBlocker(goal);
		return 1;
	}
	
	if(data.next != nil)
	{
		func_counter -= effect.Interval - 1;
		if(func_counter <= 0)
		{
			data->next();
		}
	}
	return 1;
}

global func AllowPurchase(what)
{
	if(what == nil) {purchaseable = nil; return;}
	if(purchaseable == nil) purchaseable = [];
	PushBack(purchaseable, what);
}

global func Next(what, when)
{
	func_counter = when;
	round_data[current_step].next = what;
}


func OnRoundStart(int round)
{
	if (goal)
	{
		RoundManager()->RegisterRoundEndBlocker(goal);
		RoundManager()->RemoveRoundEndBlocker(GetRoundHelper());
	}

   //former func OnNewRound()

	purchaseable = [];
	
	SetWealth(PLAYER, 0);
	var c = GetCrew(PLAYER);
	for(var obj in FindObjects(Find_Container(c))) obj->RemoveObject();
	for(var obj in FindObjects(Find_Func("IsTutProp"))) obj->RemoveObject();
	for(var obj in FindObjects(Find_ID(BigBomb_BombZone))) obj->RemoveObject();
	
	if(current_step >= GetLength(round_data)) return;
	var data = round_data[current_step];
	
	goal.Description = data.txt;
	
	tutorial_paused = false;
	
	if(data.start) data->start();
	
	//if(!data.already)
	{
		data.already = true;
		if(data.init) data->init();
	}
}

func QueryRejectPurchase(player, item)
{
	if(purchaseable == nil) return false;
	return GetIndexOf(purchaseable, item) == -1;
}

global func CreateDummy(x, y)
{
	var o = CreateObject(DummyTarget, x, y, GetPlayerByIndex(0, C4PT_Script));
	return o;
}

global func DummyCount()
{
	return ObjectCount(Find_ID(DummyTarget));
}

global func CreateLine()
{
	CreateObject(SignLine, 447, 178);
}

func Clear()
{
	TutorialMessage("");
}

//////////////////// WELCOME
func WelcomeInit()
{
	TutorialMessage("$Welcome1$");
	Next(Scenario.WelcomeInit2, 2 * STD_TIME);
}

func WelcomeInit2()
{
	TutorialMessage("$Welcome2$");
	Next(Scenario.WelcomeInit3, 3 * STD_TIME);
}

func WelcomeInit3()
{
	TutorialMessage("$Welcome3$");
	Next(Scenario.Clear, 2 * STD_TIME);
}

func WelcomeBegin()
{
	SetWealth(PLAYER, 1000);
	AllowPurchase(nil);
}

func WelcomeCheck()
{
	return GetCrew(PLAYER)->ContentsCount() > 3;
}

func WelcomeEnd()
{
	TutorialMessage("$WelcomeEnd$");
}

/////////////////////// PISTOL

func PistolBegin()
{
	SetWealth(PLAYER, 60);
	AllowPurchase(Pistol);
	
	CreateDummy(400, 144)->UD();
	CreateDummy(520, 144)->LR();
	CreateDummy(584, 184)->LR()->UD();
}

func Pistol1()
{
	TutorialMessage("$Pistol1$");
	Next(Scenario.Pistol2, 2 * STD_TIME);
}

func Pistol2()
{
	Clear();
	if(!GetCursor(PLAYER)->FindContents(Pistol)) return Next(Scenario.Pistol2, 30);
	TutorialMessage("$Pistol2$");
	Next(Scenario.Pistol3, 2 * STD_TIME);
}

func Pistol3()
{
	TutorialMessage("$Pistol3$");
	Next(Scenario.Pistol4, 3 * STD_TIME);
}


func Pistol4()
{
	TutorialMessage("$Pistol4$");
	Next(Scenario.Clear, 3 * STD_TIME);
}

func PistolCheck() {return DummyCount() == 0;}

func PistolEnd()
{
	TutorialMessage("$PistolEnd$");
}

//////////////////// SHOTGUN
func ShotgunBegin()
{
	SetWealth(PLAYER, 60);
	AllowPurchase(Shotgun);
	
	CreateDummy(400, 50)->LR();
	CreateDummy(400, 100)->LR()->CreateContents(FlakVest);
	CreateDummy(400, 150)->LR();
}

func Shotgun1()
{
	TutorialMessage("$Shotgun1$");
	Next(Scenario.Shotgun2, 2 * STD_TIME);
}

func Shotgun2()
{
	Clear();
	if(!GetCursor(PLAYER)->FindContents(Shotgun)) return Next(Scenario.Shotgun2, 30);
	TutorialMessage("$Shotgun2$");
	Next(Scenario.Shotgun3, 2 * STD_TIME);
}

func Shotgun3()
{
	TutorialMessage("$Shotgun3$");
	Next(Scenario.Shotgun4, 3 * STD_TIME);
}

func Shotgun4()
{
	TutorialMessage("$Shotgun4$");
	Next(Scenario.Clear, 3 * STD_TIME);
}

func ShotgunCheck() {return DummyCount() == 0;}

func ShotgunEnd()
{
	TutorialMessage("$ShotgunEnd$");
}

//////////////////// SNIPER
func SniperBegin()
{
	SetWealth(PLAYER, 60);
	AllowPurchase(Rifle);
	
	CreateDummy(400, 144)->LR();
	CreateDummy(520, 144)->LR()->CreateContents(Helmet);
	CreateDummy(584, 184)->UD()->CreateContents(Helmet);
}

func Sniper1()
{
	TutorialMessage("$Sniper1$");
	Next(Scenario.Sniper2, 2 * STD_TIME);
}

func Sniper2()
{
	Clear();
	if(!GetCursor(PLAYER)->FindContents(Rifle)) return Next(Scenario.Sniper2, 30);
	TutorialMessage("$Sniper2$");
	Next(Scenario.Sniper3, 2 * STD_TIME);
}

func Sniper3()
{
	TutorialMessage("$Sniper3$");
	Next(Scenario.Clear, 3 * STD_TIME);
}


func SniperCheck() {return DummyCount() == 0;}

func SniperEnd()
{
	TutorialMessage("$SniperEnd$");
}

//////////////////// MGL
func MGLBegin()
{
	SetWealth(PLAYER, 140);
	AllowPurchase(GrenadeThrower);
	AllowPurchase(Camera);
	
	CreateLine();
	
	CreateDummy(400, 144)->LR()->UD();
	CreateDummy(520, 144)->LR();
	CreateDummy(584, 184)->LR();
}

func MGL1()
{
	TutorialMessage("$MGL1$");
	Next(Scenario.MGL2, 2 * STD_TIME);
}

func MGL2()
{
	Clear();
	if(!(GetCursor(PLAYER)->FindContents(GrenadeThrower) && GetCursor(PLAYER)->FindContents(Camera))) return Next(Scenario.MGL2, 30);
	TutorialMessage("$MGL2$");
	Next(Scenario.MGL3, 3 * STD_TIME);
}

func MGL3()
{
	TutorialMessage("$MGL3$");
	Next(Scenario.MGL4, 3 * STD_TIME);
}

func MGL4()
{
	TutorialMessage("$MGL4$");
	Next(Scenario.Clear, 3 * STD_TIME);
}


func MGLCheck() {return DummyCount() == 0;}

func MGLEnd()
{
	TutorialMessage("$MGLEnd$");
}

//////////////////// NADES
func NadesBegin()
{
	SetWealth(PLAYER, 1000);
	AllowPurchase(C4);
	AllowPurchase(IronBomb);
	
	CreateDummy(400, 144)->LR()->UD();
	CreateDummy(520, 144)->LR()->UD();
	CreateDummy(584, 184)->LR()->UD();
	CreateDummy(660, 296)->LR();
}

func Nades1()
{
	TutorialMessage("$Nades1$");
	Next(Scenario.Nades2, 2 * STD_TIME);
}

func Nades2()
{
	Clear();
	if(!GetCursor(PLAYER)->FindContents(C4)) return Next(Scenario.Nades2, 30);
	TutorialMessage("$Nades2$");
	Next(Scenario.Nades3, 3 * STD_TIME);
}

func Nades3()
{
	TutorialMessage("$Nades3$");
	Next(Scenario.Nades4, 3 * STD_TIME);
}

func Nades4()
{
	TutorialMessage("$Nades4$");
	Next(Scenario.Clear, 3 * STD_TIME);
}


func NadesCheck() {return DummyCount() == 0;}

func NadesEnd()
{
	TutorialMessage("$NadesEnd$");
}

//////////////////// BIG BOMB
func BigBombBegin()
{
	AllowPurchase(nil);
	SetWealth(PLAYER, 1000);
	CreateObject(BigBomb_BombZone, 408, 193, NO_OWNER);
	var f = FindObject(Find_ID(Flagpole));
	var b = CreateObject(BigBomb, f->GetX(), f->GetY(), NO_OWNER);
	b.team = 1;
}

func BigBomb1()
{
	TutorialMessage("$BigBomb1$");
	Next(Scenario.BigBomb2, 2 * STD_TIME);
}

func BigBomb2()
{
	
	TutorialMessage("$BigBomb2$");
	Next(Scenario.BigBomb3, 2 * STD_TIME);
}

func BigBomb3()
{
	if(!GetCursor(PLAYER)->FindContents(BigBomb)) return Next(Scenario.BigBomb3, 30);
	TutorialMessage("$BigBomb3$");
	Next(Scenario.BigBomb4, 3 * STD_TIME);
}

func BigBomb4()
{
	TutorialMessage("$BigBomb4$");
	Next(Scenario.BigBomb5, 2 * STD_TIME);
}

func BigBomb5()
{
	var bomb = FindObject(Find_ID(BigBomb));
	if(!bomb) return;
	if(bomb.Collectible) return Next(Scenario.BigBomb5, 30);
	TutorialMessage("$BigBomb5$");
	Next(Scenario.BigBomb6, 2 * STD_TIME);
	bomb.time_to_blow_up = 10 * 35;
}

func BigBomb6()
{
	TutorialMessage("$BigBomb6$");
	Next(Scenario.Clear, 3 * STD_TIME);
}

func BigBombCheck() {return ObjectCount(Find_ID(BigBomb)) == 0;}
global func CaedesBombExploded(asd){} // cathc

func BigBombEnd()
{
	TutorialMessage("$BigBombEnd$");
}

//////////////////// FALCON PUNCH
func FalconPunchBegin()
{
	CreateDummy(300, 260);
	CreateDummy(350, 200);
}

func FalconPunch1()
{
	TutorialMessage("$FalconPunch1$");
	Next(Scenario.FalconPunch2, 4 * STD_TIME);
}

func FalconPunch2()
{
	
	TutorialMessage("$FalconPunch2$");
	Next(Scenario.FalconPunch3, 3 * STD_TIME);
}

func FalconPunch3()
{
	TutorialMessage("$FalconPunch3$");
	Next(Scenario.FalconPunch4, 3 * STD_TIME);
}

func FalconPunch4()
{
	TutorialMessage("$FalconPunch4$");
	Next(Scenario.FalconPunch5, 2 * STD_TIME);
}

func FalconPunch5()
{
	TutorialMessage("$FalconPunch5$");
	Next(Scenario.Clear, 3 * STD_TIME);
}

func FalconPunchCheck() {return DummyCount() == 0;}

func FalconPunchEnd()
{
	TutorialMessage("$FalconPunchEnd$");
}

////////////// EPOLOGE

func EpilogueInit()
{
	TutorialMessage("$Epilogue1$");
	goal.fulfilled = true;
}

func EpilogueCheck()
{
	return false;
}
