protected func Activate(int plr)
{
	MessageWindow(GetProperty("Description"), plr);
	return true;
}

protected func Initialize() 
{
	AddTimer("Timer",20);
}

protected func Timer() 
{
	for (var clonk in FindObjects(Find_Category(C4D_Living), Find_OCF(OCF_Alive), Find_NoContainer(), Find_Not(Find_Owner(NO_OWNER)))) 
	{
		if (clonk->GetY() > (LandscapeHeight()-50)) {
			LaunchShark(clonk);
		}
	}
}

protected func LaunchShark(clonk) 
{
	var Shark = CreateObject(SharkPit_Shark);
	
	Shark->SetPosition(clonk->GetX()-10,LandscapeHeight()-50);
	Shark->SetSpeed(30,-40);
	
	if (clonk->GetEnergy() < clonk.MaxEnergy) {
		clonk->SetKiller(clonk->GetKiller());
	} else {
		clonk->SetKiller(NO_OWNER);
	}

	DoDmg(clonk.MaxEnergy + 1, nil, clonk, 1, nil, Shark, SharkPit_Shark);
}


local Name = "Shark Pit";
local Description = "$Description$";
