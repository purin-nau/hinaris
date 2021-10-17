#include "snd_drv.h"

// トラック
struct st_track snd_track[SND_TRACK_MAX];
UBYTE se_busy, se_pri;
void snd_ch_on_off(struct st_track *snd_trk);

// 音程ごとの周波数
UWORD scale_frequency[] = {0,
	// C    C+   D    D+   E    F    F+   G    G+   A    A+   B // Oct
	  44, 156, 262, 363, 457, 547, 631, 710, 786, 854, 923, 986,// 0
	1046,1102,1155,1205,1253,1297,1339,1379,1417,1452,1486,1517,// 1
	1546,1575,1602,1627,1650,1673,1694,1714,1732,1750,1767,1783,// 2
	1798,1812,1825,1837,1849,1860,1871,1881,1890,1899,1907,1915,// 3
	1923,1930,1936,1943,1949,1954,1959,1964,1969,1974,1978,1982,// 4
	1985,1988,1992,1995,1998,2001,2004,2006,2009,2011,2013,2015,// 5
	2017
};

// テンポごとの音長
UBYTE tempo_length[8][10] = {
	// 32    16    16.    8     8.    4     4.    2     2.    1    // Tempo
	{   1-1,  2-1,  3-1,  4-1,  6-1,  8-1, 12-1, 16-1, 24-1, 32-1},// 1
	{   2-1,  4-1,  6-1,  8-1, 12-1, 16-1, 24-1, 32-1, 48-1, 64-1},// 2
	{   3-1,  6-1,  9-1, 12-1, 18-1, 24-1, 36-1, 48-1, 72-1, 96-1},// 3
	{   4-1,  8-1, 12-1, 16-1, 24-1, 32-1, 48-1, 64-1, 96-1,128-1},// 4
	{   5-1, 10-1, 15-1, 20-1, 30-1, 40-1, 60-1, 80-1,120-1,160-1},// 5
	{   6-1, 12-1, 18-1, 24-1, 36-1, 48-1, 72-1, 96-1,144-1,192-1},// 6
	{   7-1, 14-1, 21-1, 28-1, 42-1, 56-1, 84-1,112-1,168-1,224-1},// 7
	{   8-1, 16-1, 24-1, 32-1, 48-1, 64-1, 96-1,128-1,182-1,256-1},// 8
};

// チャンネルごとのパンポット設定
UBYTE ch_panpot[4][4] = {
	// 消音  左    右    両方
	{  0x00, 0x10, 0x01, 0x11},// ch1
	{  0x00, 0x20, 0x02, 0x22},// ch2
	{  0x00, 0x40, 0x04, 0x44},// ch3
	{  0x00, 0x80, 0x08, 0x88},// ch4
};

// 音程ごとのノイズ設定(NR42(下位4ビット),NR43)
UWORD scale_noise[] = {0,
	//  C       C+      D       D+      E       F       F+      G       G+      A       A+      B  // Oct
	0x0016, 0x0026, 0x0036, 0x0046, 0x0043, 0x0053, 0x0063, 0x0073, 0x0070, 0x0080, 0x0090, 0x00A0,// 1
	0x0116, 0x0126, 0x0136, 0x0146, 0x0143, 0x0153, 0x0163, 0x0173, 0x0170, 0x0180, 0x0190, 0x01A0,// 2
	0x0216, 0x0226, 0x0236, 0x0246, 0x0243, 0x0253, 0x0263, 0x0273, 0x0270, 0x0280, 0x0290, 0x02A0,// 3
	0x0316, 0x0326, 0x0336, 0x0346, 0x0343, 0x0353, 0x0363, 0x0373, 0x0370, 0x0380, 0x0390, 0x03A0,// 4
	0x0416, 0x0426, 0x0436, 0x0446, 0x0443, 0x0453, 0x0463, 0x0473, 0x0470, 0x0480, 0x0490, 0x04A0,// 5
	0x0516, 0x0526, 0x0536, 0x0546, 0x0543, 0x0553, 0x0563, 0x0573, 0x0570, 0x0580, 0x0590, 0x05A0,// 6
	0x0616, 0x0626, 0x0636, 0x0646, 0x0643, 0x0653, 0x0663, 0x0673, 0x0670, 0x0680, 0x0690, 0x06A0,// 7
	0x0716, 0x0726, 0x0736, 0x0746, 0x0743, 0x0753, 0x0763, 0x0773, 0x0770, 0x0780, 0x0790, 0x07A0,// 8
};

