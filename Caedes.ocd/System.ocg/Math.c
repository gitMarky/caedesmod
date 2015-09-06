global func MirrorAngle(int angle1, int axis)
{
	angle1 = Normalize(angle1);
	axis = Normalize(axis);
	var a = Normalize(axis - angle1) - 180;
	a = Normalize(a);
	return axis + a;
}


global func GetAngleFromVector(vec1)
{
	return Angle(0, 0, vec1.x, vec1.y);
}

global func GetSurfaceVector(int x, int y)
{
	var normal = {x = 0, y = 0};
	
	var fac = 1;
	for(var fac = 1; fac <= 4; fac *= 2)
	{
		if(GBackSolid(fac, 0)) {--normal.x;}
		if(GBackSolid(-fac, 0)) {++normal.x;}
	
		if(GBackSolid(0, fac)) {--normal.y;}
		if(GBackSolid(0, -fac)) {++normal.y;}
	}
	
	return normal;
}