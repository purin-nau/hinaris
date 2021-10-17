#include <stdio.h>
#include <gb.h>
#include <stdlib.h>
#include <rand.h>

// �T�E���h�h���C�o
#include "snd_drv.c"

// �T�E���h�f�[�^
#include "snd_data.c"

// �^�C���f�[�^
#include "testtile.c"//�t�B�[���h�E�u���b�N�E���X
#include "hina_risu.C"//�^�C�g��

// �}�b�v�f�[�^
#include "Filed.c"//�e�g���X���
#include "hina_risu_map.C"//�^�C�g��

// �����C���N���[�h
#include "tetris.h"//�e�g���X����
#include "anime_hina.h"//�A�j���[�V��������

//�Q�[���̏��
#define GAMEMODE_INIT    0U
#define GAMEMODE_TITLE   2U
#define GAMEMODE_PLAY    3U
#define GAMEMODE_END     4U

/*�u���b�N�̗������x(�ݒ�l���̃t���[���o�߂Ńu���b�N��1�}�X����*/
#define FALLING_FRAME 20U

//pause�t���O
UBYTE pause_flag;

//�����ϐ�
UBYTE CountFrm;

// �����V�[�h
fixed seed;

//�T�E���h�ϐ�
UBYTE CountSnd;

// �L�[���͗p
UBYTE input_on, input_old, input_edge, input_trg;

//
UBYTE block_cnt;

//���X�̃A�j���[�V���������ǂ����̃t���O
UBYTE squirrel_flag;

//���X�̏ꏊ
UBYTE squirrel_point;

//�Q�[���̏��
UBYTE game_mode;

// �t�@�[�X�g�i����j�t���O
UBYTE first;


// BGM���N�G�X�g
void sndreq_bgm(UBYTE req_bgm)
{
//	input_trg = joypad();// �L�[����
if (CountSnd == 1U) {
	// BGM�Đ�
	snd_req_bgm(snd_bgm_adr[req_bgm][0],snd_bgm_adr[req_bgm][1],snd_bgm_adr[req_bgm][2],snd_bgm_adr[req_bgm][3]);
CountSnd = 0U;	
}
 // BGM�����
 snd_driver();
}

// S.E.���N�G�X�g
void sndreq_se(UBYTE req_se)
{
	// SE�Đ�
	snd_req_se(snd_se_adr[req_se], snd_se_atr[req_se][0], snd_se_atr[req_se][1]);
	 // BGM�����
 	snd_driver();
}

// �X�e�[�W������
/*
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
*/
//�u���b�N�̍��W��߂�
/*
void my_init_var2(){
	block_x = 7;
	block_y = 0;
	make_block_flag = 1;
	turn_point = 0;
	input_trg_flag = 0;
}
*/
//�@�u���b�N�̓o�^
/*
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
*/
//�A�C�R���\��
/*
void view_hina(){
	
	UBYTE i,j,k;

	//�X�v���C�g�ԍ�7�`23���q�i1
	for(i = 0U;i < 4U; i++){
		for(j = 0U;j < 4U; j++){
			set_sprite_tile( j+6U+(i*4U), j+6U+(i*4U)); //6U�͏����\��
			move_sprite( j+6U+i*4U, j*8U+136U, i*8U+40); //hina��\���ɍ��킹��
		}
	}

}*/

//�q�i�̃A�C�R�����Z�b�g
/*
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
}*/

// �t�B�[���h�փX�e�[�W�ƃu���b�N�̏d�ˍ��킹�đ��
/*
void my_make_field(){

	// ���[�v�p�ϐ�
	UBYTE i,j,x,y;

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
*/
//�ublock�v���ustage�v�ɕۑ�����
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


/*// �`�揈��
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
*/

/*//�t�B�[���h�N���A����
void my_clear_field(){
	UBYTE i,j;
	memset(field, 0U, sizeof(field)); 
}
*/

//�u���b�N��������
/*
void my_fall_block(UBYTE cnt){
	if(make_block_flag == 0 && cnt == block_y){
		block_y++;
	}
}
*/

//�����蔻�菈������
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
//�����蔻�菈���E��
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
//�����蔻�菈����
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
//��ԏ�܂Ńu���b�N�����B��������
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
//�Q�[���I�[�o�[����
/*
void my_gameover(){
	my_collision_center();

	if(collision_flag != 0){
		gameover_flag = 1;
	}
}
*/

