/*-- Tutorial Helpers --*/

static const SHOWCTRLPOS_Top = 0,
              SHOWCTRLPOS_TopLeft = 3,
              SHOWCTRLPOS_TopRight = 4,
              SHOWCTRLPOS_BottomLeft = 2,
              SHOWCTRLPOS_BottomRight = 1;

static g_msgpos, g_msgoffx, g_msgoffy, g_msgwdt;

global func HasSpeech(string strMessage)
{
	var len = GetLength(strMessage);
	for (var i = 0; i < len; i++)
		if (GetChar(strMessage, i) == GetChar("$"))
			return true;
	return false;
}

global func TutorialMessage(string strMessage)
{
	// Message with speech marker
	if (HasSpeech(strMessage))
	{
		// PlayerMessage will handle the speech output (and it won't show the message)
		var dummy = CreateObject(Dummy);
		dummy->PlayerMessage(0, strMessage);
		dummy->RemoveObject();
	}
	// Normal message display, in addition to speech output
	if (GetLength(strMessage)) strMessage = Format("@%s", strMessage);
	CustomMessage(strMessage, nil, nil, g_msgoffx, g_msgoffy, 0xffffff, GUI_MenuDeco, CaedesLogo, g_msgpos | MSG_DropSpeech, g_msgwdt);
}


global func SetTutorialMessagePos(int posflags, int offx, int offy, int wdt)
{
	g_msgpos = posflags;
	g_msgoffx = offx;
	g_msgoffy = offy;
	g_msgwdt = wdt;
	return true;
}
