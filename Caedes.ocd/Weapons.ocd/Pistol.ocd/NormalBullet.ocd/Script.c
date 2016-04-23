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
	/*var self = this;
	
	if(!instant)
	{
		SetXDir(0);
		SetYDir(0);
	
		if(nextX)
		{
			var x = GetX(), y = GetY();
			var a = Angle(lastX, lastY, nextX, nextY);
			var max = Max(Abs(GetXDir()/10),AbsY(GetYDir()/10));
			for(var cnt = 0; cnt < max; cnt += 2)
			{
				nextX = Sin(a, cnt);
				nextY = -Cos(a, cnt);
				if(GBackSolid(lastX + nextX - x, lastY + nextY - y))
				{
					SetPosition(lastX + nextX, lastY + nextY);
					if(trail)
						trail->Traveling();
					break;
				}
			}
		}
			
		DoHitCheckCall();
	}
	
	if(self)
	{
		Sound("Objects::Weapons::Musket::BulletHitGround?");
		CreateImpactEffect(Max(5, damage*2/3));
	  	
	  	RemoveObject();
	}*/
	
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
	fx.dist = dist;
	fx.dmg = dmg;
	fx.shooter = shooter;
	fx.particle_ray = 
		{
			Size = 3,
			Alpha = PV_Linear(255,0),
			BlitMode = GFX_BLIT_Additive,
			Rotation = fx.a / 100,
			G = 200,
			B = 100,
		};
	
	/*
	// set position to final point
	var x_p = GetX();
	var y_p = GetY();
	
	var t_x = GetX() + Sin(angle, bulletRange, 100);
	var t_y = GetY() - Cos(angle, bulletRange, 100);
	
	var coords = PathFree2(x_p, y_p, t_x, t_y);
	
	if(!coords) // path is free
		SetPosition(t_x, t_y);
	else SetPosition(coords[0], coords[1]);
		
	// we are at the end position now, check targets
	var hit_object = false;
	for (obj in FindObjects(
								Find_OnLine(x_p - GetX(), y_p - GetY(), 0, 0),
								Find_NoContainer(),
								//Find_Layer(GetObjectLayer()),
								//Find_PathFree(target),
								Find_Exclude(shooter),
								Sort_Distance(x_p - GetX(), y_p - GetY())
							))
	{
		if (obj->~IsProjectileTarget(this, shooter) || obj->GetOCF() & OCF_Alive)
		{
			var objdist = Distance(x_p, y_p, obj->GetX(), obj->GetY());
			SetPosition(x_p + Sin(angle, objdist, 100), y_p - Cos(angle, objdist, 100));
			var self = this;
			HitObject(obj, true);
			hit_object = true;
			break;
		}
	}
	
	// at end position now
	for(var obj in FindObjects(Find_OnLine(x_p - GetX(), y_p - GetY()), Find_Func("IsProjectileInteractionTarget")))
	{
		obj->~OnProjectileInteraction(x_p, y_p, angle, shooter, damage);
	}
	
	if(!shooter.silencer)
	{
		var particle_ray = 
		{
			Size = 6,
			Alpha = PV_Linear(255,0),
			BlitMode = GFX_BLIT_Additive,
			Rotation = angle / 100,
			B = 150
		};
		DrawParticleLine("RaySpark", x_p - GetX(), y_p - GetY(), 0, 0, 4, 0, 0, 3, particle_ray);
	}
	
	var self = this;
	if(!hit_object)
	{
		var hit = GBackSolid(Sin(angle, 2, 100), -Cos(angle, 2, 100));
		
		if(hit)
			Hit();
	}
	if(self) RemoveObject();*/
}

func FxTravelStart(object target, proplist fx, int temporary)
{
	fx.ox = GetX();
	fx.oy = GetY();
}

func FxTravelTimer(object target, proplist fx, int time)
{
	var tx = GetX() + Sin(fx.a, speed, 100);
	var ty = GetY() + -Cos(fx.a, speed, 100);
	
	var coords = PathFree2(GetX(), GetY(), tx, ty);
	var hit = false;
	var objhit = false;
	
	if(coords)
	{
		tx = coords[0];
		ty = coords[1];
		hit = true;
	}
	
	for (var obj in FindObjects(
								Find_OnLine(tx - GetX(), ty - GetY(), 0, 0),
								Find_NoContainer(),
								//Find_Layer(GetObjectLayer()),
								//Find_PathFree(target),
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
		DrawParticleLine("RaySpark", 0, 0, fx.ox - GetX(), fx.oy - GetY(), 1, 0, 0, 10, fx.particle_ray);
		/*DrawParticleLine("RaySpark", 0, 0, fx.ox - GetX(), fx.oy - GetY(), 1, 0, 0, 10, fx.particle_ray);
		DrawParticleLine("RaySpark", 0, 0, fx.ox - GetX(), fx.oy - GetY(), 1, 0, 0, 10, fx.particle_ray);
		DrawParticleLine("RaySpark", 0, 0, fx.ox - GetX(), fx.oy - GetY(), 1, 0, 0, 10, fx.particle_ray);
		DrawParticleLine("RaySpark", 0, 0, fx.ox - GetX(), fx.oy - GetY(), 1, 0, 0, 10, fx.particle_ray);
		DrawParticleLine("RaySpark", 0, 0, fx.ox - GetX(), fx.oy - GetY(), 1, 0, 0, 10, fx.particle_ray);*/
	}
	
	fx.ox = GetX();
	fx.oy = GetY();
	
	if(hit)
	{
		Hit();
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