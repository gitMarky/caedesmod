
local menus;
local menu_object;

func IsBuyMenu(){return true;}

func SetMenuObject(object to) { menu_object = to; }

func Destruction()
{
	// remove Dummy objects used as menu targets
	var c = ContentsCount(), obj;
	while (obj = Contents(--c))
		obj->RemoveObject();
}

func OpenBuyMenu()
{
	this.Visibility = VIS_Owner;
	var menu = 
	{
		Target = this,
		Style = GUI_GridLayout
		// outer box is empty
	};
	
	menu.weapons = MakeMenu("~IsPrimaryWeapon", 1);
	menu.secondaryweapons = MakeMenu("~IsSecondaryWeapon", 2);
	menu.perks = MakeMenu("perks", 3);
	menu.gadgets = MakeMenu("~IsGadget", 4);
	
	GuiOpen(menu);
}

func MakeMenu(string what, int priority)
{
	var text = "$PrimaryWeapons$";
	if (what == "~IsSecondaryWeapon") text = "$SecondaryWeapons$";
	else if (what == "~IsGadget") text = "$Gadgets$";
	else if (what == "perks") text = "$Perks$";
	var purple = RGB(175, 0, 175);
	
	var menu =
	{
		Priority = priority,
		BackgroundColor = RGBa(0, 0, 0, 200),
		Style = GUI_FitChildren,
		titlebar = 
		{
			Bottom = "1.25em",
			Symbol = 0,
			text = 
			{
				Left = "1em",
				Text = Format("<c %x>%s</c>", purple, text)
			},
			bottomline = 
			{
				Top = "100% - 0.1em",
				BackgroundColor = purple
			}
		},
		contents =
		{
			Style = GUI_GridLayout | GUI_FitChildren,
			Top = "1.5em", Left = "0.5em", Bottom = "100% - 0.5em", Right = "100% - 0.5em"
		}
	};
	
	var selection, perks = false;
	if (what == "perks")
	{
		perks = true;
		selection = GetPossiblePerks(menu_object->GetOwner());
	}
	else selection = GetBuyableObjects(menu_object->GetOwner());
	
	var counter = 0;
	for (var obj in selection)
	{
		if (!perks && !obj->Call(what)) continue;
		++counter;
		
		// prepare desc graphics
		var dummy = CreateContents(Dummy);
		var desc_ID = nil;//GUI_CaedesTextContainer;
		var desc = obj->~GetDescID();
		if (GetType(desc) == C4V_Def)
		{
			desc_ID = desc;
			desc = "Info";
		}
		dummy->SetGraphics(desc, desc_ID, 1, GFXOV_MODE_Picture, nil, GFX_BLIT_Parent);
		
		var item =
		{
			BackgroundColor = {Std = RGB(50, 50, 50), OnHover = RGB(100, 0, 0)},
			Right = "6em", Bottom = "2.5em", Margin = "0.005em",
			icon =
			{
				Margin = "0.25em",
				Right = "2.5em",
				Symbol = GetTitleDef(obj)
			},
			desc =
			{
				Left = "2.5em",
				Symbol = dummy,
				Style = GUI_TextBottom | GUI_TextRight,
				Text = Format("%d Credits", obj->GetValue())
			},
			OnMouseIn = GuiAction_SetTag("OnHover"),
			OnMouseOut = GuiAction_SetTag("Std"),
			OnClick = GuiAction_Call(this, "OnItemSelection", obj)
		};
		if (what == "perks")
			item.desc.Text = nil;
		var extramargin = {Priority = counter, item = item, Left = item.Left, Top = item.Top, Right = item.Right, Bottom = item.Bottom};
		GuiAddSubwindow(extramargin, menu.contents);
	}
	return menu;
}


public func OnItemSelection(proplist item)
{
	var plr = menu_object->GetOwner();
	var ID = item;
	
	if(ID->~IsWeapon())
	{
		if(BuyWeapon(plr, ID))
		{
			//item->SetCount(item->GetCount() + 1);
		}
		return 1;
	}
	else if(ID->~IsPerk())
	{
		if(HasPerk(plr, ID)) return 1;
		
		AddPerk(plr, ID);
		
		if(PerkChangeDenied(plr))
		{
			CustomMessage("<c ff0000>$PerkNextRound$</c>", nil, plr);
			Sound("Munch?", false, nil, plr);
		}
		else
		{
			menu_object->Sound("Breathing", false, 50);
			
			var flm = CreateObject(FloatingMessage, menu_object->GetX() - GetX(), menu_object->GetY() - GetY());
			flm->SetMessage(Format("Perk: %s", ID->GetName()));
			flm->FadeOut(5, 10);
			flm->SetColor(255, 255, 100); 
			flm.Visibility = VIS_Allies | VIS_Owner | VIS_God;
		}
		return 1;
	}
	else if(ID->~IsSpecial())
	{
		ID->~Apply(menu_object);
	}
	return 0;
}

func BuyWeapon(plr, ID)
{
	if(GameCall("QueryRejectPurchase", plr, ID))
	{
		Sound("Nope", false, nil, plr);
		return false;
	}
	
	if(ID->~Exclusive())
		if(menu_object->FindContents(ID))
		{
			var returnval = Menu_ReloadWeapons->TryReload(menu_object, menu_object->FindContents(ID));
			if(returnval > 0)
				menu_object->Sound("Cash", false, nil, plr);
			else 
				if(returnval == 0)
					Sound("Nope", false, nil, plr);
			return false;
		}
	
	if(
		((menu_object->ContentsCount() >= menu_object.MaxContentsCount) && (!ID->~NeedsNoSlot()))
		|| ID->~RejectPurchase(menu_object)
		)
	{
		Sound("Nope", false, nil, plr);
		return false;
	}
			
	if(ID->GetValue() > GetWealth(plr))
	{
			Sound("Prrrrb", false, nil, plr);
			return false;
	}
	
	DoWealth(plr, -ID->GetValue());
	menu_object->CreateContents(ID);
	menu_object->Sound("Cash");
	
	var flm = CreateObject(FloatingMessage, menu_object->GetX() - GetX(), menu_object->GetY() - GetY());
	flm->SetMessage(Format("%d", -ID->GetValue()));
	flm->FadeOut(5, 10);
	flm->SetColor(100, 255, 100);
	flm.Visibility = VIS_Allies | VIS_Owner | VIS_God;
	return true; 
}

func Show(){}

func Close()
{
	RemoveObject();
}
