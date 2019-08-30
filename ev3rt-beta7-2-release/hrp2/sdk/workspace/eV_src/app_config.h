/*
 * app_config.h
 *
 *  Created on: 2018/07/31
 *      Author: toyoda
 */

/* コンパイルSW用ヘッダファイル */

#ifndef EV_SRC_APP_CONFIG_H_
#define EV_SRC_APP_CONFIG_H_

/* コースの設定 */
/* 1:左コース	2:右コース */
#define RUN_LEFT_COURSE	(1)
#define RUN_RIGHT_COURSE	(2)
/* エッジの設定 */
#define RUN_LEFT_EDGE	(1)
#define RUN_RIGHT_EDGE	(2)

#define RUN_COURSE (RUN_LEFT_COURSE) /* ←ここにコースを記載 */
//#define RUN_COURSE (RUN_RIGHT_COURSE)

/* 走行するエッジの設定 */
#if RUN_COURSE == RUN_LEFT_COURSE
#define RUN_EDGE (RUN_LEFT_EDGE)
#else
#define RUN_EDGE (RUN_RIGHT_EDGE)
#endif

/* デバッグモード */
/* 0:通常走行モード	1:デバッグ走行モード */
#define USE_DEBUG_MODE		(1)


/* キャリブレーション実施 */
/* 0:実施しない	1:実施する */
#define RUN_COLOR_CALIBRATION	(0)
#define DEFAULT_COLOR_WHITE	(35)
#define DEFAULT_COLOR_BLACK	(1)

/* リモコン操作設定 */
/* 0:リモコン操作なし	1:リモコン操作あり */
#define USE_REMOTE_CONTROL_MODE	(1)


/* ロガー設定 */
/* 0:ログ出力なし	1:ログ出力あり */
#define USE_OUTPUT_LOG		(1)


/* CSV出力設定 */
/* 0:CSV出力なし	1:CSV出力あり */
#define USE_OUTPUT_CSV		(1)


/* ライントレースON */
/* 0:ライントレースOFF	1:ライントレースON */
#define USE_LINE_TRACE		(1)


/* PID切り替え無効 */
/* 0:PID切り替えあり	1:PID切り替え無し */
#define USE_CONSTANT_PID		(1)


/* FORWARD値設定 */
#define CONSTANT_FORWARD_VAL	(20)

/* サウンドの音量設定 */
#define SOUND_VOL				(4)

/* ルックアップゲートテスト用設定 */
/* 0:テストモードOFF	1:テストモードON */
#define LOOKUPGATE_TEST_MODE	(0)

/* ブロックならべテスト用設定 */
/* 0:テストモードOFF	1:テストモードON */
#define BLOCK_TEST_MODE	(1)

/* ブロックならべテスト用設定 */
/* 0:サンプルコース設定	　1:本番コース設定 */
#define BLOCK_SMALL_TEST_MODE	(0)

/* 紙コース用設定 */
/* 0:布コース	1:紙コース */
#define PERPAR_MODE	(0)

/* グレー対応閾値設定 */
/* 0:グレー対応OFF	1:グレー対応ON */
#define USE_GRAY_THRESH	(0)

#endif /* EV_SRC_APP_CONFIG_H_ */
