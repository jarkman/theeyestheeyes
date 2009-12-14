
#include "mbed.h"
#include "ucam.h"
#include "Servo.h"




LocalFileSystem local("local");
//UCam ucam(p13, p14);


void UCamInit();
Frame* UCamGetDiff( );



int main() {
   

   
    UCamInit();
    while( true )
    {
        UCamGetDiff();
    }
       
}