// 波形メモリ音源のプリセット
UBYTE voluntary_wave[4][16] = {
	// 
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},// 矩形波
	{ 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF},// ノコギリ波
	{ 0x0b, 0xde, 0xef, 0xff, 0xff, 0xfe, 0xed, 0xb0, 0x04, 0x21, 0x10, 0x00, 0x00, 0x01, 0x12, 0x40},// 正弦波
	{ 0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF, 0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10},// 三角波
//	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},// 
};

// サウンド関連の全レジスタを初期化
void snd_reg_init()
{
	NR10_REG = 0x08;
	NR11_REG = 0;
	NR12_REG = 0;
	NR13_REG = 0;
	NR14_REG = 0;
	NR21_REG = 0;
	NR22_REG = 0;
	NR23_REG = 0;
	NR24_REG = 0;
	NR30_REG = 0;
	NR31_REG = 0;
	NR32_REG = 0;
	NR33_REG = 0;
	NR34_REG = 0;
	NR41_REG = 0;
	NR42_REG = 0;
	NR43_REG = 0;
	NR44_REG = 0;
	NR50_REG = 0;
	NR51_REG = 0;
	NR52_REG = 0;
}

// トラックデータの初期化
void snd_track_init()
{
	UBYTE i;
	for (i = 0; i != SND_TRACK_MAX; i++) {
		snd_track[i].ch = i+1; // チャンネル(1～4)
		snd_track[i].play = 0; // 再生フラグ
		snd_track[i].on   = 0; // ON/OFFフラグ
		snd_track[i].vol  = 0; // 音量(0～15)※チャンネル3の波形メモリ音源は0～3(ミュート(0%出力=0、100%出力=1、50%出力=2、25%出力=3)
		snd_track[i].duty = 0; // デューティ比(0～3)
		snd_track[i].pan  = 0; // パンポット(0～3)消音=0、左=1、右=2、両方=3
		snd_track[i].tmp  = 0; // テンポ(1～8)
		snd_track[i].len  = 0; // デフォルト音長
		snd_track[i].cnt  = 0; // カウンタ
		snd_track[i].oct  = 0; // オクターブ(0～5)
		snd_track[i].scl  = 0; // 音階(0～11)C,C+,D,D+,E,F,F+,G,G+,A,A+,B
		snd_track[i].note = 0; // オクターブ×音階(0～71)
		snd_track[i].flag = 0; // フラグ b0:波形メモリ転送フラグ
		snd_track[i].pp   = 0; // 再生ポインタ
		snd_track[i].lp   = 0; // ループポインタ
		snd_track[i].rp1  = 0; // リピート1ポインタ
		snd_track[i].rpc1 = 0; // リピート1カウンタ
		snd_track[i].rp2  = 0; // リピート2ポインタ
		snd_track[i].rpc2 = 0; // リピート2カウンタ
		snd_track[i].detn = 0; // デチューン値
		snd_track[i].lfo  = 0; // LFO速度
		snd_track[i].lfoc = 0; // LFOカウンタ
		snd_track[i].vibm = 0; // ビブラート最大値
		snd_track[i].vibe = 0; // ビブラート現在値
		snd_track[i].mute = 0; // ミュートフラグ
		snd_track[i].env  = 0; // エンベロープ
		snd_track[i].swep = 0x08; // スイープ
		snd_track[i].freq = 0; // 周波数
	}
	// SE用
	se_busy = se_pri = 0;
}

