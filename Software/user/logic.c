/*
 * logic.c
 *
 *  Created on: 16 feb 2020 �.
 *      Author: VasiliSk
 */
#include "Resconfig.h"
#include "levcan.h"
#include "levcan_objects.h"
#include "Data/parameters.h"
#include "logic.h"
#include "pwm.h"
#include "adc.h"
#include "network.h"
#include <string.h>

typedef struct
{
	struct
	{
		uint8_t Brake;
		int16_t MotorT;
		int16_t ContrT;
	} Inputs;
	union
	{
		uint8_t PWMFunctionOutputs[Func_MAX];
		struct
		{
			uint8_t PWM_Off;
			uint8_t PWM_On;
			uint8_t PWM_Button;
			uint8_t PWM_TurnLeft;
			uint8_t PWM_TurnRight;
			uint8_t PWM_Brake;
			uint8_t PWM_LowBeam;
			uint8_t PWM_HighBeam;
			uint8_t PWM_Reverse;
			uint8_t PWM_Horn;
			uint8_t PWM_MotorT;
			uint8_t PWM_ContrT;
			uint8_t PWM_T1;
			uint8_t PWM_T2;
		};
	};
} logicData_t;

//local functions
int utils_map_int(int x, int in_min, int in_max, int out_min, int out_max);
uint8_t getButton(uint8_t button);
//variables
//const char functions[] = "OFF\nON\nBTTN\nTL\nTR\nBK";
uint32_t logic_tick = 0;
LC_Obj_Buttons_t can_buttons = { 0 };
LC_Obj_Temperature_t can_contrTemp = { 0 };
logicData_t logicData = { 0 };
//int brightness = 0;

