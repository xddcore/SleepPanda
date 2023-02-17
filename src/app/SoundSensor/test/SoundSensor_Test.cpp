#include "../SoundSensor.h"

#include "gtest/gtest.h"
//The following code is used for the unit test of the SoundSensor.
//What you will see: Make a sound, 
//the DO pin level of the sound sensor will jump from low level to high level (rising edge interrupt), 
//trigger a sound event, and the screen will print Rpi_SoundSensor Class DEBUG: SoundSensor_SoundEvent_Handle() was triggered .

class Rpi_SoundSensor : public SoundSensor {
    using SoundSensor::SoundSensor;
    public:
        bool ISR_CheckPoint = false;
	    virtual void SoundSensor_SoundEvent_Handle() {
		    printf("Rpi_SoundSensor Class DEBUG: SoundSensor_SoundEvent_Handle() was triggered.\r\n");
            ISR_CheckPoint = true;
	    }
};

TEST(SoundSensor_Test, Check_SoundSensor_Event_ISR)
{
    SoundSensor_Settings My_SoundSensor_Settings;
    My_SoundSensor_Settings.Trigger_Method = RISING_EDGE; //when sound appears
    Rpi_SoundSensor My_Rpi_SoundSensor(My_SoundSensor_Settings);
    getchar();
    printf("My_Rpi_SoundSensor.ISR_CheckPoint:%d\r\n",My_Rpi_SoundSensor.ISR_CheckPoint);
    ASSERT_TRUE(My_Rpi_SoundSensor.ISR_CheckPoint);//pass gtest
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    printf("\r\nMake a sound please, then press the ENTER of the keyboard.\r\n");
    return RUN_ALL_TESTS();
}