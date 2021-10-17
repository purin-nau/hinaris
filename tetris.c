#include <rand.h>
#include "tetris.h"
#include "anime_hina.h"

/*ブロックの横位置*/
UBYTE block_x;

/*ブロックの縦位置*/
UBYTE block_y;

// ステージ・フィールド定義
UBYTE block[BLOCK_HEIGHT][BLOCK_WIDTH];
UBYTE stage[FIELD_HEIGHT][FIELD_WIDTH];
UBYTE field[FIELD_HEIGHT][FIELD_WIDTH];

//回転後のブロック
UBYTE turn_block[BLOCK_HEIGHT][BLOCK_WIDTH];

// 壁とブロックの当たり判定フラグ
UBYTE collision_flag;

// ブロックを生成するかどうかのフラグ
UBYTE make_block_flag;

//ゲームオーバーしたかどうか
UBYTE gameover_flag;

//ブロックの識別用
UBYTE block_id;

//回転状態
UBYTE turn_point;

//キーを押したかどうかのフラグ
UBYTE input_trg_flag;

//ラインが消えたかどうか
UBYTE clear_count;

//消すべきライン行を格納
UBYTE clear_line_point[20];

//消すかどうかフラグ
UBYTE clear_flag;

//点数表示用
UBYTE ScoreLine;

// ブロック□ 定義
UBYTE blocks[BLOCK_HEIGHT * 7][BLOCK_WIDTH * 4] = {
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0},
	{0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},

	{0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0},
	{0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0},
	{0,0,1,1,0,1,0,0,0,0,1,1,0,1,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},

	{0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0},
	{0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0},
	{1,1,0,0,0,0,1,0,1,1,0,0,0,0,1,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},

	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,1,0,0,0,0,1,1,0,0,0,0,0,0,1,0},
	{0,1,1,1,0,0,1,0,0,1,1,1,0,0,1,0},
	{0,0,0,0,0,0,1,0,0,0,0,1,0,1,1,0},

	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,1,1,1,0,1,1,0,0,0,1,0,0,1,0,0},
	{0,1,0,0,0,0,1,0,1,1,1,0,0,1,0,0},
	{0,0,0,0,0,0,1,0,0,0,0,0,0,1,1,0},

	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,1,0,0,0,0,0,0,0,1,0,0,0,1,0,0},
	{0,1,1,0,1,1,1,0,1,1,0,0,1,1,1,0},
	{0,1,0,0,0,1,0,0,0,1,0,0,0,0,0,0},

	{0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0},
	{0,1,0,0,1,1,1,1,0,1,0,0,1,1,1,1},
	{0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0},
	{0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0}
};


// ステージ初期化
void my_init_var(){
	
	//ループ用変数
	UBYTE i,j;

	//壁となるところに9を代入
	for(i=0;i<FIELD_HEIGHT;i++){
		for(j=0;j<FIELD_WIDTH;j++){
			stage[i][0] = 9;
			stage[i][1] = 9;
			stage[i][2] = 9;
			stage[17][j] = 9;
			stage[18][j] = 9;
			stage[19][j] = 9;
			stage[i][13] = 9;
			stage[i][14] = 9;
			stage[i][15] = 9;		
		}
	}

	//ブロックの初期位置
	block_x = 7;
	block_y = 0;

	//ブロック固定フラグ
	collision_flag = 0;
	//ゲームオーバーフラグ
	gameover_flag = 0;
	//??
	make_block_flag = 1;
	//ブロック識別フラグ
	block_id = 0;
	//ラインが消えたかフラグ初期化
	clear_count = 0;
	//回転フラグ初期化
	turn_point = 0;
	//キー入力したかどうかフラグを初期化
	input_trg_flag = 0;
}