//�������C���̒T��
/*
void my_search_line(){

	//���[�v�p�ϐ�
	UBYTE i,j;
	UBYTE lc = 0U;
	//�S�v�f���O�ɏ�����
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
}*/

//���C���폜
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
*/
//
/*
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
*/
//��]����
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
//�E��]����
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
//�u���b�N�Œ菈��
/*
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
*/
//���X�̃A�j���[�V����
void squirrel_anime(){

	if(squirrel_point % 32 == 1){
		//snd_reg_init();// �T�E���h���W�X�^������
	        //snd_track_init();// �T�E���h�g���b�N������
	        //snd_req_bgm(snd_bgm_adr[2][0],snd_bgm_adr[2][1],snd_bgm_adr[2][2],snd_bgm_adr[2][3]);
	        sndreq_se(3U);
	}

	if(squirrel_point == 144){
		squirrel_point = 0;
		squirrel_flag = 0;
		//�t�B�[���h���Z�b�g����
		memset(stage, 0U, sizeof(stage)); 
		my_init_var();// �e�g���X������
		sndreq_se(1U);//���ʉ��ݒ�
		my_clear_field();       
		my_make_field2();
		my_draw_field();//�`�揈��
	        snd_reg_init();// �T�E���h���W�X�^������
	        //snd_track_init();// �T�E���h�g���b�N������
	        snd_req_bgm(snd_bgm_adr[3][0],snd_bgm_adr[3][1],snd_bgm_adr[3][2],snd_bgm_adr[3][3]);
	}

    set_sprite_tile(24, 2); //�X�v���C�gk�ԂɃ^�C��1�Ԃ�ݒ�
	set_sprite_tile(25, 3); //�X�v���C�gk�ԂɃ^�C��1�Ԃ�ݒ�
	set_sprite_tile(26, 2); //�X�v���C�gk�ԂɃ^�C��1�Ԃ�ݒ�
	set_sprite_tile(27, 3); //�X�v���C�gk�ԂɃ^�C��1�Ԃ�ݒ�
	//move_sprite(6, 0, squirrel_point);
	//move_sprite(6, 8, squirrel_point);
	//�肷A
	move_sprite(24, 16+squirrel_point, squirrel_point);
	move_sprite(25, 24+squirrel_point, squirrel_point);
	//�肷B
	move_sprite(26, 72-squirrel_point, squirrel_point+4);
	move_sprite(27, 80-squirrel_point, squirrel_point+4);
	squirrel_point++;
}
//�L�[����
void my_get_key(){

	//�L�[���͑҂�
	input_old = input_on;
	input_on = joypad();
	input_edge = input_on ^ input_old;
	input_trg = input_on & input_edge;
  	
	//�L�[�����͂��ꂽ���Ƀt���O�𗧂Ă�
	if(input_on == 0U){
		input_trg_flag = 1U;
	}

	if(input_trg_flag == 1U){

	if (input_on & J_RIGHT){ // �E����
			my_collision_right();//�����蔻��`�F�b�N
			if(collision_flag == 0){//�E�ɕǂ������Ȃ�
				block_x++;//�u���b�N���E�ֈړ�
			//	sndreq_se(2U); //����炷��BGM�������Ȃ�B
			}
	}else
	if (input_on & J_LEFT){ // ������
			my_collision_left();//�����蔻��`�F�b�N
			if(collision_flag == 0){//���ɕǂ������Ȃ�
				block_x--;//�u���b�N�����ֈړ�
			//	sndreq_se(2U);
			}
	}else
  	if (input_on & J_DOWN){ //������
		
			CountFrm = 0U;//�S�̃��[�v��1�}�X�u���b�N�����ֈړ�
			
		}else
		if(input_trg & J_A){//A�{�^��
		sndreq_se(1U);
		my_turn_right();
		
		}
	if (input_trg & J_START){//PAUSE����
	//	pause_flag = 1U;//PAUSE��Ԃɂ���B
		sndreq_se(3U);//���ʉ��ݒ�
	}
	if (input_trg & J_B){//B�{�^��
	//	pause_flag = 1U;//PAUSE��Ԃɂ���B
		if(hina_state_num == 2){
		squirrel_flag = 1U;//�A�j���[�V�����t���O��ON
		//B�{�^������������
		/*
		memset(stage, 0U, sizeof(stage)); 
		my_init_var();// �e�g���X������
		sndreq_se(1U);//���ʉ��ݒ�
		my_clear_field();	
		my_make_field2();
		my_draw_field();//�`�揈��
		*/
		}
	}

	}
}