void LogicTick(uint32_t dt)
{
	logic_tick += dt;
	logicData.PWM_Off = 0;
	logicData.PWM_On = 100;
	int shutdown = 0;
	{	//Control
		static uint16_t sent_data = 0;
		sent_data += dt;
		if (Config.InputsCfg.SendControl && sent_data >= 20)
		{
			//50hz throttle and brake
			static LC_Obj_ThrottleV_t throttle = { 0 };
			static LC_Obj_BrakeV_t brake = { 0 };

			const LC_ObjectRecord_t throttle_send =
			{
				.Address = &throttle,
				.Size = sizeof(throttle),
				.Attributes.TCP = 0,
				.Attributes.Priority = LC_Priority_Mid,
				.NodeID = LC_Broadcast_Address
			};

			const LC_ObjectRecord_t brake_send =
			{
				.Address = &brake,
				.Size = sizeof(brake),
				.Attributes.TCP = 0,
				.Attributes.Priority = LC_Priority_Mid,
				.NodeID = LC_Broadcast_Address
			};

			throttle.ThrottleV = ADC_ValuesF.VThrottle;
			LC_SendMessage(LevcanNodePtr, (void*) &throttle_send, LC_Obj_ThrottleV);

			brake.BrakeV = ADC_ValuesF.VBrake;
			LC_SendMessage(LevcanNodePtr, (void*) &brake_send, LC_Obj_BrakeV);

			/*buttons.Brake = RD.Control.Embedd.Ports.Brake;
			 buttons.Cruise = RD.Control.Embedd.Ports.Cruise;
			 buttons.Speed = RD.Control.Embedd.Ports.Speed;
			 buttons.Reverse = RD.Control.Embedd.Ports.Reverse;
			 LC_SendMessage(LevcanNodePtr, (void*) &btns_send, LC_Obj_Buttons);*/

		}
		if (Config.InputsCfg.SendPorts && sent_data >= 20)
		{
			static LC_Obj_Buttons_t buttons;
			const LC_ObjectRecord_t btns_send =
			{
				.Address = &buttons,
				.Size = sizeof(buttons),
				.Attributes.TCP = 0,
				.Attributes.Priority = LC_Priority_Mid,
				.NodeID = LC_Broadcast_Address
			};

			memset(&buttons, 0, sizeof(buttons));

			if (Config.InputsCfg.SendPorts == 1)
			{
				buttons.ExButton1 = RD.Buttons.Int1;
				buttons.ExButton2 = RD.Buttons.Int2;
				buttons.ExButton3 = RD.Buttons.Int3;
				buttons.ExButton4 = RD.Buttons.Int4;
				buttons.ExButton5 = RD.Buttons.Int5;
				buttons.ExButton6 = RD.Buttons.Int6;
				buttons.ExButton7 = RD.Buttons.Int7_T1;
				buttons.ExButton8 = RD.Buttons.Int8_T2;

			}
			else if (Config.InputsCfg.SendPorts == 2)
			{
				buttons.ExButton9 = RD.Buttons.Int1;
				buttons.ExButton10 = RD.Buttons.Int2;
				buttons.ExButton11 = RD.Buttons.Int3;
				buttons.ExButton12 = RD.Buttons.Int4;
				buttons.ExButton13 = RD.Buttons.Int5;
				buttons.ExButton14 = RD.Buttons.Int6;
				buttons.ExButton15 = RD.Buttons.Int7_T1;
				buttons.ExButton16 = RD.Buttons.Int8_T2;
			}
			LC_SendMessage(LevcanNodePtr, (void*) &btns_send, LC_Obj_Buttons);
		}
		if (sent_data >= 20)
			sent_data = 0;
	}
	{	//Beam function
		int lbt = getButton(Config.Func.Beam.LowBeamButton);
		int hbt = getButton(Config.Func.Beam.HighBeamButton);

		if (lbt)
			logicData.PWM_LowBeam = Config.Func.Beam.LowDuty;
		else
			logicData.PWM_LowBeam = Config.Func.Beam.MinDuty;

		if (Config.Func.Beam.BeamsMode == Beams_Separate)
		{
			if (hbt)
				logicData.PWM_HighBeam = Config.Func.Beam.HighDuty;
			else
				logicData.PWM_HighBeam = 0;
		}
		else
		{ //one channel for beam
			if (hbt)
				logicData.PWM_LowBeam = Config.Func.Beam.HighDuty;

			logicData.PWM_HighBeam = logicData.PWM_LowBeam;
		}
		//Brightness mode
		/*switch (Config.InputsCfg.Brightness) {
		 case Brightness_OFF:
		 brightness=0;
		 break;
		 case Brightness_BeamLow:
		 brightness = lbt;
		 break;
		 case Brightness_BeamHigh:
		 brightness = hbt;
		 break;
		 }*/

	}

	{ //Brake
		if (can_buttons.Brake)
			logicData.PWM_Brake = Config.Func.Brake.HighBrakeDuty;
		else
			logicData.PWM_Brake = Config.Func.Brake.LowBrakeDuty;

		//Enable brake lights when VBrake > LowBrakeVoltage
		if (!can_buttons.Brake && Config.Func.Brake.LowBrakeVoltage > 0)
		{
			if (ADC_ValuesF.VBrake >= Config.Func.Brake.LowBrakeVoltage)
				logicData.PWM_Brake = Config.Func.Brake.HighBrakeDuty;
			else
				logicData.PWM_Brake = Config.Func.Brake.LowBrakeDuty;
		}
	}

	{ //Reverse
		if (can_buttons.Reverse)
			logicData.PWM_Reverse = Config.Func.Reverse.HighDuty;
		else
			logicData.PWM_Reverse = Config.Func.Reverse.LowDuty;
	}

	{ //Horn
		if (getButton(Config.Func.Horn.HornButton))
			logicData.PWM_Horn = 100;
		else
			logicData.PWM_Horn = 0;
	}

	{ // turn
		static uint32_t turn_timer = 0;
		static uint8_t state = 0;
		uint8_t any_button = getButton(Config.Func.Turns.LeftButton)
				| getButton(Config.Func.Turns.RightButton)
				| getButton(Config.Func.Turns.WarningButton);
		if (any_button)
		{
			turn_timer += dt;

			uint32_t maxtime = 0;
			if (!state) //inverted logic, get work time
				maxtime = Config.Func.Turns.OnTime * 100;
			else
				maxtime = Config.Func.Turns.OffTime * 100;

			if (turn_timer >= maxtime)
			{
				turn_timer = 0;
				state = !state;
			}
			if (Config.Func.Turns.OffTime == 0)
			{
				//change state back if off time is 0
				//helps to avoid small blinking
				state = 0;
			}
		}
		else
		{
			turn_timer = 0;
			state = 0;
		}

		if (state == 0 && (getButton(Config.Func.Turns.LeftButton) || getButton(Config.Func.Turns.WarningButton)))
			logicData.PWM_TurnLeft = Config.Func.Turns.HighDuty;
		else
			logicData.PWM_TurnLeft = 0;

		if (state == 0 && (getButton(Config.Func.Turns.RightButton) || getButton(Config.Func.Turns.WarningButton)))
			logicData.PWM_TurnRight = Config.Func.Turns.HighDuty;
		else
			logicData.PWM_TurnRight = 0;
	}

	{ //FAN control
		for (int i = 0; i < TsensFunc_MAX; i++)
		{
			int16_t input = 234;
			switch (i)
			{
			default: //motor
				input = can_contrTemp.ExternalTemp * 10;
				break;
			case TsensFunc_Contr:
				input = can_contrTemp.InternalTemp * 10;
				break;
			case TsensFunc_T1:
				input = ADC_ValuesF.T1;
				break;
			case TsensFunc_T2:
				input = ADC_ValuesF.T2;
				break;
			}
			int ratio = utils_map_int(input,
					(int) Config.Func.FanConrol.Tmin[i] * 10,
					(int) Config.Func.FanConrol.Tmax[i] * 10,
					Config.Func.FanConrol.OutMin[i],
					Config.Func.FanConrol.OutMax[i]);
			if (ratio < Config.Func.FanConrol.OutMin[i])
				ratio = Config.Func.FanConrol.OutMin[i];
			if (ratio > Config.Func.FanConrol.OutMax[i])
				ratio = Config.Func.FanConrol.OutMax[i];
			logicData.PWMFunctionOutputs[Func_MotorT + i] = ratio;
		}
	}

	if (Config.Func.AloneCANshutdown)
	{
		uint16_t pos = 0;
		LC_NodeShortName_t nname =
		{ 0 };
		int controller_or_lcd = 0;
		do
		{
			nname = LC_GetActiveNodes(LevcanNodePtr, &pos);
			if (nname.NodeID != LC_Broadcast_Address && (nname.DeviceType == LC_Device_Controller || nname.DeviceType == LC_Device_Display))
			{
				controller_or_lcd = 1;
				break;
			}
		} while (nname.NodeID != LC_Broadcast_Address);

		shutdown = !controller_or_lcd;
	}
	//apply pwm based on function
	for (int i = 0; i < 10; i++)
	{
		if (shutdown)
			PWMsetOutput(i, 0);
		else
		{
			uint8_t function = Config.PWMouts.PWMoutArray[i];
			if (function == 2)
			{
				//button logic here
				PWMsetOutput(i, logicData.PWMFunctionOutputs[function]);
			}
			else
				//classic function output
				PWMsetOutput(i, logicData.PWMFunctionOutputs[function]);
		}
	}
}

