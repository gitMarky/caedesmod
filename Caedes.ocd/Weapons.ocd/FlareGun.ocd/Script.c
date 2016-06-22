#include Pistol

func IsPrimaryWeapon(){return false;}
func TitleDef(){return FlareGun_Title;}

public func GetDescID() { return FlareGun_Title; }
public func GetCarryTransform()
{
	return Trans_Rotate(-90, 0, 1, 0);
}

local animation_set;

local weapon_properties =
{
	max_ammo = 10,
	clip = 2,
	recharge = 10,
	
	spread = 1,
	spread_factor = 1000,
	
	bullet_id = FlareGun_Flare,
	damage = 0,
	range = 50,
	
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
		LoadTime       = 120,
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
	Sound("Fire::Inflame");
}

func FireEffect(clonk, angle, off_x, off_y)
{
	
}

func Definition(def) {
	SetProperty("PictureTransformation",Trans_Mul(Trans_Rotate(90,0,1,0),Trans_Rotate(270,0,0,1)),def);
}

local Name = "$Name$";
local Description = "$Description$";
local UsageHelp = "$UsageHelp$";
local Collectible = 1;
local Rebuy = true;
