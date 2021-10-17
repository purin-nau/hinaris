#include <rand.h>
#include "tetris.h"
#include "anime_hina.h"

/*�u���b�N�̉��ʒu*/
UBYTE block_x;

/*�u���b�N�̏c�ʒu*/
UBYTE block_y;

// �X�e�[�W�E�t�B�[���h��`
UBYTE block[BLOCK_HEIGHT][BLOCK_WIDTH];
UBYTE stage[FIELD_HEIGHT][FIELD_WIDTH];
UBYTE field[FIELD_HEIGHT][FIELD_WIDTH];

//��]��̃u���b�N
UBYTE turn_block[BLOCK_HEIGHT][BLOCK_WIDTH];

// �ǂƃu���b�N�̓����蔻��t���O
UBYTE collision_flag;

// �u���b�N�𐶐����邩�ǂ����̃t���O
UBYTE make_block_flag;

//�Q�[���I�[�o�[�������ǂ���
UBYTE gameover_flag;

//�u���b�N�̎��ʗp
UBYTE block_id;

//��]���
UBYTE turn_point;

//�L�[�����������ǂ����̃t���O
UBYTE input_trg_flag;

//���C�������������ǂ���
UBYTE clear_count;

//�����ׂ����C���s���i�[
UBYTE clear_line_point[20];

//�������ǂ����t���O
UBYTE clear_flag;

//�_���\���p
UBYTE ScoreLine;

// �u���b�N�� ��`
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


// �X�e�[�W������
void my_init_var(){
	
	//���[�v�p�ϐ�
	UBYTE i,j;

	//�ǂƂȂ�Ƃ����9����
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

	//�u���b�N�̏����ʒu
	block_x = 7;
	block_y = 0;

	//�u���b�N�Œ�t���O
	collision_flag = 0;
	//�Q�[���I�[�o�[�t���O
	gameover_flag = 0;
	//??
	make_block_flag = 1;
	//�u���b�N���ʃt���O
	block_id = 0;
	//���C�������������t���O������
	clear_count = 0;
	//��]�t���O������
	turn_point = 0;
	//�L�[���͂������ǂ����t���O��������
	input_trg_flag = 0;
}

//�u���b�N�̍��W��߂�
void my_init_var2(){
	block_x = 7;
	block_y = 0;
	make_block_flag = 1;
	turn_point = 0;
	input_trg_flag = 0;
}
//�@�u���b�N�̓o�^
void my_make_block(){

	//���[�v�p�ϐ�
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
// �t�B�[���h�փX�e�[�W�ƃu���b�N�̏d�ˍ��킹�đ��
void my_make_field(){

	// ���[�v�p�ϐ�
	UBYTE x,y;

	//
	UBYTE fld_tmp = 0U;
	
	// �t�B�[���h�փX�e�[�W�̒l����
	memcpy(field, stage, sizeof(stage));
	
	// �t�B�[���h�փu���b�N�̏����ʒu����u���b�N�𑫂������̂���
	for(y=0;y<BLOCK_HEIGHT;y++){
		for(x=0;x<BLOCK_WIDTH;x++){
			fld_tmp = field[block_y + y][block_x + x];
			field[block_y + y][block_x + x] = fld_tmp + block[y][x];		
		}
	}	
}
//�ublock�v���ustage�v�ɕۑ�����
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

// �`�揈��
void my_draw_field(){

	// ���[�v�p�ϐ�
	UBYTE i,j,k,l;

	k = 0U;

//H20W16
	for(i=0;i<FIELD_HEIGHT;i++){
		for(j=0;j<FIELD_WIDTH;j++){
			if(field[i][j] == 1){
				k++;
				set_sprite_tile( k, 1); //�X�v���C�gk�ԂɃ^�C��1�Ԃ�ݒ�
				move_sprite( k, j*8+8-16, i*8+16); //�u���b�N�̕`��e�g���X�z��̒l��(8,16)��������GB�̕\���ɍ��킹��
			}
		}
	}
}

//�t�B�[���h�N���A����
void my_clear_field(){
	UBYTE i,j;
	memset(field, 0U, sizeof(field)); 
}

//�u���b�N��������
void my_fall_block(UBYTE cnt){
	if(make_block_flag == 0 && cnt == block_y){
		block_y++;
	}
}

//�����蔻�菈������
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

//�����蔻�菈���E��
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

//�����蔻�菈����
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

//��ԏ�܂Ńu���b�N�����B��������
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

//�Q�[���I�[�o�[����
void my_gameover(void){
	my_collision_center();

	if(collision_flag != 0){
		gameover_flag = 1;
	}
}

//�������C���̒T��
void my_search_line(){

	//���[�v�p�ϐ�
	UBYTE i,j;
	UBYTE lc = 0U;
	//�S�v�f���O�ɏ�����
	/*for(i=0;i<FIELD_HEIGHT - 3;i++){
		clear_line_point[i] = 0;
	}*/
	memset(clear_line_point, 0U, sizeof(clear_line_point)); 

	//�S�T�����A�󔒂�����΂P�𗧂Ăă��[�v���甲����
	//�܂�A���ׂău���b�N�Ȃ�O�̂܂�
	for(i=0;i<FIELD_HEIGHT - 3;i++){
		for(j=3;j<FIELD_WIDTH - 3;j++){
			if(stage[i][j] == 0){
				clear_line_point[i] = 1;
				break;
			}
		}
	}
	//�O�Ȃ�N���A�t���O���P�ɂ���
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

//���C���폜
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
		//�����̏����ŃG���[�B����BYTE�^�ň�����̈�𒴂��Ă���B�}�C�i�X�����͈����Ȃ��B
		//for(i=0;i<FIELD_HEIGHT;i++){
		//for(j=0;j<FIELD_WIDTH;j++){
		for(i=FIELD_HEIGHT-4U;i>0;i--){
			//���ׂău���b�N�Ȃ�O�����A�u���b�N������Ƃ������ׂău���b�N�ł͂Ȃ��s
			if(clear_line_point[i] != 0){	//���̍s�ԍ���remain_line_point�Ɋi�[
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

	// �t�B�[���h�փX�e�[�W�̒l����
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

//��]����
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

//�E��]����
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

//�u���b�N�Œ菈��
void my_fix_block(){

	my_collision_bottom();

	//�Œ肳�ꂽ�Ȃ�
	if(collision_flag != 0){



		// ���[�v�p�ϐ�
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
		
		sndreq_se(1U);//�ڒn�����特�Ȃ炷
		
		if(clear_flag == 0){
			my_init_var2();
		}



	}
}
