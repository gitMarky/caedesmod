/**
	BigBomb
	A massive bomb.
*/

local Name = "$Name$";
local Description = "$Description$";

local counter;

local Collectible = 1;
local team;
local bar;
local time_to_blow_up;

local last_interact_clonk;

static const BigBomb_MaxCounter_Plant = 150; // 5 * 30
static const BigBomb_MaxCounter_Defuse = 360;

func HasVeryLowPriority(){return true;}
func IsWeapon(){return true;}

func NoRemove(){return true;} // dont remove on Clonk death
func OnNewRound(){RemoveObject();}

func Departure(from)
{
	
	ScheduleCall(this, "DepartureEx", 1, 0, from);
}

func DepartureEx(from)
{
	if(from)
		if(Stuck()) SetPosition(from->GetX(), from->GetY());
	SetSpeed(0, 0);
}

func RejectEntrance(to)
{	
	if(team != nil)
	if(GetPlayerTeam(to->GetOwner()) != team) return true;
	
	return false;
}

public func Initialize()
{
	time_to_blow_up = 35 * 36;
	AddTimer("Check", 30);
	CaedesSetBombPlanted(false);
	AddEffect("MRotate", this, 1, 5, this);
}

func FxMRotateTimer(target, effect, time)
{
	this.MeshTransformation = Trans_Rotate(time, 0, 1, 0);
}

func Hit()
{
	Sound("Objects::Clonk");
}

func Interact(object clonk)
{
	if(Contained())
	{
		if(clonk != Contained()) return true;
	}
	else if(!this.Collectible)
	{
		if(GetPlayerTeam(clonk->GetOwner()) == team) return true;
	}
	
	last_interact_clonk = clonk;
}

func Check()
{
	if(this.Collectible)
	{
		if(!Exec())
		{
			if(this.bar)
				this.bar->Close();
			counter = 1;
			if(last_interact_clonk)
				RemoveEffect("ResetView", last_interact_clonk);
			last_interact_clonk = nil;
			return;
		}	
	}
}

func FxUpdateBarTimer(target, effect, time)
{
	if(!this.bar) return -1;
	this.bar->SetValue(time);
}

func Exec()
{
		if(!Contained()) return false;
		if(Contained() != last_interact_clonk) return false;
		if(!FindObject(Find_Distance(25), Find_ID(BigBomb_BombZone))) return false;
		
		//var old = GetEffect("OneSecondDelay", Contained()) || (counter > 1);
		
		++counter;
			
		if(this.bar)
		{
			//this.bar->SetValue(counter);
		}
		else
		{
			this.bar = Contained()->CreateProgressBar(GUI_RingProgressBar, BigBomb_MaxCounter_Plant - counter*30, 0, 35, Contained()->GetOwner(), nil, VIS_Owner | VIS_Allies);
			AddEffect("UpdateBar", this, 1, 5, this);
		}
		
		if(counter*30 >= BigBomb_MaxCounter_Plant)
		{
			RemoveEffect("ResetView", Contained());
			
			if(this.bar) this.bar->Close();
			DoPlrScore(Contained()->GetOwner(), 15);
			Message("<c ff0000>!!!</c>");
			CaedesSetBombPlanted(true);
			this.Collectible = false;
			Exit();
			AddEffect("BlowUpTimer", this, 1, 5, this);
			RemoveTimer("Check");
			MakeDefusable(BigBomb_MaxCounter_Defuse, team);
			Sound("Fire::Fuse");
		}
		else
		{
			LessenViewRange(Contained());
		}
	return true;
}

func LessenViewRange(obj)
{
	var plr = obj->GetOwner();
	var e = GetEffect("ResetView", obj);
	if(!e)
	{
		e = AddEffect("ResetView", obj, 1, 35, nil, BigBomb);
		SetPlayerZoomByViewRange(plr, CAEDES_ViewRange / 3, CAEDES_ViewRange / 3, PLRZOOM_LimitMax);
	}
	e.plr = plr;
	e.t = 2;
}

func FxResetViewTimer(target, effect, time)
{
	effect.t -= 1;
	if(effect.t == 0) return -1;
	return 1;
}

func FxResetViewStop(target, effect, reason, temp)
{
	if(temp) return;
	
	SetPlayerZoomByViewRange(effect.plr, CAEDES_ViewRange, CAEDES_ViewRange, PLRZOOM_LimitMax);
	SetPlayerZoomByViewRange(effect.plr, CAEDES_ViewRange, CAEDES_ViewRange);
}

