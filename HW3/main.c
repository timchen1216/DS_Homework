// HW2 shogi

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <ev.h>

/*........................................constant......................................*/
#define BWEIGHT 9
#define BHEIGHT 9
#define NAMESIZE 4
#define STRLENGTH 8
#define STACKSIZE 100
#define DATESIZE 40

/*.....................................Data Structure...................................*/
struct piece
{
    char name[NAMESIZE];
    char controller[STRLENGTH];
};

// Linked List
struct node
{
    char word[STRLENGTH];
    struct node *next;
};
typedef struct node NODE;

/*.........................................function......................................*/
/*.............board.............*/

void board_initial(struct piece board[][BHEIGHT])
{
    // name
    for (int i = 0; i < BWEIGHT; i++)
    {
        for (int j = 0; j < BHEIGHT; j++)
        {
            if (j == 2 || j == 6)
                strcpy((*(board + i) + j)->name, "步");
            else if ((i == 1 && j == 7) || (i == 7 && j == 1))
                strcpy((*(board + i) + j)->name, "角");
            else if ((i == 1 && j == 1) || (i == 7 && j == 7))
                strcpy((*(board + i) + j)->name, "飛");
            else if ((i == 0 && j == 0) || (i == 8 && j == 0) || (i == 0 && j == 8) || (i == 8 && j == 8))
                strcpy((*(board + i) + j)->name, "香");
            else if ((i == 1 && j == 0) || (i == 7 && j == 0) || (i == 1 && j == 8) || (i == 7 && j == 8))
                strcpy((*(board + i) + j)->name, "桂");
            else if ((i == 2 && j == 0) || (i == 6 && j == 0) || (i == 2 && j == 8) || (i == 6 && j == 8))
                strcpy((*(board + i) + j)->name, "銀");
            else if ((i == 3 && j == 0) || (i == 5 && j == 0) || (i == 3 && j == 8) || (i == 5 && j == 8))
                strcpy((*(board + i) + j)->name, "金");
            else if ((i == 4 && j == 0) || (i == 4 && j == 8))
                strcpy((*(board + i) + j)->name, "王");
            else
                strcpy((*(board + i) + j)->name, "  ");
        }
    }

    // controller
    for (int i = 0; i < BWEIGHT; i++)
    {
        for (int j = 0; j < BHEIGHT; j++)
        {
            if (j == 0 || (i == 1 && j == 1) || (i == 7 && j == 1) || j == 2)
                strcpy((*(board + i) + j)->controller, "Red");
            else if (j == 6 || (i == 1 && j == 7) || (i == 7 && j == 7) || j == 8)
                strcpy((*(board + i) + j)->controller, "Blue");
            else
                strcpy((*(board + i) + j)->controller, "Free");
        }
    }
}

void board_show(struct piece (*board)[BHEIGHT])
{
    printf("9 8 7 6 5 4 3 2 1\n");
    for (int j = 0; j < BHEIGHT; j++)
    {
        for (int i = 0; i <= BWEIGHT; i++)
        {
            if (i == BWEIGHT)
                printf("%d\n", j + 1);
            else
            {
                if (strcmp((*(board + i) + j)->controller, "Red") == 0)
                    printf("\033[31m%s\033[m", (*(board + i) + j)->name);
                else if (strcmp((*(board + i) + j)->controller, "Blue") == 0)
                    printf("\033[34m%s\033[m", (*(board + i) + j)->name);
                else
                    printf("%s", (*(board + i) + j)->name);
            }
        }
    }
}

void board_show_controller(struct piece (*board)[BHEIGHT])
{
    printf("9 8 7 6 5 4 3 2 1\n");
    for (int j = 0; j < BHEIGHT; j++)
    {
        for (int i = 0; i <= BWEIGHT; i++)
        {
            if (i == BWEIGHT)
                printf("%d\n", j + 1);
            else
                printf("%s", (*(board + i) + j)->controller);
        }
    }
}

/*.............move.............*/
void swap(struct piece *A, struct piece *B)
{
    struct piece word = *A;
    *A = *B;
    *B = word;
}

void input_movement(bool *turn, char *alpha, int *input_Srow, int *input_Ecol, int *input_Erow)
{
    *turn ? printf("\n\033[31m紅方行棋\033[m\n") : printf("\n\033[34m藍方行棋\033[m\n");
    printf("請輸入棋子的段(行)、筋(列)和下一步的段(行)、筋(列)。中間以空格區隔(悔棋:0 存檔:s/S 結束:e/E)：");
    *alpha = ' ';
    scanf("%s", alpha);
    if (*alpha == '0' || *alpha == 's' || *alpha == 'S' || *alpha == 'e' || *alpha == 'E')
        return;
    scanf("%d %d %d", input_Srow, input_Ecol, input_Erow);
}

