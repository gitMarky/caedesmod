
#appendto Clonk

// Should be made nicer..
func DoRoll() {}
func DoKneel() {}

func CreateMuzzleFlash(int x, int y, int angle, int size)
{
	// main muzzle flash
	CreateParticle("MuzzleFlash", x, y, 0, 0, 10, {Prototype = Particles_MuzzleFlash(), Size = size, Rotation = angle});
	// and some additional little sparks
	var xdir = Sin(angle, size * 2);
	var ydir = -Cos(angle, size * 2);
	CreateParticle("StarFlash", x, y, PV_Random(xdir - size, xdir + size), PV_Random(ydir - size, ydir + size), PV_Random(5, 15), Particles_Glimmer(), size);
}

func Recruitment()
{
	this.ThrowSpeed *= 2;
	this.JumpSpeed = 500;
	
	if(this.ActMap == this.Prototype.ActMap)
	{
		this.ActMap = {Prototype = this.Prototype.ActMap};
		this.ActMap.Jump = {Prototype = this.ActMap.Jump};
	}
	this.ActMap["Hangle"] = nil;
	this.ActMap["Scale"] = nil;
	
	//PushActionSpeed("Scale", 150);
	//PushActionSpeed("Hangle", 150);
	
	//PushActionSpeed("Walk", 200);
	AddEffect("OverallDamageStuff", this, 20, 0, this);
	AddEffect("IntAimRestarter", this, 1, 50+Random(10), this);
	return _inherited(...);
}

func SetLastDamagingWeapon(ID)
{
	this.last_damaging_weapon = ID;
}

func Destruction()
{
	/*if(GetAlive())
	{
		var re = GetEffect("LogDamage", this);
		Log("DMG#4: Clonkr emove at %d|%d, %dHP, %down, %dre", GetX(), GetY(), GetEnergy(), GetOwner(), re);
	}*/
	return _inherited(...);
}

func BloodSplatter(level)
{
	var particles = 
	{
		Size = PV_Random(1, 2),
		R = 255, G = 1, B = 1,
		Attach = ATTACH_Front
	};
	FindObject(Find_ID(CallbackRule))->CreateParticle("SmokeDirty", PV_Random(-7, 7), PV_Random(-7, 7), 0, 0, 0, particles, level);
}

func CatchBlow(level, by)
{
	//if(GetEffect("LogDamage", this)) Log("DMG: Hit by %s for %d|%i - %v", by->GetName(), level, by->GetID(), by);
	if(by)
		SetLastDamagingWeapon(Symbol_DeathByObject);
	BloodSplatter(level);
	return _inherited(level, by, ...);
}

func FxOverallDamageStuffDamage(pTarget, iEffectNumber, iDmgEngy, iCause, iBy)
{
	var iPlr = pTarget->GetOwner();
	if(iDmgEngy < 0)
	{
  		pTarget.kill_bounty = 0;
		if(pTarget->GetEnergy() + iDmgEngy/1000 <= 0)
		{
			var hands = [GetHandItem(0), GetHandItem(1)];
			for(var obj in FindObjects(Find_Container(pTarget)))
			{
				if(GetIndexOf(hands, obj) != -1) continue;
				
				if(!obj->~NoRemove())
				{
					AddEffect("ScheduledRemove", obj, 1, 1, nil, Clonk, pTarget);
					pTarget.kill_bounty += Max(1, (obj->GetValue() / 5)) * 2;
				}
			}
  		}
   

		if(iPlr != iBy)
		{
			if(IsAllied(iPlr, iBy))
			{
				var crew=GetCrew(iBy);
				if(crew)
				{
					var dmg=iDmgEngy/2;
					//crew->DoEnergy(dmg, crew, 1, iCause, iBy+1);
					crew->DoDmg(-dmg, nil, crew, 1, nil, crew, Rule_Restart);
					//PlayerMessage(iBy, "<c ff0000>$teamdamage$</c>");
					iDmgEngy/=2;
				}
			}
			else
				if(Hostile(iPlr, iBy))
				{
					Caedes_player_round_damage[iBy] += Min(-iDmgEngy, pTarget->GetEnergy() * 1000);
				}
			
			// You see the damage you deal to enemies.
			if (iBy != NO_OWNER)
			{
				var helper = FindObject(Find_ID(FloatingMessage), Find_Owner(iBy), Find_ActionTarget(this));
				if (!helper)
				{
					helper = CreateObject(FloatingMessage, nil, -5, iBy);
					helper.Visibility = VIS_Owner;
					helper.damage = 0;
					helper->SetAction(helper->GetAction(), this, nil, true);
					helper->FadeOut(1, 3);
					helper->SetYDir(-1);
				}
				helper.damage += -iDmgEngy;
				helper.alpha = 255;
				helper->SetPosition(GetX(), GetY() - 5);
				helper->SetMessage(Format("-%d", helper.damage / 1000));
				var clr = BoundBy(helper.damage / 100, 0, 255);
				helper->SetColor(clr, 255 - clr, 0);
			}
		}
	}

	return iDmgEngy;
}

