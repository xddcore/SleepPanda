/***********************************************************************
 * \file RD117piB.cpp    (main)                                        *
 *                                                                     *
 *                                                                     *
 * Project:     RD117piA                                               *
 * Filename:    RD117piA.cpp                                           *
 * References:    Code adapted from:                                   *
 *     Maxim:                                                          *
 *         https://github.com/MaximIntegratedRefDesTeam/RD117_ARDUINO  *
 *     Robert Fraczkiewicz                                             *
 *         https://github.com/aromring/MAX30102_by_RF/blob/master/ )   *
 *     pigpio c library example code at:                               *
 *         http://abyz.me.uk/rpi/pigpio/code/                          *
 *                                                                     *
 * Description: Raspberry Pi main code file for MAX3010x protos.       *
 *                                                                     *
 * Revision History:                                                   *
 *   01-18-2016 Rev 01.00 GL Initial release.                          *
 *   12-22-2017 Rev 02.00 Greatly mod'd by Robert Fraczkiewicz (RF)    *
 *                          to use Wire lib vs. MAXIM's SoftI2C lib.   *
 *   06/18/2020 Rev 03.00 Ported to Raspberry Pi using "pigpio" lib.   *
 *                          This version includes algorithm*           *
 ***********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <math.h>
#include <linux/i2c-dev.h>
#include <thread>
#include <chrono>
#include <pigpio.h>
#include <gtk/gtk.h>

#include "max30102pi.h"
#include "max30102pi.cpp"
#include "algorithm2.h"
#include "algorithm2.cpp"
#include "algorithm2_by_RF.h"
#include "algorithm2_by_RF.cpp"

#define DUMPREGSSIZE 20
#define ITER 10

int setup(int* i2cDevHndl);
int writeReadingsToLog(FILE *ofpL, int basePtr);
int writeRegsToLog(FILE *ofpR);
void end_program (GtkWidget *wid, gpointer ptr);
void guiInit (void);

unsigned char byte1;
unsigned char byte2;
char buf[16];
int i2cHndl;
int regDumpArray[DUMPREGSSIZE];

FILE *ofpR;
FILE *ofpL;

uint32_t elapsedTime,timeStart;

uint32_t aun_ir_buffer[BUFFER_SIZE];   // infrared LED sensor data
uint32_t aun_red_buffer[BUFFER_SIZE];  // red LED sensor data
int tckVal_buffer[BUFFER_SIZE];        // uSec time intervals
double old_n_spo2;                     // Previous SPO2 value
unsigned char uch_dummy;

double n_spo2,ratio,correl;  //SPO2 value
int8_t ch_spo2_valid;        //indicator to show if the SPO2 calculation is valid
int32_t n_heart_rate;        //heart rate value
int8_t  ch_hr_valid;         //indicator to show if the heart rate calculation is valid
int32_t i;
char hr_str[10];

double n_spo2_maxim;         //SPO2 value
int8_t ch_spo2_valid_maxim;  //indicator to show if the SPO2 calculation is valid
int32_t n_heart_rate_maxim;  //heart rate value
int8_t  ch_hr_valid_maxim;   //indicator to show if the heart rate calculation is valid

char spbuff[10];

int main (int argc, char **argv)
{
  // int j = 0;
  system("sudo killall pigpiod");
  usleep(250000);
  setup(&i2cHndl);
  max30102pi_dumpRegs(i2cHndl, regDumpArray);
  writeRegsToLog(ofpR);

  gtk_init (&argc, &argv);

  GtkWidget *lbl1 = gtk_label_new ("Sp02:");
  GtkWidget *lbl2 = gtk_label_new ("HR:");
  GtkWidget *lbl3 = gtk_label_new ("98.7");
  GtkWidget *lbl4 = gtk_label_new ("72");
  GtkWidget *win  = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  GtkWidget *tbl  = gtk_table_new (2, 2, TRUE);

  g_signal_connect (win, "delete_event", G_CALLBACK (end_program), NULL);
  gtk_table_attach_defaults (GTK_TABLE (tbl), lbl1, 0, 1, 0, 1);
  gtk_table_attach_defaults (GTK_TABLE (tbl), lbl3, 1, 2, 0, 1);
  gtk_table_attach_defaults (GTK_TABLE (tbl), lbl2, 0, 1, 1, 2);
  gtk_table_attach_defaults (GTK_TABLE (tbl), lbl4, 1, 2, 1, 2);
  gtk_container_add (GTK_CONTAINER (win), tbl);

  char fontDescr[] = "Tahoma Bold 120.0";

  // Helpful GTK font change links:
  //   https://developer.gnome.org/pango/stable/pango-Fonts.html
  //   https://developer.gnome.org/pango/stable/
  //                                pango-Fonts.html#PangoFontDescription
  gtk_widget_modify_font(lbl1, \
		  pango_font_description_from_string(fontDescr));
  gtk_widget_modify_font(lbl2, \
		  pango_font_description_from_string(fontDescr));
  gtk_widget_modify_font(lbl3, \
		  pango_font_description_from_string(fontDescr));
  gtk_widget_modify_font(lbl4, \
		  pango_font_description_from_string(fontDescr));

  gtk_widget_show_all (win);

  while(gtk_events_pending())
  {
    gtk_main_iteration_do (false);
  }
    
  printf("guiInit Done.\n");
  
  //buffer length of BUFFER_SIZE stores ST seconds of samples running at FS sps
  //read BUFFER_SIZE samples, and determine the signal range
  uint32_t lastTick, nextTick;
  int diffTick;
  lastTick = gpioTick();

  for(int k = 0; k < ITER; k++)
  {
    printf("==================================================\n");
    printf("%d\n", k);
    for(int i = 0; i < BUFFER_SIZE; i++)
    {
      while(gpioRead(INTPIN)==1);
      i2cReadByteData(i2cHndl, REG_INTR_STATUS_1);                           // Clear ISR1.
      max30102pi_read_fifo(i2cHndl, (aun_red_buffer+i), (aun_ir_buffer+i));  // Read from MAX30102 FIFO.
      nextTick = gpioTick();
      diffTick = nextTick - lastTick;
      tckVal_buffer[i] = diffTick;
      lastTick = nextTick;
    }
  

    rf_heart_rate_and_oxygen_saturation(aun_ir_buffer, BUFFER_SIZE, aun_red_buffer,
                                        &n_spo2, &ch_spo2_valid, &n_heart_rate,
                                        &ch_hr_valid, &ratio, &correl); 

    if (ch_spo2_valid == 1)
    {
      sprintf(spbuff, "%4.1f", n_spo2);
      gtk_label_set_text (GTK_LABEL (lbl3), spbuff);
      sprintf(spbuff, "%3d", n_heart_rate);
      gtk_label_set_text (GTK_LABEL (lbl4), spbuff);
    }
    
    printf("RF: SpO2: %lf\n", n_spo2);
    printf("    HR:   %d\n", n_heart_rate);
    printf("---------------\n");

    maxim_heart_rate_and_oxygen_saturation(aun_ir_buffer, BUFFER_SIZE, aun_red_buffer,
                                           &n_spo2_maxim, &ch_spo2_valid_maxim,
					   &n_heart_rate_maxim, &ch_hr_valid_maxim);

    printf("MX: SpO2: %lf\n", n_spo2_maxim);
    printf("    HR:   %d\n", n_heart_rate_maxim);
    printf("---------------\n");

    writeReadingsToLog(ofpL, k);

    while(gtk_events_pending())
    {
      gtk_main_iteration_do (false);
    }
    
  }

  max30102pi_dumpRegs(i2cHndl, regDumpArray);
  i2cWriteByteData(i2cHndl, REG_MODE_CONFIG,0x40);    // Reset MAX30102.
  sleep(1);                                           // Delay 1 sec.
  writeRegsToLog(ofpR);
  fclose(ofpR);
  fclose(ofpL);
  i2cClose(i2cHndl);
  gpioTerminate();

  return 0;
}  // end of int main (int argc, char **argv)


int setup(int* i2cDevHndl)
{
  int hndl;
  
  gpioInitialise();

  gpioSetMode(INTPIN, PI_INPUT);         // RaspPi pin 23 connects to the
                                         // MAX30102 interrupt output pin
  gpioSetPullUpDown(INTPIN, PI_PUD_UP);  // Sets a pull-up (also 6.8k ext. to 3.3V).

  // Open port (i2c) for reading and writing
  hndl = max30102pi_initDevice();
  if (hndl == -1)
  {
    exit(1);
  }
  *i2cDevHndl = hndl;

  ofpR = fopen("oxiRegs.dat", "w");
  ofpL = fopen("oxiData.csv", "w");

  i2cReadByteData(i2cHndl, REG_INTR_STATUS_1);  // clear ISR1.

  return 0;
}  // End of int setup(int* i2cDevHndl)


int writeReadingsToLog(FILE *ofpL, int basePtr)
{
  for(int i = 0; i < BUFFER_SIZE; i++)
  {
    fprintf(ofpL, "%i, %i, %i, %i, %d\n", basePtr, i, aun_red_buffer[i], aun_ir_buffer[i], tckVal_buffer[i]);
  }

  return 0;
}  // End of int writeReadingsToLog(int ofpL)

int writeRegsToLog(FILE *ofpR)
{
  fprintf(ofpR, "----Begin MAX3010x Regs------------\n");
  for(int i = 0; i < DUMPREGSSIZE; i++)
  {
    fprintf(ofpR, "%02X\n", regDumpArray[i]);
  }
  fprintf(ofpR, "----End   MAX3010x Regs------------\n");
  return 0;
}  // End of int writeRegsToLog(int ofpR)

void end_program (GtkWidget *wid, gpointer ptr)
{
  gtk_main_quit ();
}