void transfer(int *input_Scol, int *input_Srow, int *input_Ecol, int *input_Erow, int *tr_Scol, int *tr_Srow, int *tr_Ecol, int *tr_Erow, int *move_x, int *move_y)
{
    *tr_Scol = *input_Scol - 1;
    *tr_Ecol = *input_Ecol - 1;
    *tr_Srow = 9 - *input_Srow;
    *tr_Erow = 9 - *input_Erow;
    *move_x = *tr_Erow - *tr_Srow;
    *move_y = *tr_Ecol - *tr_Scol;

    // printf("%d %d\n", *move_x, *move_y);
}

bool pawn(const int *move_x, const int *move_y, int *Scol, int *Srow, int *Ecol, int *Erow, struct piece (*board)[BHEIGHT])
{
    // Blue
    if (strcmp((*(board + *Srow) + *Scol)->controller, "Blue") == 0)
    {
        if (*move_x == 0 && *move_y == -1 && strcmp((*(board + *Erow) + *Ecol)->controller, "Blue") != 0)
            return true;
    }
    // Red
    if (strcmp((*(board + *Srow) + *Scol)->controller, "Red") == 0)
    {
        if (*move_x == 0 && *move_y == 1 && strcmp((*(board + *Erow) + *Ecol)->controller, "Red") != 0)
            return true;
    }
    return false;
}

bool Lance(const int *move_x, const int *move_y, int *Scol, int *Srow, int *Ecol, int *Erow, struct piece (*board)[BHEIGHT])
{
    // Blue
    if (strcmp((*(board + *Srow) + *Scol)->controller, "Blue") == 0)
    {
        if (*move_x == 0 && *move_y <= -1)
        {
            for (int i = -1; i >= *move_y; i--)
            {
                if (strcmp((*(board + *Srow) + *Scol + i)->controller, "Blue") == 0)
                    return false;
            }
            return true;
        }
    }
    // Red
    if (strcmp((*(board + *Srow) + *Scol)->controller, "Red") == 0)
    {
        if (*move_x == 0 && *move_y >= 1)
        {
            for (int i = 1; i <= *move_y; i++)
            {
                if (strcmp((*(board + *Srow) + *Scol + i)->controller, "Red") == 0)
                    return false;
            }
            return true;
        }
    }
    return false;
}

bool knight(const int *move_x, const int *move_y, int *Scol, int *Srow, int *Ecol, int *Erow, struct piece (*board)[BHEIGHT])
{
    // Blue
    if (strcmp((*(board + *Srow) + *Scol)->controller, "Blue") == 0)
    {
        if (((*move_x == -1 && *move_y == -2) || (*move_x == 1 && *move_y == -2)) && strcmp((*(board + *Erow) + *Ecol)->controller, "Blue") != 0)
            return true;
    }
    // Red
    if (strcmp((*(board + *Srow) + *Scol)->controller, "Red") == 0)
    {
        if (((*move_x == -1 && *move_y == 2) || (*move_x == 1 && *move_y == 2)) && strcmp((*(board + *Erow) + *Ecol)->controller, "Red") != 0)
            return true;
    }
    return false;
}

bool Silver(const int *move_x, const int *move_y, int *Scol, int *Srow, int *Ecol, int *Erow, struct piece (*board)[BHEIGHT])
{
    // Blue
    if (strcmp((*(board + *Srow) + *Scol)->controller, "Blue") == 0)
    {
        if (strcmp((*(board + *Erow) + *Ecol)->controller, "Blue") != 0)
        {
            if (*move_x == -1 && *move_y == -1)
                return true;
            if (*move_x == 0 && *move_y == -1)
                return true;
            if (*move_x == 1 && *move_y == -1)
                return true;
            if (*move_x == -1 && *move_y == 1)
                return true;
            if (*move_x == 1 && *move_y == 1)
                return true;
            else
                return false;
        }
    }
    // Red
    if (strcmp((*(board + *Srow) + *Scol)->controller, "Red") == 0)
    {
        if (strcmp((*(board + *Erow) + *Ecol)->controller, "Red") != 0)
        {
            if (*move_x == -1 && *move_y == 1)
                return true;
            if (*move_x == 0 && *move_y == 1)
                return true;
            if (*move_x == 1 && *move_y == 1)
                return true;
            if (*move_x == -1 && *move_y == -1)
                return true;
            if (*move_x == 1 && *move_y == -1)
                return true;
            else
                return false;
        }
    }
    return false;
}