// BGMリクエスト
void snd_req_bgm(UBYTE *ch1, UBYTE *ch2, UBYTE *ch3, UBYTE *ch4)
{
	if (ch1 != 0) {
		snd_track[0].ch   = 1;
		snd_track[0].play = 1;
		snd_track[0].on   = 0;
		snd_track[0].vol  = 15;
		snd_track[0].duty = 0;
		snd_track[0].pan  = 3;
		snd_track[0].tmp  = 4;
		snd_track[0].len  = 5;
		snd_track[0].cnt  = 0;
		snd_track[0].oct  = 3;
		snd_track[0].scl  = 0;
		snd_track[0].note = 0;
		snd_track[0].flag = 0;
		snd_track[0].pp   = ch1;
		snd_track[0].lp   = 0;
		snd_track[0].rp1  = 0;
		snd_track[0].rpc1 = 0;
		snd_track[0].rp2  = 0;
		snd_track[0].rpc2 = 0;
		snd_track[0].detn = 0;
		snd_track[0].lfo  = 0;
		snd_track[0].lfoc = 0;
		snd_track[0].vibm = 0;
		snd_track[0].vibe = 0;
		snd_track[0].mute = 0;
		snd_track[0].env  = 0;
		snd_track[0].swep = 0x08;
		snd_track[0].freq = 0;
	}
	if (ch2 != 0) {
		snd_track[1].ch   = 2;
		snd_track[1].play = 1;
		snd_track[1].on   = 0;
		snd_track[1].vol  = 15;
		snd_track[1].duty = 0;
		snd_track[1].pan  = 3;
		snd_track[1].tmp  = 4;
		snd_track[1].len  = 5;
		snd_track[1].cnt  = 0;
		snd_track[1].oct  = 3;
		snd_track[1].scl  = 0;
		snd_track[1].note = 0;
		snd_track[1].flag = 0;
		snd_track[1].pp   = ch2;
		snd_track[1].lp   = 0;
		snd_track[1].rp1  = 0;
		snd_track[1].rpc1 = 0;
		snd_track[1].rp2  = 0;
		snd_track[1].rpc2 = 0;
		snd_track[1].detn = 0;
		snd_track[1].lfo  = 0;
		snd_track[1].lfoc = 0;
		snd_track[1].vibm = 0;
		snd_track[1].vibe = 0;
		snd_track[1].mute = 0;
		snd_track[1].env  = 0;
		snd_track[1].swep = 0;
		snd_track[1].freq = 0;
	}
	if (ch3 != 0) {
		snd_track[2].ch   = 3;
		snd_track[2].play = 1;
		snd_track[2].on   = 0;
		snd_track[2].vol  = 15;
		snd_track[2].duty = 0;
		snd_track[2].pan  = 3;
		snd_track[2].tmp  = 4;
		snd_track[2].len  = 5;
		snd_track[2].cnt  = 0;
		snd_track[2].oct  = 3;
		snd_track[2].scl  = 0;
		snd_track[2].note = 0;
		snd_track[2].flag = 0;
		snd_track[2].pp   = ch3;
		snd_track[2].lp   = 0;
		snd_track[2].rp1  = 0;
		snd_track[2].rpc1 = 0;
		snd_track[2].rp2  = 0;
		snd_track[2].rpc2 = 0;
		snd_track[2].detn = 0;
		snd_track[2].lfo  = 0;
		snd_track[2].lfoc = 0;
		snd_track[2].vibm = 0;
		snd_track[2].vibe = 0;
		snd_track[2].mute = 0;
		snd_track[2].env  = 0;
		snd_track[2].swep = 0;
		snd_track[2].freq = 0;
	}
	if (ch4 != 0) {
		snd_track[3].ch   = 4;
		snd_track[3].play = 1;
		snd_track[3].on   = 0;
		snd_track[3].vol  = 15;
		snd_track[3].duty = 0;
		snd_track[3].pan  = 3;
		snd_track[3].tmp  = 4;
		snd_track[3].len  = 5;
		snd_track[3].cnt  = 0;
		snd_track[3].oct  = 3;
		snd_track[3].scl  = 0;
		snd_track[3].note = 0;
		snd_track[3].flag = 0;
		snd_track[3].pp   = ch4;
		snd_track[3].lp   = 0;
		snd_track[3].rp1  = 0;
		snd_track[3].rpc1 = 0;
		snd_track[3].rp2  = 0;
		snd_track[3].rpc2 = 0;
		snd_track[3].detn = 0;
		snd_track[3].lfo  = 0;
		snd_track[3].lfoc = 0;
		snd_track[3].vibm = 0;
		snd_track[3].vibe = 0;
		snd_track[3].mute = 0;
		snd_track[3].env  = 0;
		snd_track[3].swep = 0;
		snd_track[3].freq = 0;
	}

	// チャンネル制御
	// b7:全チャンネルON=1 / b3-0:チャンネル4～1再生中フラグは読込のみ
	NR52_REG = 0x80;
	// b7:左VinOF=0F / b6-4:左音量=7 / b3:右VinOFF=0 / b2-0:右音量=7
	NR50_REG = 0x77;
	// b7-4:チャンネル4～1左出力ON=F(1111) / b3-0:チャンネル4～1右出力ON=F(1111)
	NR51_REG = 0x00;
}

