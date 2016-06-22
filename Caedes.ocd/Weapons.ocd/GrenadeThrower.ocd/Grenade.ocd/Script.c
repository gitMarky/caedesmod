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

	var fx = AddEffect("SmokeTrail", this, 1, 1, this);
	fx.particles = 
	{
		Size = PV_KeyFrames(0, 0, 0, 100, PV_Random(2, 6), 1000, PV_Random(8, 10)),
		Alpha = PV_Linear(128, 0),
		ForceX = PV_Random(-1, 1, 10),
		ForceY = PV_Random(-1, 1, 10),
	};
	fx.x = GetX();
	fx.y = GetY();
}

func FxSmokeTrailTimer(object target, effect fx, int time)
{
	var xd = GetXDir(1), yd = GetYDir(1);
	SetR(Angle(0, 0, xd, yd));
	DrawParticleLine("SmokeDirty", AbsX(fx.x), AbsY(fx.y), 0, 0, 1, 0, 0, PV_Random(5, 10), fx.particles);
	fx.x = GetX();
	fx.y = GetY();
}

func Hit(int x, int y)
{
	var angle = Angle(0, 0, x, y);
	var speed = Distance(0, 0, x, y);
	var vec = GetSurfaceVector(0, 0);
	var axis = Angle(0, 0, vec[0], vec[1]);
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
	
	var rx = 20 * vec[0];
	var ry = 20 * vec[1];
	CreateParticle("StarSpark", PV_Random(-1, 1), PV_Random(-1, 1), PV_Random(rx - 30, rx + 30), PV_Random(ry - 30, ry + 30), PV_Random(3, 10), Particles_Glimmer(), 50); 
}

func BlowUp()
{
	var ray_particles = 
	{
		Size = 4,
		Alpha = PV_KeyFrames(0, 0, 255, 500, 255, 1000, 0),
		BlitMode = GFX_BLIT_Additive,
		G = 200,
		B = 100,
	};
	
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
		
		var fx = AddEffect("ShrapnelTrail", shrapnel, 1, 1, nil, GetID(), GetX(), GetY());
		fx.particles = {Prototype = ray_particles, Rotation = a};
		fx.x = GetX();
		fx.y = GetY();
		
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

func FxShrapnelTrailTimer(object target, effect fx)
{
	target->DrawParticleLine("RaySpark", 0, 0, target->AbsX(fx.x), target->AbsY(fx.y), 1, 0, 0, 10, fx.particles);
	fx.x = target->GetX();
	fx.y = target->GetY();
} 