bool Gold(const int *move_x, const int *move_y, int *Scol, int *Srow, int *Ecol, int *Erow, struct piece (*board)[BHEIGHT])
{
    // Blue
    if (strcmp((*(board + *Srow) + *Scol)->controller, "Blue") == 0)
    {
        if (strcmp((*(board + *Erow) + *Ecol)->controller, "Blue") != 0)
        {
            if (*move_x == -1 && *move_y == -1)
                return true;
            if (*move_x == 0 && *move_y == -1)
                return true;
            if (*move_x == 1 && *move_y == -1)
                return true;
            if (*move_x == -1 && *move_y == 0)
                return true;
            if (*move_x == 1 && *move_y == 0)
                return true;
            if (*move_x == 0 && *move_y == 1)
                return true;
            else
                return false;
        }
    }
    // Red
    if (strcmp((*(board + *Srow) + *Scol)->controller, "Red") == 0)
    {
        if (strcmp((*(board + *Erow) + *Ecol)->controller, "Red") != 0)
        {
            if (*move_x == -1 && *move_y == 1)
                return true;
            if (*move_x == 0 && *move_y == 1)
                return true;
            if (*move_x == 1 && *move_y == 1)
                return true;
            if (*move_x == -1 && *move_y == 0)
                return true;
            if (*move_x == 1 && *move_y == 0)
                return true;
            if (*move_x == 0 && *move_y == -1)
                return true;
            else
                return false;
        }
    }
    return false;
}

bool King(const int *move_x, const int *move_y, int *Scol, int *Srow, int *Ecol, int *Erow, struct piece (*board)[BHEIGHT])
{
    // Blue
    if (strcmp((*(board + *Srow) + *Scol)->controller, "Blue") == 0)
    {
        if (strcmp((*(board + *Erow) + *Ecol)->controller, "Blue") != 0)
        {
            if (*move_x == -1 && *move_y == -1)
                return true;
            if (*move_x == 0 && *move_y == -1)
                return true;
            if (*move_x == 1 && *move_y == -1)
                return true;
            if (*move_x == -1 && *move_y == 0)
                return true;
            if (*move_x == 1 && *move_y == 0)
                return true;
            if (*move_x == -1 && *move_y == 1)
                return true;
            if (*move_x == 0 && *move_y == 1)
                return true;
            if (*move_x == 1 && *move_y == 1)
                return true;
            else
                return false;
        }
    }
    // Red
    if (strcmp((*(board + *Srow) + *Scol)->controller, "Red") == 0)
    {
        if (strcmp((*(board + *Erow) + *Ecol)->controller, "Red") != 0)
        {
            if (*move_x == -1 && *move_y == 1)
                return true;
            if (*move_x == 0 && *move_y == 1)
                return true;
            if (*move_x == 1 && *move_y == 1)
                return true;
            if (*move_x == -1 && *move_y == 0)
                return true;
            if (*move_x == 1 && *move_y == 0)
                return true;
            if (*move_x == -1 && *move_y == -1)
                return true;
            if (*move_x == 0 && *move_y == -1)
                return true;
            if (*move_x == 1 && *move_y == -1)
                return true;
            else
                return false;
        }
    }
    return false;
}

