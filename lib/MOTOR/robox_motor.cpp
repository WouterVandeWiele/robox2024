#include <Arduino.h>
#include <mutex>
#include "robox_motor.h"
#include "general_definitions.h"
#include "general_config.h"

#define MAX_12_BITS 4095

static TaskHandle_t motorTaskHandle;
QueueHandle_t xQueueMotor;

enum MotorMove {centerRotateRight, centerRotateLeft, onlyRotateRight, onlyRotateLeft, forwardReverse};

typedef struct MotorAction{
  MotorMove move;
  float speed;      // 0 - 1
} MotorAction;

void motor_loop(void* parameter) {

    RoboxMotor *motor = (RoboxMotor*) parameter;

    MotorAction action;

    while (true) {

        if (xQueueMotor == NULL) {
            delay(100);
            continue;
        }

        if(xQueueReceive( xQueueMotor, &(action), 0) == pdTRUE) {

            motor->enable();
                    
            switch (action.move)
            {
            case centerRotateRight:
                /* code */
                break;

            case centerRotateLeft:
                break;

            case onlyRotateRight:
                break;

            case onlyRotateLeft:
                break;

            case forwardReverse:
                break;
            
            default:
                break;
            }

            motor->shutdown();
        }
        delay(10);
    }
}

void RoboxMotor::init() {

    // pinMode(MOTOR_1_SPD, OUTPUT);
    // pinMode(MOTOR_2_SPD, OUTPUT);
    // analogWriteFrequency(MOTOR_PWM_FREQ);

    // speed pins
    ledcSetup(MOTOR_1_PWM_CHANNEL, MOTOR_PWM_FREQ, MOTOR_TIMER_BITS);
    ledcAttachPin(MOTOR_1_SPD, MOTOR_1_PWM_CHANNEL); 
    ledcSetup(MOTOR_2_PWM_CHANNEL, MOTOR_PWM_FREQ, MOTOR_TIMER_BITS);
    ledcAttachPin(MOTOR_2_SPD, MOTOR_2_PWM_CHANNEL);

    // direction pins
    pinMode(MOTOR_1_DIR, OUTPUT);
    pinMode(MOTOR_2_DIR, OUTPUT);

    // io expander pins

    // motor task loop (timed dance moves)
    // xQueueMotor = xQueueCreate(10, sizeof(MotorAction));
    // xTaskCreatePinnedToCore(
    //     motor_loop,       //Function to implement the task 
    //     "motor_task", //Name of the task
    //     1000,       //Stack size in words 
    //     (void *)this,       //Task input parameter 
    //     PRIORITY_MOTOR,          //Priority of the task 
    //     &motorTaskHandle,       //Task handle.
    //     1           // Core you want to run the task on (0 or 1)
    // );
}

void RoboxMotor::set_speed(float m1, float m2) {
    // m1 and m2 between 0 and 1

    ledcWrite(MOTOR_1_PWM_CHANNEL, (uint32_t)(MAX_12_BITS * m1));
    ledcWrite(MOTOR_2_PWM_CHANNEL, (uint32_t)(MAX_12_BITS * m2));
    // analogWrite(MOTOR_1_SPD, (uint32_t)(MAX_12_BITS * m1));
    // analogWrite(MOTOR_2_SPD, (uint32_t)(MAX_12_BITS * m2));
}

void RoboxMotor::set_direction(bool m1, bool m2) {
    digitalWrite(MOTOR_1_DIR, (m1) ? LOW : HIGH);
    digitalWrite(MOTOR_2_DIR, (m2) ? HIGH : LOW);
}

void RoboxMotor::enable() {
    io->set_output(LCD_CONTROL_PORT, (MOTOR_EN | MOTOR_STANDBY), (MOTOR_EN | MOTOR_STANDBY));
}

void RoboxMotor::shutdown() {
    io->set_output(LCD_CONTROL_PORT, ~(MOTOR_EN | MOTOR_STANDBY), (MOTOR_EN | MOTOR_STANDBY));
}


ExpanderConfig RoboxMotor::io_config() {
    ExpanderConfig config = RoboxIoExpander::get_default_config();

    config.out_state[LCD_CONTROL_PORT] = ~(MOTOR_EN | MOTOR_STANDBY);

    config.out_config[LCD_CONTROL_PORT] = ~(MOTOR_EN | MOTOR_STANDBY);

    // config.interrupts[LCD_CONTROL_PORT] = <fault sense pin>

    return config;
}

void RoboxMotor::io_interrupt_observer(std::vector<uint8_t>& data) {
    // RoboxLcdScreen * self = static_cast<RoboxLcdScreen*>(this_pointer);

    // implement callback code when an interrupt is generated
}