#include <stdio.h>
#include <gb.h>
#include <stdlib.h>
#include <rand.h>

// サウンドドライバ
#include "snd_drv.c"

// サウンドデータ
#include "snd_data.c"

// タイルデータ
#include "testtile.c"//フィールド・ブロック・リス
#include "hina_risu.C"//タイトル

// マップデータ
#include "Filed.c"//テトリス画面
#include "hina_risu_map.C"//タイトル

// 処理インクルード
#include "tetris.h"//テトリス処理
#include "anime_hina.h"//アニメーション処理

//ゲームの状態
#define GAMEMODE_INIT    0U
#define GAMEMODE_TITLE   2U
#define GAMEMODE_PLAY    3U
#define GAMEMODE_END     4U

/*ブロックの落下速度(設定値分のフレーム経過でブロックが1マス落下*/
#define FALLING_FRAME 20U

//pauseフラグ
UBYTE pause_flag;

//落下変数
UBYTE CountFrm;

// 乱数シード
fixed seed;

//サウンド変数
UBYTE CountSnd;

// キー入力用
UBYTE input_on, input_old, input_edge, input_trg;

//
UBYTE block_cnt;

//リスのアニメーション中かどうかのフラグ
UBYTE squirrel_flag;

//リスの場所
UBYTE squirrel_point;

//ゲームの状態
UBYTE game_mode;

// ファースト（初回）フラグ
UBYTE first;


// BGMリクエスト
void sndreq_bgm(UBYTE req_bgm)
{
//	input_trg = joypad();// キー入力
if (CountSnd == 1U) {
	// BGM再生
	snd_req_bgm(snd_bgm_adr[req_bgm][0],snd_bgm_adr[req_bgm][1],snd_bgm_adr[req_bgm][2],snd_bgm_adr[req_bgm][3]);
CountSnd = 0U;	
}
 // BGM流れる
 snd_driver();
}

// S.E.リクエスト
void sndreq_se(UBYTE req_se)
{
	// SE再生
	snd_req_se(snd_se_adr[req_se], snd_se_atr[req_se][0], snd_se_atr[req_se][1]);
	 // BGM流れる
 	snd_driver();
}

// ステージ初期化
/*
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
*/
//ブロックの座標を戻す
/*
void my_init_var2(){
	block_x = 7;
	block_y = 0;
	make_block_flag = 1;
	turn_point = 0;
	input_trg_flag = 0;
}
*/
//　ブロックの登録
/*
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
*/
//アイコン表示
/*
void view_hina(){
	
	UBYTE i,j,k;

	//スプライト番号7～23がヒナ1
	for(i = 0U;i < 4U; i++){
		for(j = 0U;j < 4U; j++){
			set_sprite_tile( j+6U+(i*4U), j+6U+(i*4U)); //6Uは初期表示
			move_sprite( j+6U+i*4U, j*8U+136U, i*8U+40); //hinaを表示に合わせる
		}
	}

}*/

//ヒナのアイコンをセット
/*
void set_hina(UBYTE num)
{
	switch(num){
		case 0:
			set_sprite_data(6,16, HinaTiles01);//ヒナ（通常）
			gotoxy(12,3);
			printf(" ");
			break;
		case 1:
			set_sprite_data(6,16, HinaTiles02);//ヒナ（つぼみ）
			gotoxy(12,3);
			printf(" ");
			sndreq_se(3U);//効果音設定
			break;
		case 2:
			set_sprite_data(6,16, HinaTiles03);//ヒナ（花）
			sndreq_se(3U);//効果音設定
			gotoxy(12,3);
		//	printf("B");
			break;
	}
}*/

// フィールドへステージとブロックの重ね合わせて代入
/*
void my_make_field(){

	// ループ用変数
	UBYTE i,j,x,y;

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
*/
//「block」を「stage」に保存する
/*
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
}*/


/*// 描画処理
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
*/

/*//フィールドクリア処理
void my_clear_field(){
	UBYTE i,j;
	memset(field, 0U, sizeof(field)); 
}
*/

