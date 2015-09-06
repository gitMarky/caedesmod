/**
	Debris
	Scrap.
*/

local Name = "$Name$";
local Description = "$Description$";

local ActMap =
{
	Be =
	{
		Name="Be",
		Procedure=DFA_NONE,
		FacetBase=1,
		Delay=0,
		Length=1,
		NextAction="Be",
	}
};

public func Initialize()
{
	SetAction("Be");
	SetComDir(COMD_None);
	if(Random(3)) SetGraphics(Format("%d", Random(2) + 1));
	SetRDir(RandomX(-45, 45));
	AddEffect("RemoveMe", this, 1, 10, this);
	return true;
}

func FxRemoveMeTimer(target, effect, time)
{
	if(time > 35 * 2)
	{
		var a = 10 * (time - 35 * 2);
		if(a > 200) return RemoveObject();
		SetClrModulation(RGBa(255,255,255, 255 - a));
		return 1;
	}
	return 1;
}

func Hit()
{
	CreateParticle("SmokeDirty", 0, 0, PV_Random(-20, 20), PV_Random(-20, 20), PV_Random(10, 60), Particles_Glimmer(), 40);
	Sound("RockHit?", nil, 50);
	if(GetActTime() < 40) return;
	Sound("LightMetalHit?", nil, 20);
	RemoveObject();
}