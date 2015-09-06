/**
	DummyTarget
	It wants you dead.

	@author <Insert Author>
*/

local Name = "$Name$";
local Description = "$Description$";

local MaxEnergy = 25000;

local left_right, up_down;
local rotate_dir = 1;

local ActMap = {
Float = {
	Prototype = Action,
	Name = "Float",
	Procedure = DFA_FLOAT,
	Length = 1,
	Delay = 0,
	FacetBase=1,
}
};

func UD(){up_down = true; return this;}
func LR(){left_right = true; return this;}

func IsTutProp(){return true;}

public func Initialize()
{
	rotate_dir += Random(3);
	if(Random(2)) rotate_dir = -rotate_dir;
	DoEnergy(100);
	
	SetAction("Float");
	SetComDir(COMD_None);
	AddTimer("Movement", 30);
	AddEffect("Rotate", this, 1, 2, this);
	AddViewPoint();
	return true;
}

func FxRotateTimer(target, effect, time)
{
	effect.r = (effect.r + rotate_dir) % 360;
	if(effect.r < 0) effect.r = 360 + effect.r;
	this.MeshTransformation = Trans_Mul(Trans_Rotate(effect.r, 0, 1, 0), Trans_Rotate(Cos(time, 50), 1, 0, 0));
}

func Movement()
{
	if(!Random(3)) SetSpeed();
	
	if(up_down)
	{
		var d = 1; if(Random(2)) d = -1;
		if(GBackSolid(0, 15)) d = -1;
		else if(GBackSolid(0, -15)) d = 1;
		SetYDir(10 * d);
	}
	if(left_right)
	{
		var d = 1; if(Random(2)) d = -1;
		if(GBackSolid(10, 0)) d = -1;
		else if(GBackSolid(-10, 0)) d = 1;
		SetXDir(10 * d);
	}
}

func Death(int killed_by)
{
	FindObject(Find_ID(CallbackRule))->OnClonkDeath(this, killed_by);
	ScheduleCall(this, "Remove", 2);
}

func Remove()
{
	RemoveObject();
}

func SetLastDamagingWeapon(ID)
{
	this.last_damaging_weapon = ID;
}

func OnHit(dmg, type, from_obj)
{
	CreateParticle("SmokeDirty", PV_Random(-5, 5), PV_Random(-5, 5), PV_Random(-40, 40), PV_Random(-40, 40), PV_Random(10, 100), Particles_Glimmer(), 50);
}

func Stop()
{
	SetSpeed();
}

func ContactLeft(){Stop();}
func ContactRight(){Stop();}
func ContactUp(){Stop();}
func ContactDown(){Stop();}