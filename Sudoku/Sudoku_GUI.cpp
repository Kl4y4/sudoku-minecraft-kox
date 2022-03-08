#define OLC_PGE_APPLICATION

#include "olcPixelGameEngine.h"
#include <algorithm>

/*
#if defined(__linux__)
    
#else
    #include "olcPixelGameEngine Win.h"
#endif
*/

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

bool check_draw() {
    for(int i = 0; i < 7; i++)
        if(board[0][i] == 0) return false; 
    return true;
} 

// -------------------------------------- AI sprawy --------------------------------------


// Jakiś sposób żeby pokazać że wygrana
int eval_board(int const player){
    int eval_scores[5] = {
        0, 
        0,
        1,  // powtarza sie 2 razy
        5,  // powtarza sie 3 razy
        170 // powtarza sie 4 razy
    };
    
    int n1 = 0,n2 = 0, k = player, score = 0;

    for(int i = 0; i < 6; i++){
        for(int j = 0; j < 7;j++ ){
            if(board[i][j] == k)      { n1++; score -= eval_scores[n2]; n2 = 0;}
            else if(board[i][j] != 0) { n2++; score += eval_scores[n1]; n1 = 0;}
            else                      { score += eval_scores[n1] - eval_scores[n2] ;n1 = 0; n2 = 0;}
        }
        if(!n1 || !n2) score += eval_scores[n1] - eval_scores[n2];     
        n1 = 0;
        n2 = 0;
    }
    
    for(int i = 0; i < 7; i++){
        for(int j = 0; j < 6;j++ ){
            if(board[j][i] == k)      { n1++; score -= eval_scores[n2]; n2 = 0;}
            else if(board[j][i] != 0) { n2++; score += eval_scores[n1]; n1 = 0;}
            else                      { score += eval_scores[n1] - eval_scores[n2] ;n1 = 0; n2 = 0;}
        }
        if(!n1 || !n2) score += eval_scores[n1] - eval_scores[n2];   
        n1 = 0;
        n2 = 0;
    }
    
    
    
    for(int i = 3; i < 7; i++){
        int y = 0;
        for(int x = i; x >= 0; x--){
            if(board[y][x] == k)      { n1++; score -= eval_scores[n2]; n2 = 0;}
            else if(board[y][x] != 0) { n2++; score += eval_scores[n1]; n1 = 0;}
            else                      { score += eval_scores[n1] - eval_scores[n2] ;n1 = 0; n2 = 0;}
            y++;
            if(y == 6) break;            
        }
        if(!n1 || !n2) score += eval_scores[n1] - eval_scores[n2];  
        n1 = 0;
        n2 = 0;
    }
    for(int i = 1; i < 3; i++){
        int y = i;
        for(int x = 6; x >= 0; x--){
            if(board[y][x] == k)      { n1++; score -= eval_scores[n2]; n2 = 0;}
            else if(board[y][x] != 0) { n2++; score += eval_scores[n1]; n1 = 0;}
            else                      { score += eval_scores[n1] - eval_scores[n2] ;n1 = 0; n2 = 0;}
            y++;
            if(y == 6) break; 
        }
        if(!n1 || !n2) score += eval_scores[n1] - eval_scores[n2];
        n1 = 0;
        n2 = 0;
    }
    
    

    for(int i = 3; i >= 0; i--){
        int y = 0;
        for(int x = i; x < 7; x++){
            if(board[y][x] == k)      { n1++; score -= eval_scores[n2]; n2 = 0;}
            else if(board[y][x] != 0) { n2++; score += eval_scores[n1]; n1 = 0;}
            else                      { score += eval_scores[n1] - eval_scores[n2] ;n1 = 0; n2 = 0;}
            y++;
            if(y == 6) break; 
        }
        if(!n1 || !n2) score += eval_scores[n1] - eval_scores[n2];
        n1 = 0;
        n2 = 0;
    }    
    for(int i = 1; i < 3; i++){
        int y = i;
        for(int x = 0; x < 7; x++){
            if(board[y][x] == k)      { n1++; score -= eval_scores[n2]; n2 = 0;}
            else if(board[y][x] != 0) { n2++; score += eval_scores[n1]; n1 = 0;}
            else                      { score += eval_scores[n1] - eval_scores[n2] ;n1 = 0; n2 = 0;}
            y++;
            if(y == 6) break; 
        }
        if(!n1 || !n2) score += eval_scores[n1] - eval_scores[n2];
        n1 = 0;
        n2 = 0;
    }
    
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
    for (int i = 5; i >= 0; i--) {
        if (board[i][x] == 0) board[i+1][x] = 0;
    }
}

const int computer_t = 2;
const int player_t = 1;
int comp_move = 0;

int minmax(int b[6][7], int depth, int alpha, int beta, bool maximizingPlayer) {
    int eval = eval_board(computer_t);
    if (depth == 0 || check_draw() || eval > 100) // Koniec sprawdzanias
        return eval;
    if (maximizingPlayer) {
        int maxEval = -99999;
        for (int i = 0; i < 7; i++) {
            if (insert_token(i, computer_t)) {
                eval = minmax(b, depth - 1, alpha, beta, false);
                remove_token(i);
                if (eval > maxEval) {
                    maxEval = eval;
                    comp_move = i;
                }
                alpha = MAX(alpha, eval);
                if (beta <= alpha) break;
            }
        }
        return maxEval;
    }
    else {
        int minEval = 99999;
        for (int i = 0; i < 7; i++) {
            if (insert_token(i, player_t)) {
                eval = minmax(b, depth - 1, alpha, beta, true);
                remove_token(i);
                minEval = MIN(minEval, eval);
                beta = MIN(beta, eval);
                if (beta <= alpha) break;
            }
        }
        return minEval;
    }
}

