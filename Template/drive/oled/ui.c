#include "ui.h"

U_SWING swingmode = NOSWING;

UI ui = UI_FUNCTION_LIST;
uint8_t index = 1;
uint8_t BowknotFlashFlag = 0;
uint8_t SingleShowFlag = 0;
uint8_t ArrowClickFlag = 0;

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
UIIcon swing = {
    .Index = 6,
    .Location = LOCATION_NULL,
    .Image = noSwingImg
};
UIIcon sleep = {
    .Index = 7,
    .Location = LOCATION_NULL,
    .Image = noSleepImg
};
UIIcon auxheat = {
    .Index = 8,
    .Location = LOCATION_NULL,
    .Image = noAuxHeat
};
UIIcon wifi = {
    .Index = 9,
    .Location = LOCATION_NULL,
    .Image = WiFiImg
};
UIIcon update = {
    .Index = 10,
    .Location = LOCATION_NULL,
    .Image = UpdateImg
};
UIIcon info = {
    .Index = 11,
    .Location = LOCATION_NULL,
    .Image = InfoImg
};
UIIcon Befor, Node, Next;

void UI_WiFi_Process(KeyboardCode keyCode){
    OLED_ClearArea(0, 8, 128, 56);
    
    OLED_ShowString(6, 23, " :Tenda_EXT", OLED_6X8);
    OLED_ShowImage(4, 21, 8, 8, single_img);
    OLED_ShowString(6, 33, " :wcx502503ok..", OLED_6X8);
    OLED_ShowImage(6, 33, 6, 8, wifi_key_img);

    if(keyCode == KEY_CODE_O){
        ui = UI_FUNCTION_LIST;
        OLED_Clear();
    }

    OLED_Update();
}

void UI_Update_Process(KeyboardCode keyCode){

    OLED_ShowImage(0, 8, 128, 56, Update_UI);

    if(keyCode == KEY_CODE_O){
        ui = UI_FUNCTION_LIST;
        OLED_Clear();
    }

    OLED_Update();
}

void UI_Info_Process(KeyboardCode keyCode){
    OLED_ClearArea(0, 8, 128, 56);
    OLED_ShowImage(16, 16, 32, 32, InfoIconImg);
    uint32_t UID1 = *(uint32_t *)(0x1FFFF7E8);
    uint32_t UID2 = *(uint32_t *)(0x1FFFF7EC);
    uint32_t UID3 = *(uint32_t *)(0x1FFFF7F0);
    OLED_DrawLine(53, 8, 53, 55);
    OLED_ShowString(76, 13, "UID", OLED_6X8);
    OLED_ShowHexNum(61, 23, UID1, 8, OLED_6X8);
    OLED_ShowHexNum(61, 33, UID2, 8, OLED_6X8);
    OLED_ShowHexNum(61, 43, UID3, 8, OLED_6X8);

    if(keyCode == KEY_CODE_O){
        ui = UI_FUNCTION_LIST;
        OLED_Clear();
    }

    OLED_Update();
}

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

