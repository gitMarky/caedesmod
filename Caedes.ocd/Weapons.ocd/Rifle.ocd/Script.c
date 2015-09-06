#include Pistol

func TitleDef(){return Rifle_Title;}

public func GetDescID() { return "SniperDesc"; }
public func GetCarryMode(clonk) { if(fAiming >= 0) return CARRY_Musket; }
public func GetCarrySpecial(clonk) { if(fAiming > 0) return "pos_hand2"; }
public func GetCarryBone()	{	return	"main";	}
public func GetCarryTransform()
{
	return Trans_Rotate(-90, 0, 1, 0);
}

local animation_set;

local weapon_properties =
{
	max_ammo = 9,
	clip = 3,
	recharge = 35, // exactly the fasted shooting time
	
	spread = 1,
	spread_factor = 1000,
	//spread_decrease = 30,
	
	bullet_id = NormalBullet,
	damage = 40,
	range = 300,
	
	bullet_distance = 10,
	fire_immediate = false,
	delay_shot = 36,
	
	gfx_length = 20,
	//gfx_up = 15,
	gfx_off_y = -6,
	silencer = true
};

func AimStartSound()
{
	Contained()->Sound("SniperAim");
}





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

public func GetAnimationSet() { return animation_set; }

func FireSound()
{
	Sound("SniperShot");
}

func FireEffect(clonk, angle, off_x, off_y)
{
	clonk->CreateMuzzleFlash(off_x, off_y, angle, 30);
	CreateCartridgeEffect(10, -Sin(angle,500)/80, -RandomX(6,12));
}

func Definition(def) {
	def.PictureTransformation = Trans_Mul(Trans_Rotate(-30, 0, 0, 1), Trans_Translate(0, 9000, 0), Trans_Scale(1500, 1500, 1500));
}

func ManipulateBullet(object bullet)
{ 
	AddEffect("SniperBullet", bullet, 20, 1, 0, GetID());
}

func FxSniperBulletStart(object pTarget, proplist effect, int temp, object by_obj)
{
	if (temp) return;
	effect.x = pTarget->GetX();
	effect.y = pTarget->GetY();
	
	effect.particles = Particles_Smoke();
	effect.particles.ForceX = 0;
	effect.particles.ForceY = 0;
}

func FxSniperBulletTimer(object pTarget, proplist effect)
{
	EffectCall(pTarget, effect, "Draw");

	effect.x = pTarget->GetX();
	effect.y = pTarget->GetY();
	
}

func FxSniperBulletDraw(pTarget,proplist effect)
{
	effect.particles.R = 200;
	effect.particles.G = 200;
	effect.particles.B = 200;
	effect.particles.Size = PV_Linear(PV_Random(1, 5), PV_Random(10, 15));
	DrawParticleLine2("Smoke", effect.x, effect.y, pTarget->GetX(), pTarget->GetY(), 1, 0, 0, 30, effect.particles);
	effect.particles.Size = PV_Linear(PV_Random(1, 2), PV_Random(5, 6));
	effect.particles.R = 255;
	effect.particles.G = 255;
	effect.particles.B = 255;
	DrawParticleLine2("SmokeThick", effect.x, effect.y, pTarget->GetX(), pTarget->GetY(), 1, 0, 0, 25, effect.particles);
}

func FxSniperBulletStop(pTarget,proplist effect)
{
	if(!pTarget->GetXDir() && !pTarget->GetYDir())
		ShakeViewport(30, pTarget->GetX(), pTarget->GetY());
	
	EffectCall(pTarget, effect, "Draw");
}


local Name = "$Name$";
local Description = "$Description$";
local UsageHelp = "$UsageHelp$";
local Collectible = 1;
local Rebuy = true;
