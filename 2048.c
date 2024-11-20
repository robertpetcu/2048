#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>

//am creat functia sa adauge random numere in casutele goale
void addRandom(int grid[4][4]) {
    int gol[16][2];
    int k = 0, i, j;

    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            if (grid[i][j] == 0) {
                gol[k][0] = i;
                gol[k][1] = j;
                k++;
            }
        }
    }
//daca gaseste casute goale adauga 2 sau 4
    if (k > 0) {
        int randNumber = rand() % k;
        int x = gol[randNumber][0];
        int y = gol[randNumber][1];
        int randomValue = rand() % 10; 
        if (randomValue < 5) {
            grid[x][y] = 2; 
}           else {
            grid[x][y] = 4; 
}

    }
}
// functia pentru meniu 
void meniu(int selectie){
    clear(); 

//obtin dimensiunea terminalului
    int screen_height, screen_width;
    getmaxyx(stdscr, screen_height, screen_width);
    
    start_color();
    init_pair(1, COLOR_BLUE, COLOR_BLACK); 
    init_pair(2, COLOR_BLACK, COLOR_RED); 

//cel mai desen
    const char *asciiArt[] = {
        "  .-----.   .----.      .---.    .-----.   ",
        " / ,-.   \\ /  ..  \\    / .  |   /  .-.  \\  ",
        "'-'  |  |.  /  \\  .  / /|  |  |   \\_.' /  ",
        "   .'  / |  |  '  | / / |  |_  /  .-. '.  ",
        " .'  /__ '  \\  /  '/  '-'    ||  |   |  | ",
        "|       | \\  `'  / `----|  |-' \\  '-'  /  ",
        "`-------'  `---''       `--'    `----''    "
    };

//de unde sa inceapa 
    int artWidth = strlen(asciiArt[0]);
    int artStartX = (screen_width - artWidth) / 2;
    int artStartY = (screen_height - 10) / 2; 
    int artLine;

//afiseaza desenul
    for(artLine = 0; artLine < 7; artLine++) {
        mvprintw(artStartY + artLine, artStartX, "%s", asciiArt[artLine]);
    }

//afiseaza meniul
    attron(COLOR_PAIR(1) | A_BOLD);
    mvprintw(artStartY + 8, (screen_width - strlen("Meniu Principal")) / 2, "Meniu Principal");
    attroff(COLOR_PAIR(1) | A_BOLD);

//declar optiunile
    const char *optiuni[3] = {"New Game", "Resume", "Quit"};
    int i;
    int longestOptionLength = 0;
    for (i = 0; i < 3; ++i) {
        if (strlen(optiuni[i]) > longestOptionLength) {
            longestOptionLength = strlen(optiuni[i]);
        }
    }
    int menuStartX = (screen_width - (longestOptionLength + 3)) / 2;

//alegerea optiunilor
    for (i = 0; i < 3; ++i) {
        if (i == selectie) {
            attron(COLOR_PAIR(2));
            mvprintw(artStartY + 10 + i, menuStartX, "-> %s", optiuni[i]);
        } else {
            attron(COLOR_PAIR(1));
            mvprintw(artStartY + 10 + i, menuStartX, "   %s", optiuni[i]);
        }
        attroff(COLOR_PAIR(1) | COLOR_PAIR(2));
    }

    refresh(); 
}


