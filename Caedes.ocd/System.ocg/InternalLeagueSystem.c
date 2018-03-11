/*-- League System --*/

static const Z4LEAGUE_KEY_NAME="CaedesLeagueScore001";
static const Z4LEAGUE_AVERAGE_POINTS=30;
static const Z4LEAGUE_MAX_MU_CHANGE=30;
static const Z4LEAGUE_POINTS_FACTOR=1;

static const Z4LEAGUE_STD_POINTS = 500;
static const Z4LEAGUE_STD_MU = 100;
//static const Z4LEAGUE_NEWBIE_PROTECTION=250;

global func Z4GetLeagueRanks(iNum)
{
 if(GameCall("Z4HasOwnLeagueRanks"))
  return GameCall("Z4GetSpecificLeagueRanks",iNum);
 var ar=["Clonk", "Fähnrich", "Leutnant", "Hauptmann", "Major", "Oberst", "Brigadegeneral", "Generalmajor", "Generalleutnant", "General", "Feldmarschall", "Ritter", "Held", "Volksheld", "Fürst", "König", "Kaiser", "Kampfbuggi", "Phantom", "Gaya", "Gil-Galad", "Smaug", "Sauron", "Legolas", "Godfather", "Schwarzer Peter", "Wipfmetz", "Badewannenkapitän", "Der mit dem Wipf tanzt", "Olliphant", "Hornblower", "Devastor", "Ventilator", "Cycle", "Wurstwasser", "Bayer", "Pfälzer", "Nordbär", "Esper", "Meister", "Zorb", "Goldgräber", "Fling", "Luftschiff", "Nullpointer", "Sven2", "Svalinz Palist Goold", "Durarug"];
 if(iNum) return GetLength(ar);
 return ar;
}

global func Z4GetLeaguePoints(iPlr, fMu)
{
 if(!GetPlayerName(iPlr))return 0;
 var key = Z4LEAGUE_KEY_NAME;
 var std = Z4LEAGUE_STD_POINTS;
 if(fMu)
 {
 	key = Format("%smu", key);
 	std = Z4LEAGUE_STD_MU;
 }
 var dat=GetPlrExtraData(iPlr, key);
 if(!dat)
 {
  dat=std;
  SetPlrExtraData(iPlr,Z4LEAGUE_KEY_NAME,dat);
 }
  
 return dat;
}

global func Z4GetTeamScores(int team)
{
	var team_score = [];
	for(var i = 0; i < GetPlayerCount(); ++i)
	{
		var p = GetPlayerByIndex(i);
		if(GetPlayerType(p) != C4PT_User) continue;
		if(GetPlayerTeam(p) != team) continue;
		PushBack(team_score, [Z4GetLeaguePoints(p, false), Z4GetLeaguePoints(p, true)]);
	}
	return team_score;
}

global func Z4ResetAllLeagueScores()
{
	for(var i = 0; i < GetPlayerCount(); ++i)
	{
		var p = GetPlayerByIndex(i);
		
		var mukey = Format("%smu", Z4LEAGUE_KEY_NAME);
		SetPlrExtraData(p, Z4LEAGUE_KEY_NAME, 0);
		SetPlrExtraData(p, mukey, 0);
		Z4LeagueSystemRefreshScoreboard(p);
	}
}

