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
    Plane scoreText;
    Player player;
    int playerLives = 3, playerScore = 0;
    std::vector<Plane> playerLifeIndicators;
    std::vector<Plane> playerScoreIndicators;
    std::vector<Plane> barricades;
    std::vector<PlayerBullet> playerBullets;
    std::vector<EnemyBullet> enemyBullets;
    std::vector<Alien> aliens;

    SDL_Surface* images[19];
    GLuint textures[19];

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

    	glGenTextures(19, textures);

        for (int i = 0; i < 19; i++) {
            glBindTexture(GL_TEXTURE_2D, textures[i]);
        	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, images[i]->w, images[i]->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, images[i]->pixels);
        	glGenerateMipmap(GL_TEXTURE_2D);
        	glBindTexture(GL_TEXTURE_2D, 0);
        	SDL_FreeSurface(images[i]);
        }
    }

    bool DoCollisions(double deltaTime) {
        bulletTimer += deltaTime;

        std::vector<Plane>::iterator barricadeIT;
        std::vector<Alien>::iterator alienIT;
        std::vector<PlayerBullet>::iterator pBulletIT;
        std::vector<EnemyBullet>::iterator eBulletIT;

        for (pBulletIT = playerBullets.begin(); pBulletIT < playerBullets.end();) {
            bool shouldRemove = false;

            if (pBulletIT->shouldDestroy) shouldRemove = true;

            if (!shouldRemove) {
                for (alienIT = aliens.begin(); alienIT < aliens.end(); alienIT++) {
                    if (alienIT->CheckCollision(pBulletIT->position.x, pBulletIT->position.y + (pBulletIT->h / 2)) && !shouldRemove && alienIT->isAlive) {
                        BulletImpact();
                        alienIT->isAlive = false;
                        shouldRemove = true;
                        playerScore += 10;
                        break;
                    }
                }

                for (barricadeIT = barricades.begin(); barricadeIT != barricades.end();) {
                    if (barricadeIT->CheckCollision(pBulletIT->position.x, pBulletIT->position.y) && !shouldRemove) {
                        barricadeIT = barricades.erase(barricadeIT);
                        shouldRemove = true;
                        break;
                    } else {
                        ++barricadeIT;
                    }
                }
            }

            if (shouldRemove) pBulletIT = playerBullets.erase(pBulletIT);
            else pBulletIT++;
        }
        for (alienIT = aliens.begin(); alienIT < aliens.end();) {
            if (!alienIT->isAlive) {
                alienIT->deathAnimTimer += deltaTime;
                alienIT->position.Move(0, deltaTime * -0.5f, 0);
                alienIT->rotation.Move(deltaTime * -75.0f, deltaTime * -75.0f, deltaTime * -75.0f);
                alienIT->scale.Move(deltaTime * -0.75f, deltaTime * -0.75f, deltaTime * -0.75f);

                if (alienIT->deathAnimTimer >= 1.5f) alienIT = aliens.erase(alienIT);
                else alienIT++;
            } else {
                alienIT++;
            }
        }

        bool hitPlayer;
        for (eBulletIT = enemyBullets.begin(); eBulletIT < enemyBullets.end();) {
            bool shouldRemove = false, hitPlayer = false;

            if (eBulletIT->shouldDestroy) shouldRemove = true;

            if (!shouldRemove) {
                for (barricadeIT = barricades.begin(); barricadeIT != barricades.end();) {
                    if (barricadeIT->CheckCollision(eBulletIT->position.x, eBulletIT->position.y) && !shouldRemove) {
                        barricadeIT = barricades.erase(barricadeIT);
                        shouldRemove = true;
                        break;
                    } else {
                        ++barricadeIT;
                    }
                }

                if (player.CheckCollision(eBulletIT->position.x, eBulletIT->position.y) && !shouldRemove && !hitPlayer) {
                    std::cout << "hit player" << std::endl;
                    hitPlayer = true;
                    shouldRemove = true;
                    playerLives--;
                    if (playerLifeIndicators.size() > 0) playerLifeIndicators.pop_back();
                }
            }

            if (playerLives < 1) return true;
            if (shouldRemove) eBulletIT = enemyBullets.erase(eBulletIT);
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
