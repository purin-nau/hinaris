#ifndef	_ANIME_HINA_H_
#define _ANIME_HINA_H_

#pragma once
#include <stdio.h>
#include "hina_icon_01.c"//ヒナ（通常）
#include "hina_icon_02.c"//ヒナ（つぼみ）
#include "hina_icon_03.c"//ヒナ（花）

//ヒナの状態
extern UBYTE hina_state;
//ヒナの状態番号
extern UBYTE hina_state_num;
//アニメーション中かどうかのフラグ
extern UBYTE anime_flag;

//アイコン表示
void view_hina(void);
//ヒナのアイコンをセット
void set_hina(UBYTE num);
//アニメーション描画
void hina_anime(void);
//花アニメーション描画
void hana_anime(void);

#endif
