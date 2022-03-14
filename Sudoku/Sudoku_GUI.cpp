#define OLC_PGE_APPLICATION

//#include "olcPixelGameEngine.h"
#include "olcPixelGameEngine Win.h"
#include <algorithm>
#include <limits.h>


using namespace std;

#define W 1000
#define H 750
#define SCALE 1
#define RESET_SPEED 3
#define POZIOM_TRUDNOSCI 8

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

enum WINDOWS {
    TITLE_SCREEN,
    PLAYER_CHOICE,
    GAME
};

// =^w^=
int board[6][7] { 0 };
int current_window = TITLE_SCREEN;
int comp_move = -1;
// https://stackoverflow.com/questions/740308/how-do-i-modify-this-easing-function-to-bounce-less#740350
// Jak się chce zmienić bounce 
float EaseBounceOut(float t, float d){
    t /= d;
    if (t < 1.4 / 2.75) {
        return 3.858419 * t * t;
    }
    else if (t < 2.1 / 2.75) {
        t -= 1.75f / 2.75;
        return 7.5625 * t * t + 0.8775f;
    }
    else if (t < 2.5 / 2.75) {
        t -= 2.3f / 2.75;
        return 7.5625 * t * t + 0.96f;
    }
    t -= 2.625f / 2.75;
    return 7.5625 * t * t + 0.984375f;    
}

float lerp(float start, float end, float pct){
    return (start + (end - start) * pct);
}

bool check_draw() {
    for(int i = 0; i < 7; i++)
        if(board[0][i] == 0) return false; 
    return true;
} 


bool win_move(const int p) {
    int n = 0;
    
    for(int i = 0; i < 6; i++){ // Poziomo
        for(int j = 0; j < 7;j++ ){
            if(board[i][j] == p)      { n++;if(n == 4) return true;}
            else n = 0;
        }   
        n = 0;
    }
    
    for(int i = 0; i < 7; i++){ // Pionowo
        for(int j = 0; j < 6;j++ ){
            if(board[j][i] == p)      { n++;if(n == 4) return true;}
            else n = 0;
        }
        n = 0;
    }
    
    
     // ---------------- DIAGONALS ------------------
    for(int i = 3; i < 7; i++){
        int y = 0;
        for(int x = i; x >= 0; x--){
            if(board[y][x] == p)      { n++; if(n == 4) return true;}
            else n = 0;
            y++;
            if(y == 6) break;            
        }
        n = 0;
    }
    for(int i = 1; i < 3; i++){
        int y = i;
        for(int x = 6; x >= 0; x--){
            if(board[y][x] == p)      { n++; if(n == 4) return true;}
            else n = 0;
            y++;
            if(y == 6) break;  
        }
        n = 0;
    }
    // =========================================================
    
    // -------------------- To też ----------------------
    for(int i = 3; i >= 0; i--){
        int y = 0;
        for(int x = i; x < 7; x++){
            if(board[y][x] == p)      { n++; if(n == 4) return true;}
            else n = 0;
            y++;
            if(y == 6) break;  
        }
        n = 0;
    }    
    for(int i = 1; i < 3; i++){
        int y = i;
        for(int x = 0; x < 7; x++){
            if(board[y][x] == p)      { n++; if(n == 4) return true;}
            else n = 0;
            y++;
            if(y == 6) break;  
        }
        n = 0;
    }
    // ==========================================================
    
    return false;
}

// -------------------------------------- AI sprawy --------------------------------------

int heurFunction(unsigned int g, unsigned int b, unsigned int z) {
	int score = 0;
	if (g == 4) { score += 500001; } // preference to go for winning move vs. block
	else if (g == 3 && z == 1) { score += 5000; }
	else if (g == 2 && z == 2) { score += 500; }
	else if (b == 2 && z == 2) { score -= 501; }  // preference to block
	else if (b == 3 && z == 1) { score -= 5001; } // preference to block
	else if (b == 4) { score -= 500000; }
	return score;
}

bool insert_token(int x, int player){
    for (int i = 5; i >= 0; i--) {
        if (board[i][x] == 0) {
            board[i][x] = player;
            return true;
        }
    }
    return false;
}
void remove_token(int x) {
    for (int i = 0; i < 6; i++) {
        if (board[i][x] != 0){ board[i][x] = 0; return;}
    }
}