bool Bishop(const int *move_x, const int *move_y, int *Scol, int *Srow, int *Ecol, int *Erow, struct piece (*board)[BHEIGHT])
{
    // Blue
    if (strcmp((*(board + *Srow) + *Scol)->controller, "Blue") == 0)
    {
        if (*move_x == *move_y) //左上往右下
        {
            if (*Ecol < *Scol) //左上
            {
                for (int i = -1; i >= *move_x; i--)
                {
                    if (strcmp((*(board + *Srow + i) + *Scol + i)->controller, "Blue") == 0)
                        return false;
                }
                return true;
            }
            if (*Ecol > *Scol) //右下
            {
                for (int i = 1; i <= *move_x; i++)
                {
                    if (strcmp((*(board + *Srow + i) + *Scol + i)->controller, "Blue") == 0)
                        return false;
                }
                return true;
            }
        }
        if (*move_x == (*move_y * -1)) //右上往左下
        {
            if (*Ecol < *Scol) //右上
            {
                for (int i = 1; i <= *move_x; i++)
                {
                    if (strcmp((*(board + *Srow + i) + *Scol - i)->controller, "Blue") == 0)
                        return false;
                }
                return true;
            }
            if (*Ecol > *Scol) //右下
            {
                for (int i = -1; i >= *move_x; i--)
                {
                    if (strcmp((*(board + *Srow + i) + *Scol - i)->controller, "Blue") == 0)
                        return false;
                }
                return true;
            }
        }
    }
    // Red
    if (strcmp((*(board + *Srow) + *Scol)->controller, "Red") == 0)
    {
        if (*move_x == *move_y) //左上往右下
        {
            if (*Ecol < *Scol) //左上
            {
                for (int i = -1; i >= *move_x; i--)
                {
                    if (strcmp((*(board + *Srow + i) + *Scol + i)->controller, "Red") == 0)
                        return false;
                }
                return true;
            }
            if (*Ecol > *Scol) //右下
            {
                for (int i = 1; i <= *move_x; i++)
                {
                    if (strcmp((*(board + *Srow + i) + *Scol + i)->controller, "Red") == 0)
                        return false;
                }
                return true;
            }
        }
        if (*move_x == (*move_y * -1)) //右上往左下
        {
            if (*Ecol < *Scol) //右上
            {
                for (int i = 1; i <= *move_x; i++)
                {
                    if (strcmp((*(board + *Srow + i) + *Scol - i)->controller, "Red") == 0)
                        return false;
                }
                return true;
            }
            if (*Ecol > *Scol) //右下
            {
                for (int i = -1; i >= *move_x; i--)
                {
                    if (strcmp((*(board + *Srow + i) + *Scol - i)->controller, "Red") == 0)
                        return false;
                }
                return true;
            }
        }
    }
    return false;
}

bool Rook(const int *move_x, const int *move_y, int *Scol, int *Srow, int *Ecol, int *Erow, struct piece (*board)[BHEIGHT])
{
    // Blue
    if (strcmp((*(board + *Srow) + *Scol)->controller, "Blue") == 0)
    {
        if (*move_x == 0) //直走
        {
            if (*move_y < 0) //上
            {
                for (int i = -1; i >= *move_y; i--)
                {
                    if (strcmp((*(board + *Srow) + *Scol + i)->controller, "Blue") == 0)
                        return false;
                }
                return true;
            }
            if (*move_y > 0) //下
            {
                for (int i = 1; i <= *move_y; i++)
                {
                    if (strcmp((*(board + *Srow) + *Scol + i)->controller, "Blue") == 0)
                        return false;
                }
                return true;
            }
        }
        if (*move_y == 0) //橫走
        {
            if (*move_x < 0) //左
            {
                for (int i = -1; i >= *move_x; i--)
                {
                    if (strcmp((*(board + *Srow + i) + *Scol)->controller, "Blue") == 0)
                        return false;
                }
                return true;
            }
            if (*move_x > 0) //右
            {
                for (int i = 1; i <= *move_x; i++)
                {
                    if (strcmp((*(board + *Srow + i) + *Scol)->controller, "Blue") == 0)
                        return false;
                }
                return true;
            }
        }
    }
    // Red
    if (strcmp((*(board + *Srow) + *Scol)->controller, "Red") == 0)
    {
        if (*move_x == 0) //直走
        {
            if (*move_y < 0) //上
            {
                for (int i = -1; i >= *move_y; i--)
                {
                    if (strcmp((*(board + *Srow) + *Scol + i)->controller, "Red") == 0)
                        return false;
                }
                return true;
            }
            if (*move_y > 0) //下
            {
                for (int i = 1; i <= *move_y; i++)
                {
                    if (strcmp((*(board + *Srow) + *Scol + i)->controller, "Red") == 0)
                        return false;
                }
                return true;
            }
        }
        if (*move_y == 0) //橫走
        {
            if (*move_x < 0) //左
            {
                for (int i = -1; i >= *move_x; i--)
                {
                    if (strcmp((*(board + *Srow + i) + *Scol)->controller, "Red") == 0)
                        return false;
                }
                return true;
            }
            if (*move_x > 0) //右
            {
                for (int i = 1; i <= *move_x; i++)
                {
                    if (strcmp((*(board + *Srow + i) + *Scol)->controller, "Red") == 0)
                        return false;
                }
                return true;
            }
        }
    }
    return false;
}

