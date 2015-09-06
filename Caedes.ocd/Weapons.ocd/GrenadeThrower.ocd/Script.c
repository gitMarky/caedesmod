#include Pistol

func TitleDef(){return GrenadeThrower_Title;}

public func GetDescID() { return GrenadeThrower_Title; }
public func GetCarryMode(clonk) { if(fAiming >= 0) return CARRY_Grappler; }
public func GetCarrySpecial(clonk) { if(fAiming > 0) return "pos_hand2"; }
public func GetCarryBone()	{	return	"main";	}
public func GetCarryTransform()
{
	return Trans_Rotate(-90, 0, 1, 0);
}

local animation_set;

local weapon_properties =
{
	max_ammo = 5,
	clip = 1,
	recharge = 10,
	
	spread = 1,
	spread_factor = 1000,
	
	bullet_id = GrenadeThrower_Grenade,
	damage = 50,
	range = 200,
	
	bullet_distance = 8,
	fire_immediate = true,
	
	gfx_length = 15,
	//gfx_up = 15,
	gfx_off_y = -6,
};

func Initialize()
{
	rounds = weapon_properties.clip;
	total_ammo = weapon_properties.max_ammo;
		
	animation_set = {
		AimMode        = AIM_Position, // The aiming animation is done by adjusting the animation position to fit the angle
		AnimationAim   = "MusketAimArms",
		AnimationLoad  = "MusketLoadArms",
		LoadTime       = 160,
		AnimationShoot = nil,
		ShootTime      = 20,
		WalkSpeed      = nil,
		WalkBack       = nil,
	};
}

public func GetAnimationSet() { return animation_set; }

local iBarrel;


func FireSound()
{
	Sound("EnergyBladeSwing*");
}

func FireEffect(clonk, angle, off_x, off_y)
{
	for(var i=0; i<10; ++i)
	{
		var speed = RandomX(0,10);
		var r = angle;
		//CXreateParticle("ExploSmoke",off_x,off_y,+Sin(r,speed)+RandomX(-2,2),-Cos(r,speed)+RandomX(-2,2),RandomX(10,40),RGBa(255,255,255,50));
		Smoke(off_x, off_y, 1, nil, true);
		r+= RandomX(-10,10);
		DrawLightningSmall(off_x, off_y, Sin(r, 10), -Cos(r, 10));
	}
	
}

func Definition(def) {
	SetProperty("PictureTransformation",Trans_Mul(Trans_Rotate(90,0,1,0),Trans_Rotate(270,0,0,1)),def);
}

local Name = "$Name$";
local Description = "$Description$";
local UsageHelp = "$UsageHelp$";
local Collectible = 1;
local Rebuy = true;