//ブロック落下処理
/*
void my_fall_block(UBYTE cnt){
	if(make_block_flag == 0 && cnt == block_y){
		block_y++;
	}
}
*/

//当たり判定処理左側
/*
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
*/
//当たり判定処理右側
/*
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
*/
//当たり判定処理底
/*
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
	
}*/
//一番上までブロックが到達した判定
/*
void my_collision_center(){
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
*/
//ゲームオーバー処理
/*
void my_gameover(){
	my_collision_center();

	if(collision_flag != 0){
		gameover_flag = 1;
	}
}
*/

//消すラインの探索
/*
void my_search_line(){

	//ループ用変数
	UBYTE i,j;
	UBYTE lc = 0U;
	//全要素を０に初期化
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
}*/

//ライン削除
/*
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
*/
//
/*
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
*/
//回転処理
/*
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
*/
//右回転処理
/*
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
*/
//ブロック固定処理
/*
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
*/
//リスのアニメーション
void squirrel_anime(){

	if(squirrel_point % 32 == 1){
		//snd_reg_init();// サウンドレジスタ初期化
	        //snd_track_init();// サウンドトラック初期化
	        //snd_req_bgm(snd_bgm_adr[2][0],snd_bgm_adr[2][1],snd_bgm_adr[2][2],snd_bgm_adr[2][3]);
	        sndreq_se(3U);
	}

	if(squirrel_point == 144){
		squirrel_point = 0;
		squirrel_flag = 0;
		//フィールドリセット処理
		memset(stage, 0U, sizeof(stage)); 
		my_init_var();// テトリス初期化
		sndreq_se(1U);//効果音設定
		my_clear_field();       
		my_make_field2();
		my_draw_field();//描画処理
	        snd_reg_init();// サウンドレジスタ初期化
	        //snd_track_init();// サウンドトラック初期化
	        snd_req_bgm(snd_bgm_adr[3][0],snd_bgm_adr[3][1],snd_bgm_adr[3][2],snd_bgm_adr[3][3]);
	}

    set_sprite_tile(24, 2); //スプライトk番にタイル1番を設定
	set_sprite_tile(25, 3); //スプライトk番にタイル1番を設定
	set_sprite_tile(26, 2); //スプライトk番にタイル1番を設定
	set_sprite_tile(27, 3); //スプライトk番にタイル1番を設定
	//move_sprite(6, 0, squirrel_point);
	//move_sprite(6, 8, squirrel_point);
	//りすA
	move_sprite(24, 16+squirrel_point, squirrel_point);
	move_sprite(25, 24+squirrel_point, squirrel_point);
	//りすB
	move_sprite(26, 72-squirrel_point, squirrel_point+4);
	move_sprite(27, 80-squirrel_point, squirrel_point+4);
	squirrel_point++;
}
//キー入力
void my_get_key(){

	//キー入力待ち
	input_old = input_on;
	input_on = joypad();
	input_edge = input_on ^ input_old;
	input_trg = input_on & input_edge;
  	
	//キーが入力された時にフラグを立てる
	if(input_on == 0U){
		input_trg_flag = 1U;
	}

	if(input_trg_flag == 1U){

	if (input_on & J_RIGHT){ // 右入力
			my_collision_right();//当たり判定チェック
			if(collision_flag == 0){//右に壁が無いなら
				block_x++;//ブロックを右へ移動
			//	sndreq_se(2U); //音を鳴らすとBGMも早くなる。
			}
	}else
	if (input_on & J_LEFT){ // 左入力
			my_collision_left();//当たり判定チェック
			if(collision_flag == 0){//左に壁が無いなら
				block_x--;//ブロックを左へ移動
			//	sndreq_se(2U);
			}
	}else
  	if (input_on & J_DOWN){ //下入力
		
			CountFrm = 0U;//全体ループで1マスブロックが下へ移動
			
		}else
		if(input_trg & J_A){//Aボタン
		sndreq_se(1U);
		my_turn_right();
		
		}
	if (input_trg & J_START){//PAUSE処理
	//	pause_flag = 1U;//PAUSE状態にする。
		sndreq_se(3U);//効果音設定
	}
	if (input_trg & J_B){//Bボタン
	//	pause_flag = 1U;//PAUSE状態にする。
		if(hina_state_num == 2){
		squirrel_flag = 1U;//アニメーションフラグをON
		//Bボタン押下時処理
		/*
		memset(stage, 0U, sizeof(stage)); 
		my_init_var();// テトリス初期化
		sndreq_se(1U);//効果音設定
		my_clear_field();	
		my_make_field2();
		my_draw_field();//描画処理
		*/
		}
	}

	}
}

