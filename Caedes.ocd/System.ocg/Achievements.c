/*-- Achievements --*/

static aPlayerAchievements;
static aAchievementJedi;
static aAchievementLucker;
static aAchievementSupporter;
static aAchievementBobTheBuilder;
static aAchievementWrecker;
static aAchievementMisuse;
static aAchievementCCCCombo;

static fAchievementsActivated;

global func cdsInitAchievements()
{
 aPlayerAchievements=[];
 aAchievementJedi=[];
 aAchievementLucker=[];
 aAchievementSupporter=[];
 aAchievementBobTheBuilder=[];
 aAchievementWrecker=[];
 aAchievementMisuse=[];
 aAchievementCCCCombo=[];
 fAchievementsActivated=true;
}

global func GameOver()
{
 cdsAddAchievementsToEvaluation();
 return _inherited(...);
}

global func EliminatePlayer(par1,par2,par3,par4,par5)
{
 if(!fAchievementsActivated)cdsInitAchievements();
 cdsAddPlayerAchievementsToEvaluation(par1);
 return _inherited(par1,par2,par3,par4,par5);
}

global func cdsAddAchievementsToEvaluation()
{
 if(!fAchievementsActivated)cdsInitAchievements();
 for(var i=0;i<GetPlayerCount();i++)
 {
  var iPlr=GetPlayerByIndex(i);
  cdsAddPlayerAchievementsToEvaluation(iPlr);
 }
}

global func cdsAddPlayerAchievementsToEvaluation(iPlr)
{
 if(!(GetType(aPlayerAchievements[iPlr]) == C4V_Array))return;
  var sA="";
  for(var ID in aPlayerAchievements[iPlr])
  {
   sA=Format("%s{{%i}}",sA,ID);
  }
  AddEvaluationData(sA,GetPlayerID(iPlr));
}

global func cdsAddAchievement(id ID, int iPlr)
{
 if(!fAchievementsActivated)cdsInitAchievements();
 if(!(GetType(aPlayerAchievements[iPlr]) == C4V_Array))aPlayerAchievements[iPlr]=[];
 else
 {
  if(GetIndexOf(ID,aPlayerAchievements[iPlr]) != -1) return;
 }
 var cursor=GetCursor(iPlr);
 PushBack(ID, aPlayerAchievements[iPlr]);
 CustomMessage(Format("@<c ff0000>%s</c>|%s", ID->GetName(), ID.Description), nil, iPlr, -40, 50, 0xffffff, nil, ID, MSG_Right | MSG_Top, 180);
 GetCursor(iPlr)->Message(Format("@<c ff0000><i>%s</i></c>|{{%i}}",ID->GetName()),ID);
 //Schedule(Format("cdsClearAchievementMessage(%d,%i)",iPlr,ID),35*8);
 AddEffect("ClearAchievementMessage",0,1,35*8,0,0,iPlr,ID);
 
 Log("$GotAchievement$",GetTaggedPlayerName(iPlr), ID->GetName());
 
 Sound("Achievement01",0,cursor,0,iPlr);
}

global func FxClearAchievementMessageStart(pTarget,effect,iTemp,par1,par2)
{
 if(iTemp)return;
 effect.p1=par1;
 effect.p2=par2;
} 

global func FxClearAchievementMessageTimer(pTarget,effect,iEffectTime)
{
 cdsClearAchievementMessage(effect.p1,effect.p2);
 return -1;
}

global func cdsClearAchievementMessage(int iPlr, ID)
{
 CustomMessage("", 0, iPlr, 0, 0, 0, nil, ID, MSG_Right | MSG_Top);
 Message("",GetCursor(iPlr));
}

global func cdsGetAchievement(id ID, int iPlr)
{ 
 if(!fAchievementsActivated)cdsInitAchievements();
 if(!(GetType(aPlayerAchievements[iPlr]) == C4V_Array))aPlayerAchievements[iPlr]=[];
 else
 {
  if(GetIndexOf(ID,aPlayerAchievements[iPlr]) != -1) return;
 }
 
 return GetIndexOf(ID,aPlayerAchievements[iPlr]) != -1;
}

global func cdsAchievementJedi(int iPlr)
{
 if(cdsGetAchievement(AC01,iPlr))return;
 aAchievementJedi[iPlr]++;
 if(aAchievementJedi[iPlr] >= 5)
  cdsAddAchievement(AC01,iPlr);
}

global func cdsAchievementLucker(int iPlr)
{
 if(cdsGetAchievement(AC02,iPlr))return;
 aAchievementLucker[iPlr]++;
 if(aAchievementLucker[iPlr] >= 5)
  cdsAddAchievement(AC02,iPlr);
}

global func cdsAchievementSupporter(int iPlr)
{
 if(cdsGetAchievement(AC03,iPlr))return;
 aAchievementSupporter[iPlr]++;
 if(aAchievementSupporter[iPlr] >= 3)
  cdsAddAchievement(AC03,iPlr);
}

global func cdsAchievementRetreat(int iPlr)
{
 if(cdsGetAchievement(AC04,iPlr))return;
 cdsAddAchievement(AC04,iPlr);
}

global func cdsAchievementProxy(int iPlr)
{
 if(cdsGetAchievement(AC05,iPlr))return;
 cdsAddAchievement(AC05,iPlr);
}

global func cdsAchievementBobTheBuilder(int iPlr)
{
 if(cdsGetAchievement(AC06,iPlr))return;
 aAchievementBobTheBuilder[iPlr]++;
 if(aAchievementBobTheBuilder[iPlr] >= 10)
  cdsAddAchievement(AC06,iPlr);
}

global func cdsAchievementWrecker(int iPlr, bool fClear)
{
 if(cdsGetAchievement(AC07,iPlr))return;
 if(fClear)
 {
  aAchievementWrecker[iPlr]=0;
  return;
 }
 aAchievementWrecker[iPlr]++;
 if(aAchievementWrecker[iPlr] >= 3)
  cdsAddAchievement(AC07,iPlr);
}

global func cdsAchievementMisuse(int iPlr)
{
 if(cdsGetAchievement(AC08,iPlr))return;
 aAchievementMisuse[iPlr]++;
 if(aAchievementMisuse[iPlr] >= 3)
  cdsAddAchievement(AC08,iPlr);
}

global func cdsAchievementCCCCombo(int iPlr)
{
 if(cdsGetAchievement(AC09,iPlr))return;
 aAchievementCCCCombo[iPlr]++;
 if(aAchievementCCCCombo[iPlr] >= 3)
  cdsAddAchievement(AC09,iPlr);
}