uint8_t getButton(uint8_t button)
{
	if (button < BtMax)
	{
		return RD.Buttons.ButtonArray[button];
	}
	else
		return 0;
}

void LogicProcessData(LC_NodeDescriptor_t *node, LC_Header_t header, void *data, int32_t size)
{
	(void) node;
	(void) size;
	static uint32_t last_tick_updated = 0;
	static LC_Obj_Buttons_t buttons = { 0 };
	static LC_Obj_Temperature_t controller_temp = { INT16_MIN, INT16_MIN, INT16_MIN, INT16_MIN };

	switch (header.MsgID)
	{
	case LC_Obj_ActiveFunctions:
	{
		LC_Obj_ActiveFunctions_t *func = data;
		buttons.Brake |= func->Brake;
		buttons.Reverse |= func->Reverse;
	}
		break;
	case LC_Obj_Buttons:
	{
		LC_Obj_Buttons_t *bttns = data;
		buttons.Buttons |= bttns->Buttons;
		buttons.ExtraButtons |= bttns->ExtraButtons;
	}
		break;
	case LC_Obj_Temperature:
	{
		LC_Obj_Temperature_t *temp = data;
		LC_NodeShortName_t sender = LC_GetNode(LevcanNodePtr, header.Source);
		if (sender.DeviceType == LC_Device_Controller)
		{
			if (temp->ExternalTemp > controller_temp.ExternalTemp)
				controller_temp.ExternalTemp = temp->ExternalTemp; //motor
			if (temp->InternalTemp > controller_temp.InternalTemp)
				controller_temp.InternalTemp = temp->InternalTemp; //FET
		}
	}
		break;
	}

	if (logic_tick - last_tick_updated >= 50)
	{
		//20hz can data update button and stuff
		can_buttons = buttons;
		can_contrTemp = controller_temp;

		buttons.Buttons = 0;
		buttons.ExtraButtons = 0;
		controller_temp = (LC_Obj_Temperature_t) { INT16_MIN, INT16_MIN, INT16_MIN, INT16_MIN };
		last_tick_updated = logic_tick;

		for (int i = 0; i < 16; i++)
			RD.Buttons.ButtonArray[i + BtExt_1] = (can_buttons.ExtraButtons & (1 << i)) ? 1 : 0;
		LC_SendRequest(LevcanNodePtr, LC_Broadcast_Address, LC_Obj_ActiveFunctions);
		LC_SendRequest(LevcanNodePtr, LC_Broadcast_Address, LC_Obj_Temperature);
	}
}

int utils_map_int(int x, int in_min, int in_max, int out_min, int out_max)
{
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
