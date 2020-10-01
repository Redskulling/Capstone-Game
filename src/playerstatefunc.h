#pragma once
#include "player.h"
#include "playerstates.h"

void playerStateStationary(Player *p);
void playerStateMoing(Player *p);
void playerStateDash(Player *p);
void playerStateAttacking(Player *p);


void runPlayerState(Player *p);
