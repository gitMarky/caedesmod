/* shrapnel */

local from_ID, flight_time, damage;

protected func Initialize()
{
	SetAction("Flight");
	SetComDir(COMD_None);
	flight_time = 6;
	damage = 3;
	from_ID = GetID();
	AddEffect("Fade", this, 1, 1, this);
}

protected func FxFadeTimer(object target, int num, int timer)
{
	if(timer > flight_time) {Hit(); return -1;}
}

protected func Hit()
{
	//ShakeFree(6);
	var self = this;
	DoHitCheckCall();
	if(self)
		RemoveObject();
}

public func HitObject(object obj)
{
	DoDmg(damage, nil, obj, nil, nil, nil, from_ID);
	Sound("Hits::ProjectileHitLiving?");
	RemoveObject();
}

public func TrailColor(int time)
{
	return RGBa(100,100,100,240*Max(0,flight_time-time)/flight_time);
}

local ActMap = {
	Fly = {
		Prototype = Action,
		Name = "Fly",
		Procedure = DFA_FLIGHT,
		NextAction = "Fly",
		Delay = 1,
		Length = 1,
	},
};