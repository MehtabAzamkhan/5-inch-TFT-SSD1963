#include <lvgl.h>
#define LGFX_USE_V1
#include <LovyanGFX.hpp>
#include "ui.h"





const int relay = 21;
volatile bool ledState=false;
// int values[11];



class LGFX : public lgfx::LGFX_Device {
  lgfx::Panel_SSD1963 _panel_instance;
  lgfx::Bus_Parallel8 _bus_instance;
  lgfx::Touch_XPT2046 _touch_instance;





public:
  LGFX(void) {
    {
      auto cfg = _bus_instance.config();
      cfg.i2s_port = I2S_NUM_0;
      cfg.freq_write = 20000000;
      cfg.pin_wr = 4;
      cfg.pin_rd = 2;
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
      cfg.pin_cs = 5;
      cfg.pin_rst = 33;
      cfg.pin_busy = -1;


      cfg.panel_width = 800;
      cfg.panel_height = 480;
      cfg.offset_x = 0;
      cfg.offset_y = 0;
      cfg.offset_rotation = 0;
      cfg.dummy_read_pixel = 8;
      cfg.dummy_read_bits = 1;
      cfg.readable = true;
      cfg.invert = false;
      cfg.rgb_order = true;
      cfg.dlen_16bit = false;
      cfg.bus_shared = false;

      _panel_instance.config(cfg);
    }

    {
      auto cfg = _touch_instance.config();
      cfg.x_min = 0;
      cfg.x_max = 800;
      cfg.y_min = 0;
      cfg.y_max = 480;
      cfg.pin_int = -1;
      cfg.bus_shared = false;
      cfg.offset_rotation = 0;


      cfg.spi_host = VSPI_HOST;
      cfg.freq = 1000000;
      cfg.pin_sclk = 18;
      cfg.pin_mosi = 23;
      cfg.pin_miso = 19;
      cfg.pin_cs = 21;
      _touch_instance.config(cfg);
      _panel_instance.setTouch(&_touch_instance);
    }
    setPanel(&_panel_instance);
  }
};

LGFX tft;

/*Change to your screen resolution*/
static const uint16_t screenWidth = 800;
static const uint16_t screenHeight = 480;
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[screenWidth * 10];

/* Display flushing */
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {

  uint16_t w = (area->x2 - area->x1 + 1);
  uint16_t h = (area->y2 - area->y1 + 1);

  tft.startWrite();
  tft.setAddrWindow(area->x1, area->y1, w, h);
 tft.pushColors( ( uint16_t * )&color_p->full, w * h, true );
 tft.writePixels((lgfx::rgb565_t *)&color_p->full, w * h);
  tft.endWrite();

  lv_disp_flush_ready(disp);
}

/*Read the touchpad*/
void my_touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data) {
  uint16_t touchX, touchY;
  bool touched = tft.getTouch(&touchX, &touchY);
  if (!touched) {
    data->state = LV_INDEV_STATE_REL;
  } else {
    data->state = LV_INDEV_STATE_PR;

    /*Set the coordinates*/
    data->point.x = touchX;
    data->point.y = touchY;

   // Serial.print("Data x ");
    Serial.println(touchX);

    //Serial.print("Data y ");
    Serial.println(touchY);
  }
}


void setup() {
   Serial.begin(115200);
   pinMode(relay, OUTPUT);
   tft.begin();        
   tft.setRotation(2);
   tft.setBrightness(255);
   uint16_t calData[] = {3924, 2807, 3917, 319, 807, 2712, 728, 294};
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
   
    
  // lv_obj_add_event_cb(ui_ImgButton4, LED_on_1, LV_EVENT_ALL, NULL);
   ui_Screen2_screen_init();
   lv_obj_add_event_cb(ui_ImgButton4, my_event_cb, LV_EVENT_CLICKED, NULL);
}

void loop() {

    lv_timer_handler(); /* let the GUI do its work */
    digitalWrite(relay, ledState);
    
    delay(5);
}

// 


