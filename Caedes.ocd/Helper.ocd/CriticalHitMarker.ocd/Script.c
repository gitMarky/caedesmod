/*-- Neues Objekt --*/


local ActMap =
{
	Float =
	{
		Name="Float",
		Procedure=DFA_FLOAT,
		FacetBase=1,
		Delay=0,
		Length=1,
		NextAction="Float",
	}
};

func NoWarp(){return 1;}

func Initialize() {

	AddTimer("Check", 5);
  SetClrModulation(RGB(0,255,0));  
  SetAction("Float");
  SetComDir(COMD_None);
  SetYDir(-5);
  SetObjDrawTransform(600,0,0,0,600,0);
  return(1);
}

func Check()
{
 var time=GetActTime()*5;
 SetClrModulation(RGBa(time,255-time,0,time));
 if(time >= 255)return RemoveObject();
}

global func CreateCriticalHitMark(x,y,iPlr)
{
 var obj=CreateObject(Critmarker,AbsX(x),AbsY(y),iPlr);
}
