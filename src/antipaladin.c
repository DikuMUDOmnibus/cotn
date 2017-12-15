#if defined(macintosh)
#include<types.h>
#else
#include <sys/types.h>
#endif
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"

// Anti-Paladin Class by Xrakisis


void do_raise( CHAR_DATA *ch, char *argument )
{
  char buf [MAX_STRING_LENGTH];
  OBJ_DATA *corpse;
  CHAR_DATA *victim;
  char arg[MSL];
  AFFECT_DATA af;

  one_argument (argument, arg);


  if (IS_NPC(ch)) return;

  if (!IS_CLASS(ch, CLASS_ANTIPALADIN))
  {  
    send_to_char("#0This is an Antipaladin Ability.#n\n\r",ch);
    return;
  }
  if (!IS_SET(ch->pcdata->powers[15], APOWER_RAISE))
  {
    send_to_char("#0You need to Delve Raise Dead first#n\n\r",ch);
    return;
  }
      if (ch->pcdata->powers[RAISETIMER] > 0)
      {
        send_to_char("#0You may not raise the dead quite so soon.#n\n\r",ch);
        return;
      }

  if ((corpse = get_obj_room (ch, arg)) == NULL)
    {
      stc ("That corpse isnt here.\n\r", ch);
      return;
    }
  if (corpse->item_type != ITEM_CORPSE_NPC)
    {
      stc ("Only on mobile corpses.\n\r", ch);
      return;
    }

    victim=create_mobile( get_mob_index( MOB_VNUM_NIGHTMARE ));
    free_string(victim->short_descr);
    free_string(victim->name);
    free_string(victim->long_descr);
    victim->name = str_dup( corpse->name);
    victim->short_descr = str_dup(corpse->short_descr);
    victim->long_descr = str_dup(corpse->description);
    victim->level = ch->spl[RED_MAGIC];
    victim->hit = 200 * ch->spl[RED_MAGIC]; 
    victim->max_hit = 200 * ch->spl[RED_MAGIC];
    victim->hitroll =  ch->spl[RED_MAGIC]; 
    victim->damroll =  ch->spl[RED_MAGIC];  
    char_to_room( victim, ch->in_room );
    add_follower( victim, ch );
    af.type      = skill_lookup ("charm person");
    af.duration  = 666;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_CHARM;
    affect_to_char( victim, &af );
    SET_BIT(victim->act,ACT_NOEXP);
    SET_BIT(victim->act,PLR_HOLYLIGHT);
    do_group(ch,victim->name);
    if (corpse != NULL)
    extract_obj (corpse);
    ch->pcdata->powers[RAISETIMER] += 20;
    return;

}

void do_morph( CHAR_DATA *ch, char *argument )
{
  char buf [MAX_STRING_LENGTH];

  if (IS_NPC(ch)) return;

  if (!IS_CLASS(ch, CLASS_ANTIPALADIN))
  {  
    send_to_char("#0This is an Antipaladin Ability.#n\n\r",ch);
    return;
  }
  if (!IS_SET(ch->pcdata->powers[15], APOWER_MORPH))
  {
    send_to_char("#0You need to Delve morph first#n\n\r",ch);
    return;
  }

  if (IS_SET(ch->newbits, NEW_CUBEFORM))
  {
    REMOVE_BIT(ch->newbits, NEW_CUBEFORM);
    REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
    act( "$n turns into a human.",ch,NULL,NULL,TO_ROOM);
    send_to_char("You turn into your normal form.\n\r",ch);
    ch->damroll -=400;
    ch->hitroll -=400;
    free_string(ch->morph);
    ch->morph=str_dup("");
    return;
  }
  if (ch->move < 2000 || ch->mana < 2000)
  {
    send_to_char("#0You need 2000 mana and 2000 move to make yourself a true Anti-Paladin.#n\n\r",ch);
    return;
  }
  else
  {
    act( "$n transforms into an Anti-Paladin.",ch,NULL,NULL,TO_ROOM);
    send_to_char("#0Takhisis bestows power upon you.#n\n\r",ch);
    SET_BIT(ch->newbits, NEW_CUBEFORM);
    SET_BIT(ch->affected_by, AFF_POLYMORPH);
    sprintf(buf,"%s the #GAn#gti#0-#7Pal#gad#Gin#n",ch->name);
    free_string(ch->morph);
    ch->morph = str_dup(buf);
    ch->move -= 2000;
    ch->mana -= 2000;
    ch->damroll +=400;
    ch->hitroll +=400;
    return;
  }
  return;
}


