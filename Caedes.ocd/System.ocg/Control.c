
global func Control2Player(int plr, int ctrl, int x, int y, int strength, bool repeat, bool release)
{
	// cursor pos info - store in player values
	if (ctrl == CON_CursorPos)
	{
		if (!g_player_cursor_pos) g_player_cursor_pos = CreateArray(plr+1);
		g_player_cursor_pos[plr] = [x, y];
		return true;
	}
	
	if(ctrl == CON_CaedesAimingCursor)
	{
		var c = GetCursor(plr);
		if(c)
		{
			if(c.aim_stop) return true;
			c->~SetAimPosition(Normalize(Angle(c->GetX(), c->GetY(), x, y), -180));
		}
		return true;
	}
	
	// chat menu?
	if((ctrl == CON_Backpack) && !release)
	{
		var c = GetCursor(plr);
		if(!c) return true;
		if(c->Contained()) return true;

		var m = c->~GetMenu();
		if(m)
		{
			if(m->~GetSymbol() == ChatMenuController)
				m->Close();
			return true;
		}
		
		
		c->CreateChatMenu(g_player_cursor_pos[plr][0], g_player_cursor_pos[plr][1]);
		return true;
	}
	
	// reload!?
	if(ctrl == CON_NextCrew)
	{
		var c = GetCursor(plr);
		if(c)
		{
			var weapon = c->GetHandItem(0);
			if(weapon)
			if(weapon.weapon_properties != nil)
			{
				weapon->~CheckReload(true);
				return true;
			}
		}
	}
	
	if (ctrl == CON_Contents)
	{
		var c = GetCursor(plr);
		if (c)
		{
			if (c->GetMenu())
			{
				var was_buy_menu = c->GetMenu()->~IsBuyMenu();
				c->CancelMenu();
				if (was_buy_menu) return 1;
			}
			return !!c->OpenBuyMenu();
		}
	}
	
	// ffffalcon punch?
	if(ctrl == CON_PreviousCrew)
	{
		var c = GetCursor(plr);
		if(c)
		{
			c->~DoFalconPunch();
			return true;
		}
	}
	
	if (ctrl == CON_Jump)
	{
		var cursor = GetCursor(plr);
		if (cursor && !GetEffect("DoubleJumpCooldown", cursor) && cursor->IsJumping() && (cursor->GetYDir() <= 0))
		{
			var jumpspeed = cursor.JumpSpeed;
			cursor.JumpSpeed *= 2;
			cursor->SetAction("Climb", nil, nil, true);
			if (cursor->GetDir() == DIR_Right) cursor->SetDir(DIR_Left);
			else cursor->SetDir(DIR_Right);
			cursor->ControlJump();
			cursor.JumpSpeed = jumpspeed;
			cursor->AddEffect("DoubleJumpCooldown", cursor, 1, 30);
			return true;
		}
	}
	return inherited(plr, ctrl, x, y, strength, repeat, release);
}

global func OpenBuyMenu()
{
	// Safety checks.
	if (!this) return;
	if (!(GetOCF() & OCF_CrewMember)) return;
	if (!(this->~HasMenuControl())) return;
	if(Contained()) return 1;
	if(!ClonkCanBuy(this)) return 1;
	
	// Create the menu controller.
	var controller = CreateObject(BuyMenu_Controller, nil, nil, GetOwner());
		controller->SetMenuObject(this);
	this->SetMenu(controller);
	
	controller->OpenBuyMenu();

	
	// Show and return content menu.
	controller->Show();
	return controller;
}

global func ClonkCanBuy(clnk)
{
	var flag = GetFlagpoleForPosition(clnk->GetX() - GetX(), clnk->GetY() - GetY());
	if(!flag) return false;
	return flag.Team == GetPlayerTeam(clnk->GetOwner());
}