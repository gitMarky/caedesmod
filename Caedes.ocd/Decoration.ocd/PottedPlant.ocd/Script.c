/**
	Plant
	Environment.
*/

local Name = "$Name$";
local Description = "$Description$";

private func SetMeshTransformation(array additional)
{
	this.MeshTransformation = Trans_Mul(additional, Trans_Translate(-7000, 11000, 0), Trans_Rotate(90, 0, 0, 1));
}

public func Initialize()
{
	SetMeshTransformation(Trans_Rotate(RandomX(-5, 5), 0, 1, 0));
	this.particles = 
	{
		R = PV_Random(100, 155), B = PV_Random(100, 155), G = 155,
		Alpha = PV_KeyFrames(0, 0, 255, 500, 255, 1000, 0),
		ForceY = PV_Gravity(200),
		ForceX = PV_Random(-20, 20, 35),
		DampingX = 995, DampingY = 995,
		CollisionVertex = 0,
		OnCollision = PC_Stop(),
		Size = PV_KeyFrames(0, 0, 0, 100, PV_Random(1, 10), 1000, 0),
		Phase = PV_Random(0, 2),
		Rotation = PV_Direction()
	};
	return true;
}

func IsProjectileInteractionTarget(){return true;}

func OnProjectileInteraction(source_x, source_y, angle, shooter, damage)
{
	SetMeshTransformation(Trans_Rotate(RandomX(-5, 5), 0, 1, 0));
	var mx = GetX(), my = GetY();
	var d = Distance(source_x, source_y, mx, my);
	
	var main_y = source_y - Cos(angle, d, 100);
	
	var xd = Sin(angle, 30, 100);
	var yd = -Cos(angle, 30, 100);
	
	if(main_y < GetY() + 3)
	{
		var x = source_x + Sin(angle, d, 100) - mx;
		var y = source_y + -Cos(angle, d, 100) - my;
		CreateParticle("LeafDetailed", PV_Random(x - 3, x + 3), PV_Random(y - 3, y + 3), PV_Random(xd - 10, xd + 10), PV_Random(yd - 10, yd + 10), PV_Random(60, 100), this.particles, damage * 2);
	}
}