int scoreSet(int v[], unsigned int p) {
	
	unsigned int good = 0;    // points in favor of p
	unsigned int bad = 0;    // points against p
	unsigned int empty = 0; // neutral spots
	
	for (int i = 0; i < 4; i++) { // just enumerate how many of each
		good  += (v[i] == p) ? 1 : 0;
		bad   += (v[i] == 1 || v[i] == 2) ? 1 : 0;
		empty += (v[i] == 0) ? 1 : 0;
	}

	bad -= good;
	return heurFunction(good, bad, empty);
}

int tabScore(int b[6][7], const int p) {
	int score = 0;
	
	int rs[7] = {0};
	int cs[6] = {0};
	int set[4] = {0};
	
	for (unsigned int r = 0; r < 6; r++) {
		for (unsigned int c = 0; c < 7; c++) {
			rs[c] = b[r][c]; // this is a distinct row alone
		}
		for (unsigned int c = 0; c < 7 - 3; c++) {
			for (int i = 0; i < 4; i++) {
				set[i] = rs[c + i]; // for each possible "set" of 4 spots from that row
			}
			score += scoreSet(set, p); // find score
		}
	}
	// vertical
	for (unsigned int c = 0; c < 7; c++) {
		for (unsigned int r = 0; r < 6; r++) {
			cs[r] = b[r][c];
		}
		for (unsigned int r = 0; r < 6 - 3; r++) {
			for (int i = 0; i < 4; i++) {
				set[i] = cs[r + i];
			}
			score += scoreSet(set, p);
		}
	}
	// diagonals
	for (unsigned int r = 0; r < 6 - 3; r++) {
		for (unsigned int c = 0; c < 7; c++) {
			rs[c] = b[r][c];
		}
		for (unsigned int c = 0; c < 7 - 3; c++) {
			for (int i = 0; i < 4; i++) {
				set[i] = b[r + i][c + i];
			}
			score += scoreSet(set, p);
		}
	}
	for (unsigned int r = 0; r < 6 - 3; r++) {
		for (unsigned int c = 0; c < 7; c++) {
			rs[c] = b[r][c];
		}
		for (unsigned int c = 0; c < 7 - 3; c++) {
			for (int i = 0; i < 4; i++) {
				set[i] = b[r + 3 - i][c + i];
			}
			score += scoreSet(set, p);
		}
	}
	return score;
}

int minmax(int b[6][7], int depth , int alpha , int beta , bool computer){
    if (depth == 0 || check_draw()) {
		// get current score to return
		return tabScore(b, 2);
	}
	
	if (computer) { // if AI player
		int maxVal = INT_MIN;
		if (win_move(1))   // if player about to win
			return maxVal; // force it to say it's worst possible score, so it knows to avoid move

		for (int i = 0; i < 7; i++) { // for each possible move
			if (b[0][i] == 0) { // but only if that column is non-full
                insert_token(i,2);
				int score = minmax(b, depth - 1, alpha, beta, false); // find move based on that new board state
				remove_token(i);
				if (score > maxVal) { // if better score, replace it, and consider that best move (for now)
					maxVal = score;
					if(depth == POZIOM_TRUDNOSCI) comp_move = i;
				}
				alpha = MAX(alpha, maxVal);
				if (alpha >= beta) break;// for pruning
			}
		}
		return maxVal; // return best possible move
	}
	else {
		int minVal = INT_MAX; // since PLAYER is minimized, we want moves that diminish this score
		if (win_move(2)) {
			return minVal; // if about to win, report that move as best
		}
		for (int i = 0; i < 7; i++) {
			if (b[0][i] == 0) {
				insert_token(i,1);
				int score = minmax(b, depth - 1, alpha, beta, true);
				if (score < minVal) {
					minVal = score;
					//comp_move = i;
				}
				remove_token(i);
				beta = MIN(beta, minVal);
				if (alpha >= beta) break;
			}
		}
		return minVal;
	}	
}

// -------------------------------------------------


bool animate_token(float time, float start_pos, float end_pos, float durration, float& token_pos){
    if(time > durration) return true;
    token_pos = (EaseBounceOut(time,durration) * end_pos) + start_pos;
    return false;
}

