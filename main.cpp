#include <conio.h>
#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <chrono>
#include <thread>

const int MAP_WIDTH = 40;
const int MAP_HEIGHT = 20;
const int MAX_HP = 5;
const int FRAME_MS = 50;

struct Asteroid {
    int x;
    int y;
    int hp;
    int points;
};

struct Bullet {
    int x;
    int y;
};

unsigned long long getTimeMs() {
    using namespace std::chrono;
    return static_cast<unsigned long long>(duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count());
}

Asteroid createAsteroid() {
    int points = rand() % 5 + 1;
    int hp = (points >= 3 ? 2 : 1);
    return {rand() % MAP_WIDTH, 0, hp, points};
}

void clearScreen() {
    std::cout << std::string(50, '\n');
}

int main() {
    srand(static_cast<unsigned int>(time(NULL)));

    int shipX = MAP_WIDTH / 2;
    int hp = MAX_HP;
    int score = 0;
    bool running = true;
    bool laserMode = false;
    unsigned long long laserReleaseTime = 0;
    unsigned long long lastBulletTime = 0;
    unsigned long long lastSpawnTime = 0;

    std::vector<Asteroid> asteroids;
    std::vector<Bullet> bullets;

    while (running) {
        unsigned long long frameStart = getTimeMs();

        if (_kbhit()) {
            int key = _getch();
            if (key == 0 || key == 224) {
                int arrow = _getch();
                if (arrow == 75 && shipX > 0) {
                    shipX -= 1;
                } else if (arrow == 77 && shipX < MAP_WIDTH - 1) {
                    shipX += 1;
                }
            } else if (key == 'q' || key == 'Q' || key == 27) {
                running = false;
            }
        }

        if (frameStart - lastBulletTime >= 150) {
            bullets.push_back({shipX, MAP_HEIGHT - 2});
            lastBulletTime = frameStart;
        }

        if (frameStart - lastSpawnTime >= 500) {
            asteroids.push_back(createAsteroid());
            lastSpawnTime = frameStart;
        }

        if (!laserMode && score >= 40) {
            laserMode = true;
            laserReleaseTime = frameStart + 2000;
        }
        if (laserMode && frameStart >= laserReleaseTime) {
            laserMode = false;
        }

        for (size_t i = 0; i < bullets.size();) {
            bullets[i].y -= 1;
            if (bullets[i].y < 0) {
                bullets.erase(bullets.begin() + static_cast<long>(i));
            } else {
                ++i;
            }
        }

        for (size_t i = 0; i < asteroids.size();) {
            asteroids[i].y += 1;
            if (asteroids[i].y >= MAP_HEIGHT) {
                asteroids.erase(asteroids.begin() + static_cast<long>(i));
            } else {
                ++i;
            }
        }

        for (size_t b = 0; b < bullets.size();) {
            bool bulletRemoved = false;
            for (size_t a = 0; a < asteroids.size() && !bulletRemoved;) {
                if (bullets[b].x == asteroids[a].x && bullets[b].y == asteroids[a].y) {
                    asteroids[a].hp -= 1;
                    bullets.erase(bullets.begin() + static_cast<long>(b));
                    bulletRemoved = true;
                    if (asteroids[a].hp <= 0) {
                        score += asteroids[a].points;
                        asteroids.erase(asteroids.begin() + static_cast<long>(a));
                    } else {
                        ++a;
                    }
                } else {
                    ++a;
                }
            }
            if (!bulletRemoved) {
                ++b;
            }
        }

        if (laserMode) {
            for (size_t a = 0; a < asteroids.size();) {
                if (asteroids[a].x == shipX && asteroids[a].y < MAP_HEIGHT - 1) {
                    score += asteroids[a].points;
                    asteroids.erase(asteroids.begin() + static_cast<long>(a));
                } else {
                    ++a;
                }
            }
        }

        for (size_t a = 0; a < asteroids.size();) {
            if (asteroids[a].y == MAP_HEIGHT - 1 && asteroids[a].x == shipX) {
                hp -= 1;
                asteroids.erase(asteroids.begin() + static_cast<long>(a));
            } else {
                ++a;
            }
        }

        if (hp <= 0) {
            running = false;
        }

        std::vector<std::string> screen(MAP_HEIGHT, std::string(MAP_WIDTH, ' '));
        for (const auto& asteroid : asteroids) {
            if (asteroid.y >= 0 && asteroid.y < MAP_HEIGHT && asteroid.x >= 0 && asteroid.x < MAP_WIDTH) {
                char symbol = static_cast<char>('0' + asteroid.points);
                screen[asteroid.y][asteroid.x] = symbol;
            }
        }
        for (const auto& bullet : bullets) {
            if (bullet.y >= 0 && bullet.y < MAP_HEIGHT && bullet.x >= 0 && bullet.x < MAP_WIDTH) {
                screen[bullet.y][bullet.x] = '|';
            }
        }
        if (laserMode) {
            for (int y = 0; y < MAP_HEIGHT - 1; ++y) {
                screen[y][shipX] = '^';
            }
        }
        screen[MAP_HEIGHT - 1][shipX] = 'A';

        clearScreen();
        std::cout << "PONTOS: " << score << "   HP: ";
        for (int i = 0; i < hp; ++i) {
            std::cout << "<3";
        }
        std::cout << "   ";
        if (laserMode) {
            std::cout << "LASER ATIVO!";
        } else if (score >= 40) {
            std::cout << "LASER PRONTO!";
        }
        std::cout << "\n";
        std::cout << std::string(MAP_WIDTH + 2, '=') << "\n";

        for (int y = 0; y < MAP_HEIGHT; ++y) {
            std::cout << "|" << screen[y] << "|\n";
        }
        std::cout << std::string(MAP_WIDTH + 2, '=') << "\n";
        std::cout << "Use as setas esquerda/direita ou Q para sair." << "\n";
        std::cout << "Aperte Enter para sair apos o game over." << "\n";

        unsigned long long frameEnd = getTimeMs();
        unsigned long long elapsed = frameEnd > frameStart ? frameEnd - frameStart : 0;
        if (elapsed < FRAME_MS) {
            std::this_thread::sleep_for(std::chrono::milliseconds(FRAME_MS - elapsed));
        }
    }

    std::cout << "\nGAME OVER\n";
    std::cout << "Pontuacao final: " << score << "\n";
    std::cout << "Pressione qualquer tecla para fechar...";
    _getch();
    return 0;
}
