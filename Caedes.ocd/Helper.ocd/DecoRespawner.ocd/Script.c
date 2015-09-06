/**
	DecoRespawner
	Respawns deco.

*/

local my_deco;
local deco_id;
local x, y;

local drawtransform;

global func CreateRespawner(dt)
{
	
	var obj = CreateObject(DecoRespawner, AbsX(10), AbsY(10), -1);
	obj.deco_id = GetID();
	obj.x = GetX();
	obj.y = GetY() + GetDefCoreVal("Height", "DefCore") / 2;
	obj.my_deco = this;
	obj.drawtransform = dt;
	obj->ApplyDrawTransform();
	return this;
}

func OnNewRound()
{
	if(my_deco) my_deco->RemoveObject();
	my_deco = CreateObjectAbove(deco_id, x - GetX(), y - GetY(), NO_OWNER);
	ApplyDrawTransform();
}

func ApplyDrawTransform()
{
	if(!drawtransform) return;
	my_deco->SetObjDrawTransform(drawtransform.w, 0, 0, 0, drawtransform.h);
}