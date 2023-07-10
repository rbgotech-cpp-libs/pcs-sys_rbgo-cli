#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <fcntl.h>

#include <rbgo-cli.h>

namespace rbgo {
/*
https://man7.org/linux/man-pages/man0/termios.h.0p.html
https://www.ibm.com/docs/en/zos/2.2.0?topic=functions-tcsetattr-set-attributes-terminal
*/

typedef struct {
	termios tiosBck;
	termios tiosCfg;
	Ti fcntlBck;
	Ti fcntlCfg;
} CMData;

void* CliMode::vp = 0;

void CliMode::initialize(){

	CMData *cmd;
	if(!vp){
		cmd = new CMData();

		tcgetattr(STDIN_FILENO, &cmd->tiosBck);
		cmd->tiosCfg = cmd->tiosBck;

		cmd->fcntlBck = fcntl(STDIN_FILENO, F_GETFL, 0);
		cmd->fcntlCfg = cmd->fcntlBck;

		vp = cmd;
	}
}


void CliMode::inputBlock(bool b){

	CMData *cmd;
	if(vp){
		cmd = (CMData *)vp;

		if(b){
			cmd->tiosCfg.c_lflag |= ((Tu)ICANON);
			cmd->fcntlCfg &= ~((Ti)O_NONBLOCK);
		}else{
			cmd->tiosCfg.c_lflag &= ~((Tu)ICANON);
			cmd->fcntlCfg |= ((Ti)O_NONBLOCK);
		}

		tcsetattr(STDIN_FILENO, TCSANOW, &cmd->tiosCfg);
		fcntl(STDIN_FILENO, F_SETFL, cmd->fcntlCfg);
	}
}


void CliMode::inputEcho(bool b){

	CMData *cmd;
	if(vp){
		cmd = (CMData *)vp;

		if(b){
			cmd->tiosCfg.c_lflag |= ((Tu)ECHO);
			tcsetattr(STDIN_FILENO, TCSANOW, &cmd->tiosCfg);
		}else{
			cmd->tiosCfg.c_lflag &= ~((Tu)ECHO);
			tcsetattr(STDIN_FILENO, TCSANOW, &cmd->tiosCfg);
		}
	}
}

void CliMode::inputBlockEcho(){

	inputBlock(true);
	inputEcho(true);
}

void CliMode::inputNoBlockNoEcho(){

	inputBlock(false);
	inputEcho(false);
}

void CliMode::outputNoBuf(){

	setbuf(stdout, NULL);
}

void CliMode::restore(){

	CMData *cmd;
	if(vp){
		cmd = (CMData *)vp;

		tcsetattr(STDIN_FILENO, TCSANOW, &cmd->tiosBck);
		fcntl(STDIN_FILENO, F_SETFL, cmd->fcntlBck);

		delete (CMData *)vp;
		vp = 0;
	}
}

}//ns