static void my_event_cb(lv_event_t *event) {
  // Toggle the LED state
  ledState = !ledState;

  // Update the state of the button based on the LED state
  lv_imgbtn_set_src(ui_ImgButton4, LV_IMGBTN_STATE_RELEASED, NULL, &ui_img_244644622, NULL);
  lv_imgbtn_set_src(ui_ImgButton4, LV_IMGBTN_STATE_PRESSED, NULL, &ui_img_group_154_png, NULL);

  // Print a message (optional)
  printf("Clicked. LED state: %s\n", ledState ? "ON" : "OFF");

  // Add any additional actions you want to perform when the button is clicked

  // You can also control the physical LED directly here if needed
   digitalWrite(relay, ledState);
}


// static void event_handler(lv_event_t * e)
// {
//     lv_event_code_t code = lv_event_get_code(e);

//    if(code == LV_EVENT_VALUE_CHANGED) {
//         Serial.println("Relay turned CHNAFED REACHED");
//         digitalWrite(relay, HIGH);
//         Serial.println("Relay turned ON");
//     }
// }



// static void LED_on_1(lv_event_t *e)
// {
//     lv_event_code_t code = lv_event_get_code(e);
//     lv_obj_t *ui_ImgButton4 = lv_event_get_target(e);
    
//     if (code == LV_EVENT_VALUE_CHANGED) { 
//         // Button is checked (pressed)
//         ledState = true;
//         digitalWrite(relay, HIGH);
//         Serial.println("Relay turned ON");
//     } else if (code == LV_EVENT_RELEASED) {
//         // Button is unchecked (released)
//         ledState = false;
//         digitalWrite(relay, LOW);
//         Serial.println("Relay turned OFF");
//     }
// }
// static void event_handler(lv_event_t * e)
// {
//     lv_event_code_t code = lv_event_get_code(e);

//     if(code == LV_EVENT_CLICKED) {
//         LV_LOG_USER("Clicked");
//     }
//     else if(code == LV_EVENT_VALUE_CHANGED) {
//         LV_LOG_USER("Toggled");
//     }
// }



// static void LED(lv_obj_t *obj, lv_event_t event) {
//     if (event == LV_EVENT_VALUE_CHANGED) {
//         if (lv_imgbtn_get_state(obj) == LV_STATE_CHECKED) {
//             // Button is checked (pressed)
//             ledState = true;
//             digitalWrite(relay, HIGH);
//             Serial.println("Relay turned ON");
//         } else {
//             // Button is unchecked (released)
//             ledState = false;
//             digitalWrite(relay, LOW);
//             Serial.println("Relay turned OFF");
//         }
//     }
// }


  
  
// }




////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//for ON Button with interval
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// static void btn_event_cb(lv_event_t * e)
// {
//     lv_event_code_t code = lv_event_get_code(e);
//     lv_obj_t * btn = lv_event_get_target(e);
//     if(code == LV_EVENT_CLICKED) {
//         digitalWrite(relay, LOW);
//         Serial.print("Hello low");
//         delay(5000);
//         digitalWrite(relay, HIGH);
//         Serial.print("Hello High");
//         delay(5000);
//         digitalWrite(relay, LOW);
//         Serial.print("Hello low");
//         delay(5000);
//         digitalWrite(relay, HIGH);
//         Serial.print("Hello High");




//         /*Get the first child of the button which is the label and change its text*/
//         lv_obj_t * label = lv_obj_get_child(btn, 0);
        
//     }
// }

// /**
//  * Create a button with a label and react on click event.
//  */
// void lv_example_get_started_1(void)
// {
//     lv_obj_t * btn = lv_btn_create(lv_scr_act());     /*Add a button the current screen*/
//     lv_obj_set_pos(btn, 10, 10);                            /*Set its position*/
//     lv_obj_set_size(btn, 200, 200);                          /*Set its size*/
//     lv_obj_add_event_cb(btn, btn_event_cb, LV_EVENT_ALL, NULL);           /*Assign a callback to the button*/

//     lv_obj_t * label = lv_label_create(btn);          /*Add a label to the button*/
//     lv_label_set_text(label, "Button");                     /*Set the labels text*/
//     lv_obj_center(label);
// }
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




