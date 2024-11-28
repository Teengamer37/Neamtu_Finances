#include "gtest/gtest.h"

#include "../GameCharacter.h"


class GameCharacterSuite : public ::testing::Test {

protected:
    virtual void SetUp() {
        c = new GameCharacter(10);
        c->setXPos(10);
        c->setYPos(10);
        c->setFighting(true);
    }

    GameCharacter* c = nullptr;  // XXX C++11 standard initialization
};


TEST_F(GameCharacterSuite, TestMove) {
    c->move(12, 14);

    ASSERT_EQ(22, c->getXPos());
    ASSERT_EQ(24, c->getYPos());
}

TEST_F(GameCharacterSuite, TestFight) {
    GameCharacter enemy(3);
    ASSERT_EQ(enemy.getHP(), 3);
    c->fight(enemy);
    ASSERT_EQ(enemy.getHP(), 2);
    c->fight(enemy);
    ASSERT_EQ(enemy.getHP(), 1);
    c->fight(enemy);
    ASSERT_EQ(enemy.getHP(), 0);
    c->fight(enemy);
    ASSERT_EQ(enemy.getHP(), 0);
}
