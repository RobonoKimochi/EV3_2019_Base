#ifndef EV3_APP_COMMANDMAKER_H_
#define EV3_APP_COMMANDMAKER_H_

#include "app.h"
#include "app_config.h"

#if RUN_COURSE == RUN_LEFT_COURSE
#define INIT_X  (0)
#define INIT_Y  (4)
#else
#define INIT_X  (6)
#define INIT_Y  (4)
#endif

#define FRONT  (0)
#define REAR   (1)

#define CIR  (0)
#define BAR  (1)
#define BIN  (2)
#define NON  (3)

#define N_DIR  (0)
#define E_DIR  (1)
#define S_DIR  (2)
#define W_DIR  (3)

#define FOR_POS  (0)
#define RGT_POS  (1)
#define LFT_POS  (2)
#define BAK_POS  (3)

#if RUN_COURSE == RUN_LEFT_COURSE
#define INIT_DIRECTION  E_DIR
#else
#define INIT_DIRECTION  W_DIR
#endif

class CommandMaker {
public:

    typedef enum {
        BC_Forward     =  0,
		BC_Half_For    =  1,
		BC_Rturn       =  2,
		BC_Lturn       =  3,
		BC_PassCircle  =  4,
		BC_RTurnCircle =  5,
		BC_LTurnCircle =  6,
		BC_UTurnCircle =  7,
		BC_SetRgt      =  8,
		BC_BackLft     =  9,
		BC_BackRgt     =  10,
		BC_Med_For     =  11,
		BC_Short_For   =  12,
		BC_SetLft      =  13,
		BC_GetBlock    =  14,
		BC_GetLft      =  15,
		BC_GetRgt      =  16,
		BC_RemainFor   =  17,
		BC_Err_CIR_CIR =  90,
		BC_Err_CIR_BIN =  91,
		BC_Err_BAR_BAR =  92,
		BC_Err_BIN_CIR =  93,
		BC_ERR_BIN_BIN =  94,
		BC_Err_BAR_UT  =  95,
		BC_ERR_NOT_SET =  96,
		BC_Err_NOT_BAK =  97,
		BC_Err_NON     =  98,
		BC_End         =  100
    } block_command;

    typedef struct {
       char x;
       char y;
    } coord;

	CommandMaker(coord* route, int routeIndex);
    virtual ~CommandMaker();