bool move_avaliable(bool *turn, const int *move_x, const int *move_y, int *Scol, int *Srow, int *Ecol, int *Erow, struct piece (*board)[BHEIGHT])
{
    if (strcmp((*(board + *Srow) + *Scol)->controller, "Free") == 0) // no piece
        return false;
    else if (strcmp((*(board + *Srow) + *Scol)->controller, (*(board + *Erow) + *Ecol)->controller) == 0) // Pieces of the same faction
        return false;
    else if ((strcmp((*(board + *Srow) + *Scol)->controller, "Red") == 0 && *turn == 0) || (strcmp((*(board + *Srow) + *Scol)->controller, "Blue") == 0 && *turn == 1)) // not your turn
        return false;
    else if (strcmp((*(board + *Srow) + *Scol)->name, "步") == 0)
        return pawn(move_x, move_y, Scol, Srow, Ecol, Erow, board);
    else if (strcmp((*(board + *Srow) + *Scol)->name, "香") == 0)
        return Lance(move_x, move_y, Scol, Srow, Ecol, Erow, board);
    else if (strcmp((*(board + *Srow) + *Scol)->name, "桂") == 0)
        return knight(move_x, move_y, Scol, Srow, Ecol, Erow, board);
    else if (strcmp((*(board + *Srow) + *Scol)->name, "銀") == 0)
        return Silver(move_x, move_y, Scol, Srow, Ecol, Erow, board);
    else if (strcmp((*(board + *Srow) + *Scol)->name, "金") == 0)
        return Gold(move_x, move_y, Scol, Srow, Ecol, Erow, board);
    else if (strcmp((*(board + *Srow) + *Scol)->name, "王") == 0)
        return King(move_x, move_y, Scol, Srow, Ecol, Erow, board);
    else if (strcmp((*(board + *Srow) + *Scol)->name, "角") == 0)
        return Bishop(move_x, move_y, Scol, Srow, Ecol, Erow, board);
    else if (strcmp((*(board + *Srow) + *Scol)->name, "飛") == 0)
        return Rook(move_x, move_y, Scol, Srow, Ecol, Erow, board);
    else
        return false;
}

void move(int *Scol, int *Srow, int *Ecol, int *Erow, struct piece (*board)[BHEIGHT], char *word)
{
    // Word
    strcpy(word, ""); // initial

    if (strcmp((*(board + *Srow) + *Scol)->name, "步") == 0)
        word[0] = 'p';
    else if (strcmp((*(board + *Srow) + *Scol)->name, "香") == 0)
        word[0] = 'l';
    else if (strcmp((*(board + *Srow) + *Scol)->name, "桂") == 0)
        word[0] = 'n';
    else if (strcmp((*(board + *Srow) + *Scol)->name, "銀") == 0)
        word[0] = 's';
    else if (strcmp((*(board + *Srow) + *Scol)->name, "金") == 0)
        word[0] = 'g';
    else if (strcmp((*(board + *Srow) + *Scol)->name, "王") == 0)
        word[0] = 'k';
    else if (strcmp((*(board + *Srow) + *Scol)->name, "角") == 0)
        word[0] = 'b';
    else if (strcmp((*(board + *Srow) + *Scol)->name, "飛") == 0)
        word[0] = 'r';
    else
        ;

    word[1] = *Scol + 1 + 48;
    word[2] = 9 - *Srow + 48;
    word[3] = *Ecol + 1 + 48;
    word[4] = 9 - *Erow + 48;

    // Update Board
    if (strcmp((*(board + *Srow) + *Scol)->controller, "Free") != 0 && strcmp((*(board + *Erow) + *Ecol)->controller, "Free") != 0 && strcmp((*(board + *Srow) + *Scol)->controller, (*(board + *Erow) + *Ecol)->controller) != 0)
    {
        if (strcmp((*(board + *Erow) + *Ecol)->name, "步") == 0)
            word[5] = 'p';
        else if (strcmp((*(board + *Erow) + *Ecol)->name, "香") == 0)
            word[5] = 'l';
        else if (strcmp((*(board + *Erow) + *Ecol)->name, "桂") == 0)
            word[5] = 'n';
        else if (strcmp((*(board + *Erow) + *Ecol)->name, "銀") == 0)
            word[5] = 's';
        else if (strcmp((*(board + *Erow) + *Ecol)->name, "金") == 0)
            word[5] = 'g';
        else if (strcmp((*(board + *Erow) + *Ecol)->name, "王") == 0)
            word[5] = 'k';
        else if (strcmp((*(board + *Erow) + *Ecol)->name, "角") == 0)
            word[5] = 'b';
        else if (strcmp((*(board + *Erow) + *Ecol)->name, "飛") == 0)
            word[5] = 'r';
        else
            ;
        word[6] = '\0';
        swap((*(board + *Srow) + *Scol), (*(board + *Erow) + *Ecol));
        strcpy((*(board + *Srow) + *Scol)->name, "  ");
        strcpy((*(board + *Srow) + *Scol)->controller, "Free");
    }
    else
    {
        word[5] = '\0';
        swap((*(board + *Srow) + *Scol), (*(board + *Erow) + *Ecol));
    }
}