//アニメーション描画
/*
void hina_anime()
{
	UBYTE i,j;
//	wait_vbl_done();

	//アニメーション描画が終わったらフラグを初期化（外付けでもいいかも）
	anime_flag = 0U;
	//delay(1000);

//	j = (rand() % 3;

	for(i=0;i<3;i++){
		j = (rand() % 3);
		set_hina(j);
		delay(100);
		sndreq_se(3U);//効果音設定
	
	}
	hina_state_num = j;//ヒナのアイコン番号を格納



}
*/
//花アニメーション描画
/*
void hana_anime()
{

	UBYTE i,j,k;

	//アニメーション描画が終わったらフラグを初期化（外付けでもいいかも）
	anime_flag = 0U;

	//set_sprite_tile(5,30);

	for(i=0U;i<127U;i++){
		delay(100);
		move_sprite( 5, 48, i); //hinaを表示に合わせる
	}
}
*/

//タイトル時初期化
void init_title()
{
	set_bkg_data(0,255,TileData);
	set_bkg_tiles(0,0,20,18,MapData);
}

void init_game()
{
	seed.b.h = DIV_REG;
	initrand(seed.w);// 乱数初期化
	ScoreLine = 0U;//スコア初期化
	hina_state = 0U;//ヒナ状態
	hina_state_num = 0U;//ヒナ番号
	anime_flag = 0U;
	squirrel_flag = 0U;//リスフラグを初期化
	game_mode = GAMEMODE_INIT;//2はタイトル

	//空セット
	printf(" ");
	DISPLAY_OFF;// 画面全体を非表示
	HIDE_SPRITES;// スプライトを非表示 

	SPRITES_8x8;// スプライトモードを8*8に設定
	set_sprite_data(0, 6, TestTileLabel); // タイルデータをスプライトとして設定(0番から4つ)
	set_sprite_data(6,16, HinaTiles01);//ヒナ（通常）

	set_bkg_data(0, 6,TestTileLabel); // タイルデータをBGとして設定

	set_bkg_tiles(0, 0, 32, 20, FieldTileLabel);

	my_init_var();// テトリス初期化
	view_hina();//ヒナ表示

	SHOW_BKG;// BGを表示
	SHOW_SPRITES;// スプライトを表示 
	DISPLAY_ON;// 画面全体を表示

}

// game_mode毎の初期化（ファースト）
void init()
{
	if (first == 0U) {
		switch (game_mode) {
			case GAMEMODE_INIT:
				game_mode = GAMEMODE_TITLE;// タイトルへ
			case GAMEMODE_TITLE:
				init_title();
				break;
			case GAMEMODE_PLAY:
				init_game();
				break;
		}
		first = 1U;
	}
}

//タイトル画面処理
void proc_title()
{
	SHOW_BKG;
	seed.b.l = DIV_REG;


	waitpad(J_START);// スタートキーの入力待ち
	game_mode = GAMEMODE_PLAY;//2はタイトル
	first = 0U;// ファーストフラグ
}

