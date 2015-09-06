/**
	ExplosiveBarrel
	Dangerous to environment.
*/

local Name = "$Name$";
local Description = "$Description$";

public func Initialize()
{
	this.MeshTransformation = Trans_Rotate(RandomX(-45, 45), 0, 1, 0);
	return true;
}

func IsProjectileTarget(){return true;}

func Damage()
{
	BlowUp();
}

func OnHit(dmg, type, from, weapon)
{
	if(from)
		SetController(from->GetController());
}

func BlowUp()
{
	ExplosionEffect(20);
	Sound("Blast3");
	for(var obj in FindObjects(Find_Distance(30), Find_OCF(OCF_Alive), Find_NoContainer(), Find_PathFree(this)))
	{
		DoDmg(30 - ObjectDistance(this, obj), nil, obj, nil, nil, nil, GetID());
	}
	
	for(var i = 0; i < 360; i += 30)
	{
		var x = Sin(i, 50);
		var y = -Cos(i, 50);
		var d = CreateObject(Debris, 0, 0, -1);
		d->SetController(GetController());
		d->SetSpeed(x, y);
		AddEffect("ShortSmoke", d, 1, 2, nil, GetID());
	}
	
	RemoveObject();
}

func FxShortSmokeStart(target, effect, temp)
{
	if (temp) return;
	effect.particles =  
	{
		Prototype = Particles_Smoke(),
		ForceY = 0
	};
}

func FxShortSmokeTimer(target, effect, time)
{
	if(time > 35) return -1;
	var dt = time * 3;
	CreateParticle("Smoke", target->GetX(), target->GetY(), 0, 0, 10, effect.particles, 1);	
}