//ブロックの座標を戻す
void my_init_var2(){
	block_x = 7;
	block_y = 0;
	make_block_flag = 1;
	turn_point = 0;
	input_trg_flag = 0;
}
//　ブロックの登録
void my_make_block(){

	//ループ用変数
	UBYTE x,y;

	if(make_block_flag == 1){
		block_id = (rand() % 7);
		for(y=0;y<BLOCK_HEIGHT;y++){
			for(x=0;x<BLOCK_WIDTH;x++){
				block[y][x] = blocks[(block_id * BLOCK_HEIGHT) + y][x];
			}
		}
		make_block_flag = 0;
	}	


}
// フィールドへステージとブロックの重ね合わせて代入
void my_make_field(){

	// ループ用変数
	UBYTE x,y;

	//
	UBYTE fld_tmp = 0U;
	
	// フィールドへステージの値を代入
	memcpy(field, stage, sizeof(stage));
	
	// フィールドへブロックの初期位置からブロックを足したものを代入
	for(y=0;y<BLOCK_HEIGHT;y++){
		for(x=0;x<BLOCK_WIDTH;x++){
			fld_tmp = field[block_y + y][block_x + x];
			field[block_y + y][block_x + x] = fld_tmp + block[y][x];		
		}
	}	
}
//「block」を「stage」に保存する
void my_save_block(){
	UBYTE x,y;
	UBYTE stage_tmp = 0U;
	for(y=0;y<BLOCK_HEIGHT;y++){
		for(x=0;x<BLOCK_WIDTH;x++){
			stage_tmp = stage[block_y + y][block_x + x];
		//	stage[block_y + y][block_x + x] += block[y][x];
			stage[block_y + y][block_x + x]  = stage_tmp + block[y][x];
		}
	}
}

// 描画処理
void my_draw_field(){

	// ループ用変数
	UBYTE i,j,k,l;

	k = 0U;

//H20W16
	for(i=0;i<FIELD_HEIGHT;i++){
		for(j=0;j<FIELD_WIDTH;j++){
			if(field[i][j] == 1){
				k++;
				set_sprite_tile( k, 1); //スプライトk番にタイル1番を設定
				move_sprite( k, j*8+8-16, i*8+16); //ブロックの描画テトリス配列の値に(8,16)を加えてGBの表示に合わせる
			}
		}
	}
}

//フィールドクリア処理
void my_clear_field(){
	UBYTE i,j;
	memset(field, 0U, sizeof(field)); 
}

//ブロック落下処理
void my_fall_block(UBYTE cnt){
	if(make_block_flag == 0 && cnt == block_y){
		block_y++;
	}
}

//当たり判定処理左側
void my_collision_left(){
	UBYTE x,y;
	
	collision_flag = 0;

	for(y=0;y<BLOCK_HEIGHT;y++){
		for(x=0;x<BLOCK_WIDTH;x++){
			if(block[y][x] != 0){
				if(stage[block_y + y][block_x + (x - 1)] != 0){
					collision_flag = 1;
				}
			}
		}
	}
}

//当たり判定処理右側
void my_collision_right(){
	UBYTE x,y;

	collision_flag = 0;

	for(y=0;y<BLOCK_HEIGHT;y++){
		for(x=0;x<BLOCK_WIDTH;x++){
			if(block[y][x] != 0){
				if(stage[block_y + y][block_x + (x + 1)] != 0){
					collision_flag = 1;
				}
			}
		}
	}
}

//当たり判定処理底
void my_collision_bottom(){
	UBYTE x,y;

	collision_flag = 0;

	for(y=0;y<BLOCK_HEIGHT;y++){
		for(x=0;x<BLOCK_WIDTH;x++){
			if(block[y][x] != 0){
				if(stage[block_y + (y + 1)][block_x + x] != 0){
					collision_flag = 1;
					
				}
			}
		}
	}
	
}

//一番上までブロックが到達した判定
void my_collision_center(void){
	UBYTE x,y;

	collision_flag = 0;

	for(y=0;y<BLOCK_HEIGHT;y++){
		for(x=0;x<BLOCK_WIDTH;x++){
			if(block[y][x] != 0){
				if(stage[block_y + y][block_x + x] != 0){
					collision_flag = 1;
				}
			}
		}
	}
}

//ゲームオーバー処理
void my_gameover(void){
	my_collision_center();

	if(collision_flag != 0){
		gameover_flag = 1;
	}
}

//消すラインの探索
void my_search_line(){

	//ループ用変数
	UBYTE i,j;
	UBYTE lc = 0U;
	//全要素を０に初期化
	/*for(i=0;i<FIELD_HEIGHT - 3;i++){
		clear_line_point[i] = 0;
	}*/
	memset(clear_line_point, 0U, sizeof(clear_line_point)); 

	//全探査し、空白があれば１を立ててループから抜ける
	//つまり、すべてブロックなら０のまま
	for(i=0;i<FIELD_HEIGHT - 3;i++){
		for(j=3;j<FIELD_WIDTH - 3;j++){
			if(stage[i][j] == 0){
				clear_line_point[i] = 1;
				break;
			}
		}
	}
	//０ならクリアフラグを１にする
	for(i=0;i<FIELD_HEIGHT - 3;i++){
		if(clear_line_point[i] == 0){
			clear_flag = 1;
			ScoreLine++;
			lc++;
			//hina_state++;
		//	break;
		}
	}
	if(lc == 4){
		hina_state++;
	}
}