func FxScheduledRemoveStart(target, effect, iTemp, p)
{
	if(iTemp)return;
	effect.clonk = p;
}

func FxScheduledRemoveStop(target, effect, iReason, iTemp)
{
	if(iTemp)return;
	if(effect.clonk)
		if(effect.clonk->GetAlive())return;
	if(!target) return;
	if(target->Contained()) return;
	//Log("DMG#3: remove %s", target->GetName());
	target->RemoveObject();
}

func OnHit(dmg, type, from_obj, weapon)
{
	//if(GetEffect("LogDamage", this)) 
	//	Log("DMG#2: Hit by %s for %d|%i - %v", from_obj->GetName(), dmg, weapon, weapon);
	
	BloodSplatter(dmg);
}

func FxFadeOutTimer(target, effect, time)
{
	effect.fade += effect.fade_speed;
	target->SetClrModulation(RGBa(255, 255, 255,effect.fade));
	if(effect.fade >= 250)
		target->RemoveObject();
	return 1;
}

func Death()
{
	var e = AddEffect("FadeOut", this, 1, 2, this);
	e.fade_speed = 20;
	CastObjects(Flesh, 8, 50);
	return _inherited(...);
}


func UpdateHUD()
{
	if(!this.hud_helper)
	{
		this.hud_helper = FindObject(Find_ID(Caedes_HUDHelper), Find_Owner(GetOwner()));
		if(!this.hud_helper)
			this.hud_helper = CreateObject(Caedes_HUDHelper, 0, 0, GetOwner());
	}
	var msg = "@";
	
	if(Caedes_IsBombPlanted)
	{
		var color = RGB(255, 0, 0);
		if(GetPlayerTeam(GetOwner()) == Caedes_BombingTeam)
			color = RGB(0, 255, 0);
		msg = Format("%s<c %x>BOMB PLANTED!</c>|", msg, color);
	}
	
	var weapon = GetHandItem(0); // left hand
	if(weapon)
		if(weapon->~IsWeapon())
		if(weapon.weapon_properties != nil)
		{
			msg = Format("%s{{%i}} <c aaff00>%d/%d</c>", msg, GetTitleDef(weapon->GetID()), weapon.rounds, weapon.total_ammo);
		}
	var perks = GetPlayerPerks(GetOwner());
	{ // scope
		msg = Format("%s|Perks: ", msg);
		for(var p in perks)
			msg = Format("%s<c ff3333><i>%s</i></c> ", msg, p->GetName());
	}
	CustomMessage(msg, this.hud_helper, nil, 0, 0, 0xffffff, nil, nil, MSG_NoLinebreak | MSG_Left);
	return _inherited(...);
}

