#ifndef	_TETRIS_H_
#define _TETRIS_H_

#pragma once
#include <stdio.h>

//ブロックの縦幅・横幅
#define BLOCK_HEIGHT 4
#define BLOCK_WIDTH 4

//フィールドの縦幅・横幅
#define FIELD_HEIGHT 20
#define FIELD_WIDTH 16

//ブロックの横位置
extern UBYTE block_x;
//ブロックの縦位置
extern UBYTE block_y;

// ステージ・フィールド定義
extern UBYTE block[BLOCK_HEIGHT][BLOCK_WIDTH];
extern UBYTE stage[FIELD_HEIGHT][FIELD_WIDTH];
extern UBYTE field[FIELD_HEIGHT][FIELD_WIDTH];

//回転後のブロック
extern UBYTE turn_block[BLOCK_HEIGHT][BLOCK_WIDTH];

// 壁とブロックの当たり判定フラグ
extern UBYTE collision_flag;

// ブロックを生成するかどうかのフラグ
extern UBYTE make_block_flag;

//ゲームオーバーしたかどうか
extern UBYTE gameover_flag;

//ブロックの識別用
extern UBYTE block_id;

//回転状態
extern UBYTE turn_point;

//キーを押したかどうかのフラグ
extern UBYTE input_trg_flag;

//ラインが消えたかどうか
extern UBYTE clear_count;

//消すべきライン行を格納
extern UBYTE clear_line_point[20];

//消すかどうかフラグ
extern UBYTE clear_flag;

//点数表示用
extern UBYTE ScoreLine;

//ブロック
extern UBYTE blocks[BLOCK_HEIGHT * 7][BLOCK_WIDTH * 4]; 

//プロトタイプ宣言
//ステージ初期化
void my_init_var(void);
//ブロックの座標を戻す
void my_init_var2(void);
//　ブロックの登録
void my_make_block(void);
// フィールドへステージとブロックの重ね合わせて代入
void my_make_field(void);
//「block」を「stage」に保存する
void my_save_block(void);
// 描画処理
void my_draw_field(void);
//フィールドクリア処理
void my_clear_field(void);
//ブロック落下処理
void my_fall_block(UBYTE cnt);
//当たり判定処理左側
void my_collision_left(void);
//当たり判定処理右側
void my_collision_right(void);
//当たり判定処理底
void my_collision_bottom(void);
//一番上までブロックが到達した判定
void my_collision_center(void);
//ゲームオーバー判定
void my_gameover(void);
//消すラインの探索
void my_search_line(void);
//ライン削除
void my_clear_line(void);
//
void my_make_field2(void);
//回転処理
void my_collision_turn(void);
//右回転処理
void my_turn_right(void);
//ブロック固定処理
void my_fix_block(void);


#endif
