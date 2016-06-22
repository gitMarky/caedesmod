/**
	Camera
	Makes close enemies visible.

*/

local Name = "$Name$";
local Description = "$Description$";

func TitleDef(){return Camera_Title;}

func IsWeapon(){return true;}
func IsGadget(){return true;}
public func GetDescID() { return "CameraDesc"; }


func Transform(p)
{
	this.MeshTransformation = Trans_Mul(Trans_Scale(2000, 2000, 2000), p);
}

func Definition(def)
{
	def.PictureTransformation = Trans_Mul(Trans_Rotate(180, 1, 0, 0), Trans_Rotate(10, 0, 1, 0));
}

public func Initialize()
{
	this.MeshTransformation = Trans_Scale(2000, 2000, 2000);
	return true;
}

func IsBulletTarget(){return true;}

func Departure(obj)
{
	if(!obj) return;
	if(!obj->~IsClonk()) return;
	this.Visibility = obj.Visibility;
}

func Damage(dmg)
{
	if(!Random(dmg/10)) return;
	ExplosionEffect(10);
	RemoveObject();
}

func Hit()
{
	var a_x, a_y;
	if(GBackSolid(-6, 0)) {a_x = -5; a_y = 0; Transform(Trans_Rotate(180, 0, 0,1));}
	else if(GBackSolid(6, 0)) {a_x = 5; a_y = 0; Transform(Trans_Rotate(180, 1, 0, 0));}
	else if(GBackSolid(0, -6)) {a_x = 0; a_y = -6; Transform(Trans_Mul(Trans_Rotate(180, 1, 0, 0), Trans_Rotate(180, 0, 0, 1), Trans_Rotate(270, 0, 1, 0)));}
	else if(GBackSolid(0, 6)) {a_x = 0; a_y = 6; Transform(Trans_Mul(Trans_Rotate(180, 1, 0, 0), Trans_Rotate(270, 0, 1, 0)));}
	
	SetCategory(C4D_StaticBack);
	AddTimer("Check", 5);
	AddTimer("Animation", 60);
	
	SetOwner(GetController());
	//AddViewPoint();
	SetLightRange(200, 10);
	MakeDefusable(35*1, GetPlayerTeam(GetController()));
}

func OnDefused()
{
	Smoke(0, 0, 10);
	RemoveObject();
}

func Animation()
{
	PlayAnimation("Zoom", 1, Anim_Linear(0, 0, GetAnimationLength("Zoom"), 60, ANIM_Remove), Anim_Const(500 + Random(500)));
	PlayAnimation("UpDown", 1, Anim_Linear(0, 0, GetAnimationLength("Zoom"), 60, ANIM_Remove), Anim_Const(500 + Random(500)));
}

func Check()
{
	
}