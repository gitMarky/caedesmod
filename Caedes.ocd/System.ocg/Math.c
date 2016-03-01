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
