/* This file is part of the ScriptDev2 Project. See AUTHORS file for Copyright information
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/* ScriptData
SDName: Hyjal
SD%Complete: 80
SDComment:
SDCategory: Caverns of Time, Mount Hyjal
EndScriptData */

/* ContentData
npc_jaina_proudmoore
npc_thrall
npc_tyrande_whisperwind
npc_building_trigger
EndContentData */

#include "AI/ScriptDevAI/include/precompiled.h"
#include "hyjalAI.h"

UnitAI* GetAI_npc_jaina_proudmoore(Creature* creature)
{
    hyjalAI* tempAI = new hyjalAI(creature);

    tempAI->m_aSpells[0].m_uiSpellId = SPELL_BLIZZARD;
    tempAI->m_aSpells[0].m_uiCooldown = urand(15000, 35000);
    tempAI->m_aSpells[0].m_pType = TARGETTYPE_RANDOM;

    tempAI->m_aSpells[1].m_uiSpellId = SPELL_PYROBLAST;
    tempAI->m_aSpells[1].m_uiCooldown = urand(2000, 9000);
    tempAI->m_aSpells[1].m_pType = TARGETTYPE_RANDOM;

    tempAI->m_aSpells[2].m_uiSpellId = SPELL_SUMMON_ELEMENTALS;
    tempAI->m_aSpells[2].m_uiCooldown = urand(15000, 45000);
    tempAI->m_aSpells[2].m_pType = TARGETTYPE_SELF;

    return tempAI;
}


bool GossipHello_npc_jaina_proudmoore(Player* pPlayer, Creature* pCreature)
{
	if (instance_mount_hyjal * instance = (instance_mount_hyjal*)pCreature->GetInstanceData())
	{
		if (hyjalAI * jainaAI = dynamic_cast<hyjalAI*>(pCreature->AI()))
		{
			uint32 RageEncounter = instance->GetData(TYPE_WINTERCHILL);
			uint32 AnetheronEncounter = instance->GetData(TYPE_ANETHERON);
			if (RageEncounter == NOT_STARTED)
			{
				pPlayer->ADD_GOSSIP_ITEM_ID(GOSSIP_ICON_CHAT, GOSSIP_ITEM_JAIN_START, GOSSIP_SENDER_MAIN, GOSSIP_OPTION_SD2_1);
				pPlayer->SEND_GOSSIP_MENU(9168, pCreature->GetObjectGuid());
			}
			else if (RageEncounter == DONE && AnetheronEncounter == NOT_STARTED)
			{
				pPlayer->ADD_GOSSIP_ITEM_ID(GOSSIP_ICON_CHAT, JAINA_GOSSIP_ITEM_ANATHERON, GOSSIP_SENDER_MAIN, GOSSIP_OPTION_SD2_2);
				pPlayer->SEND_GOSSIP_MENU(9380, pCreature->GetObjectGuid());
			}
			else if (RageEncounter == DONE && AnetheronEncounter == DONE)
			{
				pPlayer->ADD_GOSSIP_ITEM_ID(GOSSIP_ICON_CHAT, JAINA_GOSSIP_ITEM_SUCCESS, GOSSIP_SENDER_MAIN, GOSSIP_OPTION_SD2_3);
				pPlayer->SEND_GOSSIP_MENU(9387, pCreature->GetObjectGuid());
			}
		}
	}
	return true;
}


bool GossipSelect_npc_jaina_proudmoore(Player* player, Creature* creature, uint32 /*uiSender*/, uint32 action)
{
    if (instance_mount_hyjal* instance = (instance_mount_hyjal*)creature->GetInstanceData())
    {
        if (hyjalAI* jainaAI = dynamic_cast<hyjalAI*>(creature->AI()))
        {
            switch (action)
            {
                case GOSSIP_OPTION_SD2_1:
                    instance->StartEvent(JAINA_FIRST_BOSS);
                    jainaAI->EventStarted();
                    player->PrepareGossipMenu(creature, 7556);
                    player->SendPreparedGossip(creature);
                    break;
                case GOSSIP_OPTION_SD2_2:
                    instance->StartEvent(JAINA_SECOND_BOSS);
                    jainaAI->EventStarted();
                    player->PrepareGossipMenu(creature, 7689);
                    player->SendPreparedGossip(creature);
                    break;
                case GOSSIP_OPTION_SD2_3:
                    if (instance->GetData(TYPE_AZGALOR) != DONE) // Jaina has the same gossip menu when spawned in orc base, but nothing should happen when selecting her gossip menu options
                    {
                        creature->SetActiveObjectState(true); // Set active object to prevent issues if players go out of range after talking to her (could lead to ores not spawning)
                        instance->StartEvent(JAINA_WIN);
                        creature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP); // ToDo: Jaina should not lose gossip flag, but also should not stop movement when a player opens her gossip window. Currently we have no way to achieve this.
                    }
                    player->CLOSE_GOSSIP_MENU();
                    break;
            }
        }
    }
    return false;
}

