#include <rand.h>
#include <gb.h>
#include "anime_hina.h"


//�q�i�̏��
UBYTE hina_state;
//�q�i�̏�Ԕԍ�
UBYTE hina_state_num;
//�A�j���[�V���������ǂ����̃t���O
UBYTE anime_flag;

//�A�C�R���\��
void view_hina(){
	
	UBYTE i,j,k;

	//�X�v���C�g�ԍ�7�`23���q�i1
	for(i = 0U;i < 4U; i++){
		for(j = 0U;j < 4U; j++){
			set_sprite_tile( j+6U+(i*4U), j+6U+(i*4U)); //6U�͏����\��
			move_sprite( j+6U+i*4U, j*8U+136U, i*8U+40); //hina��\���ɍ��킹��
		}
	}
}

//�q�i�̃A�C�R�����Z�b�g
void set_hina(UBYTE num)
{
	switch(num){
		case 0:
			set_sprite_data(6,16, HinaTiles01);//�q�i�i�ʏ�j
			gotoxy(12,3);
			printf(" ");
			break;
		case 1:
			set_sprite_data(6,16, HinaTiles02);//�q�i�i�ڂ݁j
			gotoxy(12,3);
			printf(" ");
			sndreq_se(3U);//���ʉ��ݒ�
			break;
		case 2:
			set_sprite_data(6,16, HinaTiles03);//�q�i�i�ԁj
			sndreq_se(3U);//���ʉ��ݒ�
			gotoxy(12,3);
		//	printf("B");
			break;
	}
}


//�A�j���[�V�����`��
void hina_anime()
{
	UBYTE i,j;

	//�A�j���[�V�����`�悪�I�������t���O���������i�O�t���ł����������j
	anime_flag = 0U;

	for(i=0;i<3;i++){
		j = (rand() % 3);
		set_hina(j);
		delay(100);
		sndreq_se(3U);//���ʉ��ݒ�
	
	}
	hina_state_num = j;//�q�i�̃A�C�R���ԍ����i�[

}
//�ԃA�j���[�V�����`��
void hana_anime()
{

	UBYTE i,j,k;

	//�A�j���[�V�����`�悪�I�������t���O���������i�O�t���ł����������j
	anime_flag = 0U;

	//set_sprite_tile(5,30);

	for(i=0U;i<127U;i++){
		delay(100);
		move_sprite( 5, 48, i); //hina��\���ɍ��킹��
	}
}


