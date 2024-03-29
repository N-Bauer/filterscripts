// New SA-MP callbacks by Emmet
// Created on Sunday, January 26, 2014 at 11:58 AM.

#if !defined POTENTIAL_CRASH_DAMAGE
	#define POTENTIAL_CRASH_DAMAGE (55.0)
#endif

#if !defined POTENTIAL_SPEED_DROP
	#define POTENTIAL_SPEED_DROP (5.0)
#endif

enum e_CB_DATA {
	e_CB_TIME,
	e_CB_TIMER,
	e_CB_PRESSES,
	e_CB_LAST,
 	e_CB_VEND,
  	e_CB_FPS,
  	e_CB_DRUNK,

  	#if defined USE_OPTIMAL_MODE
  	    e_CB_TICK,
  	#endif

	bool:e_CB_INIT,
	bool:e_CB_UPDATE,
	bool:e_CB_PAUSED,
	bool:e_CB_HOLDING,
	Float:e_CB_HEALTH,
	Float:e_CB_PACKET,
	Float:e_CB_POS[3]
};

static stock
	g_CallbackData[MAX_PLAYERS][e_CB_DATA],
	g_DamageStatus[MAX_VEHICLES][4 char],
	Float:g_VehicleHealth[MAX_VEHICLES],
	Float:g_VehicleSpeed[MAX_VEHICLES]
;

static const Float:gVendingMachines[62][3] =
{
    {2480.9548, -1958.5217, 13.5830}, {2325.9185, -1645.9553, 14.8270},
    {2352.9932, -1357.1597, 24.3984}, {2153.8535, -1015.7230, 62.8835},
    {2140.2710, -1161.4998, 23.9922}, {2224.4141, -1153.4441, 1025.7969},
    {2060.1152, -1898.4554, 13.5538}, {1634.0741, -2238.2815, 13.5078},
    {1729.7947, -1943.8628, 13.5687}, {-16.1516, -90.8512, 1003.5469},
    {-17.5205, -90.9663, 1003.5469}, {1929.5516, -1772.4803, 13.5469},
    {496.0115, -23.5306, 1000.6797}, {500.6090, -2.1074, 1000.6797},
    {501.7604, -2.2185, 1000.6797}, {-19.1310, -57.0453, 1003.5469},
    {-36.1302, -57.1351, 1003.5469}, {1153.9125, -1460.9729, 15.7969},
    {1788.3973, -1369.2472, 15.7578}, {2575.7300, -1284.6108, 1060.9844},
    {662.4797, -551.4141, 16.3359}, {200.2021, -107.5868, 1.5513},
    {1278.5977, 372.2211, 19.5547}, {2271.7053, -77.2283, 26.5810},
    {-2119.4229, -422.2747, 35.5313}, {-2119.7649, -423.4770, 35.5313},
    {-2068.6082, -397.5225, 35.5313}, {-2039.8768, -397.5218, 35.5313},
    {-2011.1292, -397.5222, 35.5313}, {-2005.6145, -490.8686, 35.5313},
    {-2034.4011, -490.8754, 35.5313}, {-2063.2300, -490.8705, 35.5313},
    {-2092.0771, -490.8697, 35.5313}, {-2229.1521, 287.1645, 35.3203},
    {-1981.6025, 142.6991, 27.6875}, {-1349.2971, 492.2820, 11.1953},
    {-1349.3674, 493.9295, 11.1953}, {-2419.5833, 984.5712, 45.2969},
    {-2419.5457, 986.0020, 45.2969}, {-1455.0173, 2592.4175, 55.8359},
    {-252.9913, 2599.6885, 62.8582}, {-252.9274, 2597.8921, 62.8582},
    {-862.7581, 1537.4230, 22.5870}, {-75.2812, 1227.9269, 19.7359},
    {-14.7254, 1176.1750, 19.5634}, {1398.8198, 2223.3601, 11.0234},
    {1659.4318, 1722.0974, 10.8281}, {1519.3350, 1055.2886, 10.8203},
    {2503.1318, 1244.5094, 10.8203}, {2647.7424, 1128.9138, 11.1797},
    {2845.7429, 1294.2975, 11.3906}, {2320.0618, 2532.0364, 10.8203},
    {2086.5894, 2071.4111, 11.0579}, {-32.5264, -185.9041, 1003.5469},
    {-33.8699, -186.0213, 1003.5469}, {361.6433, 159.3575, 1008.3828},
    {374.7823, 188.2360, 1008.3893}, {351.6473, 206.1147, 1008.3828},
    {371.6005, 177.7106, 1019.9844}, {373.8686, -178.9308, 1000.6328},
    {378.2982, -178.9220, 1000.6328}, {315.9102, -140.5504, 999.6016}
};

