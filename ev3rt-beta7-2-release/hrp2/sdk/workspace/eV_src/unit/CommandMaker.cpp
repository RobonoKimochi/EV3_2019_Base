#include "CommandMaker.h"
#include "app.h"


/**
 * コンストラクタ
 */
CommandMaker::CommandMaker()
{
}

/**
 * デストラクタ
 */
CommandMaker::~CommandMaker()
{
}


int CommandMaker::get_attri(int x, int y)
{
	return (attribute[y][x]);
}

int CommandMaker::calc_pos_relation(int before, int after)
{
	int pos_relate = 0;

	if (before == after) {
	      pos_relate = FOR_POS;
	} else if (before < after) {
	    if (before + 1 == after) {
	        pos_relate = RGT_POS;
	    } else if (before + 2 == after) {
	        pos_relate = BAK_POS;
	    } else if (before + 3 == after) {
	        pos_relate = LFT_POS;
	    }
	} else {
	    if (before - 1 == after) {
	        pos_relate = LFT_POS;
	    } else if (before - 2 == after) {
	        pos_relate = BAK_POS;
	    } else if (before - 3 == after) {
	        pos_relate = RGT_POS;
	    }
	}

	  return pos_relate;
}

int CommandMaker::calc_next_dir(int a_idx, int a_point_atri)
{
	  int move_x = mRoute[a_idx + 1].x - mRoute[a_idx].x;
	  int move_y = mRoute[a_idx + 1].y - mRoute[a_idx].y;
	  int dir = 0;
	  int drive_pos;

	  if (a_point_atri == BIN) {
	      drive_pos = REAR;
	  } else {
	      drive_pos = FRONT;
	  }

	  if (drive_pos == FRONT) {

	      if (move_x == 1) {
	          dir = E_DIR;
	      } else if (move_x == -1) {
	          dir = W_DIR;
	      } else if (move_y == 1) {
	          dir = S_DIR;
	      } else if (move_y == -1) {
	          dir = N_DIR;
	      }

	  } else {
	      if (move_x == 1) {
	          dir = W_DIR;
	      } else if (move_x == -1) {
	          dir = E_DIR;
	      } else if (move_y == 1) {
	          dir = N_DIR;
	      } else if (move_y == -1) {
	          dir = S_DIR;
	      }
	  }

	  return dir;
}

void CommandMaker::set_Command(block_command Command)
{
	  mCommand[mCmdIndex] = Command;
	  mCmdIndex++;
}


void CommandMaker::make_command()
{

  int direction = INIT_DIRECTION;
  int point_atri,next_atri,next_dir,pos_relate;
  for (int idx = 0; idx < mRouteIndex - 1; idx++) {
      point_atri = get_attri(mRoute[idx    ].x,mRoute[idx    ].y);
      next_atri  = get_attri(mRoute[idx + 1].x,mRoute[idx + 1].y);

      next_dir = calc_next_dir(idx, point_atri);

      pos_relate = calc_pos_relation(direction, next_dir);

      if (point_atri == CIR) {
          if (next_atri == CIR) {
        	  set_Command(BC_Err_CIR_CIR);
          } else if (next_atri == BAR) {
              if (pos_relate == FOR_POS) {
            	  set_Command(BC_PassCircle);
              } else if (pos_relate == RGT_POS) {
            	  set_Command(BC_RturnCircle);
              } else if (pos_relate == BAK_POS) {
            	  set_Command(BC_UTurnCircle);
              } else if (pos_relate == LFT_POS) {
            	  set_Command(BC_LTurnCircle);
              }
          } else if (next_atri == BIN) {
        	  set_Command(BC_Err_CIR_BIN);
          }

      } else if (point_atri == BAR) {
          if (next_atri == CIR) {
              if (pos_relate == FOR_POS) {
            	  set_Command(BC_Forward);
              } else if (pos_relate == RGT_POS) {
            	  set_Command(BC_BackShallow);
            	  set_Command(BC_Rturn);
                  set_Command(BC_Half_For);
              } else if (pos_relate == LFT_POS) {
            	  set_Command(BC_BackDeep);
            	  set_Command(BC_Ltrun);
                  set_Command(BC_Half_For);
              } else {
            	  set_Command(BC_Err_BAR_UT);
              }
          } else if (next_atri == BIN) {
              if (pos_relate == RGT_POS) {
            	  set_Command(BC_Half_For);
            	  set_Command(BC_Rturn);
              } else if (pos_relate == LFT_POS) {
            	  set_Command(BC_Half_For);
            	  set_Command(BC_Ltrun);
              } else {
            	  set_Command(BC_ERR_NOT_SET);
              }

          } else if (next_atri == BAR) {
        	  set_Command(BC_Err_BAR_BAR);
          }
      } else if (point_atri == BIN) {
          if (next_atri == CIR) {
        	  set_Command(BC_Err_BIN_CIR);
          } else if (next_atri == BAR) {

              if (pos_relate == FOR_POS) {
            	  set_Command(BC_Set);
              } else {
            	  set_Command(BC_Err_NOT_BAK);
              }

          } else if (next_atri == BIN) {
        	  set_Command(BC_ERR_BIN_BIN);
          }
      } else if (point_atri == NON) {
    	  set_Command(BC_Err_NON);
      }

      direction = next_dir;
   }

}
