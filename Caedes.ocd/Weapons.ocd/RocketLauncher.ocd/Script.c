#include Pistol

func TitleDef(){return RocketLauncher_Title;}

public func GetDescID() { return "RocketLauncherDesc"; }
public func Exclusive() { return false; }
public func IsWeapon() { return true; }
public func IsSecondaryWeapon(){return true;}

func IsFirstHandItem(){return false;}
func IsPrimaryWeapon(){return false;}

public func GetCarryTransform()
{
	return Trans_Mul(Trans_Rotate(-90, 0, 1, 0), Trans_Rotate(90, 1, 0, 0));
}

local animation_set;

local weapon_properties =
{
	max_ammo = 0,
	clip = 1,
	recharge = 60, 
	delay_shot = 25,
	
	spread = 1,
	spread_factor = 1000,
	//spread_decrease = 30,
	
	bullet_id = Rocket,
	damage = 40,
	range = 140,
	
	bullet_distance = 10,
	fire_immediate = false,
	
	gfx_length = 10,
	gfx_off_y = -6,
};

func Construction()
{
	this.weapon_properties = {Prototype = this.Prototype.weapon_properties};
	
	rounds = weapon_properties.clip;
	total_ammo = weapon_properties.max_ammo;
	
	animation_set = {
		AimMode        = AIM_Position, // The aiming animation is done by adjusting the animation position to fit the angle
		AnimationAim   = "MusketAimArms",
		AnimationLoad  = "MusketLoadArms",
		LoadTime       = 80,
		AnimationShoot = nil,
		ShootTime      = 20,
		WalkSpeed      = Clonk.ActMap.Walk.Speed / 2,
		WalkBack       = Clonk.ActMap.Walk.Speed / 2,
	};
}

func AimStartSound()
{
	Contained()->Sound("SniperAim");
}

func ResetAim(angle)
{
	var e = GetEffect("DelayShot", this);
	
	if(e)
	{
		if(Abs(e.angle - angle) <= 2)
			return;
	}
	else
	{
		e = AddEffect("DelayShot", this, 1, 4, this);
		e.max_time = weapon_properties.delay_shot;
		e.time = weapon_properties.delay_shot;
	}
	
	e.angle = angle;
}

public func GetAnimationSet() { return animation_set; }

func FireSound()
{
	Sound("RocketLaunch");
}

func Fire()
{
	_inherited(...);
	
	this.Collectible=false; 
	Exit(0,0,Random(360), RandomX(-1,1), RandomX(-2,-1), RandomX(-20,20));
}

func FireEffect(clonk, angle, off_x, off_y)
{
	var particles = 
	{
		Prototype = Particles_Smoke(true),
		DampingX = PV_Random(900, 990), DampingY = PV_Random(900, 990),
		Size = PV_KeyFrames(0, 0, 0, 100, 10, 1000, 0)
	};
	var speed_x = -Sin(angle, 10);
	var speed_y = Cos(angle, 10);
	CreateParticle("SmokeDirty", -off_x, -off_y, PV_Random(speed_x - 5, speed_x + 5), PV_Random(speed_y - 5, speed_y + 5), PV_Random(40, 60), particles, 60);

	CreateMuzzleFlash(-off_x, -off_y, angle, 50);
}

func Definition(def) {
	SetProperty("PictureTransformation",Trans_Mul(Trans_Rotate(90,0,1,0),Trans_Rotate(270,0,0,1)),def);
}

local Name = "$Name$";
local Description = "$Description$";
local Collectible = 1;
local Rebuy = true;
