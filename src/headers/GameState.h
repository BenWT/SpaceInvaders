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

    SDL_Surface* images[2];
    GLuint textures[2];

    double bulletTimer = 0.0, fireDelay = 0.5;

    GameState() {
        bulletTimer = fireDelay;
    }

    void GenerateTextures() {
    	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    	glGenTextures(2, textures);

    	glBindTexture(GL_TEXTURE_2D, textures[0]);
    	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, images[0]->w, images[0]->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, images[0]->pixels);
    	glGenerateMipmap(GL_TEXTURE_2D);
    	glBindTexture(GL_TEXTURE_2D, 0);
    	SDL_FreeSurface(images[0]);

        glBindTexture(GL_TEXTURE_2D, textures[1]);
    	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, images[1]->w, images[1]->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, images[1]->pixels);
    	glGenerateMipmap(GL_TEXTURE_2D);
    	glBindTexture(GL_TEXTURE_2D, 0);
    	SDL_FreeSurface(images[1]);
    }

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
            alienIT->moveAmount += deltaTime * 0.005f;

            if (!alienIT->isAlive) {
                alienIT->deathAnimTimer += deltaTime;
                alienIT->Move(0, deltaTime * -0.3f, 0);
                alienIT->Rotate(deltaTime * -45.0f, deltaTime * -45.0f, deltaTime * -45.0f);
                alienIT->Scale(deltaTime * -0.45f, deltaTime * -0.45f, deltaTime * -0.45f);

                if (alienIT->deathAnimTimer >= 2.0f) alienIT = aliens.erase(alienIT);
                else alienIT++;
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
