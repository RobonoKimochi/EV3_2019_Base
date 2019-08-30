/******************************************************************************
 *  app.h (for LEGO Mindstorms EV3)
 *  Created on: 2015/01/25
 *  Definition of the Task main_task
 *  Author: Kazuhiro.Kawachi
 *  Copyright (c) 2015 Embedded Technology Software Design Robot Contest
 *****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#include "ev3api.h"
#include "app_config.h"

/*
 *  各タスクの優先度の定義
 */
// #define TMIN_TPRI        1            /* タスク優先度の最小値（最高値） */
// #define TMIN_APP_TPRI        (TMIN_TPRI + 8)
#define MAIN_PRIORITY       TMIN_APP_TPRI + 1      /* mainタスクの優先度       (10) */
#define TRACER_PRIORITY     TMIN_APP_TPRI + 2      /* Tracerタスクの優先度     (11) */
#define ODMETRY_PRIORITY    TMIN_APP_TPRI + 3      /* Odmetryタスクの優先度    (12) */
#define TAILMOTOR_PRIORITY  TMIN_APP_TPRI + 6      /* TailMotorタスクの優先度  (15) */
#define UI_PRIORITY         TMIN_APP_TPRI + 4      /* UIタスクの優先度         (13) */
#define REMOTE_PRIORITY     TMIN_APP_TPRI + 5      /* Remoteタスクの優先度     (14) */
#define LOGGER_PRIORITY     TMIN_APP_TPRI + 5      /* Loggerタスクの優先度     (14) */
#define BLOCK_PRIORITY      TRACER_PRIORITY        /* blockタスクの優先度      (11) トレーサタスクと同じ優先度で設定 */
#define CALIB_PRIORITY      TRACER_PRIORITY        /* calibタスクの優先度      (11) トレーサタスクと同じ優先度で設定 */
#define ARM_PRIORITY        TMIN_APP_TPRI + 4      /* armタスクの優先度        (13) */
#define CMD_PRIORITY        TMIN_APP_TPRI + 4      /* cmdタスクの優先度        (13) */
#define INI_PRIORITY        TMIN_APP_TPRI + 4      /* iniタスクの優先度        (13) */
#define SOUND_PRIORITY      TMIN_APP_TPRI + 6      /* soundタスクの優先度      (15) */


/*
 *  ターゲットに依存する可能性のある定数の定義
 */
#ifndef STACK_SIZE
#define STACK_SIZE      4096        /* タスクのスタックサイズ */
#endif /* STACK_SIZE */

/*
 *  関数のプロトタイプ宣言
 */
#ifndef TOPPERS_MACRO_ONLY

extern void main_task(intptr_t exinf);
extern void tracer_task(intptr_t exinf);
extern void ev3_cyc_tracer(intptr_t exinf);
extern void block_task(intptr_t exinf);
extern void ev3_cyc_block(intptr_t exinf);
extern void calib_task(intptr_t exinf);
extern void ev3_cyc_calib(intptr_t exinf);
extern void sound_task(intptr_t exinf);
extern void ev3_cyc_sound(intptr_t exinf);
extern void cmd_task(intptr_t exinf);
extern void ini_task(intptr_t exinf);
extern void arm_task(intptr_t exinf);
extern void ev3_cyc_arm(intptr_t exinf);
extern void tailmotor_task(intptr_t exinf);
extern void ev3_cyc_tailmotor(intptr_t exinf);
extern void odmetry_task(intptr_t exinf);
extern void ev3_cyc_odmetry(intptr_t exinf);
extern void ui_task(intptr_t exinf);
extern void ev3_cyc_ui(intptr_t exinf);
extern void logger_task(intptr_t exinf);
extern void ev3_cyc_logger(intptr_t exinf);
extern void remote_task(intptr_t exinf);
extern void ev3_cyc_remote(intptr_t exinf);

#endif /* TOPPERS_MACRO_ONLY */

#ifdef __cplusplus
}
#endif
