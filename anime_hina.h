#ifndef	_ANIME_HINA_H_
#define _ANIME_HINA_H_

#pragma once
#include <stdio.h>
#include "hina_icon_01.c"//�q�i�i�ʏ�j
#include "hina_icon_02.c"//�q�i�i�ڂ݁j
#include "hina_icon_03.c"//�q�i�i�ԁj

//�q�i�̏��
extern UBYTE hina_state;
//�q�i�̏�Ԕԍ�
extern UBYTE hina_state_num;
//�A�j���[�V���������ǂ����̃t���O
extern UBYTE anime_flag;

//�A�C�R���\��
void view_hina(void);
//�q�i�̃A�C�R�����Z�b�g
void set_hina(UBYTE num);
//�A�j���[�V�����`��
void hina_anime(void);
//�ԃA�j���[�V�����`��
void hana_anime(void);

#endif
