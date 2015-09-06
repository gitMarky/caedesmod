/*
		Shows an icon for the ringmenu.
*/


local myid;
local amnt;
local data;
local size;

local bg;

static const MI_CHAT_BG_LAYER = 0;				// background
static const MI_CHAT_ICON_LAYER = 2;				// icon
static const MI_CHAT_EXTRASLOT_BG_LAYER = 4;		// background for extraslot
static const MI_CHAT_EXTRASLOT_LAYER = 6;		// icon for extraslot

											// amount, e.g. x192
static const MI_CHAT_AMOUNTX_LAYER = 9;			// x
static const MI_CHAT_AMOUNT100_LAYER = 10;		// 1
static const MI_CHAT_AMOUNT10_LAYER = 11;		// 9
static const MI_CHAT_AMOUNT1_LAYER = 12;			// 2

local angle, size;

protected func Construction()
{
	bg = true;
	myid = nil;
	// visibility
	this.Visibility = VIS_Owner;
	// parallaxity
	this.Parallaxity = [0,0];
	size=1000;
}

public func SetBG(bool newbg) {
	bg = newbg;
	SetSymbol(GetSymbol());
}

public func SetNothing() // No item, no image, no whatever, just a plain button
{
	SetAmount(nil);
	SetSymbol();
}

func SetDrawTransform(s, r, layer, xoff, yoff)
{
  size = size ?? 1000;
  var fsin=Sin(r, 1000), fcos=Cos(r, 1000);
  // set matrix values
  SetObjDrawTransform (
    s * (+fcos) / 1000, s*(+fsin) / 1000, s*((1000-fcos)*xoff - fsin*yoff)/1000,
    s*(-fsin) / 1000, s*(+fcos)/1000, s*((1000-fcos)*yoff + fsin*xoff) / 1000, layer
  );
}

func SetAngle(a)
{
	angle = a;
	SetDrawTransform(size, angle, MI_CHAT_BG_LAYER);
	SetSymbol(GetSymbol());
	SetAmount(GetAmount());
}

public func SetSize(int s)
{
	size = s;
	SetDrawTransform(size, angle, MI_CHAT_BG_LAYER);
	SetSymbol(GetSymbol());
	SetAmount(GetAmount());
}

public func ResetSize() { SetSize(1000); }


public func SetSymbol(obj)
{

	if(bg) {
		SetGraphics(nil,nil,MI_CHAT_BG_LAYER);
	} else {
		SetGraphics("NoBG",nil,MI_CHAT_BG_LAYER);
	}

	this.Visibility = VIS_Owner;
		
	if(!obj)
	{
		SetGraphics(nil, nil, MI_CHAT_ICON_LAYER-1);
		SetGraphics(nil, nil, MI_CHAT_ICON_LAYER);
		SetGraphics(nil, nil, MI_CHAT_EXTRASLOT_BG_LAYER);
		SetGraphics(nil, nil, MI_CHAT_EXTRASLOT_LAYER);
	}
	else
	{
		SetGraphics(nil,obj,MI_CHAT_ICON_LAYER-1,GFXOV_MODE_IngamePicture);
		SetGraphics(nil,obj,MI_CHAT_ICON_LAYER,GFXOV_MODE_IngamePicture);
		SetClrModulation(RGB(10, 10, 10), MI_CHAT_ICON_LAYER-1);
		SetDrawTransform(size + 300, angle, MI_CHAT_ICON_LAYER-1);
		SetDrawTransform(size, angle, MI_CHAT_ICON_LAYER);
		SetName(obj->GetName());
	}
	myid = obj;
}

func Hover(bool f)
{
	if(f) SetClrModulation(RGB(255, 255, 0), MI_CHAT_ICON_LAYER);
	else  SetClrModulation(RGB(255, 255, 255), MI_CHAT_ICON_LAYER);
}

public func GetAmount()    { return amnt; }
public func GetExtraData() { return data; }
public func GetSymbol()    { return myid; }
public func GetSize()      { return size; }

public func SetExtraData(extradata)
{
	data=extradata;
}
public func SetAmount(Amount)
{
}