#ifndef EV3_APP_COMMANDMAKER_H_
#define EV3_APP_COMMANDMAKER_H_

#define INIT_X  (0)
#define INIT_Y  (4)

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
#define INIT_DIRECTION  E_DIR

class CommandMaker {
public:

    typedef enum {
        BC_Forward     =  0,
		BC_Half_For    =  1,
		BC_Rturn       =  2,
		BC_Ltrun       =  3,
		BC_PassCircle  =  4,
		BC_RturnCircle =  5,
		BC_LTurnCircle =  6,
		BC_UTurnCircle =  7,
		BC_Set         =  8,
		BC_BackShallow =  9,
		BC_BackDeep    =  10,
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

	CommandMaker();
    virtual ~CommandMaker();

    typedef struct {
       int x;
       int y;
    } coord;

    coord mRoute[1000] = {};
    block_command mCommand[1000] = {};
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