forward OnPlayerPause(playerid);
forward OnPlayerResume(playerid, time);
forward OnPlayerHoldingKey(playerid, keys);
forward OnPlayerReleaseKey(playerid, keys, time);
forward OnPlayerFall(playerid, Float:height, Float:damage);
forward OnPlayerPacketLoss(playerid, Float:oldloss, Float:newloss);
forward OnPlayerUseVending(playerid);
forward OnPlayerCrashVehicle(playerid, vehicleid, Float:damage);
forward OnPlayerFPSChange(playerid, oldfps, newfps);

stock IsPlayerPaused(playerid)
	return (g_CallbackData[playerid][e_CB_PAUSED]);

stock GetPlayerPausedTime(playerid)
	return (IsPlayerPaused(playerid)) ? (GetTickCount() - g_CallbackData[playerid][e_CB_TIME]) : (0);

stock GetPlayerFPS(playerid)
	return (g_CallbackData[playerid][e_CB_FPS]);

stock IsPlayerNearVending(playerid)
{
    for (new i = 0; i < sizeof(gVendingMachines); i ++) if (IsPlayerInRangeOfPoint(playerid, 3.0, gVendingMachines[i][0], gVendingMachines[i][1], gVendingMachines[i][2])) {
		return 1;
	}
	return 0;
}

stock IsPlayerHoldingKey(playerid, keys)
{
	static
		iKeyData[2];

	GetPlayerKeys(playerid, iKeyData[0], iKeyData[1], iKeyData[1]);
	return (g_CallbackData[playerid][e_CB_HOLDING] && iKeyData[0] & keys);
}

stock Float:GetPlayerPacketLoss(playerid)
{
	static
	    stats[512],
		pos;

	GetPlayerNetworkStats(playerid, stats, 512);

	if ((pos = strfind(stats, "Packetloss", true)) != -1)
	{
		if ((pos = strfind(stats, "%", true, pos)) != -1)
		{
		    stats[pos] = '\0';
		    return floatstr(stats[pos - 3]);
		}
	}
	return 0.0;
}

stock static Float:OPCV_GetVehicleSpeed(vehicleid)
{
	static
	    Float:x,
	    Float:y,
	    Float:z,
		Float:speed;

	GetVehicleVelocity(vehicleid, x, y, z);
	speed = floatsqroot((x * x) + (y * y) + (z * z)) * 100;

	return speed;
}

public OnFilterScriptInit()
{
	SetTimer("@tab_timer", 600, true);

	#if defined USE_OPTIMAL_MODE
	for (new i = 0, j = GetMaxPlayers(); i != j; i ++)
	{
  		if (!IsPlayerConnected(i))
      		continue;

 		g_CallbackData[i][e_CB_TICK] = SetTimerEx("@tick_timer", 600, true, "d", i);
	}
	#endif

	#if defined CB_OnFilterScriptInit
	    CB_OnFilterScriptInit();
	#endif
	return 1;
}

public OnGameModeInit()
{
	SetTimer("@tab_timer", 600, true);

    #if defined USE_OPTIMAL_MODE
	for (new i = 0, j = GetMaxPlayers(); i != j; i ++)
	{
  		if (!IsPlayerConnected(i))
      		continue;

 		g_CallbackData[i][e_CB_TICK] = SetTimerEx("@tick_timer", 600, true, "d", i);
	}
	#endif

	#if defined CB_OnGameModeInit
	    CB_OnGameModeInit();
	#endif
	return 1;
}

public OnPlayerConnect(playerid)
{
	#if defined USE_OPTIMAL_MODE
	    g_CallbackData[playerid][e_CB_TICK] = SetTimerEx("@tick_timer", 600, true, "d", playerid);
	#endif

	#if defined CB_OnPlayerConnect
	    CB_OnPlayerConnect(playerid);
	#endif
	return 1;
}

public OnPlayerDisconnect(playerid, reason)
{
    #if defined USE_OPTIMAL_MODE
	    KillTimer(g_CallbackData[playerid][e_CB_TICK]);
	#endif

	#if defined CB_OnPlayerDisconnect
	    CB_OnPlayerDisconnect(playerid, reason);
	#endif
	return 1;
}

#if defined USE_OPTIMAL_MODE
	forward @tick_timer(playerid); public @tick_timer(playerid)
#else
	public OnPlayerUpdate(playerid)
