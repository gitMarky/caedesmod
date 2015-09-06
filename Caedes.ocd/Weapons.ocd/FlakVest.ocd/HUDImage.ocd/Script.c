/**
	HUDImage
	shows stuff.
*/

func GetBarGraphicsName(int p)
{
	if(p >= 75) return "75";
	if(p >= 50) return "50";
	if(p >= 25) return "25";
	return nil;
}

func GetBarColor(int p)
{
	var charge = (255 * p) / 100;
	var r = 255, g = 255;
	if(p > 50) g = BoundBy(255 - charge * 2, 0, 255);
	if(p < 50) r = BoundBy(charge * 2, 0, 255);
	return {r = r, g = g, b = 1};
}