void move_back(struct piece (*board)[BHEIGHT], char *word)
{
    int Scol = word[1] - 48, Srow = word[2] - 48, Ecol = word[3] - 48, Erow = word[4] - 48;
    Scol = Scol - 1;
    Ecol = Ecol - 1;
    Srow = 9 - Srow;
    Erow = 9 - Erow;

    if (word[5] == 'p' || word[5] == 'l' || word[5] == 'n' || word[5] == 's' || word[5] == 'g' || word[5] == 'k' || word[5] == 'b' || word[5] == 'r') // Eat
    {
        swap((*(board + Srow) + Scol), (*(board + Erow) + Ecol));

        if (word[5] == 'p')
            strcpy((*(board + Erow) + Ecol)->name, "步");
        else if (word[5] == 'l')
            strcpy((*(board + Erow) + Ecol)->name, "香");
        else if (word[5] == 'n')
            strcpy((*(board + Erow) + Ecol)->name, "桂");
        else if (word[5] == 's')
            strcpy((*(board + Erow) + Ecol)->name, "銀");
        else if (word[5] == 'g')
            strcpy((*(board + Erow) + Ecol)->name, "金");
        else if (word[5] == 'k')
            strcpy((*(board + Erow) + Ecol)->name, "王");
        else if (word[5] == 'b')
            strcpy((*(board + Erow) + Ecol)->name, "角");
        else if (word[5] == 'r')
            strcpy((*(board + Erow) + Ecol)->name, "飛");
        else
            strcpy((*(board + Srow) + Scol)->name, "錯");

        if (strcmp((*(board + Srow) + Scol)->controller, "Blue") == 0)
            strcpy((*(board + Erow) + Ecol)->controller, "Red");
        else
            strcpy((*(board + Erow) + Ecol)->controller, "Blue");
    }
    else
        swap((*(board + Erow) + Ecol), (*(board + Srow) + Scol));
}

/*.......record stack........*/
bool empty(NODE *stack)
{
    if (stack == NULL)
        return true;
    else
        return false;
}

// bool overflow(struct stack *s)
// {
//     if (s->top == STACKSIZE - 1)
//         return true;
//     else
//         return false;
// }

NODE *pop(NODE *stack, char *w)
{
    if (stack == NULL)
        return stack;
    NODE *tmp;
    tmp = stack;
    stack = stack->next;
    strcpy(w, tmp->word);
    free(tmp);
    return stack;
}

NODE *push(NODE *stack, char *w)
{
    NODE *tmp;
    tmp = (NODE *)malloc(sizeof(NODE));
    strcpy(tmp->word, w);
    tmp->next = stack;
    stack = tmp;
    return stack;
}

NODE *inverse(NODE *stack)
{
    NODE *front, *fnext;
    front = NULL;
    while (stack != NULL)
    {
        fnext = front;
        front = stack;
        stack = stack->next;
        front->next = fnext;
    }
    return front;
}

void printlist(NODE *stack)
{
    NODE *tmp;
    tmp = stack;
    printf("stack");
    while (tmp != NULL)
    {
        printf(" -> %s", tmp->word);
        tmp = tmp->next;
    }
    printf(" -> NULL\n");
    free(tmp);
}

/*.............load file.............*/
void GetSeek(FILE *fptr, int *line) //指針回到上一行
{
    char str[DATESIZE];
    int i = 1;

    fseek(fptr, SEEK_SET, 0); //確保指針在開頭

    while (!(*line == i))
    {
        fgets(str, DATESIZE, fptr);
    }
    *line--;
}

/*...............timer...............*/
int turns;
int now;
int c = 0;
int indepx = 0, indepy = 0;
ev_io stdin_watcher;
ev_timer timeout_watcherx;
ev_timer timeout_watchery;

