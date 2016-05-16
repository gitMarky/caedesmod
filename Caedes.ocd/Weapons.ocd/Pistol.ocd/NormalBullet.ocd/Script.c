/*-- Bullet --*/

local damage;
local from_ID;
local user;
local deviation;
local bulletRange;
local speed;

local lastX, lastY, nextX, nextY;
local trail;

local instant;

protected func Initialize()
{
	speed = 25;
	SetAction("Travel");
}

protected func Hit()
{
	Sound("Objects::Weapons::Musket::BulletHitGround?");
	CreateImpactEffect(Max(5, damage*2/3));
	RemoveObject();
}

func Remove()
{
	var self = this;
	DoHitCheckCall();
	if(self) RemoveObject();
}

public func Fire(object shooter, int angle, int dev, int dist, int dmg, id weapon, range)
{
	from_ID = weapon;
	user = shooter;
	damage = dmg;
	deviation = dev;
	bulletRange = range;
	
	instant = true;
		
	SetController(shooter->GetController());

	angle *= 100;
	angle += RandomX(-deviation, deviation);
	
	var fx = AddEffect("Travel", this, 1, 1, this);
	fx.a = angle;
	fx.range = range;
	fx.dmg = dmg;
	fx.shooter = shooter;
	fx.particle_ray = 
		{
			Size = 6,
			Alpha = PV_KeyFrames(0, 0, 255, 500, 255, 1000, 0),
			BlitMode = GFX_BLIT_Additive,
			Rotation = fx.a / 100,
			G = 200,
			B = 100,
		};
}

func FxTravelStart(object target, proplist fx, int temporary)
{
	fx.ox = GetX();
	fx.oy = GetY();
	
	fx.startx = GetX();
	fx.starty = GetY();
	fx.counter = 0;
}

func FxTravelTimer(object target, proplist fx, int time)
{
	var hit = false;
	var objhit = false;
	var erase = false;
	
	if (time > 5 && time%2==0)
		fx.counter++;

	var tx = GetX() + Sin(fx.a, speed, 100);
	var ty = GetY() + -Cos(fx.a, speed, 100) + fx.counter;
	
	/*if(Distance(fx.startx, fx.starty, tx, ty) > fx.range)
	{
		var curr = Distance(fx.startx, fx.starty, GetX(), GetY());
		tx = GetX() + Sin(fx.a, fx.range - curr, 100);
		ty = GetY() + -Cos(fx.a, fx.range - curr, 100);
		erase = true;
	}*/
	
	
	var coords = PathFree2(GetX(), GetY(), tx, ty);

	
	if(coords)
	{
		tx = coords[0];
		ty = coords[1];
		hit = true;
	}
	
	for (var obj in FindObjects(
								Find_OnLine(tx - GetX(), ty - GetY(), 0, 0),
								Find_NoContainer(),
								Find_Exclude(fx.shooter),
								Sort_Distance(tx - GetX(), ty - GetY())
							))
	{
		if(obj->~IsProjectileInteractionTarget())
		{
			obj->~OnProjectileInteraction(tx, ty, fx.a, fx.shooter, fx.dmg);
		}
		
		if (!objhit && (obj->~IsProjectileTarget(this, fx.shooter) || obj->GetOCF() & OCF_Alive))
		{
			var objdist = Distance(tx, ty, obj->GetX(), obj->GetY());
			SetPosition(tx + Sin(fx.a, objdist, 100), ty - Cos(fx.a, objdist, 100));
			HitObject(obj, true);
			objhit=true;
		}
	}
	
	
	SetPosition(tx, ty);
	if(time)
	{
		DrawParticleLine("RaySpark", 0, 0, fx.ox - GetX(), fx.oy - GetY(), 1, 0, 0, 5, fx.particle_ray);
	}
	
	fx.ox = GetX();
	fx.oy = GetY();
	
	if(hit)
	{
		Hit();
		return -1;
	}
	
	if(erase)
	{
		RemoveObject();
		return -1;
	}
}

func FxTravelStop(object target, proplist fx, int reason, bool temporary)
{

}

func CreateTrail()
{
	// neat trail
	trail = CreateObject(BulletTrail,0,0);
	trail->SetObjectBlitMode(GFX_BLIT_Additive);
	trail->Set(2, 125, this);
}

func FxPositionCheckTimer(target, effect, time)
{
	lastX = GetX();
	lastY = GetY();
	nextX = lastX + GetXDir()/10;
	nextY = lastY + GetYDir()/10;
}

func Traveling()
{
}

func TrailColor(int acttime){/*return 0x88fffffff;*/ return RGBa(255,255 - Min(150, acttime*20) ,75,255);}

public func HitObject(object obj, bool no_remove)
{
	if(obj.receive_crits > 0)
	{
		damage = (3 * damage) / 2;
		this.crit = true;
	}
	
	DoDmg(damage, nil, obj, nil, nil, this, from_ID);
	if(this.crit == true)
	{
		CreateCriticalHitMark(GetX(), GetY(), -1);
		Sound("Crit1");
	}
	CreateImpactEffect(Max(5, damage*2/3));
	
	if(!no_remove) RemoveObject();
}

// called by successful hit of object after from ProjectileHit(...)
public func OnStrike(object obj)
{
	if(obj->GetAlive())
		Sound("Hits::ProjectileHitLiving?");
	else
		Sound("Objects::Weapons::Musket::BulletHitGround?");
}



private func SquishVertices(bool squish)
{
	if(squish==true)
	{
		SetVertex(1,VTX_X,0,2);
		SetVertex(1,VTX_Y,0,2);
		SetVertex(2,VTX_X,0,2);
		SetVertex(2,VTX_Y,0,2);
	return 1;
	}

	if(squish!=true)
	{
		SetVertex(1,VTX_X,-3,2);
		SetVertex(1,VTX_Y,1,2);
		SetVertex(2,VTX_X,3,2);
		SetVertex(2,VTX_Y,1,2);
	return 0;
	}

}

local ActMap = {

	Travel = {
		Prototype = Action,
		Name = "Travel",
		Procedure = DFA_FLOAT,
		NextAction = "Travel",
		Length = 1,
		Delay = 1,
		FacetBase = 1,
		FacetCall="Traveling",
	},
};
local Name = "$Name$";
local Description = "$Description$";