//ライン削除
void my_clear_line(){
	UBYTE i,j;
	UBYTE remain_line_point[20] = {0};
	UBYTE remain_line_index = 0;


	if(clear_count < 2){
		for(i=0;i<FIELD_HEIGHT-3;i++){
			if(clear_line_point[i] == 0){
				for(j=3;j<FIELD_WIDTH-3;j++){
					stage[i][j] = 0;
				}
			}
		}
		clear_count++;
	}
	else{
		//ここの処理でエラー。多分BYTE型で扱える領域を超えている。マイナス符号は扱えない。
		//for(i=0;i<FIELD_HEIGHT;i++){
		//for(j=0;j<FIELD_WIDTH;j++){
		for(i=FIELD_HEIGHT-4U;i>0;i--){
			//すべてブロックなら０だが、ブロックがあるとき→すべてブロックではない行
			if(clear_line_point[i] != 0){	//その行番号をremain_line_pointに格納
				remain_line_point[remain_line_index] = i;
				remain_line_index++;
				//	stage[i][j] = stage[i][j+1];
			}
		}
		
		remain_line_index = 0;
		for(i=FIELD_HEIGHT-4U;i>0;i--){
			for(j=3;j<FIELD_WIDTH-3;j++){
				stage[i][j] = stage[remain_line_point[remain_line_index]][j];
			}
			remain_line_index++;
		}
		clear_flag = 0;
		clear_count = 0;
		my_init_var2();
	}

}

//
void my_make_field2(){
	UBYTE i,j;

	// フィールドへステージの値を代入
	memcpy(field, stage, sizeof(stage));

	sndreq_se(0U);
		for(i=0;i<FIELD_HEIGHT;i++){
			for(j=0;j<FIELD_WIDTH;j++){
				if(field[i][j] == 0){
					gotoxy(j-2,i);
					setchar(5);
					stage[i][j] = 0U;
				}
				if(field[i][j] == 2){
					gotoxy(j-2,i);
					setchar(1);
				}
				if(field[i][j] == 1){
					gotoxy(j-2,i);
					setchar(1);
					stage[i][j] = 1U;
				}
			}
		}

}

//回転処理
void my_collision_turn(){
	UBYTE x,y;

	collision_flag = 0;

	for(y=0;y<BLOCK_HEIGHT;y++){
		for(x=0;x<BLOCK_WIDTH;x++){
			if(turn_block[y][x] != 0){
				if(stage[block_y + y][block_x + x] != 0){
					collision_flag = 1;
				}
			}
		}
	}
}

//右回転処理
void my_turn_right(){
	UBYTE x,y;

	turn_point++;
	
	for(y=0;y<BLOCK_HEIGHT;y++){
		for(x=0;x<BLOCK_WIDTH;x++){
			turn_block[y][x] = blocks[(block_id * BLOCK_HEIGHT) + y][(turn_point % 4 * BLOCK_WIDTH) + x];
		}
	}

	my_collision_turn();

	if(collision_flag == 0){
		for(y=0;y<BLOCK_HEIGHT;y++){
			for(x=0;x<BLOCK_WIDTH;x++){
				block[y][x] = turn_block[y][x];
			}
		}
	}
	else{
		turn_point--;
	}
}

//ブロック固定処理
void my_fix_block(){

	my_collision_bottom();

	//固定されたなら
	if(collision_flag != 0){



		// ループ用変数
		UBYTE i,j;

		for(i=0;i<FIELD_HEIGHT;i++){
			for(j=0;j<FIELD_WIDTH;j++){
				if(field[i][j] == 1){
					gotoxy(j-2,i);
					setchar(1);
					stage[i][j] = 1U;
				}
			}
		}

		my_save_block();
		my_search_line();
		
		sndreq_se(1U);//接地したら音ならす
		
		if(clear_flag == 0){
			my_init_var2();
		}



	}
}
