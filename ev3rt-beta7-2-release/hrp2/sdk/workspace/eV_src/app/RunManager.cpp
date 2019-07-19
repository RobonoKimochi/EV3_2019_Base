#include "RunManager.h"
#include "app.h"


// 次のゾーンに入ったかを判断する際に、コース形状を考慮に入れるかを選択する。
#define CHECK_COURSE(x) (line == (x))
//#define CHECK_COURSE(x) (1)

// テスト Lコース
#define ZONE1_LEN 540
//#define ZONE2_LEN 250
#define ZONE3_LEN 318
//#define ZONE4_LEN 318
#define ZONE5_LEN 74
//#define ZONE6_LEN 195
//#define ZONE7_LEN 112
#define ZONE8_LEN 123
//#define ZONE9_LEN 52
#define ZONE10_LEN 467
#define ZONE12_LEN 85
#define ZONE15_LEN 191
#define ZONE16_LEN 867
#define ZONE18_LEN 156

#define ZONE2_THETA ((float)(PI / 2))
#define ZONE4_THETA ((float)(PI / 2))
#define ZONE6_THETA ((float)(PI * 3 / 2))
#define ZONE7_THETA ((float)(PI * 5 / 4))
#define ZONE9_THETA ((float)(PI / 2))
#define ZONE11_THETA ((float)(PI / 2))
#define ZONE13_THETA ((float)(PI / 4))
#define ZONE14_THETA ((float)(PI * 5 / 4))
#define ZONE17_THETA ((float)(PI / 2))

/**
 * コンストラクタ
 * @param lineMonitor     ライン判定
 * @param balancingWalker 倒立走行
 */
RunManager::RunManager()
    : mZone(START),
      mDeltaTheta(0),
	  dLine(0),
	  dDist(0),
	  dCount(0),
	  dTheta(0),
      mXOrigin(0),
      mYOrigin(0),
	  mTOrigin(0),
      mX(0),
      mY(0),
      mDistance(0),
      mDeviation(0),
      mIntegral(0),
      mKp(10.00),
      mKi(0.00),
      mKd(0.00)
{

}
/**
 * 現在走行しているコースを判断する関数
 * (前回区間から一定距離以上走行) かつ (ラインの状態が正しい）
 * の2つの条件から区間の切り替わりを判断します
 */

RunManager::Section RunManager::determineCourse() {
    static uint8_t line;    // ライン情報（直線、右曲線、左曲線）
    static uint16_t dist;    // 距離

    line = determineLine();
    dist = (uint16_t)getDistanceFromOrigin();
    dLine = line;
    dDist = dist;
    dTheta = ABS(mOdmetry->getTheta() - mTOrigin);

    switch(mZone) {
    case START:
        mZone = ZONE1;
        setOrigin();

        return STRAIGHT_ZONE;

    case ZONE1:
        if(dist > ZONE1_LEN) {
//        if(dist > ZONE1_LEN && CHECK_COURSE(LEFT_CURVE)) {
            mZone = ZONE2;
            setOrigin();
            mSound->ok();
        }
        return STRAIGHT_ZONE;

    case ZONE2:
        if(dTheta > ZONE2_THETA && CHECK_COURSE(STRAIGHT)) {
            mZone = ZONE3;
            setOrigin();
            mSound->ok();
        }
        return TIGHT_CURVE_ZONE;

    case ZONE3:
        if(dist > ZONE3_LEN) {
//       	if(dist > ZONE3_LEN && CHECK_COURSE(RIGHT_CURVE)) {
            mZone = ZONE4;
            setOrigin();
            mSound->ok();
        }
        return STRAIGHT_ZONE;

    case ZONE4:
        if(dTheta > ZONE4_THETA && CHECK_COURSE(STRAIGHT)) {
            mZone = ZONE5;
            setOrigin();
            mSound->ok();
        }
        return LOOSE_CURVE_ZONE;

    case ZONE5:
        if(dist > ZONE5_LEN) {
//       	if(dist > ZONE5_LEN && CHECK_COURSE(RIGHT_CURVE)) {
            mZone = ZONE6;
            setOrigin();
            mSound->ok();
        }
        return STRAIGHT_ZONE;

    case ZONE6:
        if(dTheta > ZONE6_THETA || CHECK_COURSE(RIGHT_CURVE)) {
            mZone = ZONE7;
            setOrigin();
            mSound->ok();
        }
        return TIGHT_CURVE_ZONE;

    case ZONE7:
        if(dTheta > ZONE6_THETA && CHECK_COURSE(STRAIGHT)) {
            mZone = ZONE8;
            setOrigin();
            mSound->ok();
        }
        return TIGHT_CURVE_ZONE;

    case ZONE8:
        if(dist > ZONE8_LEN) {
            setOrigin();
            mSound->ok();
            mZone = ZONE9;
        }
        return STRAIGHT_ZONE;

    case ZONE9:
        if(dTheta > ZONE9_THETA && CHECK_COURSE(STRAIGHT)) {
            setOrigin();
            mSound->ok();
            mZone = ZONE10;
        }
        return LOOSE_CURVE_ZONE;
    case ZONE10:
        if(dist > ZONE10_LEN) {
            setOrigin();
            mSound->ok();
            mZone = ZONE11;
        }
        return STRAIGHT_ZONE;
    case ZONE11:
        if(dTheta > ZONE9_THETA && CHECK_COURSE(STRAIGHT)) {
            setOrigin();
            mSound->ok();
            mZone = ZONE12;
        }
        return LOOSE_CURVE_ZONE;
    case ZONE12:
        if(dist > ZONE12_LEN) {
            setOrigin();
            mSound->ok();
            mZone = ZONE13;
        }
        return STRAIGHT_ZONE;
    case ZONE13:
        if(dTheta > ZONE13_THETA || CHECK_COURSE(LEFT_CURVE)) {
            setOrigin();
            mSound->ok();
            mZone = ZONE14;
        }
        return LOOSE_CURVE_ZONE;
    case ZONE14:
        if(dTheta > ZONE14_THETA && CHECK_COURSE(STRAIGHT)) {
            setOrigin();
            mSound->ok();
            mZone = ZONE15;
        }
        return TIGHT_CURVE_ZONE;
    case ZONE15:
        if(dist > ZONE15_LEN) {
            setOrigin();
            mSound->ok();
            mZone = ZONE16;
        }
        return STRAIGHT_ZONE;
    case ZONE16:
        if(dist > ZONE16_LEN) {
            setOrigin();
            mSound->ok();
            mZone = ZONE17;
        }
        return SLOW;
    case ZONE17:
        if(dTheta > ZONE17_THETA && CHECK_COURSE(STRAIGHT)) {
            setOrigin();
            mSound->ok();
            mZone = ZONE18;
        }
        return SLOW;
    case ZONE18:
        if(dist > ZONE8_LEN) {
            setOrigin();
            mSound->ok();
            mZone = ZONE19;
        }
        return SLOW;

    case ZONE19:
        return FINISHED;
    default:
        return SECTION_ERROR;
    }
}

