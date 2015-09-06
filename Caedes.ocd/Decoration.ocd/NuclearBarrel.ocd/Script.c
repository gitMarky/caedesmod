/**
	Barrel
	Dangerous to environment.
*/

local Name = "$Name$";
local Description = "$Description$";

public func Initialize()
{
	SetGraphics(nil, ExplosiveBarrel);
	SetMeshMaterial("BarrelMaterialNuclear");
	this.MeshTransformation = Trans_Rotate(RandomX(-20, 20), 0, 1, 0);
	return true;
}

func IsProjectileTarget(){return false;}


func BlowUp()
{
	ExplosionEffect(20);
	Sound("Blast3");
	var particles = 
	{
		Prototype = Particles_Smoke(),
		R = 1, G = 255, B = 1,
		Size = PV_KeyFrames(0, 0, 0, 500, PV_Random(35, 45)),
		ForceY = PV_Random(-2, 2), ForceX = PV_Random(-2, 2),
		BlitMode = GFX_BLIT_Additive,
		Rotation = PV_Step(PV_Random(-1, 1), PV_Random(180))
	};
	CreateParticle("SmokeDirty", 0, 0, 0, 0, PV_Random(20, 40), particles, 10);
	RemoveObject();
}