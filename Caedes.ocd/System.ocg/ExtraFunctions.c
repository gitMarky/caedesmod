
global func DrawParticleLine2(string particle, int x0, int y0, int x1, int y1, int prtdist, int rndoffset, int rnddir, ...)
{
	if(prtdist == 0)
		prtdist = 1;

	var angle = Angle(x0, y0, x1, y1) - 90;
	var dist = Distance(x0, y0, x1, y1);
	
	for(var i = 0; i < dist; i += prtdist)
	{
		var x = x0 + Cos(angle, i);
		var y = y0 + Sin(angle, i); 
		CreateParticle(particle, 
			PV_Random(x - rndoffset, x + rndoffset), 
			PV_Random(y - rndoffset, y + rndoffset), 
			PV_Random(-rnddir, rnddir), PV_Random(-rnddir, rnddir), ...);
	}
}