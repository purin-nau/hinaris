#include <gb.h>

// 波形メモリ音源のレジスタ定義
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
// チャンネル制御
NR52_REG = 0x8F;// b7:全チャンネルON=1 / b3-0:チャンネル4～1再生フラグON=F(1111)
NR50_REG = 0x77;// b7:左VinOF=0F / b6-4:左音量=7 / b3:右VinOFF=0 / b2-0:右音量=7
NR51_REG = 0xFF;// b7-4:チャンネル4～1左出力ON=F(1111) / b3-0:チャンネル4～1右出力ON=F(1111)
// チャンネル1(矩形波スイープあり)
NR10_REG = 0x00;// b6-4:スイープ時間=0 / b3:スイープ方向=0(上) / b2-0:スイープ変化量=0
NR11_REG = 0xC0;// b7-6:デューティ比=3 / b5-0:音長カウンタ=0
NR12_REG = 0xF0;// b7-4:初期音量=15 / b3:エンベロープ増減=0(減) / b2-0:エンベロープ単位時間=0
NR13_REG = 0x0A;// b7-0:周波数(11ビットの下位8ビット)
NR14_REG = 0x86;// b7:開始フラグ=1 / b6:カウンタ再生フラグ=0 / b2-0:周波数(11ビットの上位3ビット)
// チャンネル2(矩形波)
NR21_REG = 0xC0;// b7-6:デューティ比=3 / b5-0:音長カウンタ=0
NR22_REG = 0xF0;// b7-4:初期音量=15 / b3:エンベロープ増減=0(減) / b2-0:エンベロープ単位時間=0
NR23_REG = 0xB2;// b7-0:周波数(11ビットの下位8ビット)
NR24_REG = 0x86;// b7:開始フラグ=1 / b6:カウンタ再生フラグ=0 / b2-0:周波数(11ビットの上位3ビット)
// チャンネル3(波形メモリ音源)
NR30_REG = 0x80;// b7:出力ON=1
NR31_REG = 0x00;// b7-0:音長=0
NR32_REG = 0x20;// b6-5:音量 100%出力=1 ※ミュート(0%出力)=0、50%出力=2、25%出力=3
NR33_REG = 0x72;// b7-0:周波数(11ビットの下位8ビット)
NR34_REG = 0x86;// b7:開始フラグ=1 / b6:カウンタ再生フラグ=0 / b2-0:周波数(11ビットの上位3ビット)
// チャンネル4(ノイズ)
NR41_REG = 0x00;// b5-0:音長=0
NR42_REG = 0xF3;// b7-4:初期音量=15 / b3:エンベロープ増減=0(減) / b2-0:エンベロープ単位時間=3
NR43_REG = 0x33;// b7-4:周波数シフト量=3 / b3:カウンタ単位(0:15/1:7)=0 / b2-0:周波数=7
NR44_REG = 0x80;// b7:開始フラグ=1 / b6:カウンタ再生フラグ=0
*/

struct st_track {
	UBYTE ch;   // チャンネル(1～4)
	UBYTE play; // 再生フラグ
	UBYTE on;   // ON/OFFフラグ(音符=1、休符=0)
	UBYTE vol;  // 音量(0～15)※チャンネル3の波形メモリ音源は0～3(ミュート(0%出力=0、100%出力=1、50%出力=2、25%出力=3)
	UBYTE duty; // デューティ比(0～3)
	UBYTE pan;  // パンポット(0～3)消音=0、左=1、右=2、両方=3
	UBYTE tmp;  // テンポ(1～8)
	UBYTE len;  // デフォルト音長
	UBYTE cnt;  // カウンタ
	UBYTE oct;  // オクターブ(0～5)
	UBYTE scl;  // 音階(0～11)C,C+,D,D+,E,F,F+,G,G+,A,A+,B
	UBYTE note; // オクターブ×音階(0～71)
	UBYTE flag; // フラグ b0:波形メモリ転送フラグ
	UBYTE *pp;  // 再生ポインタ
	UBYTE *lp;  // ループポインタ
	UBYTE *rp1; // リピート1ポインタ
	UBYTE rpc1; // リピート1カウンタ
	UBYTE *rp2; // リピート2ポインタ
	UBYTE rpc2; // リピート2カウンタ
	UBYTE detn; // デチューン値
	UBYTE lfo;  // LFO速度
	UBYTE lfoc; // LFOカウンタ
	UBYTE vibm; // ビブラート最大値
	UBYTE vibe; // ビブラート現在値
	UBYTE mute; // ミュートフラグ
	UBYTE env;  // エンベロープ
	UBYTE swep; // スイープ
	UWORD freq; // 周波数
};

// サウンドコマンド
#define SNDENV		0xEE // 0xEE：エンベロープ指定
						 // 	, 設定値
#define SNDSWEEP	0xEF // 0xEF：スイープ指定
						 // 	, 設定値
#define SNDTONE		0xF0 // 0xF0：音色指定
						 // 	, 音色番号
#define SNDREP1ST	0xF1 // 0xF1：リピート1開始＆カウンタ初期化
#define SNDREP1ED	0xF2 // 0xF2：リピート1終了＆カウンタインクリメント＆判定
						 // 	, リピート回数
#define SNDREP2ST	0xF3 // 0xF3：リピート2開始＆カウンタ初期化
#define SNDREP2ED	0xF4 // 0xF4：リピート2終了＆カウンタインクリメント＆判定
						 // 	, リピート回数
#define SNDLOOPST	0xF5 // 0xF5：ループ開始位置＆アドレス記録
#define SNDLOOPED	0xF6 // 0xF6：ループ終了位置＆アドレスジャンプ
#define SNDTEMPO	0xF7 // 0xF7：テンポ指定
 						 // 	, テンポ番号（0～9:テンポ値）
#define SNDTIE		0xF8 // 0xF8：タイ指定
						 // 	, 音長
#define SNDVOL		0xF9 // 0xF9：音量指定
						 // 	, 設定値（0～15）
#define SNDDETUNE	0xFA // 0xFA：デチューン指定
						 // 	, 設定値
#define SNDVIBE		0xFB // 0xFB：ビブラート指定
						 // 	, 設定値
						 // 	, 追加再生音長
#define SNDLFO		0xFC // 0xFC：LFO速度指定
						 // 	, 設定値
#define SNDLONG		0xFD // 0xFD：省略音長指定
						 // （	, SNDTEMPO,テンポ番号（0～9:テンポ値））
						 // 	, 音長番号
#define SNDPAN		0xFE // 0xFE：パンポット指定
						 // 	, 設定値
#define SNDEND		0xFF // 0xFF：終了

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
