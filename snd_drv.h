#include <gb.h>

// �g�`�����������̃��W�X�^��`
#define VW30_REG	(*(UBYTE *)0xFF30)	/* Sound register */
#define VW31_REG	(*(UBYTE *)0xFF31)	/* Sound register */
#define VW32_REG	(*(UBYTE *)0xFF32)	/* Sound register */
#define VW33_REG	(*(UBYTE *)0xFF33)	/* Sound register */
#define VW34_REG	(*(UBYTE *)0xFF34)	/* Sound register */
#define VW35_REG	(*(UBYTE *)0xFF35)	/* Sound register */
#define VW36_REG	(*(UBYTE *)0xFF36)	/* Sound register */
#define VW37_REG	(*(UBYTE *)0xFF37)	/* Sound register */
#define VW38_REG	(*(UBYTE *)0xFF38)	/* Sound register */
#define VW39_REG	(*(UBYTE *)0xFF39)	/* Sound register */
#define VW3A_REG	(*(UBYTE *)0xFF3A)	/* Sound register */
#define VW3B_REG	(*(UBYTE *)0xFF3B)	/* Sound register */
#define VW3C_REG	(*(UBYTE *)0xFF3C)	/* Sound register */
#define VW3D_REG	(*(UBYTE *)0xFF3D)	/* Sound register */
#define VW3E_REG	(*(UBYTE *)0xFF3E)	/* Sound register */
#define VW3F_REG	(*(UBYTE *)0xFF3F)	/* Sound register */

#define SND_TRACK_MAX  5
#define SND_FLAG_VW    0x01

/*
// �`�����l������
NR52_REG = 0x8F;// b7:�S�`�����l��ON=1 / b3-0:�`�����l��4�`1�Đ��t���OON=F(1111)
NR50_REG = 0x77;// b7:��VinOF=0F / b6-4:������=7 / b3:�EVinOFF=0 / b2-0:�E����=7
NR51_REG = 0xFF;// b7-4:�`�����l��4�`1���o��ON=F(1111) / b3-0:�`�����l��4�`1�E�o��ON=F(1111)
// �`�����l��1(��`�g�X�C�[�v����)
NR10_REG = 0x00;// b6-4:�X�C�[�v����=0 / b3:�X�C�[�v����=0(��) / b2-0:�X�C�[�v�ω���=0
NR11_REG = 0xC0;// b7-6:�f���[�e�B��=3 / b5-0:�����J�E���^=0
NR12_REG = 0xF0;// b7-4:��������=15 / b3:�G���x���[�v����=0(��) / b2-0:�G���x���[�v�P�ʎ���=0
NR13_REG = 0x0A;// b7-0:���g��(11�r�b�g�̉���8�r�b�g)
NR14_REG = 0x86;// b7:�J�n�t���O=1 / b6:�J�E���^�Đ��t���O=0 / b2-0:���g��(11�r�b�g�̏��3�r�b�g)
// �`�����l��2(��`�g)
NR21_REG = 0xC0;// b7-6:�f���[�e�B��=3 / b5-0:�����J�E���^=0
NR22_REG = 0xF0;// b7-4:��������=15 / b3:�G���x���[�v����=0(��) / b2-0:�G���x���[�v�P�ʎ���=0
NR23_REG = 0xB2;// b7-0:���g��(11�r�b�g�̉���8�r�b�g)
NR24_REG = 0x86;// b7:�J�n�t���O=1 / b6:�J�E���^�Đ��t���O=0 / b2-0:���g��(11�r�b�g�̏��3�r�b�g)
// �`�����l��3(�g�`����������)
NR30_REG = 0x80;// b7:�o��ON=1
NR31_REG = 0x00;// b7-0:����=0
NR32_REG = 0x20;// b6-5:���� 100%�o��=1 ���~���[�g(0%�o��)=0�A50%�o��=2�A25%�o��=3
NR33_REG = 0x72;// b7-0:���g��(11�r�b�g�̉���8�r�b�g)
NR34_REG = 0x86;// b7:�J�n�t���O=1 / b6:�J�E���^�Đ��t���O=0 / b2-0:���g��(11�r�b�g�̏��3�r�b�g)
// �`�����l��4(�m�C�Y)
NR41_REG = 0x00;// b5-0:����=0
NR42_REG = 0xF3;// b7-4:��������=15 / b3:�G���x���[�v����=0(��) / b2-0:�G���x���[�v�P�ʎ���=3
NR43_REG = 0x33;// b7-4:���g���V�t�g��=3 / b3:�J�E���^�P��(0:15/1:7)=0 / b2-0:���g��=7
NR44_REG = 0x80;// b7:�J�n�t���O=1 / b6:�J�E���^�Đ��t���O=0
*/

