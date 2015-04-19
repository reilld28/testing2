/* Simple send and receive C example for communicating with the
*  Arduino echo program using UART4.
*
* Written by Derek Molloy for the book "Exploring BeagleBone: Tools and
* Techniques for Building with Embedded Linux" by John Wiley & Sons, 2014
* ISBN 9781118935125. Please see the file README.md in the repository root
* directory for copyright and GNU GPLv3 license information.            */

#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<termios.h>
#include<string.h>

int main(int argc, char *argv[]){
   int file2, file4, count;
   char command[100];
   char command1[100];
   char myorder[100];
   if(argc!=2){
       printf("Please enter more characters2!\n");
       return -2;
   }


   if ((file2 = open("/dev/ttyO2", O_RDWR | O_NOCTTY |O_NDELAY ))<0){
      perror("UART: Failed to open UART2.\n");
      return -1;
   }
   if ((file4 = open("/dev/ttyO4", O_RDWR | O_NOCTTY | O_NDELAY ))<0){
         perror("UART: Failed to open UART4.\n");
         return -1;
      }
   struct termios options;
   tcgetattr(file2, &options);
   options.c_cflag = B9600 | CS8 | CREAD | CLOCAL;
   options.c_iflag = IGNPAR | ICRNL;
   options.c_cflag &= ~(ICANON | ECHO | ECHONL | ISIG | IEXTEN);
   options.c_oflag &= ~(ONLCR | OCRNL);
/
   tcflush(file2, TCIFLUSH);

   tcgetattr(file4, &options);
   options.c_cflag = B9600 | CS8 | CREAD | CLOCAL;
   options.c_iflag = IGNPAR | ICRNL;
   options.c_cflag &= ~(ICANON | ECHO | ECHONL | ISIG | IEXTEN);
   options.c_oflag &= ~(ONLCR | OCRNL);
   tcflush(file4, TCIFLUSH);


   // send the string to file (/dev/ttyO2)
   if ((count = write(file2, "GO\n", 3))<0){
	   perror("Failed to write GO to the output 2\n");
       return -1;
   }

   printf(" Sent GO with [%d] characters to 2 \n",count);

   // send the string to file1 (/dev/ttyO4)
   if ((count = write(file4, "GO\n", 3))<0){
	   perror("Failed to write GO to the output 4\n");
       return -1;
   }

   printf(" Sent GO with [%d] characters to 4 \n",count);

   usleep(500000);


   while (1==1){

	   printf("Enter an order:");
	   gets(myorder);
	   strcpy (command,"");
	   strcpy (command1,"");
	   printf("command is %s",myorder);

	   if (strcmp (myorder, "w")==0){
		   strcpy(command, "F1000\n");
		   strcpy(command1,"F1000\n");
	   }
	   else if (strcmp (myorder, "s")==0){
		   strcpy(command, "R1000\n");
		   strcpy(command1,"R1000\n");
	   }
	   else if (strcmp (myorder, "d")==0){
		   strcpy(command, "F1500\n");
		   strcpy(command1,"F750\n");
	   }
	   else if (strcmp (myorder, "a")==0){
		   strcpy(command, "F750\n");
		   strcpy(command1,"F1500\n");
	   }
	   else if (strcmp (myorder, "x")==0){
		   strcpy(command, "X\n");
		   strcpy(command1,"X\n");
	   }
	   else if (strcmp (myorder, "e")==0){
	   		   strcpy(command, "F1000\n");
	   		   strcpy(command1,"R1000\n");
	   	   }
	   else if (strcmp (myorder, "q")==0){
	   	   		   strcpy(command, "R1000\n");
	   	   		   strcpy(command1,"F1000\n");
	   	   	   }
	   else if (strcmp (myorder, " ")==0){
		   return -1;
	   }

	   printf("The command is %s and %s\n",command,command1);


   if ((count = write(file2, command, strlen(command)))<0){
	   perror("Failed to write F2000 to the output 2\n");
       return -1;
   }

   printf(" Sent file2 with [%d] characters to 2 \n",count);
   // send the string to file1 (/dev/ttyO4)

   if ((count = write(file4, command1,strlen(command1) ))<0){
      perror("Failed to write F2000 to the output 4\n");
      return -1;
   }
   printf(" Sent %s [%d] to 4 \n", "F2000",count);

   usleep(500000);
   
      printf("about to read from 2\n");
   unsigned char receive[100];
   if ((count = read(file2, (void*)receive, 100))<0){
      perror("Failed to read from the input 2\n");
//      return -1;
   }
   if (count==0) printf("There was no data available to read on 2!\n");
   else {
      receive[count]=0;  //There is no null character sent by the Arduino
      printf("The following was read in on 2: [%d]: %s\n",count,receive);
   }

   printf("about to read from 4 now\n");
   if ((count = read(file4, (void*)receive, 100))<0){
      perror("Failed to read from the input 4\n");
    //  return -1;
   }
   if (count==0) printf("There was no data available to read on 4!\n");
   else {
      receive[count]=0;  //There is no null character sent by the Arduino
      printf("The following was read in on 4: [%d]: %s\n",count,receive);
   }
   }
   printf("on my way out now\n");
   close(file2);
   close(file4);
   return 0;
}