//�A�j���[�V�����`��
/*
void hina_anime()
{
	UBYTE i,j;
//	wait_vbl_done();

	//�A�j���[�V�����`�悪�I�������t���O���������i�O�t���ł����������j
	anime_flag = 0U;
	//delay(1000);

//	j = (rand() % 3;

	for(i=0;i<3;i++){
		j = (rand() % 3);
		set_hina(j);
		delay(100);
		sndreq_se(3U);//���ʉ��ݒ�
	
	}
	hina_state_num = j;//�q�i�̃A�C�R���ԍ����i�[



}
*/
//�ԃA�j���[�V�����`��
/*
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
*/

//�^�C�g����������
void init_title()
{
	set_bkg_data(0,255,TileData);
	set_bkg_tiles(0,0,20,18,MapData);
}

void init_game()
{
	seed.b.h = DIV_REG;
	initrand(seed.w);// ����������
	ScoreLine = 0U;//�X�R�A������
	hina_state = 0U;//�q�i���
	hina_state_num = 0U;//�q�i�ԍ�
	anime_flag = 0U;
	squirrel_flag = 0U;//���X�t���O��������
	game_mode = GAMEMODE_INIT;//2�̓^�C�g��

	//��Z�b�g
	printf(" ");
	DISPLAY_OFF;// ��ʑS�̂��\��
	HIDE_SPRITES;// �X�v���C�g���\�� 

	SPRITES_8x8;// �X�v���C�g���[�h��8*8�ɐݒ�
	set_sprite_data(0, 6, TestTileLabel); // �^�C���f�[�^���X�v���C�g�Ƃ��Đݒ�(0�Ԃ���4��)
	set_sprite_data(6,16, HinaTiles01);//�q�i�i�ʏ�j

	set_bkg_data(0, 6,TestTileLabel); // �^�C���f�[�^��BG�Ƃ��Đݒ�

	set_bkg_tiles(0, 0, 32, 20, FieldTileLabel);

	my_init_var();// �e�g���X������
	view_hina();//�q�i�\��

	SHOW_BKG;// BG��\��
	SHOW_SPRITES;// �X�v���C�g��\�� 
	DISPLAY_ON;// ��ʑS�̂�\��

}

