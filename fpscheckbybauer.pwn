												/*-****************************
												***  FPS Check by Bauer     ***
												***  Made for ZB:SAMP       ***
												*** All rights reserverd 2013**
												*******************************
****************************************************************************************************************************/

//include
#include <a_samp>
#include <newcall>
#include <zcmd>
#include <foreach>
#include <sscanf2>
//new-ovi, definicije, skracenice...
#define LAGERI_FPS      1800
#define ZELENASVETLA 	0x80FF00FF
#define PLAVAS 			0x80FFFFFF
#define CRVENAT 		0xFF0000FF
#define SCM             SendClientMessage
#define SPD             ShowPlayerDialog

//td-ovi
new Text:Textdraw0;
new Text:Textdraw1;
new PlayerText:Textdraw2[MAX_PLAYERS];
new PlayerText:Textdraw3[MAX_PLAYERS];

#if defined FILTERSCRIPT

public OnFilterScriptInit()
{
	print("\n--------------------------------------");
	print(" FPS Check by Bauer");
	print("--------------------------------------\n");
	return 1;
}

public OnFilterScriptExit()
{
	return 1;
}

#else

main()
{
	print("\n----------------------------------");
	print(" Blank Gamemode by your name here");
	print("----------------------------------\n");
}

#endif

public OnGameModeInit()
{
	//td
    Textdraw0 = TextDrawCreate(22.000000, 110.000000, "FPS:");
	TextDrawBackgroundColor(Textdraw0, 255);
	TextDrawFont(Textdraw0, 2);
	TextDrawLetterSize(Textdraw0, 0.480000, 1.700000);
	TextDrawColor(Textdraw0, -16776961);
	TextDrawSetOutline(Textdraw0, 1);
	TextDrawSetProportional(Textdraw0, 1);

	Textdraw1 = TextDrawCreate(22.000000, 125.000000, "OLD FPS:");
	TextDrawBackgroundColor(Textdraw1, 255);
	TextDrawFont(Textdraw1, 2);
	TextDrawLetterSize(Textdraw1, 0.480000, 1.700000);
	TextDrawColor(Textdraw1, -16776961);
	TextDrawSetOutline(Textdraw1, 1);
	TextDrawSetProportional(Textdraw1, 1);
	return 1;
}

public OnPlayerRequestClass(playerid, classid)
{
	SetPlayerPos(playerid, 1958.3783, 1343.1572, 15.3746);
	SetPlayerCameraPos(playerid, 1958.3783, 1343.1572, 15.3746);
	SetPlayerCameraLookAt(playerid, 1958.3783, 1343.1572, 15.3746);
	return 1;
}

public OnPlayerSpawn(playerid)
{
	//td
	TextDrawShowForPlayer(playerid, Textdraw0);
	TextDrawShowForPlayer(playerid, Textdraw1);
	FpsTd(playerid);
	return 1;
}

public OnPlayerText(playerid, text[])
{
	return 1;
}

public OnPlayerClickPlayer(playerid, clickedplayerid, source)
{
	return 1;
}
public OnPlayerFPSChange(playerid, oldfps, newfps)
{
	new string[256],string1[256];
    format(string, 24, "~w~ %d", newfps);
    PlayerTextDrawSetString(playerid, Textdraw2[playerid], string);
    format(string1, 24, "~w~ %d", oldfps);
    PlayerTextDrawSetString(playerid, Textdraw3[playerid], string1);

    return 1;
}
//////////////////////////////////////////////////////////ZCMD Komande///////////////////////////////////////
CMD:proverifps(playerid, params[],help)
{
    if(IsPlayerAdmin(playerid))
    {
        new player, string[256];
        if(sscanf(params, "ud", player)) return SendClientMessage(playerid, -1, "FPS CHECK Pomoc | /proverifps [ID/Ime]");
		format(string, sizeof(string), "FPS CHECK | Igrac %s FPS: %d !", GetName(player), GetPlayerFPS(player));
        SendClientMessage(player, -1, string);
    }
    return 1;
}
//
CMD:lageri(playerid, params[],help)
{
    new string[256];
	if(IsPlayerAdmin(playerid))
	{
	    foreach(Player, i)
    	{
    	if (GetPlayerFPS(i) < 30)
        {
            format(string, sizeof(string), "Igrac %s ID: %s FPS:%d\n", GetName(i), i, GetPlayerFPS(i));
  		}
		}
	}
    return SPD(playerid, LAGERI_FPS, DIALOG_STYLE_LIST, "Lista lagera online", string, "OK", "");
}
//
//funkcije
stock GetName(playerid)
{
    new name[MAX_PLAYER_NAME];
    GetPlayerName(playerid, name, sizeof(name));
    return name;
}
//
stock FpsTd(playerid)
{
	Textdraw2[playerid] = CreatePlayerTextDraw(playerid,119.000000, 125.000000, "0");
	PlayerTextDrawBackgroundColor(playerid,Textdraw2[playerid], 255);
	PlayerTextDrawFont(playerid,Textdraw2[playerid], 2);
	PlayerTextDrawLetterSize(playerid,Textdraw2[playerid], 0.480000, 1.700000);
	PlayerTextDrawColor(playerid,Textdraw2[playerid], -1);
	PlayerTextDrawSetOutline(playerid,Textdraw2[playerid], 1);
	PlayerTextDrawSetProportional(playerid,Textdraw2[playerid], 1);

	Textdraw3[playerid] = CreatePlayerTextDraw(playerid,70.000000, 110.000000, "1");
	PlayerTextDrawBackgroundColor(playerid,Textdraw3[playerid], 255);
	PlayerTextDrawFont(playerid,Textdraw3[playerid], 2);
	PlayerTextDrawLetterSize(playerid,Textdraw3[playerid], 0.480000, 1.700000);
	PlayerTextDrawColor(playerid,Textdraw3[playerid], -1);
	PlayerTextDrawSetOutline(playerid,Textdraw3[playerid], 1);
	PlayerTextDrawSetProportional(playerid,Textdraw3[playerid], 1);
	PlayerTextDrawShow(playerid, Textdraw2[playerid]);
	PlayerTextDrawShow(playerid, Textdraw3[playerid]);
	return 1;
}
