/**
*/

// this array contains all the menus
// index 0 = calling object
// index >0 = first crew, then containers
local circ_menus;
local menu_object;
local my_menu;

//func IsChatMenu(){return true;}

/** Creates a content menu for the calling object. This is supposed to be a crew member, 
	controlled by a player.
	@return a pointer to the created menu, or \c nil if failed.
*/
global func CreateChatMenu(x, y)
{
	// Safety checks.
	if (!this) return;
	if (!(GetOCF() & OCF_CrewMember)) return;
	if (!(this->~HasMenuControl())) return;

	// Create the menu controller.
	var controller = CreateObject(ChatMenuController,0, 0, this->GetOwner());
	controller->Setup(x, y, this);
	return controller;
}

/** Returns the object for which the menu is shown. */
public func GetMenuObject() { return menu_object; }


func SetMenuObject(object menuObject)
{
	menu_object = menuObject;
}

func Construction()
{
	circ_menus = [];
}

func Close() 
{
	RemoveObject();
}

func Destruction() 
{
	if(my_menu)
		my_menu->Close();
}

func Show(bool no_update) 
{
	my_menu->Show();
}

func Hide() 
{
	my_menu->Hide();
}

func Setup(x, y, clnk)
{
	menu_object = clnk;
	var self = this;
	var menu = menu_object->CreateChatRingMenu(ChatMenuController, this);//CreateObject(GUI_RingMenu, 0, 0, GetOwner());
	my_menu = menu;
	menu->SetPosition(x,y);

	//menu->SetSymbol(Clonk);
	//menu->SetMenuObject(menu_object);
	//menu->SetCommander(this);
	//menu->SetMenu(menu_object, this);
	//menu->SetDragDropMenu(false);
	
	
	var chat_ids = [ChatYes, ChatNo, ChatAttack, ChatDefend, ChatRetreat, ChatSupport, ChatGrenades, ChatBomb];
	
	for(var ID in chat_ids)
	{
		/*var item = CreateObject(GUI_MenuItem);
		Log("%v, : %v"
		if (!menu->AddItem(item))
		{
			item->RemoveObject();
			continue;
		}
		
		item->SetSymbol(ID);
		item->SetData(ID);*/
		menu->AddItem(ID, nil, ID);
	}

	menu->Show();

	//UpdateContentMenus();
	return;
}


/* Interface to menu item as commander_object */

public func OnMouseClick(int x, int y, bool alt)
{
	return;
}

func Selected(menu, item, alt)
{
	return OnItemSelection(menu, item);
}

public func OnItemSelection(object menu, object item)
{
	var chat = item->GetExtraData();
	
	var flm = CreateObject(FloatingMessage, menu_object->GetX() - GetX(), menu_object->GetY() - GetY());
	flm->SetMessage(chat->GetName());
	flm->FadeOut(3, 20);
	flm->SetColor(100, 255, 100);
	flm.Visibility = VIS_Allies | VIS_Owner | VIS_God;
	
	// play sound for allies
	var owner = menu_object->GetOwner();
	for(var i = 0; i < GetPlayerCount(); ++i)
	{
		var p = GetPlayerByIndex(i);
		if(Hostile(p, owner)) continue;
		menu_object->Sound(Format("%i*", chat), false, 75, p);
	}
	Close();
	return true;
}

public func OnItemSelectionAlt(object menu, object item)
{
	return OnItemSelection(menu, item);
}
