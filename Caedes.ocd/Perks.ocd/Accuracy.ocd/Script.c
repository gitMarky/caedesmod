/**
	Accuracy
	Increases the accuracy when shooting.

	@author <Insert Author>
*/

local Name = "$Name$";
local Description = "$Description$";

func IsPerk(){return true;}
public func GetDescID() { return "AccuracyDesc"; }

func Apply(clnk)
{
	if(clnk.accuracy == nil)
		clnk.accuracy = 0;
	clnk.accuracy++;
}

func Unapply(clnk)
{
	clnk.accuracy = Max(clnk.accuracy -1, 0);
}
