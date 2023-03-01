#include "WM8960.h"

#include "gtest/gtest.h"

//The following code is used for the unit test of the buzzer.
//When the unit test program runs:
//Will use WM8960 to record for 3 seconds and create a Record_Wav_File_Test.wav file.

inline bool Check_Exists_File (const std::string& name) {
    struct stat buffer;   
    return (stat (name.c_str(), &buffer) == 0); 
}


TEST(WM8960_Test, Record_Wav_File_Test)
{
    WM8960_Settings My_WM8960_Settings;
    WM8960 MyWM8960;
    MyWM8960.Record(My_WM8960_Settings,3,"Record_Wav_File_Test.wav");
    ASSERT_TRUE(Check_Exists_File("Record_Wav_File_Test.wav"));
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}