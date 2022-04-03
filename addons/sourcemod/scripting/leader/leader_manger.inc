/*
 * =============================================================================
 * File:		  leader_manger.inc
 * Type:		  Base
 * Description:   Plugin's base file.
 *
 * Copyright (C)   Anubis Edition. All rights reserved.
 * =============================================================================
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License, version 3.0, as published by the
 * Free Software Foundation.
 * 
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * As a special exception, AlliedModders LLC gives you permission to link the
 * code of this program (as well as its derivative works) to "Half-Life 2," the
 * "Source Engine," the "SourcePawn JIT," and any Game MODs that run on software
 * by the Valve Corporation.  You must obey the GNU General Public License in
 * all respects for all other code used.  Additionally, AlliedModders LLC grants
 * this exception to all derivative works.  AlliedModders LLC defines further
 * exceptions, found in LICENSE.txt (as of this writing, version JULY-31-2007),
 * or <http://www.sourcemod.net/license.php>.
 */

void LeaderManegerOnCommandsCreate()
{
	RegConsoleCmd("sm_currentleader", CurrentLeader);
	RegConsoleCmd("sm_cl", CurrentLeader);
	RegAdminCmd("sm_removeleader", RemoveTheLeader, ADMFLAG_GENERIC);
	RegAdminCmd("sm_rl", RemoveTheLeader, ADMFLAG_GENERIC);
}

public void SetLeader(int client)
{
	if(IsValidClient(g_iLeaderClient))
	{
		RemoveLeader(g_iLeaderClient);
		CPrintToChatAll("%t", "Leader has been removed!");
	}

	if(IsValidClient(client))
	{
		g_iLeaderClient = client;
	}

	for (int i = 1; i <= MAXPLAYERS; i++)
	{
		g_iVotedFor[i] = -1;
		g_iVotedRemovedFor[i] = -1;
		g_iVoteCount[i] = 0;
		g_iVoteRemoveCount[i] = 0;
	}
}

public void SetLeaderHelper(int client)
{
	if(IsValidClient(g_iHelperClient))
	{
		RemoveLeader(g_iHelperClient);
		CPrintToChatAll("%t", "Leader has been removed!");
	}

	if(IsValidClient(client))
	{
		g_iHelperClient = client;
		CPrintToChatAll("%t", "The new leader", client);
		LeaderMenu(client);
	}
}

public Action RemoveTheLeader(int client, int args)
{
	if(IsValidClient(g_iLeaderClient))
	{
		CPrintToChatAll("%t", "Leader has been removed!");
		RemoveLeader(g_iLeaderClient);

		for (int i = 1; i <= MAXPLAYERS; i++)
		{
			g_iVotedFor[i] = -1;
			g_iVotedRemovedFor[i] = -1;
			g_iVoteCount[i] = 0;
			g_iVoteRemoveCount[i] = 0;
		}
		return Plugin_Handled;
	}
	else
	{
		CPrintToChat(client, "%t", "No current leader");
		return Plugin_Handled;
	}
}

public void RemoveLeader(int client)
{
	RemoveSprite(client);
	RemoveAllMarker(client);
	KillBeacon(client);
	RemoveSkin(client);
	RemoveMute();
	if (g_iLeaderClient == client) g_iLeaderClient = -1;
	if (g_iHelperClient == client) g_iHelperClient = -1;
	
}

public Action CurrentLeader(int client, int args)
{
	if(IsValidClient(g_iLeaderClient))
	{
		CPrintToChat(client, "%t", "The current leader", g_iLeaderClient);
		return Plugin_Handled;
	}
	else
	{
		CPrintToChat(client, "%t", "No current leader");
		return Plugin_Handled;
	}
}

public void LeaderManagerOnClientDisconnect(int client)
{
	if(client == g_iLeaderClient)
	{
		CPrintToChatAll("%t", "The leader has disconnected");
		RemoveLeader(client);
		RemoveMute();
	}
	else if(client == g_iHelperClient)
	{
		CPrintToChatAll("%t", "The leader has disconnected");
		RemoveLeader(client);
		RemoveMute();
	}
}

public void LeaderManagerEventPlayerDeath(int client)
{
	if(client == g_iLeaderClient)
	{
		CPrintToChatAll("%t", "The leader has died");
		RemoveLeader(client);
	}
	else if(client == g_iHelperClient)
	{
		CPrintToChatAll("%t", "The leader has died");
		RemoveLeader(client);
	}
}

void LeaderManagerOnMapEndStart()
{
	if(IsValidClient(g_iLeaderClient)) RemoveLeader(g_iLeaderClient);
	if(IsValidClient(g_iHelperClient)) RemoveLeader(g_iHelperClient);
}

void LeaderManagerEventRoundStartEnd()
{
	if(g_eCvarsList.g_bCVAR_ROUND_END_RELEADER)
	{
		if(IsValidClient(g_iLeaderClient))
		{
			RemoveSprite(g_iLeaderClient);
			RemoveAllMarker(g_iLeaderClient);
			RemoveSkin(g_iLeaderClient);
		}
		if(IsValidClient(g_iHelperClient))
		{
			RemoveSprite(g_iHelperClient);
			RemoveAllMarker(g_iHelperClient);
			RemoveSkin(g_iHelperClient);
		}
	}
}