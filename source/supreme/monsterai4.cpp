void AI_Lazybones(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y;

	// mind1=exact facing for shooting
	// mind=timer to wake up
	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_SKELOUCH,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_SKELDIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK)
		{
			x=me->x+Cosine(me->mind1)*16;
			y=me->y+Sine(me->mind1)*16;
			if(me->frm==2)
				MakeSound(SND_SKELSHOOT,me->x,me->y,SND_CUTOFF,950);
			FireBullet(x,y,me->mind1,BLT_ENERGY,me->friendly);
			me->mind1+=8;
			me->facing=(me->mind1/32);
			if(me->frm==17)
			{
				me->reload=10;
				me->seq=ANIM_A2;
				me->frm=0;
				me->frmAdvance=128;
			}
		}
		if(me->seq==ANIM_A1 && me->frm==6)
		{
			me->seq=ANIM_ATTACK;
			me->frm=0;
			me->frmAdvance=128;
		}
		if(me->seq==ANIM_A2 && me->frm==6)
		{
			me->seq=ANIM_IDLE;
			me->frm=0;
			me->frmAdvance=128;
			me->action=ACTION_IDLE;
		}
		return;	// can't do nothin' right now
	}

	if((me->ouch || RangeToTarget(me,goodguy)<256*FIXAMT) && me->reload==0)
	{
		MakeSound(SND_SKELALIVE,me->x,me->y,SND_CUTOFF,1200);
		me->seq=ANIM_A1;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=128;
		me->action=ACTION_BUSY;
		me->dx=0;
		me->dy=0;
		me->reload=0;
		me->mind1=Random(256);
		return;
	}
}

void AI_StareyBat(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp==0)
			MakeSound(SND_BATOUCH,me->x,me->y,SND_CUTOFF,1200);
	}

	FaceGoodguy2(me,goodguy);

	if(me->mind>0)
	{
		me->mind++;
		if(me->mind==15)
			me->mind=0;
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK)
		{
			if(me->mind==0)
			{
				MakeSound(SND_BATSTARE,me->x,me->y,SND_CUTOFF,500);
				me->mind=1;
			}
		}

		if(me->seq==ANIM_ATTACK && me->frm==4)
		{
			if(RangeToTarget(me,goodguy)<300*FIXAMT)
			{
				me->frmTimer=0;
				if(goodguy->x<me->x)
				{
					goodguy->dx-=(340*FIXAMT/128);
					goodguy->dy-=(340*FIXAMT/1024);
				}
				else
				{
					goodguy->dx+=(340*FIXAMT/128);
					goodguy->dy+=(340*FIXAMT/1024);
				}
				if(goodguy->y<me->y)
				{
					goodguy->dy-=(340*FIXAMT/128);
					goodguy->dx+=(340*FIXAMT/1024);
				}
				else
				{
					goodguy->dy+=(340*FIXAMT/128);
					goodguy->dx-=(340*FIXAMT/1024);
				}
				Clamp(&goodguy->dx,FIXAMT*7);
				Clamp(&goodguy->dy,FIXAMT*7);
			}
		}
		return;	// can't do nothin' right now
	}

	if(RangeToTarget(me,goodguy)<270*FIXAMT)
	{
		me->seq=ANIM_ATTACK;
		me->action=ACTION_BUSY;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=128;
	}
}

void AI_Madman(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y;

	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_WACKOOUCH,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_WACKODIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm==5 && me->reload==0)
		{
			x=me->x+Cosine(me->facing*32)*16;
			y=me->y+Sine(me->facing*32)*16;
			if(me->AttackCheck(32,x>>FIXSHIFT,y>>FIXSHIFT,goodguy))
				goodguy->GetShot(Cosine(me->facing*32)*4,Sine(me->facing*32)*4,8,map,world);
			me->reload=10;
		}

		if(me->seq==ANIM_DIE && me->frm==5)
			me->frmAdvance=128;
		return;	// can't do nothin' right now
	}

	FaceGoodguy(me,goodguy);

	if(RangeToTarget(me,goodguy)<(96*FIXAMT) && Random(16)==0 && me->reload==0)
	{
		// stab him
		MakeSound(SND_CRAZYSTAB,me->x,me->y,SND_CUTOFF,1200);
		me->seq=ANIM_ATTACK;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=255;
		me->action=ACTION_BUSY;
		me->dx=0;
		me->dy=0;
		me->reload=1;
		return;
	}

	me->dx=Cosine(me->facing*32)*5;
	me->dy=Sine(me->facing*32)*5;
	if(me->seq!=ANIM_MOVE)
	{
		me->seq=ANIM_MOVE;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=128;
	}
}

void AI_PygmyAcct(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y,ang;

	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_PYGMYOUCH,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_PYGMYDIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm>=2 && me->frm<=5 && me->reload==0)
		{
			ang=(me->facing*32-40+(20*(me->frm-2)))&255;
			x=me->x+Cosine(ang)*16;
			y=me->y+Sine(ang)*16;
			FireBullet(x,y,ang,BLT_PAPER,me->friendly);
			me->reload=2;
		}
		return;	// can't do nothin' right now
	}

	if(me->mind==0)	// not currently aware of goodguy
	{
		me->frmAdvance=32;	// overcome the auto-128 frmAdvance of guy.cpp

		if(me->seq==ANIM_MOVE)
		{
			me->seq=ANIM_IDLE;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=32;
		}
		if(me->mind1<101)
			me->mind1++;

		if(me->mind1>100 && Random(200)==0)
		{
			if(Random(4)==0)
			{
				me->seq=ANIM_A1;	// spin the spear
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=64;
				me->mind1=0;
				me->action=ACTION_BUSY;
			}
			else	// just change facing
			{
				me->facing=(me->facing+(-1+Random(3)))&7;	//change facing
				me->mind1=0;
			}
		}
		if(goodguy)
		{
			if(RangeToTarget(me,goodguy)<350*FIXAMT || me->ouch>0)
			{
				// if the goodguy is near, or he shot me
				me->mind=1;	// start hunting
				me->mind1=120;	// for 4 seconds minimum

				FaceGoodguy(me,goodguy);

				// do angry animation
				me->action=ACTION_BUSY;
				me->seq=ANIM_A2;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=128;
				MakeSound(SND_PYGMYANGRY,me->x,me->y,SND_CUTOFF,1200);
			}
		}
		me->dx=0;
		me->dy=0;
	}
	else
	{
		FaceGoodguy(me,goodguy);

		if(Random(50)==0 && me->reload==0)
		{
			// fire at him
			MakeSound(SND_PYGMYCASE,me->x,me->y,SND_CUTOFF,1200);
			me->seq=ANIM_ATTACK;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
			me->action=ACTION_BUSY;
			me->dx=0;
			me->dy=0;
			me->reload=1;
			return;
		}

		if(me->ouch && me->mind1<240)
			me->mind1+=10;
		if(me->mind1)
			me->mind1--;
		else
		{
			if(RangeToTarget(me,goodguy)>380*FIXAMT)
				me->mind=0;	// get bored again
			else
				me->mind1=20;	// stay on trail a little longer
			return;
		}
		me->dx=Cosine(me->facing*32)*3;
		me->dy=Sine(me->facing*32)*3;
		if(me->seq!=ANIM_MOVE)
		{
			me->seq=ANIM_MOVE;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
		}
	}
}

void AI_Crazypants(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y;

	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_WACKOOUCH,me->x,me->y,SND_CUTOFF,1200);
		else if(me->mind==3)
			MakeSound(SND_WACKODIE,me->x,me->y,SND_CUTOFF,1200);
		else
		{
			if(!me->friendly)
				player.enemiesSlain--;
			me->hp=monsType[me->type].hp;
			me->mind++;
			me->seq=ANIM_A2;
			me->frm=0;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=255;
			me->action=ACTION_BUSY;
			MakeSound(SND_CRAZYMAD,me->x,me->y,SND_CUTOFF,1200);
			FaceGoodguy(me,goodguy);
		}
	}

	if(me->mind==0)	// have yet to spot Bouapha
	{
		if(me->ouch || RangeToTarget(me,goodguy)<300*FIXAMT)
		{
			me->seq=ANIM_A2;
			me->frm=0;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=255;
			me->mind=1;
			me->action=ACTION_BUSY;
			MakeSound(SND_CRAZYMAD,me->x,me->y,SND_CUTOFF,1200);
			FaceGoodguy(me,goodguy);
		}
		return;
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm>=2 && me->frm<=12)
		{
			me->dx=Cosine(me->facing*32)*5*me->mind;
			me->dy=Sine(me->facing*32)*5*me->mind;

			if(me->reload==0)
			{
				x=me->x+Cosine(me->facing*32)*16;
				y=me->y+Sine(me->facing*32)*16;
				if(me->AttackCheck(40,x>>FIXSHIFT,y>>FIXSHIFT,goodguy))
				{
					goodguy->GetShot(Cosine(me->facing*32)*6,Sine(me->facing*32)*6,8,map,world);
					me->reload=6;
				}
			}

		}
		else if(me->seq==ANIM_A1 && me->frm>2)
		{
			if(me->frm&1)
			{
				me->dx+=Cosine(me->facing*32)*2;
				me->dy+=Sine(me->facing*32)*2;
				Clamp(&me->dx,3*me->mind);
				Clamp(&me->dy,3*me->mind);
				Dampen(&me->dx,FIXAMT/2);
				Dampen(&me->dy,FIXAMT/2);
			}
			me->facing=(me->facing+1)&7;
			// spin!
			if(me->reload==0)
			{
				x=me->x+Cosine(me->facing*32)*16;
				y=me->y+Sine(me->facing*32)*16;
				if(me->AttackCheck(40,x>>FIXSHIFT,y>>FIXSHIFT,goodguy))
				{
					goodguy->GetShot(Cosine(me->facing*32)*6,Sine(me->facing*32)*6,12,map,world);
					me->reload=6;
				}
			}
		}
		else if(me->seq==ANIM_DIE)
		{
			// spin to death
			me->facing=(me->facing+1)&7;
		}
		return;	// can't do nothin' right now
	}

	if(me->mind2==0)		// when mind2=0, singlemindedly drift towards Bouapha
	{
		if(RangeToTarget(me,goodguy)<(256*FIXAMT) && Random(12)==0)
		{
			// get him!
			MakeSound(SND_CRAZYDASH,me->x,me->y,SND_CUTOFF,1200);
			me->seq=ANIM_ATTACK;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
			me->action=ACTION_BUSY;
			me->dx=0;
			me->dy=0;
			me->reload=0;
			return;
		}
		else if(RangeToTarget(me,goodguy)<(128*FIXAMT) && Random(7)==0)
		{
			// get him!
			MakeSound(SND_CRAZYSPIN,me->x,me->y,SND_CUTOFF,1200);
			me->seq=ANIM_A1;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=255;
			me->action=ACTION_BUSY;
			me->dx=0;
			me->dy=0;
			me->reload=0;
			return;
		}
		FaceGoodguy(me,goodguy);

		me->dx=Cosine(me->facing*32)*me->mind*3;
		me->dy=Sine(me->facing*32)*me->mind*3;
		if(me->seq!=ANIM_MOVE)
		{
			me->seq=ANIM_MOVE;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128+me->mind*32;
		}
		if(me->mind1>0 && RangeToTarget(me,goodguy)>(64*FIXAMT))
		{
			me->mind2=1;	// bumped a wall, so Randomy maneuver
			me->facing=(byte)Random(8);
			me->mind3=10;
		}
	}
	else if(me->mind2==1)	// random wandering
	{
		if(!(me->mind3--))	// time to get a new direction
		{
			me->mind2=0;	// get back on track
		}

		me->dx=Cosine(me->facing*32)*me->mind*3/2;
		me->dy=Sine(me->facing*32)*me->mind*3/2;
		if(me->seq!=ANIM_MOVE)
		{
			me->seq=ANIM_MOVE;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128+me->mind*32;
		}
	}

	me->mind1=0;
}

void AI_Yerfdog(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	if(me->z>FIXAMT*8 && me->facing!=1 && abs(me->dz)<FIXAMT*2)
	{
		me->mind1=2-me->facing;
		me->facing=1;
	}
	else if(me->z<=FIXAMT*8)
	{
		me->facing=me->mind1;
	}

	me->dz+=FIXAMT*3/2;
	if(me->z==0)
	{
		me->dz=FIXAMT*3+Random(FIXAMT*7);
	}
}

void AI_EliteSeal(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y;
	byte f;

	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_SEALOUCH,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_SEALDIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frmTimer<32 && me->frm==3)
		{
			f=me->facing*32;
			x=me->x+Cosine(f)*16;
			y=me->y+Sine(f)*16;
			if(me->aiType==MONS_ATOMICSEAL)
			FireBullet(x,y,me->facing,BLT_ROCKET,me->friendly);
			else
			FireBullet(x,y,me->facing,BLT_MISSILE,me->friendly);
			MakeSound(SND_MISSILELAUNCH,x,y,SND_CUTOFF,1050);
			me->reload=10;
		}
		if(me->seq==ANIM_DIE && me->frm>=4)
		{
			for(f=0;f<8;f++)
				SpecialSnow(me->x+((-32+Random(65))<<FIXSHIFT),
							me->y+((-32+Random(65))<<FIXSHIFT));
		}
		return;	// can't do nothin' right now
	}

	if(me->mind==0)		// when mind=0, singlemindedly lumber towards Bouapha
	{
		if(goodguy)
		{
			if(Random(64)==0 && me->reload==0 && RangeToTarget(me,goodguy)<700*FIXAMT)
			{
				// get him!
				me->seq=ANIM_ATTACK;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=256;
				me->action=ACTION_BUSY;
				me->dx=0;
				me->dy=0;
				me->reload=0;
				return;
			}
			FaceGoodguy(me,goodguy);

			me->dx=Cosine(me->facing*32)*2;
			me->dy=Sine(me->facing*32)*2;
			if(me->seq!=ANIM_MOVE)
			{
				me->seq=ANIM_MOVE;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=128;
			}
			if(Random(128)==0)
			{
				me->mind=1;		// occasionally wander
				me->mind1=1;
			}
		}
		else
		{
			me->mind=1;	// if there's no goodguy, get random
			me->mind1=1;
		}
	}
	else if(me->mind==1)	// random wandering
	{
		if(!(me->mind1--))	// time to get a new direction
		{
			if((goodguy) && Random(3)==0)
				me->mind=0;	// get back on track
			else
				me->facing=(byte)Random(8);
			me->mind1=Random(40)+1;
		}

		me->dx=Cosine(me->facing*32)*2;
		me->dy=Sine(me->facing*32)*2;
		if(me->seq!=ANIM_MOVE)
		{
			me->seq=ANIM_MOVE;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
		}
	}
}

void AI_Cone(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y;
	byte f;

	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		/*
		if(me->hp>0)
			MakeSound(SND_SEALOUCH,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_SEALDIE,me->x,me->y,SND_CUTOFF,1200);
			*/
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm==2 && me->reload==0)
		{
			x=me->x+Cosine(me->facing*32)*16;
			y=me->y+Sine(me->facing*32)*16;
			if(me->AttackCheck(35,x>>FIXSHIFT,y>>FIXSHIFT,goodguy))
			{
				goodguy->GetShot(Cosine(me->facing*32)*2,Sine(me->facing*32)*2,3,map,world);
				me->reload=6;
			}
		}
		if(me->seq==ANIM_A1 && me->frm==6 && me->reload==0)
		{
			f=me->facing*32;
			FireExactBullet(me->x,me->y,FIXAMT*10,Cosine(f)*10,Sine(f)*10,0,0,20,f,BLT_BIGSHELL,me->friendly);
			MakeSound(SND_MUSHMISSILE,x,y,SND_CUTOFF,1050);
			me->reload=4;
		}
		if(me->seq==ANIM_DIE)
			me->frmAdvance=256;

		return;	// can't do nothin' right now
	}

	if(me->mind==0)		// when mind=0, don't move
	{
		if(me->ouch)
			me->mind=1;
		return;
	}
	else
	{
		if(RangeToTarget(me,goodguy)<48*FIXAMT && me->reload==0)
		{
			// get him!
			me->seq=ANIM_ATTACK;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
			me->action=ACTION_BUSY;
			me->dx=0;
			me->dy=0;
			me->reload=0;
			return;
		}
		else if(RangeToTarget(me,goodguy)<512*FIXAMT && me->reload==0 &&
			Random(30)==0)
		{
			// get him!
			me->seq=ANIM_A1;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
			me->action=ACTION_BUSY;
			me->dx=0;
			me->dy=0;
			me->reload=0;
			return;
		}

		FaceGoodguy2(me,goodguy);

		me->dx=Cosine(me->facing*32)*2;
		me->dy=Sine(me->facing*32)*2;
		if(me->seq!=ANIM_MOVE)
		{
			me->seq=ANIM_MOVE;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
		}
	}
}

void AI_PygmyDiver(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y,ang;

	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_GLUP,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_DIVERDIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->seq!=ANIM_DIE)
	{
		me->dz=0;
		if(me->z<FIXAMT*10)
			me->z+=FIXAMT/4;
		else me->z=FIXAMT*10;
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm==6 && me->reload==0)
		{
			ang=((me->facing+1)&7)*32;
			x=me->x+Cosine(ang)*16;
			y=me->y+Sine(ang)*16;

			FireExactBullet(x,y,FIXAMT*10,Cosine(me->facing*32)*12,Sine(me->facing*32)*12,FIXAMT,0,90,me->facing,BLT_HARPOON,me->friendly);
			FireExactBullet(x,y,FIXAMT*10,Cosine((me->facing*32+16)&255)*12,Sine((me->facing*32+16)&255)*12,FIXAMT,0,90,me->facing,BLT_HARPOON,me->friendly);
			FireExactBullet(x,y,FIXAMT*10,Cosine((me->facing*32+256-16)&255)*12,Sine((me->facing*32+256-16)&255)*12,FIXAMT,0,90,me->facing,BLT_HARPOON,me->friendly);

			me->reload=2;
		}
		if(me->seq==ANIM_A1 && me->frm==2)
		{
			me->seq=ANIM_MOVE;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
			me->action=ACTION_IDLE;
		}
		return;	// can't do nothin' right now
	}

	if(me->mind==0)	// not currently aware of goodguy
	{
		me->frmAdvance=64;
		if(me->seq==ANIM_MOVE)
		{
			me->seq=ANIM_IDLE;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=64;
		}
		if(me->mind1<101)
			me->mind1++;

		if(me->mind1>100 && Random(200)==0)
		{
			me->facing=(me->facing+(-1+Random(3)))&7;	//change facing
			me->mind1=0;
		}
		if(RangeToTarget(me,goodguy)<350*FIXAMT || me->ouch>0)
		{
			// if the goodguy is near, or he shot me
			me->mind=1;	// start hunting
			me->mind1=120;	// for 4 seconds minimum

			FaceGoodguy(me,goodguy);

			// do to-swim animation
			me->action=ACTION_BUSY;
			me->seq=ANIM_A1;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
			MakeSound(SND_DIVERANGRY,me->x,me->y,SND_CUTOFF,1200);
		}

		me->dx=0;
		me->dy=0;
	}
	else
	{
		FaceGoodguy(me,goodguy);

		if(Random(30)==0 && me->reload==0)
		{
			// fire at him
			MakeSound(SND_SKELKICK,me->x,me->y,SND_CUTOFF,1200);
			me->seq=ANIM_ATTACK;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
			me->action=ACTION_BUSY;
			me->dx=0;
			me->dy=0;
			me->reload=1;
			return;
		}

		if(me->ouch && me->mind1<240)
			me->mind1+=10;
		if(me->mind1)
			me->mind1--;
		else
		{
			if(RangeToTarget(me,goodguy)>380*FIXAMT)
			{
				me->mind=0;	// get bored again
				me->action=ACTION_BUSY;
				me->seq=ANIM_A2;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=128;
			}
			else
				me->mind1=20;	// stay on trail a little longer
			return;
		}
		me->dx=Cosine(me->facing*32)*4;
		me->dy=Sine(me->facing*32)*4;
		if(me->seq!=ANIM_MOVE || (me->seq==ANIM_MOVE && me->frm==12))
		{
			if((me->seq==ANIM_A1 && me->frm==2) || me->seq!=ANIM_A1)
			{
				me->seq=ANIM_MOVE;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=128;
			}
		}
	}
}

void AI_Puffyfish(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,ang;

	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
		{
			if(me->mind<4)
			{
				me->mind++;
				me->mind2=60;
				if(me->mind==4)
				{
					me->hp=1;
					me->GetShot(0,0,1,map,world);
				}
			}
		}
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_DIE && me->frm==3 && me->reload==0)
		{
			MakeSound(SND_PUMPKINDIE,me->x,me->y,SND_CUTOFF,1200);
			me->reload=10;
		}
		if(me->seq==ANIM_DIE && me->frm>=3)
		{
			for(x=0;x<16;x++)
			{
				ang=Random(256);
				FireBullet(me->x,me->y,ang,BLT_SPINE,me->friendly);
			}
		}
		return;	// can't do nothin' right now
	}

	me->mind2--;
	if(me->mind2==0)
	{
		if(me->mind>0)
			me->mind--;

		me->mind2=60;
	}

	switch(me->mind)
	{
		case 0:
			if(me->seq!=ANIM_MOVE || me->frm==8)
			{
				me->seq=ANIM_MOVE;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=128;
			}
			break;
		case 1:
		case 2:
		case 3:
			if(me->seq!=ANIM_A1+me->mind-1 || me->frm==8)
			{
				me->seq=ANIM_A1+me->mind-1;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=128;
			}
			break;
	}


	if(RangeToTarget(me,goodguy)<FIXAMT*128 && me->reload==0)
	{
		if(me->mind<4)
		{
			me->mind++;
			me->mind2=60;
			if(me->mind==4)
			{
				me->hp=1;
				me->GetShot(0,0,1,map,world);
				return;
			}
		}
		me->reload=10;
	}

	if(me->aiType==MONS_PUFFYFISH2)
		me->mind1-=2;
	else
		me->mind1+=2;
	me->facing=(me->mind1/32)&7;

	me->dx=Cosine(me->mind1)*2;
	me->dy=Sine(me->mind1)*2;
}

void AI_Yeti2(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y;
	Guy *g;

	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_YETIOUCH,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_YETIDIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_DIE)
		{
			me->facing=2;	// can only die forward
			if(me->frm>=4)
			{
				x=me->x>>FIXSHIFT;
				y=me->y>>FIXSHIFT;
				BlowUpGuy(x+me->rectx,y+me->recty,x+me->rectx2,y+me->recty2,me->z,2);
				BlowSmoke((x+me->rectx+Random(me->rectx2-me->rectx))<<FIXSHIFT,
						  (y+me->recty+Random(me->recty2-me->recty))<<FIXSHIFT,
						  me->z,FIXAMT);
			}
		}
		if(me->seq==ANIM_ATTACK && me->frm==7 && me->frmTimer<32)
		{
			x=me->x+Cosine(me->facing*32)*72;
			y=me->y+Sine(me->facing*32)*72;
			x+=Cosine(((me->facing+2)*32)&255)*32;
			y+=Sine(((me->facing+2)*32)&255)*32;
			g=AddBaby(x,y,FIXAMT*10,MONS_SNOWBALL+Random(2),me);	// Randomy pick which type of snowball, normal or reverse
			if(g && !g->CanWalk(g->x,g->y,map,world))
				RemoveGuy(g);
			else if(g)
			{
				g->facing=me->facing;
			}
			me->reload=10;
		}
		if(me->seq==ANIM_A2 && me->frm>3)
		{
			ShakeScreen(3);
			// snowballs fall all over
			x=me->x+((320-Random(641))<<FIXSHIFT);
			y=me->y+((240-Random(481))<<FIXSHIFT);
			if(Random(3)==0)
				FireExactBullet(x,y,400*FIXAMT,0,0,0,0,100,me->facing*32,BLT_BIGSNOW,me->friendly);
			else
				FireExactBullet(x,y,400*FIXAMT,0,0,0,0,100,me->facing*32,BLT_SNOWBALL,me->friendly);
		}
		return;	// can't do nothin' right now
	}

	if(me->mind==0)
	{
		if((goodguy && RangeToTarget(me,goodguy)<(300*FIXAMT)) || (me->ouch))
		{
			me->mind=1;	// start into action
			me->mind1=0;

			me->seq=ANIM_A3;	// wave hello first
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=64;
			me->facing=2;
			me->action=ACTION_BUSY;

			MakeSound(SND_OLAFHI,me->x,me->y,SND_CUTOFF,1200);
		}
	}
	else
	{
		if(me->mind1)
			me->mind1--;
		else
		{
			// rotate
			me->facing=(me->facing-1+Random(3))&7;
			me->mind1=Random(30)+1;
		}

		if((!me->reload) && goodguy && Random(20)==0)
		{
			// select an attack method
			x=Random(16);

			if(x<12) // 75% chance of primary weapon
			{
				// Olaf rolls snowballs
				me->seq=ANIM_ATTACK;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=128;
				FaceGoodguy2(me,goodguy);
				me->action=ACTION_BUSY;
			}
			else	// 50% chance of yodel
			{
				MakeSound(SND_YETIYODEL,me->x,me->y,SND_CUTOFF,1500);
				me->seq=ANIM_A2;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=64;
				me->facing=2;
				me->action=ACTION_BUSY;
			}
			me->dx=0;
			me->dy=0;
			return;
		}

		me->dx=Cosine(me->facing*32)*3;
		me->dy=Sine(me->facing*32)*3;

		if(me->seq!=ANIM_MOVE)
		{
			me->seq=ANIM_MOVE;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
		}

	}
}

void AI_Spikey(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y;
	Guy *g;

	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_VAMPOUCH,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_VAMPDIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm>=2 && (me->frm<=6) & me->aiType!=MONS_VAMPLORD)
		{
			x=me->x+Cosine(me->facing*32)*16;
			y=me->y+Sine(me->facing*32)*16;
			if(me->aiType==MONS_DRACULITE)
			{FireExactBullet(me->x,me->y,me->z+24,Cosine(me->facing*32)*12,Sine(me->facing*32)*12,0,0,16,me->facing*32+Random(8)-4,BLT_SPORE,me->friendly);}
			else
			FireBullet(x,y,(me->facing*32-24+Random(49))&255,BLT_SPINE,me->friendly);
			me->reload=5;
		}
		if(me->aiType==MONS_VAMPLORD && me->seq==ANIM_ATTACK && me->frm==5 && me->reload==0)
		{
			me->reload=2;
			x=me->x+Cosine(me->facing*32)*32;
			y=me->y+Sine(me->facing*32)*32;
			g=AddBaby(x,y,FIXAMT*20,MONS_BAT,me);
			if(g && (!g->CanWalk(g->x,g->y,map,world)))
				RemoveGuy(g);
			else if(g)
			{
				MakeSound(SND_BATDIVE,g->x,g->y,SND_CUTOFF,1200);
				g->facing= me->facing;
				g->seq=ANIM_A1;	// diving attack move
				g->frm=0;
				g->frmTimer=0;
				g->frmAdvance=128;
				g->action=ACTION_BUSY;
				g->dx=Cosine(g->facing*32)*12-FIXAMT*2+Random(FIXAMT*4);
				g->dy=Sine(g->facing*32)*12;
			}
		}
		return;	// can't do nothin' right now
	}

	if(me->mind==0)		// when mind=0, singlemindedly drift towards Bouapha
	{
		if(player.garlic && goodguy && RangeToTarget(me,goodguy)<500*FIXAMT)
		{
			MakeSound(SND_VAMPFEAR,me->x,me->y,SND_CUTOFF,1200);
			me->mind=2;	// enter fear mode
			return;
		}

		if(goodguy)
		{
			if((RangeToTarget(me,goodguy)<(512*FIXAMT) && Random(8)==0 && me->aiType!=MONS_VAMPLORD)||(RangeToTarget(me,goodguy)<(512*FIXAMT) && Random(16)==0 && me->aiType==MONS_VAMPLORD))
			{
				// get him!
				MakeSound(SND_VAMPATTK,me->x,me->y,SND_CUTOFF,1200);
				me->seq=ANIM_ATTACK;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=320;
				me->action=ACTION_BUSY;
				me->dx=0;
				me->dy=0;
				me->reload=0;
				return;
			}
			FaceGoodguy(me,goodguy);

			me->dx=Cosine(me->facing*32)*7;
			me->dy=Sine(me->facing*32)*7;
			if(me->seq!=ANIM_MOVE)
			{
				me->seq=ANIM_MOVE;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=64;
			}
			if(me->mind1>0 && RangeToTarget(me,goodguy)>(64*FIXAMT))
			{
				me->mind=1;	// bumped a wall, so Randomy maneuver
				me->facing=(byte)Random(8);
				me->mind2=10;
			}
		}
		else
		{
			me->mind=1;	// if there's no goodguy, get random
			me->facing=(byte)Random(8);
		}
	}
	else if(me->mind==1)	// random wandering
	{
		if(!(me->mind2--))	// time to get a new direction
		{
			me->mind=0;	// get back on track
		}

		me->dx=Cosine(me->facing*32)*6;
		me->dy=Sine(me->facing*32)*6;
		if(me->seq!=ANIM_MOVE)
		{
			me->seq=ANIM_MOVE;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=64;
		}
	}
	else
	{
		if(player.garlic && goodguy)
		{
			// run from the garlic
			FaceGoodguy(me,goodguy);
			me->facing=(me->facing+4)&7;	// face opposite direction
			me->dx=Cosine(me->facing*32)*7;
			me->dy=Sine(me->facing*32)*7;
			if(me->seq!=ANIM_MOVE)
			{
				me->seq=ANIM_MOVE;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=64;
			}
		}
		else
		{
			me->mind=0;
		}
		return;
	}

	me->mind1=0;
}

void AI_Bombot(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y;

	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_ROBOOUCH,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_ROBODIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm==3 && me->reload==0)
		{
			MakeSound(SND_ROBOSHOOT,me->x,me->y,SND_CUTOFF,1200);
			x=me->x+Cosine(((me->facing+2)&7)*32)*24;
			y=me->y+Sine(((me->facing+2)&7)*32)*20;
			FireBullet(x,y,me->facing*32-16+Random(33),BLT_BOMB,me->friendly);
			x=me->x+Cosine(((me->facing-2)&7)*32)*24;
			y=me->y+Sine(((me->facing-2)&7)*32)*20;
			FireBullet(x,y,me->facing*32-16+Random(33),BLT_BOMB,me->friendly);
			me->reload=2;
		}
		if(me->seq==ANIM_DIE && me->frm>3 && me->frmTimer<63)
		{
			x=me->x>>FIXSHIFT;
			y=me->y>>FIXSHIFT;
			BlowUpGuy(x+me->rectx,y+me->recty,x+me->rectx2,y+me->recty2,me->z,1);
		}
		return;	// can't do nothin' right now
	}

	if(me->mind==0)		// when mind=0, singlemindedly roll towards Bouapha
	{
		if(goodguy)
		{
			FaceGoodguy3(me,goodguy);
			if(RangeToTarget(me,goodguy)<(512*FIXAMT) && Random(72)==0)
			{
				// shoot at him
				me->seq=ANIM_ATTACK;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=128;
				me->action=ACTION_BUSY;
				me->dx=0;
				me->dy=0;
				me->reload=0;
				return;
			}
			me->dx=Cosine(me->facing*32)*3;
			me->dy=Sine(me->facing*32)*3;
			if(me->seq!=ANIM_MOVE)
			{
				me->seq=ANIM_MOVE;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=128;
			}
			if(Random(48)==0)
			{
				me->mind=1;		// occasionally wander
				me->mind1=1;
			}
		}
		else
		{
			me->mind=1;	// if there's no goodguy, get random
			me->mind1=1;
		}
	}
	else if(me->mind==1)	// random wandering
	{
		if(goodguy)
		{
			if(RangeToTarget(me,goodguy)<(512*FIXAMT) && Random(256)==0)
			{
				// shoot at him
				me->seq=ANIM_ATTACK;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=128;
				me->action=ACTION_BUSY;
				me->dx=0;
				me->dy=0;
				me->reload=0;
				FaceGoodguy(me,goodguy);
				return;
			}
		}
		if(!(me->mind1--))	// time to get a new direction
		{
			if((goodguy) && Random(3)==0)
				me->mind=0;	// get back on track
			else
				me->facing=(byte)Random(8);
			me->mind1=Random(40)+1;
		}

		me->dx=Cosine(me->facing*32)*3;
		me->dy=Sine(me->facing*32)*3;
		if(me->seq!=ANIM_MOVE)
		{
			me->seq=ANIM_MOVE;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
		}
	}
}

void AI_FollowBunny(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x;

	goodguy=GetGuyOfAIType(MONS_BOUAPHA);
	if(!goodguy)
		return;

	if(me->ouch==4)
	{
		MakeSound(SND_BUNNYDIE,me->x,me->y,SND_CUTOFF,800);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_DIE)
		{
			for(x=0;x<8;x++)
				SpecialSnow(me->x+((-16+Random(33))<<FIXSHIFT),
							me->y+((-16+Random(33))<<FIXSHIFT));
		}
		return;
	}

	if(me->mind==0)	// not activated yet
	{
		if(RangeToTarget(me,goodguy)<72*FIXAMT)
		{
			me->mind=1;
			MakeNormalSound(SND_FOLLOWHI);
		}
	}
	else if(me->mind==1)	// following Bouapha
	{
		Dampen(&me->dx,FIXAMT/8);
		Dampen(&me->dy,FIXAMT/8);

		if(me->x<goodguy->x)
			me->dx+=FIXAMT;
		else
			me->dx-=FIXAMT;
		if(me->y<goodguy->y)
			me->dy+=FIXAMT;
		else
			me->dy-=FIXAMT;

		me->dx+=-FIXAMT/2+Random(FIXAMT);
		me->dy+=-FIXAMT/2+Random(FIXAMT);

		if(RangeToTarget(me,goodguy)<48*FIXAMT)
		{
			me->mind=2;
			me->facing=Random(8);
			me->dx=Cosine(me->facing*32)*2;
			me->dy=Sine(me->facing*32)*2;
			me->mind1=Random(20)+10;
		}
	}
	else // not following, got too close
	{
		me->mind1--;
		if(!me->mind1)
			me->mind=1;
	}

	Clamp(&me->dx,FIXAMT*4);
	Clamp(&me->dy,FIXAMT*4);

	if(me->dx>FIXAMT)
	{
		if(me->dy>FIXAMT)
			me->facing=1;
		else if(me->dy<-FIXAMT)
			me->facing=7;
		else
			me->facing=0;
	}
	else if(me->dx<-FIXAMT)
	{
		if(me->dy>FIXAMT)
			me->facing=3;
		else if(me->dy<-FIXAMT)
			me->facing=5;
		else
			me->facing=4;
	}
	else
	{
		if(me->dy>FIXAMT)
			me->facing=2;
		else if(me->dy<-FIXAMT)
			me->facing=6;
	}

	if(me->mind!=0)
	{
		if(me->seq!=ANIM_MOVE)
		{
			me->seq=ANIM_MOVE;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
		}
	}
}

void AI_Autozoid(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_SERPENTOUCH,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_SERPENTDIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_DIE)
		{
			x=me->x>>FIXSHIFT;
			y=me->y>>FIXSHIFT;
			BlowUpGuy(x+me->rectx,y+me->recty,x+me->rectx2,y+me->recty2,me->z,1);
		}
		return;	// can't do nothin' right now
	}

	if(me->mind==0)	// not activated yet
	{
		if(RangeToTarget(me,goodguy)<512*FIXAMT || me->ouch)
		{
			me->mind=1;
			me->mind2=3;
			MakeNormalSound(SND_CARSTART);
		}
	}
	else if(me->mind==1)	// active, zooming after Bouapha
	{
		if(me->mind2)
			me->mind2--;
		else
		{
			me->mind2=3;
			FaceGoodguy2(me,goodguy);
		}

		Dampen(&me->dx,FIXAMT/8);
		Dampen(&me->dy,FIXAMT/8);

		me->dx+=Cosine(me->facing*32)/2;
		me->dy+=Sine(me->facing*32)/2;

		if(RangeToTarget(me,goodguy)<64*FIXAMT)
		{
			x=me->x+Cosine(me->facing*32)*16;
			y=me->y+Sine(me->facing*32)*16;
			if(me->AttackCheck(35,x>>FIXSHIFT,y>>FIXSHIFT,goodguy))
			{
				goodguy->GetShot(Cosine(me->facing*32)*8,Sine(me->facing*32)*8,10,map,world);
				me->dx=-me->dx;
				me->dy=-me->dy;
				me->mind1=0;
				MakeSound(SND_SERPENTSPIT,me->x,me->y,SND_CUTOFF,1200);
			}
		}
		if(me->mind1)	// hit a wall
		{

			x=(abs(me->dx)+abs(me->dy))*20/(FIXAMT*16);
			if(x<1)
				x=1;
			if(me->mind1&1)
				me->dx=-me->dx/2;
			if(me->mind1&2)
				me->dy=-me->dy/2;
			me->mind1=0;
			me->GetShot(0,0,x,map,world);
			MakeSound(SND_BOMBBOOM,me->x,me->y,SND_CUTOFF,200);
			me->facing=Random(8);
		}

		x=(me->x+me->dx)/(TILE_WIDTH*FIXAMT);
		y=(me->y+me->dy)/(TILE_HEIGHT*FIXAMT);
		if(x>=0 && y>=0 && x<map->width && y<map->height &&
			!map->map[x+y*map->width].wall &&
			(!(GetTerrain(world,map->map[x+y*map->width].floor)->flags&TF_MINECART)))
		{
			// you would've gone offroad
			me->dx=0;
			me->dy=0;
			me->facing=Random(8);
		}

	}

	Clamp(&me->dx,FIXAMT*10);
	Clamp(&me->dy,FIXAMT*10);

	if(me->mind!=0)
	{
		if(me->seq==ANIM_IDLE)
		{
			me->seq=ANIM_MOVE;
			me->frm=0;
			me->frmTimer=0;
		}
		if(me->seq==ANIM_MOVE)
			me->frmAdvance=(abs(me->dx)+abs(me->dy))*512/(FIXAMT*20);
	}
}

void AI_Chest(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y;

	if(me->seq==ANIM_DIE && me->frm==4)
	{
		x=me->x-32*FIXAMT+Random(64*FIXAMT);
		y=me->y-24*FIXAMT+Random(48*FIXAMT);
		ExplodeParticles2(PART_DIRT,x,y,FIXAMT*30,20,5);
	}
	// Just sit there.
}

void AI_Jellofish(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		MakeSound(SND_JELLOOUCH,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->action==ACTION_BUSY)
	{
		return;	// can't do nothin' right now
	}

	if(me->mind==0)	// not currently aware of goodguy
	{
		if(RangeToTarget(me,goodguy)<320*FIXAMT || me->ouch>0)
		{
			// if the goodguy is near, or he shot me
			me->mind=1;	// start hunting
			if(me->ouch==0)
				me->mind1=60;	// for 2 seconds minimum
			else
				me->mind1=120;	// 4 seconds, because they HATE getting shot
			FaceGoodguy2(me,goodguy);
		}
		me->dx=0;
		me->dy=0;
	}
	else if(me->mind==1)
	{
		FaceGoodguy2(me,goodguy);

		if(me->ouch && me->mind1<240)
			me->mind1+=10;
		if(me->mind1)
			me->mind1--;
		else
		{
			if(RangeToTarget(me,goodguy)>256*FIXAMT)
				me->mind=0;	// get bored again
			else
				me->mind1=20;	// stay on trail a little longer
			return;
		}
		me->dx=Cosine(me->facing*32)*3;
		me->dy=Sine(me->facing*32)*3;

		if(RangeToTarget(me,goodguy)<48*FIXAMT)
		{
			me->mind=2;
			me->facing=Random(8);
			me->dx=Cosine(me->facing*32)*2;
			me->dy=Sine(me->facing*32)*2;
			me->mind1=Random(20)+10;
		}
	}
	else if(me->mind==2)
	{
		if(me->mind1)
			me->mind1--;
		else
		{
			if(RangeToTarget(me,goodguy)>256*FIXAMT)
				me->mind=0;	// get bored again
			else
			{
				me->mind=1;
				me->mind1=20;	// stay on trail a little longer
			}
		}
	}

	if(me->mind && RangeToTarget(me,goodguy)<(140*FIXAMT) && Random(5)==0 && me->reload==0)
	{
		// zap him!
		me->reload=8;
		MakeSound(SND_LIGHTNING,me->x,me->y,SND_CUTOFF,1200);
		LightningBolt(me->x,me->y-FIXAMT*30,goodguy->x,goodguy->y-FIXAMT*30);
		goodguy->GetShot(0,0,2,map,world);
		return;
	}

	me->frmAdvance=128;
}

void AI_KingCone(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	byte spd;
	int i,x,y;
	Guy *g;

	if(me->reload)
		me->reload--;
	if(me->mind3)
		me->mind3--;

	if(me->ouch==4)
	{
		if(me->hp<=0)
			MakeSound(SND_KINGDIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm==4 && me->reload==0)
		{
			me->reload=5;
			MakeSound(SND_MUSHMISSILE,me->x,me->y,SND_CUTOFF,1500);
			spd=Random(8)+1;
			for(i=0;i<8;i++)
				FireExactBullet(me->x,me->y,FIXAMT*128,Cosine(i*32)*spd,Sine(i*32)*spd,FIXAMT*4,0,120,0,BLT_GRENADE,me->friendly);
		}
		if(me->seq==ANIM_MOVE && me->frm==6 && me->reload==0)
		{
			MakeSound(SND_BOMBBOOM,me->x,me->y,SND_CUTOFF,1500);
			me->reload=5;
			ShakeScreen(10);
			spd=Random(24)+2;
			for(i=0;i<spd;i++)
			{
				x=me->x+(-128+Random(257))*FIXAMT;
				y=me->y+(-128+Random(257))*FIXAMT;
				if(x<me->x-FIXAMT*64 || x>me->x+FIXAMT*64 || y<me->y-FIXAMT*64 || y>me->y+FIXAMT*64)
				{
					g=AddBaby(x,y,FIXAMT*10,MONS_CONE,me);
					if(g && !g->CanWalk(g->x,g->y,map,world))
						g->type=MONS_NONE;
				}
			}

		}
		if(me->seq==ANIM_DIE && me->reload==0)
		{
			me->reload=Random(5);
			x=me->x>>FIXSHIFT;
			y=me->y>>FIXSHIFT;
			BlowUpGuy(x+me->rectx,y+me->recty,x+me->rectx2,y+me->recty2,me->z,1);
		}
		me->facing=0;
		return;	// can't do nothin' right now
	}

	if(!(me->mind&7))
		me->facing=Random(5);	// look in a different direction

	if(RangeToTarget(me,goodguy)<FIXAMT*800)
		me->mind2=1;

	if(me->mind3==0 && me->ouch && me->hp>30 && RangeToTarget(me,goodguy)<FIXAMT*600)
	{
		switch(Random(3))
		{
			case 0:
				MakeSound(SND_KINGCONES,me->x,me->y,SND_CUTOFF,1500);
				break;
			case 1:
				MakeSound(SND_KINGTICKLE,me->x,me->y,SND_CUTOFF,1500);
				break;
			case 2:
				MakeSound(SND_KINGSCOFF,me->x,me->y,SND_CUTOFF,1500);
				break;
		}
		me->mind3=60+Random(180);
	}

	if(me->mind2)
	{
		if(me->mind)
		{
			me->mind--;
			if(me->mind==0)
			{
				me->mind=120;
				me->seq=ANIM_ATTACK;
				me->frm=0;
				me->frmTimer=0;
				me->action=ACTION_BUSY;
				me->frmAdvance=128;
				me->facing=0;
			}
			if(me->mind==60 && me->mind2)
			{
				me->seq=ANIM_MOVE;
				me->frm=0;
				me->frmTimer=0;
				me->action=ACTION_BUSY;
				me->frmAdvance=128;
				me->facing=0;
			}
		}
		else
			me->mind=120;
	}

	me->dx=0;
	me->dy=0;
}

void AI_Mailboxer(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y;

	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_ZOMBIEOUCH,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_ZOMBIEDIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm==3 && me->reload==0)
		{
			x=me->x+Cosine(me->facing*32)*20;
			y=me->y+Sine(me->facing*32)*20;
			if(me->AttackCheck(24,x>>FIXSHIFT,y>>FIXSHIFT,goodguy))
				goodguy->GetShot(Cosine(me->facing*32)*4,Sine(me->facing*32)*4,4,map,world);
			me->reload=5;
			x=Random(5);
			if(x==0)	// do another quick jab
			{
				me->frm=0;
				me->frmTimer=0;
				MakeSound(SND_SKELKICK,me->x,me->y,SND_CUTOFF,1200);
			}
			else if(x==1)	// do a power punch
			{
				me->dx=Cosine(me->facing*32);
				me->dy=Sine(me->facing*32);
				me->frm=0;
				me->frmTimer=0;
				me->seq=ANIM_A1;
				MakeSound(SND_SKELKICK,me->x,me->y,SND_CUTOFF,1200);
			}
		}
		if(me->seq==ANIM_A1 && me->frm==6 && me->reload==0)
		{
			x=me->x+Cosine(me->facing*32)*20;
			y=me->y+Sine(me->facing*32)*20;
			if(me->AttackCheck(24,x>>FIXSHIFT,y>>FIXSHIFT,goodguy))
				goodguy->GetShot(Cosine(me->facing*32)*8,Sine(me->facing*32)*8,12,map,world);
			me->reload=5;
			me->mind1=1;
		}
		return;	// can't do nothin' right now
	}

	if(me->mind==0)		// when mind=0, singlemindedly lumber towards Bouapha
	{
		if(RangeToTarget(me,goodguy)<(64*FIXAMT) && Random(8)==0)
		{
			// get him!
			MakeSound(SND_SKELKICK,me->x,me->y,SND_CUTOFF,1200);
			me->seq=ANIM_ATTACK;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
			me->action=ACTION_BUSY;
			me->dx=0;
			me->dy=0;
			me->reload=0;
			return;
		}
		FaceGoodguy(me,goodguy);

		me->dx=Cosine(me->facing*32)*4;
		me->dy=Sine(me->facing*32)*4;
		if(me->seq!=ANIM_MOVE)
		{
			me->seq=ANIM_MOVE;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
		}
		if(Random(128)==0)
		{
			me->mind=1;		// occasionally wander
			me->mind1=1;
		}
	}
	else if(me->mind==1)	// random wandering
	{
		if(RangeToTarget(me,goodguy)<(64*FIXAMT) && Random(8)==0)
		{
			// get him!
			MakeSound(SND_SKELKICK,me->x,me->y,SND_CUTOFF,1200);
			me->seq=ANIM_ATTACK;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
			me->action=ACTION_BUSY;
			me->dx=0;
			me->dy=0;
			me->reload=0;
			return;
		}

		if(!(me->mind1--))	// time to get a new direction
		{
			if(Random(3)==0)
				me->mind=0;	// get back on track
			else
				me->facing=(byte)Random(8);
			me->mind1=Random(20)+1;
		}

		me->dx=Cosine(me->facing*32)*4;
		me->dy=Sine(me->facing*32)*4;
		if(me->seq!=ANIM_MOVE)
		{
			me->seq=ANIM_MOVE;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
		}
	}
}

void AI_Piranha(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y;

	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp==0)
			MakeSound(SND_FISHDIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK)
		{
			if(me->frm<4)
			{
				me->dx=Cosine(me->facing*32)*12;
				me->dy=Sine(me->facing*32)*12;
				x=me->x+Cosine(me->facing*32)*10;
				y=me->y+Sine(me->facing*32)*10;
				if(me->AttackCheck(10,x>>FIXSHIFT,y>>FIXSHIFT,goodguy))
				{
					MakeSound(SND_SPD1GNAW,me->x,me->y,SND_CUTOFF,1000);
					goodguy->GetShot(Cosine(me->facing*32)*2,Sine(me->facing*32)*2,2,map,world);
					me->frm=4;	// bounce back
					me->frmTimer=0;
				}
			}
			else
			{
				me->dx=-Cosine(me->facing*32)*6;
				me->dy=-Sine(me->facing*32)*6;
			}
			me->reload=10;
		}
		if(me->seq==ANIM_DIE)
		{
			me->z+=FIXAMT*2-me->dz;
			me->dz=0;
		}
		return;	// can't do nothin' right now
	}

	if(me->mind==2)		// when mind=2, pursue relentlessly
	{
		if(goodguy)
		{
			if(RangeToTarget(me,goodguy)<(96*FIXAMT))
			{
				me->mind=1;	// start circling
			}
			FaceGoodguy2(me,goodguy);

			me->dx=Cosine(me->facing*32)*8;
			me->dy=Sine(me->facing*32)*8;
			if(me->seq!=ANIM_MOVE)
			{
				me->seq=ANIM_MOVE;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=256;
			}
		}
		else
		{
			me->mind=0;	// if there's no goodguy, get random
			me->mind1=1;
		}
	}
	else if(me->mind==0)	// random wandering
	{
		me->mind=2;
	}
	else if(me->mind==1)	// circling
	{
		FaceGoodguy2(me,goodguy);

		// move sideways
		me->dx=Cosine((me->facing*32+64)&255)*6;
		me->dy=Sine((me->facing*32+64)&255)*6;
		if(me->seq!=ANIM_MOVE)
		{
			me->seq=ANIM_MOVE;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=256;
		}

		if(goodguy && RangeToTarget(me,goodguy)>96*FIXAMT)
		{
			me->mind=2;	// chase him down
		}

		if((!me->reload) && Random(16)==0)
		{
			me->seq=ANIM_ATTACK;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
			me->action=ACTION_BUSY;
			me->dx=0;
			me->dy=0;
		}
	}
}

void AI_Yugo(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y;
	int diff,dir;
	static byte noiseLoop=0;

	goodguy=GetGuyOfAIType(MONS_BOUAPHA);
	if(!goodguy)
	{
		if(me->mind>1)
		{
			player.vehicle=0;
			me->type=MONS_NONE;
			x=me->x/FIXAMT;
			y=me->y/FIXAMT;
			BlowUpGuy(x+me->rectx,y+me->recty,x+me->rectx2,y+me->recty2,me->z,1);
		}
		return;
	}

	if(me->mind3)
		me->mind3--;

	if(me->mind>1)
	{
		me->ouch=goodguy->ouch;
		me->facing=me->mind2/32;	// mind2 holds the 'true' visual facing 0-255

		x=goodguy->facing*32;
		// mind2 is the visual facing
		// so rotate mind2 towards the player's actual facing to steer
		if(x>me->mind2)
		{
			diff=x-me->mind2;
			if(diff>128)
			{
				dir=-1;
				diff=256-diff;
			}
			else
				dir=1;
		}
		else if(x<me->mind2)
		{
			diff=me->mind2-x;
			if(diff>128)
			{
				dir=1;
				diff=256-diff;
			}
			else
				dir=-1;
		}
		else
		{
			diff=0;
			dir=0;
		}

		if(diff>8)
			diff=8;

		me->mind2=(me->mind2+diff*dir)&255;
	}

	if(me->mind==0)	// too recently driven, don't allow another drive until player steps away
	{
		if(RangeToTarget(me,goodguy)>32*FIXAMT)
			me->mind=1;	// okay, that's far enough
	}
	else if(me->mind==1)	// waiting to be driven
	{
		if(RangeToTarget(me,goodguy)<32*FIXAMT && player.vehicle==0)
		{
			if(!(GetTerrain(world,map->GetTile(me->mapx,me->mapy)->floor)->flags&TF_MINECART))
			{
				// somehow the car ended up on non-minecart ground.. ridiculous!
				for(x=me->mapx-1;x<=me->mapx+1;x++)
					for(y=me->mapy-1;y<=me->mapy+1;y++)
					{
						if(x>=0 && y>=0 && x<map->width && y<map->height &&
							(GetTerrain(world,map->map[x+y*map->width].floor)->flags&TF_MINECART) &&
							(map->map[x+y*map->width].wall==0) &&
							(!(GetItem(map->map[x+y*map->width].item)->flags&(IF_SOLID|IF_BULLETPROOF)))
							)
						{
							// found a spot that IS minecart neighboring
							me->x=(x*TILE_WIDTH+TILE_WIDTH/2)*FIXAMT;
							me->y=(y*TILE_HEIGHT+TILE_HEIGHT/2)*FIXAMT;
							me->mapx=x;
							me->mapy=y;
							x=me->mapx+2;
							y=me->mapy+2;
						}
					}
			}
			MakeSound(SND_CARSTART,goodguy->x,goodguy->y,SND_CUTOFF,1200);
			noiseLoop=0;
			me->mind=2;
			player.vehicle=VE_YUGO;
			goodguy->parent=me;
			goodguy->x=me->x;
			goodguy->y=me->y;
			me->dx=0;
			me->dy=0;
		}
	}
	else	// being driven
	{
		if(!editing && !player.cheated && verified)
			profile.progress.driveDistance+=abs(me->dx/FIXAMT)+abs(me->dy/FIXAMT);

		noiseLoop++;
		if(noiseLoop>=5)
		{
			noiseLoop=0;
			MakeSound(SND_CARGO,goodguy->x,goodguy->y,SND_CUTOFF,1200);
		}

		x=(me->x+me->dx)/(TILE_WIDTH*FIXAMT);
		y=(me->y+me->dy)/(TILE_HEIGHT*FIXAMT);
		if(x>=0 && y>=0 && x<map->width && y<map->height &&
			!map->map[x+y*map->width].wall &&
			(!(GetTerrain(world,map->map[x+y*map->width].floor)->flags&TF_MINECART)))
		{
			// you would've gotten off of driveable terrain
			me->mind3=0;
			me->x=(me->mapx*TILE_WIDTH+TILE_WIDTH/2)*FIXAMT;
			me->y=(me->mapy*TILE_HEIGHT+TILE_HEIGHT/2)*FIXAMT;
			me->mind=0;
			me->dx=0;
			me->dy=0;
			goodguy->dx=0;
			goodguy->dy=0;
			player.vehicle=0;
			goodguy->parent=NULL;
			me->frm=0;
			me->seq=ANIM_IDLE;
			me->frmTimer=0;
			me->frmAdvance=128;
			return;
		}

		x=(abs(me->dx)+abs(me->dy))*20/(FIXAMT*16);
		if(x<1)
			x=1;
		y=0;
		if(me->mind1&1)	// bounced a horizontal wall
		{
			me->dx=-me->dx/2;
			goodguy->GetShot(0,0,x,map,world);	// bounce, ouch!  More damage at higher speed
			me->ouch=4;	// light the car up red, since you can't see bouapha
			y=1;
		}
		if(me->mind1&2)	// bounced a vertical wall
		{
			me->dy=-me->dy/2;
			if(!y)	// don't get hurt twice for one bounce
				goodguy->GetShot(0,0,x,map,world);	// bounce, ouch!  More damage at higher speed
			me->ouch=4;	// light the car up red, since you can't see bouapha
		}
		me->mind1=0;

		if(goodguy->hp==0)
		{
			player.vehicle=0;
			me->type=MONS_NONE;
			x=me->x/FIXAMT;
			y=me->y/FIXAMT;
			BlowUpGuy(x+me->rectx,y+me->recty,x+me->rectx2,y+me->recty2,me->z,1);
			return;
		}

		if(!(GetTerrain(world,map->map[me->mapx+me->mapy*map->width].floor)->flags&TF_ICE) && me->z==0)
		{
			Dampen(&me->dx,FIXAMT/8);
			Dampen(&me->dy,FIXAMT/8);

			if(me->mind3>60)	// set max speed higher if in turbo mode
			{
				x=FIXAMT*14;
				// and spew particles for power!!
				SpurtParticles(PART_HAMMER,0,me->x,me->y,FIXAMT*4,((me->mind2)+128)&255,6);
				SpurtParticles(PART_HAMMER,1,me->x,me->y,FIXAMT*4,((me->mind2)+128)&255,6);
				MakeSound(SND_FLAMEGO,me->x,me->y,SND_CUTOFF,200);
			}
			else
			{
				x=FIXAMT*9;
			}

			if((me->dx+Cosine(me->mind2)/2<x &&
				me->dx+Cosine(me->mind2)/2>-x) ||
				(me->dx<0 && Cosine(me->mind2)>0) ||
				(me->dx>0 && Cosine(me->mind2)<0))
				me->dx+=Cosine(me->mind2)/2;
			if((me->dy+Sine(me->mind2)/2<x &&
				me->dy+Sine(me->mind2)/2>-x) ||
				(me->dy<0 && Sine(me->mind2)>0) ||
				(me->dy>0 && Sine(me->mind2)<0))
				me->dy+=Sine(me->mind2)/2;
		}
		else if(me->z==0)
		{
			if(me->mind3>60)	// you can only accelerate on ice if in turbo mode
			{
				x=FIXAMT*14;
				// and spew particles for power!!
				SpurtParticles(PART_HAMMER,0,me->x,me->y,FIXAMT*4,((me->mind2)+128)&255,6);
				SpurtParticles(PART_HAMMER,1,me->x,me->y,FIXAMT*4,((me->mind2)+128)&255,6);
				MakeSound(SND_FLAMEGO,me->x,me->y,SND_CUTOFF,200);
				if((me->dx+Cosine(me->mind2)/32<x &&
					me->dx+Cosine(me->mind2)/32>-x))
					me->dx+=Cosine(me->mind2)/32;
				if((me->dy+Sine(me->mind2)/32<x &&
					me->dy+Sine(me->mind2)/32>-x))
					me->dy+=Sine(me->mind2)/32;
			}
		}
		if((GetTerrain(world,map->map[me->mapx+me->mapy*map->width].floor)->flags&TF_MUD) && me->z==0)
		{
			Dampen(&me->dx,FIXAMT/4);
			Dampen(&me->dy,FIXAMT/4);
			if(me->dx!=0 || me->dy!=0)
			{
				SpurtParticles(PART_DIRT,0,me->x,me->y,FIXAMT*4,((me->mind2)+128)&255,3);
				SpurtParticles(PART_DIRT,1,me->x,me->y,FIXAMT*4,((me->mind2)+128)&255,3);
			}
		}
		if((GetTerrain(world,map->map[me->mapx+me->mapy*map->width].floor)->flags&TF_RUBBER) && me->z==0)
		{
			me->dz=FIXAMT*16;
			MakeSound(SND_BOING+Random(3),me->x,me->y,SND_CUTOFF,2);
		}
		if((GetTerrain(world,map->map[me->mapx+me->mapy*map->width].floor)->flags&TF_WATER) && me->z==0)
		{
			goodguy->dx=0;
			goodguy->dy=0;
			goodguy->z=0;
			goodguy->dz=0;
			me->type=MONS_NONE;
			x=me->x/FIXAMT;
			y=me->y/FIXAMT;
			BlowUpGuy(x+me->rectx,y+me->recty,x+me->rectx2,y+me->recty2,me->z,1);
			// drown player:
			if(player.weapon==WPN_PWRARMOR)
				player.weapon=0;
			goodguy->facing=(4+goodguy->facing)&7;
			goodguy->hp=0;
			SetPlayerHP(goodguy->hp);
			goodguy->seq=ANIM_A3;
			goodguy->frm=0;
			goodguy->frmAdvance=255;
			goodguy->frmTimer=0;
			goodguy->action=ACTION_BUSY;
			if(player.playAs==PLAY_LUNACHIK)
				MakeSound(SND_LUNADROWN,x,y,SND_CUTOFF|SND_ONE,65536);
			else if(player.playAs==PLAY_MYSTIC)
				MakeSound(SND_DROWNKM,x,y,SND_CUTOFF|SND_ONE,65536);
			else
				MakeSound(SND_DROWN,x,y,SND_CUTOFF|SND_ONE,65536);
			if(player.shield)
				CompleteGoal(57);	// fell in water while shielded
			return;
		}


		// confine the player to the car
		goodguy->x=me->x+me->dx;
		goodguy->y=me->y+me->dy;
		goodguy->dz=me->dz;
		goodguy->z=me->z;

		if(me->seq!=ANIM_MOVE)
		{
			me->seq=ANIM_MOVE;
			me->frm=0;
			me->frmTimer=0;
		}
		if(me->seq==ANIM_MOVE)
			me->frmAdvance=(abs(me->dx)+abs(me->dy))*512/(FIXAMT*18);
	}
}

void AI_Punkin(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp==0)
			MakeSound(SND_PUMPKINDIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_DIE && me->reload==0)
		{
			ExplodeParticles(PART_HAMMER,me->x,me->y,me->z,8);
			me->reload=2;
		}
		return;	// can't do nothin' right now
	}

	if(me->seq==ANIM_MOVE && me->reload==0)	// hits on this frame
	{
		FindVictim(me->x>>FIXSHIFT,me->y>>FIXSHIFT,24,Cosine(me->facing*32)*7,Sine(me->facing*32)*7,8,map,world,me->friendly);
		me->reload=4;
	}

	if(me->mind==0)	// not currently aware of goodguy
	{

		if(me->seq==ANIM_MOVE)
		{
			me->seq=ANIM_IDLE;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=32;
		}

		if(me->mind1)
			me->mind1--;
		else if(RangeToTarget(me,goodguy)<512*FIXAMT || me->ouch>0)
		{
			// if the goodguy is near, or he shot me
			me->mind=1;	// start charging
			me->mind1=15+Random(15);
			MakeSound(SND_OYOYOY,me->x,me->y,SND_CUTOFF,900);
			FaceGoodguy(me,goodguy);
		}

		me->dx=0;
		me->dy=0;
	}
	else
	{
		if(me->mind1)
			me->mind1--;
		else
		{
			me->mind=0;	// get bored again
			me->mind1=20+Random(40);
			return;
		}
		me->dx=Cosine(me->facing*32)*8;
		me->dy=Sine(me->facing*32)*8;

		if(me->seq!=ANIM_MOVE)
		{
			me->seq=ANIM_MOVE;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=256;
		}
	}
}

void AI_SeaCuke(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y;

	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_CUKEOUCH,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_CUKEDIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm>=6 && me->frm<8 && me->reload==0)
		{
			x=me->x+Cosine(me->facing*32)*16;
			y=me->y+Sine(me->facing*32)*16;
			FireBullet(x,y,me->facing,BLT_DIRTSPIKE,me->friendly);
			me->reload=1;
		}
		if(me->seq==ANIM_A1 && me->frm>2 && me->reload==0)
		{
			me->dx=Cosine(me->facing*32)*10;
			me->dy=Sine(me->facing*32)*10;
			x=me->x+Cosine(me->facing*32)*16;
			y=me->y+Sine(me->facing*32)*16;
			if(me->AttackCheck(16,x>>FIXSHIFT,y>>FIXSHIFT,goodguy))
				goodguy->GetShot(Cosine(me->facing*32)*8,Sine(me->facing*32)*8,8,map,world);
			me->reload=3;
		}
		return;	// can't do nothin' right now
	}

	if(me->mind==0)		// when mind=0, singlemindedly lumber towards Bouapha
	{
		FaceGoodguy2(me,goodguy);

		if(RangeToTarget(me,goodguy)<(512*FIXAMT) && Random(30)==0)
		{
			// get him!
			if(Random(2)==0)
				MakeSound(SND_RUFFIAN,me->x,me->y,SND_CUTOFF,1200);
			else
				MakeSound(SND_HAVEATYOU,me->x,me->y,SND_CUTOFF,1200);
			me->seq=ANIM_ATTACK;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
			me->action=ACTION_BUSY;
			me->dx=0;
			me->dy=0;
			me->reload=0;
			return;
		}
		if(RangeToTarget(me,goodguy)<(256*FIXAMT) && Random(32)==0)
		{
			// dance at him
			FaceGoodguy(me,goodguy);
			MakeSound(SND_CUKEDANCE,me->x,me->y,SND_CUTOFF,1200);
			me->seq=ANIM_A1;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
			me->action=ACTION_BUSY;
			me->dx=Cosine(me->facing*32)*10;
			me->dy=Sine(me->facing*32)*10;
			me->reload=0;
			return;
		}

		me->dx=Cosine(me->facing*32)*4;
		me->dy=Sine(me->facing*32)*4;
		if(me->seq!=ANIM_MOVE || me->frm==12)
		{
			me->seq=ANIM_MOVE;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
		}
		if(Random(64)==0)
		{
			me->mind=1;		// occasionally wander
			me->mind1=1;
		}
	}
	else if(me->mind==1)	// random wandering
	{
		if(RangeToTarget(me,goodguy)<(512*FIXAMT) && Random(30)==0)
		{
			// get him!
			FaceGoodguy(me,goodguy);
					if(Random(2)==0)
				MakeSound(SND_RUFFIAN,me->x,me->y,SND_CUTOFF,1200);
			else
				MakeSound(SND_HAVEATYOU,me->x,me->y,SND_CUTOFF,1200);
			me->seq=ANIM_ATTACK;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
			me->action=ACTION_BUSY;
			me->dx=0;
			me->dy=0;
			me->reload=0;
			return;
		}
		if(RangeToTarget(me,goodguy)<(256*FIXAMT) && Random(32)==0)
		{
			// dance at him
			FaceGoodguy(me,goodguy);
			MakeSound(SND_CUKEDANCE,me->x,me->y,SND_CUTOFF,1200);
			me->seq=ANIM_A1;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
			me->action=ACTION_BUSY;
			me->dx=Cosine(me->facing*32)*10;
			me->dy=Sine(me->facing*32)*10;
			me->reload=0;
			return;
		}
		if(!(me->mind1--))	// time to get a new direction
		{
			if(Random(3)==0)
				me->mind=0;	// get back on track
			else
				me->facing=(byte)Random(8);
			me->mind1=Random(40)+1;
		}

		me->dx=Cosine(me->facing*32)*4;
		me->dy=Sine(me->facing*32)*4;
		if(me->seq!=ANIM_MOVE)
		{
			me->seq=ANIM_MOVE;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
		}
	}
}

void AI_Sneaky2(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y;

	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp==0)
			MakeSound(SND_SHARKDIE,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_SHARKOUCH,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm>=8 && me->frm<16)
		{
			me->frmAdvance=128;
			FireBulletZ(me->x,me->y,FIXAMT*12,((7-(me->frm-8))*32-16+(byte)Random(33))&255,BLT_SHARK,me->friendly);
			FireBulletZ(me->x,me->y,FIXAMT*12,((7-(me->frm-8))*32-16+(byte)Random(33))&255,BLT_SHARK,me->friendly);
		}
		else if(me->seq==ANIM_DIE)
		{
			FireBulletZ(me->x,me->y,FIXAMT*12,Random(256),BLT_SHARK,me->friendly);
			x=me->x>>FIXSHIFT;
			y=me->y>>FIXSHIFT;
			BlowUpGuy(x+me->rectx,y+me->recty,x+me->rectx2,y+me->recty2,me->z,1);
		}
		return;
	}

	if(me->seq==ANIM_IDLE)
	{
		me->frmAdvance=32;
		me->dx=0;
		me->dy=0;
		if(me->mind==0)
			x=128*FIXAMT;	// detects at half-range until he's awakened once already
		else
			x=256*FIXAMT;
		if(RangeToTarget(me,goodguy)<x || me->ouch)
		{
			me->mind=1;
			me->mind1=40+(byte)Random(60);
			MakeSound(SND_SHARKATTACK,me->x,me->y,SND_CUTOFF,1200);
			me->action=ACTION_BUSY;
			me->seq=ANIM_ATTACK;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=256;
			return;
		}
	}
}

void AI_Underzoid(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y;

	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_SERPENTOUCH,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_SERPENTDIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm==6 && me->reload==0 && goodguy)
		{
			// spit stuff
			x=me->x+Cosine(me->facing*32)*50;
			y=me->y+Sine(me->facing*32)*50;
			FireBullet(x,y,me->facing*32,BLT_ENERGY,me->friendly);
			FireBullet(x,y,(me->facing*32+4)&255,BLT_ENERGY,me->friendly);
			FireBullet(x,y,(me->facing*32-4)&255,BLT_ENERGY,me->friendly);
			me->reload=15;
			me->mind=0;
		}
		if(me->seq==ANIM_DIE)
		{
			me->z+=FIXAMT*2-me->dz;
			me->dz=0;
		}
		return;	// can't do nothin' right now
	}

	if(goodguy)
	{
		if(RangeToTarget(me,goodguy)<(512*FIXAMT) && Random(16)==0 && me->reload==0)
		{
			MakeSound(SND_SERPENTSPIT,me->x,me->y,SND_CUTOFF,1200);
			FaceGoodguy(me,goodguy);
			// spit on him!
			me->seq=ANIM_ATTACK;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
			me->action=ACTION_BUSY;
			me->dx=0;
			me->dy=0;
			me->reload=0;
			return;
		}
	}

	if(me->mind)
		me->mind--;
	if(!me->mind)	// time to get a new direction
	{
		me->facing=(byte)Random(8);
		me->mind=Random(60)+1;
	}

	me->dx=Cosine(me->facing*32)*2;
	me->dy=Sine(me->facing*32)*2;
	if(me->seq!=ANIM_MOVE)
	{
		me->seq=ANIM_MOVE;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=128;
	}
}

void AI_Starfish(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y,i,a;

	if(me->reload)
		me->reload--;

	if(me->mind1)
		me->mind1--;

	if(me->ouch==4)
	{
		//if(me->hp>0)
		//	MakeSound(SND_SERPENTOUCH,me->x,me->y,SND_CUTOFF,1200);
		//else
			//MakeSound(SND_SERPENTDIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm>1)
		{
			if(me->z<FIXAMT*10)
				me->z+=FIXAMT*2;
			me->dz=0;
			if(me->frm==9)
			{
				// if Bouapha is still near, keep blasting!
				if(RangeToTarget(me,goodguy)<256*FIXAMT)
				{
					me->frm=2;
				}
			}
			// shoot
			if(me->frm<=9 && me->reload==0)
			{
				me->reload=2;
				me->mind3+=4;	// rotate the angle of fire
				for(i=0;i<5;i++)
				{
					a=(me->mind3+(i*256)/5)&255;
					x=me->x+Cosine(a)*16;
					y=me->y+Sine(a)*16;
					FireBullet(x,y,a,BLT_ENERGY,me->friendly);
				}
				MakeSound(SND_BULLETFIRE,me->x,me->y,SND_CUTOFF,1200);
			}
		}
		return;	// can't do nothin' right now
	}

	if(RangeToTarget(me,goodguy)<256*FIXAMT)
	{
		me->seq=ANIM_ATTACK;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=128;
		me->action=ACTION_BUSY;
		me->dx=0;
		me->dy=0;
		me->reload=0;
		FaceGoodguy(me,goodguy);
		return;
	}
	if(me->mind==0)	// standing around
	{
		if(me->mind1==0 || me->ouch)
		{
			me->mind=1;
			if(!me->ouch)
				me->facing=Random(8);
			else
				FaceGoodguy(me,goodguy);
			me->seq=ANIM_MOVE;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
			me->mind1=Random(50)+15;
			me->dx=0;
			me->dy=0;
		}
	}
	else	// rolling to a good home
	{
		if(me->frm==2)
		{
			me->dx=Cosine(me->facing*32)*4;
			me->dy=Sine(me->facing*32)*4;
		}
		if(me->frm==8 && me->frmTimer>127)
		{
			me->frm=2;
			me->frmTimer=0;
		}
		if(me->mind1==0)
		{
			me->mind=0;
			me->mind1=10+Random(50);
			me->frm=10;
			me->frmTimer=0;
			me->dx=0;
			me->dy=0;
		}
		if(me->ouch)	// if you get hit, head for the source
		{
			FaceGoodguy(me,goodguy);
			me->dx=Cosine(me->facing*32)*4;
			me->dy=Sine(me->facing*32)*4;
			me->mind1+=15;
		}
	}
}

void AI_WetSeal(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y;
	byte f;

	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_SEALOUCH,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_SEALDIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frmTimer<32)
		{
			f=me->facing*32;
			switch(me->frm)
			{
				case 3:
					f-=20;
					break;
				case 5:
					f-=10;
					break;
				case 7:
					f-=4;
					break;
				case 9:
					f+=4;
					break;
				case 11:
					f+=10;
					break;
				case 13:
					f+=20;
					break;
				default:
					return;	// don't fire on this frame
			}
			f=f-8+Random(17);
			x=me->x+Cosine(f)*16;
			y=me->y+Sine(f)*16;
			FireBullet(x,y,f,BLT_ENERGY,me->friendly);
			MakeSound(SND_BULLETFIRE,x,y,SND_CUTOFF,1050);
			me->reload=20;
		}
		if(me->seq==ANIM_DIE && me->frm>=4)
		{
			for(f=0;f<8;f++)
				SpecialSnow(me->x+((-32+Random(65))<<FIXSHIFT),
							me->y+((-32+Random(65))<<FIXSHIFT));
		}
		return;	// can't do nothin' right now
	}

	if(me->mind==0)		// when mind=0, singlemindedly lumber towards Bouapha
	{
		if(goodguy)
		{
			if(Random(64)==0 && me->reload==0 && RangeToTarget(me,goodguy)<700*FIXAMT)
			{
				// get him!
				me->seq=ANIM_ATTACK;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=256;
				me->action=ACTION_BUSY;
				me->dx=0;
				me->dy=0;
				me->reload=0;
				return;
			}
			FaceGoodguy(me,goodguy);

			me->dx=Cosine(me->facing*32)*3;
			me->dy=Sine(me->facing*32)*3;
			if(me->seq!=ANIM_MOVE)
			{
				me->seq=ANIM_MOVE;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=128;
			}
			if(Random(128)==0)
			{
				me->mind=1;		// occasionally wander
				me->mind1=1;
			}
		}
		else
		{
			me->mind=1;	// if there's no goodguy, get random
			me->mind1=1;
		}
	}
	else if(me->mind==1)	// random wandering
	{
		if(!(me->mind1--))	// time to get a new direction
		{
			if((goodguy) && Random(3)==0)
				me->mind=0;	// get back on track
			else
				me->facing=(byte)Random(8);
			me->mind1=Random(40)+1;
		}

		me->dx=Cosine(me->facing*32)*3;
		me->dy=Sine(me->facing*32)*3;
		if(me->seq!=ANIM_MOVE)
		{
			me->seq=ANIM_MOVE;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
		}
	}
}

void AI_WetWilly(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y,i;

	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_WILLYOUCH,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_WILLYDIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm>=3 && me->frm<=7)
		{
			MakeSound(SND_CRAZYSTAB,me->x,me->y,SND_CUTOFF,1200);
			x=me->x+Cosine(me->facing*32)*16;
			y=me->y+Sine(me->facing*32)*16;
			for(i=0;i<3;i++)
				FireBulletZ(x,y,FIXAMT*12,me->facing*32-16+(byte)Random(33),BLT_SHARK,me->friendly);

			me->reload=10;
		}

		if(me->seq==ANIM_DIE && me->frm==8)
		{
			for(x=0;x<16;x++)
				FireBullet(me->x,me->y,(byte)Random(256),BLT_SHARK,me->friendly);
			ExplodeParticles2(PART_WATER,me->x,me->y,FIXAMT*20,10,10);
		}
		return;	// can't do nothin' right now
	}

	if(me->seq==ANIM_MOVE && Random(20)==0)
	{
		x=Random(256);
		if(x<64)	// make sound sometimes
		{
			if(x&1)
				MakeSound(SND_WILLYIDLE,me->x,me->y,SND_CUTOFF,10);
			else
				MakeSound(SND_WILLYIDLE2,me->x,me->y,SND_CUTOFF,10);
		}
		FireExactBullet(me->x,me->y,FIXAMT*14,Cosine(x)*2,Sine(x)*2,Random(4*FIXAMT),0,30,((x+16)&255)/32,BLT_SHARK,me->friendly);
	}

	if(RangeToTarget(me,goodguy)<(128*FIXAMT) && Random(16)==0)
	{
		// get him!
		FaceGoodguy(me,goodguy);
		me->seq=ANIM_ATTACK;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=128;
		me->action=ACTION_BUSY;
		me->dx=0;
		me->dy=0;
		me->reload=0;
		return;
	}

	if(me->mind==0)		// when mind=0, singlemindedly lumber towards Bouapha
	{
		if(goodguy)
		{
			FaceGoodguy(me,goodguy);

			me->dx=Cosine(me->facing*32)*3;
			me->dy=Sine(me->facing*32)*3;
			if(me->seq!=ANIM_MOVE)
			{
				me->seq=ANIM_MOVE;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=128;
			}
			if(Random(64)==0)
			{
				me->mind=1;		// occasionally wander
				me->mind1=1;
			}
		}
		else
		{
			me->mind=1;	// if there's no goodguy, get random
			me->mind1=1;
		}
	}
	else if(me->mind==1)	// random wandering
	{
		if(!(me->mind1--))	// time to get a new direction
		{
			if((goodguy) && Random(3)==0)
				me->mind=0;	// get back on track
			else
				me->facing=(byte)Random(8);
			me->mind1=Random(40)+1;
		}

		me->dx=Cosine(me->facing*32)*3;
		me->dy=Sine(me->facing*32)*3;
		if(me->seq!=ANIM_MOVE)
		{
			me->seq=ANIM_MOVE;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
		}
	}
}

void AI_CrabPuff(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp==0)
			MakeSound(SND_PUMPKINDIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_DIE && me->reload==0)
		{
			ExplodeParticles(PART_WATER,me->x,me->y,me->z,8);
			me->reload=2;
		}
		return;	// can't do nothin' right now
	}

	if(me->seq==ANIM_MOVE && me->reload==0)
	{
		if(FindVictim(me->x>>FIXSHIFT,me->y>>FIXSHIFT,24,Cosine(me->facing*32)*4,Sine(me->facing*32)*4,2,map,world,me->friendly))
		{
			me->reload=4;
		}
	}

	if(me->mind1)
		me->mind1--;
	else
	{
		me->mind1=5;
		FaceGoodguy2(me,goodguy);
	}

	me->dx=Cosine(me->facing*32)*3;
	me->dy=Sine(me->facing*32)*3;

	if(me->seq!=ANIM_MOVE)
	{
		MakeSound(SND_CRABCLICK,me->x,me->y,SND_CUTOFF,2);
		me->seq=ANIM_MOVE;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=256;
	}
}

void AI_Patch(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	Guy *g;
	byte summonTab[]={MONS_PUMPKIN,MONS_PUMPKIN,MONS_BOOMKIN,MONS_SQUASH,MONS_FROG2,MONS_BEETLE};
	byte timerTab[]={30*5,30,30*5,30*3,30*3,30*2};

	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp==0)
			MakeSound(SND_PATCHDIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm==18)
		{
			me->seq=ANIM_A1;
			me->frm=0;
		}
		if(me->seq==ANIM_A1 && me->frm==7 && me->reload==0)
		{
			if(me->aiType<=MONS_PATCH4)
			g=AddBaby(me->x,me->y+FIXAMT*2,0,summonTab[me->aiType-MONS_PATCH],me);
			else
			g=AddBaby(me->x,me->y+FIXAMT*2,0,summonTab[me->aiType-MONS_PATCH5+4],me);
			if(g)
			{
				g->mind=1;
				g->mind1=30;
				g->facing=Random(8);
			}
			if(g && (!g->CanWalk(g->x,g->y,map,world)))
				RemoveGuy(g);
			me->reload=timerTab[me->aiType-MONS_PATCH];
		}
		return;
	}

	if((!me->reload) && Random(50)==0)
	{
		MakeSound(SND_EGGOPEN,me->x,me->y,SND_CUTOFF,1200);
		me->seq=ANIM_ATTACK;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=128;
		me->action=ACTION_BUSY;
	}
}

void AI_Patty(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y;
	byte f;
	Guy *g;

	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_PATTYOUCH,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_PATTYDIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_DIE)
		{
			me->facing=2;
			if(Random(3))
			{
					g=AddBaby(me->x+((-64+Random(129))<<FIXSHIFT),
							me->y+((-64+Random(129))<<FIXSHIFT),0,MONS_CRABPUFF,me);
				if(g && !g->CanWalk(g->x,g->y,map,world))
					RemoveGuy(g);
			}
		}
		if(me->seq==ANIM_ATTACK && me->frm>2 && me->frm<6 && me->reload==0)
		{
			// only actually fire if the range is reasonable to avoid creating excess bullets
			if(goodguy && RangeToTarget(me,goodguy)<700*FIXAMT)
			{
				// spit bubbles
				x=me->x+Cosine(me->facing*32)*50-FIXAMT*4+(Random(9)<<FIXSHIFT);
				y=me->y+Sine(me->facing*32)*50-FIXAMT*4+(Random(9)<<FIXSHIFT);
				f=me->facing*32-16+Random(33);

				FireBullet(x,y,f,BLT_BUBBLE,me->friendly);
			}
			me->reload=0;
		}
		if(me->seq==ANIM_A1 && me->frm>=3 && me->frm<7)
		{
			x=me->x+Cosine(me->facing*32)*48;
			y=me->y+Sine(me->facing*32)*48;
			x=x+Cosine((me->facing*32+64)&255)*32-FIXAMT*16+Random(FIXAMT*33);
			y=y+Sine((me->facing*32+64)&255)*32-FIXAMT*16+Random(FIXAMT*33);

			g=AddBaby(x,y,0,MONS_CRABPUFF,me);
			if(g && !g->CanWalk(x,y,map,world))
				RemoveGuy(g);

			x=me->x+Cosine(me->facing*32)*48;
			y=me->y+Sine(me->facing*32)*48;
			x=x+Cosine((me->facing*32+128+64)&255)*32-FIXAMT*16+Random(FIXAMT*33);
			y=y+Sine((me->facing*32+128+64)&255)*32-FIXAMT*16+Random(FIXAMT*33);

			g=AddBaby(x,y,0,MONS_CRABPUFF,me);
			if(g && !g->CanWalk(x,y,map,world))
				RemoveGuy(g);
		}
		return;	// can't do nothin' right now
	}

	x=RangeToTarget(me,goodguy);
	if(Random(72)==0 && me->reload==0 && x<600*FIXAMT)
	{
		MakeSound(SND_PATTYSUMMON,me->x,me->y,SND_CUTOFF,1200);
		FaceGoodguy(me,goodguy);
		// summon crabbies
		me->seq=ANIM_A1;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=128;
		me->action=ACTION_BUSY;
		me->dx=0;
		me->dy=0;
		me->reload=1;
		return;
	}
	if(x<(350*FIXAMT) && Random(24)==0 && me->reload==0)
	{
		FaceGoodguy(me,goodguy);
		// spew bubbles
		MakeSound(SND_BLOWBUBBLE,me->x,me->y,SND_CUTOFF,1200);
		me->seq=ANIM_ATTACK;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=128;
		me->action=ACTION_BUSY;
		me->dx=0;
		me->dy=0;
		me->reload=1;
		return;
	}

	if(me->mind1)
	{
		me->mind=0;
		me->mind1=0;
	}

	if(me->mind)
		me->mind--;
	if(!me->mind)	// time to get a new direction
	{
		if(Random(5)==0)
		{
			FaceGoodguy(me,goodguy);
			me->facing=(me->facing-2)&7;
		}
		me->facing=(byte)Random(8);
		me->mind=Random(120)+1;

		// move sideways
		if(Random(2)==0)
		{
			me->dx=Cosine((me->facing*32+64)&255)*3;
			me->dy=Sine((me->facing*32+64)&255)*3;
		}
		else
		{
			me->dx=Cosine((me->facing*32+128+64)&255)*3;
			me->dy=Sine((me->facing*32+128+64)&255)*3;
		}
	}


	if(me->seq!=ANIM_MOVE)
	{
		me->seq=ANIM_MOVE;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=128;

		if(me->dx==0 && me->dy==0)
		{
			// move sideways
			if(Random(2)==0)
			{
				me->dx=Cosine((me->facing*32+64)&255)*3;
				me->dy=Sine((me->facing*32+64)&255)*3;
			}
			else
			{
				me->dx=Cosine((me->facing*32+128+64)&255)*3;
				me->dy=Sine((me->facing*32+128+64)&255)*3;
			}
		}
	}
}

void DozerFace(Guy *me,Guy *goodguy)
{
	FaceGoodguy(me,goodguy);

	// central turret position
	if(abs(goodguy->x-me->x)<FIXAMT*64 || goodguy->y>me->y+FIXAMT*512)
	{
		if(goodguy->y<me->y)
		{
			if(me->facing<6)
				me->facing=4;
			else
				me->facing=0;
		}
		else
			me->facing=2;
	}
	else if(goodguy->x<me->x)	// left turret positions
	{
		if(goodguy->y<me->y+FIXAMT*60)
			me->facing=4;
		else
			me->facing=3;
	}
	else	// right turret positions
	{
		if(goodguy->y<me->y+FIXAMT*60)
			me->facing=0;
		else
			me->facing=1;
	}
}

void AI_Dozer(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y,x2,y2;

	x=me->x/FIXAMT+me->rectx;
	x2=me->x/FIXAMT+me->rectx2;
	y=me->y/FIXAMT+me->recty;
	y2=me->y/FIXAMT+me->recty2;

	if(goodguy->y>me->y+FIXAMT*300)
		me->dy+=FIXAMT/16;
	else if(goodguy->y<me->y+FIXAMT*230)
		me->dy-=FIXAMT/16;
	else
		Dampen(&me->dy,FIXAMT/32);

	Dampen(&me->dy,FIXAMT/32);

	Clamp(&me->dy,FIXAMT*4);

	switch(me->mind)
	{
		case 0:	// just born
			me->facing=0;
			me->mind=1;
			break;
		case 1:	// fully armed
			me->facing=0;
			break;
		case 2:	// disarmed
			DozerFace(me,goodguy);
			me->facing+=19;
			break;
		case 3:	// degrilled
			DozerFace(me,goodguy);
			me->facing+=24;
			break;
	}

	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		MakeSound(SND_DOZEROUCH,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK)
		{
			if(me->frm>=3 && me->frm<=14)
			{
				me->mind2+=4;
				FireBullet(me->x-FIXAMT*200+Cosine(me->mind2)*48,me->y+FIXAMT*160+Sine(me->mind2)*48,me->mind2,BLT_ENERGY,me->friendly);
			}
		}
		if(me->seq==ANIM_A1)
		{
			if(me->frm>=3 && me->frm<=14)
			{
				me->mind2-=4;
				FireBullet(me->x+FIXAMT*200+Cosine(me->mind2)*48,me->y+FIXAMT*160+Sine(me->mind2)*48,me->mind2,BLT_ENERGY,me->friendly);
			}
		}
		if(me->seq==ANIM_DIE)
		{
			ShakeScreen(10);
			me->facing=0;
			BlowUpGuy(x-10,y-10,x2+10,y2+10,0,4);
		}
		if(me->seq==ANIM_A2)
		{
			// explode the arms
			ShakeScreen(3);
			BlowUpGuy(x-10,y-10,x+50,y2+10,0,2);
			BlowUpGuy(x2-50,y-10,x2+10,y2+10,0,2);
			if(me->frm==6)
			{
				me->action=ACTION_IDLE;
				me->seq=ANIM_IDLE;
				me->frm=0;
				me->frmTimer=0;
				me->mind=2;	// disarmed!
				DozerFace(me,goodguy);
				me->facing+=19;
			}
		}
		if(me->seq==ANIM_A3)
		{
			// explode the grill
			ShakeScreen(3);
			BlowUpGuy(x+30,y2-50,x2-30,y2+10,0,2);

			if(me->frm==5)
			{
				me->action=ACTION_IDLE;
				me->seq=ANIM_IDLE;
				me->frm=0;
				me->frmTimer=0;
				me->mind=3;	// degrilled
				DozerFace(me,goodguy);
				me->facing+=24;
			}
		}
		return;
	}

	if(goodguy->x>me->x-FIXAMT*200 && goodguy->x<me->x+FIXAMT*200 && goodguy->y>me->y-FIXAMT*160 && goodguy->y<me->y+FIXAMT*160)
		goodguy->GetShot(0,0,1,map,world);

	if(me->mind3)
	{
		me->mind3--;
		if(!me->mind3)
		{
			me->mind3=30*5;
		}
		if(me->mind3<30 && (me->mind3&3)==0)
		{
			MakeSound(SND_MUSHMISSILE,me->x,me->y,SND_CUTOFF,1200);
			FireExactBullet(me->x-FIXAMT*80,me->y-FIXAMT*90,FIXAMT*70,Cosine(0+me->mind3*3)*8,Sine(0+me->mind3*3)*8,FIXAMT*19,0,180,0,BLT_GRENADE,me->friendly);
		}
	}
	else
		me->mind3=30*5;

	if(me->hp<=me->maxHP*0.7 && me->mind<2)	// lose arms at 400hp
	{
		me->seq=ANIM_A2;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=64;
		me->action=ACTION_BUSY;
		return;
	}
	if(me->hp<=me->maxHP*0.4 && me->mind<3)	// lose grill at 200hp
	{
		me->seq=ANIM_A3;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=64;
		me->action=ACTION_BUSY;
		return;
	}

	switch(me->mind)
	{
		case 1:	// fully armed
			if(me->mind1)
			{
				me->mind1--;
				if(me->mind1==0)
					me->mind1=120;
				else if(me->mind1==90)
				{
					MakeSound(SND_DOZERSPIN,me->x,me->y,SND_CUTOFF,600);
					me->mind2=256-16;
					me->seq=ANIM_ATTACK;
					me->frm=0;
					me->frmTimer=0;
					me->frmAdvance=128;
					me->action=ACTION_BUSY;
				}
				else if(me->mind1==30)
				{
					MakeSound(SND_DOZERSPIN,me->x,me->y,SND_CUTOFF,600);
					me->mind2=128+16;
					me->seq=ANIM_A1;
					me->frm=0;
					me->frmTimer=0;
					me->frmAdvance=128;
					me->action=ACTION_BUSY;
				}
			}
			else
				me->mind1=120;
			break;
		case 2:	// disarmed
			if(me->mind1)
			{
				me->mind1--;
				if(me->mind1==0)
					me->mind1=120;
				else if((me->mind1>=90 && me->mind1<100) ||
						(me->mind1>=30 && me->mind1<40))
				{
					x=((me->facing-19)*32-20+Random(41))&255;
					MakeSound(SND_ARMORSHOOT,me->x,me->y,SND_CUTOFF,1200);
					FireExactBullet(me->x+Cosine((me->facing-19)*32)*64,
									me->y-FIXAMT*64+Sine((me->facing-19)*32)*48,
									FIXAMT*70,Cosine(x)*6,Sine(x)*6,-FIXAMT/2,0,90,x,BLT_BIGSHELL,me->friendly);
				}
			}
			else
				me->mind1=120;
			break;
		case 3:	// degrilled
			if(me->mind1)
			{
				me->mind1--;
				if(me->mind1==0)
					me->mind1=120;
				else if((me->mind1>=90 && me->mind1<100) ||
						(me->mind1>=30 && me->mind1<40))
				{
					x=((me->facing-24)*32-20+Random(41))&255;
					MakeSound(SND_ARMORSHOOT,me->x,me->y,SND_CUTOFF,1200);
					FireExactBullet(me->x+Cosine((me->facing-24)*32)*64,
									me->y-FIXAMT*64+Sine((me->facing-24)*32)*48,
									FIXAMT*70,Cosine(x)*6,Sine(x)*6,-FIXAMT/2,0,90,x,BLT_BIGSHELL,me->friendly);
				}
				if(me->mind1==5)
					LaunchMegabeam(me->x,me->y+FIXAMT*140,me->ID);
				if(((me->mind1>=110 && me->mind1<117) ||
					(me->mind1>=60 && me->mind1<67)) && (me->mind1&3)==0)
				{
					MakeSound(SND_ARMORSHOOT,me->x,me->y,SND_CUTOFF,1200);
					for(x=0;x<5;x++)
					{
						FireExactBullet(me->x-FIXAMT*90,
										me->y+FIXAMT*140,
										FIXAMT*20,Cosine(64+40-20*x)*6,Sine(64+40-20*x)*6,-FIXAMT/2,0,90,0,BLT_BIGSHELL,me->friendly);
						FireExactBullet(me->x+FIXAMT*90,
										me->y+FIXAMT*140,
										FIXAMT*20,Cosine(64+40-20*x)*6,Sine(64+40-20*x)*6,-FIXAMT/2,0,90,0,BLT_BIGSHELL,me->friendly);
					}
				}
			}
			else
				me->mind1=120;
			break;
	}
}

void AI_MutantZombie(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y;

	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_ZOMBIEOUCH,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_ZOMBIEDIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->mind3)
		me->mind3--;
	else
	{
		me->mind3=10;
		if(me->hp<me->maxHP && me->hp>0)
			me->hp++;
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm<7)
		{
			for(x=0;x<5;x++)
				FireBullet(me->x-FIXAMT*20+Random(FIXAMT*40),me->y-FIXAMT*20+Random(FIXAMT*40),Random(256),BLT_SPORE,me->friendly);
		}
		if(me->seq==ANIM_ATTACK && me->frm==5)
		{
			me->dx=Cosine(me->facing*32)*12;
			me->dy=Sine(me->facing*32)*12;
		}
		if(me->seq==ANIM_ATTACK && me->frm>6)
		{
			Dampen(&me->dx,FIXAMT/2);
			Dampen(&me->dy,FIXAMT/2);
			if(me->frm==13 && me->frmTimer<64)
			{
				for(x=0;x<256;x+=8)
				{
					FireExactBullet(me->x,me->y,FIXAMT*10,Cosine(x)*12,Sine(x)*12,0,0,16,x,BLT_SPORE,me->friendly);
					FireExactBullet(me->x,me->y,FIXAMT*10,Cosine(x)*6,Sine(x)*6,0,0,16,x,BLT_SPORE,me->friendly);
				}
			}
		}
		if(me->seq==ANIM_ATTACK && me->frm>=7 && me->frm<13 && me->reload==0 && goodguy)
		{
			x=me->x+Cosine(me->facing*32)*16;
			y=me->y+Sine(me->facing*32)*16;
			if(me->AttackCheck(8,x>>FIXSHIFT,y>>FIXSHIFT,goodguy))
			{
				goodguy->GetShot(Cosine(me->facing*32)*4,Sine(me->facing*32)*4,1,map,world);
				me->reload=2;
			}
		}
		return;	// can't do nothin' right now
	}

	if(Random(10)==0)
		FireBullet(me->x-FIXAMT*20+Random(FIXAMT*40),me->y-FIXAMT*20+Random(FIXAMT*40),Random(256),BLT_SPORE,me->friendly);

	if(me->mind==0)		// when mind=0, singlemindedly lumber towards Bouapha
	{
		if(goodguy)
		{
			if(RangeToTarget(me,goodguy)<(200*FIXAMT) && Random(5)==0)
			{
				// get him!
				MakeSound(SND_ZOMBIELEAP,me->x,me->y,SND_CUTOFF,1200);
				me->seq=ANIM_ATTACK;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=128;
				me->action=ACTION_BUSY;
				me->dx=0;
				me->dy=0;
				me->reload=0;
				return;
			}

			FaceGoodguy(me,goodguy);

			me->dx=Cosine(me->facing*32)*2;
			me->dy=Sine(me->facing*32)*2;
			if(me->seq!=ANIM_MOVE)
			{
				me->seq=ANIM_MOVE;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=128;
			}
			if(Random(64)==0)
			{
				me->mind=1;		// occasionally wander
				me->mind1=1;
			}
		}
		else
		{
			me->mind=1;	// if there's no goodguy, get random
			me->mind1=1;
		}
	}
	else if(me->mind==1)	// random wandering
	{
		if(!(me->mind1--))	// time to get a new direction
		{
			if((goodguy) && Random(3)==0)
				me->mind=0;	// get back on track
			else
				me->facing=(byte)Random(8);
			me->mind1=Random(40)+1;
		}

		me->dx=Cosine(me->facing*32)*2;
		me->dy=Sine(me->facing*32)*2;
		if(me->seq!=ANIM_MOVE)
		{
			me->seq=ANIM_MOVE;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
		}
	}
}

void AI_Whatsit(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y;

	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp==0)
			MakeSound(SND_PUMPKINDIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->parent)
	{
		// sitting on The Thing's tongue
		switch(me->parent->frm)
		{
			case 4:
			case 5:
				me->y+=10*FIXAMT;
				break;
			case 6:
			case 7:
				me->y+=15*FIXAMT;
				break;
			case 8:
				me->parent=NULL;	// I am free!
				if(me->hp>0)
				{
					me->seq=ANIM_MOVE;
					me->frm=0;
					me->frmTimer=0;
					if(me->aiType==MONS_DUMBLE)
					me->frmAdvance=192;
					else
					me->frmAdvance=128;
					me->mind1=200;
					me->mind=1;	// pop out in hunt mode
				}
				if(!me->CanWalk(me->x,me->y,map,world))
					me->type=MONS_NONE;	// vanish if this spot is occupied
				break;
		}
		return;
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->reload==0 && me->frm==5)
		{
			x=me->x+Cosine(me->facing*32)*16;
			y=me->y+Sine(me->facing*32)*16;
			if(me->aiType==MONS_DUMBLE)
			{
				FireExactBullet(x,y,FIXAMT*12,Cosine(me->facing*32+256-24)*8,Sine(me->facing*32+256-24)*8,-FIXAMT/4,0,60,me->facing*32,BLT_ENERGY,me->friendly);
				FireExactBullet(x,y,FIXAMT*12,Cosine(me->facing*32+256-12)*8,Sine(me->facing*32+256-12)*8,-FIXAMT/4,0,60,me->facing*32,BLT_ENERGY,me->friendly);
				FireExactBullet(x,y,FIXAMT*12,Cosine(me->facing*32+12)*8,Sine(me->facing*32+12)*8,-FIXAMT/4,0,60,me->facing*32,BLT_ENERGY,me->friendly);
				FireExactBullet(x,y,FIXAMT*12,Cosine(me->facing*32+24)*8,Sine(me->facing*32+24)*8,-FIXAMT/4,0,60,me->facing*32,BLT_ENERGY,me->friendly);
			}
			else
			FireExactBullet(x,y,FIXAMT*12,Cosine(me->facing*32)*8,Sine(me->facing*32)*8,-FIXAMT/4,0,60,me->facing*32,BLT_BIGSHELL,me->friendly);
			me->reload=5;
		}
		return;	// can't do nothin' right now
	}

	if(me->mind==0)	// not currently aware of goodguy
	{
		if(me->seq==ANIM_MOVE)
		{
			me->seq=ANIM_IDLE;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=32;
		}
		if(me->mind1<31)
			me->mind1++;

		if(me->mind1>30 && Random(50)==0)
		{
			switch(Random(16))
			{
				case 0:
					MakeSound(SND_BABYSNORE,me->x,me->y,SND_CUTOFF,800);
					me->seq=ANIM_A1;	// take a nap
					me->frm=0;
					me->frmTimer=0;
					me->frmAdvance=32;
					me->mind1=0;
					me->action=ACTION_BUSY;
					break;
				case 1:
				case 2:
				case 3:

					me->seq=ANIM_A3;	// look right
					me->frm=0;
					me->frmTimer=0;
					me->frmAdvance=64;
					me->mind1=0;
					me->action=ACTION_BUSY;
					break;
				case 4:
				case 5:
				case 6:
					me->seq=ANIM_A4;	// look left
					me->frm=0;
					me->frmTimer=0;
					me->frmAdvance=64;
					me->mind1=0;
					me->action=ACTION_BUSY;
					break;
				case 7:
				case 8:
					me->mind=2;	// go into wander mode
					me->mind1=Random(40)+1;
					break;
				default:	// blink is very common
					me->seq=ANIM_A2;
					me->frm=0;
					me->frmTimer=0;
					me->frmAdvance=128;
					me->mind1=0;
					me->action=ACTION_BUSY;
					break;
			}
		}
		if(RangeToTarget(me,goodguy)<128*FIXAMT || me->ouch>0)
		{
			// if the goodguy is near, or he shot me
			me->mind=1;	// start hunting
			me->mind1=255;

			FaceGoodguy(me,goodguy);
		}
		me->dx=0;
		me->dy=0;
	}
	else if(me->mind==1)	// hunting mode
	{
		FaceGoodguy(me,goodguy);
		if(!goodguy)
		{
			me->mind=0;	// go back to being bored, nothing to see here
			me->mind1=0;
			return;
		}
		if(RangeToTarget(me,goodguy)<(512*FIXAMT) && Random(8)==0 && me->reload==0)
		{
			me->seq=ANIM_ATTACK;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
			me->action=ACTION_BUSY;
			me->dx=0;
			me->dy=0;
			me->reload=1;
			return;
		}

		if(me->ouch && me->mind1<240)
			me->mind1+=10;
		if(me->mind1)
			me->mind1--;
		else
		{
			if(RangeToTarget(me,goodguy)>256*FIXAMT)
				me->mind=0;	// get bored again
			else
				me->mind1=20;	// stay on trail a little longer
			return;
		}
		if(Random(80)==0)
		{
			me->mind1=20;
			me->mind=2;	// wander
		}
		if (me->aiType==MONS_DUMBLE)
		{me->dx=Cosine(me->facing*32)*2;
		me->dy=Sine(me->facing*32)*2;}
		else
		{me->dx=Cosine(me->facing*32)*1;
		me->dy=Sine(me->facing*32)*1;}
		if(me->seq!=ANIM_MOVE)
		{
			me->seq=ANIM_MOVE;
			me->frm=0;
			me->frmTimer=0;
			if (me->aiType==MONS_DUMBLE)
			me->frmAdvance=144;
			else
			me->frmAdvance=96;
		}
	}
	else	// random wander mode
	{
		if(!(me->mind1--))	// time to get a new direction
		{
			switch(Random(3))
			{
				case 0:
					if(goodguy)
						me->mind=1;	// hunt him
					break;
				case 1:
					me->mind=0;	// stop here and rest
					me->mind1=0;
					break;
				case 2:	// just wander a different way
					me->facing=(byte)Random(8);
					me->mind1=Random(20)+1;
					break;
			}
		}

		if (me->aiType==MONS_DUMBLE)
		{me->dx=Cosine(me->facing*32)*2;
		me->dy=Sine(me->facing*32)*2;}
		else
		{me->dx=Cosine(me->facing*32)*1;
		me->dy=Sine(me->facing*32)*1;}
		if(me->seq!=ANIM_MOVE)
		{
			me->seq=ANIM_MOVE;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=96;
		}
	}
	// this is needed because his idle frame doesn't fit into his walking animation, so this skips it
	if(me->seq==ANIM_MOVE && me->frm==10 && me->frmTimer>127)
	{
		me->frm=0;
		me->frmTimer=0;
		if (me->aiType==MONS_DUMBLE)
		me->frmAdvance=144;
		else
		me->frmAdvance=96;
	}
}

void AI_ShockGhost(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
		{
			MakeSound(SND_GHOSTOUCH,me->x,me->y,SND_CUTOFF,1200);
		}
		else
			MakeSound(SND_GHOSTDIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(Random(3)==0)
	{
		if(RangeToTarget(me,goodguy)<180*FIXAMT && me->hp>0)
		{
			goodguy->GetShot(0,0,1,map,world);
			LightningBolt(
					me->x-FIXAMT*32+Random(FIXAMT*64),
					me->y-FIXAMT*82+Random(FIXAMT*64),
					goodguy->x-FIXAMT*32+Random(FIXAMT*64),
					goodguy->y-FIXAMT*52+Random(FIXAMT*64));
			MakeSound(SND_LIGHTNING,me->x,me->y,SND_CUTOFF,400);
		}
		else
		{
			// zap around yourself
			LightningBolt(
					me->x-FIXAMT*32+Random(FIXAMT*64),
					me->y-FIXAMT*82+Random(FIXAMT*64),
					me->x-FIXAMT*32+Random(FIXAMT*64),
					me->y-FIXAMT*82+Random(FIXAMT*64));
		}
		map->BrightTorch((me->x/TILE_WIDTH)>>FIXSHIFT,
							 (me->y/TILE_HEIGHT)>>FIXSHIFT,8,4);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm==2 && me->reload==0)
		{
			me->reload=10;
		}

		return;	// can't do nothin' right now
	}

	if(me->mind==0 && me->hp>0)	// hasn't spotted Bouapha yet
	{
		if((goodguy && RangeToTarget(me,goodguy)<500*FIXAMT) || (me->ouch>0))
		{
			MakeSound(SND_GHOSTYELL,me->x,me->y,SND_CUTOFF,1200);
			me->mind=1;
			me->action=ACTION_BUSY;
			me->seq=ANIM_ATTACK;	// materialize
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
			me->dx=0;
			me->dy=0;
		}
		return;
	}
	else if(me->mind==1)	// has spotted him, give chase
	{
		FaceGoodguy3(me,goodguy);
		me->dx=Cosine(me->facing*32)*2;
		me->dy=Sine(me->facing*32)*2;
		if(RangeToTarget(me,goodguy)<250*FIXAMT && Random(60)==1)
		{
			// scream
			MakeSound(SND_GHOSTYELL,me->x,me->y,SND_CUTOFF,1200);
			me->action=ACTION_BUSY;
			me->seq=ANIM_ATTACK;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=72;
			me->dx=0;
			me->dy=0;
			return;
		}
	}
}

void AI_Mechazoid(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y;

	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_SERPENTOUCH,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_SERPENTDIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm>=6 && me->frm<=10)
		{
			// spit stuff
			x=me->x+Cosine(me->facing*32)*50;
			y=me->y+Sine(me->facing*32)*50;
			FireBullet(x,y,(me->facing*32-12+Random(25))&255,BLT_ENERGY,me->friendly);
			me->reload=15;
			me->mind=0;
		}
		if(me->seq==ANIM_DIE && me->frm>3 && Random(4)==0)
		{
			x=me->x>>FIXSHIFT;
			y=me->y>>FIXSHIFT;
			BlowUpGuy(x+me->rectx,y+me->recty,x+me->rectx2,y+me->recty2,me->z,1);
		}
		return;	// can't do nothin' right now
	}

	if(goodguy)
	{
		if(RangeToTarget(me,goodguy)<(512*FIXAMT) && Random(16)==0 && me->reload==0)
		{
			MakeSound(SND_SERPENTSPIT,me->x,me->y,SND_CUTOFF,1200);
			FaceGoodguy(me,goodguy);
			// spit on him!
			me->seq=ANIM_ATTACK;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
			me->action=ACTION_BUSY;
			me->dx=0;
			me->dy=0;
			me->reload=0;
			return;
		}
	}

	if(me->mind)
		me->mind--;
	if(!me->mind)	// time to get a new direction
	{
		me->facing=(byte)Random(8);
		me->mind=Random(60)+1;
	}

	me->dx=Cosine(me->facing*32)*2;
	me->dy=Sine(me->facing*32)*2;
	if(me->seq!=ANIM_MOVE)
	{
		me->seq=ANIM_MOVE;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=128;
	}
}

void AI_Sucker(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int smlFactor,bigFactor;

	if(me->reload)
		me->reload--;

	if(me->mind1<255)
		me->mind1++;

	if(me->seq!=ANIM_MOVE)
	{
		me->seq=ANIM_MOVE;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=0;
	}

	if(me->aiType==MONS_SUCKER1 || me->aiType==MONS_SUCKER3)	// normal strength
	{
		smlFactor=128;
		bigFactor=1024;
	}
	else	// strong sucking
	{
		smlFactor=32;
		bigFactor=256;
	}

	if(me->aiType==MONS_SUCKER3 || me->aiType==MONS_SUCKER4)	// short range
		if(RangeToTarget(me,goodguy)>512*FIXAMT)
			return;

	if(goodguy)
	{
		if(goodguy->x<me->x)
		{
			goodguy->dx+=(me->mind1*FIXAMT/smlFactor);
			goodguy->dy+=(me->mind1*FIXAMT/bigFactor);
		}
		else
		{
			goodguy->dx-=(me->mind1*FIXAMT/smlFactor);
			goodguy->dy-=(me->mind1*FIXAMT/bigFactor);
		}
		if(goodguy->y<me->y)
		{
			goodguy->dy+=(me->mind1*FIXAMT/smlFactor);
			goodguy->dx-=(me->mind1*FIXAMT/bigFactor);
		}
		else
		{
			goodguy->dy-=(me->mind1*FIXAMT/smlFactor);
			goodguy->dx+=(me->mind1*FIXAMT/bigFactor);
		}
		Clamp(&goodguy->dx,FIXAMT*6);
		Clamp(&goodguy->dy,FIXAMT*6);
	}
}

void AI_Blower(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int smlFactor,bigFactor;

	if(me->reload)
		me->reload--;

	if(me->mind1<255)
		me->mind1++;

	if(me->seq!=ANIM_MOVE)
	{
		me->seq=ANIM_MOVE;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=0;
	}

	if(me->aiType==MONS_BLOWER1 || me->aiType==MONS_BLOWER3)	// normal strength
	{
		smlFactor=128;
		bigFactor=1024;
	}
	else	// strong blowing
	{
		smlFactor=32;
		bigFactor=256;
	}

	if(me->aiType==MONS_BLOWER3 || me->aiType==MONS_BLOWER4)	// short range
		if(RangeToTarget(me,goodguy)>512*FIXAMT)
			return;

	if(goodguy)
	{
		if(goodguy->x<me->x)
		{
			goodguy->dx-=(me->mind1*FIXAMT/smlFactor);
			goodguy->dy-=(me->mind1*FIXAMT/bigFactor);
		}
		else
		{
			goodguy->dx+=(me->mind1*FIXAMT/smlFactor);
			goodguy->dy+=(me->mind1*FIXAMT/bigFactor);
		}
		if(goodguy->y<me->y)
		{
			goodguy->dy-=(me->mind1*FIXAMT/smlFactor);
			goodguy->dx+=(me->mind1*FIXAMT/bigFactor);
		}
		else
		{
			goodguy->dy+=(me->mind1*FIXAMT/smlFactor);
			goodguy->dx-=(me->mind1*FIXAMT/bigFactor);
		}
		Clamp(&goodguy->dx,FIXAMT*6);
		Clamp(&goodguy->dy,FIXAMT*6);
	}
}

void AI_Beetle(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp==0)
			MakeSound(SND_PUMPKINDIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_DIE && me->reload==0)
		{
			ExplodeParticles(PART_SNOW2,me->x,me->y,me->z,4);
			me->reload=2;
		}
		return;	// can't do nothin' right now
	}

	if(me->seq==ANIM_MOVE && me->reload==0)
	{
		if(FindVictim(me->x>>FIXSHIFT,me->y>>FIXSHIFT,24,Cosine(me->facing*32)*4,Sine(me->facing*32)*4,2,map,world,me->friendly))
		{
			me->reload=4;
		}
	}

	if(me->mind1)
		me->mind1--;
	else
	{
		me->mind1=5;
		FaceGoodguy2(me,goodguy);
	}

	me->dx=Cosine(me->facing*32)*4;
	me->dy=Sine(me->facing*32)*4;

	if(me->seq!=ANIM_MOVE)
	{
		MakeSound(SND_CRABCLICK,me->x,me->y,SND_CUTOFF,2);
		me->seq=ANIM_MOVE;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=256;
	}
}

void AI_Scarab(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y,i;
	Guy *g;

	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_SPD3OUCH,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_SPD3DIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_DIE)
		{
			me->facing=2;
			if(Random(3))
			{
					g=AddBaby(me->x+((-64+Random(129))<<FIXSHIFT),
							me->y+((-64+Random(129))<<FIXSHIFT),0,MONS_BEETLE,me);
				if(g && !g->CanWalk(g->x,g->y,map,world))
					RemoveGuy(g);
			}
		}
		if(me->seq==ANIM_ATTACK && me->frm>2 && me->frm<9 && me->reload==0)
		{
			// only actually fire if the range is reasonable to avoid creating excess bullets
			if(goodguy && RangeToTarget(me,goodguy)<700*FIXAMT)
			{
				// spit flames
				for(i=0;i<4;i++)
				{
					x=me->x+Cosine(me->facing*32)*50-FIXAMT*16+(Random(33)<<FIXSHIFT);
					y=me->y+Sine(me->facing*32)*50-FIXAMT*16+(Random(33)<<FIXSHIFT);
					FireBullet(x,y,me->facing,BLT_FLAME,me->friendly);
				}
			}
			me->reload=0;
		}
		if(me->seq==ANIM_A1 && me->frm>=3 && me->frm<7)
		{
			x=me->x+Cosine(me->facing*32)*48;
			y=me->y+Sine(me->facing*32)*48;
			x=x+Cosine((me->facing*32+64)&255)*32-FIXAMT*16+Random(FIXAMT*33);
			y=y+Sine((me->facing*32+64)&255)*32-FIXAMT*16+Random(FIXAMT*33);

			g=AddBaby(x,y,0,MONS_BEETLE,me);
			if(g && !g->CanWalk(x,y,map,world))
				RemoveGuy(g);

			x=me->x+Cosine(me->facing*32)*48;
			y=me->y+Sine(me->facing*32)*48;
			x=x+Cosine((me->facing*32+128+64)&255)*32-FIXAMT*16+Random(FIXAMT*33);
			y=y+Sine((me->facing*32+128+64)&255)*32-FIXAMT*16+Random(FIXAMT*33);

			g=AddBaby(x,y,0,MONS_BEETLE,me);
			if(g && !g->CanWalk(x,y,map,world))
				RemoveGuy(g);
		}
		return;	// can't do nothin' right now
	}

	x=RangeToTarget(me,goodguy);
	if(Random(72)==0 && me->reload==0 && x<600*FIXAMT)
	{
		MakeSound(SND_SPD3SPEW,me->x,me->y,SND_CUTOFF,1200);
		FaceGoodguy(me,goodguy);
		// summon beetles
		me->seq=ANIM_A1;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=128;
		me->action=ACTION_BUSY;
		me->dx=0;
		me->dy=0;
		me->reload=1;
		return;
	}
	if(x<(350*FIXAMT) && Random(24)==0 && me->reload==0)
	{
		MakeSound(SND_SPD3CHOMP,me->x,me->y,SND_CUTOFF,1200);
		FaceGoodguy(me,goodguy);
		// spew flame
		me->seq=ANIM_ATTACK;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=128;
		me->action=ACTION_BUSY;
		me->dx=0;
		me->dy=0;
		me->reload=1;
		return;
	}

	if(me->mind1)
	{
		me->mind=0;
		me->mind1=0;
	}

	if(me->mind)
		me->mind--;
	if(!me->mind)	// time to get a new direction
	{
		if(Random(5)==0)
		{
			FaceGoodguy2(me,goodguy);
		}
		me->facing=(byte)Random(8);
		me->mind=Random(60)+1;

		me->dx=Cosine((me->facing*32)&255)*2;
		me->dy=Sine((me->facing*32)&255)*2;
	}


	if(me->seq!=ANIM_MOVE)
	{
		me->seq=ANIM_MOVE;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=128;

		if(me->dx==0 && me->dy==0)
		{
			me->dx=Cosine((me->facing*32)&255)*2;
			me->dy=Sine((me->facing*32)&255)*2;
		}
	}
}

void AI_Frostbiter(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y;

	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_SPD2OUCH,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_SPD2DIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm==6 && me->reload==0)
		{
			// spit ice shot
			MakeSound(SND_SPD2SPIT,me->x,me->y,SND_CUTOFF,1200);
			x=me->x+Cosine(me->facing*32)*8;
			y=me->y+Sine(me->facing*32)*8;
			FireBullet(x,y,me->facing*32,BLT_FREEZE,me->friendly);
			me->reload=40;
		}
		return;	// can't do nothin' right now
	}

	if(goodguy)
	{
		if(RangeToTarget(me,goodguy)<(256*FIXAMT) && Random(32)==0 && me->reload==0)
		{
			FaceGoodguy(me,goodguy);
			// spit on him!
			me->seq=ANIM_ATTACK;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
			me->action=ACTION_BUSY;
			me->dx=0;
			me->dy=0;
			me->reload=0;
			return;
		}
	}

	if(me->mind)
		me->mind--;
	if(!me->mind)	// time to get a new direction
	{
		me->facing=(byte)Random(8);
		me->mind=Random(120)+1;
	}

	me->dx=Cosine(me->facing*32)*3;
	me->dy=Sine(me->facing*32)*3;
	if(me->seq!=ANIM_MOVE)
	{
		me->seq=ANIM_MOVE;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=128;
	}
}

void AI_UnderMagmazoid(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y;

	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_SERPENTOUCH,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_SERPENTDIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm>=6 && me->frm<=12 && me->reload==0 && goodguy)
		{
			// spit stuff
			x=me->x+Cosine(me->facing*32)*50;
			y=me->y+Sine(me->facing*32)*50;
			FireBullet(x,y,me->facing,BLT_FLAME2,me->friendly);
			me->reload=2;
			me->mind=0;
		}
		if(me->seq==ANIM_ATTACK && me->frm==13 && me->reload==0)
			me->reload=20;

		if(me->seq==ANIM_DIE)
		{
			me->z+=FIXAMT*2-me->dz;
			me->dz=0;
		}
		return;	// can't do nothin' right now
	}

	if(goodguy)
	{
		if(RangeToTarget(me,goodguy)<(512*FIXAMT) && Random(16)==0 && me->reload==0)
		{
			FaceGoodguy(me,goodguy);
			// spit on him!
			me->seq=ANIM_ATTACK;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
			me->action=ACTION_BUSY;
			me->dx=0;
			me->dy=0;
			me->reload=0;
			return;
		}
	}

	if(me->mind)
		me->mind--;
	if(!me->mind)	// time to get a new direction
	{
		me->facing=(byte)Random(8);
		me->mind=Random(60)+1;
	}

	me->dx=Cosine(me->facing*32)*2;
	me->dy=Sine(me->facing*32)*2;
	if(me->seq!=ANIM_MOVE)
	{
		me->seq=ANIM_MOVE;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=128;
	}
}

void AI_ParkedCar(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	if(me->reload)
		me->reload--;

	if(me->mind==0)
	{
		me->facing=Random(8);
		me->mind=1;	// randomize the facings for art
	}
	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_DIE && me->reload==0)
		{
			FireBullet(me->x,me->y,0,BLT_BOOM,me->friendly);
			me->reload=5;
		}
		return;	// can't do nothin' right now
	}
}

void AI_Traffic(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y;
	byte ok[4];
	int diff,dir;
	static byte noiseLoop=0;

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_DIE && me->reload==0)
		{
			FireBullet(me->x,me->y,0,BLT_BOOM,me->friendly);
			me->reload=10;
		}
		return;	// can't do nothin' right now
	}

	if(me->mind==0)
	{
		me->friendly=1-me->friendly;
		FindVictims((me->x+me->dx)>>FIXSHIFT,(me->y+me->dy)>>FIXSHIFT,16,-FIXAMT*10+Random(FIXAMT*21),-FIXAMT*10+Random(FIXAMT*21),20,map,world,me->friendly);
		me->friendly=1-me->friendly;
		FindVictims((me->x+me->dx)>>FIXSHIFT,(me->y+me->dy)>>FIXSHIFT,16,-FIXAMT*10+Random(FIXAMT*21),-FIXAMT*10+Random(FIXAMT*21),20,map,world,me->friendly);
	}

	me->facing=me->mind2/32;	// mind2 holds the 'true' visual facing 0-255

	// mind2 is the visual facing, mind3 is the actual facing,
	// so rotate mind2 towards mind3
	if(me->mind3>me->mind2)
	{
		diff=me->mind3-me->mind2;
		if(diff>128)
		{
			dir=-1;
			diff=256-diff;
		}
		else
			dir=1;
	}
	else if(me->mind3<me->mind2)
	{
		diff=me->mind2-me->mind3;
		if(diff>128)
		{
			dir=1;
			diff=256-diff;
		}
		else
			dir=-1;
	}
	else
	{
		diff=0;
		dir=0;
	}

	if(diff>me->mind1/4)
		diff=me->mind1/4;

	me->mind2=(me->mind2+diff*dir)&255;

	if(!me->mind)	// not stuck
	{
		noiseLoop++;
		if(noiseLoop>=5)
		{
			noiseLoop=0;
			MakeSound(SND_CARGO,me->x,me->y,SND_CUTOFF,1);
		}
	}

	// if you've reached the center of a tile, time to decide where to go
	x=(me->mapx*TILE_WIDTH+TILE_WIDTH/2)*FIXAMT;
	y=(me->mapy*TILE_HEIGHT+TILE_HEIGHT/2)*FIXAMT;
	if((me->x<=x && me->oldx>x) || (me->x>=x && me->oldx<x) ||
	   (me->y<=y && me->oldy>y) || (me->y>=y && me->oldy<y))
	{
		me->x=x;
		me->y=y;
		// figure out which directions are valid minecart paths
		if(me->mapx<map->width-1 && (GetTerrain(world,map->map[me->mapx+1+me->mapy*map->width].floor)->flags&TF_MINECART) &&
			!(GetTerrain(world,map->map[me->mapx+1+me->mapy*map->width].floor)->flags&TF_SOLID) &&
			!(GetItem(map->map[me->mapx+1+me->mapy*map->width].item)->flags&IF_SOLID))
			ok[0]=1;
		else
			ok[0]=0;
		if(me->mapy<map->height-1 && (GetTerrain(world,map->map[me->mapx+(me->mapy+1)*map->width].floor)->flags&TF_MINECART) &&
			!(GetTerrain(world,map->map[me->mapx+(me->mapy+1)*map->width].floor)->flags&TF_SOLID) &&
			!(GetItem(map->map[me->mapx+(me->mapy+1)*map->width].item)->flags&IF_SOLID))
			ok[1]=1;
		else
			ok[1]=0;
		if(me->mapx>0 && (GetTerrain(world,map->map[me->mapx-1+me->mapy*map->width].floor)->flags&TF_MINECART) &&
			!(GetTerrain(world,map->map[me->mapx-1+me->mapy*map->width].floor)->flags&TF_SOLID) &&
			!(GetItem(map->map[me->mapx-1+me->mapy*map->width].item)->flags&IF_SOLID))
			ok[2]=1;
		else
			ok[2]=0;
		if(me->mapy>0 && (GetTerrain(world,map->map[me->mapx+(me->mapy-1)*map->width].floor)->flags&TF_MINECART) &&
			!(GetTerrain(world,map->map[me->mapx+(me->mapy-1)*map->width].floor)->flags&TF_SOLID) &&
			!(GetItem(map->map[me->mapx+(me->mapy-1)*map->width].item)->flags&IF_SOLID))
			ok[3]=1;
		else
			ok[3]=0;

		ok[(((me->mind3/32)+4)&7)/2]=0;

		// count how many directions are available
		y=0;
		for(x=0;x<4;x++)
			if(ok[x])
				y++;

		if(y==0)	// no directions to go!
		{
			me->mind=1;
			me->dx=0;
			me->dy=0;
			return;
		}
		else
		{
			if(y==1)
			{
				// face the only valid direction
				for(x=0;x<4;x++)
					if(ok[x])
						me->mind3=x*64;
			}
			else
			{
				// if you can go straight, then that's what you do
				if(ok[me->mind3/64])
				{
					me->mind3=(me->mind3/64)*64;
				}
				else
				{
					// in the case of multiple directions, we must decide on one
					// based on the type of car
					if(me->aiType==MONS_TRAFFIC)
					{
						if(ok[(me->mind3/64+1)&3])	// direction to your right
							me->mind3=((me->mind3/64+1)&3)*64;
						else if(ok[(me->mind3/64/2+3)&3])	// direction to your left
							me->mind3=((me->mind3/64+3)&3)*64;
						else
						{
							// no valid direction except back!
							me->mind=1;
						}
					}
					else
					{
						if(ok[(me->mind3/64+3)&3])	// direction to your left
							me->mind3=((me->mind3/64+3)&3)*64;
						else if(ok[(me->mind3/64+1)&3])	// direction to your right
							me->mind3=((me->mind3/64+1)&3)*64;
						else
						{
							// no valid direction except back!
							me->mind=1;
						}
					}

				}
			}
		}
	}

	// move forward at that pace
	me->dx=Cosine(me->mind3)*(me->mind1/8);
	me->dy=Sine(me->mind3)*(me->mind1/8);

	if(me->mind1<64)
		me->mind1++;

	if(me->seq!=ANIM_MOVE)
	{
		me->seq=ANIM_MOVE;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=256;
	}
}

void AI_ShroomLord(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y,i,j;
	Guy *g;
	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_MUSHOUCH,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_MUSHDIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm>=6 && me->frm<=8 && me->reload==0)
		{
			if(me->frm==6)
				MakeSound(SND_SKELSHOOT,me->x,me->y,SND_CUTOFF,1200);
			i=(me->facing*32-32+me->frm)&255;
			for(j=-32+me->frm;j<33;j+=me->frm*2)
			{
				x=me->x+Cosine(i)*32;
				y=me->y+Sine(i)*24;
				FireExactBullet(x,y,FIXAMT*20,Cosine(i)*8,Sine(i)*8,0,0,16,i,BLT_SPORE,me->friendly);
				i=(i+me->frm*2)&255;
			}
			me->reload=1;
		}
		if(me->seq==ANIM_DIE && me->frm==1 && me->frmTimer<64)
		{
			g=AddBaby(me->x+(32<<FIXSHIFT),me->y,0,MONS_SHROOM,me);
			if(g && (!g->CanWalk(g->x,g->y,map,world)))
				RemoveGuy(g);
			g=AddBaby(me->x-(32<<FIXSHIFT),me->y,0,MONS_SHROOM,me);
			if(g && (!g->CanWalk(g->x,g->y,map,world)))
				RemoveGuy(g);
			for(i=0;i<30;i++)
				BlowSmoke(me->x-32*FIXAMT+Random(64*FIXAMT),
					  	 me->y-32*FIXAMT+Random(64*FIXAMT),
								Random(64)<<FIXSHIFT,Random(FIXAMT*4));
		}
		return;	// can't do nothin' right now
	}

	if(me->mind==0)		// when mind=0, charge towards Bouapha
	{
		if(goodguy)
		{
			if(RangeToTarget(me,goodguy)<(128*FIXAMT) && Random(8)==0)
			{
				// get him!
				me->seq=ANIM_ATTACK;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=128;
				me->action=ACTION_BUSY;
				me->dx=0;
				me->dy=0;
				return;
			}
			FaceGoodguy2(me,goodguy);

			me->dx=Cosine(me->facing*32)*4;
			me->dy=Sine(me->facing*32)*4;
			if(me->seq!=ANIM_MOVE)
			{
				me->seq=ANIM_MOVE;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=128;
			}
			if(Random(256)==0)
			{
				me->mind=1;		// occasionally rest
				me->mind1=(byte)Random(100);
			}
		}
		else
		{
			me->mind=1;	// if there's no goodguy, sit around
			me->mind1=(byte)Random(100);
		}
	}
	else if(me->mind==1)	// random wandering
	{
		if(goodguy && RangeToTarget(me,goodguy)<(128*FIXAMT) && Random(8)==0)
		{
			// get him!
			me->seq=ANIM_ATTACK;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
			me->action=ACTION_BUSY;
			me->dx=0;
			me->dy=0;
			return;
		}
		FaceGoodguy2(me,goodguy);

		if(!(me->mind1--))	// time to get a new direction
		{
			me->mind=0;	// get back on track
		}

		me->dx=0;
		me->dy=0;
		if(me->seq!=ANIM_IDLE)
		{
			me->seq=ANIM_IDLE;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=64;
		}
	}
}

void AI_BigBat(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	Guy *g;

	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_SCARIEROUCH,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_SCARIERDIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_DIE)
		{
			BlowSmoke(me->x,me->y,me->z,FIXAMT*2);
		}
		if(me->seq==ANIM_ATTACK)
		{
			if(me->frm==2 && me->frmTimer<32)
			{
				g=AddBaby(me->x,me->y,me->z,MONS_BAT,me);
				if(g)
				{
					if(!g->CanWalk(g->x,g->y,map,world))
						g->type=MONS_NONE;
					g->z=me->z;
					g->facing=me->facing;
					g->action=ACTION_BUSY;
					g->seq=ANIM_A1;	// diving attack move
					g->frm=0;
					g->frmTimer=0;
					g->frmAdvance=128;
					g->dx=Cosine(g->facing*32)*12;
					g->dy=Sine(g->facing*32)*12;
				}
				me->reload=30;
			}
		}
		return;	// can't do nothin' right now
	}

	if(me->mind==0)		// when mind=0, singlemindedly fly towards the player
	{
		if(goodguy)
		{
			if(me->mind1>0)
				me->mind1--;
			else
			{
				FaceGoodguy2(me,goodguy);
				me->mind1=8;
				if(Random(16)==0)
				{
					me->mind=1;		// occasionally wander
					me->mind1=60;
				}
			}

			me->dx=Cosine(me->facing*32)*2;
			me->dy=Sine(me->facing*32)*2;
			if(me->seq!=ANIM_MOVE)
			{
				me->seq=ANIM_MOVE;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=128;
			}
		}
		else
		{
			me->mind=1;	// if there's no goodguy, get random
			me->mind1=60;
		}
	}
	else if(me->mind==1)	// random wandering
	{
		if(me->mind1)
			me->mind1--;
		else
		{
			if((goodguy) && Random(16)==0)
				me->mind=0;	// get back on track
			else
				me->facing=(me->facing-1+Random(3))&7;
			me->mind1=8;
		}

		me->dx=Cosine(me->facing*32)*2;
		me->dy=Sine(me->facing*32)*2;
		if(me->seq!=ANIM_MOVE)
		{
			me->seq=ANIM_MOVE;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
		}
	}
	if(me->reload==0 && Random(32)==0 && goodguy && RangeToTarget(me,goodguy)<512*FIXAMT)
	{
		me->action=ACTION_BUSY;
		me->seq=ANIM_ATTACK;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=128;
		MakeSound(SND_SCARIERSPIT,me->x,me->y,SND_CUTOFF,1200);
	}
}

void AI_HugeBat(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y;
	Guy *g;

	if(me->reload)
		me->reload--;

	if(me->mind1)
		me->mind1--;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_UBEROUCH,me->x,me->y,SND_CUTOFF,1800);
		else
			MakeSound(SND_UBERDIE,me->x,me->y,SND_CUTOFF,2000);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm==2 && me->reload==0)
		{
			x=me->x-FIXAMT*32;
			y=me->y+FIXAMT*20;
			g=AddBaby(x,y,FIXAMT*40,MONS_BAT,me);
			if(g)
			{
				g->facing=3;
				g->action=ACTION_BUSY;
				g->seq=ANIM_A1;	// diving attack move
				g->frm=0;
				g->frmTimer=0;
				g->frmAdvance=128;
				g->dx=Cosine(g->facing*32)*12;
				g->dy=Sine(g->facing*32)*12;
			}
			x=me->x;
			g=AddBaby(x,y,FIXAMT*40,MONS_BAT,me);
			if(g)
			{
				g->facing=2;
				g->action=ACTION_BUSY;
				g->seq=ANIM_A1;	// diving attack move
				g->frm=0;
				g->frmTimer=0;
				g->frmAdvance=128;
				g->dx=Cosine(g->facing*32)*12;
				g->dy=Sine(g->facing*32)*12;
			}
			x=me->x+FIXAMT*32;
			g=AddBaby(x,y,FIXAMT*40,MONS_BAT,me);
			if(g)
			{
				g->facing=1;
				g->action=ACTION_BUSY;
				g->seq=ANIM_A1;	// diving attack move
				g->frm=0;
				g->frmTimer=0;
				g->frmAdvance=128;
				g->dx=Cosine(g->facing*32)*12;
				g->dy=Sine(g->facing*32)*12;
			}
			me->reload=60;
		}
		if(me->seq==ANIM_DIE)
		{
			if(me->frm>=8)
			{
				x=me->x>>FIXSHIFT;
				y=me->y>>FIXSHIFT;
				BlowUpGuy(x+me->rectx,y+me->recty,x+me->rectx2,y+me->recty2,me->z,1);
				BlowSmoke((x+me->rectx+Random(me->rectx2-me->rectx))<<FIXSHIFT,
						  (y+me->recty+Random(me->recty2-me->recty))<<FIXSHIFT,
						  me->z,FIXAMT);
			}
		}

		return;	// can't do nothin' right now
	}

	if(goodguy && me->reload==0 && RangeToTarget(me,goodguy)<512*FIXAMT)
	{
		// spit some bats
		me->seq=ANIM_ATTACK;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=128;
		me->action=ACTION_BUSY;
		me->dx=0;
		me->dy=0;
		MakeSound(SND_UBERSPIT,me->x,me->y,SND_CUTOFF,1200);
	}
}

void AI_Werewolf(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y,i;
	Guy *g;

	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_WOLFOUCH,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_WOLFDIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->mind3)
		me->mind3--;

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm==5)
		{
			me->dx=Cosine(me->facing*32)*10;
			me->dy=Sine(me->facing*32)*10;
		}
		if(me->seq==ANIM_ATTACK && me->frm>=6 && me->frm<=10)
		{
			Dampen(&me->dx,FIXAMT);
			Dampen(&me->dy,FIXAMT);
			if(me->reload==0 && me->AttackCheck(40,me->x>>FIXSHIFT,me->y>>FIXSHIFT,goodguy))
			{
				goodguy->GetShot(Cosine(me->facing*32)*8,Sine(me->facing*32)*8,8,map,world);
				me->reload=10;
			}
			if(me->frm==10)
			{
				me->dx=0;
				me->dy=0;
			}
		}
		if(me->seq==ANIM_A2 && me->frm>=5 && me->frm<=15)
		{
			x=me->x+Cosine(me->facing*32)*32;
			y=me->y+Sine(me->facing*32)*32;
			if(me->AttackCheck(16,x>>FIXSHIFT,y>>FIXSHIFT,goodguy))
			{
				goodguy->GetShot(Cosine(me->facing*32)*4,Sine(me->facing*32)*4,2,map,world);
			}
		}
		if(me->seq==ANIM_A2)
		{
			// constantly turn toward goodguy
			FaceGoodguy2(me,goodguy);
			if(me->frm<15 && me->frm>4)
			{
				x=Cosine(me->facing*32)*12;
				y=Sine(me->facing*32)*12;
			}
			else
			{
				x=0;
				y=0;
			}
			me->dx-=x;
			Dampen(&me->dx,FIXAMT+FIXAMT/2);
			me->dx+=x;
			me->dy-=y;
			Dampen(&me->dy,FIXAMT+FIXAMT/2);
			me->dy+=y;
		}
		if(me->seq==ANIM_A3 && me->frm==6 && me->reload==0)
		{
			// howl effect
			if(me->aiType==MONS_WOLF2)
			{
				me->reload=10;
				me->mind3=30*5;
			}
			else
			{
				for(i=0;i<4;i++)
				{
					x=me->x+Cosine(i*64)*64;
					y=me->y+Sine(i*64)*64;
					g=AddBaby(x,y,0,MONS_WOLF,me);
					MakeSound(SND_WOLFPOUNCE,me->x,me->y,SND_CUTOFF,1200);
					if(g && (!g->CanWalk(g->x,g->y,map,world)))
						RemoveGuy(g);
					else if(g)
					{
						g->seq=ANIM_A3;	// arising from the ground
						g->frm=0;
						g->frmTimer=0;
						g->frmAdvance=128;
						g->action=ACTION_BUSY;
					}
				}
			}
		}
		if(me->seq==ANIM_A4 && me->frm>6 && me->reload==0)
		{
			// howl effect
			me->reload=2;
			FireBullet(me->x,me->y,me->facing*32-8+Random(16),BLT_FREEZE2,me->friendly);
		}
		if(me->seq==ANIM_DIE)
			me->frmAdvance=190;
		return;	// can't do nothin' right now
	}

	if(RangeToTarget(me,goodguy)<(128*FIXAMT) && Random(12)==0)
	{
		// get him!
		MakeSound(SND_WOLFPOUNCE,me->x,me->y,SND_CUTOFF,1200);
		me->seq=ANIM_ATTACK;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=160;
		me->action=ACTION_BUSY;
		me->reload=0;
		me->dx=0;
		me->dy=0;
		return;
	}
	if(me->aiType==MONS_ARCTICWOLF && RangeToTarget(me,goodguy)<(128*FIXAMT) && Random(24)==0)
	{
		// get him!
		MakeSound(SND_WOLFPOUNCE,me->x,me->y,SND_CUTOFF,1200);
		me->seq=ANIM_A4;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=160;
		me->action=ACTION_BUSY;
		me->reload=0;
		me->dx=0;
		me->dy=0;
		return;
	}
	if(me->aiType==MONS_WOLF2 && RangeToTarget(me,goodguy)<(256*FIXAMT) && Random(32)==0)
	{
		// charge!!!
		MakeSound(SND_WOLFCHARGE,me->x,me->y,SND_CUTOFF,1200);
		me->seq=ANIM_A2;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=128;
		me->action=ACTION_BUSY;
		me->dx=0;
		me->dy=0;
		me->reload=0;
		FaceGoodguy(me,goodguy);
		return;
	}
	if(me->aiType==MONS_ARCTICWOLF && RangeToTarget(me,goodguy)<(256*FIXAMT) && Random(32)==0)
	{
		// charge!!!
		MakeSound(SND_WOLFCHARGE,me->x,me->y,SND_CUTOFF,1200);
		me->seq=ANIM_A4;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=64;
		me->action=ACTION_BUSY;
		me->dx=0;
		me->dy=0;
		me->reload=0;
		FaceGoodguy(me,goodguy);
		return;
	}
	if((me->aiType==MONS_WOLF2||me->aiType==MONS_DIREWOLF) && me->mind3==0 && me->mind2==0 && Random(128)==0)
	{
		// howl
		MakeSound(SND_WOLFHOWL,me->x,me->y,SND_CUTOFF,1200);
		me->seq=ANIM_A3;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=128;
		me->action=ACTION_BUSY;
		me->dx=0;
		me->dy=0;
		me->reload=0;
		return;
	}
	
	if(me->seq==ANIM_MOVE)
		me->frmAdvance=256;

	if(me->mind==0)		// when mind=0, singlemindedly lumber towards Bouapha
	{
		FaceGoodguy(me,goodguy);

		x=4+2*(me->aiType==MONS_WOLF);
		me->dx=Cosine(me->facing*32)*x;
		me->dy=Sine(me->facing*32)*x;
		if(me->seq!=ANIM_MOVE)
		{
			me->seq=ANIM_MOVE;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128-32*(me->aiType==MONS_WOLF2);
		}
		if(Random(512)==0)
		{
			me->mind=1;		// occasionally wander
			me->mind1=1;
		}
	}
	else if(me->mind==1)	// random wandering
	{
		if(!(me->mind1--))	// time to get a new direction
		{
			if((goodguy) && Random(10)==0)
				me->mind=0;	// get back on track
			else
				me->facing=(byte)Random(8);
			me->mind1=(byte)Random(30)+1;
		}

		x=4+2*(me->aiType==MONS_WOLF)-1*(me->aiType==MONS_ARCTICWOLF)-1*(me->aiType==MONS_DIREWOLF);
		me->dx=Cosine(me->facing*32)*x;
		me->dy=Sine(me->facing*32)*x;
		if(me->seq!=ANIM_MOVE)
		{
			me->seq=ANIM_MOVE;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128-32*(me->aiType==MONS_WOLF2);
		}
	}
}

byte TargetInSight(Guy *me,Map *map,Guy *goodguy)
{
	if(!goodguy || goodguy->type==MONS_NOBODY)
		return 0;

	switch(me->mind2)
	{
		case 0:
			if(goodguy->mapx>me->mapx && goodguy->mapx-me->mapx<12)
			{
				// on the right side anyway, and within the range
				if(abs(goodguy->mapy-me->mapy)<2+(goodguy->mapx-me->mapx)/(12/6))
				{
					// within the cone
					return map->FindGuy(me->mapx,me->mapy,8,goodguy);
				}
			}
			return 0;
			break;
		case 2:
			if(goodguy->mapy>me->mapy && goodguy->mapy-me->mapy<12)
			{
				// on the right side anyway, and within the range
				if(abs(goodguy->mapx-me->mapx)<2+(goodguy->mapy-me->mapy)/(12/6))
				{
					// within the cone
					return map->FindGuy(me->mapx,me->mapy,8,goodguy);
				}
			}
			return 0;
			break;
		case 4:
			if(goodguy->mapx<me->mapx && -goodguy->mapx+me->mapx<12)
			{
				// on the right side anyway, and within the range
				if(abs(goodguy->mapy-me->mapy)<2+(-goodguy->mapx+me->mapx)/(12/6))
				{
					// within the cone
					return map->FindGuy(me->mapx,me->mapy,8,goodguy);
				}
			}
			return 0;
			break;
		case 6:
			if(goodguy->mapy<me->mapy && -goodguy->mapy+me->mapy<12)
			{
				// on the right side anyway, and within the range
				if(abs(goodguy->mapx-me->mapx)<2+(-goodguy->mapy+me->mapy)/(12/6))
				{
					// within the cone
					return map->FindGuy(me->mapx,me->mapy,8,goodguy);
				}
			}
			return 0;
			break;
	}
	return 0;
}

void AI_Patrol(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y;
	byte a;

	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_ZOMBIEOUCH,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_ZOMBIEDIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm==5 && me->reload==0 && goodguy)
		{
			// crush skull
			x=me->x+Cosine(me->facing*32)*20;
			y=me->y+Sine(me->facing*32)*20;
			if(me->AttackCheck(24,x>>FIXSHIFT,y>>FIXSHIFT,goodguy))
			{
				goodguy->GetShot(Cosine(me->facing*32)*10,Sine(me->facing*32)*10,12,map,world);
			}
			me->reload=5;
		}
		if(me->seq==ANIM_ATTACK && me->reload==0 && (me->aiType==MONS_DPATROLLR || me->aiType==MONS_DPATROLUD))
		{
			x=me->x+Cosine(me->facing*32)*20;
			y=me->y+Sine(me->facing*32)*20;
			a=me->facing*32-16+Random(32);
			FireExactBullet(x,y,FIXAMT*20,Cosine(a)*6,Sine(a)*6,FIXAMT*5,0,60,0,BLT_BOMB,me->friendly);
			me->reload=2;
		}
		return;	// can't do nothin' right now
	}

	if(abs(me->mind2-me->facing)==4)	// max distance
	{
		if(Random(2))
			me->facing++;
		else
			me->facing--;
		me->facing&=7;
	}
	else if(me->mind2==me->facing)
	{
	}
	else	// rotate towards true facing
	{
		int dir,diff;

		if(me->mind2>me->facing)
		{
			diff=me->mind2-me->facing;
			if(diff>4)
			{
				dir=-1;
				diff=8-diff;
			}
			else
				dir=1;
		}
		else
		{
			diff=me->facing-me->mind2;
			if(diff>4)
			{
				dir=1;
				diff=8-diff;
			}
			else
				dir=-1;
		}
		me->facing=(me->facing+dir)&7;
	}
	if(me->mind==0)	// newborn
	{
		me->facing=Random(8);
		if(me->aiType==MONS_PATROLLR || me->aiType==MONS_DPATROLLR)
			me->mind2=0;	// desired facing/also direction of movement
		else
			me->mind2=2;	// desired facing/also direction of movement

		me->mind=1;
	}
	else if(me->mind==1)	// pacing
	{
		if(me->mind1)	// bonked a wall
		{
			me->mind1=0;
			if(me->mind2<4)
				me->mind2+=4;
			else
				me->mind2-=4;
		}
		if(me->mind2!=me->facing)
		{
			me->dx=0;
			me->dy=0;
		}
		else
		{
			me->dx=Cosine(me->mind2*32);
			me->dy=Sine(me->mind2*32);
		}

		if(me->seq!=ANIM_MOVE)
		{
			me->seq=ANIM_MOVE;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=64;
		}

		if(me->mind2==me->facing && TargetInSight(me,map,goodguy))
		{
			MakeSound(SND_MUMBLECRUSH,me->x,me->y,SND_CUTOFF,1000);
			me->seq=ANIM_ATTACK;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=256;
			me->action=ACTION_BUSY;
			me->dx=0;
			me->dy=0;
			me->reload=0;
			if(goodguy->aiType==MONS_BOUAPHA)
				player.spotted=30*8;
			me->mind=2;	// start clapping!
		}
	}
	else	// staring at goodguy
	{
		if(TargetInSight(me,map,goodguy))
		{
			if(goodguy->aiType==MONS_BOUAPHA)
				player.spotted=30*8;
			MakeSound(SND_CLAP,me->x,me->y,SND_CUTOFF,1000);
			me->seq=ANIM_ATTACK;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=256;
			me->action=ACTION_BUSY;
			me->dx=0;
			me->dy=0;
			me->reload=0;
		}
		else
			me->mind=1;
	}
}

void AI_Weatherman(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y;

	if(me->reload)
		me->reload--;

	WeathermanWeather(me->x,me->y);
	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_PYGMYOUCH,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_PYGMYDIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(RangeToTarget(me,goodguy)<100*FIXAMT && Random(2)==0 && me->hp>0)
	{
		goodguy->GetShot(0,0,2,map,world);
		LightningBolt(
				me->x-FIXAMT*32+Random(FIXAMT*64),
				me->y-FIXAMT*82+Random(FIXAMT*64),
				goodguy->x-FIXAMT*32+Random(FIXAMT*64),
				goodguy->y-FIXAMT*52+Random(FIXAMT*64));
		MakeSound(SND_LIGHTNING,me->x,me->y,SND_CUTOFF,400);
		map->BrightTorch((me->x/TILE_WIDTH)>>FIXSHIFT,
						 (me->y/TILE_HEIGHT)>>FIXSHIFT,8,4);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm>=4 && me->frm<11 && goodguy)
		{
			x=me->x+Cosine(me->facing*32)*8;
			y=me->y+Sine(me->facing*32)*8;
			// throw ice and rain
			MakeSound(SND_CRAZYSTAB,me->x,me->y,SND_CUTOFF,1200);
			FireBulletZ(x,y,FIXAMT*12,(me->facing*32-16+(byte)Random(33))&255,BLT_SHARK,me->friendly);
			if(Random(2)==0)
				FireBulletZ(x,y,FIXAMT*12,(me->facing*32-16+(byte)Random(33))&255,BLT_FREEZE2,me->friendly);
		}
		if(me->seq==ANIM_A1 && goodguy->aiType==MONS_BOUAPHA)	// unga bunga!
		{
			me->mind1=255;

			if(goodguy->x<me->x)
			{
				goodguy->dx+=(me->mind1*FIXAMT/16);
				goodguy->dy+=(me->mind1*FIXAMT/128);
			}
			else
			{
				goodguy->dx-=(me->mind1*FIXAMT/16);
				goodguy->dy-=(me->mind1*FIXAMT/128);
			}
			if(goodguy->y<me->y)
			{
				goodguy->dy+=(me->mind1*FIXAMT/16);
				goodguy->dx-=(me->mind1*FIXAMT/128);
			}
			else
			{
				goodguy->dy-=(me->mind1*FIXAMT/16);
				goodguy->dx+=(me->mind1*FIXAMT/128);
			}
			Clamp(&goodguy->dx,FIXAMT*6);
			Clamp(&goodguy->dy,FIXAMT*6);
		}
		return;	// can't do nothin' right now
	}

	if(me->mind==0)	// not currently aware of goodguy
	{
		me->frmAdvance=32;	// overcome the auto-128 frmAdvance of guy.cpp

		if(me->seq==ANIM_MOVE)
		{
			me->seq=ANIM_IDLE;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=32;
		}
		if(me->mind1<101)
			me->mind1++;

		if(me->mind1>100 && Random(200)==0)
		{
			me->facing=(me->facing+(-1+Random(3)))&7;	//change facing
			me->mind1=0;
		}
		if(RangeToTarget(me,goodguy)<320*FIXAMT || me->ouch>0)
		{
			// if the goodguy is near, or he shot me
			me->mind=1;	// start hunting
			me->mind1=255;	// for a long time
			FaceGoodguy(me,goodguy);

			// do angry animation
			me->action=ACTION_BUSY;
			me->seq=ANIM_A2;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=255;	// very fast
			MakeSound(SND_PYGMYANGRY,me->x,me->y,SND_CUTOFF,1200);
		}
		me->dx=0;
		me->dy=0;
	}
	else
	{
		FaceGoodguy2(me,goodguy);

		if(RangeToTarget(me,goodguy)<(512*FIXAMT) && Random(64)==0 && me->reload==0)
		{
			// fire ice & rain
			me->seq=ANIM_ATTACK;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=255;
			me->action=ACTION_BUSY;
			me->dx=0;
			me->dy=0;
			me->reload=1;
			return;
		}

		if(RangeToTarget(me,goodguy)<(512*FIXAMT) && Random(64)==0 && me->reload==0)
		{
			// suck him in
			MakeSound(SND_UNGADANCE,me->x,me->y,SND_CUTOFF,1200);
			MakeSound(SND_PYGMYSPIN,me->x,me->y,SND_CUTOFF,1200);
			me->seq=ANIM_A1;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=96;
			me->action=ACTION_BUSY;
			me->dx=0;
			me->dy=0;
			me->reload=1;
			return;
		}

		if(me->ouch && me->mind1<240)
			me->mind1+=10;
		if(me->mind1)
			me->mind1--;
		else
		{
			if(RangeToTarget(me,goodguy)>256*FIXAMT)
				me->mind=0;	// get bored again
			else
				me->mind1=20;	// stay on trail a little longer
			return;
		}
		me->dx=Cosine(me->facing*32)*3;
		me->dy=Sine(me->facing*32)*3;
		if(me->seq!=ANIM_MOVE)
		{
			me->seq=ANIM_MOVE;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
		}
	}
}

void AI_HunterEgg(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	if(me->ouch==4)
	{
		MakeSound(SND_EGGOUCH,me->x,me->y,SND_CUTOFF,1200);

		if(me->seq!=ANIM_ATTACK)
		{
			MakeSound(SND_EGGOPEN,me->x,me->y,SND_CUTOFF,1200);
			me->seq=ANIM_ATTACK;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=127;
			me->action=ACTION_BUSY;
		}
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm==6 && me->reload==0)
		{
			AddBaby(me->x+FIXAMT*20,me->y+FIXAMT*10,0,MONS_ALIEN,me);
			// then die
			MakeSound(SND_EGGDIE,me->x,me->y,SND_CUTOFF,1200);
			me->hp=1;
			me->GetShot(0,0,1,map,world);
			me->reload=10;
		}

		if(me->seq==ANIM_DIE)
		{
			FireBullet(me->x,me->y,(byte)Random(256),BLT_ACID,me->friendly);
			ExplodeParticles2(PART_SLIME,me->x,me->y,FIXAMT*20,4,10);
		}
		return;	// can't do nothin' right now
	}

	if(goodguy && RangeToTarget(me,goodguy)<180*FIXAMT)
	{
		// goodguy got close, pop out
		MakeSound(SND_EGGOPEN,me->x,me->y,SND_CUTOFF,1200);
		me->seq=ANIM_ATTACK;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=127;
		me->action=ACTION_BUSY;
	}
}

void AI_PunkBunny(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y;
	Guy *bouapha;

	bouapha=GetGuyOfAIType(MONS_BOUAPHA);
	if(!bouapha)
		return;

	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		MakeSound(SND_BUNNYDIE,me->x,me->y,SND_CUTOFF,800);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && (me->frm==3 || me->frm==6 || me->frm==9) && me->reload==0)
		{
			x=me->x+Cosine(me->facing*32)*20;
			y=me->y+Sine(me->facing*32)*20;

			FireBullet(x,y,(me->facing*32-8+Random(17))&255,BLT_ENERGY,me->friendly);
			MakeSound(SND_BULLETFIRE,x,y,SND_CUTOFF,100);
			me->reload=3;
		}
		if(me->seq==ANIM_A1 && me->frm==2 && me->reload==0)
		{
			x=me->x+Cosine(me->facing*32)*10;
			y=me->y+Sine(me->facing*32)*10;

			FireBullet(x,y,me->facing*32,BLT_BOMB,me->friendly);
			me->reload=3;
		}
		if(me->seq==ANIM_DIE)
		{
			for(x=0;x<8;x++)
				SpecialSnow(me->x+((-16+Random(33))<<FIXSHIFT),
							me->y+((-16+Random(33))<<FIXSHIFT));
		}
		return;
	}

	if(me->mind==0)	// not activated yet
	{
		if(RangeToTarget(me,bouapha)<72*FIXAMT)
		{
			me->mind=1;
			MakeNormalSound(SND_FOLLOWHI);
		}
	}
	else if(me->mind==1)	// following Bouapha
	{
		Dampen(&me->dx,FIXAMT/8);
		Dampen(&me->dy,FIXAMT/8);

		if(me->x<bouapha->x)
			me->dx+=FIXAMT;
		else
			me->dx-=FIXAMT;
		if(me->y<bouapha->y)
			me->dy+=FIXAMT;
		else
			me->dy-=FIXAMT;

		me->dx+=-FIXAMT/2+Random(FIXAMT);
		me->dy+=-FIXAMT/2+Random(FIXAMT);

		if(RangeToTarget(me,bouapha)<48*FIXAMT)	// got too close
		{
			me->mind=2;
			me->facing=Random(8);
			me->dx=Cosine(me->facing*32)*2;
			me->dy=Sine(me->facing*32)*2;
			me->mind1=Random(20)+10;
		}
	}
	else if(me->mind==2) // not following, got too close
	{
		me->mind1--;
		if(!me->mind1)
			me->mind=1;
	}
	else	// hunting for fresh meat
	{
		if(goodguy==NULL || goodguy->type==MONS_NOBODY || RangeToTarget(me,bouapha)>320*FIXAMT)	// got too far from papa!!
		{
			me->mind=1;
			return;
		}
		else
		{
			Dampen(&me->dx,FIXAMT/8);
			Dampen(&me->dy,FIXAMT/8);

			if(me->x<goodguy->x)
				me->dx+=FIXAMT;
			else
				me->dx-=FIXAMT;
			if(me->y<goodguy->y)
				me->dy+=FIXAMT;
			else
				me->dy-=FIXAMT;

			me->dx+=-FIXAMT/2+Random(FIXAMT);
			me->dy+=-FIXAMT/2+Random(FIXAMT);

			if(RangeToTarget(me,goodguy)<280*FIXAMT && Random(10)==0)
			{
				me->seq=ANIM_ATTACK;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=256;
				me->dx=0;
				me->dy=0;
				me->action=ACTION_BUSY;
				FaceGoodguy(me,goodguy);
				return;
			}
			if(Random(50)==0)
			{
				me->seq=ANIM_A1;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=128;
				me->dx=0;
				me->dy=0;
				me->action=ACTION_BUSY;
				FaceGoodguy(me,goodguy);
				return;
			}
		}
	}

	// if following Bouapha, and an enemy comes within sight, and you aren't already far from Bouapha
	if((me->mind==1 || me->mind==2) && RangeToTarget(me,goodguy)<320*FIXAMT &&
		RangeToTarget(me,bouapha)<100*FIXAMT)
	{
		me->mind=3;	// enter assault mode
	}

	Clamp(&me->dx,FIXAMT*4);
	Clamp(&me->dy,FIXAMT*4);

	if(me->dx>FIXAMT)
	{
		if(me->dy>FIXAMT)
			me->facing=1;
		else if(me->dy<-FIXAMT)
			me->facing=7;
		else
			me->facing=0;
	}
	else if(me->dx<-FIXAMT)
	{
		if(me->dy>FIXAMT)
			me->facing=3;
		else if(me->dy<-FIXAMT)
			me->facing=5;
		else
			me->facing=4;
	}
	else
	{
		if(me->dy>FIXAMT)
			me->facing=2;
		else if(me->dy<-FIXAMT)
			me->facing=6;
	}

	if(me->mind!=0)
	{
		if(me->seq!=ANIM_MOVE)
		{
			me->seq=ANIM_MOVE;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
		}
	}
}

void AI_Skeleton(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y,i;

	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_SKELOUCH,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_SKELDIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm==12 && me->reload==0 && goodguy)
		{
			ShakeScreen(120);
			MakeSound(SND_BOMBBOOM,me->x,me->y,SND_CUTOFF,1200);
			FireBullet(me->x,me->y-FIXAMT,0,BLT_SHOCKWAVE,me->friendly);
			for(i=0;i<10;i++)
			{
				x=me->x+((256-Random(513))<<FIXSHIFT);
				y=me->y+((256-Random(513))<<FIXSHIFT);
				FireExactBullet(x,y,FIXAMT*80+(Random(40)<<FIXSHIFT),
								0,0,0,0,30,2,BLT_ROCK,me->friendly);
			}
			me->reload=5;
		}
		if(me->seq==ANIM_ATTACK)
			me->dz+=FIXAMT/4;
		return;	// can't do nothin' right now
	}

	// Randomy decide to point at Bouapha to unnerve him
	// (but only if in pursuit mode, because otherwise you'd point the wrong way)
	if((!Random(100)) && me->mind==0)
	{
		me->seq=ANIM_A2;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=64;
		me->action=ACTION_BUSY;
		me->dx=0;
		me->dy=0;
		return;
	}

	if(me->mind==0)		// when mind=0, singlemindedly lumber towards Bouapha
	{
		if(goodguy)
		{
			if(RangeToTarget(me,goodguy)<(512*FIXAMT) && Random(32)==0)
			{
				// get him!
				MakeSound(SND_SKELKICK,me->x,me->y,SND_CUTOFF,1200);
				me->seq=ANIM_ATTACK;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=256;
				me->action=ACTION_BUSY;
				me->dx=0;
				me->dy=0;
				me->dz=FIXAMT*10;
				me->reload=0;
				return;
			}
			FaceGoodguy2(me,goodguy);

			me->dx=Cosine(me->facing*32)*1;
			me->dy=Sine(me->facing*32)*1;
			if(me->seq!=ANIM_MOVE)
			{
				me->seq=ANIM_MOVE;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=32;
			}
			if(Random(64)==0)
			{
				me->mind=1;		// occasionally wander
				me->mind1=1;
			}
		}
		else
		{
			me->mind=1;	// if there's no goodguy, get random
			me->mind1=1;
		}
	}
	else if(me->mind==1)	// random wandering
	{
		if(!(me->mind1--))	// time to get a new direction
		{
			if((goodguy) && Random(3)==0)
				me->mind=0;	// get back on track
			else
				me->facing=(me->facing-1+Random(3))&7;
			me->mind1=Random(40)+1;
		}

		me->dx=Cosine(me->facing*32)*1;
		me->dy=Sine(me->facing*32)*1;
		if(me->seq!=ANIM_MOVE)
		{
			me->seq=ANIM_MOVE;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=32;
		}
	}
}

void AI_Microzoid(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y;

	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp==0)
			MakeSound(SND_MINIZOIDDIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm==6 && me->reload==0 && goodguy)
		{
			// spit stuff
			x=me->x+Cosine(me->facing*32)*8;
			y=me->y+Sine(me->facing*32)*8;
			FireBullet(x,y,me->facing*32,BLT_ENERGY,me->friendly);
			me->reload=15;
			me->mind=0;
		}
		return;	// can't do nothin' right now
	}

	if(goodguy)
	{
		if(RangeToTarget(me,goodguy)<(512*FIXAMT) && Random(16)==0 && me->reload==0)
		{
			MakeSound(SND_MINIZOIDSPIT,me->x,me->y,SND_CUTOFF,1200);
			FaceGoodguy(me,goodguy);
			// spit on him!
			me->seq=ANIM_ATTACK;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=256;
			me->action=ACTION_BUSY;
			me->dx=0;
			me->dy=0;
			me->reload=0;
			return;
		}
	}

	if(me->mind)
		me->mind--;
	if(!me->mind)	// time to get a new direction
	{
		me->facing=(byte)Random(8);
		me->mind=Random(60)+1;
	}

	me->dx=Cosine(me->facing*32)*4;
	me->dy=Sine(me->facing*32)*4;
	if(me->seq!=ANIM_MOVE)
	{
		me->seq=ANIM_MOVE;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=256;
	}
}

void AI_HotShroom(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int i;

	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		/*
		if(me->hp>0)
			MakeSound(SND_SKELOUCH,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_SKELDIE,me->x,me->y,SND_CUTOFF,1200);
			*/
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && (me->frm==5 || me->frm==6) && me->reload==0 && goodguy)
		{
			for(i=0;i<8;i++)
			{
				if (me->aiType==MONS_DEATHCAP)
				FireBullet(me->x,me->y,i,BLT_ROCKET,me->friendly);
				else
				FireBullet(me->x,me->y,i,BLT_FLAME,me->friendly);
			}

			me->reload=2;
		}
		return;	// can't do nothin' right now
	}

	if(RangeToTarget(me,goodguy)<(160*FIXAMT) && Random(16)==0)
	{
		// get him!
		MakeSound(SND_SKELKICK,me->x,me->y,SND_CUTOFF,1200);
		me->seq=ANIM_ATTACK;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=128;
		me->action=ACTION_BUSY;
		me->dx=0;
		me->dy=0;
		me->reload=0;
		return;
	}
	if(me->mind==0)		// when mind=0, singlemindedly lumber towards Bouapha
	{
		if(goodguy)
		{
			FaceGoodguy(me,goodguy);

			me->dx=Cosine(me->facing*32)*2;
			me->dy=Sine(me->facing*32)*2;
			if(me->seq!=ANIM_MOVE)
			{
				me->seq=ANIM_MOVE;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=128;
			}
			if(Random(64)==0)
			{
				me->mind=1;		// occasionally wander
				me->mind1=1;
			}
		}
		else
		{
			me->mind=1;	// if there's no goodguy, get random
			me->mind1=1;
		}
	}
	else if(me->mind==1)	// random wandering
	{
		if(!(me->mind1--))	// time to get a new direction
		{
			if((goodguy) && Random(3)==0)
				me->mind=0;	// get back on track
			else
				me->facing=(byte)Random(8);
			me->mind1=Random(40)+1;
		}

		me->dx=Cosine(me->facing*32)*2;
		me->dy=Sine(me->facing*32)*2;
		if(me->seq!=ANIM_MOVE)
		{
			me->seq=ANIM_MOVE;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
		}
	}
}

void AI_MiniWacko(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y;

	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_MINIWACKOOUCH,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_MINIWACKODIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frmTimer<196 && goodguy && (me->frm==5 || me->frm==11 || me->frm==17))
		{
			MakeSound(SND_CRAZYSTAB,me->x,me->y,SND_CUTOFF,1200);
			x=me->x+Cosine(me->facing*32)*8;
			y=me->y+Sine(me->facing*32)*8;
			if(me->AttackCheck(16,x>>FIXSHIFT,y>>FIXSHIFT,goodguy))
				goodguy->GetShot(Cosine(me->facing*32)*4,Sine(me->facing*32)*4,4,map,world);
			me->reload=10;
		}

		if(me->seq==ANIM_DIE && me->frm==5)
			me->frmAdvance=128;
		return;	// can't do nothin' right now
	}

	if(me->mind==0)
	{
		if(goodguy)
		{
			if(RangeToTarget(me,goodguy)<(48*FIXAMT) && Random(8)==0)
			{
				// get him!
				me->seq=ANIM_ATTACK;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=256;
				me->action=ACTION_BUSY;
				me->dx=0;
				me->dy=0;
				me->reload=0;
				return;
			}
			if(RangeToTarget(me,goodguy)<64*FIXAMT)
				FaceGoodguy(me,goodguy);
			else
				FaceGoodguy2(me,goodguy);

			me->dx=Cosine(me->facing*32)*5;
			me->dy=Sine(me->facing*32)*5;
			if(me->seq!=ANIM_MOVE)
			{
				me->seq=ANIM_MOVE;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=128;
			}
		}
		if(me->mind1 && RangeToTarget(me,goodguy)>64*FIXAMT)
		{
			me->mind1=0;
			me->mind=1;
			me->mind2=Random(30)+10;
		}
		else
			me->mind1=0;
	}
	else
	{
		if(RangeToTarget(me,goodguy)<64*FIXAMT)
			me->mind2=1;
		me->mind2--;
		if(!me->mind2)
			me->mind=0;

		me->facing=(me->facing+1-Random(3))&7;
	}

	me->dx=Cosine(me->facing*32)*5;
	me->dy=Sine(me->facing*32)*5;
	if(me->seq!=ANIM_MOVE)
	{
		me->seq=ANIM_MOVE;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=128;
	}
}

void AI_JackFrost(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y;

	if(me->reload)
		me->reload--;

	if(me->dx==0 && me->dy==0)
	{
		me->dx=FIXAMT*4;
		me->dy=FIXAMT*4;
	}

	JackFrostWeather(me->x,me->y);

	if(me->mind1&1)
		me->dx=-me->dx;
	if(me->mind1&2)
		me->dy=-me->dy;
	me->mind1=0;

	if(me->ouch==4)
	{
		if(me->hp==0)
			MakeSound(SND_JACKDIE,me->x,me->y,SND_CUTOFF,2000);
	}


	if(me->mind2==0)
	{
		me->mind2=5;
		FaceGoodguy(me,goodguy);
		x=me->facing*32+32-Random(65);
		if(x<0)
			x+=256;
		if(x>255)
			x-=256;
		FireBullet(me->x,me->y,x,BLT_FREEZE2,me->friendly);
	}
	else
		me->mind2--;

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm==5 && me->frmTimer<64 && (goodguy))
		{
			if(me->AttackCheck(100,me->x>>FIXSHIFT,me->y>>FIXSHIFT,goodguy))
			{
				FaceGoodguy(me,goodguy);
				goodguy->GetShot(Cosine(me->facing*32)*12,
								Sine(me->facing*32)*12,20,map,world);
			}
		}

		if(me->seq==ANIM_DIE)
		{
			x=me->x>>FIXSHIFT;
			y=me->y>>FIXSHIFT;
			//BlowUpGuy(x+me->rectx,y+me->recty,x+me->rectx2,y+me->recty2,me->z,1);
			BlowSmoke((x+me->rectx+Random(me->rectx2-me->rectx))<<FIXSHIFT,
					  (y+me->recty+Random(me->recty2-me->recty))<<FIXSHIFT,
					  me->z,FIXAMT);
		}
		return;
	}

	if(goodguy)
	{
		if(RangeToTarget(me,goodguy)<100*FIXAMT)
		{
			MakeSound(SND_JACKATTACK,me->x,me->y,SND_CUTOFF,2000);
			me->seq=ANIM_ATTACK;
			me->frm=0;
			me->frmTimer=0;
			me->action=ACTION_BUSY;
			return;
		}
	}
	if(me->seq!=ANIM_IDLE)
	{
		me->seq=ANIM_IDLE;
		me->frm=0;
		me->frmTimer=0;
	}
}

//KID MYSTIC MONSTERS
void AI_Eyeguy(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y;

	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_EYEGUYOUCH,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_EYEGUYDIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm==4 && me->reload==0 && goodguy)
		{
			x=me->x+Cosine(me->facing*32)*20;
			y=me->y+Sine(me->facing*32)*20;
			// fire a shot
			FireBullet(me->x,me->y,me->facing,BLT_ENERGY,me->friendly);
			me->reload=10;
		}
		return;	// can't do nothin' right now
	}

	if(goodguy)
	{
		if(RangeToTarget(me,goodguy)<(256*FIXAMT) && Random(32)==0 && me->reload==0)
		{
			// spit at him
			MakeSound(SND_EYEGUYSHT,me->x,me->y,SND_CUTOFF,1200);
			me->seq=ANIM_ATTACK;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
			me->action=ACTION_BUSY;
			me->dx=0;
			me->dy=0;
			me->reload=0;
			FaceGoodguy(me,goodguy);
			return;
		}
	}

	if(me->mind==0)		// when mind=0, singlemindedly lumber towards Bouapha
	{
		if(goodguy)
		{
			FaceGoodguy(me,goodguy);

			me->dx=Cosine(me->facing*32)*3;
			me->dy=Sine(me->facing*32)*3;
			if(me->seq!=ANIM_MOVE)
			{
				me->seq=ANIM_MOVE;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=128;
			}
			if(Random(64)==0)
			{
				me->mind=1;		// occasionally wander
				me->mind1=1;
			}
		}
		else
		{
			me->mind=1;	// if there's no goodguy, get random
			me->mind1=1;
		}
	}
	else if(me->mind==1)	// random wandering
	{
		if(!(me->mind1--))	// time to get a new direction
		{
			if((goodguy) && Random(3)==0)
				me->mind=0;	// get back on track
			else
				me->facing=(byte)Random(8);
			me->mind1=Random(40)+1;
		}

		me->dx=Cosine(me->facing*32)*3;
		me->dy=Sine(me->facing*32)*3;
		if(me->seq!=ANIM_MOVE)
		{
			me->seq=ANIM_MOVE;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
		}
	}
}

void AI_Peeper(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y;

	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_PEEPOUCH,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_PEEPDIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm==3 && me->reload==0 && goodguy)
		{
			x=me->x+Cosine(me->facing*32)*32;
			y=me->y+Sine(me->facing*32)*32;
			// fire a shot
			FireBullet(me->x,me->y,me->facing*32,BLT_BIGYELLOW,me->friendly);
			MakeSound(SND_PEEPSHOOT,me->x,me->y,SND_CUTOFF,1200);
			me->reload=30*4;
		}
		if(me->seq==ANIM_DIE)
		{
			x=me->x>>FIXSHIFT;
			y=me->y>>FIXSHIFT;
			BlowUpGuy(x+me->rectx,y+me->recty,x+me->rectx2,y+me->recty2,me->z,1);
			BlowSmoke((x+me->rectx+Random(me->rectx2-me->rectx))<<FIXSHIFT,
					  (y+me->recty+Random(me->recty2-me->recty))<<FIXSHIFT,
					  me->z,FIXAMT);
		}
		return;	// can't do nothin' right now
	}

	if(me->mind==0)		// not awake yet
	{
		me->seq=ANIM_A2;	// sleep
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=64;

		if(goodguy && (RangeToTarget(me,goodguy)<256*FIXAMT || me->ouch>0))
		{
			me->mind=1;	// wake up
			me->action=ACTION_BUSY;	// but don't do anything while waking up
			me->mind1=4;
		}
	}
	else	// awake and pissed
	{
		if(me->mind1)
			me->mind1--;
		
		else if(goodguy)
		{
			FaceGoodguy(me,goodguy);
			me->mind1=4;
		}

		me->dx=Cosine(me->facing*32)*3;
		me->dy=Sine(me->facing*32)*3;
		if(me->seq!=ANIM_MOVE)
		{
			me->seq=ANIM_MOVE;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
		}
		if(Random(64)==0 && me->reload==0)
		{
			me->seq=ANIM_ATTACK;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
			me->action=ACTION_BUSY;
			me->dx=0;
			me->dy=0;
		}
		if(Random(256)==0)
		{
			me->seq=ANIM_A1;	// blink
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=256;
			me->action=ACTION_BUSY;
		}
	}
}

void AI_Tower(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y;

	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_TOWEROUCH,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_TOWERDIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK)	// leaping attack
		{
			if(me->frm<4)
			{
				me->dx=0;
				me->dy=0;
				me->dz=0;
			}
			else if(me->frm==4)
			{
				me->dx=Cosine(me->facing*32)*12;
				me->dy=Sine(me->facing*32)*12;
				me->dz=10*FIXAMT;
			}
			else if(me->frm<8)
			{
				// stay the course
				Dampen(&me->dx,FIXAMT/8);
				Dampen(&me->dy,FIXAMT/8);
			}
			else if(me->frm==12 && me->frmTimer<32)
			{
				FireBullet(me->x,me->y-FIXAMT,0,BLT_SHOCKWAVE,me->friendly);
				MakeSound(SND_BOMBBOOM,me->x,me->y,SND_CUTOFF,1500);
				ShakeScreen(10);
			}
			else
			{
				Dampen(&me->dx,FIXAMT/2);
				Dampen(&me->dy,FIXAMT/2);
			}
		}
		if(me->seq==ANIM_DIE)
		{
			if(me->frm>=4)
			{
				x=me->x>>FIXSHIFT;
				y=me->y>>FIXSHIFT;
				BlowUpGuy(x+me->rectx,y+me->recty,x+me->rectx2,y+me->recty2,me->z,1);
				BlowSmoke((x+me->rectx+Random(me->rectx2-me->rectx))<<FIXSHIFT,
						  (y+me->recty+Random(me->recty2-me->recty))<<FIXSHIFT,
						  me->z,FIXAMT);
			}
		}
		if(me->seq==ANIM_A1)
		{
			if(me->frmTimer<32)
			{
				MakeSound(SND_MUSHMISSILE,me->x,me->y,SND_CUTOFF,1500);
				me->facing=(me->facing+1)&7;
				FireBullet(me->x,me->y,me->facing*32,BLT_GRENADE,me->friendly);
			}
		}
		return;	// can't do nothin' right now
	}

	if(me->mind==0)
	{
		if(goodguy)
		{
			if(RangeToTarget(me,goodguy)<300*FIXAMT)
				me->mind=1;
		}
		if(me->ouch)
			me->mind=1;
	}
	else	// chase down Kid Mystic
	{
		if(goodguy)
		{
			if(me->mind1)
				{
					me->facing=(me->facing+3+Random(3))&7;
					me->mind1=0;
				}
				else
				{
					FaceGoodguy2(me,goodguy);
				}

			me->dx=Cosine(me->facing*32)*4;
			me->dy=Sine(me->facing*32)*4;
			if(me->seq!=ANIM_MOVE)
			{
				me->seq=ANIM_MOVE;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=128;
			}
			if(RangeToTarget(me,goodguy)>200*FIXAMT && me->reload==0)
			{
				me->seq=ANIM_A1;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=128;
				me->action=ACTION_BUSY;
				me->dx=0;
				me->dy=0;
				me->reload=120;
			}
			if(Random(32)==0 || RangeToTarget(me,goodguy)<100*FIXAMT)
			{
				//leap!
				me->seq=ANIM_ATTACK;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=128;
				me->action=ACTION_BUSY;
				me->dx=0;
				me->dy=0;
			}
		}
		else
			me->mind=0;
	}
}

void AI_Pinkeye(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp<=0)
		{
			MakeSound(SND_PUMPKINDIE,me->x,me->y,SND_CUTOFF,1200);
			me->reload=255;	// set the resurrection timer
		}
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_DIE)
		{
			if(me->frm==4 && me->reload>0)
			{
				me->frm=3;	// stay still until you're done reloading!
			}
			if(me->frm==8)
			{
				// resurrect!
				me->hp=20;
				me->frm=0;
				me->seq=ANIM_IDLE;
				me->frmTimer=0;
				me->frmAdvance=128;
				me->action=ACTION_IDLE;
			}
		}
		if(me->seq==ANIM_ATTACK)
		{
			if(me->frm==5 && me->frmTimer<32)
			{
				FireBullet(me->x,me->y,(byte)((me->facing*32)-16+Random(32)),BLT_ENERGY,me->friendly);
				//MakeSound(SND_PEEPSHOOT,me->x,me->y,SND_CUTOFF,1500);
			}
		}
		return;	// can't do nothin' right now
	}

	if(goodguy)
		FaceGoodguy(me,goodguy);
	if(me->reload==0)
	{
		me->seq=ANIM_ATTACK;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=128;
		me->action=ACTION_BUSY;
		me->reload=30+Random(90);
	}
}


void AI_Lookeyloo(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_LOOKEYOUCH,me->x,me->y,SND_CUTOFF,1800);
		else
		{
			MakeSound(SND_LOOKEYDIE,me->x,me->y,SND_CUTOFF,2000);
		}
	}
	
	if(goodguy)
	{
		if(FindVictim(me->x>>FIXSHIFT,me->y>>FIXSHIFT,66,0,0,1,map,world,me->friendly))
			{
				PoisonVictim(GetLastGuyHit(),60);
			}
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_DIE)
		{
			FireBullet(me->x-128*FIXAMT+Random(256*FIXAMT),me->y-64*FIXAMT+Random(128*FIXAMT),
				(byte)Random(256),BLT_BOOM,me->friendly);
			ShakeScreen(20);
		}
		if(me->seq==ANIM_ATTACK)
		{
			if(me->frm==9 && me->frmTimer<32)
			{
				LaunchMegabeam(me->x,me->y+FIXAMT*40,me->ID);
			}
			if(me->frm==18)
				me->reload=150;
		}
		return;	// can't do nothin' right now
	}

	if(me->reload==0)
	{
		me->seq=ANIM_ATTACK;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=32;
		me->action=ACTION_BUSY;
	}
}

void AI_Golem(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y;

	if(goodguy==NULL)
	{
		// vanish if there is no goodguy to follow
		me->type=MONS_NONE;
		return;
	}

	if(me->reload)
		me->reload--;

	if(me->mind1)
		me->mind1--;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_GOLEMOUCH,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_GOLEMDIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm==5 && me->reload==0)
		{
			// smash
			MakeSound(SND_BOMBBOOM,me->x,me->y,SND_CUTOFF,600);
			ShakeScreen(5);
			x=me->x+Cosine(me->facing*32)*32;
			y=me->y+Sine(me->facing*32)*32;
			FireBullet(x,y,me->facing,BLT_SHOCKWAVE,me->friendly);
			/*
			if(goodguy)
			{
				if(me->AttackCheck(48,x>>FIXSHIFT,y>>FIXSHIFT,goodguy))
					goodguy->GetShot(Cosine(me->facing*32)*6,Sine(me->facing*32)*6,15,map,world);
			}
			*/
			me->reload=60;
		}
		if(me->seq==ANIM_A1)	// forming
		{
			if(!me->CanWalk(me->x,me->y,map,world))
				me->type=MONS_NONE;
		}
		if(me->seq==ANIM_DIE)
		{
			if(me->frm>3)
			{
				x=me->x-32*FIXAMT+Random(64*FIXAMT);
				y=me->y-32*FIXAMT+Random(64*FIXAMT);
				BlowUpGuy(x+me->rectx,y+me->recty,x+me->rectx2,y+me->recty2,me->z,1);
				BlowSmoke((x+me->rectx+Random(me->rectx2-me->rectx))<<FIXSHIFT,
						  (y+me->recty+Random(me->recty2-me->recty))<<FIXSHIFT,
						  me->z,FIXAMT);
			}
		}
		return;
	}
	if(me->mind==0)	// standard combat mode
	{
		if(me->seq!=ANIM_MOVE)
		{
			me->seq=ANIM_MOVE;
			me->frmAdvance=128;
		}
		me->dx=Cosine(me->facing*32)*2;
		me->dy=Sine(me->facing*32)*2;
		if(goodguy==NULL || goodguy->type==MONS_NOBODY || RangeToTarget(me,goodguy)>512*FIXAMT)
		{
			// too far from goodguy, run home
			me->mind=1;
		}
		else
		{
			if(RangeToTarget(me,goodguy)>300*FIXAMT)	// too far to fight, go home
			{
				me->mind=1;
			}
			else
			{
				if(me->mind1==0)
				{
					FaceGoodguy2(me,goodguy);
					me->mind1=4;
				}
				if(me->reload==0 && RangeToTarget(me,goodguy)<96*FIXAMT)
				{
					me->action=ACTION_BUSY;
					me->seq=ANIM_ATTACK;
					me->frm=0;
					me->frmTimer=0;
					me->frmAdvance=128;
					me->dx=0;
					me->dy=0;
					MakeSound(SND_GOLEMPOUND,me->x,me->y,SND_CUTOFF,1200);
				}
			}
		}
	}
	else if(me->mind==1)	// running home mode
	{
		if(RangeToTarget(me,goodguy)<128*FIXAMT)
		{
			if(goodguy && RangeToTarget(me,goodguy)<300*FIXAMT)
			{
				// got close enough, get violent again
				me->mind=0;
			}
			else
			{
				me->mind=2;	// close enough to the Kid, sit still
			}
		}
		if(me->mind1==0)
		{
			FaceGoodguy2(me,goodguy);
			me->mind1=4;
		}
		if(me->seq!=ANIM_MOVE)
		{
			me->seq=ANIM_MOVE;
			me->frmAdvance=128;
		}
		me->dx=Cosine(me->facing*32)*2;
		me->dy=Sine(me->facing*32)*2;
	}
	else	// waiting patiently mode
	{
		if(me->mind1==0)
			me->mind1=4;
		if(goodguy && RangeToTarget(me,goodguy)<300*FIXAMT)
		{
			// enemy is near, get violent again
			me->mind=0;
		}
		else if(RangeToTarget(me,goodguy)>256*FIXAMT)
		{
			me->mind=1;	// got too far from home!
		}
		if(me->seq!=ANIM_IDLE)
		{
			me->seq=ANIM_IDLE;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
		}
		me->dx=0;
		me->dy=0;
	}
}

void AI_Microfish(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_DIE)
		{
			ExplodeParticles(PART_WATER,me->x,me->y,me->z,2);
		}
		return;	// can't do nothin' right now
	}

	me->dx=Cosine(me->facing*32)*2;
	me->dy=Sine(me->facing*32)*2;

	if(me->mind==0)
	{
		me->mind=Random(10)+1;
		me->facing=(byte)Random(8);
	}
	else
		me->mind--;

}

void AI_BobbyKhan(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y,i;

	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_BOBBYOUCH,me->x,me->y,SND_CUTOFF,1800);
		else
			MakeSound(SND_BOBBYDIE,me->x,me->y,SND_CUTOFF,2000);
	}

	if(!goodguy)
		return;

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_A2 && me->frm>=3)
		{
			// spin attack, begin moving
			if(me->frm<10)
			{
				me->dx+=Cosine(me->facing*32);
				me->dy+=Sine(me->facing*32);
				Clamp(&me->dx,FIXAMT*12);
				Clamp(&me->dy,FIXAMT*12);
			}
			else
			{
				Dampen(&me->dx,FIXAMT);
				Dampen(&me->dy,FIXAMT);
			}
			x=me->x>>FIXSHIFT;
			y=me->y>>FIXSHIFT;

			if(!me->mind1)
				switch(me->frm)
				{
					case 4:
						if(me->AttackCheck2(x-180,y-100,x,y+80,goodguy))
						{
							goodguy->GetShot(-FIXAMT*8,FIXAMT*8,32,map,world);
							goodguy->dx=-FIXAMT*8;
							goodguy->dy=FIXAMT*8;
							me->mind1=1;
						}
						break;
					case 5:
						if(me->AttackCheck2(x-200,y-180,x,y,goodguy))
						{
							goodguy->GetShot(-FIXAMT*12,0,32,map,world);
							goodguy->dx=-FIXAMT*12;
							goodguy->dy=0;
							me->mind1=1;
						}
						break;
					case 6:
						if(me->AttackCheck2(x-150,y-120,x+40,y,goodguy))
						{
							goodguy->GetShot(-FIXAMT*8,-FIXAMT*8,32,map,world);
							goodguy->dx=-FIXAMT*8;
							goodguy->dy=-FIXAMT*8;
							me->mind1=1;
						}
						break;
					case 7:
						if(me->AttackCheck2(x,y-150,x+150,y-20,goodguy))
						{
							goodguy->GetShot(0,-FIXAMT*12,32,map,world);
							goodguy->dx=0;
							goodguy->dy=-FIXAMT*12;
							me->mind1=1;
						}
						break;
					case 8:
						if(me->AttackCheck2(x+20,y-100,x+230,y,goodguy))
						{
							goodguy->GetShot(FIXAMT*8,-FIXAMT*8,32,map,world);
							goodguy->dx=FIXAMT*8;
							goodguy->dy=-FIXAMT*8;
							me->mind1=1;
						}
						break;
					case 9:
						if(me->AttackCheck2(x,y,x+230,y+70,goodguy))
						{
							goodguy->GetShot(FIXAMT*12,0,32,map,world);
							goodguy->dx=FIXAMT*12;
							goodguy->dy=0;
							me->mind1=1;
						}
						break;
					case 3:
					case 10:
						if(me->AttackCheck2(x-120,y,x+45,y+160,goodguy))
						{
							goodguy->GetShot(FIXAMT*8,FIXAMT*8,32,map,world);
							goodguy->dx=FIXAMT*8;
							goodguy->dy=FIXAMT*8;
							me->mind1=1;
						}
						break;
					case 11:
						if(me->AttackCheck2(x-220,y,x-10,y+120,goodguy))
						{
							goodguy->GetShot(FIXAMT*12,0,32,map,world);
							goodguy->dx=0;
							goodguy->dy=FIXAMT*12;
							me->mind1=1;
						}
						break;
				}
		}

		if(me->seq==ANIM_A3 && (me->frm==9 || me->frm==10))
		{
			ShakeScreen(30);
			if(me->frm==9 && me->frmTimer<32)
				MakeSound(SND_SDZLPOUND,me->x,me->y,SND_CUTOFF,1200);

			for(i=0;i<15;i++)
			{
				x=me->x+((400-Random(801))<<FIXSHIFT);
				y=me->y+((400-Random(801))<<FIXSHIFT);
				FireExactBullet(x,y,FIXAMT*160+(Random(40)<<FIXSHIFT),
								0,0,0,0,30,2,BLT_ROCK,me->friendly);
			}
		}
		if(me->seq==ANIM_DIE)
		{
			if(me->frm>=8)
			{
				x=me->x>>FIXSHIFT;
				y=me->y>>FIXSHIFT;
				BlowUpGuy(x+me->rectx,y+me->recty,x+me->rectx2,y+me->recty2,me->z,1);
				BlowSmoke((x+me->rectx+Random(me->rectx2-me->rectx))<<FIXSHIFT,
						  (y+me->recty+Random(me->recty2-me->recty))<<FIXSHIFT,
						  me->z,FIXAMT);
			}
		}
		if(me->seq==ANIM_A4 && me->frm==0 && me->frmTimer<32)
		{
			for(x=0;x<6;x++)
				{
					i=Random(256);
					FireExactBullet(me->x,me->y+FIXAMT*30,FIXAMT*30,Cosine(i)*4,Sine(i)*4,0,0,255,4*32,BLT_BIGYELLOW,me->friendly);
				}
		}
		if(me->seq==ANIM_ATTACK && me->frm==5 && me->frmTimer<32)
		{
			// slash for pain
			x=(me->x>>FIXSHIFT)-64;
			y=(me->y>>FIXSHIFT);
			if(me->AttackCheck2(x,y,x+72,y+225,goodguy))
			{
				goodguy->GetShot(0,FIXAMT*12,64,map,world);
				goodguy->dx=0;
				goodguy->dy=FIXAMT*12;
			}
		}

		return;	// can't do nothin' right now
	}

	if(me->reload==0)
	{
		x=RangeToTarget(me,goodguy);
		y=0;	// flags to decide which attack to do

		// spin ANY time he gets close
		if(x<180*FIXAMT)
		{
			y|=8;
			if(goodguy->x>me->x-64*FIXAMT && goodguy->x<me->x+64*FIXAMT &&
				goodguy->y>me->y)
			{
				y|=1;	// 1=slash attack
			}
		}
		else if(x<300*FIXAMT)	// medium range
		{
			// do the slash if he is conveniently in front of you
			if(goodguy->x>me->x-64*FIXAMT && goodguy->x<me->x+64*FIXAMT &&
				goodguy->y>me->y)
			{
				y|=1;	// 1=slash attack
			}
			y|=2;	// 2=rock stomp

			// only do block if player is below me
			if(goodguy->y>me->y)
				y|=4;	// 4=shield block
		}
		else	// long range
		{
			y|=2;	// rock stomp
			y|=8;	// spin attack from afar sometimes to close the gap
		}

		if(y!=0)
		{
			if((y&1) && (Random(10)==0))	// slash attack is highly likely
			{
				MakeSound(SND_BOBBYSLASH,me->x,me->y,SND_CUTOFF,1200);
				me->reload=30;
				me->action=ACTION_BUSY;
				me->seq=ANIM_ATTACK;	// slash
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=64;
				me->dx=0;
				me->dy=0;
				me->mind=0;
				return;
			}
			if((y&2) && (Random(40)==0))	// rock stomp is rare
			{
				MakeSound(SND_BOBBYSTOMP,me->x,me->y,SND_CUTOFF,1200);
				me->reload=60;
				me->action=ACTION_BUSY;
				me->seq=ANIM_A3;	// stomp
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=128;
				me->dx=0;
				me->dy=0;
				me->mind=0;
				return;
			}
			if((y&4) && (Random(20)==0))	// shield block is less rare, but is superceded by rock stomp
			{
				me->reload=30;
				me->action=ACTION_BUSY;
				me->seq=ANIM_A1;	// block
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=128;
				me->dx=0;
				me->dy=0;
				return;
			}
			if((y&8) && (Random(20)==0))	// spin attack is relatively likely
			{
				MakeSound(SND_BOBBYSPIN,me->x,me->y,SND_CUTOFF,1200);
				me->reload=60;
				me->action=ACTION_BUSY;
				me->seq=ANIM_A2;	// spin attack
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=128;
				FaceGoodguy(me,goodguy);
				me->dx=0;
				me->dy=0;
				me->mind=0;
				return;
			}
		}
	}
	if(me->ouch>2 && me->action==ACTION_IDLE && me->reload==0 && Random(16)==0 && me->mind==0)
	{
		me->reload=60;
		me->action=ACTION_BUSY;
		me->seq=ANIM_A1;	// block
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=128;
		me->dx=0;
		me->dy=0;
		me->mind=1;
		return;
	}
	if(me->action==ACTION_IDLE)
	{
		FaceGoodguy2(me,goodguy);
		me->dx=Cosine(me->facing*32)*4;
		me->dy=Sine(me->facing*32)*4;
		me->mind1=0;
	}
	if(me->seq!=ANIM_MOVE && me->action==ACTION_IDLE)
	{
		me->seq=ANIM_MOVE;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=128;
	}
}

void AI_Crazybush(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_BUSHOUCH,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_BUSHDIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->action==ACTION_BUSY)
	{
		return;	// can't do nothin' right now
	}

	if(goodguy)
	{
		if(RangeToTarget(me,goodguy)<(50*FIXAMT) && Random(8)==0 && map->FindGuy(me->mapx,me->mapy,5,goodguy))
		{
			// get him!
			goodguy->GetShot(me->dx/2,me->dy/2,1,map,world);
		}
		FaceGoodguy2(me,goodguy);

		me->dx=Cosine(me->facing*32)*5;
		me->dy=Sine(me->facing*32)*5;
		if(me->seq!=ANIM_MOVE)
		{
			me->seq=ANIM_MOVE;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
		}
	}
}

void AI_Octopus(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y;

	if(!goodguy)
		return;

	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_OCTONOUCH,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_OCTONDIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK)
		{
			if((me->frm==4 || me->frm==10 || me->frm==16) && me->reload==0 && goodguy)
			{
				// spit stuff
				byte f;

				FaceGoodguy2(me,goodguy);
				x=me->x+Cosine(me->facing*32)*14;
				y=me->y+Sine(me->facing*32)*14;
				f=me->facing*32-12+Random(25);
				FireBullet(x,y,f,BLT_ENERGY,me->friendly);
				me->reload=2;
				MakeSound(SND_OCTONSHOOT,me->x,me->y,SND_CUTOFF,1200);
			}
			if(me->frm==20)
			{
				me->seq=ANIM_A2;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=128;
			}
		}
		if(me->seq==ANIM_A2)
		{
			if(me->frm==5)
				me->mind=0;
		}
		return;	// can't do nothin' right now
	}

	if(me->mind==0)	// lurking
	{
		if(RangeToTarget(me,goodguy)<(300*FIXAMT) && Random(48)==0 && me->reload==0)
		{
			ExplodeParticles(PART_WATER,me->x,me->y,0,10);
			MakeSound(SND_OCTONSPLASH,me->x,me->y,SND_CUTOFF,1200);
			FaceGoodguy2(me,goodguy);
			// arise
			me->seq=ANIM_A1;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
			me->dx=0;
			me->dy=0;
			me->reload=0;
			me->mind=1;
			me->mind1=0;
			return;
		}

		if(me->mind1)
			me->mind1--;
		if(!me->mind1)	// time to get a new direction
		{
			me->facing=(byte)Random(8);
			me->mind1=Random(60)+1;
		}
		me->dx=Cosine(me->facing*32)*2;
		me->dy=Sine(me->facing*32)*2;
		if(me->seq!=ANIM_MOVE)
		{
			me->seq=ANIM_MOVE;
			me->frm=0;
			me->frmTimer=0;
		}
		me->frmAdvance=Random(128)+2;
	}
	else if(me->mind==1)	// arising
	{
		if(me->frm>=5)
		{
			me->seq=ANIM_ATTACK;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=200;
			me->mind=1;
			me->action=ACTION_BUSY;
		}
	}
	if((world->terrain[map->map[me->mapx+me->mapy*map->width].floor].flags&(TF_WATER|TF_LAVA))==0)
		me->GetShot(0,0,20,map,world);
}

void AI_Incabot(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_INCAHIT,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_INCADIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->action==ACTION_BUSY)
	{
		return;	// can't do nothin' right now
	}

	if(goodguy)
	{
		me->x+=Cosine(me->facing*32)*15;
		me->y+=Sine(me->facing*32)*15;
		if(RangeToTarget(me,goodguy)<(35*FIXAMT) && me->frmTimer<128 && me->reload==0 &&
			map->FindGuy(me->mapx,me->mapy,5,goodguy))
		{
			// get him!
			goodguy->GetShot(me->dx/2,me->dy/2,1,map,world);
			me->reload=4;
		}
		me->x-=Cosine(me->facing*32)*15;
		me->y-=Sine(me->facing*32)*15;
		FaceGoodguy(me,goodguy);

		me->dx=Cosine(me->facing*32)*4;
		me->dy=Sine(me->facing*32)*4;
		if(me->seq!=ANIM_MOVE)
		{
			me->seq=ANIM_MOVE;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
		}
	}
}

void AI_Incagen(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	Guy *g;
	int rng;

	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		MakeSound(SND_INCAHIT,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_MOVE && me->frm==8 && me->reload==0)
		{
			g=AddGuy(me->x,me->y+FIXAMT*2,0,MONS_INCABOT,me->friendly);
			if(g && (!g->CanWalk(me->x,me->y,map,world)))
				g->type=MONS_NONE;	// vanish if this spot is occupied
			me->reload=60;
				
		}
		if(me->seq==ANIM_DIE)
		{
			FireBullet(me->x-48*FIXAMT+Random(97*FIXAMT),me->y-40*FIXAMT+Random(81*FIXAMT),0,BLT_BOOM,me->friendly);
		}
		return;
	}
	rng=400*FIXAMT;

	if(!me->reload && me->mind==1 && goodguy && RangeToTarget(me,goodguy)<rng)
	{
		MakeSound(SND_INCAGEN,me->x,me->y,SND_CUTOFF,1200);
		me->seq=ANIM_MOVE;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=128;
		me->action=ACTION_BUSY;
		map->map[me->mapx+me->mapy*map->width].templight=8;
		if(me->mapx>0)
			map->map[me->mapx-1+me->mapy*map->width].templight=0;
		if(me->mapx<map->width-1)
			map->map[me->mapx+1+me->mapy*map->width].templight=0;
		if(me->mapy>0)
			map->map[me->mapx+(me->mapy-1)*map->width].templight=0;
		if(me->mapy<map->height-1)
			map->map[me->mapx+(me->mapy+1)*map->width].templight=0;
	}
	if(goodguy && RangeToTarget(me,goodguy)<rng)
		me->mind=1;
}


void AI_Incagold(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	if(me->reload)
		me->reload--;

	if(me->action==ACTION_BUSY)
	{
		return;
	}

	if(me->aiType==MONS_INCAGOLD && me->mind==0)
	{
		me->mind=1;
		if(me->mapx>0 && map->map[me->mapx-1+me->mapy*map->width].wall>0)
		{
			me->facing=0;	// head right
			me->dx=FIXAMT*3/2;
			me->dy=0;
		}
		else
		{
			me->facing=4;	// head left
			me->dx=-FIXAMT*3/2;
			me->dy=0;
		}
	}
	else if(me->aiType==MONS_INCAGOLD2 && me->mind==0)
	{
		me->mind=1;
		if(me->mapy>0 && map->map[me->mapx+(me->mapy-1)*map->width].wall>0)
		{
			me->facing=2;	// head down
			me->dx=0;
			me->dy=FIXAMT*3/2;
		}
		else
		{
			me->facing=6;	// head up
			me->dx=0;
			me->dy=-FIXAMT*3/2;
		}
	}

	if(me->mind1)	// bumped a wall
	{
		me->mind1=0;

		if(me->aiType==MONS_INCAGOLD)
		{
			me->facing=4-me->facing;
			me->dx=-me->dx;
		}
		else
		{
			me->facing=8-me->facing;
			me->dy=-me->dy;
		}
	}

	if(me->seq!=ANIM_MOVE)
	{
		me->seq=ANIM_MOVE;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=128;
	}

	if(goodguy)
	{
		char sway[]={0,1,2,3,4,3,2,1,0,-1,-2,-3,-4,-3,-2,-1};

		if(me->facing==0 && goodguy->x>me->x && abs(goodguy->y-me->y)<32*FIXAMT)
		{
			FireBullet(me->x,me->y,me->facing,BLT_LASER2,me->friendly);
			if(me->reload==0)
			{
				MakeSound(SND_INCABEAM,me->x,me->y,SND_CUTOFF,100);
				me->reload=10;
			}
		}
		if(me->facing==4 && goodguy->x<me->x && abs(goodguy->y-me->y)<32*FIXAMT)
		{
			FireBullet(me->x,me->y,me->facing,BLT_LASER2,me->friendly);
			if(me->reload==0)
			{
				MakeSound(SND_INCABEAM,me->x,me->y,SND_CUTOFF,100);
				me->reload=10;
			}
		}
		if(me->facing==2 && goodguy->y>me->y && abs(goodguy->x-me->x)<48*FIXAMT)
		{
			FireBullet(me->x,me->y,me->facing,BLT_LASER2,me->friendly);
			if(me->reload==0)
			{
				MakeSound(SND_INCABEAM,me->x,me->y,SND_CUTOFF,100);
				me->reload=10;
			}
		}
		if(me->facing==6 && goodguy->y<me->y && abs(goodguy->x-me->x)<48*FIXAMT)
		{
			FireBullet(me->x,me->y,me->facing,BLT_LASER2,me->friendly);
			if(me->reload==0)
			{
				MakeSound(SND_INCABEAM,me->x,me->y,SND_CUTOFF,100);
				me->reload=10;
			}
		}
	}
}

void AI_Incaboss(Guy *me,Map *map,world_t *world,Guy *goodguy) //add new bullets!
{
	int x,y,i;

	if(me->reload)
		me->reload--;
	
	if(me->seq==ANIM_DIE)
	{
		me->dx=FIXAMT*0;
		x=me->x>>FIXSHIFT;
		y=me->y>>FIXSHIFT;
		BlowUpGuy(x+me->rectx,y+me->recty,x+me->rectx2,y+me->recty2,me->z,1);
		BlowSmoke((x+me->rectx+Random(me->rectx2-me->rectx))<<FIXSHIFT,
				  (y+me->recty+Random(me->recty2-me->recty))<<FIXSHIFT,
				  me->z,FIXAMT);
	}

	if(goodguy)
	{
	if(goodguy->y<me->y+FIXAMT*64 && abs(goodguy->x-me->x)<FIXAMT*90 && me->mind>0)
		goodguy->GetShot(0,0,10,map,world);}

	if(me->mind==0)	// not set up yet
	{
		if(goodguy && map->FindGuy(me->mapx,me->mapy,12,goodguy) && (goodguy->y<=50*TILE_HEIGHT*FIXAMT+(FIXAMT*8)))
		{
			me->mind=1;	// mode 1 = doing nothing
			me->dx=FIXAMT*4;
			me->mind3=60;	// used as a timer
		}
		else
			return;	// wait til he is close
	}

	if(me->mind1)	// bumped a wall
	{
		me->mind1=0;

		me->dx=-me->dx;
	}

	me->mind3--;
	me->z=me->mind3*2;
	
	if(me->mind3==0)
	{
		me->mind++;
		if(me->mind==6)
			me->mind=1;
		me->mind3=60;
		
		switch(me->mind)	// setup each mode
		{
			case 1:
				me->frmAdvance=128;

				break;
			case 2:	// fire breath
				me->seq=ANIM_ATTACK;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=128;
				break;
			case 3:
				break;
			case 4:	// laser eyes
				me->seq=ANIM_A2;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=128;
				break;
			case 5:	// tongue
				me->seq=ANIM_A1;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=80;
				break;
		}
	}
	switch(me->mind)
	{
		case 1:	// doing nothing
			break;
		case 2:	// fire breath
			// shoot flames
			if(me->frm>=4 && me->frm<=15)
			{
				for(i=0;i<4;i++)
					FireBullet(me->x-FIXAMT*48+Random(FIXAMT*96+1),me->y+FIXAMT*32,(byte)(2*32-8+Random(17)),BLT_FLAME2,me->friendly);
			}

			if(me->seq==ANIM_IDLE)
				me->mind3=1;
			break;
		case 3:	// waiting after fire
			break;
		case 4:	// laser eyes
			// fire lasers
			if(me->frm>=6 && me->frm<=16)
			{
				if(me->reload==0)
				{
					MakeSound(SND_FLAMEGO,me->x,me->y,SND_CUTOFF,100);
					me->reload=10;
				}
				if(me->dx>0)
				{
					FireBullet(me->x-FIXAMT*48,me->y-FIXAMT,1*32,BLT_LASER2,me->friendly);
					FireBullet(me->x+FIXAMT*42,me->y-FIXAMT,1*32,BLT_LASER2,me->friendly);
				}
				else
				{
					FireBullet(me->x-FIXAMT*48,me->y-FIXAMT,3*32,BLT_LASER2,me->friendly);
					FireBullet(me->x+FIXAMT*42,me->y-FIXAMT,3*32,BLT_LASER2,me->friendly);
				}
			}
			if(me->seq==ANIM_IDLE)
				me->mind3=1;
			break;
		case 5:	// tongue
			if(me->frm==7 && me->frmTimer<128)
				MakeSound(SND_INCATONGUE,me->x,me->y,SND_CUTOFF,100);
			break;

	}
}

void AI_Incatongue(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_INCAHIT,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_INCADIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->seq==ANIM_DIE)
	{
		me->parent->hp=0;
		me->parent->seq=ANIM_DIE;
		me->parent->action=ACTION_BUSY;
		me->parent->frm=0;
		me->parent->frmTimer=0;
		me->parent->frmAdvance=64;
	}

	if(me->action==ACTION_BUSY)
	{
		return;
	}

	me->x=me->parent->x;
	if(me->parent->seq==ANIM_A1 && me->parent->frm>5)
	{
		me->y=me->parent->y+FIXAMT*48;
		monsType[me->type].size=50;
		monsType[me->type].flags&=(~MF_INVINCIBLE);

	}
	else
	{
		me->y=me->parent->y-FIXAMT*32;
		monsType[me->type].flags|=MF_INVINCIBLE;
		monsType[me->type].size=5;
	}
}

void AI_PeepBomb(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	if(!goodguy)
		return;

	if(me->reload)
		me->reload--;

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_DIE)
		{
			me->frmAdvance=256;
			me->facing=0;
		}
		return;
	}

	if(me->mind==0)	// spinning, looking for goodguy
	{
		if(me->aiType==MONS_PEEPBOMB)
		{
			me->mind1++;
		}
		else
		{
			me->mind1--;

		}
		me->facing=me->mind1/8;	// pick the frame

		if(PeepAtKid(me->x,me->y,map,me->mind1))
		{
			MakeSound(SND_PEEPALARM,me->x,me->y,SND_CUTOFF|SND_ONE,3000);
			me->mind=1;
			me->dx=Cosine(me->mind1)*7;
			me->dy=Sine(me->mind1)*7;
			me->mind1=30;
		}
	}
	if(me->mind==1)	// chasing!
	{
		me->mind1--;
		if(!me->mind1)
		{
			me->mind=0;
			me->dx=0;
			me->dy=0;
			me->mind1=me->facing*8;
		}
		else
		{
			FaceGoodguy(me,goodguy);
			me->facing*=4;
			me->dx=Cosine(me->facing*8 )*7;
			me->dy=Sine(me->facing*8)*7;
			if(RangeToTarget(me,goodguy)<FIXAMT*64)
			{
				me->hp=0;
				me->facing=0;
				me->seq=ANIM_DIE;
				me->frmAdvance=256;
				me->frm=0;
				me->frmTimer=0;
				me->dx=0;
				me->dy=0;
				MakeSound(SND_BOMBBOOM,me->x,me->y,SND_CUTOFF,1200);
				goodguy->GetShot(-Cosine(me->facing*8)*20,-Sine(me->facing*8)*20,30,map,world);
				me->action=ACTION_BUSY;
			}
		}
	}
}

void AI_Trapazoid(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	if(me->reload)
		me->reload--;
	if(me->mind3==0)
	me->mind3=1;
	
	if(me->mind3==1)
	{
		if(me->hp==0)
			return;	// don't sweat it if you dead

		if(goodguy && RangeToTarget(me,goodguy)<150*FIXAMT && me->reload==0)
		{
			FaceGoodguy(me,goodguy);
			me->mind3=2;
			me->seq=ANIM_ATTACK;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=256;
			me->action=ACTION_BUSY;
			me->mind1=0;
			return;
		}
	}
	else
	{
		if(me->ouch==4)
		{
			if(me->hp>0)
				MakeSound(SND_SERPENTOUCH,me->x,me->y,SND_CUTOFF,1200);
			else
				MakeSound(SND_SERPENTDIE,me->x,me->y,SND_CUTOFF,1200);
		}

		if(me->action==ACTION_BUSY)
		{
			if(me->seq==ANIM_DIE && me->frm==6)
			{
				me->mind3=1;
				me->seq=ANIM_IDLE;
				me->frm=0;
				me->frmTimer=0;
			}
			if(me->seq==ANIM_ATTACK)
			{
				if(me->frm>=6 && me->frm<=8)
				{	
					if(me->aiType==MONS_TRAPAZOID2)
					FireExactBullet(me->x,me->y,FIXAMT*10,Cosine(me->facing*32+(me->frm-6)*10)*8,Sine(me->facing*32+(me->frm-6)*10)*8,0,0,32,me->facing*32+(me->frm-6)*10,BLT_BIGSHELL,me->friendly);
					else
					FireExactBullet(me->x,me->y,FIXAMT*10,Cosine(me->facing*32+(me->frm-6)*10)*8,Sine(me->facing*32+(me->frm-6)*10)*8,0,0,32,me->facing*32+(me->frm-6)*10,BLT_ENERGY,me->friendly);
				}
				if(me->frm==9)
				{
					me->frm=6;
					me->facing=(me->facing+1)&7;
					me->mind1++;
					if(me->mind1==8)
					{
						me->seq=ANIM_DIE;
						me->frm=0;
						me->frmTimer=0;
						me->frmAdvance=128;
					}
				}
			}

			return;
		}
	}
}

void AI_Whackazoid(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	if(me->reload)
		me->reload--;
	

	if(me->seq==ANIM_IDLE)
	{
		if (Random(64))
		{
			me->seq=ANIM_ATTACK;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
		}
		if(me->hp==0)
			return;	// don't sweat it if you dead
	}
	else
	{
		if(me->ouch==4)
		{
			if(me->hp>0)
				MakeSound(SND_SERPENTOUCH,me->x,me->y,SND_CUTOFF,1200);
			else
			{
				MakeSound(SND_SERPENTDIE,me->x,me->y,SND_CUTOFF,1200);
			}
		}

		if(me->action==ACTION_BUSY)
		{
			return;
		}
	}
}

void AI_Slug(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp==0)
			MakeSound(SND_SNAILDIE,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_SNAILHIT,me->x,me->y,SND_CUTOFF,1200);
		me->facing=(me->facing+1-Random(3))&7;
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_DIE && Random(7)==0)
		{
			FireBullet(me->x-FIXAMT*40+Random(FIXAMT*81),me->y-FIXAMT*30+Random(FIXAMT*61),0,BLT_SLIME,me->friendly);
		}
		return;	// can't do nothin' right now
	}

	if(me->mind1)	// bumped a wall
	{
		me->facing=(byte)Random(8);
		me->mind1=0;
	}

	if(me->reload==0 && goodguy && RangeToTarget(me,goodguy)<800*FIXAMT)
	{
		me->reload=10;
		FireBullet(me->x,me->y,0,BLT_SLIME,me->friendly);
	}

	me->dx=Cosine(me->facing*32)*2;
	me->dy=Sine(me->facing*32)*2;

	if(me->seq!=ANIM_MOVE)
	{
		me->seq=ANIM_MOVE;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=128;
	}
}

void AI_Snail(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int i,x,y;
	byte b;
	
	if((me->seq==ANIM_A1 && me->frm==8)||(me->seq==ANIM_A2 && me->frm==2))
	{
		if(me->seq==ANIM_A1)
		me->mind2=1;
		else
		me->mind2=0;
	}

	if(me->mind3==1)
	{
		if(goodguy && RangeToTarget(me,goodguy)>(260*FIXAMT))
		{
			me->seq=ANIM_A2;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=256;
			me->action=ACTION_BUSY;
			me->mind3=0;
		}
		else
		{
			me->seq=ANIM_A3;
			me->frmAdvance=0;
		}

		return;
	}

	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp==0)
			MakeSound(SND_SNAILDIE,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_SNAILHIT,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_A1 && me->frm==10)
		{
			me->seq=ANIM_IDLE;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
			me->action=ACTION_IDLE;
			me->mind3=1;
		}

		if(me->seq==ANIM_ATTACK && me->frm>=3 && me->frm<=5)
		{
			// fire lots of acid
			for(i=0;i<2;i++)
			{
				x=me->x+Cosine(me->facing*32)*20-FIXAMT*4+Random(FIXAMT*4+1);
				y=me->y+Sine(me->facing*32)*20-FIXAMT*4+Random(FIXAMT*4+1);
				b=me->facing*32-4+Random(9);
				FireExactBullet(x,y,FIXAMT*15,Cosine(b)*12,Sine(b)*12,FIXAMT*6,0,60,((b+16)&255)/32,BLT_ACID,me->friendly);
			}
		}
		if(me->seq==ANIM_DIE)
			me->frmAdvance=256;
		return;
	}

	if(goodguy && RangeToTarget(me,goodguy)<(240*FIXAMT))
	{
		// hide in your shell!
		me->seq=ANIM_A1;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=256;
		me->action=ACTION_BUSY;
		me->dx=0;
		me->dy=0;
		me->reload=0;
		return;
	}

	if(me->mind==0)		// when mind=0, singlemindedly lumber towards Bouapha
	{
		if(goodguy)
		{
			FaceGoodguy2(me,goodguy);

			me->dx=Cosine(me->facing*32)*2;
			me->dy=Sine(me->facing*32)*2;
			if(me->seq!=ANIM_MOVE)
			{
				me->seq=ANIM_MOVE;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=128;
			}
			if(Random(64)==0)
			{
				me->mind=1;		// occasionally wander
				me->mind1=1;
			}
		}
		else
		{
			me->mind=1;	// if there's no goodguy, get random
			me->mind1=1;
		}
	}
	else if(me->mind==1)	// random wandering
	{
		if(!(me->mind1--))	// time to get a new direction
		{
			if((goodguy) && Random(3)==0)
				me->mind=0;	// get back on track
			else
				me->facing=(byte)Random(8);
			me->mind1=Random(40)+1;
		}

		me->dx=Cosine(me->facing*32)*2;
		me->dy=Sine(me->facing*32)*2;
		if(me->seq!=ANIM_MOVE)
		{
			me->seq=ANIM_MOVE;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
		}
	}

	// spit if close enough and you want to
	if(goodguy && RangeToTarget(me,goodguy)<500*FIXAMT && me->reload==0)
	{
		FaceGoodguy(me,goodguy);
		me->seq=ANIM_ATTACK;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=128;
		me->action=ACTION_BUSY;
		me->dx=0;
		me->dy=0;
		me->reload=60;
	}
}

void AI_Ball(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	if(goodguy && RangeToTarget(me,goodguy)<40*FIXAMT && me->dy>=0)
	{
		MakeSound(SND_BOING+Random(3),me->x,me->y,SND_CUTOFF,2);
		if(me->dy==0)
			me->dy=-FIXAMT*4;
		else
			me->dy=-me->dy;
		me->dx=(goodguy->dx/4)+Cosine(goodguy->facing*32)*2;
	}

	if(me->mind1)
	{
		MakeSound(SND_BOING+Random(3),me->x,me->y,SND_CUTOFF,2);
		if(me->mind1&1)	// horiz hit
		{
			me->dx=-me->dx;
			me->dy+=-FIXAMT/16+Random(FIXAMT/8+1);
		}
		if(me->mind1&2)	// vert hit
		{
			me->dy=-me->dy;
			me->dx+=-FIXAMT/16+Random(FIXAMT/8+1);
		}
		me->mind1=0;
	}

	if(world->terrain[map->map[me->mapx+me->mapy*map->width].floor].flags&(TF_WATER|TF_LAVA))
	{
		FireBullet(me->x,me->y,0,BLT_MEGABOOM,0);
		FireBullet(me->x,me->y,0,BLT_MEGABOOM,1);
		me->type=MONS_NONE;
	}

	Clamp(&me->dx,FIXAMT*5);
	Clamp(&me->dy,FIXAMT*5);
}

void AI_OctoBoss(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int i,x,y;

	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_INCAHIT,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_INCADIE,me->x,me->y,SND_CUTOFF,1200);
	}

	for(i=0;i<8;i++)
	{
		x=me->x/FIXAMT-60+Random(121);
		y=me->y/FIXAMT-60+Random(121);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm>=4 && me->frm<=11)
		{
			x=me->x-FIXAMT*30+Random(60*FIXAMT+1);
			y=me->y+FIXAMT*70;
			FireBullet(x,y,2*32-14+Random(29),BLT_ENERGY,me->friendly);
			me->reload=60;
		}
		return;	// can't do nothin' right now
	}

	if(goodguy)
	{
		x=goodguy->x;
		y=goodguy->y-50*FIXAMT;

		FaceGoodguy2(me,goodguy);
		if(me->x<x)
			me->dx+=FIXAMT/8;
		if(me->y<y)
			me->dy+=FIXAMT/8;
		if(me->x>x)
			me->dx-=FIXAMT/8;
		if(me->y>y)
			me->dy-=FIXAMT/8;

		if(me->mind1)
		{
			me->mind1=0;
			me->dy-=FIXAMT/4;	// move up if bumping an obstacle
		}
		Clamp(&me->dx,FIXAMT*3);
		Clamp(&me->dy,FIXAMT*3);

		if(me->reload==0 && (abs(me->x-x)<150*FIXAMT))	// within a reasonable horiz range
		{
			me->seq=ANIM_ATTACK;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
			me->action=ACTION_BUSY;
		}
	}
}

void AI_OctoTentacle(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	if(me->reload)
		me->reload--;

	if(!me->parent || me->parent->hp==0)
	{
		me->GetShot(0,0,255,map,world);
	}

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_SPD3OUCH,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_SPD3DIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm==9 && me->reload==0)
		{
			if(!FindVictim((me->x+Cosine(me->facing*32)*160)/FIXAMT,(me->y+Sine(me->facing*32)*100)/FIXAMT,60,
				Cosine(me->facing*32)*10,Sine(me->facing*32)*10,8,map,world,me->friendly))
			{
				FindVictim((me->x+Cosine(me->facing*32)*40)/FIXAMT,(me->y+Sine(me->facing*32)*30)/FIXAMT,80,
					Cosine(me->facing*32)*10,Sine(me->facing*32)*10,8,map,world,me->friendly);
			}
			me->reload=4;
		}

		return;	// can't do nothin' right now
	}

	if(me->mind==0)		// when mind=0, singlemindedly lumber towards Bouapha
	{
		if(goodguy)
		{
			if(RangeToTarget(me,goodguy)<(200*FIXAMT) && Random(8)==0)
			{
				// get him!
				FaceGoodguy(me,goodguy);
				MakeSound(SND_SKELKICK,me->x,me->y,SND_CUTOFF,1200);
				me->seq=ANIM_ATTACK;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=128;
				me->action=ACTION_BUSY;
				me->dx=0;
				me->dy=0;
				me->reload=0;
				return;
			}
			FaceGoodguy2(me,goodguy);

			me->dx=Cosine(me->facing*32)*4;
			me->dy=Sine(me->facing*32)*4;
			if(Random(20)==0)
			{
				me->mind=1;		// occasionally wander
				me->mind1=10;
			}
		}
		else
		{
			me->mind=1;	// if there's no goodguy, get random
			me->mind1=10;
		}
	}
	else if(me->mind==1)	// random wandering
	{
		if(goodguy)
		{
			if(RangeToTarget(me,goodguy)<(200*FIXAMT) && Random(8)==0)
			{
				// get him!
				FaceGoodguy(me,goodguy);
				MakeSound(SND_SKELKICK,me->x,me->y,SND_CUTOFF,1200);
				me->seq=ANIM_ATTACK;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=128;
				me->action=ACTION_BUSY;
				me->dx=0;
				me->dy=0;
				me->reload=0;
				return;
			}
		}
		if(!(me->mind1--))	// time to get a new direction
		{
			if((goodguy) && Random(7)==0)
				me->mind=0;	// get back on track
			else
				me->facing=(byte)Random(8);
			me->mind1=Random(40)+1;
			me->dx=Cosine(me->facing*32)*4;
			me->dy=Sine(me->facing*32)*4;
		}
	}

	if(me->y<me->parent->y-FIXAMT*140)
	{
		me->dy=FIXAMT*4;
		me->mind=1;
		me->mind1=10;
	}
	if(me->y>me->parent->y+FIXAMT*140)
	{
		me->dy=-FIXAMT*4;
		me->mind=1;
		me->mind1=10;
	}
	if(me->aiType==MONS_OCTOTENT)	// left-side tentacle
	{
		if(me->x<me->parent->x-FIXAMT*220)
		{
			me->dx=FIXAMT*4;
			me->mind=1;
			me->mind1=10;
		}
		if(me->x>me->parent->x-FIXAMT*30)
		{
			me->dx=-FIXAMT*4;
			me->mind=1;
			me->mind1=10;
		}
	}
	else
	{
		if(me->x>me->parent->x+FIXAMT*220)
		{
			me->dx=-FIXAMT*4;
			me->mind=1;
			me->mind1=10;
		}
		if(me->x<me->parent->x+FIXAMT*30)
		{
			me->dx=FIXAMT*4;
			me->mind=1;
			me->mind1=10;
		}
	}

	if(RangeToTarget(me,me->parent)<FIXAMT*100)
	{
		if(me->x<me->parent->x)
			me->dx-=FIXAMT*6;
		if(me->x>me->parent->x)
			me->dx+=FIXAMT*6;
		if(me->y<me->parent->y)
			me->dy-=FIXAMT*6;
		if(me->y>me->parent->y)
			me->dy+=FIXAMT*6;
		me->mind=1;
		me->mind1=10;
	}
}

void AI_Rammy(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y;
	
	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_GOATHIT,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_GOATDIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(!goodguy)
		return;

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_MOVE)
		{
			if(me->mind<20)
			{
				me->mind++;
				me->mind1=0;	// don't let him bump stuff when slow
			}
			else
				if(FindVictim((me->x+Cosine(me->facing*32)*40)/FIXAMT,(me->y+Sine(me->facing*32)*30)/FIXAMT,40,
						Cosine(me->facing*32)*10,Sine(me->facing*32)*10,12,map,world,me->friendly) || me->mind1)
				{
					// make THUD noise
					MakeSound(SND_GOATDIZZY,me->x,me->y,SND_CUTOFF,1200);
					me->mind3=1;	// become vulnerable
					me->seq=ANIM_A1;	// get stunned
					me->frm=0;
					me->frmTimer=0;
					me->frmAdvance=128;
					me->action=ACTION_BUSY;
					me->dx=0;
					me->dy=0;
					ShakeScreen(10);
					me->mind1=0;
					return;
				}

			me->dx+=Cosine(me->facing*32)/2;
			me->dy+=Sine(me->facing*32)/2;
			Clamp(&me->dx,FIXAMT*8);
			Clamp(&me->dy,FIXAMT*8);
			if(me->reload==0)
			{
				me->reload=10;
				FaceGoodguy2(me,goodguy);
			}
			if(me->frm==12)
			{
				me->frm=0;
				MakeSound(SND_GOATRUN,me->x,me->y,SND_CUTOFF,1200);
			}
			if(Random(3)==0)
			{
				x=me->x>>FIXSHIFT;
				y=me->y>>FIXSHIFT;
				BlowSmoke((x+me->rectx+Random(me->rectx2-me->rectx))<<FIXSHIFT,
						  (y+me->recty+Random(me->recty2-me->recty))<<FIXSHIFT,
						  me->z-Random(FIXAMT*60),FIXAMT);
			}
		}
		if(me->seq==ANIM_A1 && me->frm==22)
		{
			me->mind3=0;	// invulnerable again
			me->seq=ANIM_IDLE;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
			me->action=ACTION_IDLE;
		}
		return;	// can't do nothin' right now
	}

	FaceGoodguy(me,goodguy);

	if(Random(48)==0)
	{
		MakeSound(SND_GOATRUN,me->x,me->y,SND_CUTOFF,1200);
		me->seq=ANIM_MOVE;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=128;
		me->action=ACTION_BUSY;
		me->dx=0;
		me->dy=0;
		me->reload=10;
		me->mind=0;
		return;
	}
}

void AI_Hoppy(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	if(!goodguy)
		return;

	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_GOATHIT,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_GOATDIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->mind3==1)
		me->action=ACTION_BUSY;

	if(me->action==ACTION_BUSY)
	{
		if(me->z>0 )
			me->dz+=FIXAMT/4;
		if(me->seq==ANIM_A2 && me->frm==3)
		{
			me->mind3=1;
			me->dz=FIXAMT*20;
			me->dx=Cosine(me->facing*32)*5;
			me->dy=Sine(me->facing*32)*5;
			me->z=FIXAMT;
			me->seq=ANIM_IDLE;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
		}

		if(me->z==0 && me->mind3==1 && me->seq==ANIM_IDLE)
		{
			// thud sound again
			MakeSound(SND_BOMBBOOM,me->x,me->y,SND_CUTOFF,1200);
			ShakeScreen(10);
			me->mind3=0;
			me->seq=ANIM_A3;	// squish onto the ground
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
			me->dx=0;
			me->dy=0;
			me->mind1=0;
			FireBullet(me->x,me->y,0,BLT_SHOCKWAVE,me->friendly);
		}

		return;	// can't do nothin' right now
	}

	if(RangeToTarget(me,goodguy)<(300*FIXAMT) && Random(16)==0)
	{
		// get him!
		FaceGoodguy(me,goodguy);
		MakeSound(SND_GOATSHOOT,me->x,me->y,SND_CUTOFF,1200);
		me->seq=ANIM_A2;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=128;
		me->action=ACTION_BUSY;
		me->dx=0;
		me->dy=0;
		me->reload=0;
		return;
	}

	if(!me->mind1)	// time to get a new direction
	{
		me->facing=(byte)Random(8);
		me->mind1=Random(40)+1;
		me->dx=Cosine(me->facing*32)*4;
		me->dy=Sine(me->facing*32)*4;
	}
	else
		me->mind1--;

	if(me->seq!=ANIM_MOVE)
	{
		me->seq=ANIM_MOVE;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=128;
	}
}

void AI_Gruffy(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y;

	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_GOATHIT,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_GOATDIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm==2 && me->reload==0 && goodguy)
		{
			x=me->x+Cosine(me->facing*32)*24;
			y=me->y+Sine(me->facing*32)*24;
			FireBullet(x,y,(byte)(me->facing*32-8+Random(17)),BLT_BIGYELLOW,me->friendly);
			me->reload=5;
			me->mind1=1;
		}
		return;	// can't do nothin' right now
	}

	if(me->mind==0)		// when mind=0, singlemindedly lumber towards Bouapha
	{
		if(goodguy)
		{
			if(RangeToTarget(me,goodguy)<(512*FIXAMT) && Random(64)==0)
			{
				// spit at him
				MakeSound(SND_GOATSHOOT,me->x,me->y,SND_CUTOFF,1200);
				me->seq=ANIM_ATTACK;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=128;
				me->action=ACTION_BUSY;
				me->dx=0;
				me->dy=0;
				me->reload=0;
				FaceGoodguy(me,goodguy);
				return;
			}

			FaceGoodguy2(me,goodguy);

			me->dx=Cosine(me->facing*32)*4;
			me->dy=Sine(me->facing*32)*4;
			if(me->seq!=ANIM_MOVE)
			{
				me->seq=ANIM_MOVE;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=128;
			}
			if(Random(64)==0)
			{
				me->mind=1;		// occasionally wander
				me->mind1=1;
			}
		}
		else
		{
			me->mind=1;	// if there's no goodguy, get random
			me->mind1=1;
		}
	}
	else if(me->mind==1)	// random wandering
	{
		if(goodguy)
		{
			if(RangeToTarget(me,goodguy)<(512*FIXAMT) && Random(64)==0)
			{
				// spit at him
				MakeSound(SND_SKELSHOOT,me->x,me->y,SND_CUTOFF,1200);
				me->seq=ANIM_ATTACK;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=128;
				me->action=ACTION_BUSY;
				me->dx=0;
				me->dy=0;
				me->reload=0;
				FaceGoodguy(me,goodguy);
				return;
			}
		}
		if(!(me->mind1--))	// time to get a new direction
		{
			if((goodguy) && Random(10)==0)
				me->mind=0;	// get back on track
			else
				me->facing=(byte)Random(8);
			me->mind1=Random(40)+1;
		}

		me->dx=Cosine(me->facing*32)*4;
		me->dy=Sine(me->facing*32)*4;
		if(me->seq!=ANIM_MOVE)
		{
			me->seq=ANIM_MOVE;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
		}
	}
}

void AI_StickShroom(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y,i;

	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_HAPPYOUCH,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_HAPPYDIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK)
		{
			i=(me->facing*32-8+Random(17))&255;
			if(i<0)
				i+=256;
			x=me->x+Cosine(i)*4;
			y=me->y+Sine(i)*4;
			FireExactBullet(x,y,FIXAMT*20,Cosine(i)*8,Sine(i)*8,0,0,16,i,BLT_SPORE,me->friendly);
		}
		return;	// can't do nothin' right now
	}

	if(me->mind==0)		// when mind=0, charge towards Bouapha
	{
		if(goodguy)
		{
			if(RangeToTarget(me,goodguy)<(128*FIXAMT) && Random(8)==0)
			{
				// get him!
				MakeSound(SND_HAPPYCHOMP,me->x,me->y,SND_CUTOFF,1200);
				me->seq=ANIM_ATTACK;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=128;
				me->action=ACTION_BUSY;
				me->dx=0;
				me->dy=0;
				return;
			}
			FaceGoodguy2(me,goodguy);

			me->dx=Cosine(me->facing*32)*3;
			me->dy=Sine(me->facing*32)*3;
			if(me->seq!=ANIM_MOVE)
			{
				me->seq=ANIM_MOVE;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=128;
			}
			if(Random(128)==0)
			{
				me->mind=1;		// occasionally wander
				me->mind1=(byte)Random(100);
			}
		}
		else
		{
			me->mind=1;	// if there's no goodguy, sit around
			me->mind1=(byte)Random(100);
		}
	}
	else if(me->mind==1)	// random wandering
	{
		if(goodguy && RangeToTarget(me,goodguy)<(128*FIXAMT) && Random(8)==0)
		{
			// get him!
			MakeSound(SND_HAPPYCHOMP,me->x,me->y,SND_CUTOFF,1200);
			me->seq=ANIM_ATTACK;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
			me->action=ACTION_BUSY;
			me->dx=0;
			me->dy=0;
			return;
		}
		FaceGoodguy2(me,goodguy);

		if(!(me->mind1--))	// time to get a new direction
		{
			if((goodguy) && Random(10)==0)
				me->mind=0;	// get back on track
			else
				me->facing=(byte)Random(8);
			me->mind1=Random(40)+1;
		}

		me->dx=Cosine(me->facing*32)*3;
		me->dy=Sine(me->facing*32)*3;
		if(me->seq!=ANIM_MOVE)
		{
			me->seq=ANIM_MOVE;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
		}
	}
}

void AI_StickSpider(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y,i;

	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_HAPPYOUCH,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_HAPPYDIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->reload==0)
		{
			i=(me->facing*32-8+Random(17))&255;
			if(i<0)
				i+=256;
			x=me->x+Cosine(i)*4;
			y=me->y+Sine(i)*4;
			FireBullet(x,y,i,BLT_ACID,me->friendly);
			me->reload=2;
		}
		return;	// can't do nothin' right now
	}

	if(me->mind==0)		// when mind=0, charge towards Bouapha
	{
		if(goodguy)
		{
			if(RangeToTarget(me,goodguy)<(128*FIXAMT) && Random(8)==0)
			{
				// get him!
				MakeSound(SND_HAPPYCHOMP,me->x,me->y,SND_CUTOFF,1200);
				me->seq=ANIM_ATTACK;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=128;
				me->action=ACTION_BUSY;
				me->dx=0;
				me->dy=0;
				return;
			}
			FaceGoodguy2(me,goodguy);

			me->dx=Cosine(me->facing*32)*3;
			me->dy=Sine(me->facing*32)*3;
			if(me->seq!=ANIM_MOVE)
			{
				me->seq=ANIM_MOVE;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=128;
			}
			if(Random(128)==0)
			{
				me->mind=1;		// occasionally wander
				me->mind1=(byte)Random(100);
			}
		}
		else
		{
			me->mind=1;	// if there's no goodguy, sit around
			me->mind1=(byte)Random(100);
		}
	}
	else if(me->mind==1)	// random wandering
	{
		if(goodguy && RangeToTarget(me,goodguy)<(128*FIXAMT) && Random(8)==0)
		{
			// get him!
			MakeSound(SND_HAPPYCHOMP,me->x,me->y,SND_CUTOFF,1200);
			me->seq=ANIM_ATTACK;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
			me->action=ACTION_BUSY;
			me->dx=0;
			me->dy=0;
			return;
		}
		FaceGoodguy2(me,goodguy);

		if(!(me->mind1--))	// time to get a new direction
		{
			if((goodguy) && Random(10)==0)
				me->mind=0;	// get back on track
			else
				me->facing=(byte)Random(8);
			me->mind1=Random(40)+1;
		}

		me->dx=Cosine(me->facing*32)*3;
		me->dy=Sine(me->facing*32)*3;
		if(me->seq!=ANIM_MOVE)
		{
			me->seq=ANIM_MOVE;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
		}
	}
}

void AI_StickCorpse(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y,i;

	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_HAPPYOUCH,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_HAPPYDIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK)
		{
			i=(me->facing*32-8+Random(17))&255;
			if(i<0)
				i+=256;
			x=me->x+Cosine(i)*4;
			y=me->y+Sine(i)*4;
			FireExactBullet(x,y,FIXAMT*50,Cosine(i)*8,Sine(i)*8,-FIXAMT,0,50,i,BLT_ENERGY,me->friendly);
		}
		return;	// can't do nothin' right now
	}

	if(me->mind==0)		// when mind=0, charge towards Bouapha
	{
		if(goodguy)
		{
			if(RangeToTarget(me,goodguy)<(256*FIXAMT) && Random(16)==0)
			{
				// get him!
				MakeSound(SND_HAPPYCHOMP,me->x,me->y,SND_CUTOFF,1200);
				me->seq=ANIM_ATTACK;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=128;
				me->action=ACTION_BUSY;
				me->dx=0;
				me->dy=0;
				return;
			}
			FaceGoodguy2(me,goodguy);

			me->dx=Cosine(me->facing*32)*4;
			me->dy=Sine(me->facing*32)*4;
			if(me->seq!=ANIM_MOVE)
			{
				me->seq=ANIM_MOVE;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=128;
			}
			if(Random(10)==0)
			{
				me->mind=1;		// occasionally wander
				me->mind1=(byte)Random(100);
			}
		}
		else
		{
			me->mind=1;	// if there's no goodguy, sit around
			me->mind1=(byte)Random(100);
		}
	}
	else if(me->mind==1)	// random wandering
	{
		if(goodguy && RangeToTarget(me,goodguy)<(256*FIXAMT) && Random(16)==0)
		{
			// get him!
			FaceGoodguy2(me,goodguy);
			MakeSound(SND_HAPPYCHOMP,me->x,me->y,SND_CUTOFF,1200);
			me->seq=ANIM_ATTACK;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
			me->action=ACTION_BUSY;
			me->dx=0;
			me->dy=0;
			return;
		}

		if(!(me->mind1--))	// time to get a new direction
		{
			if((goodguy) && Random(40)==0)
				me->mind=0;	// get back on track
			else
				me->facing=(byte)Random(8);
			me->mind1=Random(40)+1;
		}

		me->dx=Cosine(me->facing*32)*4;
		me->dy=Sine(me->facing*32)*4;
		if(me->seq!=ANIM_MOVE)
		{
			me->seq=ANIM_MOVE;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
		}
	}
}

void AI_StickBat(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y;
	byte f;
	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp==0)
			MakeSound(SND_HAPPYDIE,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_HAPPYOUCH,me->x,me->y,SND_CUTOFF,1200);
	}
	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK)
		{
			x=me->x+Cosine(me->facing*32)*4;
			y=me->y+Sine(me->facing*32)*4;
			f=me->facing*32-8+Random(17);
			FireExactBullet(x,y,me->z,Cosine(f)*8,Sine(f)*8,0,0,24-Random(4),f,BLT_FLAME2,me->friendly);
			MakeSound(SND_FLAMEGO,me->x,me->y,SND_CUTOFF,200);
		}
		return;	// can't do nothin' right now
	}

	if(me->mind==0)		// when mind=0, singlemindedly zip towards Bouapha
	{
		if(goodguy)
		{
			if(RangeToTarget(me,goodguy)<(128*FIXAMT) && Random(4)==0)
			{
				// get him!
				MakeSound(SND_HAPPYCHOMP,me->x,me->y,SND_CUTOFF,1200);
				me->seq=ANIM_ATTACK;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=128;
				me->action=ACTION_BUSY;
				me->dx=0;
				me->dy=0;
				me->reload=0;
				return;
			}

			FaceGoodguy(me,goodguy);

			me->dx=Cosine(me->facing*32)*6;
			me->dy=Sine(me->facing*32)*6;
			if(me->seq!=ANIM_MOVE)
			{
				me->seq=ANIM_MOVE;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=128;
			}
			if(Random(16)==0)
			{
				me->mind=1;		// occasionally wander
				me->mind1=1;
			}
		}
		else
		{
			me->mind=1;	// if there's no goodguy, get random
			me->mind1=1;
		}
	}
	else if(me->mind==1)	// random wandering
	{
		if(!(me->mind1--))	// time to get a new direction
		{
			if((goodguy) && Random(3)==0)
				me->mind=0;	// get back on track
			else
				me->facing=(byte)Random(8);
			me->mind1=Random(40)+1;
		}

		me->dx=Cosine(me->facing*32)*6;
		me->dy=Sine(me->facing*32)*6;
		if(me->seq!=ANIM_MOVE)
		{
			me->seq=ANIM_MOVE;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=110;
		}
	}
}

void DoMove(Guy *me,byte move,int dx,int dy)
{
	me->seq=move;
	me->frm=0;
	me->frmTimer=0;
	me->frmAdvance=128;
	me->action=ACTION_BUSY;
	me->dx=dx;
	me->dy=dy;
}

void AI_Dancer(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_HAPPYOUCH,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_HAPPYDIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->action==ACTION_BUSY)
	{
		return;	// can't do nothin' right now
	}
	switch(me->mind)
	{
		case 0:
		case 12:
			DoMove(me,ANIM_MOVE,-FIXAMT,0);
			break;
		case 1:
		case 2:
		case 4:
		case 8:
		case 10:
			DoMove(me,ANIM_ATTACK,0,0);
			break;
		case 3:
		case 11:
			DoMove(me,ANIM_MOVE,FIXAMT,0);
			break;
		case 5:
		case 9:
			DoMove(me,ANIM_MOVE,0,-FIXAMT);
			break;
		case 6:
		case 7:
			DoMove(me,ANIM_MOVE,0,FIXAMT);
			break;
	}
	me->mind++;
	if(me->mind==13)
		me->mind=0;
}

void AI_Log(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y;
	byte tries;

	goodguy=GetGuyOfAIType(MONS_BOUAPHA);
	if(!goodguy)
		return;
	
	if(me->hp%2==0)
		me->facing=11;
	else
		me->facing=0;
	
	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_DIE)
		{
			x=me->x/FIXAMT;
			y=me->y/FIXAMT;
			BlowUpGuy(x+me->rectx,y+me->recty,x+me->rectx2,y+me->recty2,me->z,1);
		}
		return;
	}
	
	if(me->mind==0)	// waiting to be ridden
	{
		// chill
		if(me->seq!=ANIM_MOVE)
		{
			me->seq=ANIM_MOVE;
			me->frm=0;
			me->frmTimer=0;
		}
		me->frmAdvance=32;
		me->mind2=0;
	}
	else	// being ridden
	{
		if (me->mind2==0)
		me->mind2 = 30;
		else if (me->mind2>1)
		me->mind2--;
		
		goodguy->x=me->x;
		me->dx=goodguy->dx*0.75;
		goodguy->y=(me->y)+12*FIXAMT;
		
		x=me->mapx;
		y=me->mapy;
		y++;
		if(x>=0 && x<map->width && y>=0 && y<map->height &&
						map->map[x+y*map->width].wall==0 &&
						!(GetItem(map->map[x+y*map->width].item)->flags&IF_SOLID) &&
						!(GetTerrain(world,map->map[x+y*map->width].floor)->flags&(TF_SOLID|TF_WATER|TF_LAVA)))
			{
				me->x=(me->mapx*TILE_WIDTH+TILE_WIDTH/2)*FIXAMT;
				me->y=(me->mapy*TILE_HEIGHT+TILE_HEIGHT/2)*FIXAMT;
				me->mind1=0;
				me->mind=0;
				goodguy->dx=0;
				goodguy->dy=0;
				me->dx=0;
				me->dy=0;
				goodguy->y=me->y+(TILE_HEIGHT)*FIXAMT;
				player.vehicle=0;
				return;

			}
		me->dy=(goodguy->dy+(((-0.1*me->mind2)+5.1)*FIXAMT));
		if (me->dy<0)
		me->dy=0;
		if(me->mind2==1)
		me->dx=goodguy->dx/2;
		if(me->mind1&2)
		if(me->mind1)
			me->dx=0;
		me->mind1=0;
	}
	me->frmAdvance=64;
}

//SLEEPLESS HOLLOW

void WanderAI(Guy *me,int wanderRate,int wanderTime,int unWanderRate,Map *map,world_t *world,Guy *goodguy,int speed)
{
	if(me->action==ACTION_BUSY)
		return;

	if(me->mind==0)		// when mind=0, singlemindedly lumber towards Bouapha
	{
		FaceGoodguy(me,goodguy);

		me->dx=(Cosine(me->facing*32)*speed)/FIXAMT;
		me->dy=(Sine(me->facing*32)*speed)/FIXAMT;
		if(me->seq!=ANIM_MOVE)
		{
			me->seq=ANIM_MOVE;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
		}
		if(Random(wanderRate)==0)
		{
			me->mind=1;		// occasionally wander
			me->mind1=1;
		}
	}
	else if(me->mind==1)	// random wandering
	{
		if(!(me->mind1--))	// time to get a new direction
		{
			if((goodguy) && Random(unWanderRate)==0)
				me->mind=0;	// get back on track
			else
				me->facing=(byte)Random(8);
			me->mind1=(byte)Random(wanderTime)+1;
		}

		me->dx=(Cosine(me->facing*32)*speed)/FIXAMT;
		me->dy=(Sine(me->facing*32)*speed)/FIXAMT;
		if(me->seq!=ANIM_MOVE)
		{
			me->seq=ANIM_MOVE;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
		}
	}
}

void AI_Frog(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y,i,j;

	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_FROGOUCH,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_FROGDIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->type==MONS_FROGURT && Random(3)==0)
	{
		x=me->x-10*FIXAMT+Random(21*FIXAMT);
		y=me->y-10*FIXAMT+Random(21*FIXAMT);
		FireBullet(x,y,Random(256),BLT_FLAME5,me->friendly);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm==4 && me->reload==0 && me->aiType==MONS_FROG)
		{
			x=me->x+Cosine(me->facing*32)*30;
			y=me->y+Sine(me->facing*32)*30;
			if(me->AttackCheck(10,x>>FIXSHIFT,y>>FIXSHIFT,goodguy))
			{
				goodguy->GetShot(Cosine(me->facing*32)*4,Sine(me->facing*32)*4,1,map,world);
			}
			else
			{
				x=me->x+Cosine(me->facing*32)*60;
				y=me->y+Sine(me->facing*32)*60;
				if(me->AttackCheck(10,x>>FIXSHIFT,y>>FIXSHIFT,goodguy))
				{
					goodguy->GetShot(Cosine(me->facing*32)*4,Sine(me->facing*32)*4,1,map,world);
				}
			}
			me->reload=5;
		}
		if(me->seq==ANIM_ATTACK && me->frm==4 && me->reload==0 && me->aiType==MONS_FROG2)
		{
			x=me->x+Cosine(me->facing*32)*10;
			y=me->y+Sine(me->facing*32)*10;
			FireBullet(x,y,me->facing*32-10+Random(21),BLT_ENERGY,me->friendly);
			me->reload=40;
		}
		if(me->seq==ANIM_ATTACK && me->frm==2 && me->reload==0 && me->aiType==MONS_FROG3)
		{
			i=(me->facing*32-32+me->frm)&255;
			for(j=-32+me->frm;j<33;j+=me->frm*2)
			{
				x=me->x+Cosine(i)*32;
				y=me->y+Sine(i)*32;
				FireExactBullet(x,y,FIXAMT*20,Cosine(i)*4,Sine(i)*4,0,0,16,i,BLT_SPORE,me->friendly);
				i=(i+me->frm*2)&255;
			}
			me->reload=3;
		}
		if(me->seq==ANIM_DIE && me->aiType==MONS_FROG3 && me->frm==2 && me->reload==0)
		{
			for(i=0;i<256;i+=16)
			FireBullet(me->x,me->y,i,BLT_SPORE,me->friendly);
			me->reload=3;
		}

		return;	// can't do nothin' right now
	}

	if(me->mind==0 && me->type!=MONS_FROGURT)	// heading for Bouapha
	{
		if(RangeToTarget(me,goodguy)<(128*FIXAMT) && Random(8)==0 && me->aiType==MONS_FROG)
		{
			// get him!
			MakeSound(SND_FROGLICK,me->x,me->y,SND_CUTOFF,1200);
			me->seq=ANIM_ATTACK;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
			me->action=ACTION_BUSY;
			me->dx=0;
			me->dy=0;
			me->reload=0;
			return;
		}
		if(RangeToTarget(me,goodguy)<(400*FIXAMT) && Random(8)==0 && me->aiType==MONS_FROG2 && me->reload==0)
		{
			// get him!
			MakeSound(SND_FROGLICK,me->x,me->y,SND_CUTOFF,1200);
			me->seq=ANIM_ATTACK;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=256;
			me->action=ACTION_BUSY;
			me->dx=0;
			me->dy=0;
			return;
		}
		if(RangeToTarget(me,goodguy)<(300*FIXAMT) && Random(8)==0 && me->aiType==MONS_FROG3 && me->reload==0)
		{
			// get him!
			MakeSound(SND_FROGLICK,me->x,me->y,SND_CUTOFF,1200);
			me->seq=ANIM_ATTACK;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=256;
			me->action=ACTION_BUSY;
			me->dx=0;
			me->dy=0;
			return;
		}
	}

	WanderAI(me,64,40,3,map,world,goodguy,FIXAMT*3);
}

void AI_MadCow(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x;

	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_COWOUCH,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_COWDIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_DIE && me->frm>5)
		{
			me->facing=(me->facing+1)&7;
		}
		return;	// can't do nothin' right now
	}

	if(EatHay(me,1))
	{
		me->hp+=5;
		if(me->hp>me->maxHP)
			me->hp=me->maxHP;
		me->mind1=1;
	}

	x=me->maxHP-me->hp+5;	// more damaged=faster

	if(me->mind==0)
	{
		// walking
		if(goodguy && RangeToTarget(me,goodguy)<64*FIXAMT && map->FindGuy(me->mapx,me->mapy,5,goodguy))
		{
			goodguy->GetShot(-4*FIXAMT+Random(8*FIXAMT),-4*FIXAMT+Random(8*FIXAMT),10,map,world);
			me->dx=0;
			me->dy=0;
			me->mind=1;	// switch to turning mode
			me->reload=2;
			me->action=ACTION_BUSY;
			me->seq=ANIM_ATTACK;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
			MakeSound(SND_COWMOO,me->x,me->y,SND_CUTOFF,1200);
			// pick a new direction
			if(Random(2)==0)
			{
				me->mind2=(me->facing+4+Random(3))&7;
				me->mind3=0;
			}
			else
			{
				me->mind2=(me->facing+4-Random(3))&7;
				me->mind3=2;
			}

			return;
		}
		if(me->mind1)
		{
			// bumped a wall
			MakeSound(SND_TRICERBUMP,me->x,me->y,SND_CUTOFF,1200);
			// pick a new direction
			if(Random(2)==0)
			{
				me->mind2=(me->facing+4+Random(3))&7;
				me->mind3=0;
			}
			else
			{
				me->mind2=(me->facing+4-Random(3))&7;
				me->mind3=2;
			}
			me->mind1=0;
			me->dx=0;
			me->dy=0;
			me->mind=1;	// switch to turning mode
			me->reload=2;

			me->seq=ANIM_IDLE;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
		}
		else
		{
			if((me->dx==0 && me->dy==0) || (me->ouch))
			{
				me->dx=Cosine(me->facing*32)*x/5;
				me->dy=Sine(me->facing*32)*x/5;
			}
			if(me->seq!=ANIM_MOVE)
			{
				me->seq=ANIM_MOVE;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=(x/5)*30+40;
			}
		}
	}
	else
	{
		// turning
		if(!me->reload)
		{
			me->facing=(me->facing+me->mind3-1)&7;
			if(me->facing==me->mind2)	// done turning
			{
				me->mind=0;
			}
			me->reload=2;
		}
	}
}

void AI_MadCow2(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y;

	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_COWOUCH,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_COWDIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->hp<me->maxHP)
		me->hp=me->maxHP;

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK)
		{
			x=me->x+Cosine(me->facing*32)*30;
			y=me->y+Sine(me->facing*32)*30;
			FireBullet(x,y,me->facing,BLT_FLAME2,0);
		}

		if(me->seq==ANIM_DIE && me->frm>5)
		{
			me->facing=(me->facing+1)&7;
		}
		return;	// can't do nothin' right now
	}

	if(EatHay(me,2))
	{
		me->type=MONS_MADCOW;
		me->hp=me->maxHP=MonsterHP(MONS_MADCOW);
		me->aiType=MONS_MADCOW;
		strcpy(me->name,"Annoyed Cow");
		MakeSound(SND_TRICERBUMP,me->x,me->y,SND_CUTOFF,1200);
		return;
	}

	if(Random(40)==0)
	{
		me->dx=0;
		me->dy=0;
		me->reload=2;
		me->action=ACTION_BUSY;
		me->seq=ANIM_ATTACK;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=128;
		MakeSound(SND_COWMOO,me->x,me->y,SND_CUTOFF,1200);
		if(Random(2))
			me->facing=(me->facing+1)&7;
		else
			me->facing=(me->facing-1)&7;
		return;
	}
	if(me->seq!=ANIM_MOVE)
	{
		me->seq=ANIM_MOVE;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=128;
	}
}

void AI_PumpkinFly(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int i;

	if(me->reload)
		me->reload--;

	if(me->aiType==MONS_PUMPKINFLY)
		map->DimTorch(me->mapx,me->mapy,2);
	if(me->ouch==4)
	{
		if(me->hp==0)
			MakeSound(SND_PUMPKINDIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_DIE && me->reload==0)
		{
			for(i=0;i<10;i++)
				FireBullet(me->x,me->y,0,BLT_BADSITFLAME,me->friendly);
			me->reload=100;
			if(me->aiType==MONS_PUMPKINFLY2)
				FireBullet(me->x,me->y,0,BLT_BOOM,me->friendly);
		}
		return;	// can't do nothin' right now
	}

	if(me->aiType==MONS_PUMPKINFLY2 && me->mind2<255)
	{
		me->mind2++;
	}

	if(me->aiType==MONS_PUMPKINFLY2 && me->mind1 && me->mind2>10)
	{
		// explode on impact
		SmashTrees(me,2);
		me->hp=1;
		me->GetShot(0,0,1,map,world);
		return;
	}


	if(FindVictim(me->x>>FIXSHIFT,me->y>>FIXSHIFT,32,0,0,0,map,world,me->friendly))
	{
		if(me->aiType==MONS_PUMPKINFLY2)
			SmashTrees(me,2);
		me->hp=1;
		me->GetShot(0,0,1,map,world);
		return;
	}

	if(Random(4)==0)
		FireBullet(me->x,me->y,Random(256),BLT_FLAME3,me->friendly);

	FaceGoodguy(me,goodguy);
	if(!goodguy)
	{
		return;
	}

	me->dx=Cosine(me->facing*32)*4;
	me->dy=Sine(me->facing*32)*4;

	if(me->aiType==MONS_PUMPKINFLY2)
	{
		me->dx*=2;
		me->dy*=2;
	}

	if(me->seq!=ANIM_MOVE)
	{
		me->seq=ANIM_MOVE;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=128;
	}
}

//marshmallow man!
void AI_MarshmallowMan(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y,i;

	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_SKELOUCH,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_SKELDIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK)
		{
			i=(me->facing*32-8+Random(17))&255;
			if(i<0)
				i+=256;
			x=me->x+Cosine(i)*4;
			y=me->y+Sine(i)*4;
			FireExactBullet(x,y,FIXAMT*50,Cosine(i)*8,Sine(i)*8,-FIXAMT,0,50,i,BLT_ENERGY,me->friendly);
		}
		return;	// can't do nothin' right now
	}

	if(me->mind==0)		// when mind=0, charge towards Bouapha
	{
		if(goodguy)
		{
			if(RangeToTarget(me,goodguy)<(256*FIXAMT) && Random(16)==0)
			{
				// get him!
				MakeSound(SND_SKELSHOOT,me->x,me->y,SND_CUTOFF,1200);
				me->seq=ANIM_ATTACK;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=128;
				me->action=ACTION_BUSY;
				me->dx=0;
				me->dy=0;
				return;
			}
			FaceGoodguy2(me,goodguy);

			me->dx=Cosine(me->facing*32)*4;
			me->dy=Sine(me->facing*32)*4;
			if(me->seq!=ANIM_MOVE)
			{
				me->seq=ANIM_MOVE;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=128;
			}
			if(Random(10)==0)
			{
				me->mind=1;		// occasionally wander
				me->mind1=(byte)Random(100);
			}
		}
		else
		{
			me->mind=1;	// if there's no goodguy, sit around
			me->mind1=(byte)Random(100);
		}
	}
	else if(me->mind==1)	// random wandering
	{
		if(goodguy && RangeToTarget(me,goodguy)<(256*FIXAMT) && Random(16)==0)
		{
			// get him!
			FaceGoodguy2(me,goodguy);
			MakeSound(SND_SKELSHOOT,me->x,me->y,SND_CUTOFF,1200);
			me->seq=ANIM_ATTACK;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
			me->action=ACTION_BUSY;
			me->dx=0;
			me->dy=0;
			return;
		}

		if(!(me->mind1--))	// time to get a new direction
		{
			if((goodguy) && Random(40)==0)
				me->mind=0;	// get back on track
			else
				me->facing=(byte)Random(8);
			me->mind1=Random(40)+1;
		}

		me->dx=Cosine(me->facing*32)*4;
		me->dy=Sine(me->facing*32)*4;
		if(me->seq!=ANIM_MOVE)
		{
			me->seq=ANIM_MOVE;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
		}
	}
}

#define PLYR_ACCEL	(FIXAMT)
#define PLYR_DECEL	(FIXAMT*3/4)
#define PLYR_MAXSPD (FIXAMT*6)

void AI_Reflection(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	byte c;
	mapTile_t *mapTile;

	mapTile=&map->map[me->mapx+me->mapy*map->width];

	if(me->reload)
		me->reload--;

	Dampen(&me->dx,PLYR_DECEL);
	Dampen(&me->dy,PLYR_DECEL);

	me->mind1=0;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_BOUAPHAOUCH,me->x,me->y,SND_CUTOFF|SND_ONE,2000);
		else if(me->seq==ANIM_DIE)	// so it doesn't do this if you're drowning
			MakeSound(SND_BOUAPHADIE,me->x,me->y,SND_CUTOFF|SND_ONE,2000);
	}

	c=FakeGetControls();
	DoPlayerFacing(c,me);

	// triggering stuff
	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_A3)
		{
			if(me->frm<11)
			{
				me->z=FIXAMT*8;	// hover while spinning feet in the air before plunging into water
				me->dz=FIXAMT;
			}
			else
			{
				ExplodeParticles(PART_WATER,me->x,me->y,0,16);
			}
		}
		if(me->seq==ANIM_DIE)
		{
			me->facing=(me->facing+1)&7;
		}

		return;
	}

	// not busy, let's see if you want to do something
	/*if((c&CONTROL_B1) && me->reload==0)	// pushed hammer throw button
	{
		me->seq=ANIM_ATTACK;	// even if unarmed
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=255;
		me->reload=5;
		me->action=ACTION_BUSY;
		return;
	}*/
	// if you are moving indeed
	if(c&(CONTROL_UP|CONTROL_DN|CONTROL_LF|CONTROL_RT))
	{
		if(!(GetTerrain(world,mapTile->floor)->flags&TF_ICE))
		{
			me->dx+=Cosine(me->facing*32)*3/2;
			me->dy+=Sine(me->facing*32)*3/2;
			if((Cosine(me->facing*32)<0 && me->dx>0) || (Cosine(me->facing*32)>0 && me->dx<0))
				me->dx=0;
			if((Sine(me->facing*32)<0 && me->dy>0) || (Sine(me->facing*32)>0 && me->dy<0))
				me->dy=0;

		}
		else // ice is slippery
		{
			me->dx+=Cosine(me->facing*32)/4;
			me->dy+=Sine(me->facing*32)/4;
		}

		if(!(GetTerrain(world,mapTile->floor)->flags&TF_MUD))
		{
			Clamp(&me->dx,PLYR_MAXSPD);
			Clamp(&me->dy,PLYR_MAXSPD);
		}
		else
		{
			Clamp(&me->dx,PLYR_MAXSPD/2);
			Clamp(&me->dy,PLYR_MAXSPD/2);

			if(me->z==0)
			{
				SpurtParticles(PART_DIRT,0,me->x,me->y,0,((me->facing*32)+128)&255,8);
				SpurtParticles(PART_DIRT,1,me->x,me->y,0,((me->facing*32)+128)&255,8);
			}
		}

		if(me->seq!=ANIM_MOVE)
		{
			me->seq=ANIM_MOVE;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
		}
	}
	else	// you aren't trying to move
	{
		// ice is slippery
		if(me->z==0 && me->dz==0 && (!(GetTerrain(world,mapTile->floor)->flags&TF_ICE)))
		{
			Dampen(&me->dx,PLYR_DECEL/2);
			Dampen(&me->dy,PLYR_DECEL/2);
		}
		if(me->seq==ANIM_MOVE)
		{
			me->seq=ANIM_IDLE;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
		}
	}
}

void AI_Goodbone(Guy *me,Map *map,world_t *world,Guy *badguy)
{
	byte c,a;
	int i,x,y;
	mapTile_t *mapTile;

	mapTile=&map->map[me->mapx+me->mapy*map->width];

	if(me->reload)
		me->reload--;
	
	if(goodguy && goodguy->hp==0 && me->hp>0)
	{
		me->hp=1;
		me->GetShot(0,0,255,map,world);
	}
	Dampen(&me->dx,PLYR_DECEL);
	Dampen(&me->dy,PLYR_DECEL);

	me->mind1=0;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_SKELOUCH,me->x,me->y,SND_CUTOFF|SND_ONE,2000);
		else if(me->seq==ANIM_DIE)	// so it doesn't do this if you're drowning
			MakeSound(SND_SKELDIE,me->x,me->y,SND_CUTOFF|SND_ONE,2000);
	}

	c=FakeGetControls();
	DoPlayerFacing(c,me);

	// triggering stuff
	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_A3)
		{
			if(me->frm<11)
			{
				me->z=FIXAMT*8;	// hover while spinning feet in the air before plunging into water
				me->dz=FIXAMT;
			}
			else
			{
				ExplodeParticles(PART_WATER,me->x,me->y,0,16);
			}
		}

		if(me->seq==ANIM_ATTACK && me->frm==3 && me->reload==0 && badguy)
		{
			x=me->x+Cosine(me->facing*32)*20;
			y=me->y+Sine(me->facing*32)*20;
			if(me->AttackCheck(24,x>>FIXSHIFT,y>>FIXSHIFT,badguy))
			{
				badguy->GetShot(Cosine(me->facing*32)*4,Sine(me->facing*32)*4,5,map,world);
			}
			me->reload=5;
		}
		if(me->seq==ANIM_A1 && me->frm==3 && me->reload==0 && badguy)
		{
			x=me->x+Cosine(me->facing*32)*24;
			y=me->y+Sine(me->facing*32)*24;
			a=me->facing*32-(3)*5;
			for(i=0;i<3;i++)
			{
				FireBullet(x,y,a,BLT_GREEN,me->friendly);
				a+=10;
			}
			me->reload=10;
			me->mind1=1;
		}
		if(me->seq==ANIM_DIE)
			me->frmAdvance=160;

		return;
	}

	// not busy, let's see if you want to do something

	if((c&CONTROL_B1) && me->reload==0)	// pushed hammer throw button
	{
		MakeSound(SND_SKELSHOOT,me->x,me->y,SND_CUTOFF,1200);
		me->seq=ANIM_A1;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=128;
		me->reload=0;
		me->action=ACTION_BUSY;
		me->dx=0;
		me->dy=0;
		return;
	}
	if(badguy && badguy->hp && RangeToTarget(me,badguy)<40*FIXAMT)
	{
		MakeSound(SND_SKELKICK,me->x,me->y,SND_CUTOFF,1200);
		FaceGoodguy(me,badguy);
		me->seq=ANIM_ATTACK;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=200;
		me->action=ACTION_BUSY;
		me->dx=0;
		me->dy=0;
		me->reload=0;
		return;
	}

	// if you are moving indeed
	if(c&(CONTROL_UP|CONTROL_DN|CONTROL_LF|CONTROL_RT))
	{
		if(!(GetTerrain(world,mapTile->floor)->flags&TF_ICE))
		{
			me->dx+=Cosine(me->facing*32)*3/2;
			me->dy+=Sine(me->facing*32)*3/2;
			if((Cosine(me->facing*32)<0 && me->dx>0) || (Cosine(me->facing*32)>0 && me->dx<0))
				me->dx=0;
			if((Sine(me->facing*32)<0 && me->dy>0) || (Sine(me->facing*32)>0 && me->dy<0))
				me->dy=0;

		}
		else // ice is slippery
		{
			me->dx+=Cosine(me->facing*32)/4;
			me->dy+=Sine(me->facing*32)/4;
		}

		if(!(GetTerrain(world,mapTile->floor)->flags&TF_MUD))
		{
			Clamp(&me->dx,PLYR_MAXSPD);
			Clamp(&me->dy,PLYR_MAXSPD);
		}
		else
		{
			Clamp(&me->dx,PLYR_MAXSPD/2);
			Clamp(&me->dy,PLYR_MAXSPD/2);

			if(me->z==0)
			{
				SpurtParticles(PART_DIRT,0,me->x,me->y,0,((me->facing*32)+128)&255,8);
				SpurtParticles(PART_DIRT,1,me->x,me->y,0,((me->facing*32)+128)&255,8);
			}
		}

		if(me->seq!=ANIM_MOVE)
		{
			me->seq=ANIM_MOVE;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
		}
	}
	else	// you aren't trying to move
	{
		// ice is slippery
		if(me->z==0 && me->dz==0 && (!(GetTerrain(world,mapTile->floor)->flags&TF_ICE)))
		{
			Dampen(&me->dx,PLYR_DECEL/2);
			Dampen(&me->dy,PLYR_DECEL/2);
		}
		if(me->seq==ANIM_MOVE)
		{
			me->seq=ANIM_IDLE;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
		}
	}
}

void AI_Darkness(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	if(me->hp>0)
		map->BrightTorch(me->mapx,me->mapy,-16,2);

	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp==0)
			MakeSound(SND_PUMPKINDIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_DIE && me->reload==0)
		{
			map->BrightTorch(me->mapx,me->mapy,10,6);
			me->reload=100;
		}
		return;	// can't do nothin' right now
	}

	if(me->reload==0 && FindVictim(me->x>>FIXSHIFT,me->y>>FIXSHIFT,32,Random(FIXAMT*2),Random(FIXAMT*2),1,map,world,me->friendly))
	{
		MakeSound(SND_ALIENOUCH,me->x,me->y,SND_CUTOFF,200);
		me->reload=2;
	}

	if(Random(4)==0)
		me->facing=(me->facing-1+Random(3))&7;

	me->dx=Cosine(me->facing*32)*4;
	me->dy=Sine(me->facing*32)*4;
}

void AI_Spark(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	byte wall[8];
	int i,c;

	if(me->reload)
		me->reload--;

	if(Random(2))
		ExplodeParticlesColor(7,me->x,me->y,me->z,1,1);

	// find out which sides have walls
	if(me->mapx<map->width-1)
	{
		wall[0]=(map->map[me->mapx+1+me->mapy*map->width].wall>0);
		if(me->mapy<map->height-1)
			wall[1]=(map->map[me->mapx+1+(me->mapy+1)*map->width].wall>0);
		else
			wall[1]=1;
		if(me->mapy>0)
			wall[7]=(map->map[me->mapx+1+(me->mapy-1)*map->width].wall>0);
		else
			wall[7]=1;
	}
	else
		wall[0]=1;
	if(me->mapy<map->height-1)
		wall[2]=(map->map[me->mapx+(me->mapy+1)*map->width].wall>0);
	else
		wall[2]=1;
	if(me->mapx>0)
	{
		wall[4]=(map->map[me->mapx-1+me->mapy*map->width].wall>0);
		if(me->mapy<map->height-1)
			wall[3]=(map->map[me->mapx-1+(me->mapy+1)*map->width].wall>0);
		else
			wall[3]=1;
		if(me->mapy>0)
			wall[5]=(map->map[me->mapx-1+(me->mapy-1)*map->width].wall>0);
		else
			wall[5]=1;
	}
	else
		wall[4]=1;
	if(me->mapy>0)
		wall[6]=(map->map[me->mapx+(me->mapy-1)*map->width].wall>0);
	else
		wall[6]=1;

	// shock neighboring walls for effect
	if(Random(3)==0)
	{
		if(wall[0])
			LightningBolt(me->x,me->y-me->z,((me->mapx+1)*TILE_WIDTH+TILE_WIDTH/2)*FIXAMT,(me->mapy*TILE_HEIGHT+Random(TILE_HEIGHT))*FIXAMT-FIXAMT*20);
		if(wall[2])
			LightningBolt(me->x,me->y-me->z,(me->mapx*TILE_WIDTH+Random(TILE_WIDTH))*FIXAMT,((me->mapy+1)*TILE_HEIGHT+TILE_HEIGHT/2)*FIXAMT-FIXAMT*20);
		if(wall[4])
			LightningBolt(me->x,me->y-me->z,((me->mapx-1)*TILE_WIDTH+TILE_WIDTH/2)*FIXAMT,(me->mapy*TILE_HEIGHT+Random(TILE_HEIGHT))*FIXAMT-FIXAMT*20);
		if(wall[6])
			LightningBolt(me->x,me->y-me->z,(me->mapx*TILE_WIDTH+Random(TILE_WIDTH))*FIXAMT,((me->mapy-1)*TILE_HEIGHT+TILE_HEIGHT/2)*FIXAMT-FIXAMT*20);
	}

	if(goodguy && RangeToTarget(me,goodguy)<48*FIXAMT && me->reload==0)
	{
		me->reload=20;
		LightningBolt(me->x,me->y-me->z,goodguy->x,goodguy->y-goodguy->z-FIXAMT*25);
		goodguy->GetShot(0,0,1,map,world);
	}

	if(me->mind==1)	// just entered a new space
	{
		me->mind1++;
		if(me->mind1==50)
		{
			me->type=MONS_NONE;
			return;
		}
		c=0;
		for(i=0;i<8;i++)
			if(wall[i])
				c++;

		if(c==0)	// no walls neighboring at all!
		{
			//me->type=MONS_NONE;
			//return;
		}

		me->mind=0;

		if(wall[(me->facing+2)&7])	// huggable wall
		{
			if(wall[(me->facing)])	// but forward motion blocked
			{
				if(wall[(me->facing+6)&7])	// and wrong-way blocked
					me->facing=(me->facing+4)&7;	// so turn around
				else
					me->facing=(me->facing+6)&7;
			}
			else
				me->facing=me->facing;
		}
		else
		{
			// huggable wall is missing
			if(wall[(me->facing+3)&7])
				me->facing=(me->facing+2)&7;	// can cornerhug
			else if(wall[(me->facing+6)&7])	// can hold onto opposite wall
			{
				if(wall[(me->facing)])	// but forward motion blocked
				{
					if(wall[(me->facing+2)&7])	// and right-way blocked
						me->facing=(me->facing+4)&7;	// so turn around
					else
						me->facing=(me->facing+2)&7;
				}
				else
					me->facing=me->facing;
			}
			else if(wall[(me->facing+5)&7])	// can't do that either, but can cornerhug the wrong way
				me->facing=(me->facing+6)&7;
			else if(wall[(me->facing)&7])	// headed straight into a wall
				me->facing=(me->facing+2)&7;
			else
			{
				//me->type=MONS_NONE;
				//return;	// nothing to hug in any direction!
			}
		}
	}
	me->dx=Cosine(me->facing*32)*2;
	me->dy=Sine(me->facing*32)*2;
	if(me->facing==0 || me->facing==4)
	{
		if(me->y<(me->mapy*TILE_HEIGHT+TILE_HEIGHT/2)*FIXAMT)
			me->dy+=FIXAMT;
		else
			me->dy-=FIXAMT;
	}
	if(me->facing==2 || me->facing==6)
	{
		if(me->x<(me->mapx*TILE_WIDTH+TILE_WIDTH/2)*FIXAMT)
			me->dx+=FIXAMT;
		else
			me->dx-=FIXAMT;
	}
}

void AI_LightSwitch(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	if(me->seq==ANIM_ATTACK)
	{
		if(me->frm>7)
			me->frm=7;
		map->DimTorch(me->mapx,me->mapy,3);
		//if(Random(3)==0)
			map->BrightTorch(me->mapx,me->mapy,16,2);
	}
}

void AI_LightBlaster(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y;
	byte a;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_ROBOOUCH,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_ROBODIE,me->x,me->y,SND_CUTOFF,1200);
	}
	if(me->reload)
		me->reload--;

	if(me->seq==ANIM_DIE)
	{
		x=me->x>>FIXSHIFT;
		y=me->y>>FIXSHIFT;
		BlowUpGuy(x+me->rectx,y+me->recty,x+me->rectx2,y+me->recty2,me->z,1);
	}
	if(me->seq==ANIM_ATTACK)
	{
		if(me->frm==7 && me->reload==0)
		{
			MakeSound(SND_ROBOSHOOT,me->x,me->y,SND_CUTOFF,1200);
			me->frmAdvance=128;
			a=AngleFrom(me->x,me->y,goodguy->x,goodguy->y);
			FireExactBullet(me->x,me->y,FIXAMT*30,Cosine(a)*10,Sine(a)*10,0,0,60,a,BLT_BIGSHELL,me->friendly);
			FireExactBullet(me->x,me->y,FIXAMT*30,Cosine(a-4)*10,Sine(a-4)*10,0,0,60,a,BLT_BIGSHELL,me->friendly);
			FireExactBullet(me->x,me->y,FIXAMT*30,Cosine(a+4)*10,Sine(a+4)*10,0,0,60,a,BLT_BIGSHELL,me->friendly);

			map->BrightTorch(me->mapx,me->mapy,16,2);
			me->reload=10;
		}
		return;
	}
	if(me->seq==ANIM_IDLE)
	{
		if(RangeToTarget(me,goodguy)<FIXAMT*500)
		{
			me->seq=ANIM_ATTACK;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=20;
		}
	}
}

void AI_LightSlide(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int cx,cy;
	Guy *g;

	if(me->mind1>2)
		me->mind1-=3;

	if(me->mind1>0)
	{
		map->BrightTorch(me->mapx,me->mapy,(me->mind1/20),2);
		me->seq=ANIM_ATTACK;
		me->frm=(me->mind1/10);
		if(me->mind1>=98)
		{
			me->frm=0;
			g=AddGuy(me->x,me->y,0,MONS_SPARK,me->friendly);
			if(g)
			{
				g->item=ITM_NONE;
				g->facing=6;
				g->dx=0;
				g->dy=-2*FIXAMT;
			}
			me->mind1=0;
		}
		me->frmTimer=0;
		me->frmAdvance=0;
	}
	else
	{
		me->seq=ANIM_IDLE;
		me->frm=0;
		me->frmTimer=0;
	}

	SpreadCharge(me);

	if(me->mind==1)	// sliding
	{
		cx=(me->mapx*TILE_WIDTH+TILE_WIDTH/2)*FIXAMT;
		cy=(me->mapy*TILE_HEIGHT+TILE_HEIGHT/2)*FIXAMT;

		if(me->dx>0 && me->x>=cx && (me->mapx>=map->width-1 || !Walkable(me,me->mapx+1,me->mapy,map,world)))
		{
			me->x=cx;
			me->y=cy;
			me->dx=me->dy=0;
			me->mind=0;	// rest
		}
		if(me->dx<0 && me->x<=cx && (me->mapx<=0 || !Walkable(me,me->mapx-1,me->mapy,map,world)))
		{
			me->x=cx;
			me->y=cy;
			me->dx=me->dy=0;
			me->mind=0;	// rest
		}
		if(me->dy>0 && me->y>=cy && (me->mapy>=map->height-1 || !Walkable(me,me->mapx,me->mapy+1,map,world)))
		{
			me->x=cx;
			me->y=cy;
			me->dx=me->dy=0;
			me->mind=0;	// rest
		}
		if(me->dy<0 && me->y<=cy && (me->mapy<=0 || !Walkable(me,me->mapx,me->mapy-1,map,world)))
		{
			me->x=cx;
			me->y=cy;
			me->dx=me->dy=0;
			me->mind=0;	// rest
		}
		if(me->dx==0 && me->dy==0)
			me->mind=0;
	}
}

void AI_BigHead(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y;

	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_MUSHOUCH,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_MUSHDIE,me->x,me->y,SND_CUTOFF,1200);

	}

	me->z=FIXAMT*55;
	me->dz=0;

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK)
		{
			if(me->frm<4)
				me->z-=FIXAMT*5*me->frm;
			else
				me->z-=FIXAMT*5*(7-me->frm);

			if(me->frm==3 && me->reload==0 && me->aiType==MONS_BIGHEAD1)
			{
				FireExactBullet(me->x,me->y,me->z,0,FIXAMT*4,FIXAMT*8,0,120,64,BLT_GRENADE,me->friendly);
				me->reload=45;
			}
			if(me->aiType==MONS_BIGHEAD3)
			{
				x=AngleFrom(me->x,me->y,goodguy->x,goodguy->y);
				FireExactBullet(me->x,me->y,me->z,Cosine(x)*8,Sine(x)*8,-FIXAMT/2,0,60,x,BLT_BIGSHELL,me->friendly);
				me->reload=60;
			}
		}

		if(me->seq==ANIM_DIE)
		{
			x=me->x>>FIXSHIFT;
			y=me->y>>FIXSHIFT;
			BlowUpGuy(x+me->rectx,y+me->recty,x+me->rectx2,y+me->recty2,me->z,1);
			BlowSmoke((x+me->rectx+Random(me->rectx2-me->rectx))<<FIXSHIFT,
					  (y+me->recty+Random(me->recty2-me->recty))<<FIXSHIFT,
					  me->z,FIXAMT);
		}
		return;
	}

	if(me->mind==0)
	{
		if(goodguy && (RangeToTarget(me,goodguy)<400*FIXAMT || me->ouch>0))
		{
			me->mind=1;
		}
	}
	if(me->mind==1)	// active
	{
		if(goodguy)
		{
			// calculate desired location (want to be above Bouapha)
			x=goodguy->x;
			y=goodguy->y-96*FIXAMT;

			FaceGoodguy2(me,goodguy);
			if(me->x<x)
				me->dx+=FIXAMT/4;
			if(me->y<y)
				me->dy+=FIXAMT/4;
			if(me->x>x)
				me->dx-=FIXAMT/4;
			if(me->y>y)
				me->dy-=FIXAMT/4;

			Clamp(&me->dx,FIXAMT*5);
			Clamp(&me->dy,FIXAMT*5);
		}

		if(RangeToTarget(me,goodguy)<300*FIXAMT && goodguy->y>me->y && me->reload==0)
		{
			me->seq=ANIM_ATTACK;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
			me->action=ACTION_BUSY;
			return;
		}
	}

	if(me->seq!=ANIM_IDLE)
	{
		me->seq=ANIM_IDLE;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=128;
	}
}

void AI_BigHeadBod(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	if(me->parent)
	{
		if(me->parent->hp==0 || me->parent->type==MONS_NONE)
			me->type=MONS_NONE;	// gone!

		me->x=me->parent->x;
		me->y=me->parent->y-1;
		me->z=0;
		if(me->parent->dx<FIXAMT)
		{
			me->facing=3;
		}
		else if(me->parent->dx>FIXAMT)
		{
			me->facing=1;
		}
		else
		{
			me->facing=2;
		}
	}
	else
		me->type=MONS_NONE;

	if(me->seq!=ANIM_MOVE)
	{
		me->seq=ANIM_MOVE;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=128;
	}
}

void AI_BigHead2(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y;

	me->facing=(me->mind2/32);

	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_MUSHOUCH,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_MUSHDIE,me->x,me->y,SND_CUTOFF,1200);

	}

	me->z=FIXAMT*55;
	me->dz=0;

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK)
		{
			if(me->frm<4)
				me->z-=FIXAMT*5*me->frm;
			else
				me->z-=FIXAMT*5*(7-me->frm);

			if(me->frm==3 && me->reload==0)
			{
				FireExactBullet(me->x,me->y,me->z,0,FIXAMT*4,FIXAMT*8,0,120,64,BLT_GRENADE,me->friendly);
				me->reload=45;
			}
		}

		if(me->seq==ANIM_DIE)
		{
			x=me->x>>FIXSHIFT;
			y=me->y>>FIXSHIFT;
			BlowUpGuy(x+me->rectx,y+me->recty,x+me->rectx2,y+me->recty2,me->z,1);
			BlowSmoke((x+me->rectx+Random(me->rectx2-me->rectx))<<FIXSHIFT,
					  (y+me->recty+Random(me->recty2-me->recty))<<FIXSHIFT,
					  me->z,FIXAMT);
		}
		return;
	}

	if(RangeToTarget(me,goodguy)<90*FIXAMT)
	{
		goodguy->GetShot(0,0,4,map,world);
		me->mind=1;
		me->mind2+=128;
	}

	if(me->mind==0)
	{
		if(goodguy && (RangeToTarget(me,goodguy)<400*FIXAMT || me->ouch>0))
		{
			me->mind=Random(3)+1;
			me->mind1=0;
			me->mind2=Random(256);
		}
	}
	switch(me->mind)
	{
		case 1:	// go straight
			if(me->mind1)
			{
				me->mind2+=128;	// go the opposite way
				me->mind=2+Random(3);	// and be in one of the turny modes
			}
			else
			{
				if(Random(30)==0)
					me->mind=Random(4)+1;	// change modes sometimes
			}
			break;
		case 2:	// turning left
			if(me->mind1)
			{
				me->mind2+=128;	// go the opposite way
				me->mind=2+Random(3);	// and be in one of the turny modes
			}
			me->mind2-=4;
			if(Random(30)==0)
				me->mind=Random(4)+1;	// change modes sometimes
			break;
		case 3:	// turning right
			if(me->mind1)
			{
				me->mind2+=128;	// go the opposite way
				me->mind=2+Random(3);	// and be in one of the turny modes
			}
			me->mind2+=4;
			if(Random(30)==0)
				me->mind=Random(4)+1;	// change modes sometimes
			break;
		case 4: // chasing
			if(me->mind1)
			{
				me->mind2+=128;	// go the opposite way
				me->mind=2+Random(3);	// and be in one of the turny modes
			}
			byte a=AngleFrom(me->x,me->y,goodguy->x,goodguy->y);
			me->mind2=TurnToward(me->mind2,a,4);
			if(Random(30)==0)
				me->mind=Random(4)+1;	// change modes sometimes
			break;
	}
	me->mind1=0;
	me->dx=Cosine(me->mind2)*7;
	me->dy=Sine(me->mind2)*7;
	if(Random(3)==0)
	{
		byte a;

		a=Random(256);
		FireExactBullet(me->x,me->y,FIXAMT*10,Cosine(a)*2,Sine(a)*2,FIXAMT*-1,0,30*6,0,BLT_BADSITFLAME,me->friendly);
	}
}

void AI_ScaredyBat(Guy *me,Map *map,world_t *world,Guy *badguy)
{
	int x;

	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp==0)
			MakeSound(SND_BATOUCH,me->x,me->y,SND_CUTOFF,1200);
	}
	if(me->action==ACTION_BUSY)
	{
		return;	// can't do nothin' right now
	}

	if(!goodguy)
		return;

	if(map->FindGuy(me->mapx,me->mapy,10,goodguy) && RangeToTarget(me,goodguy)<128*FIXAMT)
	{
		x=AngleFrom(me->x,me->y,goodguy->x,goodguy->y);
		x=(x+128)&255;	// face away!
		me->dx=Cosine(x)*8;
		me->dy=Sine(x)*8;
	}
	else if(badguy && map->FindGuy(me->mapx,me->mapy,10,badguy) && RangeToTarget(me,badguy)<128*FIXAMT)
	{
		x=AngleFrom(me->x,me->y,badguy->x,badguy->y);
		x=(x+128)&255;	// face away!
		me->dx=Cosine(x)*8;
		me->dy=Sine(x)*8;
	}
	else	// nobody too near, settle
	{
		me->dx=(me->dx*3)/4;
		me->dy=(me->dy*3)/4;
		if(me->z>0)
			me->dz-=FIXAMT;
		else
			me->dz=-FIXAMT;
	}

	if(badguy && RangeToTarget(me,badguy)<RangeToTarget(me,goodguy) && map->FindGuy(me->mapx,me->mapy,10,badguy))
		FaceGoodguy(me,badguy);
	else if(map->FindGuy(me->mapx,me->mapy,10,goodguy))
		FaceGoodguy(me,goodguy);

	if(me->dx==0 && me->dy==0)
	{
		if(me->z>FIXAMT)
		{
			if(me->frmAdvance>64)
				me->frmAdvance-=4;
			else
				me->frmAdvance+=4;
		}
		else
			if(me->frmAdvance>32)
				me->frmAdvance--;
			else if(me->frm==2)
				me->frmAdvance=0;
	}
	else
		me->frmAdvance=128;

	if(me->seq!=ANIM_MOVE)
	{
		me->seq=ANIM_MOVE;
		me->frm=0;
		me->frmTimer=0;
	}
}

void AI_Tree(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x;

	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_PYGMYOUCH,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_PYGMYDIE,me->x,me->y,SND_CUTOFF,1200);
		if(me->seq==ANIM_IDLE)
		{
			me->seq=ANIM_A1;
			me->frmTimer=0;
			me->frm=0;
			me->frmAdvance=32;
		}
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_DIE && me->frm==7)
		{
			for(x=0;x<16;x++)
				FireBullet(me->x,me->y,x*16,BLT_ENERGY,me->friendly);
			BlowUpGuy(me->x,me->y,me->x+1,me->y+1,0,1);
		}
		return;	// can't do nothin' right now
	}

	me->mind++;
	if(me->mind>=4)
	{
		me->mind=0;
		MakeSound(SND_BULLETFIRE,me->x,me->y,SND_CUTOFF,100);
		x=AngleFrom(me->x,me->y,goodguy->x,goodguy->y);
		FireExactBullet(me->x,me->y,FIXAMT*90,Cosine(x)*6,Sine(x)*6,-FIXAMT*2,0,90,(byte)x,BLT_ENERGY,me->friendly);
	}
}

void AI_Horseradish(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_MUSHOUCH,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_MUSHDIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->hp<100)
		me->hp=10000;

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm==2 && me->reload==0 && goodguy)
		{

			AddBaby(me->x,me->y,FIXAMT*80,MONS_PUMPKINFLY2,me);
			me->reload=5;
		}
		return;	// can't do nothin' right now
	}

	if(goodguy && RangeToTarget(me,goodguy)<48*FIXAMT)
		goodguy->GetShot(0,0,2,map,world);

	if(me->mind==0)
	{
		if(goodguy && RangeToTarget(me,goodguy)<600*FIXAMT)
			me->mind=2;
	}
	else if(me->mind==2)		// when mind=0, singlemindedly drift towards Bouapha
	{
		if(goodguy)
		{
			if(me->reload==0 && Random(50)==0)
			{
				MakeSound(SND_VAMPATTK,me->x,me->y,SND_CUTOFF,800);
				me->seq=ANIM_ATTACK;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=160;
				me->action=ACTION_BUSY;
				me->dx=me->dy=0;
				return;
			}
			FaceGoodguy(me,goodguy);

			me->dx=Cosine(me->facing*32)*6;
			me->dy=Sine(me->facing*32)*6;
			if(me->seq!=ANIM_MOVE)
			{
				me->seq=ANIM_MOVE;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=180;
			}
			if(me->mind1>0 && RangeToTarget(me,goodguy)>(64*FIXAMT))
			{
				me->mind=1;	// bumped a wall, so randomly maneuver
				me->facing=(byte)Random(8);
				me->mind2=10;
			}
		}
		else
		{
			me->mind=1;	// if there's no goodguy, get random
			me->facing=(byte)Random(8);
		}
	}
	else if(me->mind==1)	// random wandering
	{
		if(me->reload==0 && Random(50)==0)
		{
			MakeSound(SND_VAMPATTK,me->x,me->y,SND_CUTOFF,800);
			me->seq=ANIM_ATTACK;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=160;
			me->action=ACTION_BUSY;
			FaceGoodguy(me,goodguy);
			me->mind=2;
			me->dx=me->dy=0;
			return;
		}
		if(!(me->mind2--))	// time to get a new direction
		{
			me->mind=2;	// get back on track
		}

		me->dx=Cosine(me->facing*32)*6;
		me->dy=Sine(me->facing*32)*6;
		if(me->seq!=ANIM_MOVE)
		{
			me->seq=ANIM_MOVE;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
		}
	}
	else
	{
		me->mind=2;
		return;
	}

	me->mind1=0;
}

void AI_Sparky(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_WOLFOUCH,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_WOLFDIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_DIE)
			me->frmAdvance=190;
		return;	// can't do nothin' right now
	}

	if(RangeToTarget(me,goodguy)<FIXAMT*100 && me->reload==0)
	{
		me->reload=8;
		LightningBolt(me->x,me->y-FIXAMT*30,goodguy->x,goodguy->y-FIXAMT*30);
		goodguy->GetShot(Cosine(me->mind3+64)*10,Sine(me->mind3+64)*10,4,map,world);
	}
	else if(me->reload==0)
	{
		me->reload=8;
		LightningBolt(me->x,me->y-FIXAMT*30,me->x-FIXAMT*15+Random(FIXAMT*30+1),me->y-FIXAMT*30-FIXAMT*10+Random(FIXAMT*20+1));
	}

	if(me->mind==0)
	{
		if(me->ouch || RangeToTarget(me,goodguy)<200*FIXAMT)
		{
			me->mind=1;
			me->mind3=Random(256);
			me->facing=2;
			// howl
			MakeSound(SND_WOLFHOWL,me->x,me->y,SND_CUTOFF,1200);
			me->seq=ANIM_A3;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
			me->action=ACTION_BUSY;
			me->dx=0;
			me->dy=0;
			me->reload=0;
			return;
		}
		return;
	}

	if(me->mind1&1)
	{
		if(me->dx>0)
			me->mind3=64+Random(128);
		else
			me->mind3=192+Random(128);
	}
	if(me->mind1&2)
	{
		if(me->dy>0)
			me->mind3=128+Random(128);
		else
			me->mind3=Random(128);
	}
	me->mind3=TurnToward(me->mind3,AngleFrom(me->x,me->y,goodguy->x,goodguy->y),4);

	me->facing=me->mind3/32;
	me->dx=Cosine(me->mind3)*5;
	me->dy=Sine(me->mind3)*5;
	me->mind1=0;

	if(me->seq==ANIM_MOVE)
		me->frmAdvance=128;

	if(me->seq!=ANIM_MOVE)
	{
		me->seq=ANIM_MOVE;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=256;
	}
}

void AI_Grue(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y,i;
	byte f;

	if(!goodguy)
		return;

	// heal in darkness
	if(map->map[me->mapx+me->mapy*map->width].templight<-3)
	{
		if(me->hp<me->maxHP)
		{
			i=me->hp;
			me->hp+=5;
			if(me->hp>me->maxHP)
				me->hp=me->maxHP;
			ShowEnemyLife(me->name,i*128/me->maxHP,me->hp*128/me->maxHP,1);
		}
	}

	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_SPD3OUCH,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_SPD3DIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_DIE)
		{
			if(Random(2))
			{
				AddBaby(me->x+((-8+Random(17))<<FIXSHIFT),
						me->y+((-8+Random(17))<<FIXSHIFT),0,MONS_SPIDER,me);
			}
		}
		if(me->seq==ANIM_A1 && me->frm==2 && me->reload==0 && goodguy)
		{
			FaceGoodguy(me,goodguy);
			x=me->x+Cosine(me->facing*32)*32;
			y=me->y+Sine(me->facing*32)*32;
			if(me->AttackCheck(32,x>>FIXSHIFT,y>>FIXSHIFT,goodguy))
			{
				goodguy->GetShot(Cosine(me->facing*32)*12,Sine(me->facing*32)*12,10,map,world);
				me->reload=2;
			}
		}
		return;	// can't do nothin' right now
	}

	if(me->mind1)
		me->mind1--;

	if(map->map[goodguy->mapx+goodguy->mapy*map->width].templight<-8)	// if you are in the dark, you are good eats
	{
		me->mind=2;	// dash at you
		me->mind1=3;
	}

	switch(me->mind)
	{
		case 0:	// normal heading towards you
			FaceGoodguy(me,goodguy);
			if(me->mind1==0)
			{
				me->mind=1;
				me->facing=Random(8);
				me->mind1=10+Random(40);
			}
			me->dx=Cosine(me->facing*32)*2;
			me->dy=Sine(me->facing*32)*2;
			me->frmAdvance=128;
			break;
		case 1:	// normal wandering
			if(me->mind1==0)
			{
				me->mind=0;
				me->mind1=10+Random(90);
			}
			me->dx=Cosine(me->facing*32)*2;
			me->dy=Sine(me->facing*32)*2;
			me->frmAdvance=128;
			break;
		case 2:	// dashing at you
			if(me->mind1==0)
			{
				me->mind=1;
				me->mind1=10+Random(40);
			}
			f=AngleFrom(me->x,me->y,goodguy->x,goodguy->y);
			me->facing=f/32;
			me->dx=Cosine(f)*8;
			me->dy=Sine(f)*8;
			me->frmAdvance=200;
			break;
	}

	if(!me->poison)
	{
		if(Random(100)==0)
			AddBaby(me->x+((-8+Random(17))<<FIXSHIFT),me->y+((-8+Random(17))<<FIXSHIFT),0,MONS_SPIDER,me);
		if(Random(300)==0)
			AddBaby(me->x+((-8+Random(17))<<FIXSHIFT),me->y+((-8+Random(17))<<FIXSHIFT),0,MONS_BIGSPDR,me);
	}

	if(goodguy)
	{

		x=RangeToTarget(me,goodguy);
		if(x<(128*FIXAMT) && Random(16)==0 && me->reload==0)
		{
			MakeSound(SND_SPD3CHOMP,me->x,me->y,SND_CUTOFF,1200);
			FaceGoodguy(me,goodguy);
			// bite him
			me->seq=ANIM_A1;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=200;
			me->action=ACTION_BUSY;
			me->dx=0;
			me->dy=0;
			me->reload=1;
			return;
		}
	}

	if(me->mind!=2)	// when dashing, he ignores light, but otherwise wants to avoid it
	{
		x=(me->x+me->dx)/(TILE_WIDTH*FIXAMT);
		y=(me->y+me->dy)/(TILE_HEIGHT*FIXAMT);
		if(map->map[me->mapx+me->mapy*map->width].templight<-5 &&
			map->map[x+y*map->width].templight>=-5)	// you are in darkness now, but wouldn't be on moving
		{
			me->dx=0;
			me->dy=0;
			me->seq=ANIM_IDLE;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
			return;
		}
		if(map->map[me->mapx+me->mapy*map->width].templight>=-5)	// if you are in light at any time, run
		{
			me->dx=(me->dx*3)/2;
			me->dy=(me->dy*3)/2;
			me->frmAdvance=160;
		}
	}

	if(me->seq!=ANIM_MOVE)
	{
		me->seq=ANIM_MOVE;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=128;
	}
}

void AI_Spook(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y,i;

	if(me->reload)
		me->reload--;
	
	if(me->mind4)
		me->mind4--;

	if(me->type==MONS_SPOOKLEY2)
	{
		if(!me->mind4)
		me->type=MONS_SPOOKLEY;
		// heal in ghost form
		if(me->hp<me->maxHP && me->mind2==0)
		{
			i=me->hp;
			me->hp++;
			if(me->hp>me->maxHP)
				me->hp=me->maxHP;
			ShowEnemyLife(me->name,i*128/me->maxHP,me->hp*128/me->maxHP,1);
		}
		map->DimTorch(me->mapx,me->mapy,6);
	}
	else
	{
		if(me->hp<(int)me->mind3)	// been hurt too much, ghost out
		{
			me->type=MONS_SPOOKLEY2;
			me->mind4=200+Random(8)*10;
			MakeSound(SND_GHOSTTPORT,me->x,me->y,SND_CUTOFF,2000);
			map->BrightTorch(me->mapx,me->mapy,16,6);
			if(me->mind3>50)
				me->mind3-=50;
			else
				me->mind3=0;
		}
	}
	if(me->dx==0 && me->dy==0)
	{
		me->dx=FIXAMT*4;
		me->dy=FIXAMT*4;
	}

	if(me->mind1&1)
		me->dx=-me->dx;
	if(me->mind1&2)
		me->dy=-me->dy;
	me->mind1=0;

	if(me->ouch==4)
	{
		if(me->hp==0)
			MakeSound(SND_POLTERDIE,me->x,me->y,SND_CUTOFF,2000);
		else
			MakeSound(SND_POLTEROUCH,me->x,me->y,SND_CUTOFF,2000);
	}

	if(me->mind2==0)
	{
		me->mind2=5;
		FaceGoodguy(me,goodguy);
		x=me->facing*32+32-Random(65);
		if(x<0)
			x+=256;
		if(x>255)
			x-=256;
		FireBullet(me->x,me->y,x,BLT_ACID,me->friendly);
	}
	else
		me->mind2--;

	if(Random(10)==0)
		StinkySteam(me->x-FIXAMT*20+Random(FIXAMT*40),me->y-FIXAMT*20+Random(FIXAMT*40),
					me->z+FIXAMT*40,FIXAMT*2);
	if(Random(100)==0)
		AddBaby(me->x,me->y,me->z,MONS_GHOST,me);

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_DIE)
		{
			x=me->x>>FIXSHIFT;
			y=me->y>>FIXSHIFT;
			BlowSmoke((x+me->rectx+Random(me->rectx2-me->rectx))<<FIXSHIFT,
					  (y+me->recty+Random(me->recty2-me->recty))<<FIXSHIFT,
					  me->z,FIXAMT);
		}
		return;
	}

	if(goodguy)
	{
		if(me->AttackCheck(70,me->x>>FIXSHIFT,me->y>>FIXSHIFT,goodguy) && goodguy->poison==0)
		{
			MakeSound(SND_THINGVOMIT,me->x,me->y,SND_CUTOFF,1200);
			goodguy->poison=30;
		}
	}
	if(me->seq!=ANIM_IDLE)
	{
		me->seq=ANIM_IDLE;
		me->frm=0;
		me->frmTimer=0;
	}
}

void AI_Speedy(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	byte a;

	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_WOLFOUCH,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_WOLFDIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_DIE)
			me->frmAdvance=190;
		return;	// can't do nothin' right now
	}

	if(me->mind==0 && (me->ouch || RangeToTarget(me,goodguy)<400*FIXAMT))
	{
		me->mind=1;
		me->mind3=Random(256);
		me->facing=2;
		// howl
		MakeSound(SND_WOLFHOWL,me->x,me->y,SND_CUTOFF,1200);
		me->seq=ANIM_A3;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=128;
		me->action=ACTION_BUSY;
		me->dx=0;
		me->dy=0;
		me->reload=0;
		return;
	}

	if((me->mind1))	// shoot when you hit a wall
	{
		a=AngleFrom(me->x,me->y,goodguy->x,goodguy->y);
		FireExactBullet(me->x,me->y,me->z+FIXAMT*20,Cosine(a)*6,Sine(a)*6,0,0,60,a,BLT_BIGSHELL,me->friendly);
	}
	if(RangeToTarget(me,goodguy)<FIXAMT*30)
		goodguy->GetShot(Cosine(me->mind3+64)*10,Sine(me->mind3+64)*10,4,map,world);
	if(me->mind1&1)
	{
		if(me->dx>0)
			me->mind3=64+Random(128);
		else
			me->mind3=192+Random(128);
	}
	if(me->mind1&2)
	{
		if(me->dy>0)
			me->mind3=128+Random(128);
		else
			me->mind3=Random(128);
	}
	me->facing=me->mind3/32;
	me->dx=Cosine(me->mind3)*16;
	me->dy=Sine(me->mind3)*16;
	me->mind1=0;

	if(me->seq==ANIM_MOVE)
		me->frmAdvance=256;

	if(me->seq!=ANIM_MOVE)
	{
		me->seq=ANIM_MOVE;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=256;
	}
}

void AI_IronSkull(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	byte f;
	int x,y;

	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_CLANG,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_ELDERDIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK)
		{
			// still chase, but slowly
			x=goodguy->x;
			y=goodguy->y-48*FIXAMT;

			FaceGoodguy3(me,goodguy);
			if(me->x<x)
				me->dx+=FIXAMT/4;
			if(me->y<y)
				me->dy+=FIXAMT/4;
			if(me->x>x)
				me->dx-=FIXAMT/4;
			if(me->y>y)
				me->dy-=FIXAMT/4;

			Clamp(&me->dx,FIXAMT*2);
			Clamp(&me->dy,FIXAMT*2);

			if(me->frm==4)
			{
				// shoot laser
				FireBullet(me->x,me->y,1,BLT_BEAM,me->friendly);
				FireBullet(me->x,me->y,2,BLT_BEAM,me->friendly);
				FireBullet(me->x,me->y,3,BLT_BEAM,me->friendly);
			}
		}

		if(me->seq==ANIM_DIE)
		{
			x=me->x>>FIXSHIFT;
			y=me->y>>FIXSHIFT;
			BlowUpGuy(x+me->rectx,y+me->recty,x+me->rectx2,y+me->recty2,me->z,1);
			BlowSmoke((x+me->rectx+Random(me->rectx2-me->rectx))<<FIXSHIFT,
					  (y+me->recty+Random(me->recty2-me->recty))<<FIXSHIFT,
					  me->z,FIXAMT);
		}
		return;
	}

	if(me->mind==0)
	{
		if(goodguy && (RangeToTarget(me,goodguy)<400*FIXAMT || me->ouch>0))
		{
			me->mind=1;
		}
	}
	if(me->mind==1)	// active
	{
		if(Random(100)==0)
			AddBaby(me->x,me->y,0,MONS_BONEHEAD,me);

		if(goodguy)
		{
			// calculate desired location (want to be above Bouapha)
			x=goodguy->x;
			y=goodguy->y-96*FIXAMT;

			FaceGoodguy2(me,goodguy);
			if(me->x<x)
				me->dx+=FIXAMT/8;
			if(me->y<y)
				me->dy+=FIXAMT/8;
			if(me->x>x)
				me->dx-=FIXAMT/8;
			if(me->y>y)
				me->dy-=FIXAMT/8;

			Clamp(&me->dx,FIXAMT*3);
			Clamp(&me->dy,FIXAMT*4);
		}

		if(me->reload==8)
		{
			// left eye fires
			f=(me->facing*32-32+me->mind1)&255;
			// can't fire backwards
			if(f>128)
			{
				if(f<128+64)
					f=128;
				else
					f=0;
			}
			FireExactBullet(me->x-32*FIXAMT,me->y+32*FIXAMT,me->z+80*FIXAMT,
						Cosine(f)*8,Sine(f)*8,-3*FIXAMT,0,30,me->facing,BLT_BIGSHELL,me->friendly);
		}
		if(!me->reload)
		{
			// right eye fires
			f=(me->facing*32+32-me->mind1)&255;
			// can't fire backwards
			if(f>128)
			{
				if(f<128+64)
					f=128;
				else
					f=0;
			}
			FireExactBullet(me->x+32*FIXAMT,me->y+32*FIXAMT,me->z+80*FIXAMT,
						Cosine(f)*8,Sine(f)*8,-3*FIXAMT,0,30,me->facing,BLT_BIGSHELL,me->friendly);
			me->reload=16;
			me->mind1+=8;
			if(me->mind1>=64)
				me->mind1=0;
		}

		if(Random(128)==0)
		{
			me->seq=ANIM_ATTACK;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=64;
			me->action=ACTION_BUSY;
			return;
		}
	}

	if(me->seq!=ANIM_IDLE)
	{
		me->seq=ANIM_IDLE;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=128;
	}
}

void AI_Spatula(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	Guy *g;
	int x,y;
	byte minions;

	if(!goodguy)
		return;

	minions=ArrangeBats(AngleFrom(me->x,me->y,goodguy->x,goodguy->y));

	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_COUNTOUCH,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_COUNTDIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm>5 && me->frm<9 && me->reload==0)
		{
			me->reload=30;
			if(minions<8)
			{
				g=AddBaby(me->x,me->y+FIXAMT*40,FIXAMT*20,MONS_BATGUARD,me);
				if(g && (!g->CanWalk(g->x,g->y,map,world)))
					RemoveGuy(g);
				else if(g)
				{
					g->mind=64;	// current angle
					g->parent=me;
				}
			}
		}

		if(me->seq==ANIM_DIE)
		{
			if(me->reload>4)
				me->reload=4;

			if(me->reload==0)
			{
				g=GetGuyOfType(MONS_BATGUARD);
				if(g!=NULL)
				{
					g->hp=1;
					g->GetShot(0,0,100,map,world);
				}
				me->reload=4;
			}
		}
		if(me->seq==ANIM_DIE && me->frm>3)
		{
			me->frmAdvance=128;
			x=me->x>>FIXSHIFT;
			y=me->y>>FIXSHIFT;
			BlowSmoke((x+me->rectx+Random(me->rectx2-me->rectx))<<FIXSHIFT,
					  (y+me->recty+Random(me->recty2-me->recty))<<FIXSHIFT,
					  me->z-Random(FIXAMT*60),FIXAMT);
		}

		return;	// can't do nothin' right now
	}

	if(!goodguy)
		return;

	if(minions<8 && me->reload==0)
	{
		MakeSound(SND_COUNTSHOOT,me->x,me->y,SND_CUTOFF,1200);
		me->action=ACTION_BUSY;
		me->seq=ANIM_ATTACK;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=128;
		return;
	}
	if(me->mind==0)	// waiting for Bouapha
	{
		if(RangeToTarget(me,goodguy)<300*FIXAMT || me->ouch>0)
		{
			MakeSound(SND_COUNTHELLO,me->x,me->y,SND_CUTOFF,1200);
			me->mind=1;
		}
		me->mind1=60;
	}
	if(me->mind==1)	// wandering mode
	{
		// calculate desired location (want to be above Bouapha)
		x=goodguy->x;
		y=goodguy->y;

		FaceGoodguy3(me,goodguy);
		if(me->x<x)
			me->dx+=FIXAMT/8;
		if(me->y<y)
			me->dy+=FIXAMT/8;
		if(me->x>x)
			me->dx-=FIXAMT/8;
		if(me->y>y)
			me->dy-=FIXAMT/8;

		Clamp(&me->dx,FIXAMT*3);
		Clamp(&me->dy,FIXAMT*4);
	}
	if(me->mind1==0)
	{
		me->mind1=120;
		me->mind2=30;	// while non-zero, all bats constantly shoot
	}
	else
		me->mind1--;

	if(me->mind2)
		me->mind2--;
}

void AI_BatGuard(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp==0)
			MakeSound(SND_BATDIE,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_BATOUCH,me->x,me->y,SND_CUTOFF,1200);
	}
	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_A1 && me->frm<6 && me->reload==0)
		{
			FireBullet(me->x,me->y,me->mind,BLT_ENERGY_BOUNCE,me->friendly);
			MakeSound(SND_MATTIESHOOT,me->x,me->y,SND_CUTOFF,200);
			me->reload=2;
		}
		if(me->seq==ANIM_A1  &&me->frm==5 && me->parent && me->parent->mind2!=0)
			me->frm=4;	// stay there until done

		return;	// can't do nothin' right now
	}

	if(me->AttackCheck(16,me->x>>FIXSHIFT,me->y>>FIXSHIFT,goodguy))
	{
		goodguy->GetShot(Cosine(me->facing*32)*8,Sine(me->facing*32)*8,4,map,world);
	}
	if(me->parent && me->parent->mind2>0)
	{
		MakeSound(SND_BATDIVE,me->x,me->y,SND_CUTOFF,200);
		me->seq=ANIM_A1;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=128;
		me->action=ACTION_BUSY;
		return;
	}
	if(me->seq!=ANIM_MOVE)
	{
		me->seq=ANIM_MOVE;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=110;
	}
}

void DoMove2(Guy *me,int move,int frmAdv,byte busy,int dx,int dy)
{
	me->seq=move;
	me->frm=0;
	me->frmTimer=0;
	me->frmAdvance=frmAdv;
	if(busy)
		me->action=ACTION_BUSY;
	else
		me->action=ACTION_IDLE;
	me->dx=dx;
	me->dy=dy;
}

void AI_Villager(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	if(me->reload)
		me->reload--;

	if(me->ouch==4 && me->hp==0)
		MakeSound(SND_VILLDIE1+(me->type>MONS_VILLAGERF),me->x,me->y,SND_CUTOFF,1200);
	
	if(me->action==ACTION_BUSY)
	{
		return;	// can't do nothin' right now
	}

	if(me->reload==0 && Random(64)==0 && me->mind3==0)	// scared girl doesn't yawn
	{
		if(me->aiType==MONS_VILLAGERM)
			MakeSound(SND_VILLAGERSCRATCH,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_VILLAGERYAWN,me->x,me->y,SND_CUTOFF,1200);
		me->seq=ANIM_A1;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=128;
		me->action=ACTION_BUSY;
		me->dx=0;
		me->dy=0;
		me->reload=(byte)(30*4+Random(136));
		return;
	}

	if(!(me->mind2--) || (me->mind1>0 && me->mind3==0))	// time to get a new direction
	{
		me->facing=(byte)Random(8);
		me->mind2=(byte)Random(40)+20;
		me->mind=(byte)Random(4);
		if(me->mind==1)
			me->mind=2;
		if(me->mind3 && me->mind>0)
			me->mind=(byte)Random(4)+3;

	}

	if(me->mind3==0 && RangeToTarget(me,goodguy)<FIXAMT*60)
	{
		me->mind=0;
		FaceGoodguy(me,goodguy);
	}

	me->dx=(Cosine(me->facing*32)*me->mind*FIXAMT)/FIXAMT;
	me->dy=(Sine(me->facing*32)*me->mind*FIXAMT)/FIXAMT;
	if(me->mind==0)
	{
		if(me->seq!=ANIM_IDLE)
		{
			me->seq=ANIM_IDLE;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
		}
	}
	else
	{
		if(me->seq!=ANIM_MOVE)
		{
			me->seq=ANIM_MOVE;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=me->mind*32;
		}
	}
	me->mind1=0;

	if(me->mind3)
	{
		// follow obnoxiously
		FaceGoodguy(me,goodguy);
	}
	
	// villagers have lanterns in dark places
	if(map->flags&MAP_TORCHLIT)
	map->DimTorch((me->x/TILE_WIDTH)>>FIXSHIFT,(me->y/TILE_HEIGHT)>>FIXSHIFT,10);
}

void AI_Mummy(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y,z;
	Guy *g;
	
	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_ZOMBIEOUCH,me->x,me->y,SND_CUTOFF,1800);
		else
			MakeSound(SND_ZOMBIEDIE,me->x,me->y,SND_CUTOFF,2000);
	}

	if(me->type==MONS_KINGTOOT && me->seq==ANIM_DIE && Random(5)==0)
	{
		y=me->y-FIXAMT*10+Random(FIXAMT*11);
		x=me->x-32*FIXAMT+Random(65*FIXAMT);
		FireBullet(x,y,0,BLT_EVILFACE,me->friendly);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm==5 && me->reload==0)
		{
			x=me->x+Cosine(me->facing*32)*16;
			y=me->y+Sine(me->facing*32)*16;
			if(me->AttackCheck(16,x>>FIXSHIFT,y>>FIXSHIFT,goodguy))
			{
				goodguy->GetShot(Cosine(me->facing*32)*4,Sine(me->facing*32)*4,8,map,world);
			}
			me->reload=5;
		}
		if(me->seq==ANIM_A1 && me->frm==3 && me->reload==0 && me->type!=MONS_PHARAOHF)
		{
			x=me->x+Cosine(me->facing*32)*8;
			y=me->y+Sine(me->facing*32)*8;
			MakeSound(SND_MUMMYSHOOT,me->x,me->y,SND_CUTOFF,1200);
			if(me->type!=MONS_KINGTOOT)
			{
				FireBullet(x,y,me->facing*32,BLT_ENERGY,me->friendly);
				if(me->type==MONS_PHARAOHG)
				{
					FireBullet(x,y,(me->facing)&255,BLT_BEAM,me->friendly);
				}
			}
			else
				FireBullet(x,y,me->facing*32,BLT_ENERGY,me->friendly);
			me->reload=5;
		}
		if(me->seq==ANIM_A1 && me->frm>=3 && me->frm<=5 && me->reload==0 && me->type==MONS_PHARAOHF)
		{
			x=me->x+Cosine(me->facing*32)*8;
			y=me->y+Sine(me->facing*32)*8;
			MakeSound(SND_MUMMYSHOOT,me->x,me->y,SND_CUTOFF,1200);
			z=me->facing*32-12+Random(25)+256;
			FireBullet(x,y,z&255,BLT_ENERGY,me->friendly);
		}
		if(me->seq==ANIM_A2 && me->frm==4 && me->reload==0)
		{
			g=AddGuy(me->x-FIXAMT*50,me->y+FIXAMT*10,0,MONS_RAGMAN+2*(me->type==MONS_KINGTOOT),me->friendly);
			if(g && (!g->CanWalk(g->x,g->y,map,world)))
				g->type=MONS_NONE;	// vanish if this spot is occupied
			g=AddGuy(me->x+FIXAMT*50,me->y+FIXAMT*10,0,MONS_RAGMAN+2*(me->type==MONS_KINGTOOT),me->friendly);
			if(g && (!g->CanWalk(g->x,g->y,map,world)))
				g->type=MONS_NONE;	// vanish if this spot is occupied
			me->reload=5;
		}
		return;	// can't do nothin' right now
	}

	if(me->mind1==0)
		me->mind1=70;
	else
	{
		me->mind1--;
		if(me->mind1==0)
		{
			if((me->type==MONS_PHARAOHG || me->type==MONS_KINGTOOT) && Random(4)==0)
			{
				MakeSound(SND_MUMMYSUMMON,me->x,me->y,SND_CUTOFF,1200);
				me->seq=ANIM_A2;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=128;
				me->action=ACTION_BUSY;
				me->dx=0;
				me->dy=0;
				me->reload=0;
			}
		}
	}

	if(RangeToTarget(me,goodguy)<(72*FIXAMT) && Random(8)==0 &&
		map->CheckLOS(me->mapx,me->mapy,4,goodguy->mapx,goodguy->mapy))
	{
		// get him!
		MakeSound(SND_MUMBLECRUSH,me->x,me->y,SND_CUTOFF,1200);
		me->seq=ANIM_ATTACK;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=128;
		if(me->type==MONS_RAGMAN)
			me->frmAdvance=256;
		me->action=ACTION_BUSY;
		me->dx=0;
		me->dy=0;
		me->reload=0;
		return;
	}

	x=64;
	if(me->type==MONS_PHARAOHF)
		x=32;
	if(me->type!=MONS_RAGMAN && RangeToTarget(me,goodguy)<(360*FIXAMT) && Random(x)==0 &&
		map->CheckLOS(me->mapx,me->mapy,15,goodguy->mapx,goodguy->mapy))
	{
		// shoot
		me->seq=ANIM_A1;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=128;
		me->action=ACTION_BUSY;
		me->dx=0;
		me->dy=0;
		me->reload=0;
		FaceGoodguy(me,goodguy);
		return;
	}

	if(me->type==MONS_RAGMAN && me->seq==ANIM_MOVE)
		me->frmAdvance=256;

	WanderAI(me,64,40,3,map,world,goodguy,FIXAMT*3);
}

void AI_EvilTree(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y;
	Guy *g;
	
	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_TREEOUCH,me->x,me->y,SND_CUTOFF,1800);
		else
			MakeSound(SND_TREEDIE,me->x,me->y,SND_CUTOFF,2000);
	}

	// initially adjust position
	if(me->mind3==0)
	{
		me->x-=FIXAMT*3;
		me->y+=FIXAMT;
		me->mind3=1;
	}

	if(me->type==MONS_TORTREE && me->seq==ANIM_DIE && Random(10)==0)
	{
		y=me->y-FIXAMT*10+Random(FIXAMT*11);
		x=me->x-32*FIXAMT+Random(65*FIXAMT);
		FireBullet(x,y,0,BLT_EVILFACE,me->friendly);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm==4 && me->reload==0)
		{
			if(me->type==MONS_SWAMPSTUMP)
			{
				// gas attack
				MakeSound(SND_SKELGAS,me->x,me->y,SND_CUTOFF,1200);
				FireBullet(me->x,me->y,64,BLT_ENERGY,me->friendly);
				FireBullet(me->x,me->y,64+16,BLT_ENERGY,me->friendly);
				FireBullet(me->x,me->y,64-16,BLT_ENERGY,me->friendly);
				me->reload=30*2;
			}
			else if(me->type==MONS_TERRORTREE)
			{
				// summon frog
				MakeSound(SND_TREEFROG,me->x,me->y,SND_CUTOFF,1200);
				x=me->x;
				y=me->y+40*FIXAMT;
				g=AddGuy(x,y,FIXAMT*10,MONS_FROG3,me->friendly);
				if(g && (!g->CanWalk(g->x,g->y,map,world)))
					g->type=MONS_NONE;	// vanish if this spot is occupied
				me->reload=30*3;
			}
			else
			{
				// summon frog
				MakeSound(SND_TREEFROG,me->x,me->y,SND_CUTOFF,1200);
				x=me->x;
				y=me->y+40*FIXAMT;
				g=AddGuy(x,y,FIXAMT*10,MONS_FROGURT,me->friendly);
				if(g && (!g->CanWalk(g->x,g->y,map,world)))
					g->type=MONS_NONE;	// vanish if this spot is occupied
				me->reload=30*3;
			}
		}
		return;	// can't do nothin' right now
	}

	if(me->type==MONS_TERRORTREE)
	{
		// launch frogs when player is very near
		if(RangeToTarget(me,goodguy)<(256*FIXAMT) && me->reload==0 && Random(16)==0)
		{
			// shoot
			me->seq=ANIM_ATTACK;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
			me->action=ACTION_BUSY;
			return;
		}
	}
	else if(me->aiType==MONS_TORTREE)
	{
		// launch frogs when player is near
		if(RangeToTarget(me,goodguy)<(400*FIXAMT) && me->reload==0 && Random(16)==0)
		{
			// shoot
			me->seq=ANIM_ATTACK;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
			me->action=ACTION_BUSY;
			return;
		}
	}
	else
	{
		// launch gas at a standard rate
		if(me->reload==0)
		{
			// shoot
			me->seq=ANIM_ATTACK;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
			me->action=ACTION_BUSY;
			return;
		}
	}
}

void AI_Larry(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y,i;
	Guy *g;
	
	if(me->mind)
		me->mind--;
	
	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_LARRYOUCH,me->x,me->y,SND_CUTOFF,1800);
		else
			MakeSound(SND_SWAMPDIE,me->x,me->y,SND_CUTOFF,2000);
	}
	
	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm==4)
		{
			me->dx=Cosine(me->facing*32)*10;
			me->dy=Sine(me->facing*32)*10;
		}
		if(me->seq==ANIM_ATTACK && me->frm>=6 && me->frm<=10)
		{
			Dampen(&me->dx,FIXAMT);
			Dampen(&me->dy,FIXAMT);
			if(me->AttackCheck(40,me->x>>FIXSHIFT,me->y>>FIXSHIFT,goodguy))
			{
				goodguy->GetShot(Cosine(me->facing*32)*8,Sine(me->facing*32)*8,8,map,world);
			}
			if(me->frm==8 && me->frmTimer<128)
			{
				x=me->x+Cosine(me->facing*32)*48;
				y=me->y+Sine(me->facing*32)*48;
				//MakeRingParticle(x,y,0,128,64);
				if(me->AttackCheck(128,x>>FIXSHIFT,y>>FIXSHIFT,goodguy))
				{
					goodguy->GetShot(Cosine(me->facing*32)*8,Sine(me->facing*32)*8,8,map,world);
				}
				FireBullet(me->x,me->y-FIXAMT,0,BLT_SHOCKWAVE,me->friendly);
				MakeSound(SND_BOMBBOOM,me->x,me->y,SND_CUTOFF,1500);
				for(i=0;i<5;i++)
				{
					x=me->x+((256-Random(513))<<FIXSHIFT);
					y=me->y+((256-Random(513))<<FIXSHIFT);
					FireExactBullet(x,y,FIXAMT*80+(Random(40)<<FIXSHIFT),
									0,0,0,0,30,2,BLT_ROCK,me->friendly);
				}
				ShakeScreen(15);
			}
			if(me->frm==10)
			{
				me->dx=0;
				me->dy=0;
			}
		}
		if(me->seq==ANIM_A1 && (me->frm==6 || me->frm==7) && me->frmTimer<128)
		{
			x=me->x+Cosine((me->facing+7+2*(7-me->frm))*32)*20;
			y=me->y+Sine((me->facing+7+2*(7-me->frm))*32)*20;
			MakeSound(SND_LARRYDARTS,me->x,me->y,SND_CUTOFF,1200);
			for(i=0;i<10;i++)
			{
				FireBullet(x,y,(me->facing*32+(me->frm-6)*8+256-48+i*(96/10))&255,BLT_SPINE,me->friendly);
			}
			me->reload=5;
		}
		if(me->seq==ANIM_A2 && me->frm>=5 && me->frm<=15)
		{
			x=me->x+Cosine(me->facing*32)*32;
			y=me->y+Sine(me->facing*32)*32;
			if(me->AttackCheck(16,x>>FIXSHIFT,y>>FIXSHIFT,goodguy))
			{
				goodguy->GetShot(Cosine(me->facing*32)*4,Sine(me->facing*32)*4,8,map,world);
			}
		}
		if(me->seq==ANIM_DIE)
		{
			FireBullet(me->x,me->y,0,BLT_BOOM,me->friendly);
		}
		return;	// can't do nothin' right now
	}

	if(map->CheckLOS(me->mapx,me->mapy,15,goodguy->mapx,goodguy->mapy))
	{
		// if goodguy is in sight
		if(RangeToTarget(me,goodguy)<(256*FIXAMT) && Random(12)==0)
		{
			// get him!
			MakeSound(SND_LARRYPOUNCE,me->x,me->y,SND_CUTOFF,1200);
			DoMove2(me,ANIM_ATTACK,128,1,0,0);
			FaceGoodguy2(me,goodguy);
			return;
		}
		if(Random(32)==0)	// fire at any range
		{
			// shoot darts
			MakeSound(SND_LARRYPOUNCE,me->x,me->y,SND_CUTOFF,1200);
			DoMove2(me,ANIM_A1,200,1,0,0);
			FaceGoodguy2(me,goodguy);
			return;
		}
		if(me->mapx==me->mind2 && me->mapy==me->mind3)
		{
			me->mind2=goodguy->mapx;
			me->mind3=goodguy->mapy;
			SelectDestination(me,goodguy,map,0);
		}
	}
	else
	{
		if(me->mapx==me->mind2 && me->mapy==me->mind3)
			SelectDestination(me,goodguy,map,0);
	}

	/*
	if(me->mind1 && (abs(me->dx)>FIXAMT*3 || abs(me->dy)>FIXAMT*3))
	{
		me->mind1=0;
		me->dx=0;
		me->dy=0;
		SelectDestination(me,goodguy,map,1);
	}
	*/

	if(me->mapx>me->mind2)
		me->dx-=FIXAMT;
	else if(me->mapx<me->mind2)
		me->dx+=FIXAMT;
	if(me->mapy>me->mind3)
		me->dy-=FIXAMT;
	else if(me->mapy<me->mind3)
		me->dy+=FIXAMT;
	Dampen(&me->dx,FIXAMT/4);
	Dampen(&me->dy,FIXAMT/4);
	Clamp(&me->dx,FIXAMT*6);
	Clamp(&me->dy,FIXAMT*6);
	if(me->dx<-FIXAMT)
	{
		if(me->dy<-FIXAMT)
			me->facing=5;
		else if(me->dy>FIXAMT)
			me->facing=3;
		else
			me->facing=4;
	}
	else if(me->dx>FIXAMT)
	{
		if(me->dy<-FIXAMT)
			me->facing=7;
		else if(me->dy>FIXAMT)
			me->facing=1;
		else
			me->facing=0;
	}
	else
	{
		if(me->dy<-FIXAMT)
			me->facing=6;
		else
			me->facing=2;
	}

	if(me->seq!=ANIM_MOVE)
		DoMove2(me,ANIM_MOVE,180,0,me->dx,me->dy);
}

void AI_Swampdog(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y,z;
	
	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_SWAMPOUCH,me->x,me->y,SND_CUTOFF,1800);
		else
			MakeSound(SND_SWAMPDIE,me->x,me->y,SND_CUTOFF,2000);
	}

	if(me->mind)
		me->mind--;

	if(me->type==MONS_SWAMPDOG4 && me->seq==ANIM_DIE && Random(10)==0)
	{
		y=me->y-FIXAMT*20+Random(FIXAMT*41);
		x=me->x-32*FIXAMT+Random(65*FIXAMT);
		FireBullet(x,y,0,BLT_EVILFACE,me->friendly);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm==3 && me->reload==0)
		{
			x=me->x+Cosine(me->facing*32)*32;
			y=me->y+Sine(me->facing*32)*32;
			if(me->AttackCheck(32,x>>FIXSHIFT,y>>FIXSHIFT,goodguy))
			{
				goodguy->GetShot(Cosine(me->facing*32)*4,Sine(me->facing*32)*4,5,map,world);
			}
			me->reload=5;
		}
		if(me->seq==ANIM_A3 && me->frm==6 && me->reload==0)
		{
			x=me->x+Cosine(me->facing*32)*8;
			y=me->y+Sine(me->facing*32)*8;

			if(me->type==MONS_SWAMPDOG3 || me->type==MONS_SWAMPDOG4)
			{
				if(me->mind3&1)
				{
					z=(me->facing+8)*32-32;
					z+=(me->mind3-1)*8;
					z=z&255;
				}
				else
				{
					z=(me->facing+8)*32+32;
					z-=(me->mind3-1)*8;
					z=z&255;
				}
				if(me->type==MONS_SWAMPDOG4)
					FireBullet(x,y,z,BLT_BIGSHELL,me->friendly);
				else if(me->type==MONS_HOTDOG)
					FireBullet(x,y,z,BLT_FLAME5,me->friendly);
				else
					FireBullet(x,y,z,BLT_ENERGY,me->friendly);
				if(me->mind3>1)
				{
					// repeat fire
					me->mind3-=2;
					me->frm=4;
				}
				me->reload=3;
			}
			else
			{
				FireBullet(x,y,me->facing*32,BLT_ENERGY,me->friendly);
				me->reload=5;
			}
		}
		return;	// can't do nothin' right now
	}

	if(me->mind2==0)	// unalert
	{
		if(RangeToTarget(me,goodguy)<128*FIXAMT || me->ouch>0)
		{
			// seen goodguy/got shot - ALERT!!
			me->mind2=1;
			me->action=ACTION_BUSY;
			me->seq=ANIM_A1;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=256;
			me->mind=30*5;
			MakeSound(SND_SWAMPALERT,me->x,me->y,SND_CUTOFF,1200);
		}
		else
		{
			if(Random(64)==0)
				me->facing=(me->facing+7+(byte)Random(3))&7;

			if(me->mind1==0)	// standing
			{
				me->dx=0;
				me->dy=0;
				if(me->seq!=ANIM_IDLE)
				{
					me->seq=ANIM_IDLE;
					me->frm=0;
					me->frmTimer=0;
					me->frmAdvance=64;
				}
				if(me->mind==0)
				{
					// plod around a bit
					me->mind=(byte)Random(30*4);
					me->mind1=1;
				}
			}
			else	// plodding
			{
				me->dx=Cosine(me->facing*32)*2*FIXAMT;
				me->dy=Sine(me->facing*32)*2*FIXAMT;
				if(me->type==MONS_HOTDOG)
				FireBullet(x,y,me->facing+4,BLT_FLAME5,me->friendly);
				if(me->seq!=ANIM_MOVE)
				{
					me->seq=ANIM_MOVE;
					me->frm=0;
					me->frmTimer=0;
					me->frmAdvance=64;
				}
				if(me->mind==0)
				{
					// stand around a bit
					me->mind=(byte)Random(30*2);
					me->mind1=0;
				}
			}
		}
	}
	else // ALERT
	{
		if(me->ouch)
			me->mind=30*5;	// force alertness when injured

		if(me->mind==0)
		{
			// time to be bored again
			DoMove2(me,ANIM_A4,64,1,0,0);
			me->mind2=0;
		}
		if(me->seq==ANIM_A5 && me->frm>1)
			me->frm=0;

		if(me->seq==ANIM_IDLE)
		{
			// can't stand still, must charge
			FaceGoodguy(me,goodguy);
			if(me->type!=MONS_SWAMPDOG3)
			{
				MakeSound(SND_SWAMPCHARGE,me->x,me->y,SND_CUTOFF,1200);
				DoMove2(me,ANIM_A2,128,0,Cosine(me->facing*32)*5,Sine(me->facing*32)*5);
			}
			else	// Shambling Thing does stand still
				DoMove2(me,ANIM_A5,128,0,0,0);
		}
		if(RangeToTarget(me,goodguy)<(72*FIXAMT) && Random(8)==0 &&
			map->CheckLOS(me->mapx,me->mapy,15,goodguy->mapx,goodguy->mapy))
		{
			// get him!
			FaceGoodguy(me,goodguy);
			MakeSound(SND_SWAMPJAB,me->x,me->y,SND_CUTOFF,1200);
			DoMove2(me,ANIM_ATTACK,128,1,0,0);
			me->reload=0;
			return;
		}
		if(me->type==MONS_SWAMPDOG3)
			x=8;
		else
			x=32;
		if(RangeToTarget(me,goodguy)<(380*FIXAMT) && Random(x)==0 &&
			map->CheckLOS(me->mapx,me->mapy,15,goodguy->mapx,goodguy->mapy))
		{
			// get him!
			FaceGoodguy(me,goodguy);
			MakeSound(SND_SWAMPSHOOT,me->x,me->y,SND_CUTOFF,1200);
			DoMove2(me,ANIM_A3,128,1,0,0);
			me->reload=0;
			me->mind3=4*2+(byte)Random(2);

			return;
		}
	}
	if(me->seq==ANIM_A2)
	{
		// charging
		if(me->AttackCheck(32,me->x>>FIXSHIFT,me->y>>FIXSHIFT,goodguy))
		{
			goodguy->GetShot(Cosine(me->facing*32)*4,Sine(me->facing*32)*4,6,map,world);
		}
	}
}

void AI_LLGhost(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y;
	
	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_GHOSTOUCH,me->x,me->y,SND_CUTOFF,1800);
		else
			MakeSound(SND_GHOSTDIE,me->x,me->y,SND_CUTOFF,2000);
	}

	// banshees glow
	if(me->type==MONS_GHOST3)
		map->BrightTorch((me->x/TILE_WIDTH)>>FIXSHIFT,
						 (me->y/TILE_HEIGHT)>>FIXSHIFT,8,9);
	// shades make darkness
	else if(me->type==MONS_GHOST2)
		map->BrightTorch((me->x/TILE_WIDTH)>>FIXSHIFT,
						 (me->y/TILE_HEIGHT)>>FIXSHIFT,-8,9);

	if(me->mind3>0)
		me->mind3--;

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm==5)
		{
			x=me->x+Cosine(me->facing*32)*16;
			y=me->y+Sine(me->facing*32)*16;
			if(me->AttackCheck(24,x>>FIXSHIFT,y>>FIXSHIFT,goodguy))
			{
				goodguy->GetShot(Cosine(me->facing*32)*2,Sine(me->facing*32)*2,5,map,world);
			}
		}
		if(me->seq==ANIM_A1 && me->frm==4 && me->reload==0)
		{
			// banshees teleport randomly
			if(me->type==MONS_GHOST3)
			{
				me->x=(me->x-320*FIXAMT)+Random(640*FIXAMT);
				me->y=(me->y-240*FIXAMT)+Random(480*FIXAMT);
				if(me->x<32*FIXAMT)
					me->x=32*FIXAMT;
				if(me->y<32*FIXAMT)
					me->y=32*FIXAMT;
				if(me->x>map->width*TILE_WIDTH*FIXAMT-TILE_WIDTH*FIXAMT)
					me->x=map->width*TILE_WIDTH*FIXAMT-TILE_WIDTH*FIXAMT;
				if(me->y>map->height*TILE_HEIGHT*FIXAMT-TILE_HEIGHT*FIXAMT)
					me->y=map->height*TILE_HEIGHT*FIXAMT-TILE_HEIGHT*FIXAMT;
			}
			else	// shades teleport to player
			{
				me->x=Random(80*FIXAMT)+40*FIXAMT;
				if(Random(2)==0)
					me->x=-me->x;
				me->x+=goodguy->x;
				me->y=Random(60*FIXAMT)+30*FIXAMT;
				if(Random(2)==0)
					me->y=-me->y;
				me->y+=goodguy->y;
				if(me->x<32*FIXAMT)
					me->x=32*FIXAMT;
				if(me->y<32*FIXAMT)
					me->y=32*FIXAMT;
				if(me->x>map->width*TILE_WIDTH*FIXAMT-TILE_WIDTH*FIXAMT)
					me->x=map->width*TILE_WIDTH*FIXAMT-TILE_WIDTH*FIXAMT;
				if(me->y>map->height*TILE_HEIGHT*FIXAMT-TILE_HEIGHT*FIXAMT)
					me->y=map->height*TILE_HEIGHT*FIXAMT-TILE_HEIGHT*FIXAMT;
			}
			for(x=0;x<4;x++)
			{
				map->BrightTorch((me->x/TILE_WIDTH)>>FIXSHIFT,
							 (me->y/TILE_HEIGHT)>>FIXSHIFT,16,9);
			}
			me->reload=5;
		}
		if(me->seq==ANIM_A2 && (me->frm==2 || me->frm==8 || me->frm==14))
		{
			MakeSound(SND_GHOSTSPIN,me->x,me->y,SND_CUTOFF,1200);
			if(me->frm==14)
			{
				me->dx=Cosine(me->facing*32)*6;
				me->dy=Sine(me->facing*32)*6;
			}
		}
		if(me->seq==ANIM_A2 && me->frm==18)
			MakeSound(SND_GHOSTPUNCH,me->x,me->y,SND_CUTOFF,1200);
		if(me->seq==ANIM_A2 && me->frm>=17 && me->frm<=25)
		{
			x=me->x+Cosine(me->facing*32)*16;
			y=me->y+Sine(me->facing*32)*16;
			if(me->AttackCheck(24,x>>FIXSHIFT,y>>FIXSHIFT,goodguy))
			{
				goodguy->GetShot(Cosine(me->facing*32)*8,Sine(me->facing*32)*8,10,map,world);
			}
		}
		if(me->seq==ANIM_A2 && me->frm>21)
		{
			Dampen(&me->dx,FIXAMT/4);
			Dampen(&me->dy,FIXAMT/4);
		}
		if(me->seq==ANIM_A3 && me->frm==2 && me->reload==0)
		{
			// scream
			me->reload=10;

			//MakeColdRingParticle(me->x,me->y,0,180);
			if(RangeToTarget(me,goodguy)<200*FIXAMT)
			{
				goodguy->dx=0;
				goodguy->dy=0;
				MakeSound(SND_FREEZE,x*FIXAMT,y*FIXAMT,SND_CUTOFF,1200);
			}
		}
		return;	// can't do nothin' right now
	}

	// punch
	// Shades only use super punches if player is stone
	if(RangeToTarget(me,goodguy)<(88*FIXAMT) && Random(6)==0 &&
		(me->type!=MONS_GHOST2))
	{
		// get him!
		MakeSound(SND_GHOSTPUNCH,me->x,me->y,SND_CUTOFF,1200);
		DoMove2(me,ANIM_ATTACK,164,1,0,0);
		me->reload=0;
		FaceGoodguy(me,goodguy);
		return;
	}
	// Super punch!!
	if(me->type==MONS_GHOST2 && RangeToTarget(me,goodguy)<(160*FIXAMT) &&
		Random(32)==0)
	{
		DoMove2(me,ANIM_A2,256,1,0,0);
		me->reload=0;
		FaceGoodguy(me,goodguy);
		return;
	}
	// teleport if non-Ghost
	if(RangeToTarget(me,goodguy)>(320*FIXAMT) && Random(32)==0)
	{
		MakeSound(SND_GHOSTTPORT,me->x,me->y,SND_CUTOFF,1200);
		DoMove2(me,ANIM_A1,128,1,0,0);
		me->reload=0;
		return;
	}
	// Banshees scream
	if(me->aiType==MONS_GHOST3 && RangeToTarget(me,goodguy)<256*FIXAMT && me->mind3==0 && Random(128)==0)
	{
		// scream
		me->mind3=30*5;
		MakeSound(SND_GHOSTYELL,me->x,me->y,SND_CUTOFF,1200);
		DoMove2(me,ANIM_A3,128,1,0,0);
		me->reload=0;
		return;
	}
	WanderAI(me,256,20,3,map,world,goodguy,FIXAMT*3);
}

void AI_StickWitch(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y;
	Guy *g;
	int summonTab[]={MONS_STKSHROOM,MONS_STKSPIDER,MONS_STKCORPSE,MONS_STKBAT,MONS_STKSNOW};

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_STICKOUCH,me->x,me->y,SND_CUTOFF,1800);
		else
			MakeSound(SND_STICKDIE,me->x,me->y,SND_CUTOFF,2000);
	}

	if(me->mind3<2)
	{
		if(me->mind3==0 && (RangeToTarget(me,goodguy)<400*FIXAMT || me->ouch))
		{
			me->mind3=1;
			return;
		}
		else if(me->mind3==1 && (RangeToTarget(me,goodguy)<400*FIXAMT || me->ouch))
		{
			me->mind3=2;
			MakeSound(SND_STICKTAUNT,me->x,me->y,SND_CUTOFF,1200);
			DoMove2(me,ANIM_A2,128,1,0,0);
		}
		return;
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->reload==0)
		{
			// shoot
			switch(me->mind2)
			{
				case 0:	// blast spell
					MakeSound(SND_HAPPYCHOMP,me->x,me->y,SND_CUTOFF,120);
					x=me->x+Cosine(me->mind*32)*16;
					y=me->y+Sine(me->mind*32)*16;
					FireBullet(x,y,(me->mind*32+256-16+(byte)Random(32)),BLT_BIGSHELL,me->friendly);
					me->reload=2;
					break;
				case 1:	// gas blast
					MakeSound(SND_SKELGAS,me->x,me->y,SND_CUTOFF,120);
					x=me->x+Cosine(me->mind*32)*16;
					y=me->y+Sine(me->mind*32)*16;
					FireBullet(x,y,(me->mind*32+256-16+(byte)Random(32)),BLT_SPORE,me->friendly);
					me->reload=1;
					break;
				case 2:	// flames
					MakeSound(SND_BULLETFIRE,me->x,me->y,SND_CUTOFF,120);
					x=me->x+Cosine(me->mind*32)*16;
					y=me->y+Sine(me->mind*32)*16;
					FireBullet(x,y,(me->mind*32+256-16+(byte)Random(32)),BLT_FLAME5,me->friendly);
					me->reload=1;
					break;
				case 3:	// wind missiles
					MakeSound(SND_FRANKMISSILE,me->x,me->y,SND_CUTOFF,120);
					x=me->x+Cosine(me->mind*32)*16;
					y=me->y+Sine(me->mind*32)*16;
					FireBullet(x,y,(me->mind*32+256-16+(byte)Random(32)),BLT_WIND,me->friendly);
					me->reload=3;
					break;
				case 4: // earth spikes
					MakeSound(SND_SPD3SPEW,me->x,me->y,SND_CUTOFF,120);
					x=me->x+Cosine(me->mind*32)*16;
					y=me->y+Sine(me->mind*32)*16;
					FireBullet(x,y,(me->mind+8+(byte)Random(8)),BLT_DIRTSPIKE,me->friendly);
					me->reload=3;
					break;
				case 5: // summoning spikes
					g=AddGuy(x,y,FIXAMT*40,summonTab[Random(4)],me->friendly);
					if(g && !g->CanWalk(g->x,g->y,map,world))
						g->type=MONS_NONE;
					else
					{
						map->BrightTorch((g->x/TILE_WIDTH)>>FIXSHIFT,(g->y/TILE_HEIGHT)>>FIXSHIFT,18,8);
						MakeSound(SND_TELEPORT,g->x,g->y,SND_CUTOFF,1200);
					}
					me->reload=20;
					break;
			}
		}
		return;	// can't do nothin' right now
	}

	if(RangeToTarget(me,goodguy)<512*FIXAMT && Random(16)==0)
	{
		x=me->facing;
		FaceGoodguy3(me,goodguy);
		me->mind=me->facing;
		me->facing=x;
		DoMove2(me,ANIM_ATTACK,128,1,me->dx,me->dy);
		return;
	}


	if(me->mind3>2)
	{
		me->mind3--;
		if(me->mind3<10)
		{
			//MakeRingParticle(me->x,me->y,0,(15-me->mind3)*3,50);
			if(RangeToTarget(me,goodguy)<70*FIXAMT)
				goodguy->GetShot(Cosine(me->facing)*16,Sine(me->facing)*16,8,map,world);
		}
	}
	else
	{
		me->mind2=(byte)Random(6);
		me->mind3=30*2+(byte)Random(30*5);
		MakeSound(SND_STICKTAUNT,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->ouch>0)
	{
		if(me->mind3>10)
			me->mind3-=5;
		DoMove2(me,ANIM_A1,128,0,me->dx,me->dy);
		return;
	}

	if(!(me->mind1--))	// time to get a new direction
	{
		me->facing=(byte)Random(8);
		me->mind1=(byte)Random(30*3)+1;
	}

	me->dx=(Cosine(me->facing*32)*3)*FIXAMT;
	me->dy=(Sine(me->facing*32)*3)*FIXAMT;
	if(me->seq!=ANIM_MOVE)
	{
		me->seq=ANIM_MOVE;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=128;
	}
}

void AI_Frankenjulie(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y;
	Guy *g;

	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
		{
			if(me->mind<2)
				MakeSound(SND_FRANKOUCH,me->x,me->y,SND_CUTOFF,1200);
			else
				MakeSound(SND_FRANKOUCH2,me->x,me->y,SND_CUTOFF,1200);

			if(me->hp<me->maxHP*0.5 && me->mind==0)
			{
				me->mind=1;
				FireBullet(me->x,me->y,0,BLT_MEGABOOM,me->friendly);
				DoMove2(me,ANIM_A1,128,0,0,0);
				ShakeScreen(5);
			}
			if(me->hp<me->maxHP*0.25 && me->mind==1)
			{
				me->mind=2;
				FireBullet(me->x,me->y,0,BLT_MEGABOOM,me->friendly);
				DoMove2(me,ANIM_A1,128,0,0,0);
				ShakeScreen(5);
				g=AddGuy(me->x-FIXAMT*250,me->y,0,MONS_HAND,me->friendly);
				if(g && (!g->CanWalk(g->x,g->y,map,world)))
					g->type=MONS_NONE;	// vanish if this spot is occupied
				g=AddGuy(me->x+FIXAMT*250,me->y,0,MONS_HAND,me->friendly);
				if(g && (!g->CanWalk(g->x,g->y,map,world)))
					g->type=MONS_NONE;	// vanish if this spot is occupied
			}
		}
		else
		{
			MakeSound(SND_FRANKDIE,me->x,me->y,SND_CUTOFF,1200);
		}
	}

	if(me->mind2==0)
	{
		if(RangeToTarget(me,goodguy)<256*FIXAMT || me->ouch)
		{
			me->mind2=1;
		}
		return;
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_DIE)
		{
			ShakeScreen(120);
			FireBullet(me->x-60*FIXAMT+Random(120*FIXAMT),me->y-60*FIXAMT+Random(120*FIXAMT),0,BLT_MEGABOOM,me->friendly);
		}
		if(me->seq==ANIM_ATTACK)
		{
			if(me->frmTimer<128)
				//MakeSound(SND_FRANKHEAD,me->x,me->y,SND_CUTOFF,400);
			if(me->frm<16 && me->frmTimer<128)
			{
				x=(me->x+Cosine(me->frm*16)*80);
				y=(me->y+Sine(me->frm*16)*60);
				FireExactBullet(x,y,FIXAMT*70,Cosine(me->frm*16)*8,Sine(me->frm*16)*8,
					-FIXAMT*2,0,60,me->frm*16,BLT_ENERGY,me->friendly);
				x=(me->x+Cosine((me->frm+8)*16)*80);
				y=(me->y+Sine((me->frm+8)*16)*60);
				FireExactBullet(x,y,FIXAMT*70,Cosine((me->frm+8)*16)*8,Sine((me->frm+8)*16)*8,
					-FIXAMT*2,0,60,(me->frm+8)*16,BLT_ENERGY,me->friendly);

			}
		}
		if(me->seq==ANIM_A3)
		{
			if(me->frm>4 && me->frm<20)
			{
				me->dx+=Cosine(me->facing*32);
				me->dy+=Sine(me->facing*32);
				Clamp(&me->dx,6*FIXAMT);
				Clamp(&me->dy,6*FIXAMT);

				x=(me->x+Cosine((me->frm-4)*16)*200)/FIXAMT;
				y=(me->y+Sine((me->frm-4)*16)*160)/FIXAMT;
				if(me->AttackCheck(50,x,y,goodguy))
					goodguy->GetShot(Cosine((me->frm-4)*16)*8,Sine((me->frm-4)*16)*8,3,map,world);
				x=(me->x+Cosine((me->frm+4)*16)*200)/FIXAMT;
				y=(me->y+Sine((me->frm+4)*16)*160)/FIXAMT;
				if(me->AttackCheck(50,x,y,goodguy))
					goodguy->GetShot(Cosine((me->frm+4)*16)*8,Sine((me->frm+4)*16)*8,3,map,world);
			}
			else if(me->frm>=20)
			{
				me->dx=(me->dx*3)/4;
				me->dy=(me->dy*3)/4;
				if(me->frm==21)
				DoMove2(me,ANIM_A1,128,0,0,0);
			}
		}
		if(me->seq==ANIM_A4 && me->frm==4 && me->frmTimer<128)
		{
			x=me->x-120*FIXAMT;
			y=me->y-20*FIXAMT;
			FireBullet(x,y,128,BLT_MISSILE,me->friendly);
			FireBullet(x,y-30*FIXAMT,148,BLT_MISSILE,me->friendly);
			x=me->x+120*FIXAMT;
			y=me->y-20*FIXAMT;
			FireBullet(x,y,0,BLT_MISSILE,me->friendly);
			FireBullet(x,y-30*FIXAMT,236,BLT_MISSILE,me->friendly);
			//MakeSound(SND_FRANKMISSILE,me->x,me->y,SND_CUTOFF,1200);
		}
		if(me->seq==ANIM_A4 && me->frm==10)
		{
			DoMove2(me,ANIM_A2,128,0,0,0);
		}
		return;
	}
	if(RangeToTarget(me,goodguy)<96*FIXAMT)
	{
		// 'acidic'
		goodguy->GetShot(0,0,5,map,world);
	}

	FaceGoodguy2(me,goodguy);
	me->dx+=Cosine(me->facing*32)/2;
	me->dy+=Sine(me->facing*32)/2;
	
	Clamp(&me->dx,FIXAMT*5);
	Clamp(&me->dy,FIXAMT*5);

	if(me->mind==0)	// have head
	{
		if(me->mind1==0)
			me->mind1=30*3;
		else
		{
			me->mind1--;
			if(me->mind1==0)
			{
				DoMove2(me,ANIM_ATTACK,128,1,me->dx,me->dy);
			}
		}
	}
	if(me->mind==1)	// lost head already
	{
		if(me->seq==ANIM_IDLE || me->seq==ANIM_MOVE || (me->seq==ANIM_A1 && me->frm>1))
		{
			DoMove2(me,ANIM_A1,128,0,me->dx,me->dy);
		}
		if(me->mind1==0)
			me->mind1=30*3;
		else
		{
			me->mind1--;
			if(me->mind1==0)
			{
				MakeSound(SND_FRANKARMS,me->x,me->y,SND_CUTOFF,1200);
				DoMove2(me,ANIM_A3,256,1,me->dx,me->dy);
				FaceGoodguy(me,goodguy);
			}
			if(me->mind1%15==0)
			{
				MakeSound(SND_MUSHMISSILE,me->x,me->y,SND_CUTOFF,1200);
				FireBullet(me->x,me->y,(byte)Random(256),BLT_GRENADE,me->friendly);
			}
		}
	}
	if(me->mind==2)	// lost body too
	{
		if(me->seq==ANIM_IDLE || me->seq==ANIM_MOVE || (me->seq==ANIM_A2 && me->frm>1))
		{
			DoMove2(me,ANIM_A2,128,0,me->dx,me->dy);
		}
		if(me->mind1==0)
			me->mind1=30*2;
		else
		{
			me->mind1--;
			if(me->mind1==0)
			{
				DoMove2(me,ANIM_A4,128,1,me->dx,me->dy);
			}
		}
	}
}

void AI_Hand(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y;
	
	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_PUMPKINOUCH,me->x,me->y,SND_CUTOFF,1800);
		else
			MakeSound(SND_PUMPKINDIE,me->x,me->y,SND_CUTOFF,2000);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm==7 && me->reload==0)
		{
			x=me->x+Cosine(me->facing*32)*40;
			y=me->y+Sine(me->facing*32)*40;
			if(me->AttackCheck(32,x>>FIXSHIFT,y>>FIXSHIFT,goodguy))
				goodguy->GetShot(Cosine(me->facing*32)*8,Sine(me->facing*32)*8,10,map,world);
			me->reload=5;
		}
		return;	// can't do nothin' right now
	}

	if(RangeToTarget(me,goodguy)<(128*FIXAMT) && Random(20)==0)
	{
		// get him!
		MakeSound(SND_SKELKICK,me->x,me->y,SND_CUTOFF,1200);
		me->seq=ANIM_ATTACK;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=256;
		me->action=ACTION_BUSY;
		me->dx=0;
		me->dy=0;
		me->reload=0;
		return;
	}

	WanderAI(me,512,10,10,map,world,goodguy,FIXAMT*3);
}

void AI_WindElder(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y;
	Guy *g;
	
	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_ELDEROUCH,me->x,me->y,SND_CUTOFF,1800);
		else
			MakeSound(SND_ELDERDIE,me->x,me->y,SND_CUTOFF,2000);
	}

	if(me->mind3)
		me->mind3--;
	else
		me->mind3=30*6;
	
	if(me->reload)
	me->reload--;

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm<6)
		{
			me->dx=Cosine(me->facing*32)*(6+me->frm*2);
			me->dy=Sine(me->facing*32)*(6+me->frm*2);
		}
		else if(me->seq==ANIM_ATTACK)
		{
			me->dx/=2;
			me->dy/=2;
		}
		if(me->seq==ANIM_ATTACK && me->frm>=3 && me->frm<=6)
		{
			x=me->x+Cosine(me->facing*32)*30;
			y=me->y+Sine(me->facing*32)*30;
			if(me->AttackCheck(32,x>>FIXSHIFT,y>>FIXSHIFT,goodguy))
				goodguy->GetShot(Cosine(me->facing*32)*16,Sine(me->facing*32)*16,8,map,world);
			me->reload=5;
		}
		if(me->seq==ANIM_A1 && me->frmTimer<128)
		{
			x=me->x-400*FIXAMT+Random(800*FIXAMT);
			y=me->y-300*FIXAMT+Random(600*FIXAMT);
			g=AddGuy(x,y,0,MONS_GHOST,me->friendly);
			if(g && !g->CanWalk(g->x,g->y,map,world))
				g->type=MONS_NONE;
		}
		if(me->seq==ANIM_A2 && me->frm==2 && me->reload==0)
		{
			FireBullet(me->x,me->y,me->facing-1+8,BLT_WIND,me->friendly);
			FireBullet(me->x,me->y,me->facing,BLT_WIND,me->friendly);
			FireBullet(me->x,me->y,me->facing+1,BLT_WIND,me->friendly);
			me->reload=4;
		}
		if(me->seq==ANIM_DIE)
		{
			x=me->x-40*FIXAMT+Random(80*FIXAMT);
			y=me->y-30*FIXAMT+Random(60*FIXAMT);
			//explode
		}
		return;	// can't do nothin' right now
	}

	if(me->mind3==0)
	{
		// summon hordes of ghosts
		MakeSound(SND_ELDERSUMMON,me->x,me->y,SND_CUTOFF,1200);
		DoMove2(me,ANIM_A1,128,1,0,0);
		return;
	}

	if(RangeToTarget(me,goodguy)<(300*FIXAMT) && Random(20)==0)
	{
		// charge at him
		MakeSound(SND_ELDERCHARGE,me->x,me->y,SND_CUTOFF,1200);
		DoMove2(me,ANIM_ATTACK,128,1,0,0);
		return;
	}
	if(Random(50)==0)
	{
		// fire wind missiles
		MakeSound(SND_ELDERSHOOT,me->x,me->y,SND_CUTOFF,1200);
		DoMove2(me,ANIM_A2,128,1,0,0);
		return;
	}

	WanderAI(me,1024,2,1,map,world,goodguy,FIXAMT*6);
}

void AI_EarthElder(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y;
	
	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_ELDEROUCH,me->x,me->y,SND_CUTOFF,1800);
		else
			MakeSound(SND_ELDERDIE,me->x,me->y,SND_CUTOFF,2000);
	}
	
	if(me->reload)
	me->reload--;

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm==4 && me->reload==0)
		{	//dirt spike
			FireBullet(me->x,me->y,(me->facing),BLT_DIRTSPIKE,me->friendly);
			FireBullet(me->x,me->y,(me->facing-1+8),BLT_DIRTSPIKE,me->friendly);
			FireBullet(me->x,me->y,(me->facing+1),BLT_DIRTSPIKE,me->friendly);
			me->reload=5;
		}

		if(me->seq==ANIM_A1 && me->frm==3 && me->reload==0)
		{
			for(x=0;x<6;x++)
				FireBullet(me->x,me->y,(byte)Random(256),BLT_BIGYELLOW,me->friendly);
			me->reload=5;
		}
		if(me->seq==ANIM_A2 && me->frm<9)
		{
			x=me->x+Cosine(me->facing*32)*me->frm*48;
			y=me->y+Sine(me->facing*32)*me->frm*32;
			if(me->frm==8 && me->reload==0)
			{
				me->reload=3;
				FireBullet(x,y,0,BLT_MEGABOOM,me->friendly);
			}
			else if(me->frmTimer<128)
			FireBullet(x,y,0,BLT_BOOM,me->friendly);
		}
		if(me->seq==ANIM_DIE)
		{
			MakeSound(SND_ELDERDIE,me->x,me->y-200*me->frm,SND_CUTOFF,2000);
			// doesn't explode
		}
		return;	// can't do nothin' right now
	}

	if(me->mind3)
		me->mind3-=2;
	else
		me->mind3=30*8;

	if(me->mind3==30*5 || me->mind3==30*3)
	{
		// launch grenades
		MakeSound(SND_ELDERSUMMON,me->x,me->y,SND_CUTOFF,1200);
		DoMove2(me,ANIM_A1,128,1,0,0);
		return;
	}

	if(me->mind3==30*8 || me->mind3==30*6 || me->mind3==30*4)
	{
		// shoot earth spikes
		MakeSound(SND_ELDERCHARGE,me->x,me->y,SND_CUTOFF,1200);
		DoMove2(me,ANIM_ATTACK,128,1,0,0);
		return;
	}

	if(me->mind3==30*7)
	{
		// boom!
		MakeSound(SND_ELDERCHARGE,me->x,me->y,SND_CUTOFF,1200);
		DoMove2(me,ANIM_A2,128,1,0,0);
		return;
	}


	FaceGoodguy(me,goodguy);

	WanderAI(me,1024,2,1,map,world,goodguy,FIXAMT*2);
}

void AI_WaterElder(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y,f;
	Guy *g;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_ELDEROUCH,me->x,me->y,SND_CUTOFF,1800);
		else
			MakeSound(SND_ELDERDIE,me->x,me->y,SND_CUTOFF,2000);
	}
	
	if(me->mind3)
		me->mind3--;
	else
		me->mind3=30*6;
	
	if(me->reload)
	me->reload--;

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm>=3 && me->frm<=6 && me->reload==0)
		{
			if(Random(3))
			FireBullet(me->x,me->y,me->facing*32+256-8+(byte)Random(16),BLT_FREEZE,me->friendly);
			else
			{FireBullet(me->x,me->y,me->facing*32+256-8+(byte)Random(16),BLT_SHARK,me->friendly);
			FireBullet(me->x,me->y,me->facing*32+256-8+(byte)Random(16),BLT_SHARK,me->friendly);}
			me->reload=2;
		}
		if(me->seq==ANIM_A2 && me->frm>2 && me->frm<10 && me->reload==0)
		{
			x=me->x+Cosine(me->facing*32)*30;
			y=me->y+Sine(me->facing*32)*30;
			f=me->facing*32-16+Random(33);
			FireBullet(x,y,f,BLT_BUBBLE,me->friendly);
			me->reload=2;
		}
		if(me->seq==ANIM_DIE)
		{
			x=me->x-40*FIXAMT+Random(80*FIXAMT);
			y=me->y-30*FIXAMT+Random(60*FIXAMT);
			//explosion
		}
		return;	// can't do nothin' right now
	}

	if(Random(40)==0)
	{
		// fire ice shards
		MakeSound(SND_ELDERCHARGE,me->x,me->y,SND_CUTOFF,1200);
		DoMove2(me,ANIM_ATTACK,128,1,me->dx/2,me->dx/y);
		return;
	}
	if(Random(30)==0)
	{
		// fire bubbles
		MakeSound(SND_ELDERSHOOT,me->x,me->y,SND_CUTOFF,1200);
		DoMove2(me,ANIM_A2,128,1,me->dx/2,me->dx/y);
		return;
	}
	
	FaceGoodguy2(me,goodguy);
	me->dx+=Cosine(me->facing*32)*3/2;
	me->dy+=Sine(me->facing*32)*3/2;
	Clamp(&me->dx,FIXAMT*8);
	Clamp(&me->dy,FIXAMT*8);
}

void AI_FireElder(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y;
	
	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_ELDEROUCH,me->x,me->y,SND_CUTOFF,1800);
		else
			MakeSound(SND_ELDERDIE,me->x,me->y,SND_CUTOFF,2000);
	}
	
	if(me->reload)
	me->reload--;

	map->BrightTorch((me->x/TILE_WIDTH)>>FIXSHIFT,(me->y/TILE_HEIGHT)>>FIXSHIFT,16,8);

	if(me->ouch && me->action==ACTION_BUSY && me->seq!=ANIM_DIE && me->seq!=ANIM_ATTACK && Random(10)==0)
	{
		// run away from the water
		DoMove2(me,ANIM_ATTACK,128,1,0,0);
		me->facing=(byte)Random(8);
		return;
	}
	if(me->mind3)
		me->mind3--;
	else
		me->mind3=30*8;

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm<6)
		{
			me->dx=Cosine(me->facing*32)*(6+me->frm*2);
			me->dy=Sine(me->facing*32)*(6+me->frm*2);
		}
		else if(me->seq==ANIM_ATTACK)
		{
			me->dx/=2;
			me->dy/=2;
		}
		if(me->seq==ANIM_ATTACK && me->frm>=3 && me->frm<=6)
		{
			x=me->x+Cosine(me->facing*32)*30;
			y=me->y+Sine(me->facing*32)*30;
			if(me->AttackCheck(32,x>>FIXSHIFT,y>>FIXSHIFT,goodguy))
				goodguy->GetShot(Cosine(me->facing*32)*16,Sine(me->facing*32)*16,8,map,world);
			me->reload=5;
		}
		if(me->seq==ANIM_A1 && me->frm==1)
			me->mind2=me->facing*32;
		if(me->seq==ANIM_A1 && me->frm>0 && me->frm<22)
		{
			me->mind2+=8;
			me->facing=me->mind2/32;
			x=me->x+20*Cosine(me->mind2);
			y=me->y+20*Sine(me->mind2);
			FireBullet(x,y,me->mind2,BLT_FLAME5,me->friendly);
		}

		if(me->seq==ANIM_A2 && me->frm>4 && me->frm<9 && me->frmTimer<128)
		{
			x=me->x+Cosine(me->facing*32)*(me->frm-4)*48;
			y=me->y+Sine(me->facing*32)*(me->frm-4)*32;
			if(me->frm==8 && me->reload==0)
			{
				me->reload=3;
				FireBullet(x,y,0,BLT_MEGABOOM,me->friendly);
			}
			else if(me->frmTimer<128)
				FireBullet(x,y,0,BLT_BOOM,me->friendly);
		}

		if(me->seq==ANIM_DIE)
		{
			x=me->x-40*FIXAMT+Random(80*FIXAMT);
			y=me->y-30*FIXAMT+Random(60*FIXAMT);
			//explode
		}
		return;	// can't do nothin' right now
	}

	x=me->x-40*FIXAMT+Random(80*FIXAMT);
	y=me->y-30*FIXAMT+Random(60*FIXAMT);
	FireBullet(x,y,me->facing,BLT_FLAME,me->friendly);

	if(me->mind3==0 || me->mind3==30*4)
	{
		// unleash flame circle
		MakeSound(SND_ELDERSUMMON,me->x,me->y,SND_CUTOFF,1200);
		DoMove2(me,ANIM_A1,128,1,0,0);
		return;
	}

	if(me->mind3==30*2 || me->mind3==30*6)
	{
		// fire explosion stream
		MakeSound(SND_ELDERSHOOT,me->x,me->y,SND_CUTOFF,1200);
		DoMove2(me,ANIM_A2,128,1,0,0);
		return;
	}

	WanderAI(me,1024,2,1,map,world,goodguy,FIXAMT*6);
}

void AI_SummonElder(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y,a,b;
	Guy *g;
	
	if(me->seq==ANIM_DIE && Random(5)==0)
	{
		y=me->y-FIXAMT*10+Random(FIXAMT*11);
		x=me->x-32*FIXAMT+Random(65*FIXAMT);
		FireBullet(x,y,0,BLT_EVILFACE,me->friendly);
	}
	
	if(me->reload)
	me->reload--;

	map->BrightTorch((me->x/TILE_WIDTH)>>FIXSHIFT,(me->y/TILE_HEIGHT)>>FIXSHIFT,8,8);

	if(me->mind3)
		me->mind3--;
	else
		me->mind3=30*4;

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_A1 && me->frm>0 && me->frm<22 && me->reload==0)
		{
			a=Random(256);
			b=Random(60)+20;
			x=me->x+b*Cosine(a);
			y=me->y+b*Sine(a);
			
			if (me->mind2<5)
			g=AddGuy(x,y,FIXAMT*40,MONS_VAMPLORD,me->friendly);
			else if (me->mind2<5+15)
			g=AddGuy(x,y,FIXAMT*40,MONS_SPIKEY,me->friendly);
			else if (me->mind2<5+15+25)
			g=AddGuy(x,y,FIXAMT*40,MONS_DRACULITE,me->friendly);
			else
			g=AddGuy(x,y,FIXAMT*40,MONS_VAMPIRE,me->friendly);
		
			if(g && !g->CanWalk(g->x,g->y,map,world))
				g->type=MONS_NONE;
			else
			{
				MakeSound(SND_TELEPORT,me->x,me->y,SND_CUTOFF,600);
				map->BrightTorch((me->x/TILE_WIDTH)>>FIXSHIFT,
							 (me->y/TILE_HEIGHT)>>FIXSHIFT,18,8);
			}
			me->reload=(me->mind2+1)*4;
		}

		if(me->seq==ANIM_DIE)
		{
			x=me->x-40*FIXAMT+Random(80*FIXAMT);
			y=me->y-30*FIXAMT+Random(60*FIXAMT);
			//explode
		}
		return;	// can't do nothin' right now
	}

	if(me->mind3==0 || me->mind3==30*4)
	{
		// summon minions
		MakeSound(SND_ELDERSUMMON,me->x,me->y,SND_CUTOFF,1200);
		DoMove2(me,ANIM_A1,128,1,0,0);
		me->mind2=(byte)Random(100);
		return;
	}

	WanderAI(me,1024,2,1,map,world,goodguy,FIXAMT*6);
}

//evilizer
void AI_Evilizer(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int summonTab[]={MONS_CRAZYBONE,MONS_MUTANT,MONS_CREEPAZOID,MONS_MUMBLE2,MONS_ALIEN,MONS_REDBONE,MONS_WHATSIT,MONS_VAMPIRE,MONS_BAT,MONS_NIGHTVAMP,MONS_EYEGUY,MONS_STAREYBAT,MONS_MADMAN,MONS_WACKO,MONS_GHOST,MONS_SHOCKGHOST,MONS_MUMBLE,MONS_WOLF,MONS_WOLF2,MONS_BOMBIE}; //20
	int summonTab2[]={MONS_SUPERZOMBIE,MONS_BIGBAT,MONS_TOWER,MONS_MUSH,MONS_STICKMAN,MONS_MAMASPDR,MONS_MINIFRANK,MONS_EVILCLONE,MONS_CRAZYPANTS,}; //9

	int x,y;
	Guy *g;
	
	if (me->ouch == 4)
	{
		if (me->hp > 0)
			MakeSound(SND_SDZLOUCH, me->x, me->y, SND_CUTOFF, 1200);
		else
		{MakeSound(SND_ELDERDIE, me->x, me->y, SND_CUTOFF, 1200);
		KillKids(me);}
	}
	
	if (me->mind4 > 0 && me->mind == 1)
	{
		me->mind4--;
		x=me->x>>FIXSHIFT;
		y=me->y>>FIXSHIFT;
		BlowUpGuy(x+me->rectx,y+me->recty,x+me->rectx2,y+me->recty2,me->z,2);
	}

	// mind=whether uncovered or not
	if(me->mind==0)
		me->facing=0;
	else
		me->facing=16;
	
	if(me->mind==0 && me->hp < me->maxHP*0.5)
	{
		me->mind = 1;
		me->mind4 = 40;
	}
	
	// mind1=time to shoot an evilface off
	if(me->mind1==0)
	{
		y=me->y-FIXAMT;
		x=me->x-140*FIXAMT+Random(281*FIXAMT);
		FireBullet(x,y,0,BLT_EVILFACE,me->friendly);
		me->mind1=12;
		MakeSound(SND_EVILIZER,me->x,me->y,SND_CUTOFF,300);
	}
	else
	{
		me->mind1--;
		if(!(me->mind1&3))
		{
			y=me->y-FIXAMT;
			x=me->x-140*FIXAMT+Random(281*FIXAMT);
			FireBullet(x,y,0,BLT_EVILFACE,-me->friendly);
		}
	}
	
	if(me->reload)
		me->reload--;

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_DIE)
		{
			x=me->x>>FIXSHIFT;
				y=me->y>>FIXSHIFT;
				BlowUpGuy(x+me->rectx,y+me->recty,x+me->rectx2,y+me->recty2,me->z,2);
				BlowSmoke((x+me->rectx+Random(me->rectx2-me->rectx))<<FIXSHIFT,
						  (y+me->recty+Random(me->recty2-me->recty))<<FIXSHIFT,
						  me->z,FIXAMT);
		}
	}

	// mind2=grenade timer (faster as it gets hurt more)
	if(me->mind2==0)
		me->mind2=30+me->hp/80;
	else
	{
		me->mind2--;
		if(me->mind2==0)
		{
			x=180*FIXAMT;
			y=180*FIXAMT;
			FireBullet(me->x-x,me->y,64+(byte)Random(128),BLT_REDNADE,me->friendly);
			FireBullet(me->x+x,me->y,192+(byte)Random(128),BLT_REDNADE,me->friendly);
			FireBullet(me->x,me->y-y,128+(byte)Random(128),BLT_REDNADE,me->friendly);
			FireBullet(me->x,me->y+y,(byte)Random(128),BLT_REDNADE,me->friendly);
			MakeSound(SND_MUSHMISSILE,me->x,me->y,SND_CUTOFF,1200);
		}
	}

	// mind3=missile timer (only works once exposed)
	if(me->mind==1)
	{
		if(me->mind3==0)
			me->mind3=60+me->hp/60;
		else
		{
			me->mind3--;
			if(me->mind3==0)
			{
				x=100*FIXAMT;
				y=40*FIXAMT;
				FireBullet(me->x-x,me->y-y,160,BLT_MISSILE,me->friendly);
				FireBullet(me->x+x,me->y-y,224,BLT_MISSILE,me->friendly);
				FireBullet(me->x-x,me->y+y,96,BLT_MISSILE,me->friendly);
				FireBullet(me->x+x,me->y+y,32,BLT_MISSILE,me->friendly);
				MakeSound(SND_FRANKMISSILE,me->x,me->y,SND_CUTOFF,1200);
			}
		}
	}
	
	// reload=summon super duper monsters, only when exposed
		if (me->reload == 30 && me->mind==1 && Random(2))
			{
				MakeSound(SND_TELEPORT,me->x,me->y,SND_CUTOFF,1200);
				x=((Random(map->width-6)+3)*TILE_WIDTH+TILE_WIDTH/2)*FIXAMT;
				y=((Random(map->height-6)+3)*TILE_HEIGHT+TILE_HEIGHT/2)*FIXAMT;
				g=AddGuy(x,y,FIXAMT*80,summonTab[Random(20)],me->friendly);
				map->BrightTorch((g->x/TILE_WIDTH)>>FIXSHIFT,
							 (g->y/TILE_HEIGHT)>>FIXSHIFT,18,8);
			}
		else if(me->reload==0)
			{
				x=((Random(map->width-6)+3)*TILE_WIDTH+TILE_WIDTH/2)*FIXAMT;
				y=((Random(map->height-6)+3)*TILE_HEIGHT+TILE_HEIGHT/2)*FIXAMT;
				if(Random(3) && me->mind == 1)
				{g=AddGuy(x,y,FIXAMT*40,summonTab2[Random(20)],me->friendly);
				me->reload=60*Random(6);}
				else
				{g=AddGuy(x,y,FIXAMT*40,summonTab[Random(9)],me->friendly);
				me->reload=30*Random(3);}
				if(g && !g->CanWalk(g->x,g->y,map,world))
					g->type=MONS_NONE;
				else
				{map->BrightTorch((g->x/TILE_WIDTH)>>FIXSHIFT,(g->y/TILE_HEIGHT)>>FIXSHIFT,18,8);
				MakeSound(SND_TELEPORT,g->x,g->y,SND_CUTOFF,1200);}
			}
}

void AI_EvilPump(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y,i;

	if(me->reload)
		me->reload--;

	FaceGoodguy3(me,goodguy);

	if(me->ouch==4)
		MakeSound(SND_GLASSHIT,me->x,me->y,SND_CUTOFF,1200);

	if(me->seq==ANIM_IDLE && me->frm==8 && me->frmTimer<128)
	{
		for(i=0;i<16;i++)
		FireExactBullet(me->x,me->y,FIXAMT*64,Cosine(i*16)*4,Sine(i*16)*4,FIXAMT*-0.5,0,120,0,BLT_BIGSHELL,me->friendly);
		me->reload=3;
	}
	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && (me->frm==3 || me->frm==5 || me->frm==7) && me->reload==0)
		{
			MakeSound(SND_ALIENSPIT,me->x,me->y,SND_CUTOFF,1200);
			x=me->x+Cosine(me->facing*32)*48;
			y=me->y+Sine(me->facing*32)*48;
			FireBullet(x,y,(byte)(me->facing*32+240+Random(33)),BLT_GRENADE,me->friendly);
			me->reload=3;
			if(me->frm==7)
				me->reload=30*3;
		}
		if(me->seq==ANIM_DIE)
		{
			if(me->frm==0)
				me->reload=0;
			x=me->x-20*FIXAMT+Random(40*FIXAMT);
			y=me->y-20*FIXAMT+Random(40*FIXAMT);
				BlowSmoke((x+me->rectx+Random(me->rectx2-me->rectx))<<FIXSHIFT,
					  (y+me->recty+Random(me->recty2-me->recty))<<FIXSHIFT,
					  me->z,FIXAMT);
				BlowUpGuy(x+me->rectx,y+me->recty,x+me->rectx2,y+me->recty2,me->z,1);
			if(me->frm==4 && me->reload==0)
			{
				me->reload=10;
			}
		}
		return;
	}

	if(me->reload==0)
	{
		DoMove2(me,ANIM_ATTACK,128,1,0,0);
	}
}

void AI_Bonkula(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y,a,i;
	Guy *g;
	dword d;
	
	if (me->reload)
	me->reload--;

	if(map->GetTile(me->mapx,me->mapy)->light>0 && me->ouch==0 && me->hp>0)
	{
		d=monsType[me->type].flags;
		monsType[me->type].flags=0;
		me->GetShot(0,0,map->GetTile(me->mapx,me->mapy)->light,map,world);
		monsType[me->type].flags=d;
		BlowSmoke(me->x,me->y,FIXAMT*10,Random(6)*FIXAMT);
		BlowSmoke(me->x,me->y,FIXAMT*10,Random(6)*FIXAMT);
		BlowSmoke(me->x,me->y,FIXAMT*10,Random(6)*FIXAMT);
		BlowSmoke(me->x,me->y,FIXAMT*10,Random(6)*FIXAMT);
		BlowSmoke(me->x,me->y,FIXAMT*10,Random(6)*FIXAMT);
	}
	
	if(me->reload)
		me->reload--;
	
	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_BONKOUCH,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_BONKDIE,me->x,me->y-200*me->frm,SND_CUTOFF,2000);
	}
	
	if(me->ouch==4)
	{
		x=me->x-40*FIXAMT+Random(80*FIXAMT);
		y=me->y-30*FIXAMT+Random(60*FIXAMT);
		FireBullet(x,y,0,BLT_BADSITFLAME,me->friendly);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm==8 && me->reload==0)
		{
			MakeSound(SND_BOMBBOOM,me->x,me->y,SND_CUTOFF,1200);
			ShakeScreen(5);
			x=me->x+Cosine(me->facing*32)*60;
			y=me->y+Sine(me->facing*32)*60;

			FindVictim(x>>FIXSHIFT,y>>FIXSHIFT,6*15,Cosine(me->facing*32)*2,
							   Sine(me->facing*32)*2,24,map,world,me->friendly);
			me->reload=5;
		}

		if(me->seq==ANIM_DIE)
		{
			me->dx=0;
			me->dy=0;
			x=me->x-40*FIXAMT+Random(80*FIXAMT);
			y=me->y-30*FIXAMT+Random(60*FIXAMT);
			FireBullet(x,y,0,BLT_BADSITFLAME,me->friendly);
		}
		if(me->seq==ANIM_A1)
		{
			me->facing++;
			if(me->facing>7)
				me->facing=0;
			if(me->reload==0)
			{
				me->reload=9;
				me->mind3+=4;	// rotate the angle of fire
				for(i=0;i<6;i++)
				{
					a=(me->mind3+(i*256)/6)&255;
					x=me->x+Cosine(a)*6;
					y=me->y+Sine(a)*6;
					FireBullet(x,y,a,BLT_ENERGY_BOUNCE,me->friendly);
				}
				MakeSound(SND_BULLETFIRE,me->x,me->y,SND_CUTOFF,1200);
			}
		}
		return;	// can't do nothin' right now
	}

	if(me->mind3==0)
	{
		// begin spinning
		DoMove2(me,ANIM_A1,256,1,0,0);
		me->mind3=60;
	}
	else
	{
		if(me->mind3==60)
		{
			FaceGoodguy(me,goodguy);
			me->dx=Cosine(me->facing*32)*8;
			me->dy=Sine(me->facing*32)*8;
			me->dx+=-FIXAMT*2+Random(4*FIXAMT);
			me->dy+=-FIXAMT*2+Random(4*FIXAMT);
		}
		me->mind3--;
		if(RangeToTarget(me,goodguy)<200*FIXAMT)
		{
			//MakeSound(SND_BONKSWING,me->x,me->y,SND_CUTOFF,2000);
			DoMove2(me,ANIM_ATTACK,256,1,0,0);
			me->mind3=0;
		}
	}
}

void AI_Polterguy(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y,i;
	Guy *g;

	if(me->reload)
		me->reload--;

	if(me->dx==0 && me->dy==0)
	{
		me->dx=FIXAMT*4;
		me->dy=FIXAMT*4;
	}

	if(me->mind1&1)
		me->dx=-me->dx;
	if(me->mind1&2)
		me->dy=-me->dy;
	me->mind1=0;

	if(me->ouch==4)
	{
		if(me->hp==0)
			MakeSound(SND_POLTERDIE,me->x,me->y,SND_CUTOFF,2000);
		else
			MakeSound(SND_POLTEROUCH,me->x,me->y,SND_CUTOFF,2000);
	}
	
	if(me->mind==0)
	{
		DoMove2(me,ANIM_MOVE,128,1,0,0);
		me->mind=1;
		for(i=0;i<15;i++)
			LaunchJunk(map,me);
		me->reload=60;
		me->mind2=120+15;
		me->facing=(byte)(Random(4)+1)+32;
		me->dx=Cosine(me->facing)*6;
		me->dy=Sine(me->facing)*6;
	}

	if(me->mind2==0 && me->seq!=ANIM_ATTACK)
	{
		DoMove2(me,ANIM_ATTACK,128,1,0,0);
		MakeSound(SND_GHOSTYELL,me->x,me->y,SND_CUTOFF,1200);
	}
	else
		me->mind2--;

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_DIE)
		{
			x=me->x>>FIXSHIFT;
			y=me->y>>FIXSHIFT;
			BlowSmoke((x+me->rectx+Random(me->rectx2-me->rectx))<<FIXSHIFT,
					  (y+me->recty+Random(me->recty2-me->recty))<<FIXSHIFT,
					  me->z,FIXAMT);
		}
		if(me->seq==ANIM_ATTACK && me->frm==4 && me->reload==0)
		{
			LaunchJunk(map,me);
			LaunchJunk(map,me);
			me->mind2=30*2;
			me->reload=30;

			x=me->x-300*FIXAMT+Random(600)*FIXAMT;
			y=me->y-200*FIXAMT+Random(500)*FIXAMT;
			g=AddGuy(x,y,FIXAMT*50,MONS_GHOST,me->friendly);
			if(g && !g->CanWalk(g->x,g->y,map,world))
			{
				g->type=MONS_NONE;
			}
			else if(g)
			{
				DoMove2(g,ANIM_A1,128,1,0,0);
				g->frm=4;
			}
		}
		if(me->seq==ANIM_A1 && me->frm==15 && me->frmTimer<128)
		{
			Sneeze();
			MakeSound(SND_SPHINXSNEEZE,me->x,me->y,SND_CUTOFF,1200);
		}
		return;
	}

	if(FindVictim(me->x>>FIXSHIFT,me->y>>FIXSHIFT,55,0,0,1,map,world,me->friendly))
	{
		PoisonVictim(GetLastGuyHit(),60);
	}
	
	if(me->seq!=ANIM_IDLE)
	{
		me->seq=ANIM_IDLE;
		me->frm=0;
		me->frmTimer=0;
	}
}

void AI_Ghastly(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int i;
	int x,y;
	Guy *g;
	
	if(me->reload)
	me->reload--;

	if(me->mind==0 && RangeToTarget(me,goodguy)<256*FIXAMT)
	{
		me->mind=1;
		me->reload=60;
		me->mind3=120+15;
		me->facing=(byte)Random(256);
		me->dx=Cosine(me->facing)*2;
		me->dy=Sine(me->facing)*2;
	}

	if(me->mind1)
	{
		if(me->mind1&1)
			me->dx=-me->dx;
		if(me->mind1&2)
			me->dy=-me->dy;

		if(Random(20)==0)
		{
			me->facing=(byte)Random(256);
			me->dx=Cosine(me->facing)*2;
			me->dy=Sine(me->facing)*2;
		}
		me->mind1=0;
	}

	if(Random(4)==0)
		FireBullet(me->x,me->y,(byte)Random(256),BLT_SPORE,me->friendly);

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm==4 && me->reload==0)
		{
			me->reload=30*4;

			for(i=0;i<3;i++)
			{
				x=me->x-30*FIXAMT+Random(61)*FIXAMT;
				y=me->y-20*FIXAMT+Random(51)*FIXAMT;
				g=AddGuy(x,y,FIXAMT*30,MONS_GASBAT,me->friendly);
				if(g && !g->CanWalk(g->x,g->y,map,world))
				{
					g->type=MONS_NONE;
				}
			}
		}
		if(me->seq==ANIM_A1 && me->frm==15 && me->frmTimer<128)
		{
			for(i=0;i<16;i++)
				FireBullet(me->x,me->y,i*16,BLT_SPORE,me->friendly);
		}
		return;	// can't do nothin' right now
	}

	if(me->reload==0)
	{
		MakeSound(SND_POLTERGET,me->x,me->y,SND_CUTOFF,1200);
		DoMove2(me,ANIM_ATTACK,128,1,me->dx,me->dy);
	}
	if(Random(50)==0)
	{
		me->facing=(byte)Random(256);
		me->dx=Cosine(me->facing)*2;
		me->dy=Sine(me->facing)*2;
	}
}

void AI_Junk(Guy *me,Map *map,world_t *world,Guy *goodguy)
{;

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_DIE)
		{
			ExplodeParticles(PART_DIRT,me->x,me->y,me->z,16);
		}
		return;
	}
	if(me->parent==NULL || me->parent->hp==0)
	{
		me->hp=0;
		DoMove2(me,ANIM_DIE,128,1,0,0);
	}

	if(RangeToTarget(me,goodguy)<20*FIXAMT)
	{
		goodguy->GetShot(me->dx,me->dy,2,map,world);
	}

	me->frmAdvance=180;
	FaceGoodguy(me,me->parent);
	me->dx+=Cosine(me->facing*32)/2;
	me->dy+=Sine(me->facing*32)/2;
	// circling Polterguy
	if(me->mind==0)
	{
		if(me->x<me->parent->x)
		{
			// on left side
			if(me->y<me->parent->y)
			{
				// upper left
				me->dy+=FIXAMT;
			}
			else
				// lower left
				me->dx+=FIXAMT;
		}
		else
		{
			// on right side
			if(me->y<me->parent->y)
			{
				// upper left
				me->dx-=FIXAMT;
			}
			else
				// lower left
				me->dy-=FIXAMT;
		}
		Dampen(&me->dx,FIXAMT/4);
		Dampen(&me->dy,FIXAMT/4);
		Clamp(&me->dx,FIXAMT*11);
		Clamp(&me->dy,FIXAMT*11);
	}
	else
	{
		// flying outward
		me->mind1--;
		if(me->mind1==0)
			me->mind=0;
	}
}

void AI_MiniFrankenjulie(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y;

	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
		{
			MakeSound(SND_FRANKOUCH2,me->x,me->y,SND_CUTOFF,1200);

			if(me->hp < (me->maxHP*0.66) && me->mind==0)
			{
				me->mind=1;
				x=me->x>>FIXSHIFT;
				y=me->y>>FIXSHIFT;
				BlowUpGuy(x+me->rectx,y+me->recty,x+me->rectx2,y+me->recty2,me->z,1);
				BlowUpGuy(x+me->rectx,y+me->recty,x+me->rectx2,y+me->recty2,me->z,1);
				BlowUpGuy(x+me->rectx,y+me->recty,x+me->rectx2,y+me->recty2,me->z,1);
				DoMove2(me,ANIM_A1,256,0,0,0);
			}
			if(me->hp < (me->maxHP*0.33) && me->mind==1)
			{
				me->mind=2;
				x=me->x>>FIXSHIFT;
				y=me->y>>FIXSHIFT;
				BlowUpGuy(x+me->rectx,y+me->recty,x+me->rectx2,y+me->recty2,me->z,1);
				BlowUpGuy(x+me->rectx,y+me->recty,x+me->rectx2,y+me->recty2,me->z,1);
				BlowUpGuy(x+me->rectx,y+me->recty,x+me->rectx2,y+me->recty2,me->z,1);
				DoMove2(me,ANIM_A1,256,0,0,0);
			}
		}
		else
		{
			MakeSound(SND_FRANKDIE,me->x,me->y,SND_CUTOFF,1200);
		}
	}

	if(me->mind2==0)
	{
		if(RangeToTarget(me,goodguy)<256*FIXAMT || me->ouch)
		{
			me->mind2=1;
		}
		return;
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_DIE)
		{
			x=me->x>>FIXSHIFT;
			y=me->y>>FIXSHIFT;
			BlowUpGuy(x+me->rectx,y+me->recty,x+me->rectx2,y+me->recty2,me->z,1);
		}
		if(me->seq==ANIM_ATTACK)
		{
			//if(me->frmTimer<128)
				MakeSound(SND_FRANKHEAD,me->x,me->y,SND_CUTOFF,400);
			if(me->frm<16 && me->frmTimer<128)
			{
				x=(me->x+Cosine(me->frm*16)*40);
				y=(me->y+Sine(me->frm*16)*30);
				FireExactBullet(x,y,FIXAMT*40,Cosine(me->frm*16)*12,Sine(me->frm*16)*12,
					-FIXAMT*2,0,60,me->frm*16,BLT_ENERGY,me->friendly);
				x=(me->x+Cosine((me->frm+8)*16)*40);
				y=(me->y+Sine((me->frm+8)*16)*30);
				FireExactBullet(x,y,FIXAMT*40,Cosine((me->frm+8)*16)*12,Sine((me->frm+8)*16)*12,
					-FIXAMT*2,0,60,(me->frm+8)*16,BLT_ENERGY,me->friendly);
			}
		}
		if(me->seq==ANIM_A3)
		{
			if(me->frm>4 && me->frm<20)
			{
				me->dx=Cosine(me->facing*32)*18;
				me->dy=Sine(me->facing*32)*18;

				x=(me->x+Cosine((me->frm-4)*16)*100)/FIXAMT;
				y=(me->y+Sine((me->frm-4)*16)*80)/FIXAMT;
				if(me->AttackCheck(25,x,y,goodguy))
					goodguy->GetShot(Cosine((me->frm-4)*16)*8,Sine((me->frm-4)*16)*8,3,map,world);
				x=(me->x+Cosine((me->frm+4)*16)*100)/FIXAMT;
				y=(me->y+Sine((me->frm+4)*16)*80)/FIXAMT;
				if(me->AttackCheck(25,x,y,goodguy))
					goodguy->GetShot(Cosine((me->frm+4)*16)*8,Sine((me->frm+4)*16)*8,3,map,world);
			}
			else if(me->frm>=20)
			{
				me->dx=(me->dx*0.95);
				me->dy=(me->dy*0.95);
				if(me->frm==21)
					DoMove2(me,ANIM_A1,256,0,me->dx,me->dy);
			}
		}
		if(me->seq==ANIM_A4 && me->frm==4 && me->frmTimer<128)
		{
			x=me->x-60*FIXAMT;
			y=me->y-10*FIXAMT;
			FireBullet(x,y,me->facing,BLT_MISSILE,me->friendly);
			x=me->x+60*FIXAMT;
			y=me->y-10*FIXAMT;
			FireBullet(x,y,me->facing,BLT_MISSILE,me->friendly);
			MakeSound(SND_FRANKMISSILE,me->x,me->y,SND_CUTOFF,1200);
		}
		if(me->seq==ANIM_A4 && me->frm==10)
		{
			DoMove2(me,ANIM_A2,256,0,me->dx,me->dy);
		}
		return;
	}
	if(RangeToTarget(me,goodguy)<73*FIXAMT)
	{
		// 'acidic'
		goodguy->GetShot(0,0,3,map,world);
	}

	FaceGoodguy2(me,goodguy);
	me->dx+=Cosine(me->facing*32)/2;
	me->dy+=Sine(me->facing*32)/2;
	
	Clamp(&me->dx,FIXAMT*7);
	Clamp(&me->dy,FIXAMT*7);

	if(me->mind==0)	// have head
	{
		if(me->mind1==0)
			me->mind1=30*3;
		else
		{
			me->mind1--;
			if(me->mind1==0)
			{//void DoMove(Guy *me,int move,int frmAdv,byte busy,int dx,int dy)
				DoMove2(me,ANIM_ATTACK,256,1,me->dx,me->dy);
			}
		}
	}
	if(me->mind==1)	// lost head already
	{
		if(me->seq==ANIM_IDLE || me->seq==ANIM_MOVE || (me->seq==ANIM_A1 && me->frm>1))
		{
			DoMove2(me,ANIM_A1,256,0,me->dx,me->dy);
		}
		if(me->mind1==0)
			me->mind1=30*3;
		else
		{
			me->mind1--;
			if(me->mind1==0)
			{
				MakeSound(SND_FRANKARMS,me->x,me->y,SND_CUTOFF,1200);
				DoMove2(me,ANIM_A3,160,1,me->dx,me->dy);
				FaceGoodguy(me,goodguy);
			}
			if(me->mind1%15==0)
			{
				MakeSound(SND_FRANKARMS,me->x,me->y,SND_CUTOFF,1200);
				FireExactBullet(me->x,me->y,FIXAMT*128,Cosine(Random(255))*8,Sine(Random(255))*8,FIXAMT*4,0,120,0,BLT_GRENADE,me->friendly);
			}
		}
	}
	if(me->mind==2)	// lost body too
	{
		if(me->seq==ANIM_IDLE || me->seq==ANIM_MOVE || (me->seq==ANIM_A2 && me->frm>1))
		{
			DoMove2(me,ANIM_A2,256,0,me->dx,me->dy);
		}
		if(me->mind1==0)
			me->mind1=30*2;
		else
		{
			me->mind1--;
			if(me->mind1==0)
			{
				DoMove2(me,ANIM_A4,256,1,me->dx,me->dy);
			}
		}
	}
	
	if (me->seq==ANIM_A3)
	{if(me->mind3&1)
	{
		me->dx=-me->dx/2;
		me->mind2=10;
		me->facing=(byte)Random(8)*32;
	}
	if(me->mind3&2)
	{
		me->dy=-me->dy/2;
		me->mind2=10;
		me->facing=(byte)Random(8)*32;
	}}
	me->mind3=0;
}

void AI_Harry(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y,i,a,b,c;
	Guy *g;
	
	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_LARRYOUCH,me->x,me->y,SND_CUTOFF,1800);
		else
			MakeSound(SND_SWAMPDIE,me->x,me->y,SND_CUTOFF,2000);
	}

	if(me->mind3==0)
		me->mind3=30*4;
	if(me->mind3>1)
		me->mind3--;

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm==4)
		{
			me->dx=Cosine(me->facing*32)*10;
			me->dy=Sine(me->facing*32)*10;
		}
		if(me->seq==ANIM_ATTACK && me->frm>=6 && me->frm<=10)
		{
			Dampen(&me->dx,FIXAMT);
			Dampen(&me->dy,FIXAMT);
			if(me->AttackCheck(40,me->x>>FIXSHIFT,me->y>>FIXSHIFT,goodguy))
			{
				goodguy->GetShot(Cosine(me->facing*32)*8,Sine(me->facing*32)*8,8,map,world);
			}
			if(me->frm==8 && me->frmTimer<128)
			{
				x=me->x+Cosine(me->facing*32)*48;
				y=me->y+Sine(me->facing*32)*48;
				if(me->AttackCheck(128,x>>FIXSHIFT,y>>FIXSHIFT,goodguy))
				{
					goodguy->GetShot(Cosine(me->facing*32)*8,Sine(me->facing*32)*8,8,map,world);
				}
				ShakeScreen(15);
			}
			if(me->frm==10)
			{
				me->dx=0;
				me->dy=0;
			}
		}
		if(me->seq==ANIM_A1 && me->frm==6 && me->frmTimer<128)
		{
			x=me->x+Cosine((me->facing+7+2*(7-me->frm))*32)*20;
			y=me->y+Sine((me->facing+7+2*(7-me->frm))*32)*20;
			MakeSound(SND_LARRYDARTS,me->x,me->y,SND_CUTOFF,1200);
			for(i=0;i<10;i++)
			{
				FireBullet(x,y,(me->facing*32+(me->frm-6)*4+256-24+i*(48/10))&255,BLT_SPINE,me->friendly);
			}
			me->reload=5;
		}

		if(me->seq==ANIM_A2 && me->frm>=5 && me->frm<=15 && me->reload==0)
		{
			a=Random(256);
			b=Random(20)+20;
			c=Random(20)+20;
			x=me->x+b*Cosine(a);
			y=me->y+c*Sine(a);

			g=AddGuy(x,y,FIXAMT*40,MONS_DIREWOLF,me->friendly);
			if(g && !g->CanWalk(g->x,g->y,map,world))
				g->type=MONS_NONE;
			else
			{
				//MakeRingParticle(x,y,FIXAMT*40,20,40);
				BlowUpGuy(x+me->rectx,y+me->recty,x+me->rectx2,y+me->recty2,me->z,1);
			}
			me->reload=3;
		}
		if(me->seq==ANIM_DIE)
		{
			x=me->x-20*FIXAMT+Random(40*FIXAMT);
			y=me->y-20*FIXAMT+Random(40*FIXAMT);
			BlowUpGuy(x+me->rectx,y+me->recty,x+me->rectx2,y+me->recty2,me->z,1);
			if(Random(2)==0)
			{
				y=me->y-FIXAMT*10+Random(FIXAMT*11);
				x=me->x-32*FIXAMT+Random(65*FIXAMT);
				FireBullet(x,y,0,BLT_EVILFACE,me->friendly);
			}
		}
		return;	// can't do nothin' right now
	}

	if(RangeToTarget(me,goodguy)<64*FIXAMT && Random(4)==0 &&
		map->CheckLOS(me->mapx,me->mapy,5,goodguy->mapx,goodguy->mapy))
	{
		// get him!
		MakeSound(SND_LARRYPOUNCE,me->x,me->y,SND_CUTOFF,1200);
		DoMove2(me,ANIM_ATTACK,128,1,0,0);
		FaceGoodguy2(me,goodguy);
		return;
	}
	if(Random(32)==0)	// fire at any range
	{
		// shoot darts
		MakeSound(SND_LARRYPOUNCE,me->x,me->y,SND_CUTOFF,1200);
		DoMove2(me,ANIM_A1,200,1,0,0);
		FaceGoodguy2(me,goodguy);
		return;
	}

	if(me->mind3==1)
	{
		me->mind3=0;
		MakeSound(SND_WOLFHOWL,me->x,me->y,SND_CUTOFF,1000);
		DoMove2(me,ANIM_A2,128,1,0,0);
		return;
	}

	WanderAI(me,256,20,3,map,world,goodguy,FIXAMT*5);
}

void AI_MechaBonkula(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y;
	Guy *g;

	if(me->reload)
		me->reload--;

	if(me->ouch==3)
	{
		if(me->hp>0)
			MakeSound(SND_BONKOUCH,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_BONKDIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm==8 && me->reload==0)
		{
			MakeSound(SND_BONKHIT,me->x,me->y,SND_CUTOFF,1200);
			x=me->x+Cosine(me->facing*32)*60;
			y=me->y+Sine(me->facing*32)*60;

			FindVictim(x>>FIXSHIFT,y>>FIXSHIFT,6*15,Cosine(me->facing*32)*2,
							   Sine(me->facing*32)*2,8,map,world,me->friendly);
			//MakeRingParticle(x,y,0,6*15,10*15);
			me->reload=5;
		}

		if(me->seq==ANIM_DIE)
		{
			me->dx=0;
			me->dy=0;
			x=me->x-40*FIXAMT+Random(80*FIXAMT);
			y=me->y-30*FIXAMT+Random(60*FIXAMT);
			//FireBullet(x,y,0,BLT_FLAMEWALL);
			//ExplodeParticles(PART_YELLOW,x,y,Random(10*FIXAMT),5);
			MakeSound(SND_BONKDIE,me->x,me->y-200*me->frm,SND_CUTOFF,2000);
		}
		if(me->seq==ANIM_A1)
		{
			me->facing++;
			if(me->facing>7)
				me->facing=0;
			if(Random(30)==0)
			{
				x=me->x-300*FIXAMT+Random(600)*FIXAMT;
				y=me->y-300*FIXAMT+Random(600)*FIXAMT;
				g=AddGuy(x,y,FIXAMT*50,MONS_VAMPIRE,me->friendly);
				if(g && !g->CanWalk(g->x,g->y,map,world))
				{
					g->type=MONS_NONE;
				}
			}
		}
		return;	// can't do nothin' right now
	}

	if(me->mind3==0)
	{
		// begin spinning
		DoMove2(me,ANIM_A1,256,1,0,0);
		me->mind3=60;
	}
	else
	{
		if(me->mind3==60)
		{
			FaceGoodguy(me,goodguy);
			me->dx=Cosine(me->facing*32)*6;
			me->dy=Sine(me->facing*32)*6;
			me->dx+=-FIXAMT*2+Random(4*FIXAMT);
			me->dy+=-FIXAMT*2+Random(4*FIXAMT);
		}
		me->mind3--;
		if(RangeToTarget(me,goodguy)<150*FIXAMT)
		{
			MakeSound(SND_BONKSWING,me->x,me->y,SND_CUTOFF,2000);
			DoMove2(me,ANIM_ATTACK,256,1,0,0);
			me->mind3=0;
		}
	}
}

void AI_Minitilda(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y;

	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_MATTIEOUCH,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_LOOKEYDIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_DIE && me->frm==5)
		{
			me->seq=ANIM_IDLE;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
			me->action=ACTION_IDLE;
			me->reload=100;
			me->facing=0;
			return;
		}
		if(me->seq==ANIM_DIE && me->frmTimer<128)
		{
			x=me->x>>FIXSHIFT;
			y=me->y>>FIXSHIFT;
			BlowUpGuy(x+me->rectx,y+me->recty,x+me->rectx2,y+me->recty2,me->z,1);
			BlowSmoke((x+me->rectx+Random(me->rectx2-me->rectx))<<FIXSHIFT,
					  (y+me->recty+Random(me->recty2-me->recty))<<FIXSHIFT,
					  me->z,FIXAMT);
			ShakeScreen(1);
		}
		if(me->seq==ANIM_ATTACK && me->frm>3 && me->frm<11 && me->reload==0)
		{
			// shoot lil lasers
			x=me->x;
			y=me->y+FIXAMT*8;
			MakeSound(SND_BEAM2,me->x,me->y,SND_CUTOFF,1200);
			FireBullet(x,y,1,BLT_BEAM,me->friendly);
			FireBullet(x,y,2,BLT_BEAM,me->friendly);
			FireBullet(x,y,3,BLT_BEAM,me->friendly);
			me->reload=0;
			me->mind=0;
			me->reload=100;
		}
		me->facing=0;
		return;
	}

	if((!me->reload) && Random(100)==0)
	{
		me->seq=ANIM_ATTACK;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=64;	// very very slow animation
		me->action=ACTION_BUSY;
		me->dx=0;
		me->dy=0;
	}

	// do everything the brain does
	AI_MattieBrain(me,map,world,goodguy);
}

void AI_PatrolZoid(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y;

	if(me->reload)
		me->reload--;
	if(me->mind3)
		me->mind3--;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_SERPENTOUCH,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_SERPENTDIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm==6 && me->reload==0 && goodguy)
		{
			// spit stuff
			x=me->x+Cosine(me->facing*32)*8;
			y=me->y+Sine(me->facing*32)*8;
			FireBullet(x,y,me->facing*32,BLT_MISSILE,me->friendly);
			FireBullet(x,y,(me->facing*32+4),BLT_MISSILE,me->friendly);
			FireBullet(x,y,(me->facing*32+8+4),BLT_MISSILE,me->friendly);
			me->reload=20;
			me->mind=0;
		}
		return;	// can't do nothin' right now
	}

	if(goodguy)
	{
		if(RangeToTarget(me,goodguy)<(256*FIXAMT) && me->reload==0&&
		TargetInSight(me,map,goodguy))
		{
			MakeSound(SND_SERPENTSPIT,me->x,me->y,SND_CUTOFF,1200);
			// spit on him!
			me->seq=ANIM_ATTACK;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
			me->action=ACTION_BUSY;
			me->dx=0;
			me->dy=0;
			me->reload=0;
			if(goodguy->aiType==MONS_BOUAPHA)
				player.spotted=30*8;
			return;
		}
	}
	
	if(me->mind)
		me->mind--;
	if(!me->mind)	// time to get a new direction
	{
		me->mind2=(me->mind2+2)&7;
		me->mind=Random(120)+8;
	}
	
	if (me->facing != me->mind2 && !me->mind3)
	{
		me->facing=(me->facing+1)&7;
		me->mind3 = 3;
	}

	me->dx=Cosine(me->facing*32)*3;
	me->dy=Sine(me->facing*32)*3;
	if(me->seq!=ANIM_MOVE)
	{
		me->seq=ANIM_MOVE;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=128;
	}
}

void AI_XenoEmpressHead(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y,i;
	byte f;
	
	if (me->mind2)
		me->mind2--;

	if(me->reload)
		me->reload--;
	
	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_LOOKEYOUCH,me->x,me->y,SND_CUTOFF,1200);
		else
			{MakeSound(SND_LOOKEYDIE,me->x,me->y,SND_CUTOFF,1200);
			me->parent->mind4=1;}
	}
	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_DIE)
		{
			x=me->x>>FIXSHIFT;
			y=me->y>>FIXSHIFT;
			BlowUpGuy(x+me->rectx,y+me->recty,x+me->rectx2,y+me->recty2,me->z,1);
			BlowSmoke((x+me->rectx+Random(me->rectx2-me->rectx))<<FIXSHIFT,
					(y+me->recty+Random(me->recty2-me->recty))<<FIXSHIFT,
					me->z,FIXAMT);
		}
		if(me->seq==ANIM_ATTACK && me->frm>4 && me->frm<8)
		{
			if(goodguy && RangeToTarget(me,goodguy)<700*FIXAMT)
				{
					for(i=0;i<3;i++)
					{
						// spit acid glob
						x=me->x+Cosine(me->facing*32)*20-FIXAMT*4+(Random(9)<<FIXSHIFT);
						y=me->y+Sine(me->facing*32)*20-FIXAMT*4+(Random(9)<<FIXSHIFT);
						f=me->facing*32-32+Random(65);

						FireBullet(x,y,f,BLT_ACID,me->friendly);
					}
				}
		}
		return;
	}
	else if(goodguy)
	{
		if(Random(32)==0 && RangeToTarget(me,goodguy)<800*FIXAMT && me->mind2==0)
		{
			// get him!
			me->mind2=60;
			MakeSound(SND_ALIENSPIT,me->x,me->y,SND_CUTOFF,1200);
			me->action=ACTION_BUSY;
			me->seq=ANIM_ATTACK;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
			me->reload=0;
			return;
		}
	}
	if(me->seq!=ANIM_MOVE)
	{
		me->seq=ANIM_MOVE;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=128;
	}
}

void AI_XenoEmpressBody(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y,h;
	if(me->reload)
		me->reload--;
	
	if (me->mind2)
		me->mind2--;
	
	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_LOOKEYOUCH,me->x,me->y,SND_CUTOFF,1200);
		else
			{MakeSound(SND_LOOKEYDIE,me->x,me->y,SND_CUTOFF,1200);
			KillKids(me);}
	}
	
	if (me->mind2==0 && me->mind4>0)
	{
		x=me->x+Cosine(me->facing*32)*16;
		y=me->y+Sine(me->facing*32)*16;
		h = Random(64)+32;
		FireBullet(x,y,h,BLT_ACID,me->friendly);
		me->reload=2;
	}
	
	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_DIE)
		{
			x=me->x>>FIXSHIFT;
			y=me->y>>FIXSHIFT;
			BlowUpGuy(x+me->rectx,y+me->recty,x+me->rectx2,y+me->recty2,me->z,1);
			BlowSmoke((x+me->rectx+Random(me->rectx2-me->rectx))<<FIXSHIFT,
					(y+me->recty+Random(me->recty2-me->recty))<<FIXSHIFT,
					me->z,FIXAMT);
		}
		if(me->seq==ANIM_ATTACK && me->frm>4 && me->frm<8)
		{
			if(goodguy && RangeToTarget(me,goodguy)<700*FIXAMT)
				{
					x=me->x;
					y=me->y+FIXAMT*8;
					LaunchMegabeam(x,y,me->ID);
				}
		}
		return;
	}
	else if(goodguy && me->mind4>0)
	{
		if(Random(32)==0 && RangeToTarget(me,goodguy)<800*FIXAMT && me->mind2==0)
		{
			// get him!
			me->mind2=60;
			MakeSound(SND_ALIENSPIT,me->x,me->y,SND_CUTOFF,1200);
			me->action=ACTION_BUSY;
			me->seq=ANIM_ATTACK;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
			me->reload=0;
			return;
		}
	}
}

void AI_XenoEmpressTail(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y,h;
	Guy *g;
	
	if(me->reload)
		me->reload--;
	else
	{
		if (me->mind2!=0)
		me->mind2=0;
	}
	
	if(me->ouch==4)
	{
		if(me->hp==0)
			MakeSound(SND_MARKMOVIE,me->x,me->y,SND_CUTOFF,1200);
	}
	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_DIE)
		{
			x=me->x>>FIXSHIFT;
			y=me->y>>FIXSHIFT;
			BlowUpGuy(x+me->rectx,y+me->recty,x+me->rectx2,y+me->recty2,me->z,1);
			BlowSmoke((x+me->rectx+Random(me->rectx2-me->rectx))<<FIXSHIFT,
					(y+me->recty+Random(me->recty2-me->recty))<<FIXSHIFT,
					me->z,FIXAMT);
		}
		else if(me->seq==ANIM_ATTACK && me->frm==6 && me->mind2==0 && me->reload==0)
		{
			me->mind2=1;
			BlowUpGuy(x+me->rectx,y+me->recty,x+me->rectx2,y+me->recty2,me->z,1);
			h = Random(64)+32;
			FireExactBullet(me->x,me->y,me->z+FIXAMT*40,Cosine(h)*10,Sine(h)*10,FIXAMT*10,0,60,h,BLT_EGG,me->friendly);
			me->reload=2;
		}
	}
	else if(goodguy)
	{	
		if(Random(32)==0 && RangeToTarget(me,goodguy)<800*FIXAMT && me->reload==0 && me->seq!=ANIM_ATTACK)
		{
			// get him!
			me->action=ACTION_BUSY;
			me->seq=ANIM_ATTACK;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
			me->reload=0;
			me->mind2 = 0;
			return;
		}
	}
}

void AI_ScreamQueen(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y;

	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp==0)
			MakeSound(SND_PUMPKINDIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->parent)
	{
		// sitting on The Thing's tongue
		switch(me->parent->frm)
		{
			case 4:
			case 5:
				me->y+=10*FIXAMT;
				break;
			case 6:
			case 7:
				me->y+=15*FIXAMT;
				break;
			case 8:
				me->parent=NULL;	// I am free!
				if(me->hp>0)
				{
					me->seq=ANIM_MOVE;
					me->frm=0;
					me->frmTimer=0;
					me->frmAdvance=128;
					me->mind1=200;
					me->mind=1;	// pop out in hunt mode
				}
				if(!me->CanWalk(me->x,me->y,map,world))
					me->type=MONS_NONE;	// vanish if this spot is occupied
				break;
		}
		return;
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->reload==0 && me->frm==5)
		{
			x=me->x+Cosine(me->facing*32)*16;
			y=me->y+Sine(me->facing*32)*16;
			FireBullet(x,y,me->facing,BLT_BEAM,me->friendly);
			MakeSound(SND_FRANKMISSILE,me->x,me->y,SND_CUTOFF,300);
			me->reload=4;
		}
		return;	// can't do nothin' right now
	}

	if(me->mind==0)	// not currently aware of goodguy
	{
		if(me->seq==ANIM_MOVE)
		{
			me->seq=ANIM_IDLE;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=64;
		}
		if(me->mind1<31)
			me->mind1++;

		if(me->mind1>30 && Random(50)==0)
		{
			switch(Random(16))
			{
				case 0:
					MakeSound(SND_BABYSNORE,me->x,me->y,SND_CUTOFF,800);
					me->seq=ANIM_A1;	// take a nap
					me->frm=0;
					me->frmTimer=0;
					me->frmAdvance=32;
					me->mind1=0;
					me->action=ACTION_BUSY;
					break;
				case 1:
				case 2:
				case 3:

					me->seq=ANIM_A3;	// look right
					me->frm=0;
					me->frmTimer=0;
					me->frmAdvance=64;
					me->mind1=0;
					me->action=ACTION_BUSY;
					break;
				case 4:
				case 5:
				case 6:
					me->seq=ANIM_A4;	// look left
					me->frm=0;
					me->frmTimer=0;
					me->frmAdvance=64;
					me->mind1=0;
					me->action=ACTION_BUSY;
					break;
				case 7:
				case 8:
					me->mind=2;	// go into wander mode
					me->mind1=Random(40)+1;
					break;
				default:	// blink is very common
					me->seq=ANIM_A2;
					me->frm=0;
					me->frmTimer=0;
					me->frmAdvance=128;
					me->mind1=0;
					me->action=ACTION_BUSY;
					break;
			}
		}
		if(RangeToTarget(me,goodguy)<128*FIXAMT || me->ouch>0)
		{
			// if the goodguy is near, or he shot me
			me->mind=1;	// start hunting
			me->mind1=255;

			FaceGoodguy(me,goodguy);
		}
		me->dx=0;
		me->dy=0;
	}
	else if(me->mind==1)	// hunting mode
	{
		FaceGoodguy(me,goodguy);
		if(!goodguy)
		{
			me->mind=0;	// go back to being bored, nothing to see here
			me->mind1=0;
			return;
		}
		if(RangeToTarget(me,goodguy)<(512*FIXAMT) && Random(8)==0 && me->reload==0)
		{
			me->seq=ANIM_ATTACK;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
			me->action=ACTION_BUSY;
			me->dx=0;
			me->dy=0;
			me->reload=1;
			return;
		}

		if(me->ouch && me->mind1<240)
			me->mind1+=10;
		if(me->mind1)
			me->mind1--;
		else
		{
			if(RangeToTarget(me,goodguy)>256*FIXAMT)
				me->mind=0;	// get bored again
			else
				me->mind1=20;	// stay on trail a little longer
			return;
		}
		if(Random(80)==0)
		{
			me->mind1=20;
			me->mind=2;	// wander
		}
		me->dx=Cosine(me->facing*32)*5;
		me->dy=Sine(me->facing*32)*5;
		if(me->seq!=ANIM_MOVE)
		{
			me->seq=ANIM_MOVE;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=256;
		}
	}
	else	// random wander mode
	{
		if(!(me->mind1--))	// time to get a new direction
		{
			switch(Random(3))
			{
				case 0:
					if(goodguy)
						me->mind=1;	// hunt him
					break;
				case 1:
					me->mind=0;	// stop here and rest
					me->mind1=0;
					break;
				case 2:	// just wander a different way
					me->facing=(byte)Random(8);
					me->mind1=Random(20)+1;
					break;
			}
		}

		me->dx=Cosine(me->facing*32)*2;
		me->dy=Sine(me->facing*32)*2;
		if(me->seq!=ANIM_MOVE)
		{
			me->seq=ANIM_MOVE;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=96;
		}
	}
	// this is needed because his idle frame doesn't fit into his walking animation, so this skips it
	if(me->seq==ANIM_MOVE && me->frm==10 && me->frmTimer>127)
	{
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=96;
	}
}

void AI_GrueJr(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y,i;
	byte f;

	if(!goodguy)
		return;

	// heal in darkness
	if(map->map[me->mapx+me->mapy*map->width].templight<-3)
	{
		if(me->hp<me->maxHP && me->seq!=ANIM_DIE)
		{
			i=me->hp;
			me->hp+=2;
			if(me->hp>me->maxHP)
				me->hp=me->maxHP;
			ShowEnemyLife(me->name,i*128/me->maxHP,me->hp*128/me->maxHP,1);
		}
	}

	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_SPD2OUCH,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_SPD2DIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_DIE)
		{
			if(Random(2))
			{
				x = me->x >> FIXSHIFT;
				y = me->y >> FIXSHIFT;
				BlowSmoke((x + me->rectx + Random(me->rectx2 - me->rectx)) << FIXSHIFT,
					(y + me->recty + Random(me->recty2 - me->recty)) << FIXSHIFT,
					me->z, FIXAMT);
			}
		}
		if(me->seq==ANIM_A1 && me->frm==2 && me->reload==0 && goodguy)
		{
			FaceGoodguy(me,goodguy);
			x=me->x+Cosine(me->facing*32)*32;
			y=me->y+Sine(me->facing*32)*32;
			if(me->AttackCheck(32,x>>FIXSHIFT,y>>FIXSHIFT,goodguy))
			{
				goodguy->GetShot(Cosine(me->facing*32)*8,Sine(me->facing*32)*8,10,map,world);
				me->reload=2;
			}
		}
		return;	// can't do nothin' right now
	}

	if(me->mind1)
		me->mind1--;

	if(map->map[goodguy->mapx+goodguy->mapy*map->width].templight<-8)	// if you are in the dark, you are good eats
	{
		me->mind=2;	// dash at you
		me->mind1=3;
	}

	switch(me->mind)
	{
		case 0:	// normal heading towards you
			FaceGoodguy(me,goodguy);
			if(me->mind1==0)
			{
				me->mind=1;
				me->facing=Random(8);
				me->mind1=10+Random(40);
			}
			me->dx=Cosine(me->facing*32)*2;
			me->dy=Sine(me->facing*32)*2;
			me->frmAdvance=128;
			break;
		case 1:	// normal wandering
			if(me->mind1==0)
			{
				me->mind=0;
				me->mind1=10+Random(90);
			}
			me->dx=Cosine(me->facing*32)*2;
			me->dy=Sine(me->facing*32)*2;
			me->frmAdvance=128;
			break;
		case 2:	// dashing at you
			if(me->mind1==0)
			{
				me->mind=1;
				me->mind1=10+Random(40);
			}
			f=AngleFrom(me->x,me->y,goodguy->x,goodguy->y);
				ColorDrop(4,me->x,me->y,me->z+20);
			me->facing=f/32;
			me->dx=Cosine(f)*8;
			me->dy=Sine(f)*8;
			me->frmAdvance=200;
			break;
	}

	if(goodguy)
	{

		x=RangeToTarget(me,goodguy);
		if(x<(128*FIXAMT) && Random(16)==0 && me->reload==0)
		{
			MakeSound(SND_SPD3CHOMP,me->x,me->y,SND_CUTOFF,1200);
			FaceGoodguy(me,goodguy);
			// bite him
			me->seq=ANIM_A1;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=200;
			me->action=ACTION_BUSY;
			me->dx=0;
			me->dy=0;
			me->reload=1;
			return;
		}
	}

	if(me->mind!=2)	// when dashing, he ignores light, but otherwise wants to avoid it
	{
		x=(me->x+me->dx)/(TILE_WIDTH*FIXAMT);
		y=(me->y+me->dy)/(TILE_HEIGHT*FIXAMT);
		if(map->map[me->mapx+me->mapy*map->width].templight<-5 &&
			map->map[x+y*map->width].templight>=-5)	// you are in darkness now, but wouldn't be on moving
		{
			me->dx=0;
			me->dy=0;
			me->seq=ANIM_IDLE;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
			return;
		}
		if(map->map[me->mapx+me->mapy*map->width].templight>=-5)	// if you are in light at any time, run
		{
			me->dx=(me->dx*3)/2;
			me->dy=(me->dy*3)/2;
			me->frmAdvance=160;
		}
	}

	if(me->seq!=ANIM_MOVE)
	{
		me->seq=ANIM_MOVE;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=128;
	}
}

void AI_PygmyBoomer(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y,g;

	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_PYGMYOUCH,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_PYGMYDIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm==4 && me->reload==0 && goodguy)
		{
			x=me->x+Cosine(me->facing*32)*10;
			y=me->y+Sine(me->facing*32)*10;
			// throw spear
			FireBullet(x,y,me->facing*32,BLT_BOOMERANG,me->friendly);
			me->reload=5;
		}
		return;	// can't do nothin' right now
	}

	if(me->mind==0)	// not currently aware of goodguy
	{
		me->frmAdvance=32;	// overcome the auto-128 frmAdvance of guy.cpp

		if(me->seq==ANIM_MOVE)
		{
			me->seq=ANIM_IDLE;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=32;
		}
		if(me->mind1<101)
			me->mind1++;

		if(me->mind1>100 && Random(200)==0)
		{
			switch(Random(2))
			{
				case 0:
					MakeSound(SND_UNGADANCE,me->x,me->y,SND_CUTOFF,1200);
					me->seq=ANIM_A1;	// unga dance
					me->frm=0;
					me->frmTimer=0;
					me->frmAdvance=64;
					me->mind1=0;
					me->action=ACTION_BUSY;
					break;
				case 1:
					me->facing=(me->facing+(-1+Random(3)))&7;	//change facing
					me->mind1=0;
					break;
			}
		}
		if(goodguy)
		{
			if(RangeToTarget(me,goodguy)<320*FIXAMT || me->ouch>0)
			{
				// if the goodguy is near, or he shot me
				me->mind=1;	// start hunting
				if(me->ouch==0)
					me->mind1=60;	// for 2 seconds minimum
				else
					me->mind1=120;	// 4 seconds, because they HATE getting shot
				FaceGoodguy(me,goodguy);

				// do angry animation
				me->action=ACTION_BUSY;
				me->seq=ANIM_A2;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=255;	// very fast
				MakeSound(SND_PYGMYANGRY,me->x,me->y,SND_CUTOFF,1200);
			}
		}
		me->dx=0;
		me->dy=0;
	}
	else
	{
		FaceGoodguy(me,goodguy);
		if(!goodguy)
		{
			me->mind=0;	// go back to being bored, nothing to see here
			me->mind1=0;
			return;
		}
		if(RangeToTarget(me,goodguy)<(512*FIXAMT) && Random(64)==0 && me->reload==0)
		{
			// spear him!
			MakeSound(SND_PYGMYSTAB,me->x,me->y,SND_CUTOFF,1200);
			me->seq=ANIM_ATTACK;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=255;
			me->action=ACTION_BUSY;
			me->dx=0;
			me->dy=0;
			me->reload=1;
			return;
		}

		if(me->ouch && me->mind1<240)
			me->mind1+=10;
		if(me->mind1)
			me->mind1--;
		else
		{
			if(RangeToTarget(me,goodguy)>1024*FIXAMT)
				me->mind=0;	// get bored again
			else
				me->mind1=30;	// stay on trail a little longer
			return;
		}
		if(goodguy)
		{
			
			x=goodguy->x;
			y=goodguy->y;

			FaceGoodguy2(me,goodguy);
			if(me->x<x)
				me->dx+=FIXAMT*1;
			if(me->y<y)
				me->dy+=FIXAMT*1;
			if(me->x>x)
				me->dx-=FIXAMT*1;
			if(me->y>y)
				me->dy-=FIXAMT*1;

			Clamp(&me->dx,FIXAMT*5);
			Clamp(&me->dy,FIXAMT*5);
		}
		if(me->seq!=ANIM_MOVE)
		{
			me->seq=ANIM_MOVE;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
		}
	}
}

//LUNACHICK
void AI_Lunachick(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y;

	if(me->reload)
		me->reload--;

	if(me->mind3)
		me->mind3--;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_LUNAOUCH,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_LUNADIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm==3 && me->reload==0 && goodguy)
		{
			x=me->x+Cosine(me->facing*32)*16;
			y=me->y+Sine(me->facing*32)*16;
			FireExactBullet(x,y,FIXAMT*10,Cosine(me->facing*32+Random(16)-8)*6,Sine(me->facing*32+Random(16)-8)*6,0,0,30,me->facing,BLT_LUNAX2,me->friendly);
			me->reload=5;
			me->mind1=1;
		}
		if(me->seq==ANIM_A5 && me->frm > 7 && me->frm < 19 && me->frm % 2 == 0 && goodguy)
		{
			x=me->x+Cosine(me->facing*32)*16;
			y=me->y+Sine(me->facing*32)*16;
			FireExactBullet(x,y,FIXAMT*10,Cosine(me->facing*32+Random(16)-8)*(8+Random(4)),Sine(me->facing*32+Random(16)-8)*(8+Random(4)),0,0,40,me->facing,BLT_LUNAX1,me->friendly);
			me->reload=7;
			me->mind1=1;
		}
		if(me->seq==ANIM_A4) //ah, the leap
		{
			if (me->frm==2)
			{me->dx=Cosine(me->facing*32)*14;
			me->dy=Sine(me->facing*32)*14;
			me->dz=16*FIXAMT;}
			else if(me->frm<7)
			{
				// stay the course
				Dampen(&me->dx,FIXAMT/8);
				Dampen(&me->dy,FIXAMT/8);
			}
			else if(me->frm==18 && me->frmTimer<32)
			{
				FireBullet(me->x,me->y-FIXAMT,0,BLT_SHOCKWAVE,me->friendly);
				MakeSound(SND_TELEPORT,me->x,me->y,SND_CUTOFF,600);
				map->BrightTorch((me->x/TILE_WIDTH)>>FIXSHIFT,
							 (me->y/TILE_HEIGHT)>>FIXSHIFT,18,8);
				me->x+=-FIXAMT*320+Random(FIXAMT*640);
				me->y+=-FIXAMT*240+Random(FIXAMT*480);
				if(me->x<TILE_WIDTH*2*FIXAMT)
					me->x=TILE_WIDTH*2*FIXAMT;
				else if(me->x>(map->width-2)*TILE_WIDTH*FIXAMT)
					me->x=(map->width-2)*TILE_WIDTH*FIXAMT;
				if(me->y<TILE_HEIGHT*2*FIXAMT)
					me->y=TILE_HEIGHT*2*FIXAMT;
				else if(me->y>(map->height-2)*TILE_HEIGHT*FIXAMT)
					me->y=(map->height-2)*TILE_HEIGHT*FIXAMT;
				me->dz=8*FIXAMT;
			}
		}
		if(me->seq==ANIM_DIE)
			me->facing=(me->facing+1)&7;
		return;	// can't do nothin' right now
	}

	if(me->mind==0)		// when mind=0, singlemindedly lumber towards Bouapha
	{
		if(RangeToTarget(me,goodguy)<(512*FIXAMT) && Random(24)==0)
		{
			// spit at him
			MakeSound(SND_LUNASHOOT,me->x,me->y,SND_CUTOFF,1200);
			me->seq=ANIM_ATTACK;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=256;
			me->action=ACTION_BUSY;
			me->dx=0;
			me->dy=0;
			me->reload=0;
			FaceGoodguy(me,goodguy);
			return;
		}
		else if(RangeToTarget(me,goodguy)<(1024*FIXAMT) && RangeToTarget(me,goodguy)>(256*FIXAMT) && Random(36)==0)
		{
			// spit at him
			MakeSound(SND_LUNABORED,me->x,me->y,SND_CUTOFF,1200);
			me->seq=ANIM_A5;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=256;
			me->action=ACTION_BUSY;
			me->dx=0;
			me->dy=0;
			me->reload=0;
			FaceGoodguy(me,goodguy);
			return;
		}
		else if(Random(48)==0)
		{
			// spit at him
			MakeSound(SND_LUNABRAINS,me->x,me->y,SND_CUTOFF,1200);
			me->seq=ANIM_A4;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=256;
			me->action=ACTION_BUSY;
			me->dx=0;
			me->dy=0;
			me->reload=0;
			FaceGoodguy(me,goodguy);
			return;
		}
		FaceGoodguy(me,goodguy);
		
		me->dx=Cosine(me->facing*32)*5;
		me->dy=Sine(me->facing*32)*5;
		
		if(me->seq!=ANIM_MOVE)
		{
			me->seq=ANIM_MOVE;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
		}
		if(Random(128)==0)
		{
			me->mind=1;		// occasionally wander
			me->mind1=1;
		}
	}
	else if(me->mind==1)	// random wandering
	{
		if(RangeToTarget(me,goodguy)<(512*FIXAMT) && Random(24)==0)
		{
			// spit at him
			MakeSound(SND_LUNASHOOT,me->x,me->y,SND_CUTOFF,1200);
			me->seq=ANIM_ATTACK;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=256;
			me->action=ACTION_BUSY;
			me->dx=0;
			me->dy=0;
			me->reload=0;
			FaceGoodguy(me,goodguy);
			return;
		}
		else if(RangeToTarget(me,goodguy)<(1024*FIXAMT) && Random(36)==0)
		{
			// spit at him
			MakeSound(SND_LUNABORED,me->x,me->y,SND_CUTOFF,1200);
			me->seq=ANIM_A5;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=256;
			me->action=ACTION_BUSY;
			me->dx=0;
			me->dy=0;
			me->reload=0;
			FaceGoodguy(me,goodguy);
			return;
		}
		else if(Random(48)==0)
		{
			// spit at him
			MakeSound(SND_LUNABRAINS,me->x,me->y,SND_CUTOFF,1200);
			me->seq=ANIM_A4;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=256;
			me->action=ACTION_BUSY;
			me->dx=0;
			me->dy=0;
			me->reload=0;
			FaceGoodguy(me,goodguy);
			return;
		}

		if(!(me->mind1--))	// time to get a new direction
		{
			if((goodguy) && Random(3)==0)
				me->mind=0;	// get back on track
			else
				me->facing=(byte)Random(8);
			me->mind1=Random(40)+1;
		}

		me->dx=Cosine(me->facing*32)*5;
		me->dy=Sine(me->facing*32)*5;
		if(me->seq!=ANIM_MOVE)
		{
			me->seq=ANIM_MOVE;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
		}
	}
}

void AI_NightTerror(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y,i;
	byte f;

	if(!goodguy)
		return;
	
	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_VAMPOUCH,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_VAMPDIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->action==ACTION_BUSY)
	{

		if(me->seq==ANIM_DIE)
		{
			if(Random(2))
			{
				x = me->x >> FIXSHIFT;
				y = me->y >> FIXSHIFT;
				BlowSmoke((x + me->rectx + Random(me->rectx2 - me->rectx)) << FIXSHIFT,
					(y + me->recty + Random(me->recty2 - me->recty)) << FIXSHIFT,
					me->z, FIXAMT);
			}
		}
		if(me->seq==ANIM_A1 && me->frm==3 && me->reload==0 && goodguy)
		{
			FaceGoodguy(me,goodguy);
			x=me->x+Cosine(me->facing*32)*16;
			y=me->y+Sine(me->facing*32)*16;
			if(me->AttackCheck(20,x>>FIXSHIFT,y>>FIXSHIFT,goodguy))
			{
				goodguy->GetShot(Cosine(me->facing*32)*4,Sine(me->facing*32)*4,12,map,world);
			}
			me->reload=5;
		}
		return;	// can't do nothin' right now
	}

	if(me->mind1)
		me->mind1--;
	
	if (me->mind1 % 2 == 1 && me->mind !=2)
	{
		if(me->hp<me->maxHP && me->seq!=ANIM_DIE)
		{
			i=me->hp;
			me->hp+=1;
			if(me->hp>me->maxHP)
			me->hp=me->maxHP;
		}
	}

	if(map->map[me->mapx+me->mapy*map->width].templight>-8)	// if you can see him, RUN!
	{
		me->mind=2;	// dash at you
		me->mind1=3;
	}

	switch(me->mind)
	{
		case 0:	// normal heading towards you
			FaceGoodguy(me,goodguy);
			if(me->mind1==0)
			{
				me->mind=1;
				me->facing=Random(8);
				me->mind1=10+Random(40);
			}
			me->dx=Cosine(me->facing*32)*2;
			me->dy=Sine(me->facing*32)*2;
			me->frmAdvance=128;
			if(goodguy->x>me->x-FIXAMT*64 && goodguy->x<me->x+FIXAMT*48 && goodguy->y>me->y-FIXAMT*64 && goodguy->y<me->y+FIXAMT*48 && goodguy->ouch==0)
			goodguy->GetShot(0,0,1,map,world);
			break;
		case 1:	// normal wandering
			if(me->mind1==0)
			{
				me->mind=0;
				me->mind1=10+Random(90);
			}
			me->dx=Cosine(me->facing*32)*1;
			me->dy=Sine(me->facing*32)*1;
			me->frmAdvance=128;
			if(goodguy->x>me->x-FIXAMT*64 && goodguy->x<me->x+FIXAMT*48 && goodguy->y>me->y-FIXAMT*64 && goodguy->y<me->y+FIXAMT*48 && goodguy->ouch==0)
			goodguy->GetShot(0,0,2,map,world);
			break;
		case 2:	// dashing at you
			if(me->mind1==0)
			{
				me->mind=1;
				me->mind1=10+Random(40);
			}
			FaceGoodguy(me,goodguy);
			me->dx=Cosine(me->facing*32)*10;
			me->dy=Sine(me->facing*32)*10;
			me->frmAdvance=200;
			if(goodguy->x>me->x-FIXAMT*64 && goodguy->x<me->x+FIXAMT*48 && goodguy->y>me->y-FIXAMT*64 && goodguy->y<me->y+FIXAMT*48 && goodguy->ouch==0)
			goodguy->GetShot(0,0,1,map,world);
			break;
	}

	if(goodguy)
	{

		x=RangeToTarget(me,goodguy);
		if(x<(128*FIXAMT) && Random(16)==0 && me->reload==0 && me->mind == 2)
		{
			MakeSound(SND_VAMPATTK,me->x,me->y,SND_CUTOFF,1200);
			FaceGoodguy(me,goodguy);
			// bite him
			me->seq=ANIM_A1;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=200;
			me->action=ACTION_BUSY;
			me->dx=0;
			me->dy=0;
			me->reload=1;
			return;
		}
	}

	if(me->mind!=2)	// when dashing, he ignores light, but otherwise wants to avoid it
	{
		x=(me->x+me->dx)/(TILE_WIDTH*FIXAMT);
		y=(me->y+me->dy)/(TILE_HEIGHT*FIXAMT);
		if(map->map[me->mapx+me->mapy*map->width].templight<-5 &&
			map->map[x+y*map->width].templight>=-5)	// you are in darkness now, but wouldn't be on moving
		{
			me->dx=0;
			me->dy=0;
			me->seq=ANIM_IDLE;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
			return;
		}
		if(map->map[me->mapx+me->mapy*map->width].templight<=-5)	// if you are in light at any time, run
		{
			me->dx=(me->dx*3)/4;
			me->dy=(me->dy*3)/4;
			me->frmAdvance=160;
		}
	}

	if(me->seq!=ANIM_MOVE)
	{
		me->seq=ANIM_MOVE;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=128;
	}
}

void AI_CloneArmor(Guy *me, Map *map, world_t *world, Guy *goodguy)
{
	int x, y, x2, y2, f;

	if (me->reload)
		me->reload--;

	if (me->ouch == 4)
	{
		if (me->hp > 0)
			MakeSound(SND_ROBOBOUAPHAOUCH, me->x, me->y, SND_CUTOFF, 1200);
	}

	if (me->action == ACTION_BUSY)
	{
		if (me->seq == ANIM_ATTACK && me->frm > 3 && me->frm < 12 && me->frm % 2 == 1 && me->reload == 0 && goodguy)
		{
			MakeSound(SND_ARMORSHOOT, me->x, me->y, SND_CUTOFF, 1200);
			f = me->facing * 32 - 64;
			x = me->x + Cosine(me->facing * 32)*20;
			y = me->y + Sine(me->facing * 32)*20;
			x2 = x + Cosine(f)*32;
			y2 = y + Sine(f)*32;
			FireBullet(x2, y2, me->facing * 32, BLT_BIGSHELL, me->friendly);
			x2 = x - Cosine(f)*32;
			y2 = y - Sine(f)*32;
			FireBullet(x2, y2, me->facing * 32, BLT_BIGSHELL, me->friendly);
			me->mind1 = 1;
		}
		if (me->seq == ANIM_A1 && me->frm == 2 && me->reload == 0 && goodguy)
		{
			QuadMissile(me->x, me->y, me->facing*32, me->friendly);
			me->reload = 5;
			me->mind1 = 1;
		}
		if (me->frm >= 3 && me->seq == ANIM_DIE)
			{
				x = me->x >> FIXSHIFT;
				y = me->y >> FIXSHIFT;
				BlowUpGuy(x + me->rectx, y + me->recty, x + me->rectx2, y + me->recty2, me->z, 2);
				BlowSmoke((x + me->rectx + Random(me->rectx2 - me->rectx)) << FIXSHIFT,
						(y + me->recty + Random(me->recty2 - me->recty)) << FIXSHIFT,
						me->z, FIXAMT);
			}
		if (me->frm == 4 && me->seq == ANIM_DIE)
			{
				// become a zombie
				FireBullet(me->x, me->y, 0, BLT_BOOM, me->friendly);
				me->type=MONS_EVILCLONE;	// become a lesser clone
				me->aiType=MONS_EVILCLONE;
				strcpy(me->name,monsType[me->type].name);
				me->hp=32;
				me->maxHP=32;
				me->mind=0;
				me->mind1=0;
				me->facing=2;
			}
		return; // can't do nothin' right now
	}

	if (me->mind == 0) // when mind=0, singlemindedly lumber towards Bouapha
	{
		if (RangeToTarget(me, goodguy)<(512 * FIXAMT) && Random(32) == 0)
		{
			// spit at him
			me->seq = ANIM_ATTACK;
			me->frm = 0;
			me->frmTimer = 0;
			me->frmAdvance = 256;
			me->action = ACTION_BUSY;
			me->dx = 0;
			me->dy = 0;
			me->reload = 0;
			FaceGoodguy(me, goodguy);
			return;
		}
		else if (RangeToTarget(me, goodguy)<(512 * FIXAMT) && Random(128) == 0)
		{
			// spit at him
			me->seq = ANIM_A1;
			me->frm = 0;
			me->frmTimer = 0;
			me->frmAdvance = 256;
			me->action = ACTION_BUSY;
			me->dx = 0;
			me->dy = 0;
			me->reload = 0;
			FaceGoodguy(me, goodguy);
			return;
		}
		FaceGoodguy(me, goodguy);

		me->dx = Cosine(me->facing * 32)*4;
		me->dy = Sine(me->facing * 32)*4;
		if (me->seq != ANIM_MOVE)
		{
			me->seq = ANIM_MOVE;
			me->frm = 0;
			me->frmTimer = 0;
			me->frmAdvance = 128;
		}
		if (Random(64) == 0)
		{
			me->mind = 1; // occasionally wander
			me->mind1 = 1;
		}
	}
	else if (me->mind == 1) // random wandering
	{
		if (RangeToTarget(me, goodguy)<(512 * FIXAMT) && Random(32) == 0)
		{
			// spit at him
			MakeSound(SND_HAMMERTOSS, me->x, me->y, SND_CUTOFF, 1200);
			me->seq = ANIM_ATTACK;
			me->frm = 0;
			me->frmTimer = 0;
			me->frmAdvance = 256;
			me->action = ACTION_BUSY;
			me->dx = 0;
			me->dy = 0;
			me->reload = 0;
			FaceGoodguy(me, goodguy);
			return;
		}
		else if (RangeToTarget(me, goodguy)<(512 * FIXAMT) && Random(128) == 0)
		{
			// spit at him
			me->seq = ANIM_A1;
			me->frm = 0;
			me->frmTimer = 0;
			me->frmAdvance = 256;
			me->action = ACTION_BUSY;
			me->dx = 0;
			me->dy = 0;
			me->reload = 0;
			FaceGoodguy(me, goodguy);
			return;
		}

		if (!(me->mind1--)) // time to get a new direction
		{
			if ((goodguy) && Random(3) == 0)
				me->mind = 0; // get back on track
			else
				me->facing = (byte) Random(8);
			me->mind1 = Random(40) + 1;
		}

		me->dx = Cosine(me->facing * 32)*5;
		me->dy = Sine(me->facing * 32)*5;
		if (me->seq != ANIM_MOVE)
		{
			me->seq = ANIM_MOVE;
			me->frm = 0;
			me->frmTimer = 0;
			me->frmAdvance = 128;
		}
	}
}

void AI_Jumpkin(Guy *me, Map *map, world_t *world, Guy *goodguy)
{
	int i, x, y;

	if (me->reload)
		me->reload--;

	if (me->mind1)
		me->mind1--;

	if (me->ouch == 4)
	{
		if (me->hp > 0)
			MakeSound(SND_ZOMBIEOUCH, me->x, me->y, SND_CUTOFF, 1200);
		else
			MakeSound(SND_PUMPKINDIE, me->x, me->y, SND_CUTOFF, 1200);
	}

	if (me->action == ACTION_BUSY)
	{
		if (me->seq == ANIM_DIE && me->reload == 0)
		{
			ExplodeParticles(PART_HAMMER, me->x, me->y, me->z, 8);
			me->reload = 2;
		}
		if (me->seq != ANIM_DIE)
		{
			FindVictim(me->x >> FIXSHIFT, me->y >> FIXSHIFT, 24, Cosine(me->facing * 32)*4, Sine(me->facing * 32)*4, 2, map, world, me->friendly);
			
		}
		
		if (me->seq == ANIM_A1) // leaping attack
		{
			if (me->frm < 3)
			{
				me->dx = 0;
				me->dy = 0;
				me->dz = 0;
			}
			else if (me->frm == 3)
			{
				me->dx = Cosine(me->facing * 32)*12;
				me->dy = Sine(me->facing * 32)*12;
				me->dz = 16 * FIXAMT;
			}
			else if (me->frm < 12)
			{
				// stay the course
				Dampen(&me->dx, FIXAMT / 8);
				Dampen(&me->dy, FIXAMT / 8);
			}
			else if (me->frm == 12 && me->frmTimer < 32)
			{
				MakeSound(SND_ACIDSPLAT, me->x, me->y, SND_CUTOFF, 1500);
				for (i = 0; i < 8; i++)
				{
				x = me->x + Cosine(i*32)*16;
				y = me->y + Sine(i*32)*16;
				FireBullet(x, y, i*32, BLT_ENERGY, me->friendly);
				}
			}
			else
			{
				Dampen(&me->dx, FIXAMT / 2);
				Dampen(&me->dy, FIXAMT / 2);
			}
		}
		if (me->seq == ANIM_DIE)
		{
			//Poopy
		}

		return; // can't do nothin' right now
	}

	if (me->mind == 0)
	{
		if (goodguy)
		{
			if (RangeToTarget(me, goodguy) < 300 * FIXAMT)
				me->mind = 1;
		}
		if (me->ouch)
			me->mind = 1;
	}
	else if (me->mind == 2) // when mind=2, hold still and go off on Bouapha
	{
		if (goodguy)
		{
			i = RangeToTarget(me, goodguy);
			if (i < (128 * FIXAMT) && Random(10) == 0 && me->reload == 0)
			{
				// get him! (punch)
				me->seq = ANIM_A1;
				me->frm = 0;
				me->frmTimer = 0;
				me->frmAdvance = 128;
				me->action = ACTION_BUSY;
				me->dx = 0;
				me->dy = 0;
				return;
			}
			FaceGoodguy2(me, goodguy);

			me->dx = 0;
			me->dy = 0;
			if (me->seq != ANIM_IDLE)
			{
				me->seq = ANIM_IDLE;
				me->frm = 0;
				me->frmTimer = 0;
				me->frmAdvance = 128;
			}
			if (i > (80 * FIXAMT) && me->mind1 == 0)
			{
				// chase him down!
				me->mind = 1;
				me->mind1 = 8;
			}
		}
		else
		{
			// just sit there
		}
	}
	else if (me->mind == 1) // chase down Bouapha
	{
		if (goodguy)
		{
			FaceGoodguy2(me, goodguy);

			me->dx = Cosine(me->facing * 32)*4;
			me->dy = Sine(me->facing * 32)*4;
			if (me->seq != ANIM_MOVE)
			{
				me->seq = ANIM_MOVE;
				me->frm = 0;
				me->frmTimer = 0;
				me->frmAdvance = 128;
			}
			if (RangeToTarget(me, goodguy) < 128 * FIXAMT && me->mind1 == 0)
			{
				me->mind = 2; // in range, start killin'
				me->mind1 = 8;
			}
			else if (Random(64) == 0)
			{
				// leap!
				me->seq = ANIM_A1;
				me->frm = 0;
				me->frmTimer = 0;
				me->frmAdvance = 128;
				me->action = ACTION_BUSY;
				me->dx = 0;
				me->dy = 0;
				me->reload = 0;
			}
		}
		else
			me->mind = 0;
	}
}

void AI_Decoy(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int i;

	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp<=0)
			MakeSound(SND_PUMPKINDIE,me->x,me->y,SND_CUTOFF,1200);
		if(me->hp>0)
		{
			me->seq=ANIM_A2;	//sad
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=64;
		}
	}
	if(me->action==ACTION_BUSY)
	{
		FaceGoodguy(me,goodguy);
		if(me->seq==ANIM_ATTACK && me->frm==1 && me->reload==0 && goodguy)
		{
			if(RangeToTarget(me,goodguy)<60*FIXAMT)
				goodguy->GetShot(Cosine(me->facing*32)*16,Sine(me->facing*32)*16,10,map,world);
			me->reload=4;
		}
		if(me->seq==ANIM_ATTACK && me->frm==4)
		{
			me->seq=ANIM_IDLE;
			me->frm=0;
			me->action=ACTION_IDLE;
		}
		return;	// can't do nothin' right now
	}

	if(me->mind==0)	// waiting for Bouapha
	{
		if(goodguy)
		{
			if(RangeToTarget(me,goodguy)<300*FIXAMT || me->ouch)
			{
				me->action=ACTION_BUSY;
				me->seq=ANIM_MOVE;	// muscles
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=64;
				me->mind=1;
			}
		}
	}
	else if(me->mind==1)	// chase down Bouapha
	{
		if(me->z==0)
		{
			me->dz=12*FIXAMT;
			me->dx=me->dx*2.50;
			me->dy=me->dy*2.50;
		}
		if(goodguy)
		{
			i=RangeToTarget(me,goodguy);
			if(i<(60*FIXAMT) && Random(4)==0 && me->reload==0)
			{
				// get him! (chomp)
				MakeSound(SND_SKELKICK,me->x,me->y,SND_CUTOFF,1500);
				me->seq=ANIM_ATTACK;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=128;
				me->action=ACTION_BUSY;
				me->dx=Cosine(me->facing*32)*2;
				me->dy=Sine(me->facing*32)*2;
				return;
			}
			FaceGoodguy2(me,goodguy);

			if(me->seq!=ANIM_ATTACK)
			{
				me->dx=Cosine(me->facing*32)*8;
				me->dy=Sine(me->facing*32)*8;
			}
			else
			{
				me->dx=Cosine(me->facing*32)*2;
				me->dy=Sine(me->facing*32)*2;
			}
			if(me->seq!=ANIM_MOVE && me->seq!=ANIM_ATTACK)
			{
				me->seq=ANIM_MOVE;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=128;
			}
		}
		else
			me->mind=0;
	}
}

void AI_Hologram(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y,i;
	
	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_HOLOOUCH,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_HOLODIE,me->x,me->y,SND_CUTOFF,1200);
	}
	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->mind==2)
		{
			if(me->frm>5 && me->frm<14)
			{
				if(me->frm%2==1)
				{
				MakeSound(SND_BALLLIGHTNING,me->x,me->y,SND_CUTOFF,60);
				FireBullet(me->x,me->y,me->facing+7+me->frm%3,BLT_BALLLIGHTNING,me->friendly);
				}
				if (me->frm==10 && me->mind3==0)
				{
					me->mind3 = 1;
					for(i=0;i<8;i++)
					{
						FireExactBullet(me->x,me->y,FIXAMT*10,Cosine(i)*4,Sine(i)*4,0,0,128,i,BLT_BALLLIGHTNING,me->friendly);
					}
				}
				if(me->frm==13)
				me->mind=1;
			}
		}
		if (me->seq==ANIM_DIE)
		{if(me->frm>=4)
			{
				x=me->x>>FIXSHIFT;
				y=me->y>>FIXSHIFT;
				BlowSmoke((x+me->rectx+Random(me->rectx2-me->rectx))<<FIXSHIFT,
						  (y+me->recty+Random(me->recty2-me->recty))<<FIXSHIFT,
						  me->z,FIXAMT);
			}
		}
	}

	if(me->mind==0)	// if Bouapha gets near, laugh and then run
	{
		if(goodguy && RangeToTarget(me,goodguy)<160*FIXAMT)
		{
			MakeNormalSound(SND_HOLOLAUGH);
			me->seq=ANIM_ATTACK;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=96;
			me->action=ACTION_BUSY;
			me->mind=1;
			me->reload=72;
			return;
		}
	}
	if(me->mind==1)	// running away
	{
		if(me->reload)
			me->reload--;
		else
		{
			me->mind3=Random(3);
			if (me->mind3==1)
			{
				me->mind=2;
				me->mind3=0;
				me->action=ACTION_BUSY;
				me->seq=ANIM_ATTACK;
				MakeNormalSound(SND_HOLOSHOOT);
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=128;
				me->reload=0;
				FaceGoodguy2(me,goodguy);
				me->dx=0;
				me->dy=0;
			}
			else
			{
				me->facing=(me->facing+1-(byte)Random(3))&7;
				me->reload=Random(16)+1;
			}
		}

		if(me->mind1 && me->seq==ANIM_MOVE)
		{
			me->facing=(byte)Random(8);
			me->mind1=0;
			me->reload=16;
		}

		if(me->seq!=ANIM_MOVE&&me->seq!=ANIM_ATTACK)
		{
			me->seq=ANIM_MOVE;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=256;
		}
		
		if(me->seq==ANIM_MOVE)
		{
			me->dx=Cosine(me->facing*32)*10;
			me->dy=Sine(me->facing*32)*10;
		}
	}
}

void AI_Bat2(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y;

	if (me->reload)
		me->reload--;
	
	if(me->ouch==4)
	{
		if(me->hp>0)
		MakeSound(SND_BATOUCH,me->x,me->y,SND_CUTOFF,1200);
		else
		MakeSound(SND_BATDIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->aiType==MONS_GASBAT && Random(12)==0)
	{
		FireBullet(me->x,me->y,(byte)Random(256),BLT_FART,me->friendly);
	}
	else if(me->aiType==MONS_GASBAT && Random(3)==0)
	{
		FireBullet(me->x,me->y,(byte)Random(256),BLT_SPORE,me->friendly);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm==2 && me->reload==0 && goodguy)
		{
			x=me->x+Cosine(me->facing*32)*16;
			y=me->y+Sine(me->facing*32)*16;
			if(me->AttackCheck(16,x>>FIXSHIFT,y>>FIXSHIFT,goodguy))
			{
				if(goodguy)
				{
					goodguy->GetShot(Cosine(me->facing*32)*4,Sine(me->facing*32)*4,5,map,world);
					if(me->AttackCheck(12,x>>FIXSHIFT,y>>FIXSHIFT,goodguy))
					{
						if(goodguy->aiType==MONS_BOUAPHA && me->aiType==MONS_GRABBYBAT && me->mind3==0)
						{
							me->mind3=StealWeapon();
						}
						else if(me->aiType==MONS_VAMPBAT)
						{
							me->hp+=5;
							
						}
					}
				}
			}
			me->reload=2;

		}
		if(me->seq==ANIM_A1 && me->frm>1 && goodguy && me->hp>0)
		{
			// diving attack hit check
			x=me->x;
			y=me->y;
			if(me->AttackCheck(16,x>>FIXSHIFT,y>>FIXSHIFT,goodguy))
			{
				goodguy->GetShot(Cosine(me->facing*32)*8,Sine(me->facing*32)*8,5,map,world);
				me->seq=ANIM_A2;	// bounce off
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=64;
				me->dx=-me->dx/4;
				me->dy=-me->dy/4;
			}
		}
		if(me->seq==ANIM_A3 && me->frm==3 && me->reload==0)
		{
			FireBullet(me->x,me->y,me->facing*32,BLT_ENERGY_BOUNCE,me->friendly);
			me->reload=5;
		}
		if(me->seq==ANIM_DIE)
			me->frmAdvance=256;

		return;	// can't do nothin' right now
	}

	if(me->aiType!=MONS_GASBAT)
	{
		if(RangeToTarget(me,goodguy)<(48*FIXAMT) && Random(4)==0 &&
			map->CheckLOS(me->mapx,me->mapy,6,goodguy->mapx,goodguy->mapy))
		{
			// get him!
			MakeSound(SND_BATEYES,me->x,me->y,SND_CUTOFF,1200);
			me->seq=ANIM_ATTACK;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
			me->action=ACTION_BUSY;
			me->dx=0;
			me->dy=0;
			me->reload=0;
			FaceGoodguy(me,goodguy);
			return;
		}

		if(RangeToTarget(me,goodguy)>(75*FIXAMT) && Random(32)==0 && me->aiType==MONS_GARGOYLE &&
			map->CheckLOS(me->mapx,me->mapy,15,goodguy->mapx,goodguy->mapy))
		{
			FaceGoodguy(me,goodguy);
			MakeSound(SND_BATDIVE,me->x,me->y,SND_CUTOFF,1200);
			me->seq=ANIM_A1;	// diving attack move
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
			me->action=ACTION_BUSY;
			me->dx=Cosine(me->facing*32)*12;
			me->dy=Sine(me->facing*32)*12;
			return;
		}

		if(RangeToTarget(me,goodguy)>(200*FIXAMT) && Random(32)==0 && me->aiType==MONS_VAMPBAT &&
			map->CheckLOS(me->mapx,me->mapy,15,goodguy->mapx,goodguy->mapy))
		{
			MakeSound(SND_BATEYES,me->x,me->y,SND_CUTOFF,1200);
			me->seq=ANIM_A3;	// shoot
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
			me->action=ACTION_BUSY;
			me->dx=0;
			me->dy=0;
			FaceGoodguy(me,goodguy);
			return;
		}
	}
	if(me->mind3==0)
	WanderAI(me,64,40,3,map,world,goodguy,FIXAMT*10);
	else
	{
		// have stolen something, must run away!
		FaceGoodguy(me,goodguy);
		me->facing=(me->facing+4)&7;
		me->dx=Cosine(me->facing*32)*10;
		me->dy=Sine(me->facing*32)*10;
		if(me->seq!=ANIM_MOVE)
		{
			me->seq=ANIM_MOVE;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=256;
		}
		return;
	}
}

void AI_RollerKiller(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y,f;

	if(me->ouch==4)
	{
		//NO NOISE!
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_DIE)
		{
			x=me->x>>FIXSHIFT;
			y=me->y>>FIXSHIFT;
			BlowUpGuy(x+me->rectx,y+me->recty,x+me->rectx2,y+me->recty2,me->z,1);
			FireBullet(me->x+((32-Random(65))<<FIXSHIFT),me->y+((32-Random(65))<<FIXSHIFT),
				(byte)Random(8),BLT_ROCK,me->friendly);
			FireBullet(me->x+((32-Random(65))<<FIXSHIFT),me->y+((32-Random(65))<<FIXSHIFT),
				(byte)Random(8),BLT_ROCK,me->friendly);
			FireBullet(me->x+((32-Random(65))<<FIXSHIFT),me->y+((32-Random(65))<<FIXSHIFT),
				(byte)Random(8),BLT_ROCK,me->friendly);
		}
		return;	// can't do nothin' right now
	}

	if(me->mind==0)	// not activated yet
	{
		if(RangeToTarget(me,goodguy)<512*FIXAMT || me->ouch)
		{
			me->mind=1;
			me->mind2=3;
			MakeNormalSound(SND_CARSTART);
		}
	}
	else if(me->mind==1)	// active, zooming after Bouapha
	{
		if(me->mind2)
			me->mind2--;
		else
		{
			me->mind2=3;
			FaceGoodguy2(me,goodguy);
		}

		Dampen(&me->dx,FIXAMT/8);
		Dampen(&me->dy,FIXAMT/8);

		me->dx+=Cosine(me->facing*32)/2;
		me->dy+=Sine(me->facing*32)/2;

		if(RangeToTarget(me,goodguy)<64*FIXAMT)
		{
			x=me->x+Cosine(me->facing*32)*16;
			y=me->y+Sine(me->facing*32)*16;
			if(me->AttackCheck(35,x>>FIXSHIFT,y>>FIXSHIFT,goodguy))
			{
				goodguy->GetShot(Cosine(me->facing*32)*8,Sine(me->facing*32)*8,255,map,world);
				me->mind1=0;
				MakeSound(SND_ROLYPOLYWALL,me->x,me->y,SND_CUTOFF,1200);
			}
			else
			{
				me->dx=-me->dx;
				me->dy=-me->dy;	
			}
		}
		if(me->mind1)	// hit a wall
		{
			if(me->mind1&1)
				me->dx=-me->dx;
			if(me->mind1&2)
				me->dy=-me->dy;
			MakeSound(SND_ROLYPOLYWALL,me->x,me->y,SND_CUTOFF,1200);
			me->mind1=0;
		}
	}

	Clamp(&me->dx,FIXAMT*10);
	Clamp(&me->dy,FIXAMT*10);
	
	if(me->facing%2==0)
	{
		switch(me->facing)
		{
			case 0:
				f=ANIM_A1;	// roll to the right
				break;
			case 2:
				f=ANIM_MOVE; // roll down
				break;
			case 4:
				f=ANIM_A2;	// roll to the left
				break;
			case 6:
				f=ANIM_ATTACK;	// roll up
				break;
		}
		if(me->seq!=f)
		{
			me->seq=f;
			me->frmTimer=0;
			me->frmAdvance=128;
		}
	}
	
	if(me->mind!=0)
	{
		if(me->seq==ANIM_IDLE)
		{
			me->seq=ANIM_MOVE;
			me->frm=0;
			me->frmTimer=0;
		}
		if(me->seq==ANIM_MOVE)
			me->frmAdvance=(abs(me->dx)+abs(me->dy))*512/(FIXAMT*20);
	}
}

void AI_FlyingPatch(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y,i;
	Guy *g;

	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp==0)
			MakeSound(SND_PATCHDIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm==18)
		{
			me->seq=ANIM_A1;
			me->frm=0;
		}
		if(me->seq==ANIM_A1 && me->frm==7 && me->reload==0)
		{
			for (i = 0; i < 8; i++)
				{
				x = me->x + Cosine(i*32)*16;
				y = me->y + Sine(i*32)*16;
				FireBullet(x, y, i*32, BLT_FLAME5, me->friendly);
			}
			g=AddBaby(me->x,me->y+FIXAMT*2,0,MONS_PUMPKINFLY,me);
			if(g)
			{
				g->mind=1;
				g->mind1=30;
				g->facing=Random(8);
			}
			if(g && (!g->CanWalk(g->x,g->y,map,world)))
				RemoveGuy(g);
			me->reload=45;
		}
		return;
	}

	if((!me->reload) && Random(50)==0)
	{
		MakeSound(SND_EGGOPEN,me->x,me->y,SND_CUTOFF,1200);
		me->seq=ANIM_ATTACK;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=128;
		me->action=ACTION_BUSY;
	}
}

void AI_WinterGourd(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y,i;
	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp==0)
			MakeSound(SND_PUMPKINDIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_DIE)
		{
			for(i=0;i<8;i++)
			SpecialSnow(me->x+((-16+Random(33))<<FIXSHIFT), me->y+((-16+Random(33))<<FIXSHIFT));
			if(me->frm==2 && me->reload==0)
			{
				me->reload=10;
				for(i=0;i<8;i++)
				{
					x=me->x+Cosine(i*32)*8;
					y=me->y+Sine(i*32)*8;
					FireBullet(x,y,i*32,BLT_FREEZE2,me->friendly);
				}
			}
		}
		if(me->seq==ANIM_ATTACK && me->frm>=2 && me->frm<=4 && me->reload==0)
		{
			FireBullet(me->x+Cosine(me->facing*32)*16,me->y+Sine(me->facing*32)*16,me->facing*32,
				BLT_FREEZE2,me->friendly);
			me->reload=2;
		}
		return;	// can't do nothin' right now
	}

	if(me->mind3==0)
	{
		SpecialSnow(me->x+((-16+Random(33))<<FIXSHIFT), me->y+((-16+Random(33))<<FIXSHIFT));
		me->mind3=(byte)Random(10);
	}
	else
		me->mind3--;

	if(me->mind==0)	// not currently aware of goodguy
	{
		if(me->seq==ANIM_MOVE)
		{
			me->seq=ANIM_IDLE;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=32;
		}

		if(goodguy)
		{
			if(RangeToTarget(me,goodguy)<512*FIXAMT || me->ouch>0)
			{
				// if the goodguy is near, or he shot me
				me->mind=1;	// start hunting
				if(me->ouch==0)
					me->mind1=240;
				FaceGoodguy(me,goodguy);
			}
		}
		me->dx=0;
		me->dy=0;
	}
	else
	{
		FaceGoodguy(me,goodguy);
		if(!goodguy)
		{
			me->mind=0;	// go back to being bored, nothing to see here
			me->mind1=0;
			return;
		}

		if(me->mind1)
			me->mind1--;
		else
		{
			if(RangeToTarget(me,goodguy)>=512*FIXAMT)
				me->mind=0;	// get bored again
			else
				me->mind1=20;	// stay on trail a little longer
			return;
		}

		if(RangeToTarget(me,goodguy)<40*FIXAMT)
		{
			// shoot at him
			me->seq=ANIM_ATTACK;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=255;
			me->action=ACTION_BUSY;
			me->dx=0;
			me->dy=0;
			me->reload=0;
			return;
		}

		me->dx=Cosine(me->facing*32)*6;
		me->dy=Sine(me->facing*32)*6;
		if(me->seq!=ANIM_MOVE)
		{
			me->seq=ANIM_MOVE;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=255;
		}
	}
}

void AI_StickSnowman(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y,i;

	SpecialSnow(me->x+((-16+Random(33))<<FIXSHIFT), me->y+((-16+Random(33))<<FIXSHIFT));
	SpecialSnow(me->x+((-16+Random(33))<<FIXSHIFT), me->y+((-16+Random(33))<<FIXSHIFT));
	
	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_HAPPYOUCH,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_HAPPYDIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK)
		{
			i=(me->facing*32-8+Random(17))&255;
			if(i<0)
				i+=256;
			x=me->x+Cosine(i)*4;
			y=me->y+Sine(i)*4;
			FireExactBullet(x,y,FIXAMT*50,Cosine(i)*8,Sine(i)*8,-FIXAMT,0,50,i,BLT_SNOWBALL,me->friendly);
		}
		return;	// can't do nothin' right now
	}

	if(me->mind==0)		// when mind=0, charge towards Bouapha
	{
		if(goodguy)
		{
			if(RangeToTarget(me,goodguy)<(256*FIXAMT) && Random(16)==0)
			{
				// get him!
				MakeSound(SND_HAPPYCHOMP,me->x,me->y,SND_CUTOFF,1200);
				me->seq=ANIM_ATTACK;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=128;
				me->action=ACTION_BUSY;
				me->dx=0;
				me->dy=0;
				return;
			}
			FaceGoodguy2(me,goodguy);

			me->dx=Cosine(me->facing*32)*4;
			me->dy=Sine(me->facing*32)*4;
			if(me->seq!=ANIM_MOVE)
			{
				me->seq=ANIM_MOVE;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=128;
			}
			if(Random(10)==0)
			{
				me->mind=1;		// occasionally wander
				me->mind1=(byte)Random(100);
			}
		}
		else
		{
			me->mind=1;	// if there's no goodguy, sit around
			me->mind1=(byte)Random(100);
		}
	}
	else if(me->mind==1)	// random wandering
	{
		if(goodguy && RangeToTarget(me,goodguy)<(256*FIXAMT) && Random(16)==0)
		{
			// get him!
			FaceGoodguy2(me,goodguy);
			MakeSound(SND_HAPPYCHOMP,me->x,me->y,SND_CUTOFF,1200);
			me->seq=ANIM_ATTACK;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
			me->action=ACTION_BUSY;
			me->dx=0;
			me->dy=0;
			return;
		}

		if(!(me->mind1--))	// time to get a new direction
		{
			if((goodguy) && Random(40)==0)
				me->mind=0;	// get back on track
			else
				me->facing=(byte)Random(8);
			me->mind1=Random(40)+1;
		}

		me->dx=Cosine(me->facing*32)*4;
		me->dy=Sine(me->facing*32)*4;
		if(me->seq!=ANIM_MOVE)
		{
			me->seq=ANIM_MOVE;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
		}
	}
}

void AI_Gingersnap(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_GINGEROUCH,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_GINGERDIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->reload)
		me->reload--;
	
	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && (me->frm%3==0) && me->reload==0 && goodguy)
		{
			x=me->x+Cosine(me->facing*32)*32;
			y=me->y+Sine(me->facing*32)*32;
			if(me->AttackCheck(32,x>>FIXSHIFT,y>>FIXSHIFT,goodguy))
				goodguy->GetShot(Cosine(me->facing*32)*4,Sine(me->facing*32)*4,12,map,world);
			FireBullet(me->x,me->y,0,BLT_SHOCKWAVE,me->friendly);
			if(me->frm==11)
			{
				me->frmAdvance=64;
				me->dx=0;
				me->dy=0;
			}
			me->reload=4;
		}
		return;	// can't do nothin' right now
	}
	else
	{
		if(me->mind1>0)
			me->mind1--;
		else
		{
			me->mind1=6;
			me->mind2+=8;
		}
		
		if(me->z<FIXAMT)
		{
			me->dz=8*FIXAMT;
		}
	}
	
	if(goodguy)
	{
		if(RangeToTarget(me,goodguy)<(64*FIXAMT) && Random(4)==0)
		{
			// get him!
			MakeSound(SND_GINGERCOMBO,me->x,me->y,SND_CUTOFF,1200);
			me->seq=ANIM_ATTACK;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
			me->action=ACTION_BUSY;
			me->dx=0;
			me->dy=0;
			return;
		}
	}

	me->facing=floor(me->mind2/32);
	me->dx=Cosine(me->mind2)*3;
	me->dy=Sine(me->mind2)*3;
	if(me->seq!=ANIM_MOVE)
	{
		me->seq=ANIM_MOVE;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=64;
	}
}

void AI_StickFish(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y;
}

void AI_Yerfboss(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y,i;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_YERFOUCH,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_YERFLEAVE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frmTimer<32)
		{
			x = me->x + Cosine(me->facing*32)*20;
			y = me->y + Sine(me->facing*32)*20;
			if (me->frm%4==0)
			FireBullet(x,y,me->facing*32,BLT_MISSILE,me->friendly);
		}
		if(me->seq==ANIM_DIE && me->frm>=4)
		{
			me->frmAdvance=64;
		}
		return;	// can't do nothin' right now
	}
	
	if(me->mind==0)
	{
		if(goodguy && (RangeToTarget(me,goodguy)<400*FIXAMT || me->ouch>0))
		{
			me->mind=1;
		}
	}
	if(me->mind==1)	// active
	{
		if(goodguy)
		{
			// calculate desired location (want to be above Bouapha)
			x=goodguy->x;
			y=goodguy->y-96*FIXAMT;

			FaceGoodguy2(me,goodguy);
			if(me->x<x)
				me->dx+=FIXAMT/2;
			if(me->y<y)
				me->dy+=FIXAMT/2;
			if(me->x>x)
				me->dx-=FIXAMT/2;
			if(me->y>y)
				me->dy-=FIXAMT/2;

			Clamp(&me->dx,FIXAMT*6);
			Clamp(&me->dy,FIXAMT*6);
			
			if (Random(8))
			{
				FireExactBullet(me->x,me->y,me->z,Cosine(me->facing*32+256-8+Random(16))*8,Sine(me->facing*32+256-8+Random(16))*8,0,0,30,i*16,BLT_ENERGY,me->friendly);
			}
			
			if(FindVictim(me->x>>FIXSHIFT,me->y>>FIXSHIFT,36,0,0,1,map,world,me->friendly))
			{
				WeakenVictim(GetLastGuyHit(),255);
			}
		}
	}

	me->dz+=FIXAMT*3/2;
	if(me->z==0)
	{
		if(goodguy && RangeToTarget(me,goodguy)<512*FIXAMT && me->mind1==1)
		{
			FaceGoodguy(me,goodguy);
			me->seq=ANIM_ATTACK;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=64;
			me->action=ACTION_BUSY;
			me->dx=0;
			me->dy=0;
			me->dz=8*FIXAMT;
			return;
		}
		else
		{
		me->mind1 = Random(8);
		me->dz=FIXAMT*3+Random(FIXAMT*7);
			for(i=0;i<16;i++)
			{
				FireExactBullet(me->x,me->y,me->z,Cosine(i*16)*8,Sine(i*16)*8,0,0,30,i*16,BLT_ENERGY,me->friendly);
			}
		}
	}
	
	if(me->seq!=ANIM_MOVE)
	{
		me->seq=ANIM_MOVE;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=128;
	}
}

void AI_MechaCountess(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	Guy *g;
	int x,y,i;
	int xpos[]={-FIXAMT*50,0,FIXAMT*50};
	int ypos[]={0,FIXAMT*50,0};
	byte f;

	if(me->mindControl)
		f=2;
	else
		f=me->friendly;

	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_COUNTOUCH,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_COUNTDIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm>5 && me->frm<9 && me->reload==0)
		{
			me->reload=2;
			g=AddBaby(me->x,me->y+FIXAMT*40,FIXAMT*20,MONS_BAT,me);
			if(g && (!g->CanWalk(g->x,g->y,map,world)))
				RemoveGuy(g);
			else if(g)
			{
				MakeSound(SND_BATDIVE,g->x,g->y,SND_CUTOFF,1200);
				g->seq=ANIM_A1;	// diving attack move
				g->frm=0;
				g->frmTimer=0;
				g->frmAdvance=128;
				g->action=ACTION_BUSY;
				g->dx=Cosine(g->facing*32)*12-FIXAMT*2+Random(FIXAMT*4);
				g->dy=Sine(g->facing*32)*12;
			}
		}
		if(me->seq==ANIM_A1 && me->frm==2 && me->reload==0)
		{
			me->reload=5;
			for(i=0;i<3;i++)
			{
				g=AddBaby(me->x+xpos[i],me->y+ypos[i],0,MONS_BONEHEAD,me);
				if(g && (!g->CanWalk(g->x,g->y,map,world)))
					RemoveGuy(g);
				else if(g)
				{
					g->seq=ANIM_A3;	// arising from the ground
					g->frm=0;
					g->frmTimer=0;
					g->frmAdvance=128;
					g->action=ACTION_BUSY;
				}
			}
		}

		if(me->seq==ANIM_DIE && me->frm>3)
		{
			me->frmAdvance=128;
			x=me->x>>FIXSHIFT;
			y=me->y>>FIXSHIFT;
			BlowSmoke((x+me->rectx+Random(me->rectx2-me->rectx))<<FIXSHIFT,
					  (y+me->recty+Random(me->recty2-me->recty))<<FIXSHIFT,
					  me->z-Random(FIXAMT*60),FIXAMT);
		}
		if(me->seq==ANIM_A2 && me->frm==2)
		{
			me->dx=Cosine(me->facing)*14;
			me->dy=Sine(me->facing)*14;
		}

		return;	// can't do nothin' right now
	}

	if(!goodguy)
		return;

	if(me->mind==0)	// waiting for Bouapha
	{
		if(RangeToTarget(me,goodguy)<300*FIXAMT || me->ouch>0)
		{
			MakeSound(SND_COUNTHELLO,me->x,me->y,SND_CUTOFF,1200);
			me->mind=1;
		}
		me->mind1=60;
	}
	if(me->mind==1)	// wandering mode
	{
		// calculate desired location (want to be above Bouapha)
		x=goodguy->x;
		y=goodguy->y-96*FIXAMT;

		FaceGoodguy3(me,goodguy);
		if(me->x<x)
			me->dx+=FIXAMT/8;
		if(me->y<y)
			me->dy+=FIXAMT/8;
		if(me->x>x)
			me->dx-=FIXAMT/8;
		if(me->y>y)
			me->dy-=FIXAMT/8;

		Clamp(&me->dx,FIXAMT*3);
		Clamp(&me->dy,FIXAMT*4);

		if(RangeToTarget(me,goodguy)>400*FIXAMT && Random(10)==1)
		{
			MakeSound(SND_COUNTCHARGE,me->x,me->y,SND_CUTOFF,1200);
			me->mind=2;
			me->mind1=10+Random(10)+1;
			FaceGoodguy(me,goodguy);
			me->facing=((me->facing*32)-16+Random(33));
			me->action=ACTION_BUSY;
			me->seq=ANIM_A2;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=255;
			me->dx=0;
			me->dy=0;
		}
		if(me->mind1)
			me->mind1--;
		else
		{
			x=Random(4);
			switch(x)
			{
				case 0:	// shoot bats
					if(me->y>goodguy->y)
						me->mind1=10;	// don't fire bats if below player
					else
					{
						MakeSound(SND_COUNTSHOOT,me->x,me->y,SND_CUTOFF,1200);
						me->action=ACTION_BUSY;
						me->seq=ANIM_ATTACK;
						me->frm=0;
						me->frmTimer=0;
						me->frmAdvance=128;
						me->mind1=60;
						me->dx=0;
						me->dy=0;
					}
					break;
				case 1:	// summon boneheads
					if(RangeToTarget(me,goodguy)>160*FIXAMT || me->y>goodguy->y)
					{
						MakeSound(SND_COUNTSUMMON,me->x,me->y,SND_CUTOFF,1200);
						me->action=ACTION_BUSY;
						me->seq=ANIM_A1;
						me->frm=0;
						me->frmTimer=0;
						me->frmAdvance=128;
						me->mind1=60;
						me->dx=0;
						me->dy=0;
					}
					else
						me->mind1=10;	// don't summon if close, unless under goodguy
					break;
				case 2: // hyper-dash
					MakeSound(SND_COUNTCHARGE,me->x,me->y,SND_CUTOFF,1200);
					me->mind=2;
					me->mind1=10+Random(10);
					FaceGoodguy(me,goodguy);
					me->facing=((me->facing*32)-16+Random(33));
					me->action=ACTION_BUSY;
					me->seq=ANIM_A2;
					me->frm=0;
					me->frmTimer=0;
					me->frmAdvance=255;
					me->dx=0;
					me->dy=0;
					break;
				case 3:
					me->mind1=10;
					break;
			}
		}
	}
	else if(me->mind==2)
	{
		if(RangeToTarget(me,goodguy)<72*FIXAMT)
		{
			goodguy->GetShot(0,0,4,map,world);
		}
		CountessGlow(me->x,me->y);

		if(me->mind1)
			me->mind1--;
		else
		{
			me->mind=1;
			me->mind1=10;
		}
	}
}

void AI_ChefLiem(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y,i;
}

void AI_Crabshell(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y,i;
}

void AI_Noodlenode(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y,i;
}

void AI_Galvigourd(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y,i;
}

void AI_CountEshkah(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y,i;
}

void AI_Shapeshifter(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y,i,j,t;
	Guy *g,*g2;
	int bossTab[]={MONS_IMITATER,MONS_MATHEAD,MONS_THING,MONS_SVEN,MONS_BJORN,MONS_SPHINX,MONS_SDZL,MONS_COUNTESS,MONS_KONGOR,MONS_PATTY,MONS_DOZER,MONS_LOOKEYLOO,MONS_BOBBY,MONS_GOAT1,MONS_GOAT2,MONS_GOAT3,MONS_SPATULA,MONS_SKULL,MONS_RADISH,MONS_SPEEDY,MONS_GRUE,MONS_LARRY,MONS_FRANK,MONS_BIGGHOST,MONS_ELDER,MONS_ELDER2,MONS_ELDER3,MONS_ELDER4,MONS_BONKULA,MONS_YERFBOSS};//~30

	//Boss list: Imitater, Matilda, The Thing, Yeti Brothers, Sphinxter, SDZL, Countess, Kongor, Loonybot 5000,
	//			Crabby Patty, Dozer the Dozerian, The Lookey-Loo, Bobby Khan, Optimum Octon, Three Gruffs,
	//			Madame Spatula, Headless Horseradish, Speedy Hermandez, Iron Skull, The Grue, Spookley?,
	//			Frankenjulie, Larry, Polterguy, The Elders, Bonkula, Happy Stick Witch, Yerfdog
	
	if(me->action==ACTION_BUSY)
	{
		if(me->type==MONS_IMITATER && me->seq==ANIM_ATTACK && me->frm>2 && me->reload==0 && goodguy)
		{
			x=me->x+Cosine(me->facing*32)*16;
			y=me->y+Sine(me->facing*32)*16;
			FireBullet(x,y,me->facing-8+256+Random(16),BLT_EVILHAMMER,me->friendly);
			me->reload=2;
			return;
		}
		if(me->seq==ANIM_DIE)
		{
			if(me->type!=MONS_IMITATER)
			me->type=MONS_IMITATER; //turn back into your original form!
			else
			{
				x=me->x>>FIXSHIFT;
				y=me->y>>FIXSHIFT;
				BlowSmoke((x+me->rectx+Random(me->rectx2-me->rectx))<<FIXSHIFT,
						  (y+me->recty+Random(me->recty2-me->recty))<<FIXSHIFT,
						  me->z-Random(FIXAMT*60),FIXAMT);
			}
		return;
		}
	}
	
	if(Random(256)==1)
	{
		// teleport
		MakeSound(SND_GHOSTTPORT,me->x,me->y,SND_CUTOFF,1200);
		me->x+=-FIXAMT*320+Random(FIXAMT*640);
		me->y+=-FIXAMT*240+Random(FIXAMT*480);
		if(me->x<TILE_WIDTH*2*FIXAMT)
			me->x=TILE_WIDTH*2*FIXAMT;
		else if(me->x>(map->width-2)*TILE_WIDTH*FIXAMT)
			me->x=(map->width-2)*TILE_WIDTH*FIXAMT;
		if(me->y<TILE_HEIGHT*2*FIXAMT)
			me->y=TILE_HEIGHT*2*FIXAMT;
		else if(me->y>(map->height-2)*TILE_HEIGHT*FIXAMT)
			me->y=(map->height-2)*TILE_HEIGHT*FIXAMT;
		map->BrightTorch((me->x/TILE_WIDTH)>>FIXSHIFT,(me->y/TILE_HEIGHT)>>FIXSHIFT,18,8);
		MakeSound(SND_TELEPORT,me->x,me->y,SND_CUTOFF,1200);
	}
	
	if (me->reload)
	me->reload--;

	if (me->mind5)
	me->mind5--;
	else if (me->mind5==0)
	{
	x = me->x;
	y = me->y;
	me->dx = 0*FIXAMT;
	me->dy = 0*FIXAMT;
		//the effects!
		RemoveKids(me);
		TeamChangeRing(0,me->x-24*FIXAMT,me->y-24*FIXAMT,0,16,4);
		TeamChangeRing(0,me->x-24*FIXAMT,me->y+24*FIXAMT,0,16,4);
		TeamChangeRing(0,me->x+24*FIXAMT,me->y-24*FIXAMT,0,16,4);
		TeamChangeRing(0,me->x+24*FIXAMT,me->y+24*FIXAMT,0,16,4);
		TeamChangeRing(0,me->x,me->y,0,16,4);
		for(i=0;i<30;i++)
			BlowSmoke(me->x-32*FIXAMT+Random(64*FIXAMT),me->y-32*FIXAMT+Random(64*FIXAMT),Random(64)<<FIXSHIFT,Random(FIXAMT*4));
		map->BrightTorch((me->x/TILE_WIDTH)>>FIXSHIFT,(me->y/TILE_HEIGHT)>>FIXSHIFT,6,12);
		//the actual changing
		me->type=bossTab[Random(30)];
		//sounds
		if(me->friendly)
		MakeSound(SND_TURNGOOD,me->x,me->y,SND_CUTOFF,10);
		else
		MakeSound(SND_TURNEVIL,me->x,me->y,SND_CUTOFF,10);
	
	if(me->type==MONS_MATHEAD)	// Matilda, need to add all the parts
	{
		me->z=32*FIXAMT;
		g=AddGuy(x,y-32*FIXAMT,0,MONS_MATBODY,me->friendly);
		if(g)
			{g->parent=me;
			g->special=1;}
		g=AddGuy(x-40*FIXAMT,y-1*FIXAMT,32*FIXAMT,MONS_MATCLAW1,me->friendly);
		if(g)
			{g->parent=me;
			g->special=1;}
		g=AddGuy(x+40*FIXAMT,y-1*FIXAMT,32*FIXAMT,MONS_MATCLAW2,me->friendly);
		if(g)
			{g->parent=me;
			g->special=1;}
		g=AddGuy(x,y-96*FIXAMT,32*FIXAMT,MONS_MATTAIL,me->friendly);
		if(g)
			{g->parent=me;
			g->special=1;}
	}
	if(me->type==MONS_THING)	// The Thing needs to add tentacles
		{
			// lower left tentacle
			g=AddGuy(x-64*FIXAMT,y+48*FIXAMT,12*FIXAMT,MONS_THINGTENT,me->friendly);
			g->parent=me;
			g->special=1;
			g->facing=6;
			g->mind=0;	// mind tells them which tentacle they are (to constrain rotation)
			g->mind1=128;
			for(j=0;j<3;j++)
			{
				g2=g;
				// adding them all overlapping, they'll move into place when updated
				g=AddGuy(x-64*FIXAMT,y+48*FIXAMT,12*FIXAMT,MONS_THINGTENT,me->friendly);
				g->parent=g2;
				g->facing=6;
				g->mind=0;
				g->special=1;
			}
			g2=AddGuy(x-64*FIXAMT,y+48*FIXAMT,12*FIXAMT,MONS_THINGTENTTIP,me->friendly);
			g2->parent=g;
			g2->special=1;
			g2->facing=6;
			g2->mind=0;
			// lower right tentacle
			g=AddGuy(x+64*FIXAMT,y+48*FIXAMT,12*FIXAMT,MONS_THINGTENT,me->friendly);
			g->parent=me;
			g->special=1;
			g->facing=2;
			g->mind=1;
			g->mind1=128;
			for(j=0;j<3;j++)
			{
				g2=g;
				// adding them all overlapping, they'll move into place when updated
				g=AddGuy(x+64*FIXAMT,y+48*FIXAMT,12*FIXAMT,MONS_THINGTENT,me->friendly);
				g->parent=g2;
				g->special=1;
				g->facing=2;
				g->mind=1;
			}
			g2=AddGuy(x+64*FIXAMT,y+48*FIXAMT,12*FIXAMT,MONS_THINGTENTTIP,me->friendly);
			g2->parent=g;
			g2->special=1;
			g2->facing=2;
			g2->mind=1;
			// upper right tentacle
			g=AddGuy(x+64*FIXAMT,y-20*FIXAMT,12*FIXAMT,MONS_THINGTENT,me->friendly);
			g->parent=me;
			g->special=1;
			g->facing=14;
			g->mind=2;
			g->mind1=128;
			for(j=0;j<3;j++)
			{
				g2=g;
				// adding them all overlapping, they'll move into place when updated
				g=AddGuy(x+64*FIXAMT,y-20*FIXAMT,12*FIXAMT,MONS_THINGTENT,me->friendly);
				g->parent=g2;
				g->special=1;
				g->facing=14;
				g->mind=2;
			}
			g2=AddGuy(x+64*FIXAMT,y-20*FIXAMT,12*FIXAMT,MONS_THINGTENTTIP,me->friendly);
			g2->parent=g;
			g2->special=1;
			g2->facing=14;
			g2->mind=2;
			// upper left (and last!) tentacle
			g=AddGuy(x-64*FIXAMT,y-20*FIXAMT,12*FIXAMT,MONS_THINGTENT,me->friendly);
			g->parent=me;
			g->special=1;
			g->facing=10;
			g->mind=3;
			g->mind1=128;
			for(j=0;j<3;j++)
			{
				g2=g;
				// adding them all overlapping, they'll move into place when updated
					g=AddGuy(x-64*FIXAMT,y-20*FIXAMT,12*FIXAMT,MONS_THINGTENT,me->friendly);
				g->parent=g2;
				g->special=1;
				g->facing=10;
				g->mind=3;
			}
			g2=AddGuy(x-64*FIXAMT,y-20*FIXAMT,12*FIXAMT,MONS_THINGTENTTIP,me->friendly);
			g2->parent=g;
			g2->special=1;
			g2->facing=10;
			g2->mind=3;
		}
		if(me->type==MONS_SPHINX) //sphinxter gets its arms
		{
			// left arm
			g=AddGuy(x-108*FIXAMT,y+22*FIXAMT,0,MONS_SPHXARM1,me->friendly);
			g->parent=me;
			g->special=1;
			// right arm
			g=AddGuy(x+108*FIXAMT,y+22*FIXAMT,0,MONS_SPHXARM2,me->friendly);
			g->parent=me;
			g->special=1;
		}
		if(me->type==MONS_SVEN||me->type==MONS_BJORN||me->type==MONS_COUNTESS||me->type==MONS_KONGOR||me->type==MONS_SKULL||me->type==MONS_FRANK||me->type==MONS_BIGGHOST)
		{
			me->mind=1;
		}
		me->mind5=30*(Random(5)+5);
		me->seq=ANIM_MOVE;
	}
	
	//imitater
	if (me->type==MONS_IMITATER)
	{
		if(me->mind==0)		// when mind=0, singlemindedly lumber towards Bouapha
		{
			if(RangeToTarget(me,goodguy)<(512*FIXAMT) && Random(32)==0)
			{
				// spit at him
				MakeSound(SND_HAPPYCHOMP,me->x,me->y,SND_CUTOFF,1200);
				me->seq=ANIM_ATTACK;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=128;
				me->action=ACTION_BUSY;
				me->dx=0;
				me->dy=0;
				me->reload=0;
				FaceGoodguy(me,goodguy);
				return;
			}
			FaceGoodguy(me,goodguy);

			me->dx=Cosine(me->facing*32)*2;
			me->dy=Sine(me->facing*32)*2;
			if(me->seq!=ANIM_MOVE)
			{
				me->seq=ANIM_MOVE;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=128;
			}
			if(Random(64)==0)
			{
				me->mind=1;		// occasionally wander
				me->mind1=1;
			}
		}
		else if(me->mind==1)	// random wandering
		{
			if(RangeToTarget(me,goodguy)<(512*FIXAMT) && Random(32)==0)
			{
				// spit at him
				MakeSound(SND_HAPPYCHOMP,me->x,me->y,SND_CUTOFF,1200);
				me->seq=ANIM_ATTACK;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=128;
				me->action=ACTION_BUSY;
				me->dx=0;
				me->dy=0;
				me->reload=0;
				FaceGoodguy(me,goodguy);
				return;
			}

			if(!(me->mind1--))	// time to get a new direction
			{
				if((goodguy) && Random(3)==0)
					me->mind=0;	// get back on track
				else
					me->facing=(byte)Random(8);
				me->mind1=Random(40)+1;
			}

			me->dx=Cosine(me->facing*32)*2;
			me->dy=Sine(me->facing*32)*2;
			if(me->seq!=ANIM_MOVE)
			{
				me->seq=ANIM_MOVE;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=128;
			}
		}
	}
	//matilda
	if(me->type==MONS_MATHEAD)
	{
		AI_MattieSkullOrHead(me,map,world,goodguy);
	}
	//the thing
	if(me->type==MONS_THING)
	{
		AI_TheThing(me,map,world,goodguy);
	}
	//sven & bjorn
	if(me->type==MONS_SVEN||me->type==MONS_BJORN)
	{
		AI_Yeti(me,map,world,goodguy);
	}
	//sphinxter
	if(me->type==MONS_SPHINX)
	{
		AI_Sphinx(me,map,world,goodguy);
	}
	//sdzl
	if(me->type==MONS_SDZL)
	{
		AI_SDZL(me,map,world,goodguy);
	}
	//countess
	if(me->type==MONS_COUNTESS)
	{
		AI_Countess(me,map,world,goodguy);
	}
	//kongor
	if(me->type==MONS_KONGOR)
	{
		AI_Kongor(me,map,world,goodguy);
	}
	//patty
	if(me->type==MONS_PATTY)
	{
		AI_Patty(me,map,world,goodguy);
	}
	//dozer
	if(me->type==MONS_DOZER)
	{
		AI_Dozer(me,map,world,goodguy);
	}
	//lookey loo
	if(me->type==MONS_LOOKEYLOO)
	{
		AI_Lookeyloo(me,map,world,goodguy);
	}
	//booby khan
	if(me->type==MONS_BOBBY)
	{
		AI_BobbyKhan(me,map,world,goodguy);
	}
	//rammy
	if(me->type==MONS_GOAT1)
	{
		AI_Rammy(me,map,world,goodguy);
	}
	//hoppy
	if(me->type==MONS_GOAT2)
	{
		AI_Hoppy(me,map,world,goodguy);
	}
	//gruffy
	if(me->type==MONS_GOAT3)
	{
		AI_Gruffy(me,map,world,goodguy);
	}
	//madam spatula
	if(me->type==MONS_SPATULA)
	{
		AI_Spatula(me,map,world,goodguy);
	}
	//iron skull
	if(me->type==MONS_SKULL)
	{
		AI_IronSkull(me,map,world,goodguy);
	}
	//headless horseradish
	if(me->type==MONS_RADISH)
	{
		AI_Horseradish(me,map,world,goodguy);
	}
	//speedy hermandez
	if(me->type==MONS_SPEEDY)
	{
		AI_Speedy(me,map,world,goodguy);
	}
	//teh grue!
	if(me->type==MONS_GRUE)
	{
		AI_Grue(me,map,world,goodguy);
	}
	//larry
	if(me->type==MONS_LARRY)
	{
		AI_Larry(me,map,world,goodguy);
	}
	//frankenjulie
	if(me->type==MONS_FRANK)
	{
		AI_Frankenjulie(me,map,world,goodguy);
	}
	//polterguy
	if(me->type==MONS_BIGGHOST)
	{
		AI_Polterguy(me,map,world,goodguy);
	}
	//elder1
	if(me->type==MONS_ELDER)
	{
		AI_WindElder(me,map,world,goodguy);
	}
	//elder2
	if(me->type==MONS_ELDER2)
	{
		AI_EarthElder(me,map,world,goodguy);
	}
	//elder3
	if(me->type==MONS_ELDER3)
	{
		AI_WaterElder(me,map,world,goodguy);
	}
	//elder4
	if(me->type==MONS_ELDER4)
	{
		AI_FireElder(me,map,world,goodguy);
	}
	//bonkula
	if(me->type==MONS_BONKULA)
	{
		AI_Bonkula(me,map,world,goodguy);
	}
	//yerfdog
	if(me->type==MONS_YERFBOSS)
	{
		AI_Yerfboss(me,map,world,goodguy);
	}
}

void AI_Hotfoot(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int i;

	if(me->reload)
		me->reload--;

	map->DimTorch(me->mapx,me->mapy,2);
	if(me->ouch==4)
	{
		if(me->hp==0)
			MakeSound(SND_PUMPKINDIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_DIE && me->reload==0)
		{
			for(i=0;i<4;i++)
				FireBullet(me->x,me->y,0,BLT_BADSITFLAME,me->friendly);
			me->reload=100;
		}
		return;	// can't do nothin' right now
	}
	
	if(Random(4)==0)
		FireBullet(me->x,me->y,Random(256),BLT_FLAME3,me->friendly);

	FaceGoodguy(me,goodguy);
	if(!goodguy)
	{
		return;
	}

	me->dx=Cosine(me->facing*32)*6;
	me->dy=Sine(me->facing*32)*6;
	
	if(me->seq!=ANIM_MOVE)
	{
		me->seq=ANIM_MOVE;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=128;
	}
}

void AI_Yeti3(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y,i;

	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_YETIOUCH,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_YETIDIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_DIE)
		{
			me->facing=2;	// can only die forward
			if(me->frm>=4)
			{
				x=me->x>>FIXSHIFT;
				y=me->y>>FIXSHIFT;
				BlowUpGuy(x+me->rectx,y+me->recty,x+me->rectx2,y+me->recty2,me->z,2);
				BlowSmoke((x+me->rectx+Random(me->rectx2-me->rectx))<<FIXSHIFT,
						  (y+me->recty+Random(me->recty2-me->recty))<<FIXSHIFT,
						  me->z,FIXAMT);
			}
		}
		if(me->seq==ANIM_ATTACK && me->frm<8 && me->frmTimer<32)
		{
			me->facing=(me->facing+1)&7;
			if(me->frm%2==0)
			{
				x=me->x+Cosine(me->facing*32)*64;
				y=me->y+Sine(me->facing*32)*64;
				x+=Cosine(((me->facing*32)*32)&255)*32;
				y+=Sine(((me->facing*32)*32)&255)*32;
				FireExactBullet(x,y,40*FIXAMT,Cosine(me->facing*32)*8,Sine(me->facing*32)*8,4*FIXAMT,0,100,
								me->facing*32,BLT_BIGSNOW,me->friendly);
			}
		}
		if(me->seq==ANIM_ATTACK && me->frm>7 && me->frm<15 && me->frmTimer<32)
		{
			me->mind4=me->facing*32+256-8+Random(16);
			x=me->x+Cosine(me->mind4)*64;
			y=me->y+Sine(me->mind4)*64;
			x+=Cosine(((me->mind4)*32)&255)*32;
			y+=Sine(((me->mind4)*32)&255)*32;
			FireExactBullet(x,y,40*FIXAMT,Cosine(me->mind4)*8,Sine(me->mind4)*8,4*FIXAMT,0,100,
							me->mind4,BLT_SNOWBALL,me->friendly);
			me->reload=10;
		}
		if(me->seq==ANIM_A2 && me->frm>3)
		{
			ShakeScreen(5);
			// snowballs fall all over
			x=me->x+((320-Random(641))<<FIXSHIFT);
			y=me->y+((240-Random(481))<<FIXSHIFT);
			if(Random(3)==0)
				FireExactBullet(x,y,400*FIXAMT,0,0,0,0,100,me->facing*32,BLT_BIGSNOW,me->friendly);
			else
				FireExactBullet(x,y,400*FIXAMT,0,0,0,0,100,me->facing*32,BLT_SNOWBALL,me->friendly);
		}
		if(me->seq==ANIM_A1 && (me->frm==5 || me->frm==6))
		{
			for(i=0;i<4;i++)
			{
				x=me->x+Cosine(i*64)*64;
				y=me->y+Sine(i*64)*64;
				FireBullet(x,y,i*64,BLT_ICESPIKE,me->friendly);
			}
		}
		return;	// can't do nothin' right now
	}

	if(me->mind==0)
	{
		if((goodguy && RangeToTarget(me,goodguy)<(300*FIXAMT)) || (me->ouch))
		{
			me->mind=1;	// start into action
			me->mind1=0;
		}
	}
	else
	{
		if(me->mind1)
			me->mind1--;
		else
		{
			// rotate
			me->facing=(me->facing-1+Random(3))&7;
			me->mind1=Random(30)+1;
		}

		if((!me->reload) && goodguy && Random(20)==0)
		{
			// select an attack method
			x=Random(16);

			if(x<12) // 75% chance of primary weapon
			{
				// Sven throws snowballs
				if(x<7)
				{
					me->seq=ANIM_ATTACK;
					me->frm=0;
					me->frmTimer=0;
					me->frmAdvance=128;
					FaceGoodguy(me,goodguy);
					me->action=ACTION_BUSY;
				}
				else
				{
					// Bjorn causes stalag-quakes
					me->seq=ANIM_A1;
					me->frm=0;
					me->frmTimer=0;
					me->frmAdvance=128;
					FaceGoodguy(me,goodguy);
					me->action=ACTION_BUSY;
				}
			}
			else	// 25% chance of yodel
			{
				MakeSound(SND_YETIYODEL,me->x,me->y,SND_CUTOFF,1500);
				me->seq=ANIM_A2;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=64;
				me->facing=2;
				me->action=ACTION_BUSY;
			}
			me->dx=0;
			me->dy=0;
			return;
		}

		me->dx=Cosine(me->facing*32)*4;
		me->dy=Sine(me->facing*32)*4;

		if(me->seq!=ANIM_MOVE)
		{
			me->seq=ANIM_MOVE;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
		}

	}
}

void AI_Thing2(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y,f,f2;
	Guy *g;

	if(me->mindControl)
		f=2;
	else
		f=me->friendly;

	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_MUSHOUCH,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_MUSHDIE,me->x,me->y,SND_CUTOFF,1200);
	}
	
	if(me->mind1&1)
		me->dx=-me->dx*0.75;
	if(me->mind1&2)
		me->dy=-me->dy*0.75;
	me->mind1=0;

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_A2)
		{
			if(me->frm>4&&me->frm<13&&me->reload==0)
			{
				MakeSound(SND_MUSHSPORES,me->x,me->y,SND_CUTOFF,1500);
				f=Random(88)+20;
				f2=Random(88)+20;
				FireExactBullet(me->x-48*FIXAMT,me->y-10*FIXAMT,40*FIXAMT,Cosine(f)*8,Sine(f)*8,-0.5*FIXAMT,0,100,
								f,BLT_BIGSHELL,me->friendly);
				FireExactBullet(me->x+48*FIXAMT,me->y-10*FIXAMT,40*FIXAMT,Cosine(f2)*8,Sine(f2)*8,-0.5*FIXAMT,0,100,
								f2,BLT_BIGSHELL,me->friendly);
				me->reload=2;
			}
		}
		if(me->seq==ANIM_DIE)
		{
			if(me->frm==0)
				KillKids(me);	// destroy the tentacles
			x=me->x>>FIXSHIFT;
			y=me->y>>FIXSHIFT;
			BlowUpGuy(x+me->rectx,y+me->recty,x+me->rectx2,y+me->recty2,me->z,2);
			BlowSmoke((x+me->rectx+Random(me->rectx2-me->rectx))<<FIXSHIFT,
					  (y+me->recty+Random(me->recty2-me->recty))<<FIXSHIFT,
					  me->z,FIXAMT);
			me->x+=((1-Random(3))<<FIXSHIFT);
			me->y+=((1-Random(3))<<FIXSHIFT);
		}
		if(me->seq==ANIM_ATTACK && me->frm==3 && me->frmTimer<32)
		{
			g=AddBaby(me->x,me->y,0,MONS_WHATSIT,me);
			if(g)
			{
				g->parent=me;
				g->seq=ANIM_A1;	// take a nap
				g->frm=0;
				g->frmTimer=0;
				g->frmAdvance=32;
				g->action=ACTION_BUSY;
			}
		}
		return;	// can't do nothin' right now
	}
	if(Random(128)==0)	// blink
	{
		me->action=ACTION_BUSY;
		me->seq=ANIM_A2;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=64;
	}
	if(Random(128)==0)	// SWOOCE!
	{
		if (me->dy-10>=-10)
		me->dy-=10*FIXAMT;
		else
		me->dy=me->dy-10*FIXAMT;
	}
	if(me->reload==0 && Random(10)==0)
	{
		MakeSound(SND_MUSHMISSILE,me->x,me->y,SND_CUTOFF,1500);
		FireBullet(me->x-48*FIXAMT,me->y-10*FIXAMT,Random(88)+20,BLT_REDNADE,me->friendly);
		FireBullet(me->x+48*FIXAMT,me->y-10*FIXAMT,Random(88)+20,BLT_REDNADE,me->friendly);
		me->reload=30;
	}
	if(Random(128)==0)	// tongue
	{
		MakeSound(SND_THINGVOMIT,me->x,me->y,SND_CUTOFF,1500);
		me->action=ACTION_BUSY;
		me->seq=ANIM_ATTACK;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=128;
	}
	if(goodguy)
	{
		x=goodguy->x;
		y=goodguy->y-50*FIXAMT;

		FaceGoodguy2(me,goodguy);
		if(me->x<x)
			me->dx+=FIXAMT/12;
		if(me->y<y)
			me->dy+=FIXAMT/12;
		if(me->x>x)
			me->dx-=FIXAMT/12;
		if(me->y>y)
			me->dy-=FIXAMT/12;
		
		Clamp(&me->dx,FIXAMT*8);
		Clamp(&me->dy,FIXAMT*8);
	}
}

void AI_Sphinx2(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y,f2;
	Guy *g;
	byte f;

	if(me->mindControl)
		f=2;
	else
		f=me->friendly;

	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_SPHINXOUCH,me->x,me->y,SND_CUTOFF,600);
		else
			MakeSound(SND_SPHINXDIE,me->x,me->y,SND_CUTOFF,600);
	}
	
	if(me->mind==1)
	{
		if(goodguy)
		{
			// calculate desired location (want to be above Bouapha)
			x=goodguy->x;
			y=goodguy->y-96*FIXAMT;

			FaceGoodguy2(me,goodguy);
			if(me->x<x)
				me->dx+=FIXAMT/4;
			if(me->y<y)
				me->dy+=FIXAMT/4;
			if(me->x>x)
				me->dx-=FIXAMT/4;
			if(me->y>y)
				me->dy-=FIXAMT/4;

			Clamp(&me->dx,FIXAMT*6);
			Clamp(&me->dy,FIXAMT*6);
		}
	}

	if(me->action==ACTION_BUSY)
	{
		Dampen(&me->dx,FIXAMT*0.95);
		Dampen(&me->dy,FIXAMT*0.95);
		if(me->seq==ANIM_ATTACK)
		{
			if(me->frm>4&&me->frm<18&&me->reload==0)
			{
				f2=Random(88)+20;
				FireExactBullet(me->x,me->y-10*FIXAMT,56*FIXAMT,Cosine(f2)*8,Sine(f2)*8,12*FIXAMT,0,100, f2,BLT_GRENADE,me->friendly);
				MakeSound(SND_MUSHMISSILE,x,y,SND_CUTOFF,1050);
				me->reload=4;
			}
			else if(me->frm==18)
			me->reload=120;
		}
		if(me->seq==ANIM_A1 && me->frm==12 && me->frmTimer<32)
		{
			// sneeze some rocks out
			for(x=0;x<6;x++)
			{
				FireExactBullet(me->x,me->y+FIXAMT*40,60*FIXAMT,(8*FIXAMT-Random(17*FIXAMT)),
								Random(8*FIXAMT),0,0,60,2,BLT_BIGYELLOW,me->friendly);
			}
			// special rocks to deal with people trying to hide in "safe" spots
			FireExactBullet(me->x,me->y+FIXAMT*40,60*FIXAMT,8*FIXAMT,0,0,0,60,2,BLT_BIGYELLOW,me->friendly);
			FireExactBullet(me->x,me->y+FIXAMT*40,60*FIXAMT,-8*FIXAMT,0,0,0,60,2,BLT_BIGYELLOW,me->friendly);
			me->reload=120;
		}

		if(me->seq==ANIM_DIE)
		{
			if(me->frm==0 && me->frmTimer<32)
				KillKids(me);
			x=me->x>>FIXSHIFT;
			y=me->y>>FIXSHIFT;
			BlowUpGuy(x+me->rectx,y+me->recty,x+me->rectx2,y+me->recty2,me->z,2);
			BlowSmoke((x+me->rectx+Random(me->rectx2-me->rectx))<<FIXSHIFT,
					  (y+me->recty+Random(me->recty2-me->recty))<<FIXSHIFT,
					  me->z,FIXAMT);
			ShakeScreen(60);
		}
		return;
	}

	if(me->mind==0)
	{
		if(goodguy && (RangeToTarget(me,goodguy)<400*FIXAMT || me->ouch>0))
		{
			me->mind=1;
		}
	}
	if(me->mind==1)	// active
	{

		if(RangeToTarget(me,goodguy)<600*FIXAMT && goodguy->y>me->y && me->reload==0)
		{
			if(Random(3)==0)
			{
				MakeSound(SND_SPHINXSNEEZE,me->x,me->y,SND_CUTOFF,600);
				me->seq=ANIM_A1;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=128;
				me->action=ACTION_BUSY;
				return;
			}
			else	
			{
				me->seq=ANIM_ATTACK;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=128;
				me->action=ACTION_BUSY;
				return;
			}
		}
	}

	if(me->seq!=ANIM_IDLE)
	{
		me->seq=ANIM_IDLE;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=128;
	}
}

void AI_SDZL(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int i, x, y, w;
	
	w = (me->hp/me->maxHP)*-1+2; //multiplier

	if (!goodguy)
		return; // no point in doing anything without an opponent

	if (me->reload)
		me->reload--;

	if (me->ouch == 4)
	{
		if (me->hp > 0)
			MakeSound(SND_SDZLOUCH, me->x, me->y, SND_CUTOFF, 1200);
		else
			MakeSound(SND_SDZLDIE, me->x, me->y, SND_CUTOFF, 1200);
	}

	if (me->action == ACTION_BUSY)
	{
		if(me->seq==ANIM_DIE)
		{
			if(me->frm>=8)
			{
				x=me->x>>FIXSHIFT;
				y=me->y>>FIXSHIFT;
				BlowUpGuy(x+me->rectx,y+me->recty,x+me->rectx2,y+me->recty2,me->z,1);
				BlowSmoke((x+me->rectx+Random(me->rectx2-me->rectx))<<FIXSHIFT,
						  (y+me->recty+Random(me->recty2-me->recty))<<FIXSHIFT,
						  me->z,FIXAMT);
			}
		}
		
		if (me->seq == ANIM_ATTACK && me->frm >= 9 && me->frm <= 13 && me->mind1 == 0)
		{
			// slash for pain
			x = (me->x >> FIXSHIFT) + 50;
			y = (me->y >> FIXSHIFT) + 10;

			if (FindVictim(x, y, 100, -FIXAMT * 12, FIXAMT * 4, 25, map, world, me->friendly))
			{
				goodguy->dx = -FIXAMT * 24;
				goodguy->dy = FIXAMT * 4;
				me->mind1 = 1; // don't hit him again
			}
		}
		if (me->seq == ANIM_ATTACK && me->frm == 6 && me->mind1 == 0)
		{
			FireExactBullet(me->x + FIXAMT * 96, me->y + FIXAMT * 48, FIXAMT * 60, -FIXAMT * 15, FIXAMT * 8, FIXAMT *-4, 0, 60, 2, BLT_BIGSHELL, me->friendly);
			FireExactBullet(me->x + FIXAMT * 96, me->y + FIXAMT * 48, FIXAMT * 60, -FIXAMT * 12, FIXAMT * 8, FIXAMT *-4, 0, 60, 2, BLT_BIGSHELL, me->friendly);
			FireExactBullet(me->x + FIXAMT * 96, me->y + FIXAMT * 48, FIXAMT * 60, -FIXAMT * 9, FIXAMT * 8, FIXAMT *-4, 0, 60, 2, BLT_BIGSHELL, me->friendly);
			FireExactBullet(me->x + FIXAMT * 96, me->y + FIXAMT * 48, FIXAMT * 60, -FIXAMT * 6, FIXAMT * 8, FIXAMT *-4, 0, 60, 2, BLT_BIGSHELL, me->friendly);
			FireExactBullet(me->x + FIXAMT * 96, me->y + FIXAMT * 48, FIXAMT * 60, -FIXAMT * 3, FIXAMT * 8, FIXAMT *-4, 0, 60, 2, BLT_BIGSHELL, me->friendly);
			FireExactBullet(me->x + FIXAMT * 96, me->y + FIXAMT * 48, FIXAMT * 60, FIXAMT * 0, FIXAMT * 8, FIXAMT *-4, 0, 60, 2, BLT_BIGSHELL, me->friendly);
		}
		if (me->seq == ANIM_A5 && me->frm == 1 && me->frmTimer < 64)
		{
			for (i = 0; i < 5; i++)
			{
			x = me->x + Cosine(i*32)*16;
			y = me->y + Sine(i*32)*16;
			FireBullet(x, y, i*32, BLT_GRENADE, me->friendly);
			}
		}
		if (me->seq == ANIM_A4 && me->frm == 1 && me->frmTimer < 64)
		{
			for (i = 0; i < 8; i += 1)
			{FireBullet(me->x, me->y, i*32, BLT_GRENADE, me->friendly);}
			me->reload = 5;
			me->dx = Cosine(me->facing)*12*w;
			me->dy = Sine(me->facing)*12*w;
			SDZLGlow(me->x, me->y);
		}
		if (me->seq == ANIM_A4 && me->frm > 1 && me->frm % 2 == 0)
		{
			if(RangeToTarget(me,goodguy)<(80*FIXAMT) && Random(8)==0 && map->FindGuy(me->mapx,me->mapy,5,goodguy))
			{
				// get him!
				goodguy->GetShot(me->dx*4,me->dy*4,8,map,world);
			}
		}
		if (me->seq == ANIM_A2 && me->frm == 12 && me->frmTimer < 64)
		{
			// spew projectiles
			FireExactBullet(me->x, me->y + FIXAMT * 20, FIXAMT * 60,
					-FIXAMT * 12, FIXAMT * 4, FIXAMT, 0, 60, 2, BLT_GRENADE, me->friendly);
			FireExactBullet(me->x, me->y + FIXAMT * 20, FIXAMT * 60,
					-FIXAMT * 8, FIXAMT * 8, FIXAMT, 0, 60, 2, BLT_GRENADE, me->friendly);
			FireExactBullet(me->x, me->y + FIXAMT * 20, FIXAMT * 60,
					0, FIXAMT * 10, FIXAMT, 0, 60, 2, BLT_GRENADE, me->friendly);
			FireExactBullet(me->x, me->y + FIXAMT * 20, FIXAMT * 60,
					FIXAMT * 8, FIXAMT * 8, FIXAMT, 0, 60, 2, BLT_GRENADE, me->friendly);
			FireExactBullet(me->x, me->y + FIXAMT * 20, FIXAMT * 60,
					FIXAMT * 12, FIXAMT * 4, FIXAMT, 0, 60, 2, BLT_GRENADE, me->friendly);
		}
		if (me->seq == ANIM_A3 && (me->frm == 7 ))
		{
			FireBullet(me->x - FIXAMT*32, me->y + FIXAMT*96, 0, BLT_SHOCKWAVE, me->friendly);
		}
		if (me->seq == ANIM_A3 && (me->frm == 7 || me->frm == 8))
		{
			ShakeScreen(60);
			MakeSound(SND_SDZLPOUND, me->x, me->y, SND_CUTOFF, 1200);
			for (i = 0; i < 12; i++)
			{
				x = me->x + ((256 - Random(1025)) << FIXSHIFT);
				y = me->y + ((256 - Random(1025)) << FIXSHIFT);
				FireExactBullet(x, y, FIXAMT * 80 + (Random(40) << FIXSHIFT),
						0, 0, 0, 0, 30, 2, BLT_ROCK, me->friendly);
			}
		}

		if (me->seq == ANIM_DIE && me->frm == 14 && me->frmTimer < 32)
		{
		}
		return; // can't do nothin' right now
	}

	me->mind1 = 0;

	FaceGoodguy2(me, goodguy);

	if (me->reload == 0 && RangeToTarget(me, goodguy) < 256 * FIXAMT)
	{
		i = Random(1024);
		if (i < 40)
		{
			MakeSound(SND_SDZLSLASH, me->x, me->y, SND_CUTOFF, 1200);
			me->seq = ANIM_A3; // ground pound!
			me->frmTimer = 0;
			me->frm = 0;
			me->frmAdvance = 128;
			me->dx = 0;
			me->dy = 0;
			me->action = ACTION_BUSY;
			me->reload = 60;
			return;
		}
		else if (i < 100)
		{
			MakeSound(SND_SDZLSPIT, me->x, me->y, SND_CUTOFF, 1200);
			me->seq = ANIM_A2; // breathe stuff
			me->frmTimer = 0;
			me->frm = 0;
			me->frmAdvance = 128;
			me->dx = 0;
			me->dy = 0;
			me->action = ACTION_BUSY;
			me->reload = 60;
			return;
		}
		else if (i < 180)
		{
			MakeSound(SND_SDZLSLASH, me->x, me->y, SND_CUTOFF, 1200);
			me->seq = ANIM_ATTACK; // slash
			me->frmTimer = 0;
			me->frm = 0;
			me->frmAdvance = 128;
			me->dx = 0;
			me->dy = 0;
			me->action = ACTION_BUSY;
			me->reload = 30;
			return;
		}
		else if (i < 260)
		{
			MakeSound(SND_SDZLSPD, me->x, me->y, SND_CUTOFF, 1200);
			me->seq = ANIM_A4; // slash
			me->facing = ((me->facing * 32) - 16 + Random(33));
			me->frmTimer = 0;
			me->frm = 0;
			me->frmAdvance = 128;
			me->dx = 0;
			me->dy = 0;
			me->action = ACTION_BUSY;
			me->reload = 60;
			return;
		}
		else if (i < 340)
		{
			MakeSound(SND_SDZLSPIT2, me->x, me->y, SND_CUTOFF, 1200);
			me->seq = ANIM_A5; // slash
			me->facing = ((me->facing * 32) - 16 + Random(33));
			me->frmTimer = 0;
			me->frm = 0;
			me->frmAdvance = 128;
			me->dx = 0;
			me->dy = 0;
			me->action = ACTION_BUSY;
			me->reload = 30;
			return;
		}
	}

	if (me->facing == 7 || me->facing < 3)
		i = ANIM_MOVE; // move to the right
	else
		i = ANIM_A1; // move to the left
	if (me->seq == ANIM_MOVE || me->seq == ANIM_A1)
	{
		if (me->facing == 0 || me->facing == 1 || me->facing == 7)
		me->dx += FIXAMT*w;
		else if (me->facing == 3 || me->facing == 4 || me->facing == 5)
			me->dx -= FIXAMT*w;
		if (me->facing == 1 || me->facing == 2 || me->facing == 3)
			me->dy += FIXAMT*w;
		else if (me->facing == 5 || me->facing == 6 || me->facing == 7)
			me->dy -= FIXAMT*w;

		Dampen(&me->dx, FIXAMT / 2);
		Dampen(&me->dy, FIXAMT / 2);
		Clamp(&me->dx, FIXAMT * 5);
		Clamp(&me->dy, FIXAMT * 5);
	}
	if (me->seq != i)
	{
		me->seq = i;
		me->frmTimer = 0;
		me->frm = 0;
		me->frmAdvance = 128;
		me->dx = Cosine(me->facing * 32)*6;
		me->dy = Sine(me->facing * 32)*4;
	}
}