void do_unholyritual (CHAR_DATA *ch, char *argument)
{
  char arg[MAX_INPUT_LENGTH];
  char buf[MAX_STRING_LENGTH];

  argument = one_argument( argument, arg );
  if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_ANTIPALADIN))
  {
    send_to_char("Huh.\n\r",ch);
    return;
  }
  if (ch->alignment != -1000) ch->alignment = -1000;
  if (ch->pcdata->powers[DARKARTS] > 20)
  {
    send_to_char("#0Youve mastered the Dark Arts#n.\n\r",ch);
    return;
  }
  if (ch->practice < ch->pcdata->powers[DARKARTS]*50 + 50)
  {
    send_to_char("#0You need more primal#n\n\r",ch);
    return;
  }
  else  
  {
    ch->practice -= ch->pcdata->powers[DARKARTS]*50 + 50;
    send_to_char("#0You Pray to Takhisis and she bestows power upon you.#n\n\r",ch);
    ch->pcdata->powers[DARKARTS] += 1;
    return;
  }
  return;
}

void do_summonmount( CHAR_DATA *ch, char *argument )
{
  char arg1[MAX_STRING_LENGTH];
  CHAR_DATA *victim;
  AFFECT_DATA af;

  argument=one_argument(argument,arg1);

  if (IS_NPC(ch)) return;

  if (!IS_CLASS(ch, CLASS_ANTIPALADIN))
  {
    send_to_char("Huh?\n\r",ch);
    return;
  }
    if (IS_SET(ch->pcdata->powers[MOUNT_SUMMON], HAS_SUMMONED_NIGHTMARE))
    {
      send_to_char("#0You cannot summon any more Nightmares.#n\n\r",ch);
      return;
    }
    act( "A look of concentration passes over $n's face.", ch, NULL,NULL, TO_ROOM );
    stc( "You create a Nightmare.\n\r",ch);
    victim=create_mobile( get_mob_index( MOB_VNUM_NIGHTMARE ));
    free_string(victim->short_descr);
    free_string(victim->name);
    free_string(victim->long_descr);
    victim->name=str_dup("nightmare");
    victim->short_descr=str_dup("#RA Demonic Nightmare#n");
    victim->long_descr=str_dup("#Ra Demonic Nightmare here.#n\n\r");
    victim->level = ch->spl[RED_MAGIC] / 2;
    victim->hit = ch->hit;	// 100 * ch->spl[RED_MAGIC]; 
    victim->max_hit = ch->max_hit; // 100 * ch->spl[RED_MAGIC];
    victim->hitroll =  ch->spl[RED_MAGIC]; 
    victim->damroll =  ch->spl[RED_MAGIC];  
    char_to_room( victim, ch->in_room );
    add_follower( victim, ch );
    af.type      = skill_lookup ("charm person");
    af.duration  = 666;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_CHARM;
    affect_to_char( victim, &af );
    SET_BIT(victim->act,ACT_NOEXP);
    SET_BIT(victim->act,PLR_HOLYLIGHT);
    do_group(ch,victim->name);
    SET_BIT(ch->pcdata->powers[MOUNT_SUMMON], HAS_SUMMONED_NIGHTMARE);
    SET_BIT(victim->act, ACT_MOUNT);
    return;
  }