class Game : public olc::PixelGameEngine
{
public:
    uint32_t front_layer;
    uint32_t back_layer;
	
	bool front_layer_lock = true;
	bool can_insert       = true;
	bool animating        = false;
    bool game_w_computer   = false;
    
	int game_over = 0;
    int p_x,p_y,player;
	int score = 0;
	
	float curr_time = 0;
	float token_falling;
    float selection_x = 0;
    
    olc::Sprite* title_data = nullptr;
    olc::Decal* title = nullptr;

    olc::Sprite* pc_clipart_data = nullptr;
    olc::Decal* pc_clipart = nullptr;

    olc::Sprite* pady_clipart_data = nullptr;
    olc::Decal* pady_clipart = nullptr;
    
	Game()
	{
		player = 1;
		sAppName = "Connect Four/Czworki";
	}
    
    bool check_token(int x){
        int ind = 9;
        for (int i = 5; i >= 0; i--) {
            if (board[i][x] == 0) {
                ind = i;
                break;
            }
        }
        if(ind == 9) return false;
        p_y = ind;
        return true;
    }
    
    int get_user_input() {
        int mouse_x = GetMouseX();
        if(mouse_x < 160 || mouse_x > 800) return -1;
        
        selection_x = lerp(selection_x, ((mouse_x-160) / 92) * 92 + 160, 0.13);
		FillRect(selection_x,165,92,610, olc::Pixel(255, 255, 255,80));
        
        if(GetMouse(0).bPressed) return ((mouse_x-160) / 92);
        else return -1;
    }
    
    void draw_board() { // Rysuje puste pole dzięki któremu mamy otwory za którymi są żetony
        const int r = 40;
        FillRect(160,180,644,565,olc::BLUE);
        
        for(int j = 0; j < 6; j++){
            for(int i = 0;i < 7;i++)
                FillCircle(i * 92 + 205,j * 92 + 225,r,olc::BLANK);
        }
    }
    
    void draw_tokens(){ // Rysuje te żetony
        const int r = 33;
        for(int j = 0; j < 6; j++){
            for(int i = 0;i < 7;i++)
            {
                if(board[j][i] == 2)      {FillCircle(i * 92 + 205,j * 92 + 225,r+7, olc::DARK_YELLOW);FillCircle(i * 92 + 205,j * 92 + 225,r, olc::YELLOW);}
                else if(board[j][i] == 1) {FillCircle(i * 92 + 205,j * 92 + 225,r+7, olc::DARK_RED);FillCircle(i * 92 + 205,j * 92 + 225,r, olc::RED);}
                else                          continue;
            }
        }
    }   
     
    void Reset(){
        game_over = 0;
        curr_time = 0;
        can_insert = true;
        animating = false;
        front_layer_lock = true;
        for(int i = 0; i < 6; i++)
            for(int j = 0; j < 7; j++)
                board[i][j] = 0;
                
        player = player == 1 ? 2 : 1;
        current_window = PLAYER_CHOICE;
    }

public:
	bool OnUserCreate() override {
        title_data = new olc::Sprite("Title.png");
        title = new olc::Decal(title_data);

        pc_clipart_data = new olc::Sprite("computer.png");
        pc_clipart  = new olc::Decal(pc_clipart_data);
        
        pady_clipart_data = new olc::Sprite("pady.png");
        pady_clipart = new olc::Decal(pady_clipart_data);

		front_layer = CreateLayer();
		EnableLayer(front_layer,true);
		
        back_layer = CreateLayer();
		EnableLayer(back_layer,true);
		return true;
	}

