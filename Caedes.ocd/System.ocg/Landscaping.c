global func SmoothLandscape(int zoomFactor, int whatMat, string withWhat)
{
	zoomFactor = zoomFactor ?? 8;
	whatMat = whatMat ?? Material("Wall");
	withWhat = withWhat ?? "Wall-Concrete1";
	
	var zoomOff = zoomFactor/2;
	
	for(var outX = 0; outX <= LandscapeWidth(); outX += zoomFactor)
	for(var outY = 0; outY <= LandscapeHeight(); outY += zoomFactor)
	{
		var centerMat = GetMaterial(outX + zoomOff, outY + zoomOff);
		if(centerMat != whatMat) continue;
		if(!GBackSolid(outX + zoomOff, outY + zoomOff)) continue;
		
		//for(var innerX = outX - zoomFactor; innerX <= outX + zoomFactor; innerX += zoomFactor)
		var innerX = outX + zoomFactor;
		for(var innerY = outY - zoomFactor; innerY <= outY + zoomFactor; innerY += zoomFactor)
		{
			if((innerX == outX) || (innerY == outY)) continue;
			//if(innerY >= outY) continue;
			var outerMaterial = GetMaterial(innerX + zoomOff, innerY + zoomOff);
			if(outerMaterial != centerMat) continue;
			SmoothMaterialBlocks(outX, outY, innerX, innerY, zoomFactor, withWhat);
		}
		
	}
}

/*

	1******
	*     *#
	*     *##
	*******####
	       2******
	      #*     *
	     ##*     *
	   ####*******
	1******
	*     *
	*     *
	*******     
	 
*/

global func SmoothMaterialBlocks(int s_x, int s_y, int t_x, int t_y, int zoom, string material)
{
	if(t_x < s_x)
	{
		var temp_x = t_x;
		var temp_y = t_y;
		t_x = s_x;
		t_y = s_y;
		s_x = temp_x;
		s_y = temp_y;
	}
	var e1_x, e1_y;
	var e2_x, e2_y;
	var e3_x, e3_y;
	var e4_x, e4_y;
	
	if(s_y < t_y)
	{
	 	e1_x = s_x + zoom;
	 	e1_y = s_y;
	
	 	e2_x = t_x + zoom;
	 	e2_y = t_y;
	
	 	e3_x = t_x;
	 	e3_y = t_y + zoom;
	
		e4_x = s_x;
		e4_y = s_y + zoom;
	}
	else
	{
		e1_x = s_x;
		e1_y = s_y;
		
		e2_x = t_x;
		e2_y = t_y;
		
		e3_x = t_x + zoom;
		e3_y = t_y + zoom;
		
		e4_x = s_x + zoom;
		e4_y = s_y + zoom;
	}
	
	DrawMaterialQuad(material, e1_x, e1_y, e2_x, e2_y, e3_x, e3_y, e4_x, e4_y, true);
}