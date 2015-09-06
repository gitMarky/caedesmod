/*-- Shark --*/

local Lifetime = 500;

public func Initialize() {
	//this.MeshTransformation = Trans_Mul(Trans_Scale(1000, 1000, 1000),Trans_Rotate(RandomX(0,359),1,0,0),Trans_Rotate(RandomX(0,359),0,1,0),Trans_Rotate(RandomX(0,359),0,0,1));
	this.MeshTransformation = Trans_Mul(Trans_Scale(500, 500, 500),Trans_Rotate(-90,0,0,1));
	AddTimer("Timer",1);
}

protected func Timer() {
	AdjustRotation();
	if (GetY() > (LandscapeHeight()-2)) RemoveObject();
	Lifetime--;
	if (Lifetime < 0) RemoveObject();
}

protected func AdjustRotation() {

	var Ang = Angle(GetX(),GetY(),GetX()+GetXDir(),GetY()+GetYDir());
	SetR(Ang-90);
}

local Name = "$Name$";