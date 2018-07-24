//Functions for use in robot.
#include <phys253.h> //this is for the uints

#ifndef TEST_H
#define TEST_H

namespace test {
    class Test {
        public:
 
            static uint8_t error;

        private:

            bool tapeFollow(int kp, int kd, int gain, int speed);
         
            

    };
}


#endif