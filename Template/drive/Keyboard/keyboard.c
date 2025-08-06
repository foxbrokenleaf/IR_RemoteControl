#include "keyboard.h"

#define KEY_NUM 5
#define DEBOUNCE_TIME 3 // 3*10ms=30ms

typedef struct {
    uint8_t state;
    uint8_t cnt;
} KeyDebounce_t;

static KeyDebounce_t keyDebounce[KEY_NUM] = {0};
static const uint32_t keyGroup[KEY_NUM] = {KEY_1_2_3_GROUP, KEY_1_2_3_GROUP, KEY_1_2_3_GROUP, KEY_4_5_GROUP, KEY_4_5_GROUP};
static const uint32_t keyPin[KEY_NUM]   = {KEY_1, KEY_2, KEY_3, KEY_4, KEY_5};
static const KeyboardCode keyCode[KEY_NUM] = {KEY_CODE_U, KEY_CODE_L, KEY_CODE_D, KEY_CODE_R, KEY_CODE_O};

void Keyboard_Init(void){
    /* ʹ�� GPIOA �� GPIOC ʱ�� */
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOC);

    /* ���� GPIOA: PIN8, PIN9, PIN10 Ϊ����ģʽ������ */
    gpio_init(GPIOA, GPIO_MODE_IPD, GPIO_OSPEED_50MHZ, GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10);

    /* ���� GPIOC: PIN8, PIN9 Ϊ����ģʽ������ */
    gpio_init(GPIOC, GPIO_MODE_IPD, GPIO_OSPEED_50MHZ, GPIO_PIN_8 | GPIO_PIN_9);
}

KeyboardCode GetKeyboardCode(void){
    for(int i=0; i<KEY_NUM; i++) {
        uint8_t keyVal = (gpio_input_bit_get(keyGroup[i], keyPin[i]) == KEYBOARD_PRESS_V) ? 1 : 0;
        switch(keyDebounce[i].state) {
            case 0: // �ȴ�����
                if(keyVal) {
                    keyDebounce[i].state = 1;
                    keyDebounce[i].cnt = 1;
                }
                break;
            case 1: // ���¼���
                if(keyVal) {
                    keyDebounce[i].cnt++;
                    if(keyDebounce[i].cnt >= DEBOUNCE_TIME) {
                        keyDebounce[i].state = 2;
                        return keyCode[i]; // ������ȷ�ϰ���
                    }
                } else {
                    keyDebounce[i].state = 0;
                }
                break;
            case 2: // �ȴ��ɿ�
                if(!keyVal) {
                    keyDebounce[i].state = 0;
                }
                break;
        }
    }
    return UNPRESS;
}