void UIFunctionList(KeyboardCode keyCode){

    if(keyCode == KEY_CODE_O && index >= power.Index && index <= wifi.Index - 1) {
        OLED_ShowImage(120, 0, 8, 8, single_img);
        EventStartCounter = 0;
        SingleShowFlag = 1;
    }
    if(EventStartCounter > 250 && SingleShowFlag){
        SingleShowFlag = 0;
        OLED_ClearArea(120, 0, 8, 8);
    }
    if(EventStartCounter > 100 && ArrowClickFlag == 1)  ArrowClickFlag = 0;
    if(keyCode == KEY_CODE_L) {
        OLED_ShowImage(0, 28, 4, 8, LeftArrowFill);
        ArrowClickFlag = 1;
        EventStartCounter = 0;
    }
    if(!ArrowClickFlag)OLED_ShowImage(0, 28, 4, 8, LeftArrow);
    if(keyCode == KEY_CODE_R) {
        OLED_ShowImage(124, 28, 4, 8, RightArrowFill);
        ArrowClickFlag = 1;
        EventStartCounter = 0;
    }
    if(!ArrowClickFlag) OLED_ShowImage(124, 28, 4, 8, RightArrow);
    if(BowknotFlashFlag) OLED_ShowImage(57, 51, 16, 16, BowknotFill);
    else OLED_ShowImage(57, 51, 16, 16, Bowknot);


    if(index == power.Index){
        Befor = info;
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
        Next = swing;
    }
    else if(index == swing.Index){
        Befor = light;
        Node = swing;
        Next = sleep;
    }
    else if(index == sleep.Index){
        Befor = swing;
        Node = sleep;
        Next = auxheat;
    }
    else if(index == auxheat.Index){
        Befor = sleep;
        Node = auxheat;
        Next = wifi;
    }            
    else if(index == wifi.Index){
        Befor = auxheat;
        Node = wifi;
        Next = update;
    }
    else if(index == update.Index){
        Befor = wifi;
        Node = update;
        Next = info;
    }     
    else if(index == info.Index){
        Befor = update;
        Node = info;
        Next = power;
    }    

    if(keyCode == KEY_CODE_O){
        if(index == info.Index) ui = UI_INFO;
        else if(index == wifi.Index) ui = UI_WIFI;
        else if(index == update.Index) ui = UI_UPDATE;
    }
    if(keyCode == KEY_CODE_R){
        index += 1;
        index = index >= info.Index ? power.Index : index;
        
    }
    if(keyCode == KEY_CODE_L){
        index -= 1;
        index = index <= 0 ? info.Index : index;        
        
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

    OLED_Update();    
}

void UpdateUIIconData(AirData *ad, KeyboardCode keyCode){

    if(keyCode == KEY_CODE_U || keyCode == KEY_CODE_D){
        if(index == power.Index) ad->MainData.power = !ad->MainData.power;
        if(index == light.Index) ad->MainData.light = !ad->MainData.light;
        if(index == sleep.Index) ad->MainData.sleep_mode = !ad->MainData.sleep_mode;
        if(index == auxheat.Index) ad->MainData.auxiliary_heat = !ad->MainData.auxiliary_heat;
    }
    if(keyCode == KEY_CODE_U){
        if(index == mode.Index && (ad->MainData.mode >= MODE_AUTO && ad->MainData.mode < MODE_HOT)) ad->MainData.mode++;
        if(index == temp.Index && ad->MainData.temperature >= 16 && ad->MainData.temperature < 31) ad->MainData.temperature++;
        if(index == fan.Index && (ad->MainData.fan_speed >= FAN_AUTO && ad->MainData.fan_speed < FAN_HIGH)) ad->MainData.fan_speed++;
        if(index == swing.Index && swingmode >= NOSWING && swingmode < HV_SWING) swingmode++;
    }
    if(keyCode == KEY_CODE_D){
        if(index == mode.Index && (ad->MainData.mode > MODE_AUTO && ad->MainData.mode <= MODE_HOT)) ad->MainData.mode--;
        if(index == temp.Index && ad->MainData.temperature > 16 && ad->MainData.temperature <= 31) ad->MainData.temperature--;
        if(index == fan.Index && (ad->MainData.fan_speed > FAN_AUTO && ad->MainData.fan_speed <= FAN_HIGH)) ad->MainData.fan_speed--;
        if(index == swing.Index && swingmode > NOSWING && swingmode <= HV_SWING) swingmode--;
    }


    if(ad->MainData.power) power.Image = PowerFill;
    else power.Image = Power;

    if(ad->MainData.mode == MODE_AUTO) mode.Image = ModeAutoImg;
    else if(ad->MainData.mode == MODE_COLD) mode.Image = ModeSnowImg;
    else if(ad->MainData.mode == MODE_DRY) mode.Image = ModeDryImg;
    else if(ad->MainData.mode == MODE_FAN) mode.Image = ModeFanImg;
    else if(ad->MainData.mode == MODE_HOT) mode.Image = ModeHeatImg;

    temp.temp = ad->MainData.temperature;
    
    if(ad->MainData.fan_speed == FAN_AUTO) fan.Image = FanSpeedAuto;
    else if(ad->MainData.fan_speed == FAN_SLOW) fan.Image = FanSpeedLow;
    else if(ad->MainData.fan_speed == FAN_MEDIUM) fan.Image = FanSpeedMedium;
    else if(ad->MainData.fan_speed == FAN_HIGH) fan.Image = FanSpeedHigh;

    if(ad->MainData.light) light.Image = LightFill;
    else light.Image = Light;

    if(swingmode == NOSWING){
        ad->MainData.swing = 0;
        swing.Image = noSwingImg;
    }
    else if(swingmode == H_SWING){
        ad->MainData.swing = 1;
        ad->AuxData.horizontal_swing = 1;
        swing.Image = HorizontalSwingImg;
    }
    else if(swingmode == V_SWING){
        ad->MainData.swing = 1;
        ad->AuxData.vertical_swing = 1;
        swing.Image = VerticalSwingImg;
    } 
    else if(swingmode == HV_SWING){
        ad->MainData.swing = 1;
        ad->AuxData.horizontal_swing = 1;
        ad->AuxData.vertical_swing = 1;
        swing.Image = HorizontalVerticalSwingImg;
    }

    if(ad->MainData.sleep_mode) sleep.Image = isSleepImg;
    else sleep.Image = noSleepImg;

    if(ad->MainData.auxiliary_heat) auxheat.Image = AuxHeatImg;
    else auxheat.Image = noAuxHeat;

}

void UI_Process(KeyboardCode keyCode){
    
    uint8_t ChargeFlag = 1;
    uint8_t battary_value = 100;
    
    if(battary_value % 100 != 0){
        OLED_ShowNum(0, 0, battary_value, 2, OLED_6X8);
        OLED_ShowChar(12, 0, '%', OLED_6X8);
        if(ChargeFlag) OLED_ShowImage(18, 0, 8, 8, ChargeImg);
        else OLED_ShowImage(18, 0, 8, 8, BattaryImg);
    }else{
        OLED_ShowNum(0, 0, battary_value, 3, OLED_6X8);
        OLED_ShowChar(18, 0, '%', OLED_6X8);
        if(ChargeFlag) OLED_ShowImage(24, 0, 8, 8, ChargeImg);
        else OLED_ShowImage(24, 0, 8, 8, BattaryImg);        
    }
    


    switch (ui)
    {
    case UI_FUNCTION_LIST:
        UIFunctionList(keyCode);
        break;
    case UI_INFO:
        UI_Info_Process(keyCode);
        break;
    case UI_WIFI:
        UI_WiFi_Process(keyCode);
        break;
    case UI_UPDATE:
        UI_Update_Process(keyCode);
        break;
    default:
        UIFunctionList(keyCode);
        break;
    }
}