// game_mode���̏������i�t�@�[�X�g�j
void init()
{
	if (first == 0U) {
		switch (game_mode) {
			case GAMEMODE_INIT:
				game_mode = GAMEMODE_TITLE;// �^�C�g����
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

//�^�C�g����ʏ���
void proc_title()
{
	SHOW_BKG;
	seed.b.l = DIV_REG;


	waitpad(J_START);// �X�^�[�g�L�[�̓��͑҂�
	game_mode = GAMEMODE_PLAY;//2�̓^�C�g��
	first = 0U;// �t�@�[�X�g�t���O
}

//�Q�[���v���C��
void proc_game()
{
	//BGM�Đ�
//	snd_driver();

	if(pause_flag == 0){//pause��Ԃł͂Ȃ��ꍇ���Q�[���v���C�̏�Ԃ̏���

		//�����܂ł̃J�E���g�ϐ����J�E���g�_�E��
		CountFrm = abs(CountFrm - 1U);

		if(gameover_flag == 0){//�Q�[���I�[�o�[�ł͂Ȃ��Ƃ�
		
			if(anime_flag == 0 && squirrel_flag == 0){//�A�j���[�V�����`�撆�ł͂Ȃ��Ƃ�
				sndreq_bgm(SND_BGM_PLAY);// BGM�Đ�SND_BGM_PLAY

				//�X�R�A�\��
				gotoxy(12,13);
				printf("Score:%d",ScoreLine);
	
				block_cnt = block_y;
				// �e�g���X�`�揈��
				my_clear_field();

				if(clear_flag == 0){
					my_make_block();//�u���b�N�o�^
					my_gameover();//�Q�[���I�[�o�[����
					my_get_key();//�L�[����
					my_make_field();//�t�B�[���h���f
			//	my_fix_block();//�u���b�N�Œ艻BG��
		
					if(CountFrm <= 0U){//�����܂ł̃J�E���g�ϐ����[���Ȃ痎��������
						my_fix_block();//�u���b�N�Œ艻BG��
						my_fall_block(block_cnt);//�u���b�N��������
						CountFrm = 20U;//�J�E���g�ϐ��������l�ɖ߂�
					}
				}else{
				my_clear_line();
				my_make_field2();
				//�A�C�R���Z�b�g
			//	set_hina(hina_state);
				//�A�j���[�V�����`��t���O
				anime_flag = 1U;
				}

				my_draw_field();//�`�揈��
			}
			else if(anime_flag == 1){

			hina_anime();
			}
			else if(squirrel_flag == 1){
			squirrel_anime();
			}
		/*	else {
			//�ԏ�Ԃ̃A�j���[�����
			hana_anime();
			
			}*/
			
		//�Q�[���I�[�o�[������
		}else{//�ȉ��̏�����1�x����
			if(CountSnd == 0){
				gotoxy(2,10);
				printf("GAME OVER");
				CountSnd = 1U;//BGM�̃Z�b�g��1�x�����ɂ��邽�߂̃t���O
				sndreq_bgm(2U);//�Q�[���I�[�o�[�pBGM�Z�b�g
				CountSnd = 1U;
			}else{//�X�^�[�g�{�^������������܂Ŗ������[�v
				snd_driver();//BGM�Đ�
				while((joypad() & J_START)){
					reset();//�Q�[���̃��Z�b�g
				}
			}	
		}

	}else{//pause���̏���
		while(!(joypad() & J_START)){//�X�^�[�g�{�^������������܂Ŗ������[�v
		pause_flag = 0U;//pause��Ԃ̉����������t���O��0(����)�ɂ���
		}
	
	

	}

}

// game_mode���̏���
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


// VBL�����ŌĂ΂��
void vbl_isr(void)
{

//	init();
//	proc();

	wait_vbl_done();

	//BGM�Đ�
	snd_driver();

	if(pause_flag == 0){//pause��Ԃł͂Ȃ��ꍇ���Q�[���v���C�̏�Ԃ̏���

		//�����܂ł̃J�E���g�ϐ����J�E���g�_�E��
		CountFrm = abs(CountFrm - 1U);

		if(gameover_flag == 0){//�Q�[���I�[�o�[�ł͂Ȃ��Ƃ�
		
			if(anime_flag == 0 && squirrel_flag == 0){//�A�j���[�V�����`�撆�ł͂Ȃ��Ƃ�
				sndreq_bgm(SND_BGM_TITLE);// BGM�Đ�SND_BGM_PLAY

				//�X�R�A�\��
				gotoxy(12,13);
				printf("Score:%d",ScoreLine);
	
				block_cnt = block_y;
				// �e�g���X�`�揈��
				my_clear_field();

				if(clear_flag == 0){
					my_make_block();//�u���b�N�o�^
					my_gameover();//�Q�[���I�[�o�[����
					my_get_key();//�L�[����
					my_make_field();//�t�B�[���h���f
			//	my_fix_block();//�u���b�N�Œ艻BG��
		
					if(CountFrm <= 0U){//�����܂ł̃J�E���g�ϐ����[���Ȃ痎��������
						my_fix_block();//�u���b�N�Œ艻BG��
						my_fall_block(block_cnt);//�u���b�N��������
						CountFrm = FALLING_FRAME;//�J�E���g�ϐ��������l�ɖ߂�
					}
				}else{
				my_clear_line();
				my_make_field2();

				//�A�j���[�V�����`��t���O
				anime_flag = 1U;
				}

				my_draw_field();//�`�揈��
			}
			else if(anime_flag == 1){
			//�q�i�A�j���[�V�����`��
			hina_anime();
			}
			else if(squirrel_flag == 1){
			squirrel_anime();//���X���ォ��~���Ă���A�j���[�V�����`��
			}
			
		//�Q�[���I�[�o�[������
		}else{//�ȉ��̏�����1�x����
			if(CountSnd == 0){
				gotoxy(2,10);
				printf("GAME OVER");
				CountSnd = 1U;//BGM�̃Z�b�g��1�x�����ɂ��邽�߂̃t���O
				sndreq_bgm(2U);//�Q�[���I�[�o�[�pBGM�Z�b�g
				CountSnd = 1U;
			}else{//�X�^�[�g�{�^������������܂Ŗ������[�v
				snd_driver();//BGM�Đ�
				while((joypad() & J_START)){
					reset();//�Q�[���̃��Z�b�g
				}
			}	
		}

	}else{//pause���̏���
		while(!(joypad() & J_START)){//�X�^�[�g�{�^������������܂Ŗ������[�v
		pause_flag = 0U;//pause��Ԃ̉����������t���O��0(����)�ɂ���
		}

	}


}

void main()
{
	
	CountFrm = FALLING_FRAME;//�u���b�N�����p�ϐ��B�������قǗ������x�������Ȃ�B
	CountSnd = 1U;
	pause_flag = 0U;

	//�^�C�g����ʐݒ�
	set_bkg_data(0,255,TileData);
	set_bkg_tiles(0,0,20,18,MapData);

	SHOW_BKG;//��ʕ\��
	seed.b.l = DIV_REG;//��������
	game_mode = GAMEMODE_INIT;
	snd_reg_init();// �T�E���h���W�X�^������
	snd_track_init();// �T�E���h�g���b�N������
	sndreq_bgm(SND_BGM_TITLE);//�^�C�g���pBGM�ݒ�
	/*
	 SPRITES_8x8;// �X�v���C�g���[�h��8*8�ɐݒ�
	 set_sprite_data(0, 6, TestTileLabel); // �^�C���f�[�^���X�v���C�g�Ƃ��Đݒ�(0�Ԃ���4��)
	 set_sprite_data(6,16, HinaTiles01);//�q�i�i�ʏ�j
	 view_hina();//�q�i�\��
	 SHOW_SPRITES;// �X�v���C�g��\�� 
	 DISPLAY_ON;// ��ʑS�̂�\��*/
	//�^�C�g����ʃ��[�v
	while(1){

	snd_driver();
	delay(15);
		if(joypad() == J_START){
			break;
		}
	//	sndreq_se(3U);//���ʉ��ݒ�
	}
	CountSnd = 1U;
	
	seed.b.h = DIV_REG;
	initrand(seed.w);// ����������
	ScoreLine = 0U;//�X�R�A������
	hina_state = 0U;//�q�i���
	hina_state_num = 0U;//�q�i�ԍ�
	anime_flag = 0U;
	squirrel_flag = 0U;
	squirrel_point = 0U;
	game_mode = GAMEMODE_TITLE;//2�̓^�C�g��

	//��Z�b�g
	printf(" ");
	DISPLAY_OFF;// ��ʑS�̂��\��
	HIDE_SPRITES;// �X�v���C�g���\�� 

	SPRITES_8x8;// �X�v���C�g���[�h��8*8�ɐݒ�
	set_sprite_data(0, 7, TestTileLabel); // �^�C���f�[�^���X�v���C�g�Ƃ��Đݒ�(0�Ԃ���6��)
	set_sprite_data(6,16, HinaTiles01);//�q�i�i�ʏ�j

	set_bkg_data(0, 7,TestTileLabel); // �^�C���f�[�^��BG�Ƃ��Đݒ�(�O�ǁE�u���b�N�E���X�̃^�C��)

	set_bkg_tiles(0, 0, 32, 20, FieldTileLabel); //�}�b�v�f�[�^���Z�b�g(�Z�b�g�J�nx���W,�Z�b�g�J�ny���W,�Z�b�g��,�Z�b�g����)

	my_init_var();// �e�g���X������
	view_hina();//�q�i�\��

	SHOW_BKG;// BG��\��
	SHOW_SPRITES;// �X�v���C�g��\�� 
	DISPLAY_ON;// ��ʑS�̂�\��
	// set_bkg_data(0, 2,TestTileLabel); // �^�C���f�[�^��BG�Ƃ��Đݒ�

	disable_interrupts();// ��������
	add_VBL(vbl_isr);// VBL�����ɒǉ�
	enable_interrupts();// �����L��
	set_interrupts(VBL_IFLAG);// VBL�����Z�b�g

	snd_reg_init();// �T�E���h���W�X�^������
	snd_track_init();// �T�E���h�g���b�N������

	// while (1) {// �������[�v
	/*
	if(joypad() == J_START){
		 enable_interrupts();// �����L��
	}*/
	// }

}
