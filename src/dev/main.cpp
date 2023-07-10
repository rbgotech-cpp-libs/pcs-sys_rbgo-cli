#include <stdlib.h>//setbuf, system, exit, atoi
#include <stdio.h>//puts, printf, scanf
#include <unistd.h>//usleep

#include <rbgo-cli.h>

#define MENU_LENGTH 4
const char* menuStrings[MENU_LENGTH] = {
	"0	quit",
	"1	menu",
	"2	clear screen",

	"3	CliMode: if(getchar() == '0') in bussy loop to quit: "
};

int main(){

	rbgo::CliMode::initialize();
	rbgo::CliMode::outputNoBuf();

	char input[16] = {0};
	rbgo::Tu32 mc=1, i=0;
	char backspaces[16] = {0};
	while(i < 15)backspaces[i++]='\b';

	//---- Test case data alloc ----

	while(1){

		if(mc){
			printf("'0' to quit...\n");
			rbgo::CliMode::inputNoBlockNoEcho();
		}

		switch(mc){
		case 0: goto break2;
		case 1: mc=0; while(mc < MENU_LENGTH) puts(menuStrings[mc++]); break;
		case 2: system("cls | clear"); break;

		//---- Test case exec ----
		case 3:
			i = 0;
			while(1){
				if(getchar() == '0') break;
				usleep(1000000 / 100);
				printf(backspaces);
				printf("%010u", i++);
			}
		break;
		}

		puts("");
		rbgo::CliMode::inputBlockEcho();
		printf("choice: ");
		scanf("%2s", input);
		mc = atoi(input);
	}

break2://---- Test case data free ----
	rbgo::CliMode::restore();

}