#endif
{
	static
	    iDrunkLevel,
	    iState,
	    iAnimation;

	iDrunkLevel = GetPlayerDrunkLevel(playerid);
	iState = GetPlayerState(playerid);
	iAnimation = GetPlayerAnimationIndex(playerid);

	if (iDrunkLevel < 100)
		SetPlayerDrunkLevel(playerid, 2000);

	else if (g_CallbackData[playerid][e_CB_DRUNK] != iDrunkLevel)
	{
	    new
			amount = g_CallbackData[playerid][e_CB_DRUNK] - iDrunkLevel;

	    if (amount > 0 && g_CallbackData[playerid][e_CB_FPS] != amount)
	    {
	        CallLocalFunction("OnPlayerFPSChange", "ddd", playerid, g_CallbackData[playerid][e_CB_FPS], amount);
	        g_CallbackData[playerid][e_CB_FPS] = amount;
	    }
	    g_CallbackData[playerid][e_CB_DRUNK] = iDrunkLevel;
	}
	if (floatcmp(GetPlayerPacketLoss(playerid), g_CallbackData[playerid][e_CB_PACKET]) != 0)
	{
	    new
			Float:packet = GetPlayerPacketLoss(playerid)
		;
		if (floatcmp(packet, 0.0) != 0)
		{
		    CallLocalFunction("OnPlayerPacketLoss", "dff", playerid, g_CallbackData[playerid][e_CB_PACKET], packet);
    	    g_CallbackData[playerid][e_CB_PACKET] = packet;
		}
	}
	if (iState == PLAYER_STATE_ONFOOT)
	{
		if ((IsPlayerNearVending(playerid) && iAnimation == 1660) && (GetTickCount() - g_CallbackData[playerid][e_CB_VEND]) > 3500)
		{
			CallLocalFunction("OnPlayerUseVending", "d", playerid);
			g_CallbackData[playerid][e_CB_VEND] = GetTickCount();
		}
		if (!g_CallbackData[playerid][e_CB_INIT])
		{
		    g_CallbackData[playerid][e_CB_INIT] = true;

		    GetPlayerHealth(playerid, g_CallbackData[playerid][e_CB_HEALTH]);
		    GetPlayerPos(playerid, g_CallbackData[playerid][e_CB_POS][0], g_CallbackData[playerid][e_CB_POS][1], g_CallbackData[playerid][e_CB_POS][2]);
		}
		if (g_CallbackData[playerid][e_CB_INIT])
		{
			if (iAnimation < 1130 || iAnimation > 1132)
			{
				static
					Float:x,
					Float:y,
	   		     	Float:z,
					Float:health;

				GetPlayerPos(playerid, x, y, z);
				GetPlayerHealth(playerid, health);

				if ((g_CallbackData[playerid][e_CB_HEALTH] > health) && (g_CallbackData[playerid][e_CB_POS][2] > z))
				{
				    CallLocalFunction("OnPlayerFall", "dff", playerid, floatsub(g_CallbackData[playerid][e_CB_POS][2], z), floatsub(g_CallbackData[playerid][e_CB_HEALTH], health));
				}
				GetPlayerHealth(playerid, g_CallbackData[playerid][e_CB_HEALTH]);
			    GetPlayerPos(playerid, g_CallbackData[playerid][e_CB_POS][0], g_CallbackData[playerid][e_CB_POS][1], g_CallbackData[playerid][e_CB_POS][2]);
			}
			g_CallbackData[playerid][e_CB_UPDATE] = true;
			g_CallbackData[playerid][e_CB_LAST] = GetTickCount();
		}
	}
	else if (IsPlayerInAnyVehicle(playerid) && iState == PLAYER_STATE_DRIVER)
	{
	    static
	        vehicleid = INVALID_VEHICLE_ID,
			Float:health,
			Float:speed,
			status[4]
		;
		vehicleid = GetPlayerVehicleID(playerid);

		GetVehicleHealth(vehicleid, health);
		speed = OPCV_GetVehicleSpeed(vehicleid);

		if ((g_VehicleHealth[vehicleid] > health && floatabs(g_VehicleHealth[vehicleid] - health) >= POTENTIAL_CRASH_DAMAGE) && floatabs(g_VehicleSpeed[vehicleid] - speed) >= POTENTIAL_SPEED_DROP)
		{
			GetVehicleDamageStatus(vehicleid, status[0], status[1], status[2], status[3]);

			if (g_DamageStatus[vehicleid]{0} != status[0] || g_DamageStatus[vehicleid]{1} != status[1] || g_DamageStatus[vehicleid]{2} != status[2]) {
   				CallLocalFunction("OnPlayerCrashVehicle", "ddf", playerid, vehicleid, floatsub(g_VehicleHealth[vehicleid], health));
			}
			for (new i = 0; i < 4; i ++) {
  				g_DamageStatus[vehicleid]{i} = status[i];
			}
		}
		g_VehicleSpeed[vehicleid] = speed;
		g_VehicleHealth[vehicleid] = health;
	}
	#if !defined USE_OPTIMAL_MODE
		#if defined CB_OnPlayerUpdate
	    	CB_OnPlayerUpdate(playerid);
		#endif
	#endif
	return 1;
}

public OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
    if (!g_CallbackData[playerid][e_CB_HOLDING]) {
    	g_CallbackData[playerid][e_CB_HOLDING] = true;
    	g_CallbackData[playerid][e_CB_TIME] = GetTickCount();
		g_CallbackData[playerid][e_CB_TIMER] = SetTimerEx("@hold_timer", 200, true, "dd", playerid, newkeys);
	}
	#if defined CB_OnPlayerKeyStateChange
	    CB_OnPlayerKeyStateChange(playerid, newkeys, oldkeys);
	#endif
	return 1;
}

forward @hold_timer(playerid, keys); @hold_timer(playerid, keys)
{
	static
	    iKeyData[3];

	GetPlayerKeys(playerid, iKeyData[0], iKeyData[1], iKeyData[2]);

	if (iKeyData[0] & keys)
	{
	    g_CallbackData[playerid][e_CB_PRESSES]++;
	    CallLocalFunction("OnPlayerHoldingKey", "dd", playerid, keys);
	}
	else
	{
	    if (g_CallbackData[playerid][e_CB_PRESSES] > 0)
	    {
	        CallLocalFunction("OnPlayerReleaseKey", "ddd", playerid, keys, GetTickCount() - g_CallbackData[playerid][e_CB_TIME]);
	    }
	    g_CallbackData[playerid][e_CB_HOLDING] = false;
	    g_CallbackData[playerid][e_CB_PRESSES] = 0;
		KillTimer(g_CallbackData[playerid][e_CB_TIMER]);
	}
}

forward @tab_timer(); @tab_timer()
{
	for (new i = 0, j = GetMaxPlayers(); i != j; i ++)
	{
	    new iState = GetPlayerState(i);

		if (!IsPlayerConnected(i) || iState == PLAYER_STATE_NONE || iState == PLAYER_STATE_WASTED || iState == PLAYER_STATE_SPECTATING || (iState >= 4 && iState <= 6))
		    continue;

		if (g_CallbackData[i][e_CB_UPDATE])
		{
		    if (g_CallbackData[i][e_CB_PAUSED])
		    {
		        CallLocalFunction("OnPlayerResume", "dd", i, GetTickCount() - g_CallbackData[i][e_CB_TIME]);
			}
		    g_CallbackData[i][e_CB_UPDATE] = false;
		    g_CallbackData[i][e_CB_PAUSED] = false;
		}
		else if ((!g_CallbackData[i][e_CB_PAUSED]) && (GetTickCount() - g_CallbackData[i][e_CB_LAST]) >= 2000)
		{
		    g_CallbackData[i][e_CB_TIME] = GetTickCount();
            g_CallbackData[i][e_CB_PAUSED] = true;

            CallLocalFunction("OnPlayerPause", "d", i);
		}
	}
}

#if defined _ALS_OnFilterScriptInit
	#undef OnFilterScriptInit
#else
	#define _ALS_OnFilterScriptInit
#endif

#if defined _ALS_OnGameModeInit
	#undef OnGameModeInit
#else
	#define _ALS_OnGameModeInit
#endif

#if defined _ALS_OnPlayerConnect
	#undef OnPlayerConnect
#else
	#define _ALS_OnPlayerConnect
#endif

#if defined _ALS_OnPlayerDisconnect
	#undef OnPlayerDisconnect
#else
	#define _ALS_OnPlayerDisconnect
#endif

#if defined _ALS_OnPlayerKeyStateChange
	#undef OnPlayerKeyStateChange
#else
	#define _ALS_OnPlayerKeyStateChange
#endif

#if !defined USE_OPTIMAL_MODE
    #if defined _ALS_OnPlayerUpdate
		#undef OnPlayerUpdate
	#else
		#define _ALS_OnPlayerUpdate
	#endif
	#define OnPlayerUpdate CB_OnPlayerUpdate
#endif

#define OnFilterScriptInit  	CB_OnFilterScriptInit
#define OnGameModeInit 			CB_OnGameModeInit
#define OnPlayerConnect 		CB_OnPlayerConnect
#define OnPlayerDisconnect 		CB_OnPlayerDisconnect
#define OnPlayerKeyStateChange  CB_OnPlayerKeyStateChange

#if defined USE_OPTIMAL_MODE
	#if defined CB_OnPlayerUpdate
		forward CB_OnPlayerUpdate(playerid);
	#endif
#endif

#if defined CB_OnFilterScriptInit
	forward CB_OnFilterScriptInit();
#endif

#if defined CB_OnGameModeInit
	forward CB_OnGameModeInit();
#endif

#if defined CB_OnPlayerConnect
	forward CB_OnPlayerConnect(playerid);
#endif

#if defined CB_OnPlayerDisconnect
	forward CB_OnPlayerDisconnect(playerid, reason);
#endif

#if defined CB_OnPlayerKeyStateChange
	forward CB_OnPlayerKeyStateChange(playerid, newkeys, oldkeys);
#endif
