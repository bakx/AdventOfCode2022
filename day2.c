#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Game 
{
    int round;
    char playerA;
    char playerB;
    int result; // -1 lose, 0 draw, 1 won
    int points;
} Game;

#define ROCK 'X'
#define PAPER 'Y'
#define SCISSOR 'Z'

#define ROCK_OTHER 'A'
#define PAPER_OTHER 'B'
#define SCISSOR_OTHER 'C'

#define GAME_DRAW 0
#define GAME_LOSE -1
#define GAME_WIN 1

#define SCORE_DRAW 3
#define SCORE_WIN 6

int getPoints(char play)
{
    switch (play)
    {
        case ROCK:
        case ROCK_OTHER:
            return 1;
        case PAPER:
        case PAPER_OTHER:
            return 2;
        case SCISSOR:
        case SCISSOR_OTHER:
            return 3;
    }

    return 0;
}

int playGame(Game* game)
{
    if (game->playerA == ROCK)
    {
        if (game->playerB == ROCK_OTHER)
        {
            game->result = GAME_DRAW;
            game->points = SCORE_DRAW + getPoints(game->playerA);
        }
        else if (game->playerB == PAPER_OTHER)
        {
            game->result = GAME_LOSE;
            game->points = getPoints(game->playerA);
        }
        else if (game->playerB == SCISSOR_OTHER)
        {
            game->result = GAME_WIN;
            game->points = SCORE_WIN + getPoints(game->playerA);
        }
    }
    else if (game->playerA == PAPER)
    {
        if (game->playerB == ROCK_OTHER)
        {
            game->result = GAME_WIN;
            game->points = SCORE_WIN + getPoints(game->playerA);
        }
        else if (game->playerB == PAPER_OTHER)
        {
            game->result = GAME_DRAW;
            game->points = SCORE_DRAW + getPoints(game->playerA);
        }
        else if (game->playerB == SCISSOR_OTHER)
        {
            game->result = GAME_LOSE;
            game->points = getPoints(game->playerA);
        }
    }
    else if (game->playerA == SCISSOR)
    {
        if (game->playerB == ROCK_OTHER)
        {
            game->result = GAME_LOSE;
            game->points = getPoints(game->playerA);
        }
        else if (game->playerB == PAPER_OTHER)
        {
            game->result = GAME_WIN;
            game->points = SCORE_WIN + getPoints(game->playerA);
        }
        else if (game->playerB == SCISSOR_OTHER)
        {
            game->result = GAME_DRAW;
            game->points = SCORE_DRAW + getPoints(game->playerA);
        }
    }
}

int main(int argc, const char *const argv[])
{
    FILE * fp = fopen("day2.txt", "r");
    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    long int result = 0;;

    while ((read = getline(&line, &len, fp)) != -1)
    {
        // Prep game
        Game* g = malloc(sizeof(Game));
        g->playerA = line[2];
        g->playerB = line[0];

        playGame(g);
        result += g->points;
    }

    printf("Total points %ld\n", result);
    
    free(line);
    fclose(fp);
}