// SEリクエスト
void snd_req_se(UBYTE *se_data, UBYTE ch, UBYTE pri)
{
	struct st_track *snd_trk;
	if (se_pri <= pri) {
		// 直前に鳴っていたSEをオフにする
		snd_trk = &snd_track[4];
		snd_trk->on = 0;
		snd_ch_on_off(snd_trk);

		snd_track[4].ch   = ch;
		snd_track[4].play = 1;
		snd_track[4].on   = 0;
		snd_track[4].vol  = 15;
		snd_track[4].duty = 0;
		snd_track[4].pan  = 3;
		snd_track[4].tmp  = 4;
		snd_track[4].len  = 5;
		snd_track[4].cnt  = 0;
		snd_track[4].oct  = 3;
		snd_track[4].scl  = 0;
		snd_track[4].note = 0;
		snd_track[4].flag = 0;
		snd_track[4].pp   = se_data;
		snd_track[4].lp   = 0;
		snd_track[4].rp1  = 0;
		snd_track[4].rpc1 = 0;
		snd_track[4].rp2  = 0;
		snd_track[4].rpc2 = 0;
		snd_track[4].detn = 0;
		snd_track[4].lfo  = 0;
		snd_track[4].lfoc = 0;
		snd_track[4].vibm = 0;
		snd_track[4].vibe = 0;
		snd_track[4].mute = 0;
		snd_track[4].env  = 0;
		snd_track[4].swep = 0x08;
		snd_track[4].freq = 0;
		// 対象チャンネルのBGMトラックをミュートする
		se_busy = ch;
		snd_track[0].mute = 0;
		snd_track[1].mute = 0;
		snd_track[2].mute = 0;
		snd_track[3].mute = 0;
		snd_track[ch-1].mute = 1;
		se_pri = pri;
	}
}


void snd_voluntary_wave_set(UBYTE num) {
	VW30_REG = voluntary_wave[num][0];
	VW31_REG = voluntary_wave[num][1];
	VW32_REG = voluntary_wave[num][2];
	VW33_REG = voluntary_wave[num][3];
	VW34_REG = voluntary_wave[num][4];
	VW35_REG = voluntary_wave[num][5];
	VW36_REG = voluntary_wave[num][6];
	VW37_REG = voluntary_wave[num][7];
	VW38_REG = voluntary_wave[num][8];
	VW39_REG = voluntary_wave[num][9];
	VW3A_REG = voluntary_wave[num][10];
	VW3B_REG = voluntary_wave[num][11];
	VW3C_REG = voluntary_wave[num][12];
	VW3D_REG = voluntary_wave[num][13];
	VW3E_REG = voluntary_wave[num][14];
	VW3F_REG = voluntary_wave[num][15];
}

