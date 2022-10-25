// Components
#include "sra_board.h"
//#define debug

// C Headers
#include <stdio.h>
#include <math.h>

#define MAX_PWM (80.0f)
#define MIN_PWM (60.0f)

#define TAG "COMMS_WEBSOCKET_SERVER"

#include "comms_websocket_server.h"
servo_config servo_a = {
	.servo_pin = SERVO_A,
	.min_pulse_width = CONFIG_SERVO_A_MIN_PULSEWIDTH,
	.max_pulse_width = CONFIG_SERVO_A_MAX_PULSEWIDTH,
	.max_degree = CONFIG_SERVO_A_MAX_DEGREE,
	.mcpwm_num = MCPWM_UNIT_0,
	.timer_num = MCPWM_TIMER_0,
	.gen = MCPWM_OPR_A,
};

servo_config servo_b = {
	.servo_pin = SERVO_B,
	.min_pulse_width = CONFIG_SERVO_B_MIN_PULSEWIDTH,
	.max_pulse_width = CONFIG_SERVO_B_MAX_PULSEWIDTH,
	.max_degree = CONFIG_SERVO_B_MAX_DEGREE,
	.mcpwm_num = MCPWM_UNIT_0,
	.timer_num = MCPWM_TIMER_0,
	.gen = MCPWM_OPR_B,
};
servo_config servo_c = {
	.servo_pin = SERVO_C,
	.min_pulse_width = CONFIG_SERVO_C_MIN_PULSEWIDTH,
	.max_pulse_width = CONFIG_SERVO_C_MAX_PULSEWIDTH,
	.max_degree = CONFIG_SERVO_C_MAX_DEGREE,
	.mcpwm_num = MCPWM_UNIT_0,
	.timer_num = MCPWM_TIMER_1,
	.gen = MCPWM_OPR_A,
};

servo_config servo_d = {
	.servo_pin = SERVO_D,
	.min_pulse_width = CONFIG_SERVO_D_MIN_PULSEWIDTH,
	.max_pulse_width = CONFIG_SERVO_D_MAX_PULSEWIDTH,
	.max_degree = CONFIG_SERVO_D_MAX_DEGREE,
	.mcpwm_num = MCPWM_UNIT_0,
	.timer_num = MCPWM_TIMER_1,
	.gen = MCPWM_OPR_B,
};

