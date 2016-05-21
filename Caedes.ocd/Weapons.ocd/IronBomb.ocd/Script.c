/*
	IronBomb
	Author: Ringwaul

	Explodes after a short fuse.
*/
	
func TitleDef(){return IronBomb_Title;}
public func GetDescID() { return "IronbombDesc"; }

public func ControlUse(object clonk, int x, int y, bool box)
{
	// if already activated, nothing (so, throw)
	if(GetEffect("FuseBurn", this))
	{
		return false;
	}
	else
	{
		Fuse();
		return true;
	}
}

func Fuse()
{
	AddEffect("FuseBurn", this, 1,1, this);
}

func FxFuseBurnTimer(object bomb, proplist fx, int timer)
{
	var i = 3;
	var x = +Sin(GetR(), i);
	var y = -Cos(GetR(), i);
	CreateParticle("StarSpark", x, y, PV_Random(x - 2, x + 2), PV_Random(y - 2, y + 2), PV_Random(10, 40), Particles_Glimmer(), 5);
	if(timer == 1) Sound("Fire::FuseLoop",nil,nil,nil,+1);
	if(timer >= 40)
	{
		Sound("Fire::FuseLoop",nil,nil,nil,-1);
		DoExplode();
		return -1;
	}
}

func DoExplode()
{
	for(var i = 0; i < 360; i += 12)
	{
		var shrapnel = CreateObject(Shrapnel);
		shrapnel->SetController(GetController());
		shrapnel->SetVelocity(i, RandomX(100,140));
		shrapnel->SetRDir(-30+ Random(61));
		shrapnel.from_ID = IronBomb;
		CreateObject(BulletTrail)->Set(2,30,shrapnel);
		AddEffect("HitCheck2", shrapnel, 1,1, nil,nil, nil);
	}
	if(GBackLiquid())
		Sound("Fire::BlastLiquid2");
	else
		Sound("Fire::BlastMetal");
	
	ExplosionEffect(15);
	RemoveObject();
}

protected func Hit(x, y)
{
	StonyObjectHit(x,y);
	return true;
}

protected func Incineration() { Extinguish(); Fuse(); }

func RejectEntrance()
{
	return !!GetEffect("FuseBurn", this);
}

public func IsSecondaryWeapon(){return true;}
public func IsWeapon() { return true; }
public func IsArmoryProduct() { return true; }
public func HasFuse() { return true; }

local Name = "$Name$";
local Description = "$Description$";
local UsageHelp = "$UsageHelp$";
local Collectible = 1;
local Rebuy = true;
local BlastIncinerate = 1;
local ContactIncinerate = 1;
