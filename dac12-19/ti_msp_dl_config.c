/*
 *  ============ ti_msp_dl_config.c =============
 *  Configured MSPM0 DriverLib module definitions
 *
 *  DO NOT EDIT - This file is generated for the LP_MSPM0G3507
 *  by the SysConfig tool.
 */

#include "ti_msp_dl_config.h"
#include "Freq_ffz.h"


volatile double gTIMER_0Period = 4999U;
/*
 *  ======== SYSCFG_DL_init ========
 *  Perform any initialization needed before using any board APIs
 */
DL_TimerA_backupConfig gCAPTURE_0Backup;

SYSCONFIG_WEAK void SYSCFG_DL_init(void)
{
    SYSCFG_DL_initPower();
    SYSCFG_DL_GPIO_init();
    /* Module-Specific Initializations*/
    SYSCFG_DL_SYSCTL_init();
    SYSCFG_DL_DMA_init();
    SYSCFG_DL_DAC12_init();
	  SYSCFG_DL_TIMER_0_init();
	gCAPTURE_0Backup.backupRdy 	= false;
}


SYSCONFIG_WEAK void SYSCFG_DL_initPower(void)
{
    DL_GPIO_reset(GPIOA);
    DL_GPIO_reset(GPIOB);
		DL_TimerG_reset(TIMER_0_INST);
    DL_DAC12_reset(DAC0);

    DL_GPIO_enablePower(GPIOA);
    DL_GPIO_enablePower(GPIOB);
    DL_TimerG_enablePower(TIMER_0_INST);
    DL_DAC12_enablePower(DAC0);
    delay_cycles(POWER_STARTUP_DELAY);
}

SYSCONFIG_WEAK void SYSCFG_DL_GPIO_init(void)
{
				DL_GPIO_initDigitalInputFeatures(IOMUX_PINCM17,
				DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_PULL_UP,
				DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);

				DL_GPIO_initDigitalInputFeatures(IOMUX_PINCM13,
				DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_PULL_UP,
				DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);
			
				DL_GPIO_initDigitalInputFeatures(IOMUX_PINCM14,
        DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_PULL_UP,
        DL_GPIO_HYSTERESIS_ENABLE, DL_GPIO_WAKEUP_DISABLE);

//				DL_GPIO_initPeripheralAnalogFunction(IOMUX_PINCM37); // PINCM37 对应 PA15
	
    DL_GPIO_setLowerPinsInputFilter(GPIOB, DL_GPIO_PIN_4_INPUT_FILTER_8_CYCLES |
		DL_GPIO_PIN_1_INPUT_FILTER_8_CYCLES);
	
    DL_GPIO_initDigitalInputFeatures(IOMUX_PINCM49,
													DL_GPIO_INVERSION_DISABLE, 
													DL_GPIO_RESISTOR_PULL_UP,
													DL_GPIO_HYSTERESIS_ENABLE, DL_GPIO_WAKEUP_DISABLE);

    DL_GPIO_setPins(GPIOA, DL_GPIO_PIN_0);
    DL_GPIO_enableOutput(GPIOA, DL_GPIO_PIN_0);

}



SYSCONFIG_WEAK void SYSCFG_DL_SYSCTL_init(void)
{

		//Low Power Mode is configured to be SLEEP0
    DL_SYSCTL_setBORThreshold(DL_SYSCTL_BOR_THRESHOLD_LEVEL_0);

    
		DL_SYSCTL_setSYSOSCFreq(DL_SYSCTL_SYSOSC_FREQ_BASE);
		/* Set default configuration */
		DL_SYSCTL_disableHFXT();
		DL_SYSCTL_disableSYSPLL();
	    DL_SYSCTL_enableMFCLK();
    DL_SYSCTL_enableMFPCLK();
		DL_SYSCTL_setMFPCLKSource(DL_SYSCTL_MFPCLK_SOURCE_SYSOSC);

}