// -------------------------------------------------

bool win_move(int x,int y) {
    int n = 0, k = board[y][x];
    //TODO skreślanie
    // pionowo
    for(int i = y-3; i <= y+3; i++){
        if(i >= 6 || i < 0) continue;
        if(board[i][x] == k) n++;   // jeżeli się powtarza to 1
        else                 n = 0; // jeżeli jakaś inna to wyzerować
        if(n == 4) return true;
    }
    
    // poziomo
    n = 0;
    for(int i = x-3; i <= x+3; i++){
        if(i >= 7 || i < 0) continue;
        if(board[y][i] == k) n++;   // jeżeli się powtarza to 1
        else                 n = 0; // jeżeli jakaś inna to wyzerować
        if(n == 4) return true;
    }
    
    n = 0;
    for(int i = -3; i <= 3; i++){
        if((x+i >= 7 || x+i < 0) || (y+i >= 6 || y+i < 0)) continue;
        if(board[y+i][x+i] == k) n++;
        else                     n = 0;
        if(n == 4) return true;
    }

    n = 0;
    for(int i = 3; i >= -3; i--){
        if((x+i >= 7 || x+i < 0) || (y-i >= 6 || y-i < 0)) continue;
        if(board[y-i][x+i] == k) n++;
        else                     n = 0;
        if(n == 4) return true;
    }
    
    return false;
}

bool animate_token(float time, float start_pos, float end_pos, float durration, float& token_pos){
    if(time > durration) return true;
    token_pos = (EaseBounceOut(time,durration) * end_pos) + start_pos;
    return false;
}

class Game : public olc::PixelGameEngine
{
public:
    int p_x,p_y,player;
    
    uint32_t front_layer;
    uint32_t back_layer;
	
	bool front_layer_lock = true;
	bool can_insert = true;
	bool animating = false;
	bool reset = false;
    
	int game_over = 0;
	
	float curr_time = 0;
	float token_falling;
	
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
    
    int get_user_input(int& y,int player) {
        int mouse_x = GetMouseX();
        if(mouse_x < 160 || mouse_x > 800) return -1;
        
		FillRect(((mouse_x-160) / 92) * 92 + 160,80,92,670, olc::Pixel(255, 255, 255,80));
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
                if(board[j][i] == 2)      {FillCircle(i * 92 + 205,j * 92 + 225,r+7, olc::BLACK);FillCircle(i * 92 + 205,j * 92 + 225,r, olc::YELLOW);}
                else if(board[j][i] == 1) {FillCircle(i * 92 + 205,j * 92 + 225,r+7, olc::BLACK);FillCircle(i * 92 + 205,j * 92 + 225,r, olc::RED);}
                else                          continue;
            }
        }
    }   
     
    void Reset(){
        game_over = 0;
        curr_time = 0;
        can_insert = true;
        animating = false;
        reset = false;
        for(int i = 0; i < 6; i++)
            for(int j = 0; j < 7; j++)
                board[i][j] = 0;
                
        player = rand() % 2 + 1;
    }
public:
	bool OnUserCreate() override {
		front_layer = CreateLayer();
		EnableLayer(front_layer,true);
		
        back_layer = CreateLayer();
		EnableLayer(back_layer,true);
		return true;
	}

	bool OnUserUpdate(float delta) override {
        // Instant Win
        if(GetKey(olc::TAB).bReleased) game_over = 1;
        
        if(front_layer_lock){
            SetDrawTarget(front_layer);
            Clear(olc::BLANK);
            draw_board();
            front_layer_lock = false;
        }      
        
        SetDrawTarget(back_layer);
        Clear(olc::DARK_CYAN);
        draw_tokens();
        if(animating){ // trwa animacja spadania tokenu
            if(animate_token(curr_time,130,p_y * 92 + 95, p_y > 1 ? 2.0 : 1.2, token_falling)){ // Jeżeli skończył animacje to wstawić token i nie animować
                board[p_y][p_x] = player;
                animating = false;
                curr_time = 0;
                
                game_over = win_move(p_x,p_y) + check_draw();
                if(game_over) goto skip_game_over;
                
                can_insert = true;
                if(player == 1) player = 2;
                else            player = 1;
            }
            else{
                FillCircle(p_x * 92 + 205,token_falling,40, olc::BLACK);
                FillCircle(p_x * 92 + 205,token_falling,33, player == 1 ? olc::RED : olc::YELLOW);
                curr_time += delta;
            }
		}
		
		skip_game_over:
        SetDrawTarget(nullptr);
		Clear(olc::BLANK);
		
		if(!game_over){ // Póki nikt nie wygrał to bierzemy kolumne i wstawiamy token jak się zgadza
            if (player == 2 && !animating) {
                can_insert = false;
                animating = true;
                minmax(board, POZIOM_TRUDNOSCI, -9999, 9999, true);
                p_x = comp_move;
                check_token(p_x);
                goto skip_input;
            }
            if(can_insert) p_x = get_user_input(p_y,player);
            
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
                    }
                }
            }
            
            DrawString(820,sin(curr_time * RESET_SPEED) * 30 + 500,"RESET",text_col,4);
            curr_time += delta;
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