UnitAI* GetAI_npc_thrall(Creature* creature)
{
    hyjalAI* tempAI = new hyjalAI(creature);

    tempAI->m_aSpells[0].m_uiSpellId = SPELL_CHAIN_LIGHTNING;
    tempAI->m_aSpells[0].m_uiCooldown = urand(2000, 7000);
    tempAI->m_aSpells[0].m_pType = TARGETTYPE_VICTIM;

    tempAI->m_aSpells[1].m_uiSpellId = SPELL_FERAL_SPIRIT;
    tempAI->m_aSpells[1].m_uiCooldown = urand(6000, 41000);
    tempAI->m_aSpells[1].m_pType = TARGETTYPE_RANDOM;

    return tempAI;
}

bool GossipHello_npc_thrall(Player* pPlayer, Creature* pCreature)
{
	if (instance_mount_hyjal * instance = (instance_mount_hyjal*)pCreature->GetInstanceData())
	{
		if (hyjalAI * thrallAI = dynamic_cast<hyjalAI*>(pCreature->AI()))
		{
			uint32 AnetheronEncounter = instance->GetData(TYPE_ANETHERON);
			uint32 KazrogalEncounter = instance->GetData(TYPE_KAZROGAL);
			uint32 AzgalorEncounter = instance->GetData(TYPE_AZGALOR);
			if (AnetheronEncounter == DONE && KazrogalEncounter == NOT_STARTED)
			{
				pPlayer->ADD_GOSSIP_ITEM_ID(GOSSIP_ICON_CHAT, GOSSIP_ITEM_THRALL_START, GOSSIP_SENDER_MAIN, GOSSIP_OPTION_SD2_1);
				pPlayer->SEND_GOSSIP_MENU(9225, pCreature->GetObjectGuid());
			}
			else if (KazrogalEncounter == DONE && AzgalorEncounter == NOT_STARTED)
			{
				pPlayer->ADD_GOSSIP_ITEM_ID(GOSSIP_ICON_CHAT, THRALL_GOSSIP_ITEM_AZGALOR, GOSSIP_SENDER_MAIN, GOSSIP_OPTION_SD2_2);
				pPlayer->SEND_GOSSIP_MENU(9232, pCreature->GetObjectGuid());
			}
			else if (KazrogalEncounter == DONE && AzgalorEncounter == DONE)
			{
				pPlayer->ADD_GOSSIP_ITEM_ID(GOSSIP_ICON_CHAT, THRALL_GOSSIP_ITEM_SUCCESS, GOSSIP_SENDER_MAIN, GOSSIP_OPTION_SD2_3);
				pPlayer->SEND_GOSSIP_MENU(9233, pCreature->GetObjectGuid());
			}
		}
	}
	return true;
}

bool GossipSelect_npc_thrall(Player* player, Creature* creature, uint32 /*uiSender*/, uint32 action)
{
    if (instance_mount_hyjal* instance = (instance_mount_hyjal*)creature->GetInstanceData())
    {
        if (hyjalAI* thrallAI = dynamic_cast<hyjalAI*>(creature->AI()))
        {
            switch (action)
            {
            case GOSSIP_OPTION_SD2_1:
                instance->StartEvent(THRALL_FIRST_BOSS);
                thrallAI->EventStarted();
                player->PrepareGossipMenu(creature, 7584);
                player->SendPreparedGossip(creature);
                break;
            case GOSSIP_OPTION_SD2_2:
                instance->StartEvent(THRALL_SECOND_BOSS);
                thrallAI->EventStarted();
                player->PrepareGossipMenu(creature, 7701);
                player->SendPreparedGossip(creature);
                break;
            case GOSSIP_OPTION_SD2_3:
                creature->SetActiveObjectState(true); // Set active object to prevent issues if players go out of range after talking to him (could lead to ores not spawning)
                instance->StartEvent(THRALL_WIN);
                creature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP); // ToDo: Thrall should not lose gossip flag, but nothing should happen when choosing his options
                player->CLOSE_GOSSIP_MENU();
                break;
            }
        }
    }
    return true;
}