    unsigned int mCommand_index = 0;
    block_command mCommand[500] =
    {

    	       /* 複合パターン1 右折から */
    	//         BC_RTurnCircle,
    	//		 BC_Forward,
    	//		 BC_LTurnCircle,
    	//		 BC_Short_For,
    	//		 BC_SetLft,
    	//		 BC_BackRgt,
    	//		 BC_Forward,
    	//		 BC_RTurnCircle,
    	//		 BC_Forward,
    	//		 BC_RTurnCircle,
    	//		 BC_Forward,
    	//		 BC_RTurnCircle,
    	//		 BC_Short_For,
    	//		 BC_SetRgt,
    	//		 BC_BackLft,
    	//		 BC_Half_For,
    	//		 BC_PassCircle,
    	//		 BC_Half_For,


    	         /* 複合パターン2 左折から */
    	//		 BC_LTurnCircle,
    	//		 BC_Forward,
    	//		 BC_RTurnCircle,
    	//		 BC_Half_For,
    	//		 BC_Rturn,
    	//		 BC_GetLft,
    	//		 BC_LTurnCircle,
    	//		 BC_Forward,
    	//		 BC_LTurnCircle,
    	//		 BC_Short_For,
    	//		 BC_SetLft,
    	//		 BC_BackRgt,
    	//		 BC_Half_For,
    	//		 BC_PassCircle,
    	//		 BC_Half_For,

    	  		  /* 複合パターン3 直進から */
    	//	  	 BC_PassCircle,
    	//  		 BC_Short_For,
    	//   		 BC_SetLft,
    	//    	 BC_BackRgt,
    	//  		 BC_Forward, /* half _for にすると意図通りに動作しないバグの可能性あり */
    	//		 BC_RTurnCircle,
    	//   		 BC_Short_For,
    	//   		 BC_SetRgt,
    	//   		 BC_BackLft,
    	//   		 BC_Half_For,
    	//   		 BC_LTurnCircle,
    	//   		 BC_Half_For,
    	//		 BC_Lturn,
    	//   		 BC_GetRgt,
    	//  		 BC_PassCircle,
    	//   		 BC_Half_For,

    			 /* 本番コース試走用 L */
    			 BC_PassCircle,
    			 BC_Forward,
    			 BC_PassCircle,
    			 BC_Forward,
    			 BC_PassCircle,
    			 BC_Forward,
    			 BC_LTurnCircle,
    			 BC_Forward,
    			 BC_LTurnCircle,
    			 BC_Forward,
    			 BC_LTurnCircle,
    			 BC_Forward,
    			 BC_PassCircle,
    			 BC_Forward,
    			 BC_RTurnCircle,
    			 BC_Forward,
    			 BC_PassCircle,
    			 BC_Forward,
    			 BC_RTurnCircle,
    			 BC_Forward,
    			 BC_PassCircle,
    			 BC_Forward,
    			 BC_PassCircle,
    			 BC_Forward,
				 BC_RTurnCircle,
    			 BC_Forward,
    			 BC_PassCircle,
    			 BC_Forward,
    			 BC_PassCircle,
    			 BC_Forward,
				 BC_RTurnCircle,
    			 BC_Short_For,
    			 BC_SetRgt,
    			 BC_BackRgt,
				 BC_Forward,
				 BC_RTurnCircle,
    			 BC_Half_For,
    			 BC_Lturn,
    			 BC_GetRgt,
    			 BC_RTurnCircle,
    			 BC_Forward,
    	//		 BC_PassCircle,
    	//		 BC_PassCircle,
    	//		 BC_PassCircle,
    	//		 BC_PassCircle,
    	//		 BC_PassCircle,
    	//		 BC_PassCircle,
    	//		 BC_PassCircle,
    	//		 BC_PassCircle,
    	//		 BC_PassCircle,
    	//		 BC_PassCircle,
    	//		 BC_PassCircle,
    	//		 BC_PassCircle,





    	    	 /* ↓5回左折 */
    	//       BC_LTurnCircle,
    	//       BC_Short_For,
    	//       BC_SetLft,
    	//       BC_BackLft,
    	//       BC_Forward,
    	//       BC_LTurnCircle,
    	//       BC_Short_For,
    	//       BC_SetLft,
    	//       BC_BackLft,
    	//       BC_Forward,
    	//       BC_LTurnCircle,
    	//       BC_Short_For,
    	//       BC_SetLft,
    	//       BC_BackLft,
    	//       BC_Forward,
    	//       BC_LTurnCircle,
    	//       BC_Short_For,
    	//       BC_SetLft,
    	//       BC_BackLft,
    	//       BC_Forward,
    	//       BC_LTurnCircle,
    	//       BC_Short_For,
    	//       BC_SetLft,
    	//       BC_BackLft,
    	//       BC_Forward,
    	//       BC_LTurnCircle,
    	//       BC_Short_For,
    	//       BC_SetLft,
    	//       BC_BackLft,
    	//       BC_Forward,


//    	        /* ↓5回右折 */
//    	       BC_RTurnCircle,
//    		   BC_Short_For,
//    	       BC_SetRgt,
//    		   BC_BackRgt,
//    	       BC_Forward,
//    	       BC_RTurnCircle,
//    		   BC_Short_For,
//    	       BC_SetRgt,
//    		   BC_BackRgt,
//    	       BC_Forward,
//    	       BC_RTurnCircle,
//    		   BC_Short_For,
//    	       BC_SetRgt,
//    		   BC_BackRgt,
//    	       BC_Forward,
//    	       BC_RTurnCircle,
//    		   BC_Short_For,
//    	       BC_SetRgt,
//    		   BC_BackRgt,
//    	       BC_Forward,
//    	       BC_RTurnCircle,
//    		   BC_Short_For,
//    	       BC_SetRgt,
//    		   BC_BackRgt,
//    	       BC_Forward,
//    		   /* ↑5回右折 */
//    		   BC_End

   	        /*  */
//			BC_LTurnCircle,
//			BC_Forward,
//			BC_Forward,
//			BC_RTurnCircle,
//			BC_Short_For,
//			BC_SetRgt,
//			BC_BackRgt,
//			BC_RemainFor,
//			BC_RTurnCircle,
//			BC_Forward,
//			BC_RTurnCircle,
//			BC_Forward,
//			BC_PassCircle,
//			BC_Forward,
//			BC_PassCircle,
//			BC_Short_For,
//			BC_SetLft,
//			BC_BackLft,
//			BC_RemainFor,
//			BC_Forward,
//			BC_Forward,
   		   /* ↑5回右折 */
   		   BC_End
    };

    coord* gRoute;
    int mRouteIndex = 0;
    int mCmdIndex = 0;

    void make_command(void);

private:

const int attribute[7][7] = {{CIR,BAR,CIR,BAR,CIR,BAR,CIR},
                               {BAR,BIN,BAR,BIN,BAR,BIN,BAR},
                               {CIR,BAR,CIR,BAR,CIR,BAR,CIR},
                               {BAR,BIN,BAR,NON,BAR,BIN,BAR},
                               {CIR,BAR,CIR,BAR,CIR,BAR,CIR},
                               {BAR,BIN,BAR,BIN,BAR,BIN,BAR},
                               {CIR,BAR,CIR,BAR,CIR,BAR,CIR}};

    int get_attri(int x, int y);
    int calc_pos_relation(int before, int after);
    int calc_next_dir(int a_idx,int a_point_atri);
    void set_Command(block_command Command);


};


#endif  // EV3_APP_COMMANDMAKER_H_
