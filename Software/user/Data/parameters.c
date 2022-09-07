#include <stdio.h>
#include "parameters.h"
#include "string.h"
#include "levcan_paramserver.h"
#include "Resconfig.h"
#include "armutils.h"

//#define RUSSIAN
#include "directories.h"

volatile ConfigStruct_t Config;
volatile DataStruct_t Data;
volatile LifeDataStruct_t LifeData;
volatile RuntimeStruct_t RD = { 0 };

const intptr_t *Storage_Address[Save_number] =
{ (intptr_t*) &Data, (intptr_t*) &Config, (intptr_t*) &LifeData };
const uint16_t Storage_Size[Save_number] =
{ sizeof(Data), sizeof(Config), sizeof(LifeData) };

const uint16_t PWMIO_Freq[] = { 100, 500, 1000, 5000, 10000, 24000 };

void LoadDefaultParameters(void)
{

	memset((void*) &Config, 0, sizeof(Config));

	Config.PWMouts.PWM1_2freq = 1;
	Config.PWMouts.PWM3_4freq = 1;
	Config.PWMouts.PWM5_6freq = 1;
	Config.PWMouts.PWM1out = Func_LowBeam;
	Config.PWMouts.PWM2out = Func_HighBeam;
	Config.PWMouts.PWM3out = Func_Brake;
	Config.PWMouts.PWM4out = Func_Horn;
	Config.PWMouts.PWM5out = Func_TurnLeft;
	Config.PWMouts.PWM6out = Func_TurnRight;

	Config.Func.Beam.BeamsMode = Beams_Separate;
	Config.Func.Beam.HighBeamButton = BtExt_8;
	Config.Func.Beam.LowBeamButton = BtEnabled;
	Config.Func.Beam.HighDuty = 100;
	Config.Func.Beam.LowDuty = 100;
	Config.Func.Beam.MinDuty = 20;

	Config.Func.Reverse.HighDuty = 100;
	Config.Func.Reverse.LowDuty = 0;

	Config.Func.Brake.LowBrakeDuty = 20;
	Config.Func.Brake.HighBrakeDuty = 100;

	Config.Func.Horn.HornButton = BtExt_11;

	Config.Func.Turns.LeftButton = BtExt_9;
	Config.Func.Turns.RightButton = BtExt_10;
	Config.Func.Turns.HighDuty = 100;
	Config.Func.Turns.OnTime = 4; //400ms
	Config.Func.Turns.OffTime = 4; //400ms

	Config.InputsCfg.BrakeMax = 4000;
	Config.InputsCfg.BrakeMin = 1000;
	Config.InputsCfg.ThrottleMax = 4000;
	Config.InputsCfg.ThrottleMin = 1000;
	Config.InputsCfg.SendControl = 0;
	Config.InputsCfg.SendPorts = 0;
	Config.InputsCfg.T1_Threshold = 50;
	Config.InputsCfg.T2_Threshold = 50;
	//Config.InputsCfg.Brightness = Brightness_BeamHigh;

	for (int i = 0; i < TsensFunc_MAX; i++)
	{
		Config.Func.FanConrol.Tmin[i] = 40;
		Config.Func.FanConrol.Tmax[i] = 60;
		Config.Func.FanConrol.OutMin[i] = 0;
		Config.Func.FanConrol.OutMax[i] = 100;
	}
}
