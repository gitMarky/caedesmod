
func Initialize()
{
	AddEffect("FadeAway", this, 1, 1, this);
	this.Parallaxity = [0, 0];
}

func FxFadeAwayTimer(target, effect, time)
{
	if(time*2 > 250)
	{
		RemoveObject();
		return -1;
	}
	SetClrModulation(RGBa(255, 255, 255, 255 - time*2));
	SetObjDrawTransform(1000 - time * 3, 0, -(time * 2) * 700, 0, 1000 - time * 3);
}