//functia pentru tabel
void drawGrid(WINDOW *joc, int grid[4][4]) {
    werase(joc);
    int box_width = 40, box_height = 20;
    wbkgd(joc, COLOR_PAIR(4));
    box(joc, 0, 0);

    int i, j, color;
    int startx, starty;
//aici am creat patratele din tabel 
    int tile_width = box_width / 4, tile_height = box_height / 4;
//culorile pentru fiecare numar 
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            starty = i * tile_height + 1;
            startx = j * tile_width + 1;
            if (grid[i][j] == 0) {
                color = 4; 
            } else {
                switch (grid[i][j]) {
                    default:   color = 1;
                    case 2:    color = 2; break;
                    case 4:    color = 3; break;
                    case 8:    color = 5; break;
                    case 16:   color = 6; break;
                    case 32:   color = 7; break;
                    case 64:   color = 8; break;
                    case 128:  color = 10;break;
                    case 256:  color = 11;break;
                    case 512:  color = 12;break;
                }
            }
            wattron(joc, COLOR_PAIR(color));
            int x, y;
            for (y = starty; y < starty + tile_height - 1; y++) {
                for (x = startx; x < startx + tile_width - 1; x++) {
                    mvwaddch(joc, y, x, ' ');
                }
            }
            if (grid[i][j] != 0) {
                mvwprintw(joc, starty + tile_height / 2 - 1, startx + (tile_width - 4) / 2, "%4d", grid[i][j]);
            }
            wattroff(joc, COLOR_PAIR(color));
        }
    }

    for (i = 1; i < 4; i++) {
        mvwhline(joc, i * tile_height, 1, ACS_HLINE, box_width - 2);
        mvwvline(joc, 1, i * tile_width, ACS_VLINE, box_height - 2);
    }

    wrefresh(joc);
}
//aici am citit highscore ul in file
int readHighScore() {
    int highScore;
    FILE *file = fopen("highscore.txt", "r");
    if (file == NULL) {
        return 0; 
    }
    fscanf(file, "%d", &highScore);
    fclose(file);
    return highScore;

}
//functie sa scrie highscore ul 
void writeHighScore(int highScore, const char* playerName) {
    FILE *file = fopen("highscore.txt", "w");
    if (file != NULL) {
        fprintf(file, "%d %s\n", highScore, playerName);
        fclose(file);
    }
}
//functie pentru afisarea lui
void drawHighScore(WINDOW *hsWindow, int highScore) {
    werase(hsWindow);
    box(hsWindow, 0, 0);
    mvwprintw(hsWindow, 1, 1, "Highscore: %d", highScore);
    wrefresh(hsWindow);
}
//daca exista numarul 2048 in casuta am castigat
bool checkWin(int grid[4][4]) {
    int i, j;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            if (grid[i][j] == 2048) {
                return true; 
            }
        }
    }
    return false; 
}
//daca nu mai exista mutari jocul se termina
bool checkGameOver(int grid[4][4]){
    int i,j;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
//daca exista casute goale jocul continua
            if (grid[i][j] == 0) {
                return false; 
            }
//daca exista casute cu numere egale langa jocul poate continua si check ramane false
            if (i < 3 && grid[i][j] == grid[i + 1][j]) {
                return false; 
            }
            if (j < 3 && grid[i][j] == grid[i][j + 1]) {
                return false; 
            }
        }
    }
    return true; 
}
//functie pentru miscarea spre dreapta , am folosit bool pentru a verifica daca e si valida
bool moveRight(int grid[4][4], int *score) {
    int i, j;
    bool moved = false;
    for (i = 0; i < 4; i++) {
        for (j = 2; j >= 0; j--) {
            if (grid[i][j] != 0) {
                int k = j;
//daca casuta goala langa atunci casuta devine numarul din stanga si numarul devine 0 
                while (k < 3 && grid[i][k + 1] == 0) {
                    grid[i][k + 1] = grid[i][k];
                    grid[i][k] = 0;
                    k++;
                    moved = true;
                }
//daca exista numar egal in dreapta atunci  ele se combina spre dreapta si casuta originala devine 0
                if (k < 3 && grid[i][k + 1] == grid[i][k]) {
                    *score += grid[i][k + 1] * 2;
                    grid[i][k + 1] *= 2;
                    grid[i][k] = 0;
                    moved = true;
                }
            }
        }
    }
    return moved;
}

//ca si la dreapta
bool moveUp(int grid[4][4], int *score) {
    bool moved = false;
    int i, j;
    for (j = 0; j < 4; j++) {
        for (i = 1; i < 4; i++) {
            if (grid[i][j] != 0) {
                int k = i;
                while (k > 0 && grid[k - 1][j] == 0) {
                    grid[k - 1][j] = grid[k][j];
                    grid[k][j] = 0;
                    k--;
                    moved = true;
                }
                if (k > 0 && grid[k - 1][j] == grid[k][j]) {
                    *score += grid[k][j] * 2; 
                    grid[k - 1][j] *= 2;
                    grid[k][j] = 0;
                    moved = true;
                }
            }
        }
    }
    return moved;
}


