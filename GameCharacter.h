#ifndef UNIT_TESTING_GAMECHARACTER_H
#define UNIT_TESTING_GAMECHARACTER_H


class GameCharacter {
public:
    explicit GameCharacter(int hp, int x = 0, int y = 0) : HP(hp), xPos(x), yPos(y), fighting(false) { };

    void move(int x, int y);

    void fight(GameCharacter& enemy);

    bool isFighting() const {
        return fighting;
    }

    void setFighting(bool fighting) {
        GameCharacter::fighting = fighting;
    }

    int getXPos() const {
        return xPos;
    }

    void setXPos(int x) {
        GameCharacter::xPos = x;
    }

    int getYPos() const {
        return yPos;
    }

    void setYPos(int y) {
        GameCharacter::yPos = y;
    }

    int getHP() const {
        return HP;
    }

private:
    int xPos, yPos;
    int HP;
    bool fighting;
};


#endif //UNIT_TESTING_GAMECHARACTER_H