void follow_commands_task(void *arg)
{
	ESP_ERROR_CHECK(enable_motor_driver(a, NORMAL_MODE));
	int curr_angle = 0, left_angle = 0, opens = 0, forwards = 0, p = 0, q = 0, r = 0, s = 0;
	enable_servo();
	while (true)
	{
		if (read_comms().val_changed)
		{
			float speed = read_comms().speed;

			ESP_LOGI(TAG, "speed: %f", speed);
			float motor_pwm = bound((speed), MIN_PWM, MAX_PWM);
			if (read_comms().forward_charge)
			{
				ESP_LOGI(TAG, "forward_charge");
				ESP_ERROR_CHECK(set_motor_speed(MOTOR_A_0, MOTOR_FORWARD, motor_pwm));
				ESP_ERROR_CHECK(set_motor_speed(MOTOR_A_1, MOTOR_FORWARD, motor_pwm));
				reset_val_changed_coms();
			}
			else if (read_comms().backward_charge)
			{
				ESP_LOGI(TAG, "backward_charge");
				ESP_ERROR_CHECK(set_motor_speed(MOTOR_A_0, MOTOR_BACKWARD, motor_pwm));
				ESP_ERROR_CHECK(set_motor_speed(MOTOR_A_1, MOTOR_BACKWARD, motor_pwm));
				reset_val_changed_coms();
			}
			else if (read_comms().right_charge)
			{
				ESP_LOGI(TAG, "right turn_charge");
				ESP_ERROR_CHECK(set_motor_speed(MOTOR_A_0, MOTOR_FORWARD, motor_pwm));
				ESP_ERROR_CHECK(set_motor_speed(MOTOR_A_1, MOTOR_BACKWARD, motor_pwm));
				reset_val_changed_coms();
			}
			else if (read_comms().left_charge)
			{
				ESP_LOGI(TAG, "left turn_charge");
				ESP_ERROR_CHECK(set_motor_speed(MOTOR_A_0, MOTOR_BACKWARD, motor_pwm));
				ESP_ERROR_CHECK(set_motor_speed(MOTOR_A_1, MOTOR_FORWARD, motor_pwm));
				reset_val_changed_coms();
			}
			else
			{
				ESP_LOGI(TAG, "stop");
				ESP_ERROR_CHECK(set_motor_speed(MOTOR_A_0, MOTOR_STOP, 0));
				ESP_ERROR_CHECK(set_motor_speed(MOTOR_A_1, MOTOR_STOP, 0));
			}

			p = curr_angle;
			while (read_comms().front)
			{
				ESP_LOGI(TAG, "up");
				ESP_ERROR_CHECK(set_angle_servo(&servo_b, p));
				vTaskDelay(5);

				// p++;
				p+=5;
				reset_val_changed_coms();
			}
			curr_angle = p;
			p = curr_angle;
			while (read_comms().back)
			{
				ESP_LOGI(TAG, "down");
				ESP_ERROR_CHECK(set_angle_servo(&servo_b, p));
				vTaskDelay(5);
				// p--;
				p-=5;
				reset_val_changed_coms();
			}
			curr_angle = p;
			q = left_angle;
			while (read_comms().right_turn)
			{
				ESP_LOGI(TAG, "right turn");
				set_angle_servo(&servo_a, q);
				vTaskDelay(5);
				// q--;
				q-=5;
				reset_val_changed_coms();
			}
			left_angle = q;
			q = left_angle;
			while (read_comms().left_turn)
			{
				ESP_LOGI(TAG, "left turn");
				set_angle_servo(&servo_a, q);
				vTaskDelay(5);
				// q++;
				q+=5;
				reset_val_changed_coms();
			}
			left_angle = q;
			r = forwards;
			while (read_comms().forward)
			{
				ESP_LOGI(TAG, "forward");
				set_angle_servo(&servo_d, r);
				vTaskDelay(5);
				// r++;
				r+=5;
				reset_val_changed_coms();
			}
			forwards = r;
			r = forwards;
			while (read_comms().backward)
			{
				ESP_LOGI(TAG, "backward");
				set_angle_servo(&servo_d, r);
				vTaskDelay(5);
				r-=5;
				// r--;
				reset_val_changed_coms();
			}
			s = opens;
			while (read_comms().open)
			{
				ESP_LOGI(TAG, "open");
				set_angle_servo(&servo_c, s);
				vTaskDelay(5);
				// s++;
				s+=5;
				reset_val_changed_coms();
			}
			opens = s;
			s = opens;
			while (read_comms().close)
			{
				ESP_LOGI(TAG, "close");
				set_angle_servo(&servo_c, s);
				vTaskDelay(5);
				// s--;
				s-=5;
				reset_val_changed_coms();
			}
			opens = s;
			while (read_comms().zero)
			{
				ESP_LOGI(TAG, "zero");
				set_angle_servo(&servo_a, 0);
				set_angle_servo(&servo_b, 0);
				set_angle_servo(&servo_c, 0);
				set_angle_servo(&servo_d, 0);
				curr_angle = 0, left_angle = 0, opens = 0, forwards = 0, p = 0, q = 0, r = 0, s = 0;
				vTaskDelay(5);
				reset_val_changed_coms();
			}

			ESP_LOGI(TAG, "stop");
			reset_val_changed_coms();

			// reset_val_changed_coms();
		}
		vTaskDelay(5);
	}
	vTaskDelete(NULL);
}

void app_main()
{
	start_websocket_server();

	xTaskCreate(&follow_commands_task, "follow commands task", 4096, NULL, 1, NULL);
}