/**
	Flare

*/

local Name = "$Name$";
local Description = "$Description$";
local Collectible = false;

local from_ID, user, damage, remaining_time;

local ActMap = {

	Travel = {
		Prototype = Action,
		Name = "Travel",
		Procedure = DFA_FLOAT,
		NextAction = "Travel",
		Length = 1,
		Delay = 0,
		FacetBase = 1,
	},
};

local SmokeTrailEffect = new Effect
{
	Construction = func ()
	{
		this.smoke = new Particles_Smoke() {
			ForceY = 0,
			Alpha = PV_Linear(50, 0)
		}; 
		this.flare = new Particles_Flash(20) {
			Stretch = 3000,
			Rotation = PV_Random(-180, 180),
			Phase = PV_Step(1, 0, 5),
			Attach = ATTACH_Front | ATTACH_MoveRelative,
			G = 64, B = 64
		};
		this.shockwave = new this.flare {
			Size = PV_Linear(80, 50), 
			Stretch = 0,
			Alpha = PV_Linear(100, 0)
		};
		this.x = this.Target->GetX();
		this.y = this.Target->GetY();
	},
	Timer = func (int time)
	{
		this.Target->DrawParticleLine2("SmokeDirty", this.x - this.Target->GetX(), this.y - this.Target->GetY(), 0, 0, 1, 1, 1, PV_Random(10, 20), this.smoke);
		this.Target->CreateParticle("FireSharp", 0, 0, 0, 0, PV_Random(10, 20), this.flare, 4);
		this.Target->CreateParticle("Shockwave", 0, 0, 0, 0, 5, this.shockwave, 1);
		this.x = this.Target->GetX();
		this.y = this.Target->GetY();
		
		if (time > 2 * this.Target.remaining_time)
			this.Target->RemoveObject();
		return FX_OK;
	}
};

public func Fire(object shooter, int angle, int dev, int dist, int dmg, id weapon, range)
{
	from_ID = weapon;
	user = shooter;
	damage = dmg;
	
	SetR(angle);
	
	SetController(shooter->GetController());
	//remaining_time = range / 5;
	ScheduleCall(this, "Remove", remaining_time);
	
	remaining_time = range;
	
	angle *= 100;
	angle += RandomX(-dev, dev);
	
	SetAction("Travel");
	SetComDir(COMD_None);
	SetSpeed(Sin(angle, range * 10, 100), -Cos(angle, range * 10, 100), 100);

	CreateEffect(SmokeTrailEffect, 1, 1);
	
	AddEffect("HitCheck2", this, 1,1, nil,nil, shooter);
	
	SetLightRange(10, 50);
}

func Remove(){RemoveObject();}

public func HitObject(object obj)
{
	if(!Hostile(GetController(), obj->GetOwner()))
	{
		// bounce off
		if(GetX() > obj->GetX())
			SetXDir(RandomX(100, 300), 100);
		else SetXDir(-RandomX(100, 300), 100);
		
		SetYDir(GetYDir(100) + RandomX(-200, 200), 100);
		ScheduleCall(this, "ReaddHitCheck", 5, 0);
		return true;
	}
	
	AddEffect("BurnedReceiveCrits", obj, 1, 2, nil, GetID());
	
	RemoveObject();
}

func FxBurnedReceiveCritsStart(target, effect, temp)
{
	if(temp) return;
	++target.receive_crits;
}

func FxBurnedReceiveCritsStop(target, effect, reason, temp)
{
	if(temp) return;
	--target.receive_crits;
}

func FxBurnedReceiveCritsTimer(target, effect, time)
{
	if(time > 36 * 5) return -1;
	
	if(time % 2 == 0)
	{
		var x = target->GetX();
		var y = target->GetY();
		if(time < 15)
		{
			CreateParticle("Fire", PV_Random(x - 8, x + 8), PV_Random(y - 5, y + 5), PV_Random(-1, 1), PV_Random(-2, 0), PV_Random(20, 40), Particles_Fire(), 5);
		}
		CreateParticle("StarSpark", PV_Random(x - 2, x + 2), PV_Random(y - 2, y + 2), PV_Random(-10, 10), PV_Random(-10, 0), PV_Random(10, 20), Particles_Glimmer(), 4);
	}
}

func ReaddHitCheck()
{
	if(GetEffect("HitCheck2", this)) return;
	AddEffect("HitCheck2", this, 1,1, nil,nil, user);	
}

func Hit(int x, int y)
{
	Sound("Hits::GeneralHit?");
	
	if(remaining_time - GetActTime() < 20)
	{
		RemoveObject();
		return true;
	}
	
	if(GBackSolid(5, 0) || GBackSolid(-5, 0)) SetXDir(-x, 100);
	if(GBackSolid(0, 5) || GBackSolid(0, -5)) SetYDir(-y, 100);
	SetXDir(GetXDir(100) + RandomX(-50, 50), 100);
	SetYDir(GetYDir(100) + RandomX(-50, 50), 100);
	SetR(Angle(0, 0, GetXDir(), GetYDir()));
}