global func Z4PlayersWonRound(players, against_players)
{
	var scores = [[], []];
	var avg_scores = [[], []];
	
	for(var two = 0; two <= 1; ++two)
	{
		var used_array = players;
		if(two == 1) used_array = against_players;
		for(var p in used_array)
		{
			if(GetPlayerType(p) != C4PT_User) continue;
			var s = Z4GetLeaguePoints(p, false);
			var m = Z4GetLeaguePoints(p, true);
			PushBack(scores[two], [s, m]);
			avg_scores[two][0] += s;
			avg_scores[two][1] += m;
		}
		
		if(!GetLength(scores[two])) return;
		
		avg_scores[two][0] /= GetLength(scores[two]);
		avg_scores[two][1] /= GetLength(scores[two]);
	}
	
	
	
	var WIN = 0, LOSE = 1, S = 0, MU = 1;
	
	var mu_difference = avg_scores[LOSE][MU] - avg_scores[WIN][MU];
	var mu_factor = 1;
	if(mu_difference < 0) mu_factor = -1;
	mu_difference = (mu_factor * (mu_difference ** 2)) / 10;
	
	var score_difference = (avg_scores[LOSE][S] - avg_scores[WIN][S]);
	var score_factor = 1;
	if(score_difference < 0) {score_factor = -1; score_difference = -score_difference;}
	
	score_difference = score_factor * Sqrt(Abs(score_difference));
	
	// S = AVG + (+/-) Sqrt((S_l - S_w))
	var score_gain = BoundBy(Z4LEAGUE_AVERAGE_POINTS + score_difference, 0, 2 * Z4LEAGUE_AVERAGE_POINTS);

	var gainloss = [score_gain, -score_gain];
	
	// adjust by uncertainty
	for(var side = 0; side <= 1; ++side)
	{
		// S = ((50 * S) / M_o) / 10;
		gainloss[side] = ((50 * gainloss[side]) / avg_scores[1 - side][MU]) / 10;
	}
	
	var individual_gainloss = [gainloss[WIN] / GetLength(scores[WIN]), gainloss[LOSE] / GetLength(scores[LOSE])];
	
	// adjust score of each side
	var mukey = Format("%smu", Z4LEAGUE_KEY_NAME);
	for(var side = 0; side <= 1; ++side)
	{
		var who = players;
		if(side == LOSE) who = against_players;
		var cnt = 0;
		for(var p in who)
		{
			if(GetPlayerType(p) != C4PT_User) continue;
			
			var mu = scores[side][cnt][MU];
			var s = scores[side][cnt][S];
			// M_i = (100 - Abs(S - S_i)/10) * (50 + M_i - M_o)
			var mu_fac = ((100 - Abs(avg_scores[side][S] - s)/10) * (50 + mu - avg_scores[1 - side][MU]));
			// M_i = M_i / -5000
			mu_fac /= -5000;
			Log("%s: avgmu1: %d, avgmu2: %d, mu: %d, mu_fac: %d", GetTaggedPlayerName(p), avg_scores[side][MU], avg_scores[1 - side][MU], mu, mu_fac);
			s = BoundBy(s + individual_gainloss[side], 1, 1000);
			mu = BoundBy( mu + BoundBy(mu_fac, -Z4LEAGUE_MAX_MU_CHANGE, Z4LEAGUE_MAX_MU_CHANGE), 1, 100);
			
			SetPlrExtraData(p, Z4LEAGUE_KEY_NAME, s);
			SetPlrExtraData(p, mukey, mu);
			
			Scoreboard->SetPlayerData(p, "league_score", Format("%d:%d", s, mu), s);
		
			++cnt;
		}
	}
}

global func Z4LeagueSystemInitScoreboard()
{
	Scoreboard->Init(
		[{key = "league_score", title = Goal_LastManStanding, sorted = true, desc = true, priority = 1, default = Z4LEAGUE_STD_POINTS}]
	);
}

global func Z4LeagueSystemRefreshScoreboard(int plr)
{
	var s = Z4GetLeaguePoints(plr, false);
	var mu = Z4GetLeaguePoints(plr, true);
	Scoreboard->SetPlayerData(plr, "league_score", Format("%d:%d", s, mu), s);
}

global func Z4GetPlrLeagueRank(iPlr)
{
 var points=Z4GetLeaguePoints(iPlr);
 if(!points)return "Null";
 
 var ranks=Z4GetLeagueRanks();
 var size=GetLength(ranks);
 points=BoundBy(points,1,size*100);
 return ranks[points/100];
}
