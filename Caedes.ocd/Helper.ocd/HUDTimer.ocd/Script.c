
local faded;

protected func Initialize()
{
	// Set parallaxity
	this.Parallaxity = [0, 0];
	// Set visibility
	this.Visibility = VIS_All;
	
	SetPosition(-250, 40);
	
	AddEffect("CountDown", this, 1, 36, this);
	return;
}

func FxCountDownTimer(target, effect, time)
{
	var td = Caedes_ShoppingTime + Caedes_RoundTime - time;
	
	if(td < 0)
	{
		var self = this;
		CaedesTimeRanOut();
		if(self) RemoveObject();
		return -1;
	}
	
	Update(td / 36);	
	return 1;
}	

public func Update(t)
{
	var critical = t < 30;
	var msg = Format("%02d:%02d", t/60, t%60);
	var color = 0xffffffff;
	if(critical)
	{
		color = 0xff0101;
		
		if(!faded) FadeToRed();
	}
	// Display wealth via text.
	CustomMessage(Format("@%s", msg), this, nil, 60, 48, color);
	return;
}

func FadeToRed()
{
	faded = true;
	AddEffect("RedFade", this, 1, 1, this);
}

func FxRedFadeTimer(target, effect, time)
{
	var r = BoundBy(time * 10, 0, 255);
	
	SetClrModulation(RGB(r, 255 - r, 255 - r));
	
	if(r == 255) return -1;
}