void snd_ch_on_off(struct st_track *snd_trk)
{
	UBYTE tmp;
	UBYTE tie;
	UBYTE nr51;
	UWORD freq; // 周波数
	
	// チャンネル1(矩形波スイープあり)
	if (snd_trk->ch == 1) {
		if (snd_trk->on == 1) {
			// b6-4:スイープ時間=0 / b3:スイープ方向=0(上) / b2-0:スイープ変化量=0
			NR10_REG = snd_trk->swep;
			// b7-6:デューティ比=3 / b5-0:音長カウンタ=0
			NR11_REG = snd_trk->duty << 6;
			// b7-4:初期音量=15 / b3:エンベロープ増減=0(減) / b2-0:エンベロープ単位時間=0
			NR12_REG = (snd_trk->vol << 4) + snd_trk->env;
			// b7-0:周波数(11ビットの下位8ビット)
			NR13_REG = (UBYTE)(snd_trk->freq & 0x00FF);
			// b7:開始フラグ=1 / b6:カウンタ再生フラグ=0 / b2-0:周波数(11ビットの上位3ビット)
			NR14_REG = (UBYTE)((snd_trk->freq >> 8) & 0x00FF) + 0x80;
			// チャンネル毎の左右オン
			tmp = ch_panpot[snd_trk->ch-1][snd_trk->pan];
			nr51 = NR51_REG;
			NR51_REG = (nr51 & 0xee) ^ tmp;
		} else {
			NR13_REG = 0;
			NR14_REG = 0;
			// チャンネル毎の左右オフ
			nr51 = NR51_REG;
			NR51_REG = (nr51 & 0xee);
		}
	}
	// チャンネル2(矩形波)
	if (snd_trk->ch == 2) {
		if (snd_trk->on == 1) {
			// b7-6:デューティ比=3 / b5-0:音長カウンタ=0
			NR21_REG = snd_trk->duty << 6;
			// b7-4:初期音量=15 / b3:エンベロープ増減=0(減) / b2-0:エンベロープ単位時間=0
			NR22_REG = (snd_trk->vol << 4) + snd_trk->env;
			// b7-0:周波数(11ビットの下位8ビット)
			NR23_REG = (UBYTE)(snd_trk->freq & 0x00FF);
			// b7:開始フラグ=1 / b6:カウンタ再生フラグ=0 / b2-0:周波数(11ビットの上位3ビット)
			NR24_REG = (UBYTE)((snd_trk->freq >> 8) & 0x00FF) + 0x80;
			// チャンネル毎の左右オン
			tmp = ch_panpot[snd_trk->ch-1][snd_trk->pan];
			nr51 = NR51_REG;
			NR51_REG = (nr51 & 0xdd) ^ tmp;
		} else {
			NR23_REG = 0;
			NR24_REG = 0;
			// チャンネル毎の左右オフ
			nr51 = NR51_REG;
			NR51_REG = (nr51 & 0xdd);
		}
	}
	// チャンネル3(波形メモリ音源)
	if (snd_trk->ch == 3) {
		if (snd_trk->on == 1) {
			// 波形メモリ転送フラグ
			if (snd_trk->flag & SND_FLAG_VW) {
				tmp = SND_FLAG_VW ^ 0xFF;
				snd_trk->flag &= tmp;
				snd_voluntary_wave_set(snd_trk->duty);
			}
			// b7:出力ON=1
			NR30_REG = 0x80;
			// b7-0:音長=0
			NR31_REG = 0x00;
			// b6-5:音量 100%出力=1 ※ミュート(0%出力)=0、50%出力=2、25%出力=3
			//NR32_REG = snd_trk->vol << 5;
			tmp = 0;
			if (snd_trk->vol > 0) {
				tmp = 0x60;
			}
			if (snd_trk->vol > 4) {
				tmp = 0x40;
			}
			if (snd_trk->vol > 8) {
				tmp = 0x20;
			}
			NR32_REG = tmp;
			// b7-0:周波数(11ビットの下位8ビット)
			NR33_REG = (UBYTE)(snd_trk->freq & 0x00FF);
			// b7:開始フラグ=1 / b6:カウンタ再生フラグ=0 / b2-0:周波数(11ビットの上位3ビット)
			NR34_REG = (UBYTE)((snd_trk->freq >> 8) & 0x00FF) + 0x80;
			// チャンネル毎の左右オン
			tmp = ch_panpot[snd_trk->ch-1][snd_trk->pan];
			nr51 = NR51_REG;
			NR51_REG = (nr51 & 0xbb) ^ tmp;
		} else {
			NR30_REG = 0;
			NR32_REG = 0;
			NR33_REG = 0;
			NR34_REG = 0;
			// チャンネル毎の左右オフ
			nr51 = NR51_REG;
			NR51_REG = (nr51 & 0xbb);
		}
	}
	// チャンネル4(ノイズ)
	if (snd_trk->ch == 4) {
		if (snd_trk->on == 1) {
			// b5-0:音長=0
			NR41_REG = 0x00;
			// b7-4:初期音量 / b3:エンベロープ増減=0(減) / b2-0:エンベロープ単位時間
			tmp = (UBYTE)((snd_trk->freq >> 8) & 0x00FF);
			NR42_REG = tmp + (snd_trk->vol << 4);
			// b7-4:周波数シフト量 / b3:カウンタ単位(0:15/1:7)=0 / b2-0:周波数
			NR43_REG = (UBYTE)(snd_trk->freq & 0x00FF);
			// b7:開始フラグ=1 / b6:カウンタ再生フラグ=0
			NR44_REG = 0x80;
			// チャンネル毎の左右オン
			tmp = ch_panpot[snd_trk->ch-1][snd_trk->pan];
			nr51 = NR51_REG;
			NR51_REG = (nr51 & 0x77) ^ tmp;
		} else {
			NR42_REG = 0;
			NR43_REG = 0;
			NR44_REG = 0;
			// チャンネル毎の左右オフ
			nr51 = NR51_REG;
			NR51_REG = (nr51 & 0x77);
		}
	}
}

