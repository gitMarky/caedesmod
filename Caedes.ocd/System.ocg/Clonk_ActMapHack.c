#appendto Clonk

local ActMap = {
Walk = {
	Prototype = Action,
	Name = "Walk",
	Procedure = DFA_WALK,
	Accel = 200,
	Decel = 200,
	Speed = 200,
	Directions = 2,
	FlipDir = 0,
	Length = 1,
	Delay = 0,
	X = 0,
	Y = 0,
	Wdt = 8,
	Hgt = 20,
	StartCall = "StartWalk",
	AbortCall = "StopWalk",
//	InLiquidAction = "Swim",
},
Stand = {
	Prototype = Action,
	Name = "Stand",
	Procedure = DFA_THROW,
	Directions = 2,
	FlipDir = 0,
	Length = 1,
	Delay = 0,
	X = 0,
	Y = 0,
	Wdt = 8,
	Hgt = 20,
	StartCall = "StartStand",
	InLiquidAction = "Swim",
},
Kneel = {
	Prototype = Action,
	Name = "Kneel",
	Procedure = DFA_KNEEL,
	Directions = 2,
	FlipDir = 0,
	Length = 1,
	Delay = 0,
	X = 0,
	Y = 0,
	Wdt = 8,
	Hgt = 20,
//	StartCall = "StartKneel",
	InLiquidAction = "Swim",
},
Roll = {
	Prototype = Action,
	Name = "Roll",
	Procedure = DFA_NONE,
	Directions = 2,
	FlipDir = 0,
	Length = 1,
	Delay = 0,
	X = 0,
	Y = 0,
	Wdt = 8,
	Hgt = 20,
	StartCall = "OnStartRoll",
	AbortCall = "OnAbortRoll",
	NextAction = "Walk",
	InLiquidAction = "Swim",
},
Scale = {
	Prototype = Action,
	Name = "Scale",
	Procedure = DFA_SCALE,
	Speed = 60,
	Accel = 60,
	Directions = 2,
	Length = 1,
	Delay = 0,
	X = 0,
	Y = 20,
	Wdt = 8,
	Hgt = 20,
	OffX = 0,
	OffY = 0,
	StartCall = "StartScale",
	AbortCall = "StopScale",
},
Tumble = nil,
Dig = {
	Prototype = Action,
	Name = "Dig",
	Procedure = DFA_DIG,
	Speed = 50,
	Directions = 2,
	Length = 16,
	Delay = 0,//15*3*0,
	X = 0,
	Y = 60,
	Wdt = 8,
	Hgt = 20,
	NextAction = "Dig",
	StartCall = "StartDigging",
	AbortCall = "StopDigging",
	DigFree = 11,
//	InLiquidAction = "Swim",
	Attach = CNAT_Left | CNAT_Right | CNAT_Bottom,
},
BridgeStand = {
	Prototype = Action,
	Name = "BridgeStand",
	Procedure = DFA_THROW,
	Directions = 2,
	Length = 16,
	Delay = 1,
	X = 0,
	Y = 60,
	Wdt = 8,
	Hgt = 20,
	NextAction = "BridgeStand",
	StartCall = "StartStand",
	InLiquidAction = "Swim",
},
BridgeScale = {
	Prototype = Action,
	Name = "BridgeScale",
	Procedure = DFA_THROW,
	Directions = 2,
	Length = 16,
	Delay = 1,
	X = 0,
	Y = 60,
	Wdt = 8,
	Hgt = 20,
	NextAction = "BridgeScale",
	InLiquidAction = "Swim",
},
Swim = {
	Prototype = Action,
	Name = "Swim",
	Procedure = DFA_SWIM,
	Speed = 96,
	Accel = 7,
	Decel = 9,
	Directions = 2,
	Length = 1,
	Delay = 0,
	X = 0,
	Y = 80,
	Wdt = 8,
	Hgt = 20,
	OffX = 0,
	OffY = 0,
//	SwimOffset = -5,
	StartCall = "StartSwim",
	AbortCall = "StopSwim",
	Sound = "Clonk::Movement::DivingLoop*",
},
Hangle = {
	Prototype = Action,
	Name = "Hangle",
	Procedure = DFA_HANGLE,
	Speed = 48,
	Accel = 20,
	Directions = 2,
	Length = 1,
	Delay = 0,
	X = 0,
	Y = 100,
	Wdt = 8,
	Hgt = 20,
	OffX = 0,
	OffY = 3,
	StartCall = "StartHangle",
	AbortCall = "StopHangle",
	InLiquidAction = "Swim",
},
Jump = {
	Prototype = Action,
	Name = "Jump",
	Procedure = DFA_FLIGHT,
	Speed = 200,
	Accel = 50,
	Directions = 2,
	Length = 1,
	Delay = 0,
	X = 0,
	Y = 120,
	Wdt = 8,
	Hgt = 20,
	InLiquidAction = "Swim",
	PhaseCall = "CheckStuck",
	StartCall = "StartJump",
},
WallJump = {
	Prototype = Action,
	Name = "WallJump",
	Procedure = DFA_FLIGHT,
	Speed = 200,
	Accel = 14,
	Directions = 2,
	Length = 1,
	Delay = 0,
	X = 0,
	Y = 120,
	Wdt = 8,
	Hgt = 20,
	InLiquidAction = "Swim",
	PhaseCall = "CheckStuck",
},
Dive = {
	Prototype = Action,
	Name = "Dive",
	Procedure = DFA_FLIGHT,
	Speed = 200,
	Accel = 16,
	Directions = 2,
	Length = 8,
	Delay = 4,
	X = 0,
	Y = 160,
	Wdt = 8,
	Hgt = 20,
	NextAction = "Hold",
	ObjectDisabled = 1,
	InLiquidAction = "Swim",
	PhaseCall = "CheckStuck",
},
Dead = {
	Prototype = Action,
	Name = "Dead",
	Directions = 2,
	X = 0,
	Y = 240,
	Wdt = 8,
	Hgt = 20,
	Length = 1,
	Delay = 0,
	NextAction = "Hold",
	StartCall = "StartDead",
	NoOtherAction = 1,
	ObjectDisabled = 1,
},
Ride = {
	Prototype = Action,
	Name = "Ride",
	Procedure = DFA_ATTACH,
	Directions = 2,
	Length = 1,
	Delay = 0,
	X = 128,
	Y = 120,
	Wdt = 8,
	Hgt = 20,
	StartCall = "StartRiding",
	AbortCall = "StopRiding",
	InLiquidAction = "Swim",
},
Push = {
	Prototype = Action,
	Name = "Push",
	Procedure = DFA_PUSH,
	Speed = 200,
	Accel = 100,
	Directions = 2,
	Length = 8,
	Delay = 15,
	X = 128,
	Y = 140,
	Wdt = 8,
	Hgt = 20,
	NextAction = "Push",
	StartCall = "StartPushing",
	AbortCall = "StopPushing",
	InLiquidAction = "Swim",
},
Build = {
	Prototype = Action,
	Name = "Build",
	Directions = 2,
	Length = 8,
	Delay = 15,
	X = 128,
	Y = 140,
	Wdt = 8,
	Hgt = 20,
	NextAction = "Build",
	InLiquidAction = "Swim",
	Attach = CNAT_Bottom,
},
HangOnto = {
	Prototype = Action,
	Name = "HangOnto",
	Procedure = DFA_ATTACH,
	Directions = 2,
	Length = 1,
	Delay = 0,
	X = 128,
	Y = 120,
	Wdt = 8,
	Hgt = 20,
	StartCall = "StartHangOnto",
	AbortCall = "AbortHangOnto",
	InLiquidAction = "Swim",
},
Eat = {
	Prototype = Action,
	Name = "Eat",
	Procedure = DFA_NONE,
	Directions = 2,
	Length = 1,
	Delay = 45,
	X = 0,
	Y = 0,
	Wdt = 8,
	Hgt = 20,
	StartCall = "StartEat",
	NextAction = "Walk",
	InLiquidAction = "Swim",
	Attach=CNAT_Bottom,
},
};

