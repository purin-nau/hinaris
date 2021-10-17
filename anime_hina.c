#include <rand.h>
#include <gb.h>
#include "anime_hina.h"


//ヒナの状態
UBYTE hina_state;
//ヒナの状態番号
UBYTE hina_state_num;
//アニメーション中かどうかのフラグ
UBYTE anime_flag;

//アイコン表示
void view_hina(){
	
	UBYTE i,j,k;

	//スプライト番号7～23がヒナ1
	for(i = 0U;i < 4U; i++){
		for(j = 0U;j < 4U; j++){
			set_sprite_tile( j+6U+(i*4U), j+6U+(i*4U)); //6Uは初期表示
			move_sprite( j+6U+i*4U, j*8U+136U, i*8U+40); //hinaを表示に合わせる
		}
	}
}

//ヒナのアイコンをセット
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
}


//アニメーション描画
void hina_anime()
{
	UBYTE i,j;

	//アニメーション描画が終わったらフラグを初期化（外付けでもいいかも）
	anime_flag = 0U;

	for(i=0;i<3;i++){
		j = (rand() % 3);
		set_hina(j);
		delay(100);
		sndreq_se(3U);//効果音設定
	
	}
	hina_state_num = j;//ヒナのアイコン番号を格納

}
//花アニメーション描画
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