void do_layonhands( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  char buf[MAX_STRING_LENGTH];
  int dam;

  if (IS_NPC(ch)) return;
 
  if (!IS_CLASS(ch,CLASS_ANTIPALADIN) || !IS_SET(ch->pcdata->powers[15], APOWER_LAYONHANDS))
  {
    send_to_char("#0This is an Anti-Paladin Ability#n\n\r",ch);
    return;
  }
  if ( ( victim = ch->fighting ) == NULL )
  {
    send_to_char( "You aren't fighting anyone.\n\r", ch );
    return;
  }
  WAIT_STATE( ch, 12 );

  if (!IS_NPC(victim)) dam = number_range(2500,4000);
  else dam = number_range(4500,7000);
  dam += (ch->pPotentia * 2);

  sprintf(buf,"#0You lay your hands on $N #R[#0%d#R]#n\n\r",dam);
  act(buf,ch,NULL,victim,TO_CHAR);
  sprintf(buf,"$n's #0lays their hands on you!#n [%d]\n\r",dam);
  act(buf,ch,NULL,victim,TO_VICT);
  sprintf(buf,"$n's #0lays their hands on $N [%d]\n\r",dam);
  act(buf,ch,NULL,victim,TO_NOTVICT); 
  hurt_person(ch,victim, dam);
  ch->hit += dam;
  if (ch->hit > ch->max_hit) ch->hit = ch->max_hit;
  if (ch->pPotentia > 50) ch->pPotentia -= 50;
  return;
}


void do_delve( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char      arg1 [MAX_INPUT_LENGTH];
    char      arg2 [MAX_INPUT_LENGTH];
    int       inpart = 0;
    int       cost = 0;

    smash_tilde(argument);
    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_ANTIPALADIN))
    {
	    send_to_char("Huh?\n\r",ch);
	    return;
    }

    if ( arg1[0] == '\0' || arg2[0] == '\0' )
	{
	
		send_to_char( "Syntax: Delve <person> <power>\n\r", ch );
		send_to_char("#0Layonhands #0(#r5000#0)#n,  #0Charisma #0(#r7500#0)#n,  #0Might #0(#r2500#0)#n\n\r",ch);
		send_to_char("#0DetectLiving #0(#r5000#0)#n,  #0Morph #0(#r5000#0)#n,  #0BattleRage #0(#r1000#0)#n\n\r", ch );
		send_to_char("#0Entangle #0(#r10000#0)#n,  #0Flamestrike #0(#r5000#0)#n,  #0DirtKick #0(#r25000#0)#n\n\r", ch );
		send_to_char("#0Malice #0(#r20000#0)#n,  #0Sandman #0(#r25000#0)#n,  #0Grapple#0(#r5000#0)#n\n\r",ch );
		send_to_char("#0Toughness (#r10000#0)#n, #0Camouflage#0 (#r10000#0)#n #0Takhisisaura#0 (#r50000#0)#n\n\r",ch );
		send_to_char("#0Speed (#r25000#0)#n, #0RaiseDead (#r10000#0)#n\n\r",ch);
		return;
	}
