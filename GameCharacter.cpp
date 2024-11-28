#include "GameCharacter.h"


void GameCharacter::move(int x, int y) {
    xPos += x;
    yPos += y;
    if (xPos < 0)
        xPos = 0;
    if (yPos < 0)
        yPos = 0;
    fighting = false;
}

void GameCharacter::fight(GameCharacter &enemy) {
    fighting = true;
    if (enemy.HP > 0)
        enemy.HP -= 1;
}

