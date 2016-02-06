/**
	Grenade
	Hits stuff and explodes.

*/

local Name = "$Name$";
local Description = "$Description$";
local Collectible = false;

local from_ID, user, damage, remaining_time;

local ActMap = {

	Travel = {
		Prototype = Action,
		Name = "Travel",
		Procedure = DFA_NONE,
		NextAction = "Travel",
		Length = 1,
		Delay = 0,
		FacetBase = 1,
	},
};

public func Fire(object shooter, int angle, int dev, int dist, int dmg, id weapon, range)
{
	from_ID = weapon;
	user = shooter;
	damage = dmg;
	
	SetR(angle);
	
	SetController(shooter->GetController());
	remaining_time = range / 5;
	//ScheduleCall(this, "BlowUp", remaining_time);

	angle *= 100;
	angle += RandomX(-dev, dev);
	
	SetAction("Travel");
	SetSpeed(Sin(angle, range * 5, 100), -Cos(angle, range * 5, 100), 100);

	AddEffect("SmokeTrail", this, 1, 1, this);
}

func FxSmokeTrailTimer()
{
	var xd = GetXDir(1), yd = GetYDir(1);
	SetR(Angle(0, 0, xd, yd));
	DrawLightningSmall(0, 0, -xd, -yd);
	if(!Random(3))
		DrawLightningSmall(0, 0, RandomX(-30, 30), RandomX(-30, 30));
}

func Hit(int x, int y)
{
	var angle = Angle(0, 0, x, y);
	var speed = Distance(0, 0, x, y);
	var vec = GetSurfaceVector(0, 0);
	var axis = GetAngleFromVector(vec);
	var new_angle = MirrorAngle(angle, axis);
	SetXDir(Sin(new_angle, speed) + RandomX(-50, 50), 100);
	SetYDir(-Cos(new_angle, speed) + RandomX(-50, 50), 100);
	
	//if(!FindObject(Find_AtPoint(), Find_OCF(OCF_Alive))) return BlowUp();
	
	/*var x2 = (x * 3) / 2, y2 = (y * 3)/2;
	if(GBackSolid(0, -5) || GBackSolid(0, 5))
	{
		y2 = -y2;
	}
	if(GBackSolid(5, 0) || GBackSolid(-5, 0))
	{
		x2 = -x2;
	}
	SetXDir(x2 + RandomX(-100, 100), 100);
	SetYDir(y2 + RandomX(-100, 100), 100);*/
	
	remaining_time = (remaining_time * 2) / 3;
	if(remaining_time < 10) remaining_time = 0;
	ScheduleCall(this, "BlowUp", Max(1, remaining_time - GetActTime()));
	
	Sound("Hits::Materials::Metal::DullMetalHit*");
	
	for(var i = 0; i < 15; ++i)
	{
		var x2 = RandomX(-20, 20);
		var y2 = RandomX(-20, 20);
		if(GBackSolid(x2, y2)) continue;
		
		DrawLightning(0, 0, x2, y2);
	}
}

func BlowUp()
{
	//var xd = GetXDir(100);
	//var yd = GetYDir(100);
	
	
	var xd = 0, yd = 0;
	var td = 20;
	if(GBackSolid(0, 5)) yd -= td;
	if(GBackSolid(0, -5)) yd += td;
	if(GBackSolid(5, 0)) xd -= td;
	if(GBackSolid(-5, 0)) xd -= td;
	
	var angle = 0;//Angle(0, 0, xd, yd, 10);
	var amount = damage/4;
	var deviation = 0;
	var deviation_step = 3600 / amount;
	
	for(var i = 0; i < amount; ++i)
	{
		var d = deviation;
		if(Random(2)) d = -d;
		deviation += deviation_step;
		var a = angle + d;
		
		var speed = 300 + Random(300);
		var x = Sin(a, speed * 8, 10);
		var y = -Cos(a, speed * 8, 10);
		
		if(GBackSolid(x / 100, y / 100))
		{
			x = -x;
			y = -y;
		}
		
		var shrapnel = CreateObject(Shrapnel);
		
		AddEffect("RndLightning", shrapnel, 1, 1, nil, GetID(), GetX(), GetY());
		//AddEffect("SmokeParticle", shrapnel, 1, 0, nil, GetID());
		shrapnel->SetController(GetController());
		//shrapnel->SetSpeed(xd/2 + Random(xd/2), yd/2 + Random(yd/2), 100);
		
		shrapnel->SetXDir(x, 100);
		shrapnel->SetYDir(y, 100);
		//shrapnel->SetXDir(RandomX(xd/2, xd) + RandomX(-50, 50), 100);
		//shrapnel->SetYDir(RandomX(yd/2, yd) + RandomX(-50, 50), 100);
		//shrapnel->SetSpeed(RandomX(-10,10), RandomX(-10, 10));
		shrapnel.from_ID = from_ID;
		shrapnel.flight_time = 2+Random(2);
		shrapnel.damage = 6;
		//CreateObject(BulletTrail)->Set(2,30,shrapnel);
		AddEffect("HitCheck2", shrapnel, 1,1, nil,nil, nil);
	}
	
	Sound("BlastMetal*");
	ExplosionEffect(5);
	RemoveObject();
}

func FxRndLightningStart(target, effect, temp, x, y)
{
	if(temp)return;
	effect.x = x;
	effect.y = y;
	effect.time = 1 + Random(10);
}

func FxRndLightningTimer(target, effect, time)
{
	DrawLightning(effect.x, effect.y, target->GetX(), target->GetY());
	effect.x = target->GetX();
	effect.y = target->GetY();
	
	if(time > effect.time) return -1;
	return 1;
}

func FxRndLightningStop(target, effect, cause, temp)
{
	if(temp) return;
	if(!target) return;
	DrawLightning(effect.x, effect.y, target->GetX(), target->GetY());
}
