global func Particles_Blood(int size, int rotation)
{
	return
	{
		R = 200, G = 0, B = 0,
		Size = PV_KeyFrames(0, 0, 0, 100, size, 900, size, 1000, 0),
		Alpha = PV_KeyFrames(0, 0, 255, 700, 255, 1000, 0),
		DampingX = 900,  DampingY = 900,
		ForceY = PV_Gravity(400),
		Rotation = rotation,
		Attach = ATTACH_Front,
		CollisionVertex = 500,
		OnCollision = PC_Die(),
		Stretch = 1000,
		Phase = PV_Random(0, 4)
	};
}

static Caedes_ImpactParticles_Sparks;
static Caedes_ImpactParticles_Debris;

global func CreateImpactEffect(int size, int xoff, int yoff)
{
	if (!Caedes_ImpactParticles_Sparks)
	{
		Caedes_ImpactParticles_Sparks = 
		{
			Size = PV_Linear(2, 0),
		    ForceY = 2,
			Stretch = PV_Linear(10000, 0),
			Rotation = PV_Random(0, 360),
		    R = 255,
		    G = PV_Linear(255, 100),
		    B = PV_Random(0, 255, 2),
			BlitMode = GFX_BLIT_Additive,
			Phase = PV_Random(0, 4)
		};
		
		Caedes_ImpactParticles_Debris =
		{
			Prototype = Particles_Material(RGB(255, 200, 100)),
			Size = PV_Linear(PV_Random(0, 2), 0),
			Stretch = PV_Speed(500, 1000),
		};
	}
	
	CreateParticle("Frazzle", 0, 0, 0, 0, PV_Random(1, 10), Caedes_ImpactParticles_Sparks, size * 4);
	CreateParticle("SmokeDirty", PV_Random(-2, 2), PV_Random(-2, 2), PV_Random(-size * 2, size * 2), PV_Random(-size * 2, size * 2), PV_Random(10, 40), Caedes_ImpactParticles_Debris, size);
}

global func CreateCartridgeEffect(int size, int xdir, int ydir)
{
	CreateParticle("Cartridge", 0, 0, xdir, ydir, PV_Random(90, 200), Particles_Cartridge(size), 1);
}

global func Particles_Cartridge(int size)
{
	return
	{
		Alpha = PV_KeyFrames(0, 0, 255, 950, 255, 1000, 0),
		R = 255, G = 255, B = 255,
		Rotation = PV_Step(PV_Random(1, 4), PV_Random(60)),
		CollisionVertex = 0,
		OnCollision = PC_Bounce(500),
		ForceY = PV_Gravity(1000),
		Size = size
	};
}