/**
 * 角度の時間変化率から現在の走行ラインが
 * 直線、右曲線、左曲線かを判断する
 */
RunManager::Line RunManager::determineLine(){
    mDeltaTheta = mOdmetry->getDeltaTheta();
    if( mDeltaTheta > -0.0015 && mDeltaTheta < 0.0015){
        return STRAIGHT;
    }else if( mDeltaTheta <= -0.0015){
        return RIGHT_CURVE;
    }else if( mDeltaTheta >= 0.0015){
        return LEFT_CURVE;
    }else{
        return LINE_ERROR;
    }
}
/**
 * 灰色検知
 */
bool RunManager::detectGray(){
    mDeltaTheta = mOdmetry->getDeltaTheta();
    if( mDeltaTheta < -0.0005){
        return true;
    }
    return false;
}
/**
 * 現在地を原点を設定する
 */
void RunManager::setOrigin(){
    mXOrigin = mOdmetry->getX();
    mYOrigin = mOdmetry->getY();
    mTOrigin = mOdmetry->getTheta();
}
/**
 * 設定した原点から現在地までの直線距離を返す
 */
float RunManager::getDistanceFromOrigin(){
    mX = mOdmetry->getX();
    mY = mOdmetry->getY();
    return (mMat.Dist(mXOrigin,mYOrigin,mX,mY));
}

int8_t RunManager::calcTurnByTheta(float deviation) {
// #define KPID_P_FACTOR       (float)( 2.0)
// #define KPID_I_FACTOR       (float)( 0.00)
// #define KPID_D_FACTOR       (float)( 5.00)
// #define KPID_EDGE_FACTOR    (int32_t)(-1)        // ライントレース方向 1 or -1

#define KPID_TURN_LIMIT     (int32_t)(100)      // 旋回指示値 限界値

    float theta_P;                       // P成分
    float theta_I;                       // I成分
    float theta_D;                       // D成分
    int32_t turn_P;                             // P項演算値
    int32_t turn_I;                             // I項演算値
    int32_t turn_D;                             // D項演算値
    int32_t turn;                               // 旋回命令: -100 (左旋回) ～ 100 (右旋回)

    theta_P = deviation;
    theta_I = mIntegral + theta_P;
    theta_D = theta_P - mDeviation;

    mDeviation = theta_P;                              // 次回演算用に記憶
    mIntegral = theta_I;

    /* P項演算 */
    turn_P = (int32_t)(mKp * theta_P);
    // turn_P *= KPID_EDGE_FACTOR;

    /* I項演算 */
    turn_I = (int32_t)(mKi * theta_I);
    // turn_I *= KPID_EDGE_FACTOR;

    /* D項演算 */
    turn_D = (int32_t)(mKd * theta_D);
    // turn_D *= KPID_EDGE_FACTOR;

    /* 旋回指示値設定   */
    turn = turn_P + turn_I + turn_D;

    if (turn >= KPID_TURN_LIMIT) {
        turn = KPID_TURN_LIMIT;
    } else if (turn <= (-1 * KPID_TURN_LIMIT)) {
        turn = -1 * KPID_TURN_LIMIT;
    }

    return turn;
}

/**
 * PIDのパラメータを設定する
 * @param kp P制御値
 * @param ki I制御値
 * @param kd D制御値
 */
void RunManager::setPID(float kp, float ki, float kd) {
    mKp = kp;
    mKi = ki;
    mKd = kd;
}
