/*
 * sampleRecord.c
 *
 *  Created on: Feb 29, 2012
 *      Author: brent
 */
#include "sampleRecord.h"
#include "FreeRTOS.h"
#include "loggerConfig.h"

void initSampleRecord(LoggerConfig *loggerConfig,SampleRecord *sr){

	for (int i=0; i < CONFIG_ADC_CHANNELS; i++){
		ChannelSample *s = &(sr->ADCSamples[i]);
		ADCConfig *adcCfg = &(loggerConfig->ADCConfigs[i]);
		s->precision = adcCfg->loggingPrecision;
		s->channelConfig = &(adcCfg->cfg);
		s->intValue = NIL_SAMPLE;
	}
	for (int i=0; i < CONFIG_PWM_CHANNELS; i++){
		ChannelSample *s = &(sr->PWMSamples[i]);
		PWMConfig *pwmCfg = &(loggerConfig->PWMConfigs[i]);
		s->precision = pwmCfg->loggingPrecision;
		s->channelConfig = &(pwmCfg->cfg);
		s->intValue = NIL_SAMPLE;
	}
	for (int i=0; i < CONFIG_GPIO_CHANNELS; i++){
		ChannelSample *s = &(sr->GPIOSamples[i]);
		s->precision = DEFAULT_GPIO_LOGGING_PRECISION;
		s->channelConfig = &(loggerConfig->GPIOConfigs[i].cfg);
		s->intValue = NIL_SAMPLE;
	}
	for (int i=0; i < CONFIG_TIMER_CHANNELS; i++){
		ChannelSample *s = &(sr->TimerSamples[i]);
		TimerConfig *timerCfg = &(loggerConfig->TimerConfigs[i]);
		s->precision = timerCfg->loggingPrecision;
		s->channelConfig = &(timerCfg->cfg);
		s->intValue = NIL_SAMPLE;
	}
	for (int i=0; i < CONFIG_ACCEL_CHANNELS; i++){
		ChannelSample *s = &(sr->AccelSamples[i]);
		s->precision = DEFAULT_ACCEL_LOGGING_PRECISION;
		s->channelConfig = &(loggerConfig->AccelConfigs[i].cfg);
		s->intValue = NIL_SAMPLE;
	}
	{
		ChannelSample *s = &(sr->GPS_LatitueSample);
		s->precision = DEFAULT_GPS_POSITION_LOGGING_PRECISION;
		s->channelConfig = &(loggerConfig->GPSConfig.latitudeCfg);
		s->intValue = NIL_SAMPLE;
	}
	{
		ChannelSample *s = &(sr->GPS_LongitudeSample);
		s->precision = DEFAULT_GPS_POSITION_LOGGING_PRECISION;
		s->channelConfig = &(loggerConfig->GPSConfig.longitudeCfg);
		s->intValue = NIL_SAMPLE;
	}
	{
		ChannelSample *s = &(sr->GPS_VelocitySample);
		s->precision = DEFAULT_GPS_VELOCITY_LOGGING_PRECISION;
		s->channelConfig = &(loggerConfig->GPSConfig.velocityCfg);
		s->intValue = NIL_SAMPLE;
	}
	{
		ChannelSample *s = &(sr->GPS_TimeSample);
		s->precision = DEFAULT_GPS_TIME_LOGGING_PRECISION;
		s->channelConfig = &(loggerConfig->GPSConfig.timeCfg);
		s->intValue = NIL_SAMPLE;
	}
	{
		ChannelSample *s = &(sr->GPS_LapCountSample);
		s->precision = DEFAULT_LAP_COUNT_LOGGING_PRECISION;
		s->channelConfig = &(loggerConfig->GPSConfig.lapCountCfg);
		s->intValue = NIL_SAMPLE;
	}
	{
		ChannelSample *s = &(sr->GPS_LapTimeSample);
		s->precision = DEFAULT_LAP_TIME_LOGGING_PRECISION;
		s->channelConfig = &(loggerConfig->GPSConfig.lapTimeCfg);
		s->intValue = NIL_SAMPLE;
	}
	{
		ChannelSample *s = &(sr->GPS_QualitySample);
		s->precision = DEFAULT_GPS_QUALITY_LOGGING_PRECISION;
		s->channelConfig =  &(loggerConfig->GPSConfig.qualityCfg);
		s->intValue = NIL_SAMPLE;
	}
	{
		ChannelSample *s = &(sr->GPS_SatellitesSample);
		s->precision = DEFAULT_GPS_SATELLITES_LOGGING_PRECISION;
		s->channelConfig = &(loggerConfig->GPSConfig.satellitesCfg);
		s->intValue = NIL_SAMPLE;
	}
}

void clearSampleRecordBuffer(SampleRecord **srBuff, int size){
	for (int i=0;i < size; i++){
		SampleRecord *sr = srBuff[i];
		clearSampleRecord(sr);
	}
}

void clearSampleRecord(SampleRecord* sr){
	for (int s=0; s < SAMPLE_RECORD_CHANNELS; s++){
		sr->Samples[s].intValue = NIL_SAMPLE;
	}
}

SampleRecord ** createSampleRecordBuffer(LoggerConfig *loggerConfig, int size){
	SampleRecord ** srBuff = (SampleRecord **)pvPortMalloc(sizeof(SampleRecord[size]));
	initSampleRecordBuffer(loggerConfig, srBuff, size);
	return srBuff;
}

void initSampleRecordBuffer(LoggerConfig *loggerConfig, SampleRecord ** srBuff,int size){
	for (int i=0; i < size; i++) initSampleRecord(loggerConfig,srBuff[i]);
}

void freeSampleRecordBuffer(SampleRecord ** sampleRecordBuffer){
	vPortFree(sampleRecordBuffer);
}