static const DL_DMA_Config gDMA_CH0Config = {
    .transferMode   = DL_DMA_FULL_CH_REPEAT_SINGLE_TRANSFER_MODE,
    .extendedMode   = DL_DMA_NORMAL_MODE,
    .destIncrement  = DL_DMA_ADDR_UNCHANGED,
    .srcIncrement   = DL_DMA_ADDR_INCREMENT,
    .destWidth      = DL_DMA_WIDTH_HALF_WORD,
    .srcWidth       = DL_DMA_WIDTH_HALF_WORD,
    .trigger        = DAC12_INST_DMA_TRIGGER,
    .triggerType    = DL_DMA_TRIGGER_TYPE_EXTERNAL,
};

SYSCONFIG_WEAK void SYSCFG_DL_DMA_CH0_init(void)
{
    DL_DMA_initChannel(DMA, DMA_CH0_CHAN_ID , (DL_DMA_Config *) &gDMA_CH0Config);
}
SYSCONFIG_WEAK void SYSCFG_DL_DMA_init(void){
    SYSCFG_DL_DMA_CH0_init();
}


static const DL_DAC12_Config gDAC12Config = {
    .outputEnable              = DL_DAC12_OUTPUT_ENABLED,
    .resolution                = DL_DAC12_RESOLUTION_12BIT,
    .representation            = DL_DAC12_REPRESENTATION_BINARY,
    .voltageReferenceSource    = DL_DAC12_VREF_SOURCE_VDDA_VSSA,
    .amplifierSetting          = DL_DAC12_AMP_ON,
    .fifoEnable                = DL_DAC12_FIFO_ENABLED,
    .fifoTriggerSource         = DL_DAC12_FIFO_TRIGGER_HWTRIG0,
    .dmaTriggerEnable          = DL_DAC12_DMA_TRIGGER_ENABLED,
    .dmaTriggerThreshold       = DL_DAC12_FIFO_THRESHOLD_TWO_QTRS_EMPTY,
    .sampleTimeGeneratorEnable = DL_DAC12_SAMPLETIMER_DISABLE,
    .sampleRate                = DL_DAC12_SAMPLES_PER_SECOND_1M,
};
SYSCONFIG_WEAK void SYSCFG_DL_DAC12_init(void)
{
    DL_DAC12_init(DAC0, (DL_DAC12_Config *) &gDAC12Config);
    DL_DAC12_enableInterrupt(DAC0, (DL_DAC12_INTERRUPT_DMA_DONE));
    DL_DAC12_setSubscriberChanID(DAC0,DL_DAC12_SUBSCRIBER_INDEX_0,1);
	  DL_DAC12_enable(DAC0);
}


/*
 * Timer clock configuration to be sourced by MFCLK /  (4000000 Hz)
 * timerClkFreq = (timerClkSrc / (timerClkDivRatio * (timerClkPrescale + 1)))
 *   15625 Hz = 4000000 Hz / (1 * (255 + 1))
 */
static const DL_TimerG_ClockConfig gTIMER_0ClockConfig = {
    .clockSel    = DL_TIMER_CLOCK_BUSCLK,
    .divideRatio = DL_TIMER_CLOCK_DIVIDE_1,
    .prescale    = 0U,
};

/*
 * Timer load value (where the counter starts from) is calculated as (timerPeriod * timerClockFreq) - 1
 * TIMER_0_INST_LOAD_VALUE = (0.00015625 s * 32000000 Hz) - 1
 */
static DL_TimerG_TimerConfig gTIMER_0TimerConfig = 
{
    .period     = 4999U,
    .timerMode  = DL_TIMER_TIMER_MODE_PERIODIC,
    .startTimer = DL_TIMER_STOP,
};

SYSCONFIG_WEAK void SYSCFG_DL_TIMER_0_init(void)
{

    DL_TimerG_setClockConfig(TIMG0,
        (DL_TimerG_ClockConfig *) &gTIMER_0ClockConfig);

    DL_TimerG_initTimerMode(TIMG0,
        (DL_TimerG_TimerConfig *) &gTIMER_0TimerConfig);
    DL_TimerG_enableClock(TIMG0);
	 
	  DL_TimerG_enableEvent(TIMG0, DL_TIMER_EVENT_ROUTE_1, (DL_TIMER_EVENT_ZERO_EVENT));
		DL_Timer_setPublisherChanID(TIMG0,DL_TIMER_PUBLISHER_INDEX_0,1);
}