// Anti-paladin class By Xrakisis
// Battlerage = WW Rage Like Ability - xrak
// DirtKick = Blind Ability and some damage
// Entangle = Web and some damage
// Grapple = some damage and destance/stun

    if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
    {
	send_to_char( "Nobody by that name.\n\r", ch );
	return;
    }

    if ( IS_NPC(victim) )
    {
	send_to_char( "Not on NPC's.\n\r", ch );
	return;
    }

        if (!IS_CLASS(victim, CLASS_ANTIPALADIN)) {
                send_to_char("Only on Anti-Paladins.\n\r", ch );
                return;}

    if (!str_cmp(arg2,"layonhands")) 
	{inpart = APOWER_LAYONHANDS; cost = 2500;}
    else if (!str_cmp(arg2,"charisma"))
	{inpart = APOWER_CHARISMA; cost= 7500;}
    else if (!str_cmp(arg2,"might"))
	{inpart = APOWER_MIGHT; cost= 5000;}
    else if (!str_cmp(arg2,"detectliving"))
	{inpart = APOWER_DETECTLIVING; cost = 25000;}
    else if (!str_cmp(arg2,"morph"))
	{inpart = APOWER_MORPH; cost = 5000;}
    else if (!str_cmp(arg2,"battlerage"))
	{inpart = APOWER_BATTLERAGE; cost = 25000;}
    else if (!str_cmp(arg2,"entangle"))
	{inpart = APOWER_ENTANGLE; cost = 20000;}
    else if (!str_cmp(arg2,"flamestrike"))
	{inpart = APOWER_FLAMESTRIKE; cost = 5000;}
    else if (!str_cmp(arg2,"dirtkick"))
	{inpart = APOWER_DIRTKICK; cost = 1000;}
    else if (!str_cmp(arg2,"malice"))
	{inpart = APOWER_MALICE; cost = 10000;}
    else if (!str_cmp(arg2,"sandman")) 
	{inpart = APOWER_SLEEP; cost = 5000;}
    else if (!str_cmp(arg2,"grapple"))
	{inpart = APOWER_GRAPPLE; cost = 2500;}
    else if (!str_cmp(arg2,"toughness"))
	{inpart = APOWER_TOUGHNESS; cost = 10000;}
    else if (!str_cmp(arg2,"camouflage"))
        {inpart = APOWER_CAMOUFLAGE; cost = 10000;}
    else if (!str_cmp(arg2,"takhisisaura"))
	{inpart = APOWER_TAKAURA; cost = 50000;}
    else if (!str_cmp(arg2,"speed"))
	{inpart = APOWER_SPEED; cost = 25000;}
    else if (!str_cmp(arg2, "raisedead"))
	{inpart = APOWER_RAISE; cost = 10000;}
    else
	{
	
		send_to_char( "Syntax: Delve <person> <power>\n\r", ch );
		send_to_char("#0Layonhands #0(#r5000#0)#n,  #0Charisma #0(#r7500#0)#n,  #0Might #0(#r2500#0)#n.\n\r",ch);
		send_to_char("#0DetectLiving #0(#r5000#0)#n,  #0Morph #0(#r5000#0)#n,  #0BattleRage #0(#r25000#0)#n.\n\r", ch );
		send_to_char("#0Entangle #0(#r10000#0)#n,  #0Flamestrike #0(#r5000#0)#n,  #0DirtKick #0(#r25000#0)#n.\n\r", ch );
		send_to_char("#0Malice #0(#r20000#0)#n,  #0Sandman #0(#r25000#0)#n,  #0Grapple#0(#r5000#0)#n.\n\r",ch );
                send_to_char("#0Toughness (#r10000#0)#n, #0Camouflage#0 (#r10000#0)#n #0Takhisisaura#0 (#r50000#0)#n\n\r",ch );
                send_to_char("#0Speed (#r25000#0)#n, #0RaiseDead (#r10000#0)#n\n\r",ch);
                return;
	}
    if (!(ch==victim)) cost *=5;

    if (IS_SET(victim->pcdata->powers[15], inpart))
    {
	send_to_char("They have already got that power.\n\r",ch);
	return;
    }

    if( !IS_CLASS(victim, CLASS_ANTIPALADIN) )
    {
	stc( "You can not grant powers to Non Drows.\n\r",ch);
	return;
    }

    if (ch->pcdata->stats[DEMON_CURRENT] < cost) 
    {
	send_to_char("You have insufficient power to grant that gift.\n\r",ch);
	return;
    }
    SET_BIT(victim->pcdata->powers[15], inpart);
    ch->pcdata->stats[DEMON_CURRENT] -= cost;
    if (victim != ch) send_to_char("Power has been given to you from an AntiPaladin of Takhisis\n\r",victim);
    send_to_char("Ok.\n\r",ch);
    if (victim != ch) save_char_obj(ch);
    save_char_obj(victim);
    return; 
}


void do_detectliving( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) )
	return;

  if (!IS_CLASS(ch,CLASS_ANTIPALADIN) || !IS_SET(ch->pcdata->powers[15], APOWER_DETECTLIVING))
  {
    send_to_char("#0This is an Anti-Paladin Ability#n\n\r",ch);
    return;
  }

    if ( IS_SET(ch->act, PLR_HOLYLIGHT) )
    {
	REMOVE_BIT(ch->act, PLR_HOLYLIGHT);
	send_to_char( "#0You no longer detect living.#n\n\r", ch );
    }
    else
    {
	SET_BIT(ch->act, PLR_HOLYLIGHT);
	send_to_char( "#0You now detect the living#n.\n\r", ch );
    }

    return;
}