struct st_track {
	UBYTE ch;   // �`�����l��(1�`4)
	UBYTE play; // �Đ��t���O
	UBYTE on;   // ON/OFF�t���O(����=1�A�x��=0)
	UBYTE vol;  // ����(0�`15)���`�����l��3�̔g�`������������0�`3(�~���[�g(0%�o��=0�A100%�o��=1�A50%�o��=2�A25%�o��=3)
	UBYTE duty; // �f���[�e�B��(0�`3)
	UBYTE pan;  // �p���|�b�g(0�`3)����=0�A��=1�A�E=2�A����=3
	UBYTE tmp;  // �e���|(1�`8)
	UBYTE len;  // �f�t�H���g����
	UBYTE cnt;  // �J�E���^
	UBYTE oct;  // �I�N�^�[�u(0�`5)
	UBYTE scl;  // ���K(0�`11)C,C+,D,D+,E,F,F+,G,G+,A,A+,B
	UBYTE note; // �I�N�^�[�u�~���K(0�`71)
	UBYTE flag; // �t���O b0:�g�`�������]���t���O
	UBYTE *pp;  // �Đ��|�C���^
	UBYTE *lp;  // ���[�v�|�C���^
	UBYTE *rp1; // ���s�[�g1�|�C���^
	UBYTE rpc1; // ���s�[�g1�J�E���^
	UBYTE *rp2; // ���s�[�g2�|�C���^
	UBYTE rpc2; // ���s�[�g2�J�E���^
	UBYTE detn; // �f�`���[���l
	UBYTE lfo;  // LFO���x
	UBYTE lfoc; // LFO�J�E���^
	UBYTE vibm; // �r�u���[�g�ő�l
	UBYTE vibe; // �r�u���[�g���ݒl
	UBYTE mute; // �~���[�g�t���O
	UBYTE env;  // �G���x���[�v
	UBYTE swep; // �X�C�[�v
	UWORD freq; // ���g��
};

// �T�E���h�R�}���h
#define SNDENV		0xEE // 0xEE�F�G���x���[�v�w��
						 // 	, �ݒ�l
#define SNDSWEEP	0xEF // 0xEF�F�X�C�[�v�w��
						 // 	, �ݒ�l
#define SNDTONE		0xF0 // 0xF0�F���F�w��
						 // 	, ���F�ԍ�
#define SNDREP1ST	0xF1 // 0xF1�F���s�[�g1�J�n���J�E���^������
#define SNDREP1ED	0xF2 // 0xF2�F���s�[�g1�I�����J�E���^�C���N�������g������
						 // 	, ���s�[�g��
#define SNDREP2ST	0xF3 // 0xF3�F���s�[�g2�J�n���J�E���^������
#define SNDREP2ED	0xF4 // 0xF4�F���s�[�g2�I�����J�E���^�C���N�������g������
						 // 	, ���s�[�g��
#define SNDLOOPST	0xF5 // 0xF5�F���[�v�J�n�ʒu���A�h���X�L�^
#define SNDLOOPED	0xF6 // 0xF6�F���[�v�I���ʒu���A�h���X�W�����v
#define SNDTEMPO	0xF7 // 0xF7�F�e���|�w��
 						 // 	, �e���|�ԍ��i0�`9:�e���|�l�j
#define SNDTIE		0xF8 // 0xF8�F�^�C�w��
						 // 	, ����
#define SNDVOL		0xF9 // 0xF9�F���ʎw��
						 // 	, �ݒ�l�i0�`15�j
#define SNDDETUNE	0xFA // 0xFA�F�f�`���[���w��
						 // 	, �ݒ�l
#define SNDVIBE		0xFB // 0xFB�F�r�u���[�g�w��
						 // 	, �ݒ�l
						 // 	, �ǉ��Đ�����
#define SNDLFO		0xFC // 0xFC�FLFO���x�w��
						 // 	, �ݒ�l
#define SNDLONG		0xFD // 0xFD�F�ȗ������w��
						 // �i	, SNDTEMPO,�e���|�ԍ��i0�`9:�e���|�l�j�j
						 // 	, �����ԍ�
#define SNDPAN		0xFE // 0xFE�F�p���|�b�g�w��
						 // 	, �ݒ�l
#define SNDEND		0xFF // 0xFF�F�I��

#define SNDLEN32	0x70
#define SNDLEN16	0x71
#define SNDLEN16F	0x72
#define SNDLEN8		0x73
#define SNDLEN8F	0x74
#define SNDLEN4		0x75
#define SNDLEN4F	0x76
#define SNDLEN2		0x77
#define SNDLEN2F	0x78
#define SNDLEN1		0x79

#define NL	0x00

#define CN0	0x01
#define CS0	0x02
#define DN0	0x03
#define DS0	0x04
#define EN0	0x05
#define FN0	0x06
#define FS0	0x07
#define GN0	0x08
#define GS0	0x09
#define AN0	0x0A
#define AS0	0x0B
#define BN0	0x0C

#define CN1	0x0D
#define CS1	0x0E
#define DN1	0x0F
#define DS1	0x10
#define EN1	0x11
#define FN1	0x12
#define FS1	0x13
#define GN1	0x14
#define GS1	0x15
#define AN1	0x16
#define AS1	0x17
#define BN1	0x18

#define CN2	0x19
#define CS2	0x1A
#define DN2	0x1B
#define DS2	0x1C
#define EN2	0x1D
#define FN2	0x1E
#define FS2	0x1F
#define GN2	0x20
#define GS2	0x21
#define AN2	0x22
#define AS2	0x23
#define BN2	0x24

#define CN3	0x25
#define CS3	0x26
#define DN3	0x27
#define DS3	0x28
#define EN3	0x29
#define FN3	0x2A
#define FS3	0x2B
#define GN3	0x2C
#define GS3	0x2D
#define AN3	0x2E
#define AS3	0x2F
#define BN3	0x30

#define CN4	0x31
#define CS4	0x32
#define DN4	0x33
#define DS4	0x34
#define EN4	0x35
#define FN4	0x36
#define FS4	0x37
#define GN4	0x38
#define GS4	0x39
#define AN4	0x3A
#define AS4	0x3B
#define BN4	0x3C

#define CN5	0x3D
#define CS5	0x3E
#define DN5	0x3F
#define DS5	0x40
#define EN5	0x41
#define FN5	0x42
#define FS5	0x43
#define GN5	0x44
#define GS5	0x45
#define AN5	0x46
#define AS5	0x47
#define BN5	0x48

#define CN6	0x49