bool GossipHello_npc_tyrande_whisperwind(Player* pPlayer, Creature* pCreature)
{
	if (instance_mount_hyjal * instance = (instance_mount_hyjal*)pCreature->GetInstanceData())
	{
		uint32 AzgalorEvent = instance->GetData(TYPE_AZGALOR);
		// Only let them get item if Azgalor is dead.
		if (AzgalorEvent == DONE && !pPlayer->HasItemCount(ITEM_TEAR_OF_GODDESS, 1))
		{
			pPlayer->ADD_GOSSIP_ITEM_ID(GOSSIP_ICON_CHAT, TYRANDE_GOSSIP_ITEM_AID, GOSSIP_SENDER_MAIN, GOSSIP_OPTION_SD2_1);
			pPlayer->SEND_GOSSIP_MENU(9410, pCreature->GetObjectGuid());
		}
	}
	return true;
}

bool GossipSelect_npc_tyrande_whisperwind(Player* pPlayer, Creature* pCreature, uint32 /*uiSender*/, uint32 uiAction)
{
	if (!pPlayer || !pCreature)
		return false;
	if (uiAction == GOSSIP_OPTION_SD2_1)
	{
		ItemPosCountVec dest;
		uint8 msg = pPlayer->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, ITEM_TEAR_OF_GODDESS, 1);
		if (msg == EQUIP_ERR_OK)
			if (Item* item = pPlayer->StoreNewItem(dest, ITEM_TEAR_OF_GODDESS, true))
				pPlayer->SendNewItem(item, 1, true, false, true);

		pPlayer->SEND_GOSSIP_MENU(10675, pCreature->GetObjectGuid());
	}
	return true;
}

struct npc_building_triggerAI : public ScriptedAI
{
    npc_building_triggerAI(Creature* creature) : ScriptedAI(creature){}

    void Reset() override 
    {
        m_creature->AI()->SetReactState(REACT_PASSIVE);
    }

    void DamageTaken(Unit* /*dealer*/, uint32& damage, DamageEffectType /*damagetype*/)
    {
        // Never die
        damage = 0;
    }

    void MoveInLineOfSight(Unit* who) override
    {
        // Only let one ghoul attack
        if (m_creature->isInCombat())
            return;

        if (who->GetTypeId() != TYPEID_UNIT)
            return;

        if (who->GetEntry() != NPC_GHOUL && who->GetEntry() != NPC_GARGO)
            return;

        if (who->isInCombat())
            return;

        if (m_creature->IsWithinDistInMap(who, 35.0f))
        {
            who->SetInCombatWith(m_creature);
            m_creature->SetInCombatWith(who);
            if (who->GetEntry() == NPC_GHOUL)
            {
                who->GetMotionMaster()->Clear();
                who->GetMotionMaster()->MoveIdle();
                ((Creature*)who)->SetWalk(false);
                who->GetMotionMaster()->MovePoint(1, m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ());
                who->Attack(m_creature, true);
            }
            else
            {
                m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            }
        }
    }
};

UnitAI* GetAI_npc_building_trigger(Creature* creature)
{
    return new npc_building_triggerAI(creature);
}

UnitAI* GetAI_npc_tyrande_whisperwind(Creature* creature)
{
    hyjalAI* tempAI = new hyjalAI(creature);

    tempAI->m_aSpells[0].m_uiSpellId = SPELL_STARFALL;
    tempAI->m_aSpells[0].m_uiCooldown = urand(60000, 70000);
    tempAI->m_aSpells[0].m_pType = TARGETTYPE_VICTIM;

    tempAI->m_aSpells[1].m_uiSpellId = SPELL_TRUESHOT_AURA;
    tempAI->m_aSpells[1].m_uiCooldown = 4000;
    tempAI->m_aSpells[1].m_pType = TARGETTYPE_SELF;

    return tempAI;
}

void AddSC_hyjal()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "npc_jaina_proudmoore";
    pNewScript->GetAI = &GetAI_npc_jaina_proudmoore;
	pNewScript->pGossipHello = &GossipHello_npc_jaina_proudmoore;
    pNewScript->pGossipSelect = &GossipSelect_npc_jaina_proudmoore;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_thrall";
    pNewScript->GetAI = &GetAI_npc_thrall;
	pNewScript->pGossipHello = &GossipHello_npc_thrall;
    pNewScript->pGossipSelect = &GossipSelect_npc_thrall;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_tyrande_whisperwind";
	pNewScript->pGossipHello = &GossipHello_npc_tyrande_whisperwind;
	pNewScript->pGossipSelect = &GossipSelect_npc_tyrande_whisperwind;
    pNewScript->GetAI = &GetAI_npc_tyrande_whisperwind;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_building_trigger";
    pNewScript->GetAI = &GetAI_npc_building_trigger;
    pNewScript->RegisterSelf();
}