func RejectCollect(def, to_collect)
{
	// check if another weapon of same type is already held, if so: remove and reload
	if(to_collect->~IsFirstHandItem())
	{
		var obj, i = 0;
		var found = false;
		while(obj = Contents(i++))
		{
			if(obj->GetID() != def) continue;
			found = true;
			
			if(obj.weapon_properties)
			{
				obj.total_ammo = BoundBy(obj.total_ammo + to_collect.rounds + to_collect.total_ammo, 0, obj.weapon_properties.max_ammo);
				UpdateHUD();
			}
			break;
		}
		
		if(found)
		{
			Sound("ShortReload");
			to_collect->RemoveObject();
			return true;
		}
		
	}
	return _inherited(def, to_collect, ...);
}

func DoFalconPunch()
{
	if(Contained()) return;
	if(GetAction() != "Walk") return;
	if(GetEffect("FalconPunch", this)) return;
	AddEffect("FalconPunch", this, 1, 1, this);
}

func FxFalconPunchStart(target, effect, temp)
{
	if(temp) return;
	this->SetCrewEnabled(false);
	SetComDir(COMD_Stop);
	SetSpeed();
	effect.anim = PlayAnimation("Magic", 10, Anim_Linear(0, 0, GetAnimationLength("Magic"), 40, ANIM_Remove), Anim_Const(1000));
}

func FxFalconPunchTimer(target, effect, time)
{
	if(GetAction() != "Walk")
	{
		StopAnimation(effect.anim);
		return -1;
	}
	
	if(time < 30)
	{
		/*CXreateParticleAtBone("MagicFire", "pos_hand1", [0, 0, 0], [0, 0, 0], 40, RGB(200, 200, 200), target);
		CXreateParticleAtBone("MagicFire", "pos_hand2", [0, 0, 0], [0, 0, 0], 40, RGB(200, 200, 200), target);
		CXreateParticle("MagicRing", 0, 0, 0, 0, 50 + Cos(time * 2, 200), RGBa(255, 255, 255, 128), target);*/
	}
	else
	{
		//CXreateParticle("MagicRing", 0, 0, 0, 0, 60 + Sin((time - 30) * 6, 200), RGBa(255, 255, 255, 80), target);
		if(time >= 40) return -1;
		if(time == 30)
		{
			var x = 50;
			if(GetDir() == DIR_Left) x = -50;
			//DrawParticleLine2("MagicSpark", x / 10, 0, x, 0, 3, 40, RGB(200, 200, 200), 1, 1);
			for(var obj in FindObjects(Find_OnLine(0, 0, x, 0), Find_NoContainer(), Find_OCF(OCF_Alive), Find_Hostile(GetOwner()), Find_PathFree(this)))
			{
				DoDmg(100, nil, obj, nil, nil, this, Clonk);
			}
			Sound("Clonk::Verbal::Hurt*");
			effect.Interval = 1;
			return 1;
		}
	}
}

func FxFalconPunchStop(target, effect, reason, temp)
{
	if(temp) return;
	if(target) SetCrewEnabled(true);
	if(reason == FX_Call_Normal) return;
	StopAnimation(effect.anim);
}

func Hit()
{
	if (GetEffect("DoubleJumpCooldown", this))
		RemoveEffect("DoubleJumpCooldown", this);
	return inherited(...);
}

func IsAiming()
{
	var e = GetEffect("IntAim", this);
	if(!e || e.artificial) return false;
	return true;
}