static void stdin_cb(EV_P_ ev_io *w, int revents)
{
    ev_timer_stop(loop, &timeout_watcherx);
    ev_timer_stop(loop, &timeout_watchery);
    if (turns % 2)
    {
        ev_timer_stop(loop, &timeout_watcherx);
        ev_timer_start(loop, &timeout_watchery);
    }
    else
    {
        ev_timer_stop(loop, &timeout_watchery);
        ev_timer_start(loop, &timeout_watcherx);
    }
}
static void timeout_cbx(EV_P_ ev_timer *w, int revents)
{
    if (turns == 0)
    {
        printf("\033[34m藍方\033[m這回合用時: ");
        printf("%d 秒\n", (int)ev_now(loop) - now); // first time
        indepx = (int)ev_now(loop) - now;
        printf("\033[34m藍方\033[m累計用時: %d 秒\n\n", indepx);
    }
    else
    {
        printf("\033[34m藍方\033[m這回合用時: ");
        printf("%d 秒\n", (int)ev_now(loop) - now - indepx);
        indepx = (int)ev_now(loop) - now;
        // printf("cby\n");
        printf("\033[34m藍方\033[m累計用時: %d 秒\n\n", indepx);
    }
    // printf("cbx\n");
    ev_timer_stop(loop, &timeout_watcherx);
    ev_break(loop, EVBREAK_ONE);
    // printf("InCBX,turns++\n\n\n");
    turns += 1;
}
static void timeout_cby(EV_P_ ev_timer *w, int revents)
{
    printf("\033[31m紅方\033[m這回合用時: ");
    printf("%d 秒\n", (int)ev_now(loop) - now - indepy);
    indepy = (int)ev_now(loop) - now;
    printf("\033[31m紅方\033[m累計用時: %d 秒\n\n", indepy);
    // printf("cby\n");
    ev_timer_stop(loop, &timeout_watchery);
    ev_break(loop, EVBREAK_ONE);
    // printf("InCBY,turns++\n\n\n");
    turns += 1;
}