func FxBlowUpTimerStart(object target, proplist effect, temp)
{
	if (temp) return;
	var colors = [RGB(200, 0, 0), RGB(0, 200, 100)];
	var text = ["$Defuse$", "$Defend$"];
	effect.bars = [];
	for (var i = 0; i < GetPlayerCount(); ++i)
	{
		var plr = GetPlayerByIndex(i);
		var ok = 0;
		if (GetPlayerTeam(plr) == this.team) ok = 1;
		var hud_bar = CreateProgressBar(GUI_HUDProgressBar, time_to_blow_up, 0, nil, plr, nil, VIS_Owner, {color = colors[ok], text = text[ok]});
		PushBack(effect.bars, hud_bar);
	}
}

func FxBlowUpTimerTimer(target, effect, time)
{
	for (var hud_bar in effect.bars)
		hud_bar->SetValue(time);
		
	if(time >= time_to_blow_up)
	{
		Boom();
		return -1;
	}
}

func FxBlowUpTimerStop(object target, proplist effect, wut, temp)
{
	if (temp) return;
	for (var hud_bar in effect.bars)
		hud_bar->Close();
}

func Boom()
{
	CaedesBombExploded(true);
	
	var range = 100;
	
	for(var obj in FindObjects(Find_Distance(range), Find_OCF(OCF_Alive), Find_NoContainer()))
	{	
		var dmg = range;
		if(!PathFree(GetX(), GetY(), obj->GetX(), obj->GetY()))
			dmg = range - ObjectDistance(this, obj);
			
		DoDmg(dmg, nil, obj, nil, nil, this, GetID());
	}

	Schedule(nil, Format("BigBomb->CreateBigBlastParticles(%d, %d)", GetX(), GetY()), 1, 10);
	Schedule(nil, "Sound(\"BombExplosion\")", 5, 0);
	Schedule(nil, Format("ExplosionEffect(70, %d, %d)", GetX(), GetY()), 11,0);
	ShakeViewport(70);
	RemoveObject();
}

func CreateBigBlastParticles(x, y)
{
	var particles = 
	{
		Size = PV_KeyFrames(0, 0, 0, 100, PV_Random(200, 400), 1000, 100),
		Alpha = PV_KeyFrames(0, 0, 255, 1000, 0),
		BlitMode = GFX_BLIT_Additive
	};
	CreateParticle("ExploLens", x, y, 0, 0, PV_Random(20, 25), particles, 1);
}

func OnObjectDefusing(obj, counter)
{
	LessenViewRange(obj);
}

func RejectDefusingBy(object clonk)
{
	return clonk != last_interact_clonk;
}

func OnRefuseAbort(obj)
{
	if(obj)
		if(GetEffect("ResetView", obj))
			RemoveEffect("ResetView", obj);
}

func OnDefused(obj)
{	
	if(obj) DoPlrScore(obj->GetOwner(), 15);
	Schedule(this, "Smoke(0, 0, 5)", 1, 30);
	Sound("UI::Click");
	CaedesBombExploded(false);
	RemoveEffect("BlowUpTimer", this);
	return true;
}

global func MakeDefusable(int def_time, int my_team)
{
	if(!this) return;
	
	var e = AddEffect("IsDefusable", this, 1, 20, this);
	e.needed_time = def_time;
	e.team = my_team;
	e.counter = 0;
}

global func FxIsDefusableTimer(target, effect, time)
{
	var clonk = nil;
	for(var obj in FindObjects(Find_Distance(10), Find_OCF(OCF_CrewMember), Find_OCF(OCF_Alive), Find_NoContainer()))
	{
		if(effect.team)
			if(GetPlayerTeam(obj->GetOwner()) == effect.team) continue;
		if(this->~RejectDefusingBy(obj)) continue;
		
		clonk = obj;
		break;
	}
	
	if(!clonk)
	{
		if(effect.bar)
			effect.bar->Close();
		effect.bar = nil;
		
		if(effect.last_clonk || effect.counter > 0)
			this->~OnRefuseAbort(effect.last_clonk);
		effect.last_clonk = nil;
		effect.counter = 0;
		return 1;
	}
	
	effect.last_clonk = clonk;
	effect.counter += effect.Interval;
	this->~OnObjectDefusing(clonk, effect.counter);
	if(!this) return;
	
	if(effect.bar)
	{
		effect.bar->SetValue(effect.counter);
	}
	else
	{
		effect.bar = this->CreateProgressBar(GUI_RingProgressBar, effect.needed_time, effect.counter, 60, clonk->GetOwner(), nil, VIS_Owner | VIS_Allies);
	}
	
	if(effect.counter < effect.needed_time) return 1;
	this->~OnDefused(clonk);
	return -1;
}

global func FxIsDefusableStop(target, effect, cause, temp)
{
	if(temp) return;
	if(effect.bar)
		effect.bar->Close();
}