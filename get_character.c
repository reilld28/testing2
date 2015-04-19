#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<termios.h>
#include<string.h>


#include <stdlib.h>
#include <sys/select.h>
#include <termios.h>

int main(int argc, char *argv[]){
   int file1, file2, file4, count;
   char command[100];
   char command1[100];
   char myorder[100];
   unsigned char receive[100];


   if ((file1 = open("/dev/ttyO1", O_RDWR | O_NOCTTY |O_NDELAY ))<0){
      perror("UART: Failed to open UART1.\n");
      return -1;
   }

   if ((file2 = open("/dev/ttyO2", O_RDWR | O_NOCTTY |O_NDELAY ))<0){
         perror("UART: Failed to open UART2.\n");
         return -1;
      }
   if ((file4 = open("/dev/ttyO4", O_RDWR | O_NOCTTY | O_NDELAY ))<0){
            perror("UART: Failed to open UART4.\n");
            return -1;
         }
  //+++
   struct termios options;
   tcgetattr(file1, &options);
   options.c_cflag = B9600 | CS8 | CREAD | CLOCAL;
   options.c_iflag = IGNPAR | ICRNL;
   //options.c_cflag &= ~(ICANON | ECHO | ECHOE | ISIG);
   //options.c_oflag &= ~OPOST;
   tcflush(file1, TCIFLUSH);
   tcsetattr(file1, TCSANOW, &options);

   tcgetattr(file2, &options);
   options.c_cflag = B9600 | CS8 | CREAD | CLOCAL;
   options.c_iflag = IGNPAR | ICRNL;
   options.c_cflag &= ~(ICANON | ECHO | ECHONL | ISIG | IEXTEN);
   options.c_oflag &= ~(ONLCR | OCRNL);
//   options.c_cc[VMIN]=1;
//   options.c_cc[VTIME]=50;
  // tcsetattr(file, TCSANOW, &options);

   tcflush(file2, TCIFLUSH);

   tcgetattr(file4, &options);
   options.c_cflag = B9600 | CS8 | CREAD | CLOCAL;
   options.c_iflag = IGNPAR | ICRNL;
   options.c_cflag &= ~(ICANON | ECHO | ECHONL | ISIG | IEXTEN);
   options.c_oflag &= ~(ONLCR | OCRNL);
//   options.c_cc[VMIN]=1;
//   options.c_cc[VTIME]=50;
   tcflush(file4, TCIFLUSH);
   // tcsetattr(file1, TCSANOW, &options);

   // send the string plus the null character
   if ((count = write(file1, "off", strlen("off")))<0){
	  perror("Failed to write to the output 1 \n");
      return -1;
   }
   printf(" Sent off with [%d] characters to 1 \n",count);

   // send the string to file (/dev/ttyO2)
   if ((count = write(file2, "GO\n", 3))<0){
	   perror("Failed to write GO to the output 2\n");
       return -1;
   }

   printf(" Sent GO with [%d] characters to 2 \n",count);

   // send the string to file4 (/dev/ttyO4)
   if ((count = write(file4, "GO\n", 3))<0){
	   perror("Failed to write GO to the output 4\n");
       return -1;
   }

   printf(" Sent GO with [%d] characters to 4 \n",count);

   //usleep(500000);

 //+++


struct termios orig_termios;

void reset_terminal_mode()
{
    tcsetattr(0, TCSANOW, &orig_termios);
}

void set_conio_terminal_mode()
{
    struct termios new_termios;

    /* take two copies - one for now, one for later */
    tcgetattr(0, &orig_termios);
    memcpy(&new_termios, &orig_termios, sizeof(new_termios));

    /* register cleanup handler, and set the new terminal mode */
    atexit(reset_terminal_mode);
    cfmakeraw(&new_termios);
    tcsetattr(0,TCSANOW, &new_termios);

}

int kbhit()
{
    struct timeval tv = { 0L, 0L };
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(0, &fds);
    return select(1, &fds, NULL, NULL, &tv);
}

int getch()
{
    int r;
    unsigned char c;
    if ((r = read(0, &c, sizeof(c))) < 0) {
        return r;
    } else {
        return c;
    }
}
//int main(int argc, char *argv[]){

unsigned int a;
    set_conio_terminal_mode();

while (1==1) {

    while (!kbhit()) {
    	 

  // Do your work here      printf("Hello\n");
// read motor speed
// check for collision
//adjust if collision

    }

    strcpy (command,"");
    strcpy (command1,"");
    a = getch(); /* consume the character */
    if (a == 119){
    	printf("Forward\n");
    		strcpy(command,"F1000\n");
    		strcpy(command1,"F1000\n");
    	// set the motors to forward
    }
    if (a == 97) {
    	printf("Left\n");
    	 	 strcpy(command,"F1500\n");
    	 	 strcpy(command1,"F1000\n");
    }
    if (a == 115){
    	printf("Backward\n");
    		strcpy(command,"R1000\n");
    	    strcpy(command1,"R1000\n");

    }
    if (a == 100){
    	printf("Right\n");
    		strcpy(command,"F1000\n");
    	    strcpy(command1,"F1500\n");
    }
    if (a == 120){

    printf("Stop\n");
    		strcpy(command,"X\n");
        	strcpy(command1,"X\n");
    }
    if (a == 32)   {
            printf("Exit\n");
            return -1; }
   // printf("[%d] [%d] [%d] \n", a,a,a );

//}

//Send the string to file2
       if ((count = write(file2, command, strlen(command)))<0){
       	   perror("Failed to write F2000 to the output 2\n");
              return -1;
          }
    //      printf(" Sent F2000 with [%d] characters to 2 \n",count);

          // send the string to file4 (/dev/ttyO4)
       if ((count = write(file4, command1,strlen(command1)))<0){
             perror("Failed to write F2000 to the output 4\n");
             return -1;
          }
  //        printf(" Sent %s [%d] to 4 \n", "F2000",count);

         usleep(500000);
          //fcntl(file, F_SETFL, FNDELAY);
   //       printf("about to read from 2\n");

          if ((count = read(file2, (void*)receive, 100))<0){
             perror("Failed to read from the input 2\n");
       //      return -1;
          }
          if (count==0) printf("There was no data available to read on 2!\n");
          else {
             receive[count]=0;  //There is no null character sent by the Arduino
 //            printf("The following was read in on 2: [%d]: %s\n",count,receive);
          }

 //         printf("about to read from 4 now\n");
          if ((count = read(file4, (void*)receive, 100))<0){
             perror("Failed to read from the input 4\n");
           //  return -1;
          }
          if (count==0) printf("There was no data available to read on 4!\n");
          else {
             receive[count]=0;  //There is no null character sent by the Arduino
//             printf("The following was read in on 4: [%d]: %s\n",count,receive);
          }
}
          printf("on my way out now\n");
          close(file2);
          close(file4);
          close(file1);
          return 0;
}