void snd_driver()
{
	struct st_track *snd_trk;
	UBYTE i;
	UBYTE mml;
	UBYTE note;
	UBYTE len;
	UBYTE tmp;
	UBYTE tie;
	UBYTE nr51;
	UWORD freq; // 周波数
	
	for (i = 0; i != SND_TRACK_MAX; i++)
	{
		snd_trk = &snd_track[i];
		// トラック再生中？
		if (snd_trk->play == 0) {
			continue;
		}
		// 音長あり？
		snd_trk->cnt--;
		if (snd_trk->cnt != 255) {
			// ビブラート処理
			if (snd_trk->vibm != 0) {
				snd_trk->lfoc--;
				if (snd_trk->lfoc == 255) {
					snd_trk->lfoc = snd_trk->lfo - 1;
					snd_trk->vibe ^= snd_trk->vibm;
					freq = snd_trk->freq + (UWORD)snd_trk->vibe;
					if (snd_trk->mute == 0) {
						// チャンネル1(矩形波スイープあり)
						if (snd_trk->ch == 1) {
							// b7-0:周波数(11ビットの下位8ビット)
							NR13_REG = (UBYTE)(freq & 0x00FF);
							// b7:開始フラグ=1 / b6:カウンタ再生フラグ=0 / b2-0:周波数(11ビットの上位3ビット)
							NR14_REG = (UBYTE)((freq >> 8) & 0x00FF) + 0x80;
						}
						// チャンネル2(矩形波)
						if (snd_trk->ch == 2) {
							// b7-0:周波数(11ビットの下位8ビット)
							NR23_REG = (UBYTE)(freq & 0x00FF);
							// b7:開始フラグ=1 / b6:カウンタ再生フラグ=0 / b2-0:周波数(11ビットの上位3ビット)
							NR24_REG = (UBYTE)((freq >> 8) & 0x00FF) + 0x80;
						}
						// チャンネル3(波形メモリ音源)
						if (snd_trk->ch == 3) {
							// b7-0:周波数(11ビットの下位8ビット)
							NR33_REG = (UBYTE)(freq & 0x00FF);
							// b7:開始フラグ=1 / b6:カウンタ再生フラグ=0 / b2-0:周波数(11ビットの上位3ビット)
							NR34_REG = (UBYTE)((freq >> 8) & 0x00FF) + 0x80;
						}
					}
				}
			}
			continue;
		}
		note = 0;
		tie = 0;
		len = snd_trk->len;
		while (note == 0) {
			mml = *snd_trk->pp++;
			if (mml == SNDEND) {
				if (snd_trk->lp != 0) {
					snd_trk->pp = snd_trk->lp;
					continue;
				} else {
					break;
				}
			}
			if (mml < 0x70) {
				// 音程0は休符、他は音程を直接指定する
				if (mml == 0) {
					snd_trk->scl = 0;
					snd_trk->on = 0;
				} else {
					snd_trk->scl = mml;
					snd_trk->on = 1;
				}
				note = 1;
			}
			if (mml == SNDTIE) {
				snd_trk->on = 1;
				tie = 1;
				note = 1;
			}
			if (note == 0) {
				// サウンドコマンド
				if ((mml & 0xf0) >= 0xe0) {
					if (mml == SNDLONG) {
						mml = *snd_trk->pp++;
						// 音長コマンド
						if ((mml & 0xf0) == 0x70) {
							len = snd_trk->len = (mml & 0x0f);
						}
						continue;
					}
					if (mml == SNDTEMPO) {
						snd_trk->tmp = *snd_trk->pp++;
						continue;
					}
					if (mml == SNDVOL) {
						snd_trk->vol = *snd_trk->pp++;
						if (snd_trk->vol > 15) {
							snd_trk->vol = 15;
						}
						continue;
					}
					if (mml == SNDTONE) {
						snd_trk->duty = *snd_trk->pp++;
						if (snd_trk->ch != 3) {
							if (snd_trk->duty > 3) {
								snd_trk->duty = 3;
							}
						} else {
							if (snd_trk->duty > 3) {
								snd_trk->duty = 3;
							}
							snd_trk->flag |= SND_FLAG_VW;
						}
						continue;
					}
					if (mml == SNDENV) {
						snd_trk->env = *snd_trk->pp++;
						continue;
					}
					if (mml == SNDSWEEP) {
						snd_trk->swep = *snd_trk->pp++;
						continue;
					}
					if (mml == SNDPAN) {
						snd_trk->pan = *snd_trk->pp++;
						if (snd_trk->pan > 3) {
							snd_trk->pan = 3;
						}
						continue;
					}
					if (mml == SNDLOOPST) {
						snd_trk->lp = snd_trk->pp;
						continue;
					}
					if (mml == SNDREP1ST) {
						snd_trk->rp1 = snd_trk->pp;
						snd_trk->rpc1 = 0;
						continue;
					}
					if (mml == SNDREP2ST) {
						snd_trk->rp2 = snd_trk->pp;
						snd_trk->rpc2 = 0;
						continue;
					}
					if (mml == SNDREP1ED) {
						snd_trk->rpc1++;
						mml = *snd_trk->pp++;
						if (snd_trk->rpc1 != mml) {
							snd_trk->pp = snd_trk->rp1;
						}
						continue;
					}
					if (mml == SNDREP2ED) {
						snd_trk->rpc2++;
						mml = *snd_trk->pp++;
						if (snd_trk->rpc2 != mml) {
							snd_trk->pp = snd_trk->rp2;
						}
						continue;
					}
					if (mml == SNDLOOPED) {
						if (snd_trk->lp != 0) {
							snd_trk->pp = snd_trk->lp;
							continue;
						} else {
							break;
						}
					}
					// 追加コマンド
					if (mml == SNDDETUNE) {
						snd_trk->detn = *snd_trk->pp++;
						continue;
					}
					if (mml == SNDLFO) {
						snd_trk->lfo = *snd_trk->pp++;
						continue;
					}
					if (mml == SNDVIBE) {
						snd_trk->vibm = *snd_trk->pp++;
						snd_trk->vibe = 0;
						snd_trk->lfoc = 0;
						snd_trk->on = 1;
						tie = 1;
						note = 1;
					}
				}
			}
			if (note == 1) {
			//	while(1) {
					mml = *snd_trk->pp;
					// 音程は直接指定するので+-は見ない
					// コンバート時に付点のみの場合は音長データをセットするようにする
					// 音長コマンド
					if ((mml & 0xf0) == 0x70) {
						len = (mml & 0x0f);
					}
			//		break;
			//	}
				if (snd_trk->on == 1) {
					if (tie == 0) {
						if (snd_trk->ch != 4) {
						//	snd_trk->freq = scale_frequency[snd_trk->oct*12+snd_trk->scl];
							snd_trk->freq = scale_frequency[snd_trk->scl];
							if (snd_trk->scl != 0) {
								snd_trk->freq += snd_trk->detn;
							}
						} else {
						//	snd_trk->freq = scale_noise[snd_trk->oct*12+snd_trk->scl];
							snd_trk->freq = scale_noise[snd_trk->scl];
						}
					}
				} else {
					snd_trk->freq = 0;
				}
				// 音長
				snd_trk->cnt = tempo_length[snd_trk->tmp][len];
			}
		}
		if (note == 1) {
			if (tie == 0) {
				snd_trk->vibm = 0;
				if (snd_trk->mute == 0) {
					snd_ch_on_off(snd_trk);
				}
			}
		} else {
			snd_trk->play = 0;
			// チャンネル1(矩形波スイープあり)
			if (snd_trk->ch == 1) {
				nr51 = NR51_REG;
				NR51_REG = (nr51 & 0xee);
			}
			// チャンネル2(矩形波)
			if (snd_trk->ch == 2) {
				nr51 = NR51_REG;
				NR51_REG = (nr51 & 0xdd);
			}
			// チャンネル3(波形メモリ音源)
			if (snd_trk->ch == 3) {
				nr51 = NR51_REG;
				NR51_REG = (nr51 & 0xbb);
			}
			// チャンネル4(ノイズ)
			if (snd_trk->ch == 4) {
				nr51 = NR51_REG;
				NR51_REG = (nr51 & 0x77);
			}
			// SEトラックの場合
			if (i == 4) {
				// 本来鳴っていたはずの音を鳴らす
				snd_trk = &snd_track[se_busy - 1];
			//	snd_ch_on_off(snd_trk);
				snd_trk->mute = 0;
				se_busy = se_pri = 0;
			}
		}
	}
}
