/**
	BombZone
	Plant bomb here.

*/

local Name = "$Name$";
local Description = "$Description$";



public func Initialize()
{
	SetClrModulation(RGBa(255,255,255, 100));
	SetShape(-25, -25, 50, 50);
	return true;
}

global func PlaceBombZone(x, y)
{
	CreateObject(BigBomb_BombZone, x - GetX(), y - GetY());
}

public func IsInteractable(object clonk)
{
	if(!clonk->IsWalking()) return false;
	if(clonk->FindContents(BigBomb)) return true;
	
	var bomb = FindObject(Find_Distance(20), Find_ID(BigBomb));
	if(bomb) if(bomb.Collectible == false) return true;
	
	return false;
}

public func Interact(object clonk)
{
	var bomb;
	if(bomb = clonk->FindContents(BigBomb)) {bomb->Interact(clonk); return true;}
	
	var bomb = FindObject(Find_Distance(20), Find_ID(BigBomb));
	if(bomb) bomb->Interact(clonk);
	
	return false;
}