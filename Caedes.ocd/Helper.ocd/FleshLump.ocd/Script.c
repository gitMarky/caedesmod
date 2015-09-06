
global func CastBlood()
{
	if(!this) return;
	for(var i = 0; i < 5; ++i)
	{
		var b = CreateObject(FLSH);
		b->SetSpeed(RandomX(-30, 30), RandomX(-30, 0));
		b->SetRDir(RandomX(-50,50));
	}
}

func Initialize()
{
	AddEffect("Blood",this(),20,2,this());
}

func FxBloodStart(target, effect, temp)
{
	if (temp) return;
	effect.particles = Particles_Blood(90);
	effect.particle_target = FindObject(Find_ID(CallbackRule));
}
func FxBloodTimer(target, effect, time)
{
	if (time > 80) return -1;
	var size = (90 - time) / 5;
	effect.particles.Size = PV_KeyFrames(0, 0, 0, 100, size, 900, size, 1000, 0);
	effect.particle_target->CreateParticle("Blood", GetX(), GetY(), 0, 0, PV_Random(60, 90), effect.particles);
}
  
func Hit() {
  Sound("FleshHit*");
  CreateParticle("SmokeDirty", 0, 0, PV_Random(-30, 30), PV_Random(-30, 30), PV_Random(40, 90), Particles_Material(RGB(200, 1, 1)), 40);  
  RemoveObject();
  return(1);
}
