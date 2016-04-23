#include Pistol

func TitleDef(){return SMG_Title;}

public func GetDescID() { return "SMGDesc"; }
public func GetCarryMode(clonk) { if(fAiming >= 0) return CARRY_Musket; }
public func GetCarrySpecial(clonk) { if(fAiming > 0) return "pos_hand2"; }
public func GetCarryBone()	{	return	"main";	}
public func GetCarryTransform()
{
	return Trans_Mul(Trans_Translate(-10000, 0, 1000), Trans_Rotate(-90, 0, 1, 0), Trans_Rotate(90, 1, 0, 0));
}

local animation_set;

local weapon_properties =
{
	max_ammo = 100,
	clip = 5,
	recharge = 1,
	
	spread = 300,
	spread_factor = 200,
	//spread_decrease = 50,
	
	bullet_id = NormalBullet,
	damage = 6,
	range = 200,
	
	bullet_distance = 10,
	fire_immediate = false,
	full_auto = true,
	
	gfx_length = 11,
	//gfx_up = 6,
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
		LoadTime       = 40,
		AnimationShoot = nil,
		ShootTime      = 20,
		WalkSpeed      = nil,
		WalkBack       = nil,
	};
}
local iBarrel;


func FireSound()
{
	SoundAt("SMG1", 0, 0, 50);
}

func FireEffect(clonk, angle, off_x, off_y)
{
	clonk->CreateMuzzleFlash(off_x, off_y, angle, 5);
	CreateCartridgeEffect(3, -Sin(angle,500)/80, -RandomX(6,12));
}

func FireScreenshake(clonk)
{
	ShakeViewport(5, 0, 0, 10);
}

func Definition(def) {
	SetProperty("PictureTransformation",Trans_Mul(Trans_Translate(1500,0,-1500),Trans_Rotate(170,0,1,0),Trans_Rotate(30,0,0,1)),def);
}