func FxIntAimRestarterTimer(target, effect, time)
{
	// for a very quick check after cancelling the last aiming..
	if (effect.Interval == 1) effect.Interval = 10;
	
	// calling this assumes that the Clonk is not aiming currently
	var weapon = GetHandItem(0);
	if(!weapon) return;
	if(!weapon.weapon_properties) return;
	if(weapon.weapon_properties.delay_shot) return;
	if(target->GetAction() != "Walk" && target->GetAction() != "Jump") return;

	aim_set = weapon.animation_set;
	if(!aim_set) return;
	
	aim_weapon = weapon;
	
	if(aim_set["AnimationAim"] != nil)
	{
		if(aim_set["AimMode"] == AIM_Position)
			aim_animation_index = PlayAnimation(aim_set["AnimationAim"], 10, Anim_Const(GetAnimationLength(aim_set["AnimationAim"])/2), Anim_Const(1000));
		if(aim_set["AimMode"] == AIM_Weight)
		{
			aim_animation_index = PlayAnimation(aim_set["AnimationAim"],  10, Anim_Linear(0, 0, GetAnimationLength(aim_set["AnimationAim"]),  aim_set["AimTime"], ANIM_Loop), Anim_Const(1000));
			aim_animation_index = PlayAnimation(aim_set["AnimationAim2"], 10, Anim_Linear(0, 0, GetAnimationLength(aim_set["AnimationAim2"]), aim_set["AimTime"], ANIM_Loop), Anim_Const(1000), aim_animation_index);
			aim_animation_index++;
			SetAnimationWeight(aim_animation_index, Anim_Const(500));
		}
	}
	SetTurnType(1, 1);
	aim_stop = 0;
	var e = AddEffect("IntAim", this, 1, 2, this);
	e.artificial = true;
}

public func StartAim(object weapon, int angle)
{
	// special case: artificial aiming
	var e = GetEffect("IntAim", this);
	if(e && e.artificial)
	{
		RemoveEffect(nil, this, e);
	}
	return inherited(weapon, angle);
}

func FxIntAimStart(target, effect, temp)
{
	if(temp) return;
	var e = GetEffect("IntAimRestarter", this);
	if(e)
	{
		e.Interval = 0;
		SetPlayerControlEnabled(target->GetOwner(), CON_CaedesAimingCursor, true);
	}
}

func FxIntAimStop(target, effect, reason, temp)
{
	if(temp) return;
	var e = GetEffect("IntAimRestarter", target);
	if(e)
	{
		e.Interval = 1;
		SetPlayerControlEnabled(target->GetOwner(), CON_CaedesAimingCursor, false);
	}
	
	if(effect.artificial)
	{
		StopAnimation(GetRootAnimation(10));	
		aim_weapon = nil;
		aim_set = nil;
		SetTurnForced(-1);
		SetTurnType(0, -1);
		SetHandAction(0);
	}
}

func FxIntAimTimer(target, effect, time)
{
	if(effect.artificial)
	{
		var weapon = GetHandItem(0);
		if (!aim_weapon || weapon != aim_weapon) return -1;
		if  (aim_weapon->Contained() != target) return -1;
		if(target->GetAction() != "Walk" && target->GetAction() != "Jump") return -1;
	}
	
	var angle, delta_angle, length;
	var speed = aim_set["AimSpeed"];
	if(speed == nil) speed = 50;
	else speed *= 10;
	if(aim_angle < 0) SetTurnForced(DIR_Left);
	if(aim_angle > 0) SetTurnForced(DIR_Right);
	if(aim_set["AimMode"] == AIM_Position)
	{
		length = GetAnimationLength(aim_set["AnimationAim"]);
		angle = Abs(aim_angle)*10;//GetAnimationPosition(aim_animation_index)*1800/length;
		delta_angle = 0;//BoundBy(Abs(aim_angle*10)-angle, -speed, speed);
		SetAnimationPosition(aim_animation_index, Anim_Const( (angle+delta_angle)*length/1800 ));
	}
	if(aim_set["AimMode"] == AIM_Weight)
	{
		angle = Abs(aim_angle)*10;//GetAnimationWeight(aim_animation_index)*1800/1000;
		delta_angle = 0;//BoundBy(Abs(aim_angle*10)-angle, -speed, speed);
		SetAnimationWeight(aim_animation_index, Anim_Const( (angle+delta_angle)*1000/1800 ));
	}
	// We have reached the angle and we want to stop
	if(Abs(delta_angle) <= 5 && aim_stop == 1)
	{
		DoStopAim();
		return -1;
	}
}