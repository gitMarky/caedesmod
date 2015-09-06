/*-- Bullet --*/

local speed, from_ID, user, damage, deviation, range, dist;

protected func Initialize()
{
	speed = 1000;
	Sound("RocketFlight", 0, 100, nil, 1);
	dist = 25;
}

protected func Hit()
{
	DoExplode();
}

func DoExplode()
{
	for(var o in FindObjects(	Find_Distance(dist * 175 / 100),
								Find_NoContainer(),
								Find_Layer(this->GetObjectLayer()),
								Find_Or(Find_OCF(OCF_Alive), Find_Func("IsProjectileTarget", this)),
								Find_PathFree(this),
								Find_Exclude(this)))
	{
		var objdist = ObjectDistance(this, o);
		
		var angle = Angle(GetX(), GetY(), o->GetX(), o->GetY());
		
		var cat = o->GetCategory();
		
		var strength = 40;
		if(objdist > dist)
			strength = 20;
		
		if (cat & C4D_Living)
			o->Fling(Cos(angle - 90, strength), Sin(angle - 90, strength), 10, 0);
		
		DoDmg(damage - Min(objdist*2/3, damage*9/10), nil, o, nil, nil, nil, from_ID);
	}

	Sound("Blast3");
	ExplosionEffect(dist);
	RemoveObject();
}


func CreateTrail()
{
	return;
}

public func Fire(object shooter, int angle, int dev, int dist, int dmg, id weapon, iRange)
{
	from_ID = weapon;
	user = shooter;
	damage = dmg;
	deviation = dev;
	range = iRange;
	
	SetR(angle);
	SetController(shooter->GetController());
	//AddEffect("HitCheck2", this, 1,1, nil,nil, shooter);
	// remove after some time
	SetAction("Travel");
	SetComDir(COMD_None);
	
	//LaunchProjectile(angle, dist, 400);
	angle *= 100;
	angle += RandomX(-deviation, deviation);
	
	/*var x_offset = Sin(angle, dist);
	var y_offset = -Cos(angle, dist);
	
	Exit(x_offset, y_offset);*/
	SetSpeed(Sin(angle, speed, 100), -Cos(angle, speed, 100), 100);
	//Log("%d -> %d|%d", angle, GetXDir(100), GetYDir(100));
	
	var e = AddEffect("PositionCheck", this, 1, 1, this);
	EffectCall(this, e, "Timer");
	
	//AddEffect("HitCheck2", this, 1,1, nil,nil, shooter);
	AddEffect("SmokeTrail", this, 20, 1, this);
	AddEffect("RocketHitCheck", this, 20, 1, this, nil, shooter);
}

func FxRocketHitCheckStart(target, effect, int temp, object by_obj)
{
	if(temp)
		return;

	effect.shooter = by_obj;
	effect.x = GetX();
	effect.y = GetY();
	
	EffectCall(target, effect, "Timer");
}

func FxRocketHitCheckTimer(target, effect)
{
	var shooter = effect.shooter;
	var oldx = effect.x;
	var oldy = effect.y;

	for (obj in FindObjects(	Find_Distance(15),
								Find_NoContainer(),
								Find_Layer(target->GetObjectLayer()),
								Find_Or(Find_OCF(OCF_Alive), Find_Func("IsProjectileTarget", target)),
								Find_PathFree(target)))
		{
			// Excludes
			if(obj == shooter) continue;
			// Unlike in hazard, there is no NOFF rule (yet)
			// CheckEnemy
			//if(!CheckEnemy(obj,target)) continue;
			// IsProjectileTarget or Alive will be hit
			if (obj->~IsProjectileTarget(target, shooter) || obj->GetOCF() & OCF_Alive)
			{	
				/*
				var objdist = ObjectDistance(target, obj);
				
				target->SetPosition(oldx + Cos(target->GetR()-90, objdist), oldy + Sin(target->GetR()-90, objdist));
				if(target.trail)
					target.trail->Traveling();
				*/
				target->DoExplode();
				if (!target)
					return;
			}
		}
		
	effect.x = GetX();
	effect.y = GetY();
	
	
	return;
}



func FxSmokeTrailtStart(object pTarget, proplist effect, int temp, object by_obj)
{
	effect.x = pTarget->GetX();
	effect.y = pTarget->GetY();
}

func FxSmokeTrailTimer(object pTarget, proplist effect)
{
	EffectCall(pTarget, effect, "Draw");

	effect.x = pTarget->GetX();
	effect.y = pTarget->GetY();
	
}

func FxSmokeTrailDraw(pTarget,proplist effect)
{

  var dist = Distance(0,0,pTarget->GetXDir(),pTarget->GetYDir());
  var maxx = +Sin(GetR(),dist/10);
  var maxy = -Cos(GetR(),dist/10);
  var ptrdist = 50;

  for(var i=0; i<dist; i+=ptrdist) {

    var x = -maxx*i/dist;
    var y = -maxy*i/dist;


    var rand = RandomX(-30,30);
    var xdir = +Sin(GetR()+rand,5);
    var ydir = -Cos(GetR()+rand,5);

    CreateParticle("FireDense",x,y,GetXDir()/2,GetYDir()/2, PV_Random(20, 30), Particles_Fire());
    Smoke(0, 0, 5);
  }
  
}


func FxPositionCheckTimer(target, effect, time)
{
	if(time < range/20)
		return;
		
	var angle = GetR();
	
	if(angle < 0)
		angle-=1;
	else 
		if(angle > 0)
			angle+=1;
	
	SetR(angle);
	
	angle *= 100;
	
	SetSpeed(Sin(angle, speed, 100), -Cos(angle, speed, 100), 100);
}


func TrailColor(int acttime){/*return 0x88fffffff;*/ return RGBa(255,255 - Min(150, acttime*20) ,75,255);}

public func HitObject(object obj)
{
	DoExplode();
}

// called by successful hit of object after from ProjectileHit(...)
public func OnStrike(object obj)
{
	DoExplode();
	return;
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