#pragma once
#include <iostream>
#include <vector>
#include <iterator>
#include "ObjectTypes/Alien.h"
#include "ObjectTypes/Player.h"
#include "ObjectTypes/PlayerBullet.h"

class GameState {
public:
    Plane background;
    Player player;
    std::vector<PlayerBullet> playerBullets;
    std::vector<Alien> aliens;

    double bulletTimer = 0.0, fireDelay = 0.5;

    GameState() {}

    void DoCollisions(double deltaTime) {
        bulletTimer += deltaTime;

        std::vector<Alien>::iterator alienIT;
        std::vector<PlayerBullet>::iterator pBulletIT = playerBullets.begin();

        for (pBulletIT = playerBullets.begin(); pBulletIT < playerBullets.end();) {
            bool shouldRemove = false;

            for (alienIT = aliens.begin(); alienIT < aliens.end(); alienIT++) {
                if (pBulletIT->shouldDestroy) {
                    shouldRemove = true;
                    break;
                } else if (alienIT->CheckCollision(pBulletIT->xPos, pBulletIT->yPos) && alienIT->isAlive) {
                    BulletImpact();
                    alienIT->isAlive = false;
                    shouldRemove = true;
                    break;
                }
            }

            if (shouldRemove) pBulletIT = playerBullets.erase(pBulletIT);
            else pBulletIT++;
        }

        for (alienIT = aliens.begin(); alienIT < aliens.end();) {
            if (!alienIT->isAlive) {
                // do some animation stuff
                alienIT = aliens.erase(alienIT);
            } else {
                alienIT++;
            }
        }
    }

    void PlayerFire() {
        if (bulletTimer >= fireDelay) {
            PlayerBullet* b = new PlayerBullet(player.xPos, player.yPos, 0.025f, 0.12f);
            playerBullets.push_back(*b);
            delete b;

            bulletTimer = 0.0;
        }
    }

    void BulletImpact() {

    }
};
