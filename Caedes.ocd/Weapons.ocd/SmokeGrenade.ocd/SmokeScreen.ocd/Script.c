/**
	smoke
*/

local Name = "Smoke";

local ActMap = {
Smoking = {
	Prototype = Action,
	Name = "Smoking",
	Procedure = DFA_NONE,
	Length = 20,
	Delay = 3,
	X = 0,
	Y = 0,
	Wdt = 32,
	Hgt = 64,
	EndCall = "EndSmoke",
	NextAction="Smoking",
}
};

public func Init()
{
	this.Plane = 1000;
	// quick check for ceiling
	var y, found = false;
	for(y = 25; y < 50; y+=2)
	{
		if(!GBackSolid(0, +32 - y)) continue;
		found = true;
		break;
	}
	
	if(found)
	{
		var p = (y * 1000) / 64;
		SetObjDrawTransform(1000, 0, 0, 0, p, (1000 - p) * 25);
	}
	SetClrModulation(RGBa(255,255,255,200));
	AddSmokeScreen();
	AddEffect("Smoking", this, 1, 3, this);
	SetAction("Smoking");
}

func EndSmoke()
{
	SetAction("Smoking");
	SetPhase(10 + Random(3));
}

func Destruction()
{
	RemoveSmokeScreen();
}

func FxSmokingStart(target, effect, temp)
{
	if (temp) return;
	effect.particles = 
	{
		Prototype = Particles_Smoke(),
		Size = PV_KeyFrames(0, 0, 20, 500, PV_Random(40, 70), 1000, 40),
		Alpha = PV_KeyFrames(0, 0, 0, 100, 200, 600, 255, 1000, 0),
		DampingX = 950, DampingY = 950,
		ForceX = PV_Random(-1, 1), ForceY = PV_Random(-1, 1)
	};
}

func FxSmokingTimer(target, effect, time)
{
	if(time > 35 * 10)
	{
		RemoveObject();
		return -1;
	}
	
	if(Random(3)) return 1;
	CreateParticle("Smoke", PV_Random(-5, 5), PV_Random(0, 10), PV_Random(-20, 20), PV_Random(-20, -5), PV_Random(90, 180), effect.particles, 1);
	return 1;
}