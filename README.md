# Hey, here we will be working how to configure 5 inch tft with driver ssd1963.
_Here is the pin configuration of LCD with ESP32_

## //////////////////////////////////////////////////////////////////////////////////
_LCD pins_
* 31 32
* 29 30
* 27 28
* 25 26
* 23 24
* 21 22
* 19 20
* 17 18
* 15 16
* 13 14
* 11 12
* 9  10
* 7  8
* 5  6
* 3  4
* 1  2

_Connection with ESP32_
* 3.3 => 2
* GND => 1
* D15 => 7
* D2 => 5 
* D4 => 4
* RX2 => 14
* TX2 => 13
* D5 => 6
* D18 => 31
* D19 => 27
* D23 => 29
* D13 => 10
* D12 => 9
* D14 => 16
* D27 => 15
* D26 => 11
* D25 => 12
* D33 => 8

## //////////////////////////////////////////////////////////////////////////////////
_After pin connections we add following code in arduino Ide_

```
#include <lvgl.h>
#define LGFX_USE_V1
#include <LovyanGFX.hpp>
#include "ui.h"

class LGFX : public lgfx::LGFX_Device
{
 lgfx::Panel_SSD1963     _panel_instance;  
  lgfx::Bus_Parallel8       _bus_instance;   
  lgfx::Touch_XPT2046          _touch_instance;



public:
  LGFX(void)
  {
    { 
      auto cfg = _bus_instance.config();    
       cfg.i2s_port = I2S_NUM_0;    
      cfg.freq_write = 20000000;    
      cfg.pin_wr =  4;              
      cfg.pin_rd =  2;              
      cfg.pin_rs = 15;              
      cfg.pin_d0 = 12;    
      cfg.pin_d1 = 13; 
      cfg.pin_d2 = 26;  
      cfg.pin_d3 = 25; 
      cfg.pin_d4 = 17;  
      cfg.pin_d5 = 16;  
      cfg.pin_d6 = 27;  
      cfg.pin_d7 = 14;  
      _bus_instance.config(cfg);    
      _panel_instance.setBus(&_bus_instance);     
    }

    { 
      auto cfg = _panel_instance.config();    
      cfg.pin_cs           =    5; 
      cfg.pin_rst          =    33; 
      cfg.pin_busy         =    -1;  
      
     
      cfg.panel_width      =   800;  
      cfg.panel_height     =   480;  
      cfg.offset_x         =     0;  
      cfg.offset_y         =     0;  
      cfg.offset_rotation  =     0;  
      cfg.dummy_read_pixel =     1;  
      cfg.dummy_read_bits  =     8;  
      cfg.readable         =  true;  
      cfg.invert           = false;  
      cfg.rgb_order        = false;  
      cfg.dlen_16bit       = false;  
      cfg.bus_shared       = false;  

      _panel_instance.config(cfg);
    }
    
    { 
      auto cfg = _touch_instance.config();
      cfg.x_min      = 0;    
      cfg.x_max      = 800;  
      cfg.y_min      = 0;    
      cfg.y_max      = 480;  
      cfg.pin_int    = -1;   
      cfg.bus_shared = false; 
      cfg.offset_rotation = 0;
      
      
      cfg.spi_host = VSPI_HOST;
      cfg.freq = 1000000;     
      cfg.pin_sclk = 18;     
      cfg.pin_mosi = 23;     
      cfg.pin_miso = 19;     
      cfg.pin_cs   = 21;     
      _touch_instance.config(cfg);
      _panel_instance.setTouch(&_touch_instance);  。
    }
    setPanel(&_panel_instance); 
  }
};

LGFX tft;

/*Change to your screen resolution*/
static const uint16_t screenWidth  = 800;
static const uint16_t screenHeight = 480;
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[ screenWidth * 10 ];

/* Display flushing */
void my_disp_flush( lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p )
{
   uint16_t w = ( area->x2 - area->x1 + 1 );
   uint16_t h = ( area->y2 - area->y1 + 1 );

   tft.startWrite();
   tft.setAddrWindow( area->x1, area->y1, w, h );
   //tft.pushColors( ( uint16_t * )&color_p->full, w * h, true );
   tft.writePixels((lgfx::rgb565_t *)&color_p->full, w * h);
   tft.endWrite();

   lv_disp_flush_ready( disp );
}

/*Read the touchpad*/
void my_touchpad_read( lv_indev_drv_t * indev_driver, lv_indev_data_t * data )
{
   uint16_t touchX, touchY;
   bool touched = tft.getTouch( &touchX, &touchY);
   if( !touched )
   {
      data->state = LV_INDEV_STATE_REL;
   }
   else
   {
      data->state = LV_INDEV_STATE_PR;

      /*Set the coordinates*/
      data->point.x = touchX;
      data->point.y = touchY;

      Serial.print( "Data x " );
      Serial.println( touchX );

      Serial.print( "Data y " );
      Serial.println( touchY );
   }
}

void setup()
{
   Serial.begin(115200);

   tft.begin();        
   tft.setRotation(2);
   tft.setBrightness(255);
   uint16_t calData[] = {3856, 3896, 3714, 308,  239, 3926, 233, 265};
   tft.setTouchCalibrate(calData);

   lv_init();
   lv_disp_draw_buf_init( &draw_buf, buf, NULL, screenWidth * 10 );

   /*Initialize the display*/
   static lv_disp_drv_t disp_drv;
   lv_disp_drv_init(&disp_drv);

   /*Change the following line to your display resolution*/
   disp_drv.hor_res = screenWidth;
   disp_drv.ver_res = screenHeight;
   disp_drv.flush_cb = my_disp_flush;
   disp_drv.draw_buf = &draw_buf;
   lv_disp_drv_register(&disp_drv);

   /*Initialize the (dummy) input device driver*/
   static lv_indev_drv_t indev_drv;
   lv_indev_drv_init(&indev_drv);
   indev_drv.type = LV_INDEV_TYPE_POINTER;
   indev_drv.read_cb = my_touchpad_read;
   lv_indev_drv_register(&indev_drv);

   ui_init();

   
}

void loop()
{
   lv_timer_handler(); /* let the GUI do its work */
   delay( 5 );
}

```

## //////////////////////////////////////////////////////////////////////////////////
- After this we will be opening Squareline studio 
- Squareline Setting
- Resolution => width 800 height 480
- Color depth => 16 bits
- Lvgl Version =? 8.3.3


## //////////////////////////////////////////////////////////////////////////////////
- Then apply changes in project Settings
- create Template Project and import files

- Files like Screen.c, helpers.c, ui.h, ui.c, and helpers.h are to be added where arduino project is created.

## //////////////////////////////////////////////////////////////////////////////////
- Open IDE and change lvgl/lvgl.h to lvgl.h and in screens change ../ui.h to ui.h
-Add huge app partition in IDE Tool setting adjust COM and Upload the code.

## //////////////////////////////////////////////////////////////////////////////////
- Now you have to play with Code and Squareline Studio for backend. As basic setup is completed