void do_flamestrike( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  char buf[MAX_STRING_LENGTH];
  int dam;

  if (IS_NPC(ch)) return;
 
  if (!IS_SET(ch->pcdata->powers[15], APOWER_FLAMESTRIKE) || !IS_CLASS(ch,CLASS_ANTIPALADIN))
  {
    send_to_char("#0This is an Anti-Paladin Ability#n\n\r",ch);
    return;
  }
  if ( ( victim = ch->fighting ) == NULL )
  {
    send_to_char( "You aren't fighting anyone.\n\r", ch );
    return;
  }
  WAIT_STATE( ch, 12 );

  if (!IS_NPC(victim)) dam = number_range(5000,7000);
  else dam = number_range(12000,14000);
  dam += (ch->pPotentia * 2);

  sprintf(buf,"#0You #RFLAMESTRIKE $N #R[#0%d#R]#n\n\r",dam);
  act(buf,ch,NULL,victim,TO_CHAR);
  sprintf(buf,"$n's #RFLAMESTRIKES #0you!#n [%d]\n\r",dam);
  act(buf,ch,NULL,victim,TO_VICT);
  sprintf(buf,"$n's #RFLAMESTRIKES#0 $N #n[%d]\n\r",dam);
  act(buf,ch,NULL,victim,TO_NOTVICT); 
  hurt_person(ch,victim, dam);

  if (ch->pPotentia > 50) ch->pPotentia -= 50;
  return;
}


void do_grapple( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  char buf[MAX_STRING_LENGTH];
  int dam;
  int chance;

  if (IS_NPC(ch)) return;
 
  if (!IS_CLASS(ch,CLASS_ANTIPALADIN) || !IS_SET(ch->pcdata->powers[15], APOWER_GRAPPLE))
  {
    send_to_char("#0This is an Anti-Paladin Ability#n\n\r",ch);
    return;
  }
  if ( ( victim = ch->fighting ) == NULL )
  {
    send_to_char( "You aren't fighting anyone.\n\r", ch );
    return;
  }
  WAIT_STATE( ch, 12 );

  chance = number_range (1,3);

  if (chance == 1 )
{
    WAIT_STATE(ch, 4);
    do_say(ch,"#RHAHA DESTANCE!!!!#n");
    do_stance(victim, "");
}
  else if (chance == 2 )
{
    WAIT_STATE(victim, 12);
    do_say(ch,"#RHAHA STUN!!!!#n");
}

  if (!IS_NPC(victim)) dam = number_range(2000,4000);
  else dam = number_range(10000,14000);

  sprintf(buf,"#0You #0Grapple With $N #R[#0%d#R]#n\n\r",dam);
  act(buf,ch,NULL,victim,TO_CHAR);
  sprintf(buf,"$n's #0Grapples with You#n [%d]\n\r",dam);
  act(buf,ch,NULL,victim,TO_VICT);
  sprintf(buf,"$n's #0Grapples with #n$N #n[%d]\n\r",dam);
  act(buf,ch,NULL,victim,TO_NOTVICT); 
  hurt_person(ch,victim, dam);
  return;
}



void do_dirtkick( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  char buf[MAX_STRING_LENGTH];
  int dam;
  AFFECT_DATA af;

  if (IS_NPC(ch)) return;
 
  if (!IS_CLASS(ch,CLASS_ANTIPALADIN) || !IS_SET(ch->pcdata->powers[15], APOWER_DIRTKICK))
  {
    send_to_char("#0This is an Anti-Paladin Ability#n\n\r",ch);
    return;
  }
  if ( ( victim = ch->fighting ) == NULL )
  {
    send_to_char( "You aren't fighting anyone.\n\r", ch );
    return;
  }

      if (is_safe(ch, victim)) return;
      act("#0$n Kicks Dirt in $N's Eyes#n",ch,NULL,victim,TO_NOTVICT);
      act("#0You kick dirt in $N's Eyes#n'.",ch,NULL,victim,TO_CHAR);
      act("#0$n Kicks Dirt in your Eyes!#n'.",ch,NULL,victim,TO_VICT);
      if (IS_SET(victim->act,PLR_HOLYLIGHT)) REMOVE_BIT(victim->act,PLR_HOLYLIGHT);
      if (IS_SET(victim->affected_by, AFF_DETECT_HIDDEN))
        REMOVE_BIT(victim->affected_by, AFF_DETECT_HIDDEN);
      if (IS_SET(victim->affected_by, AFF_DETECT_INVIS))
        REMOVE_BIT(victim->affected_by, AFF_DETECT_INVIS);
      af.type      = skill_lookup("blindness");
      af.location  = APPLY_HITROLL;
      af.modifier  = -4;
      af.duration  = 60;
      af.bitvector = AFF_BLIND;
      affect_to_char( victim, &af );
      WAIT_STATE(ch,12);
      return;
  }


