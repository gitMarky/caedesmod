
func ShowFor(int plr)
{
	CreateObject(this, -175, 350, plr);
}

func Initialize()
{
	SetGraphics("Back", Symbol_InfoYourTeamHasBomb, 0, GFXOV_MODE_Base, nil, GFX_BLIT_ClrSfc_OwnClr);
	SetGraphics(nil, GetID(), 1, GFXOV_MODE_Base, nil, GFX_BLIT_ClrSfc_OwnClr);
	
	AddEffect("FadeAway", this, 1, 1, this);
	this.Parallaxity = [0, 0];
	this.Visibility = VIS_Owner;
}

func FxFadeAwayTimer(target, effect, time)
{
	var t2 = Max(0, time - 200);
	
	if(t2*2 > 250)
	{
		RemoveObject();
		return -1;
	}
	
	var x_pos = (t2 * 4) * 700;
	
	if(time < 100)
		x_pos = ((100 - time) * 1000);
	
	SetObjDrawTransform(1000 - t2, 0, x_pos, 0, 1000 - t2);
	SetObjDrawTransform(1000 - t2, 0, x_pos, 0, 1000 - t2, 0, 1);
	
	if(t2 == 0) return 1;
	SetClrModulation(RGBa(255, 255, 255, 255 - t2*2));
	SetClrModulation(RGBa(255, 255, 255, 255 - t2*2), 1);
	return 1;
}