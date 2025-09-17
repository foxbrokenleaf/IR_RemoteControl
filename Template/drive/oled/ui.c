#include "ui.h"

uint8_t index = 1;
uint8_t BowknotFlashFlag = 0;

UIIcon power = {
    .Index = 1,
    .Location = LOCATION_NULL,
    .Image = Power
};
UIIcon mode = {
    .Index = 2,
    .Location = LOCATION_NULL,
    .Image = SettingIcon,
    
};
UIIcon temp = {
    .Index = 3,
    .Location = LOCATION_NULL,
    .Image = Thermometer,
    
};
UIIcon fan = {
    .Index = 4,
    .Location = LOCATION_NULL,
    .Image = FanSpeedAuto,
    
};
UIIcon light = {
    .Index = 5,
    .Location = LOCATION_NULL,
    .Image = Light,
    
};

UIIcon Befor, Node, Next;

void UI_LocationLeft(const uint8_t *img){
    OLED_ShowImage(7, 16, 32, 32, img);
    if(Befor.Index == 3) OLED_ShowNum(22, 24, Befor.temp, 2, OLED_6X8);
}

void UI_LocationCenter(const uint8_t *img){
    OLED_ShowImage(48, 16, 32, 32, img);
    if(Node.Index == 3) OLED_ShowNum(63, 24, Node.temp, 2, OLED_6X8);
}

void UI_LocationRight(const uint8_t *img){
    OLED_ShowImage(89, 16, 32, 32, img);
    if(Next.Index == 3) OLED_ShowNum(104, 24, Next.temp, 2, OLED_6X8);
}

void UI_Template(KeyboardCode keyCode){

    if(keyCode == KEY_CODE_L) OLED_ShowImage(0, 28, 4, 8, LeftArrowFill);
    else OLED_ShowImage(0, 28, 4, 8, LeftArrow);
    if(keyCode == KEY_CODE_R) OLED_ShowImage(124, 28, 4, 8, RightArrowFill);
    else OLED_ShowImage(124, 28, 4, 8, RightArrow);
    if(BowknotFlashFlag) OLED_ShowImage(57, 51, 16, 16, BowknotFill);
    else OLED_ShowImage(57, 51, 16, 16, Bowknot);


    if(index == power.Index){
        Node = power;
        Next = mode;
    }
    else if(index == mode.Index){
        Befor = power;
        Node = mode;
        Next = temp;
    }
    else if(index == temp.Index){
        Befor = mode;
        Node = temp;
        Next = fan;
    }
    else if(index == fan.Index){
        Befor = temp;
        Node = fan;
        Next = light;
    }
    else if(index == light.Index){
        Befor = fan;
        Node = light;
    }
    Befor.Location = LOCATION_LEFT;
    Node.Location = LOCATION_CENTER;
    Next.Location = LOCATION_RIGHT;
    if(Befor.Index != 0 && Befor.Location == LOCATION_LEFT) UI_LocationLeft(Befor.Image);
    if(Node.Index != 0 && Node.Location == LOCATION_CENTER) UI_LocationCenter(Node.Image);
    if(Next.Index != 0 && Next.Location == LOCATION_RIGHT) UI_LocationRight(Next.Image);
    Befor.Index = 0;
    Node.Index = 0;
    Next.Index = 0;

    // if(FlashFlag) OLED_ShowImage(89, 16, 32, 32, Light);
    // else OLED_ShowImage(89, 16, 32, 32, LightFill);

    // if(speed == 0) OLED_ShowImage(89, 16, 32, 32, FanSpeedAuto);
    // else if(speed == 1) OLED_ShowImage(89, 16, 32, 32, FanSpeedLow);
    // else if(speed == 2) OLED_ShowImage(89, 16, 32, 32, FanSpeedMedium);
    // else if(speed == 3) OLED_ShowImage(89, 16, 32, 32, FanSpeedHigh);

    OLED_Update();    
}

void UpdateUIIconData(AirData *ad, KeyboardCode keyCode){
    if(keyCode == KEY_CODE_U || keyCode == KEY_CODE_D && index == 1) ad->MainData.power = !ad->MainData.power;
    if(ad->MainData.power) power.Image = PowerFill;
    else power.Image = Power;

    // if(ad->MainData.mode)

    if(keyCode == KEY_CODE_U && index == 3 && ad->MainData.temperature >= 16 && ad->MainData.temperature < 31) ad->MainData.temperature++;
    else if(keyCode == KEY_CODE_D && index == 3 && ad->MainData.temperature > 16 && ad->MainData.temperature <= 31) ad->MainData.temperature--;
    temp.temp = ad->MainData.temperature;
    
    if(keyCode == KEY_CODE_U && index == 4 && ad->MainData.fan_speed >= 0 && ad->MainData.fan_speed < 3) ad->MainData.fan_speed++;
    else if(keyCode == KEY_CODE_D && index == 4 && ad->MainData.fan_speed > 0 && ad->MainData.fan_speed <= 3) ad->MainData.fan_speed--;
    if(ad->MainData.fan_speed == FAN_AUTO) fan.Image = FanSpeedAuto;
    else if(ad->MainData.fan_speed == FAN_SLOW) fan.Image = FanSpeedLow;
    else if(ad->MainData.fan_speed == FAN_MEDIUM) fan.Image = FanSpeedMedium;
    else if(ad->MainData.fan_speed == FAN_HIGH) fan.Image = FanSpeedHigh;

    if(keyCode == KEY_CODE_U || keyCode == KEY_CODE_D && index == 5) ad->MainData.light = !ad->MainData.light;
    if(ad->MainData.light) light.Image = LightFill;
    else light.Image = Light;
}

void UI_Process(KeyboardCode keyCode){
    UI_Template(keyCode);
}