void do_apsleep( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char      arg1 [MAX_INPUT_LENGTH];
    argument = one_argument( argument, arg1 );
        
    if (IS_NPC(ch)) return;
	
  if (!IS_CLASS(ch,CLASS_ANTIPALADIN) || !IS_SET(ch->pcdata->powers[15], APOWER_SLEEP))
  {
    send_to_char("#0This is an Anti-Paladin Ability#n\n\r",ch);
    return;
  }

  if (ch->in_room != NULL)
  {
    if ( IS_SET(ch->in_room->room_flags,ROOM_SAFE) )
    {
      send_to_char( "You cannot use this power in a safe room.\n\r", ch );
      return;
    }
  }

	
    if ( arg1[0] == '\0' )
    {
	send_to_char( "Put who to sleep?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, NULL, arg1 ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( ch == victim )
    {
	send_to_char( "Not on yourself!\n\r", ch );
	return;
    }
	
    if ( ch->position == POS_FIGHTING )
    {
	send_to_char( "Not while fighting!\n\r", ch );
	return;
    }
    
	if (ch->move < 500)
	{
	send_to_char("You don't have enough movement points.\n\r", ch);
	return;
	}     
	  
    if ( victim->in_room == ch->in_room )
	{
	act("You invoke the power of Takhisis and put $N to sleep.",ch,NULL,victim,TO_CHAR);
	act("$n invokes the power of Takhisis and you drift into dreamland.",ch,NULL,victim,TO_VICT);
	victim->position = POS_SLEEPING;
	ch->move -= 500;
	WAIT_STATE(ch, 1);
	return;
	}
	
	return;
}


void do_battlerage( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char      arg1 [MAX_INPUT_LENGTH];
    argument = one_argument( argument, arg1 );
        
   CHAR_DATA *vch;
    CHAR_DATA *vch_next;
    if (IS_NPC(ch)) return;
	
  if (!IS_CLASS(ch,CLASS_ANTIPALADIN) || !IS_SET(ch->pcdata->powers[15], APOWER_BATTLERAGE))
  {
    send_to_char("#0This is an Anti-Paladin Ability#n\n\r",ch);
    return;
  }

      if (ch->pcdata->powers[APTICK] > 0)
      {
        send_to_char("#0You may not battlerage yet.#n\n\r",ch);
        return;
      }



    for ( vch = char_list; vch != NULL; vch = vch_next )
    {
	vch_next	= vch->next;
	if ( vch->in_room == NULL )
	    continue;
	if ( ch == vch )
	{
	    act("You brandish your blades and scream a war cry", ch, NULL, NULL, TO_CHAR);
	    continue;
	}
	if ( !IS_NPC(vch) && vch->pcdata->chobj != NULL )
	    continue;
	if (!IS_NPC(vch))
	{
	    if (vch->in_room == ch->in_room)
		act("$n Charges and $s screams a War Cry!", ch, NULL, vch, TO_VICT);
	    else if (vch->in_room->area == ch->in_room->area)
		send_to_char("You hear a WarCry close by!\n\r", vch);
	    else
	    if (!CAN_PK(vch)) continue;
	}
	if ( vch->in_room == ch->in_room && can_see( ch, vch) )
	{
	    multi_hit( ch, vch, TYPE_UNDEFINED );
	    if (vch == NULL || vch->position <= POS_STUNNED) continue;
	    multi_hit( ch, vch, TYPE_UNDEFINED );
	}
    }
      ch->pcdata->powers[APTICK] = 3;
      return;
}
