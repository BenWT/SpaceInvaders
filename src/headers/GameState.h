#pragma once
#include <iostream>
#include <vector>
#include <iterator>
#include "Vector3.h"
#include "ObjectTypes/Alien.h"
#include "ObjectTypes/Player.h"
#include "ObjectTypes/PlayerBullet.h"
#include "ObjectTypes/EnemyBullet.h"

class GameState {
public:
    Plane background;
    Plane asteroids;
    Player player;
    std::vector<PlayerBullet> playerBullets;
    std::vector<EnemyBullet> enemyBullets;
    std::vector<Alien> aliens;

    SDL_Surface* images[5];
    GLuint textures[5];

    double bulletTimer = 0.0, fireDelay = 0.5, endgameCounter = 0.0;
    bool isEndgame = false;

    GameState() {
        bulletTimer = fireDelay;
    }

    void GenerateTextures() {
    	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);

    	glGenTextures(5, textures);

        for (int i = 0; i < 5; i++) {
            glBindTexture(GL_TEXTURE_2D, textures[i]);
        	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, images[i]->w, images[i]->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, images[i]->pixels);
        	glGenerateMipmap(GL_TEXTURE_2D);
        	glBindTexture(GL_TEXTURE_2D, 0);
        	SDL_FreeSurface(images[i]);
        }
    }

    bool DoCollisions(double deltaTime) {
        bulletTimer += deltaTime;

        std::vector<Alien>::iterator alienIT;
        std::vector<PlayerBullet>::iterator pBulletIT;
        std::vector<EnemyBullet>::iterator eBulletIT;

        for (pBulletIT = playerBullets.begin(); pBulletIT < playerBullets.end();) {
            bool shouldRemove = false;

            for (alienIT = aliens.begin(); alienIT < aliens.end(); alienIT++) {
                if (pBulletIT->shouldDestroy) {
                    shouldRemove = true;
                    break;
                } else if (alienIT->CheckCollision(pBulletIT->position.x, pBulletIT->position.y) && alienIT->isAlive) {
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
                alienIT->deathAnimTimer += deltaTime;
                alienIT->position.Move(0, deltaTime * -0.3f, 0);
                alienIT->rotation.Move(deltaTime * -45.0f, deltaTime * -45.0f, deltaTime * -45.0f);
                alienIT->scale.Move(deltaTime * -0.45f, deltaTime * -0.45f, deltaTime * -0.45f);

                if (alienIT->deathAnimTimer >= 2.0f) alienIT = aliens.erase(alienIT);
                else alienIT++;
            } else {
                alienIT++;
            }
        }

        for (eBulletIT = enemyBullets.begin(); eBulletIT < enemyBullets.end();) {
            // TODO destroy walls here
            if (player.CheckCollision(eBulletIT->position.x, eBulletIT->position.y)) {
                std::cout << "player" << std::endl;
                return true;
            }

            if (eBulletIT->shouldDestroy) eBulletIT = enemyBullets.erase(eBulletIT);
            else eBulletIT++;
        }
        return false;
    }

    void PlayerFire() {
        if (bulletTimer >= fireDelay) {
            PlayerBullet* b = new PlayerBullet(player.position.x, player.position.y, 0.025f, 0.12f);
            playerBullets.push_back(*b);
            delete b;

            bulletTimer = 0.0;
        }
    }

    void EnemyFire(int i) {
        EnemyBullet* b = new EnemyBullet(aliens[i].position.x, aliens[i].position.y, 0.025f, 0.12f);
        enemyBullets.push_back(*b);
        delete b;
    }

    void BulletImpact() {

    }
};