	bool OnUserUpdate(float delta) override {
        int mouse_x = GetMouseX();
        switch (current_window) {
        
        case TITLE_SCREEN:
            Clear(olc::BLACK);
            DrawDecal(olc::vi2d(0,0),title);
            if (AnyInput() || GetMouse(0).bPressed) current_window = PLAYER_CHOICE;
            break;
            
        case PLAYER_CHOICE:
            Clear(olc::BLACK);
            
            DrawString(250, 70, "Wybor trybu gry",olc::WHITE,4);
            

                                    
            DrawDecal(olc::vi2d(150, 320), pady_clipart);
            DrawDecal(olc::vi2d(580, 320), pc_clipart,olc::vf2d(0.6,0.6));
            
            
            if(mouse_x < 50 || mouse_x > 950) break;
            selection_x = lerp(selection_x, ((mouse_x-50) / 450) * 450 + 50, 0.07);
            FillRect(selection_x,200,450,500, olc::Pixel(120, 120, 120));
            
            DrawString(120,640,"Gracz vs Gracz",olc::WHITE,3);
            DrawString(600,640,"Gracz vs AI",olc::WHITE,3);
            
            if(GetMouse(0).bPressed){
                game_w_computer = ((mouse_x-50) / 450);
                current_window = GAME;
            }
            
            break;

        case GAME:
            // Instant Win
            if(GetKey(olc::TAB).bReleased) game_over = 1;
        
            if(front_layer_lock){
                SetDrawTarget(front_layer);
                Clear(olc::BLANK);
                draw_board();
                front_layer_lock = false;
            }      
        
            SetDrawTarget(back_layer);
            Clear(olc::BLACK);
            draw_tokens();
            if(animating){ // trwa animacja spadania tokenu
                if(animate_token(curr_time,130,p_y * 92 + 95, p_y > 1 ? 1.4 : 1.2, token_falling)){ // Jeżeli skończył animacje to wstawić token i nie animować
                    board[p_y][p_x] = player;
                    animating = false;
                    curr_time = 0;
                
                    game_over = win_move(player) + check_draw();
                    if(game_over) goto skip_game_over;
                
                    can_insert = true;
                    if(player == 1) player = 2;
                    else            player = 1;
                }
                else{
                    FillCircle(p_x * 92 + 205,token_falling,40, player == 1 ? olc::DARK_RED : olc::DARK_YELLOW);
                    FillCircle(p_x * 92 + 205,token_falling,33, player == 1 ? olc::RED : olc::YELLOW);
                    curr_time += delta;
                }
		    }
		
		    skip_game_over:
            SetDrawTarget(nullptr);
		    Clear(olc::BLANK);
            
		    if(!game_over){ // Póki nikt nie wygrał to bierzemy kolumne i wstawiamy token jak się zgadza
                
                if (player == 2 && !animating && game_w_computer) {
                    can_insert = false;
                    animating = true;
                    minmax(board, POZIOM_TRUDNOSCI, 0 - INT_MAX, INT_MAX, true);
                    p_x = comp_move;
                    check_token(p_x);
                    goto skip_input;
                }
                if(can_insert) p_x = get_user_input();
            
            skip_input:
                char player_string[8];
                sprintf(player_string,"Gracz %i",player);
                
                DrawString(W/2 - 220,H/2 - 320, string(player_string), player == 1 ? olc::RED : olc::YELLOW,7);
            
                if(p_x != -1 && can_insert){ // Insert the token and change Player because he choosed the column
                    if(check_token(p_x)){
                        animating = true;
                        can_insert = false;
                    }
                }
		    }
		    else{ // Po wygranej, piszemy kto wygrał
                char game_over_title[20];
                if(game_over == 1)
                    sprintf(game_over_title,"Wygrana!!\nGracz %i",player);
                else
                    sprintf(game_over_title,"Remis ://");
                DrawString(W/2 - 220,H/2 - 320, string(game_over_title), player == 1 ? olc::RED : olc::YELLOW,7);
            
                olc::vi2d text_collide = GetTextSize("RESET")*4;
                olc::Pixel text_col = olc::WHITE; 
            
                olc::vi2d mouse = GetMousePos();
                if(mouse.x > 810 && mouse.x < 810 + text_collide.x + 30){
                    if(mouse.y > sin(curr_time * RESET_SPEED) * 30 + 490 && mouse.y < sin(curr_time * RESET_SPEED) * 30 + 490 + text_collide.y+20){
                        text_col = olc::GREY;
                        if(GetMouse(0).bHeld){
                            text_col = olc::DARK_RED;
                            Reset();
                            Clear(olc::BLACK);
                        }
                    }
                }
            
                DrawString(820,sin(curr_time * RESET_SPEED) * 30 + 500,"RESET",text_col,4);
                curr_time += delta;
		    }
        }
		return true;
	}
};

int main()
{
	Game game;
	if (game.Construct(W, H, SCALE, SCALE))
		game.Start();
	return 0;
}

