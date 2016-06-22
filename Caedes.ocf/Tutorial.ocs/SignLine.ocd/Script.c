/**
	nono go there

	@author <Insert Author>
*/

local Name = "DO NOT CROSS";

func IsTutProp(){return true;}

public func Initialize()
{
	AddTimer("Check", 20);
}

func Check()
{
	if(ObjectCount(Find_InRect(-20, -60, 40, 120), Find_OCF(OCF_Alive), Find_ID(Clonk)))
	{
		for(var i = 0; i < 3; ++i)
		{
			var bomb = CreateObject(IronBomb);
			bomb->SetSpeed(0 + Random(10), -20 + RandomX(-10, 10));
			bomb->ControlUse();
		}
	}
	
	var obj;
	if(obj = FindObject(Find_AtRect(20, -200, 300, 400), Find_OCF(OCF_Alive), Find_ID(Clonk)))
	{
		obj->Sound("Clonk::Verbal::Hurt*");
		obj->Fling(-2, -2);
		obj->SetSpeed(-30, -20);
	}
}