//ゲームプレイ時
void proc_game()
{
	//BGM再生
//	snd_driver();

	if(pause_flag == 0){//pause状態ではない場合＝ゲームプレイの状態の処理

		//落下までのカウント変数をカウントダウン
		CountFrm = abs(CountFrm - 1U);

		if(gameover_flag == 0){//ゲームオーバーではないとき
		
			if(anime_flag == 0 && squirrel_flag == 0){//アニメーション描画中ではないとき
				sndreq_bgm(SND_BGM_PLAY);// BGM再生SND_BGM_PLAY

				//スコア表示
				gotoxy(12,13);
				printf("Score:%d",ScoreLine);
	
				block_cnt = block_y;
				// テトリス描画処理
				my_clear_field();

				if(clear_flag == 0){
					my_make_block();//ブロック登録
					my_gameover();//ゲームオーバー判定
					my_get_key();//キー入力
					my_make_field();//フィールド反映
			//	my_fix_block();//ブロック固定化BG化
		
					if(CountFrm <= 0U){//落下までのカウント変数がゼロなら落下させる
						my_fix_block();//ブロック固定化BG化
						my_fall_block(block_cnt);//ブロック落下処理
						CountFrm = 20U;//カウント変数を初期値に戻す
					}
				}else{
				my_clear_line();
				my_make_field2();
				//アイコンセット
			//	set_hina(hina_state);
				//アニメーション描画フラグ
				anime_flag = 1U;
				}

				my_draw_field();//描画処理
			}
			else if(anime_flag == 1){

			hina_anime();
			}
			else if(squirrel_flag == 1){
			squirrel_anime();
			}
		/*	else {
			//花状態のアニメーしょん
			hana_anime();
			
			}*/
			
		//ゲームオーバー時処理
		}else{//以下の処理は1度だけ
			if(CountSnd == 0){
				gotoxy(2,10);
				printf("GAME OVER");
				CountSnd = 1U;//BGMのセットは1度だけにするためのフラグ
				sndreq_bgm(2U);//ゲームオーバー用BGMセット
				CountSnd = 1U;
			}else{//スタートボタンが押させるまで無限ループ
				snd_driver();//BGM再生
				while((joypad() & J_START)){
					reset();//ゲームのリセット
				}
			}	
		}

	}else{//pause時の処理
		while(!(joypad() & J_START)){//スタートボタンが押させるまで無限ループ
		pause_flag = 0U;//pause状態の解除を示すフラグを0(解除)にする
		}
	
	

	}

}

// game_mode毎の処理
void proc()
{
	switch (game_mode) {
		case GAMEMODE_INIT:
			game_mode = GAMEMODE_TITLE;
			break;
		case GAMEMODE_TITLE:
			proc_title();
			break;
		case GAMEMODE_PLAY:
			proc_game();
			break;
	}
//	intcnt++;
	snd_driver();
}


// VBL割込で呼ばれる
void vbl_isr(void)
{

//	init();
//	proc();

	wait_vbl_done();

	//BGM再生
	snd_driver();

	if(pause_flag == 0){//pause状態ではない場合＝ゲームプレイの状態の処理

		//落下までのカウント変数をカウントダウン
		CountFrm = abs(CountFrm - 1U);

		if(gameover_flag == 0){//ゲームオーバーではないとき
		
			if(anime_flag == 0 && squirrel_flag == 0){//アニメーション描画中ではないとき
				sndreq_bgm(SND_BGM_TITLE);// BGM再生SND_BGM_PLAY

				//スコア表示
				gotoxy(12,13);
				printf("Score:%d",ScoreLine);
	
				block_cnt = block_y;
				// テトリス描画処理
				my_clear_field();

				if(clear_flag == 0){
					my_make_block();//ブロック登録
					my_gameover();//ゲームオーバー判定
					my_get_key();//キー入力
					my_make_field();//フィールド反映
			//	my_fix_block();//ブロック固定化BG化
		
					if(CountFrm <= 0U){//落下までのカウント変数がゼロなら落下させる
						my_fix_block();//ブロック固定化BG化
						my_fall_block(block_cnt);//ブロック落下処理
						CountFrm = FALLING_FRAME;//カウント変数を初期値に戻す
					}
				}else{
				my_clear_line();
				my_make_field2();

				//アニメーション描画フラグ
				anime_flag = 1U;
				}

				my_draw_field();//描画処理
			}
			else if(anime_flag == 1){
			//ヒナアニメーション描画
			hina_anime();
			}
			else if(squirrel_flag == 1){
			squirrel_anime();//リスが上から降ってくるアニメーション描画
			}
			
		//ゲームオーバー時処理
		}else{//以下の処理は1度だけ
			if(CountSnd == 0){
				gotoxy(2,10);
				printf("GAME OVER");
				CountSnd = 1U;//BGMのセットは1度だけにするためのフラグ
				sndreq_bgm(2U);//ゲームオーバー用BGMセット
				CountSnd = 1U;
			}else{//スタートボタンが押させるまで無限ループ
				snd_driver();//BGM再生
				while((joypad() & J_START)){
					reset();//ゲームのリセット
				}
			}	
		}

	}else{//pause時の処理
		while(!(joypad() & J_START)){//スタートボタンが押させるまで無限ループ
		pause_flag = 0U;//pause状態の解除を示すフラグを0(解除)にする
		}

	}


}