/*.................................................main.......................................*/
int main(int argc, char *argv[]) // int argc, char *argv[]
{
    /*.........variable declaration & initialization.............*/
    struct piece board[BWEIGHT][BHEIGHT];
    board_initial(board);
    NODE *stack = NULL;
    char word[STRLENGTH] = "";

    bool turn = 0;
    char alpha = ' ';
    int input_Scol = 0, input_Srow = 0, input_Ecol = 0, input_Erow = 0;
    int tr_Scol = 0, tr_Srow = 0, tr_Ecol = 0, tr_Erow = 0;
    int move_x = 0, move_y = 0;

    FILE *fptr;

    /*........................choose mode...........................*/
    enum _mode
    {
        NEW_GAME,
        LOAD_FILE,
        MODE_ERROR
    } mode;

    if (argc == 4 && strcmp(argv[1], "-n") == 0 && strcmp(argv[2], "-s") == 0)
    {
        mode = NEW_GAME;
        fptr = fopen(argv[3], "w");

        if (fptr == NULL)
        {
            printf("Error!");
            exit(1);
        }

        time_t timer;
        struct tm *Now;

        time(&timer);
        Now = localtime(&timer);
        fprintf(fptr, "Date: %s\n", asctime(Now));

        // printf("%d\n", record.top);
        board_show(board);
        // board_show_controller(board);
    }
    else if (argc == 3 && strcmp(argv[1], "-1") == 0)
    {
        mode = LOAD_FILE;
        printf("檔案載入中...\n");
        if ((fptr = fopen(argv[2], "r")) == NULL)
        {
            printf("Error! opening file");

            // Program exits if the file pointer returns NULL.
            exit(1);
        }

        char date[DATESIZE];
        fgets(date, DATESIZE, fptr);
        fgets(word, STRLENGTH, fptr);
        while (fgets(word, STRLENGTH, fptr) != NULL)
        {
            stack = push(stack, word);
        }
        stack = inverse(stack);
        printlist(stack);
        board_show(board);
    }
    else
    {
        mode = MODE_ERROR;
        printf("模式輸入錯誤，請重新執行!\n");
    }

    /*........................Play Game...........................*/
    /*...........timer..........*/
    turns = 0;
    struct ev_loop *loop = EV_DEFAULT;
    now = ev_now(loop);
    int x, y, z;
    ev_io_init(&stdin_watcher, stdin_cb, /*STDIN_FILENO*/ 0, EV_READ);
    ev_io_start(loop, &stdin_watcher);
    ev_timer_init(&timeout_watcherx, timeout_cbx, 0, 1);
    ev_timer_init(&timeout_watchery, timeout_cby, 0, 1);

    while (mode == NEW_GAME)
    {
        input_movement(&turn, &alpha, &input_Srow, &input_Ecol, &input_Erow);
        if (alpha >= '1' && alpha <= '9') // 1~9
            input_Scol = alpha - 48;      // atoi

        bool in_ava = false;

        if (input_Scol >= 1 && input_Scol <= BHEIGHT && input_Srow >= 1 && input_Srow <= BWEIGHT && input_Ecol >= 1 && input_Ecol <= BHEIGHT && input_Erow >= 1 && input_Erow <= BWEIGHT)
            in_ava = true;
        else
            in_ava = false;

        transfer(&input_Scol, &input_Srow, &input_Ecol, &input_Erow, &tr_Scol, &tr_Srow, &tr_Ecol, &tr_Erow, &move_x, &move_y);

        if (alpha == '0') // regret
        {
            if (stack->next != NULL && stack != NULL)
            {
                stack = pop(stack, word);
                move_back(board, word);
                stack = pop(stack, word);
                move_back(board, word);
                // printlist(stack);
                turn ? printf("\n\033[31m紅方悔棋\033[m\n\n") : printf("\n\033[34m藍方悔棋\033[m\n\n");
                board_show(board);
            }
            else
                printf("\n悔棋無效\n");
            // printf("%d\n",record.top);
        }
        else if (move_avaliable(&turn, &move_x, &move_y, &tr_Scol, &tr_Srow, &tr_Ecol, &tr_Erow, board) && in_ava)
        {
            turn = !(turn);
            move(&tr_Scol, &tr_Srow, &tr_Ecol, &tr_Erow, board, word);
            stack = push(stack, word);
            printf("\n%s從(%d,%d)移動到(%d,%d)\n\n", board[tr_Erow][tr_Ecol].name, input_Scol, input_Srow, input_Ecol, input_Erow);
            printlist(stack);

            // timer
            // printf("turns: %d\n",turns+1);
            if (turns % 2 == 0)
            {
                // printf("input: ");
                // scanf(" %d",&x);
                ev_timer_start(loop, &timeout_watcherx);
                printf("第 %d 回合\n", turns / 2 + 1);
            }
            else
            {
                // printf("input:");
                // scanf(" %d",&x);
                ev_timer_start(loop, &timeout_watchery);
                printf("第 %d 回合\n", turns / 2 + 1);
            }
            ev_run(loop, 0);

            // printf("Outside!\n");
            ev_timer_stop(loop, &timeout_watcherx);
            ev_timer_stop(loop, &timeout_watchery);
            board_show(board);
        }
        else if (alpha == 's' || alpha == 'S')
        {
            printf("存檔中...\n");
            stack = inverse(stack);
            printlist(stack);
            NODE *tmp;
            tmp = stack;
            while (tmp != NULL)
            {
                fprintf(fptr, "%s\n", tmp->word);
                tmp = tmp->next;
            }
            printf("存檔完成\n");
            stack = inverse(stack);
            printlist(stack);
        }
        else if (alpha == 'e' || alpha == 'E')
        {
            printf("謝謝遊玩\n");
            break;
        }
        else
            printf("\n輸入錯誤，請重新輸入!\n");
        // board_show_controller(board);
    }

    char forb = ' ';
    NODE *fload;
    fload = stack;
    while (mode == LOAD_FILE)
    {
        printf("輸入 f 移動下一手，輸入 b 退回上一手，輸入 e 離開:");
        scanf("%s", &forb);
        if (forb == 'f' || forb == 'F')
        {
            if (fload == NULL)
            {
                printf("棋譜結束，沒有下一手了!\n");
            }
            else
            {
                input_Scol = fload->word[1] - 48;
                input_Srow = fload->word[2] - 48;
                input_Ecol = fload->word[3] - 48;
                input_Erow = fload->word[4] - 48;
                transfer(&input_Scol, &input_Srow, &input_Ecol, &input_Erow, &tr_Scol, &tr_Srow, &tr_Ecol, &tr_Erow, &move_x, &move_y);
                turn = !(turn);
                move(&tr_Scol, &tr_Srow, &tr_Ecol, &tr_Erow, board, word);
                printf("\n%s從(%d,%d)移動到(%d,%d)\n\n", board[tr_Erow][tr_Ecol].name, input_Scol, input_Srow, input_Ecol, input_Erow);
                fload = fload->next;
                board_show(board);
            }
        }
        else if (forb == 'b' || forb == 'B')
        {
            if (!empty(stack))
            {
                NODE *tmp;
                tmp = stack;
                while (tmp ->next != fload) // O(n)
                {
                    tmp = tmp->next;
                }
                fload = tmp;                
                strcpy(word,fload->word);
                move_back(board, word);
                printf("\n回到上一步\n\n");
                board_show(board);
                // board_show_controller(board);
            }
            else
                printf("\n輸入錯誤，請重新輸入!\n");
        }
        else if (forb == 'e' || forb == 'E')
        {
            printf("謝謝遊玩\n");
            break;
        }
        else
            printf("\n輸入錯誤，請重新輸入!\n");
    }

    fclose(fptr);

    return 0;
}
