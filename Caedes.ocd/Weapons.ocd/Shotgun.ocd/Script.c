#include Pistol

func TitleDef(){return Shotgun_Title;}

public func GetDescID() { return "ShotgunDesc"; }
public func GetCarryMode(clonk) { if(fAiming >= 0) return CARRY_Musket; }
public func GetCarrySpecial(clonk) { if(fAiming > 0) return "pos_hand2"; }
public func GetCarryBone()	{	return	"main";	}
public func GetCarryTransform()
{
	return Trans_Mul(Trans_Rotate(-90, 0, 1, 0), Trans_Rotate(90, 1, 0, 0));
}

local animation_set;

local weapon_properties =
{
	max_ammo = 10,
	clip = 1,
	recharge = 110,
	
	spread = 3000,
	spread_factor = 7000,
	//spread_decrease = 30,
	
	bullet_id = NormalBullet,
	damage = 2,
	range = 120,
	
	bullet_distance = 10,
	fire_immediate = true,
	
	gfx_length = 20,
	//gfx_up = 15,
	gfx_off_y = -6,
	silencer = true
};

func Initialize()
{
	rounds = weapon_properties.clip;
	total_ammo = weapon_properties.max_ammo;
		
	animation_set = {
		AimMode        = AIM_Position, // The aiming animation is done by adjusting the animation position to fit the angle
		AnimationAim   = "MusketAimArms",
		AnimationLoad  = "MusketLoadArms",
		LoadTime       = 90,
		AnimationShoot = nil,
		ShootTime      = 20,
		WalkSpeed      = nil,
		WalkBack       = nil,
	};
}

public func GetAnimationSet() { return animation_set; }

local iBarrel;

func FireShot(clonk, angle, off_x, off_y, range)
{
	for(var cnt = 0; cnt < 20;++cnt)
	{
		var c = CreateObject(weapon_properties.bullet_id, off_x, off_y, GetController());
		var dmg = weapon_properties.damage;
		if(clonk.crits)
			{
				dmg = (dmg * 2) / 3;
				c.crit = true;
			}
		var r = RandomX(range/3-10, range);
		c->~Fire(clonk, angle + RandomX(-1, 1), GetDeviation(), weapon_properties.bullet_distance /* distance */, dmg, GetID(), r);
		
		if(c)
			c->SetSpeed(c->GetXDir(100) + RandomX(-100, 100), c->GetYDir(100) + RandomX(-100, 100), 100);
	}
}

func FireEffect(clonk, angle, off_x, off_y)
{
	clonk->CreateMuzzleFlash(off_x, off_y, angle, 40);
	CreateCartridgeEffect(5, -Sin(angle,500)/80, -RandomX(6,12));
}

func FireScreenshake(clonk)
{
	ShakeViewport(20, 0, 0, 10);
}

func Definition(def) {
	SetProperty("PictureTransformation",Trans_Mul(Trans_Rotate(90,0,1,0),Trans_Rotate(270,0,0,1)),def);
}

local Name = "$Name$";
local Description = "$Description$";
local UsageHelp = "$UsageHelp$";
local Collectible = 1;
local Rebuy = true;
