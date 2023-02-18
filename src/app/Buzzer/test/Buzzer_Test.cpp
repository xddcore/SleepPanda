#include "Buzzer.h"

#include "gtest/gtest.h"

//The following code is used for the unit test of the buzzer.
//When the unit test program runs:
//If you hear the buzzer, please use the keyboard to type "Y" and press ENTER.

TEST(Buzzer_Test, Check_If_Buzzer_Sounds)
{
    Buzzer_Settings My_Buzzer_Settings;
    Buzzer MyBuzzer(My_Buzzer_Settings);
    MyBuzzer.Buzzer_Contorl(Buzzer_On);
    int Keyboard_Input = getchar();
    EXPECT_STREQ("Y", (const char*)&Keyboard_Input); //if Keyboard Input "Y", and then the test pass
    MyBuzzer.Buzzer_Contorl(Buzzer_Off);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    printf("\r\nPlease confirm whether the buzzer sounds (Y/N):\r\n");
    return RUN_ALL_TESTS();
}