bool moveLeft(int grid[4][4], int *score) {
    bool moved = false;
    int i, j, k;
    for (i = 0; i < 4; i++) {
        for (j = 1; j < 4; j++) {
            if (grid[i][j] != 0) {
                    k = j;
                while (k > 0 && grid[i][k - 1] == 0) {
                    grid[i][k - 1] = grid[i][k];
                    grid[i][k] = 0;
                    k--;
                    moved = true;
                }
                if (k > 0 && grid[i][k - 1] == grid[i][k]) {
                    *score += grid[i][k - 1] * 2;
                    grid[i][k - 1] *= 2;
                    grid[i][k] = 0;
                    moved = true;
                }
            }
        }
    }
    return moved;
}



bool moveDown(int grid[4][4], int *score) {
    bool moved = false;
    int i, j ,k;
    for (j = 0; j < 4; j++) {
        for (i = 2; i >= 0; i--) {
            if (grid[i][j] != 0) {
                    k = i;
                while (k < 3 && grid[k + 1][j] == 0) {
                    grid[k + 1][j] = grid[k][j];
                    grid[k][j] = 0;
                    k++;
                    moved = true;
                }
                if (k < 3 && grid[k + 1][j] == grid[k][j]) {
                    *score += grid[k + 1][j] * 2;
                    grid[k + 1][j] *= 2;
                    grid[k][j] = 0;
                    moved = true;
                }
            }
        }
    }
    return moved;}
//functia pentru newgame
void newgame() {
//aloc highscore 
    int highScore = readHighScore();
    cbreak();          
    noecho();         
    keypad(stdscr, TRUE); 
    start_color();
    int grid[4][4] = {{0}};
    addRandom(grid);
    addRandom(grid);
    
//window pentru score si timp ul de cand a inceput jocul
    WINDOW *infoBox;
    int infoBoxWidth = 40, infoBoxHeight = 3; 
    infoBox = newwin(infoBoxHeight, infoBoxWidth, 0, 0); 
    box(infoBox, 0, 0);
    
//window ul jocului respectiv
    WINDOW *joc;
    int startx, starty, width, height;
    height = 20;
    width = 40;
    int screen_height, screen_width;
    getmaxyx(stdscr, screen_height, screen_width);
    starty = (screen_height - height) / 2;
    startx = (screen_width - width) / 2;
    joc = newwin(height, width, starty, startx);
   
//window ul pentru data si ora
    WINDOW *dataOraBox;
    int dataOraBoxWidth = 20, dataOraBoxHeight = 3; 
    int dataOraPosX = screen_width - dataOraBoxWidth, dataOraPosY = 0; 
    dataOraBox = newwin(dataOraBoxHeight, dataOraBoxWidth, dataOraPosY, dataOraPosX);
    box(dataOraBox, 0, 0);
    
    
    int WinWidth = 20, WinHeight = 3;
    int WinPos1 = 0, WinPos2 = (screen_height - WinHeight) / 2;
    WINDOW *Window = newwin(WinHeight, WinWidth, WinPos2, WinPos1);
    box(Window, 0, 0);;
    drawHighScore(Window, highScore);
//folosesc bool pentru a tine while ul functional pana cand se pierde sau se castiga
    keypad(joc, TRUE);
    wtimeout(joc, 100);
    bool updated = true;
    bool gameWon = false;
    bool gameOver = false;
    int score = 0; 
    time_t startTime = time(NULL);
     int titleColor = 5;

    while (!gameOver && !gameWon) {
   
    if (updated) {
        clear();
        refresh();
        drawGrid(joc, grid);
        updated = false;
    }
//afisam scorul si timpul  
    time_t currentTime = time(NULL);
    int Seconds = difftime(currentTime, startTime);
    werase(infoBox);
    box(infoBox, 0, 0);
    mvwprintw(infoBox, 1, 1, "Timp: %d secunde, Scor: %d", Seconds, score);
    wrefresh(infoBox);

     

    titleColor = (titleColor - 12) % 3 + 13; 

    refresh();
    drawGrid(joc, grid);
//afisam timpul curent
    time_t timpCurent = time(NULL);
    struct tm *timpLocal = localtime(&timpCurent);
    char bufferDataOra[80];
    strftime(bufferDataOra, 80, "%H:%M %d/%m/%Y", timpLocal);
    werase(dataOraBox);
    box(dataOraBox, 0, 0);
     mvwprintw(dataOraBox, 1, 1, "%s", bufferDataOra);
    wrefresh(dataOraBox);

//afisam highscore ul
    drawHighScore(Window, highScore);

//inregistram tastele de la utilizator si le folosim sa utilizam jocul
    int ch = wgetch(joc);
    switch (ch) {
        case KEY_UP:
        case 'w':
            updated = moveUp(grid, &score);
            break;
        case KEY_DOWN:
        case 's':
            updated = moveDown(grid, &score);
            break;
        case KEY_LEFT:
        case 'a':
            updated = moveLeft(grid, &score);
            break;
        case KEY_RIGHT:
        case 'd':
            updated = moveRight(grid ,&score);
            break;
        case 'q':
            delwin(joc);
            return; 
    }

    if (updated) {
        addRandom(grid);
        gameWon = checkWin(grid);
        gameOver = checkGameOver(grid);
    }
}
//window pentru mesaj de pierdere, daca gameOver este true , apare box now
WINDOW *mesajPierdere;
if (gameOver) {
    int mesajWidth = 40, mesajHeight = 5;
    mesajPierdere = newwin(mesajHeight, mesajWidth, (screen_height - mesajHeight) / 2, (screen_width - mesajWidth) / 2);
    box(mesajPierdere, 0, 0);
    mvwprintw(mesajPierdere, 2, (mesajWidth - strlen("Ai pierdut! Apasa 'q' pentru a iesi.")) / 2, "Ai pierdut! Apasa 'q' pentru a iesi.");
    wrefresh(mesajPierdere);
    while ((wgetch(mesajPierdere)) != 'q') {

    }
    delwin(mesajPierdere); 
//daca score ul este mai mare decat highscore ul atunci ele se schimba si iti poti scrie numele in box
    if (score > highScore) {
        highScore = score; 
        int nameWidth = 30, nameHeight = 3;
        int namePos1 = 0, namePos2 = screen_height - nameHeight;
        WINDOW *nameWin = newwin(nameHeight, nameWidth, namePos2, namePos1);
        box(nameWin, 0, 0);
        mvwprintw(nameWin, 1, 1, "Introdu numele: ");
        wrefresh(nameWin);
        char playerName[100];
        echo(); 
        wgetnstr(nameWin, playerName, 99); 
        noecho(); 
        writeHighScore(highScore, playerName);
        delwin(nameWin); 
    }
}

delwin(joc);
}