void main()
{
	
	CountFrm = FALLING_FRAME;//ブロック落下用変数。小さいほど落下速度が速くなる。
	CountSnd = 1U;
	pause_flag = 0U;

	//タイトル画面設定
	set_bkg_data(0,255,TileData);
	set_bkg_tiles(0,0,20,18,MapData);

	SHOW_BKG;//画面表示
	seed.b.l = DIV_REG;//乱数生成
	game_mode = GAMEMODE_INIT;
	snd_reg_init();// サウンドレジスタ初期化
	snd_track_init();// サウンドトラック初期化
	sndreq_bgm(SND_BGM_TITLE);//タイトル用BGM設定
	/*
	 SPRITES_8x8;// スプライトモードを8*8に設定
	 set_sprite_data(0, 6, TestTileLabel); // タイルデータをスプライトとして設定(0番から4つ)
	 set_sprite_data(6,16, HinaTiles01);//ヒナ（通常）
	 view_hina();//ヒナ表示
	 SHOW_SPRITES;// スプライトを表示 
	 DISPLAY_ON;// 画面全体を表示*/
	//タイトル画面ループ
	while(1){

	snd_driver();
	delay(15);
		if(joypad() == J_START){
			break;
		}
	//	sndreq_se(3U);//効果音設定
	}
	CountSnd = 1U;
	
	seed.b.h = DIV_REG;
	initrand(seed.w);// 乱数初期化
	ScoreLine = 0U;//スコア初期化
	hina_state = 0U;//ヒナ状態
	hina_state_num = 0U;//ヒナ番号
	anime_flag = 0U;
	squirrel_flag = 0U;
	squirrel_point = 0U;
	game_mode = GAMEMODE_TITLE;//2はタイトル

	//空セット
	printf(" ");
	DISPLAY_OFF;// 画面全体を非表示
	HIDE_SPRITES;// スプライトを非表示 

	SPRITES_8x8;// スプライトモードを8*8に設定
	set_sprite_data(0, 7, TestTileLabel); // タイルデータをスプライトとして設定(0番から6つ)
	set_sprite_data(6,16, HinaTiles01);//ヒナ（通常）

	set_bkg_data(0, 7,TestTileLabel); // タイルデータをBGとして設定(外壁・ブロック・リスのタイル)

	set_bkg_tiles(0, 0, 32, 20, FieldTileLabel); //マップデータをセット(セット開始x座標,セット開始y座標,セット幅,セット高さ)

	my_init_var();// テトリス初期化
	view_hina();//ヒナ表示

	SHOW_BKG;// BGを表示
	SHOW_SPRITES;// スプライトを表示 
	DISPLAY_ON;// 画面全体を表示
	// set_bkg_data(0, 2,TestTileLabel); // タイルデータをBGとして設定

	disable_interrupts();// 割込無効
	add_VBL(vbl_isr);// VBL割込に追加
	enable_interrupts();// 割込有効
	set_interrupts(VBL_IFLAG);// VBL割込セット

	snd_reg_init();// サウンドレジスタ初期化
	snd_track_init();// サウンドトラック初期化

	// while (1) {// 無限ループ
	/*
	if(joypad() == J_START){
		 enable_interrupts();// 割込有効
	}*/
	// }

}