func Hit(int oldx, int oldy)
{
	if (IsJumping())
	{
		if (GetContact(-1, CNAT_Top) && (oldy < 0))
		{
			if (this.jumpfx)
				RemoveEffect(nil, nil, this.jumpfx);
			SetXDir(oldx, 100);
			SetYDir(Max(GetYDir(), 5));
		}
	}
	
	return inherited(oldx, oldy, ...);
}

func FxCheckComDirStart(target, fx, temp)
{
	if (temp) return;
	SetComDir(COMD_None);
}

func FxCheckComDirTimer(target, fx, time)
{
	if (GetPlayerControlState(GetOwner(), CON_Left) && (GetComDir() != COMD_Left) && !GetContact(-1, CNAT_Left))
	{
		SetComDir(COMD_Left);
		return -1;
	}
	if (GetPlayerControlState(GetOwner(), CON_Right) && (GetComDir() != COMD_Right) && !GetContact(-1, CNAT_Right))
	{
		SetComDir(COMD_Right);
		return -1;
	}
}

local VariableJumpHeightEffect = new Effect
{
  Timer = func(int time)
  {
    if (time > 10) return FX_Execute_Kill;
    if (!Target) return FX_Execute_Kill;
    if (Target->GetProcedure() != DFA_FLIGHT) return FX_Execute_Kill;
    var ydir = Target->GetYDir(100);
    if (ydir > 0) return FX_Execute_Kill;
    var plr = Target->GetController();   
    if (GetPlayerType(plr) == C4PT_User)
    {
      if (GetPlayerControlState(plr, CON_Up))
      {
        Target->SetYDir(ydir - this.Target.JumpAcceleration, 100);
      }
    }
    return FX_OK;
  },

  Destruction = func()
  {
    if (Target) Target.jumpfx = nil;
  }
};

public func ControlJumpExecute(int ydir, ...)
{
  if (inherited(ydir, ...))
  {
    // Add a timer to adjust vertical speed if not holding the jump key
    if (this.jumpfx) RemoveEffect(nil, nil, this.jumpfx);
    if (this.JumpAcceleration != nil)
    	this.jumpfx = CreateEffect(VariableJumpHeightEffect, 1, 1);
    return true;
  }
  return false;
}