int main() {
//initializam culorile folosite la numere si altele
   
    initscr();
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_YELLOW, COLOR_YELLOW);
    init_pair(3, COLOR_GREEN, COLOR_GREEN);
    init_pair(4, COLOR_BLACK, COLOR_WHITE); 
    init_pair(5, COLOR_RED, COLOR_MAGENTA);
    init_pair(6, COLOR_BLACK, COLOR_CYAN);
    init_color(8, 196, 30, 99);
    init_pair(7, COLOR_BLACK, 8 );
    init_color(9, 25, 13, 112);
    init_pair(8, COLOR_BLACK, 9);
    init_pair(9 ,COLOR_WHITE, COLOR_BLUE);
    init_color(10, 3, 255, 0);
    init_pair(10,COLOR_BLACK , 10);
    init_color(11 , 228, 105, 15);
    init_pair(11, COLOR_BLACK, 11);
    init_color(12, 174, 255, 255);
    init_pair(12, COLOR_BLACK , 12);
//folosim tastele sa manevram meniul 
    srand(time(NULL)); 
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    int selectie = 0;
    bool quit = false;
    while (!quit) {
        meniu(selectie);
        int ch = getch();
        switch (ch) {
//%3 pentru a ramane intre 0 si 3
            case KEY_UP:
                selectie = (selectie + 2) % 3;
                break;
            case KEY_DOWN:
                selectie = (selectie + 1) % 3;
                break;
            case 10: 
                if (selectie == 0) {
                    newgame(); 
                } else if (selectie == 2) { 
                    quit = true; 
                }
                break;
        }
    }
    endwin(); 
    return 0;
}