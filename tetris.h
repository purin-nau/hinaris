#ifndef	_TETRIS_H_
#define _TETRIS_H_

#pragma once
#include <stdio.h>

//�u���b�N�̏c���E����
#define BLOCK_HEIGHT 4
#define BLOCK_WIDTH 4

//�t�B�[���h�̏c���E����
#define FIELD_HEIGHT 20
#define FIELD_WIDTH 16

//�u���b�N�̉��ʒu
extern UBYTE block_x;
//�u���b�N�̏c�ʒu
extern UBYTE block_y;

// �X�e�[�W�E�t�B�[���h��`
extern UBYTE block[BLOCK_HEIGHT][BLOCK_WIDTH];
extern UBYTE stage[FIELD_HEIGHT][FIELD_WIDTH];
extern UBYTE field[FIELD_HEIGHT][FIELD_WIDTH];

//��]��̃u���b�N
extern UBYTE turn_block[BLOCK_HEIGHT][BLOCK_WIDTH];

// �ǂƃu���b�N�̓����蔻��t���O
extern UBYTE collision_flag;

// �u���b�N�𐶐����邩�ǂ����̃t���O
extern UBYTE make_block_flag;

//�Q�[���I�[�o�[�������ǂ���
extern UBYTE gameover_flag;

//�u���b�N�̎��ʗp
extern UBYTE block_id;

//��]���
extern UBYTE turn_point;

//�L�[�����������ǂ����̃t���O
extern UBYTE input_trg_flag;

//���C�������������ǂ���
extern UBYTE clear_count;

//�����ׂ����C���s���i�[
extern UBYTE clear_line_point[20];

//�������ǂ����t���O
extern UBYTE clear_flag;

//�_���\���p
extern UBYTE ScoreLine;

//�u���b�N
extern UBYTE blocks[BLOCK_HEIGHT * 7][BLOCK_WIDTH * 4]; 

//�v���g�^�C�v�錾
//�X�e�[�W������
void my_init_var(void);
//�u���b�N�̍��W��߂�
void my_init_var2(void);
//�@�u���b�N�̓o�^
void my_make_block(void);
// �t�B�[���h�փX�e�[�W�ƃu���b�N�̏d�ˍ��킹�đ��
void my_make_field(void);
//�ublock�v���ustage�v�ɕۑ�����
void my_save_block(void);
// �`�揈��
void my_draw_field(void);
//�t�B�[���h�N���A����
void my_clear_field(void);
//�u���b�N��������
void my_fall_block(UBYTE cnt);
//�����蔻�菈������
void my_collision_left(void);
//�����蔻�菈���E��
void my_collision_right(void);
//�����蔻�菈����
void my_collision_bottom(void);
//��ԏ�܂Ńu���b�N�����B��������
void my_collision_center(void);
//�Q�[���I�[�o�[����
void my_gameover(void);
//�������C���̒T��
void my_search_line(void);
//���C���폜
void my_clear_line(void);
//
void my_make_field2(void);
//��]����
void my_collision_turn(void);
//�E��]����
void my_turn_right(void);
//�u���b�N�Œ菈��
void my_fix_block(void);


#endif
