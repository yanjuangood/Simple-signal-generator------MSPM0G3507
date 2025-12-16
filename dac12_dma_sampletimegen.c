#include "ti_msp_dl_config.h"
#include "Freq_ffz.h"
/*正弦波（64）*/
const uint16_t gOutputSignalSine64[] = {2048, 2248, 2447, 2642, 2831, 3013,
    3185, 3347, 3496, 3631, 3750, 3854, 3940, 4007, 4056, 4086, 4095, 4086,
    4056, 4007, 3940, 3854, 3750, 3631, 3496, 3347, 3185, 3013, 2831, 2642,
    2447, 2248, 2048, 1847, 1648, 1453, 1264, 1082, 910, 748, 599, 464, 345,
    241, 155, 88, 39, 9, 0, 9, 39, 88, 155, 241, 345, 464, 599, 748, 910, 1082,
    1264, 1453, 1648, 1847};
/* 三角波（64） */
const uint16_t gOutputSignalTriangle64[] = {
    2048, 2176, 2304, 2432, 2560, 2688, 2816, 2944, 3072, 3200, 3328, 3456, 3584, 3712, 3840, 3968,
    4095, 3968, 3840, 3712, 3584, 3456, 3328, 3200, 3072, 2944, 2816, 2688, 2560, 2432, 2304, 2176,
    2048, 1920, 1792, 1664, 1536, 1408, 1280, 1152, 1024,  896,  768,  640,  512,  384,  256,  128,
       0,  128,  256,  384,  512,  640,  768,  896, 1024, 1152, 1280, 1408, 1536, 1664, 1792, 1920
};
/* 方波（64） */
const uint16_t gOutputSignalSquare64[] = {
    4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095,
    4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095,
       0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
       0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0
};
		
uint8_t Key_Num = 0;
int main(void)
{
	// 定义一个状态变量：0=正弦波, 1=三角波
    uint8_t currentWaveform = 0;
	
		uint32_t Freq = 100;
	
    SYSCFG_DL_init();

	  DL_TimerG_startCounter(TIMG0);

    DL_DMA_setSrcAddr(DMA, DMA_CH0_CHAN_ID, (uint32_t) &gOutputSignalSine64[0]);
    DL_DMA_setDestAddr(DMA, DMA_CH0_CHAN_ID, (uint32_t) & (DAC0->DATA0));
    DL_DMA_setTransferSize(DMA, DMA_CH0_CHAN_ID, sizeof(gOutputSignalSine64) / sizeof(uint16_t));
    DL_DMA_enableChannel(DMA, DMA_CH0_CHAN_ID);

		
    while (1) 
		{
			if(!DL_GPIO_readPins(GPIOB,DL_GPIO_PIN_21))
			{
				DL_Common_delayCycles(160000);
				
				//按键改变波形频率（PB21）
				while(!DL_GPIO_readPins(GPIOB,DL_GPIO_PIN_21));
				Freq += 100;
				uint32_t newPeriod = Freq_change(Freq);
				DL_TimerG_stopCounter(TIMG0);  
        // 2. 直接设置新的 Load Value (周期值)
        // 这种方法直接修改寄存器，避开了 gTIMER_0TimerConfig 变量副本的问题
        DL_TimerG_setLoadValue(TIMG0, newPeriod);
            
        // 3.重新启动定时器！(这是波形消失的直接原因)
        DL_TimerG_startCounter(TIMG0);
			}	
			
			//波形切换按键(PB1)
			if(!DL_GPIO_readPins(GPIOB,DL_GPIO_PIN_1))
			{
				DL_Common_delayCycles(160000);
				while(!DL_GPIO_readPins(GPIOB,DL_GPIO_PIN_1));
				DL_Common_delayCycles(160000);
				currentWaveform = currentWaveform + 1; // 0->1->2->0
				if(currentWaveform > 2)
				{
					currentWaveform = 0;
				}
				DL_TimerG_stopCounter(TIMG0);
            // === 关键：切换 DMA 源地址 ===
            // (A) 先禁用 DMA 通道，确保安全
            DL_DMA_disableChannel(DMA, DMA_CH0_CHAN_ID);

            // (B) 根据状态选择新的源地址
            if(currentWaveform == 0)
            {
                // 切换回正弦波
                DL_DMA_setSrcAddr(DMA, DMA_CH0_CHAN_ID, (uint32_t) &gOutputSignalSine64[0]);
            }
            else if(currentWaveform == 1)
            {
                // 切换到三角波
                DL_DMA_setSrcAddr(DMA, DMA_CH0_CHAN_ID, (uint32_t) &gOutputSignalTriangle64[0]);
            }
						else if(currentWaveform == 2)
            {
                // 切换到三角波
                DL_DMA_setSrcAddr(DMA, DMA_CH0_CHAN_ID, (uint32_t) &gOutputSignalSquare64[0]);
            }
						DL_DMA_setTransferSize(DMA, DMA_CH0_CHAN_ID, 64);
            // (C) 重新启用 DMA 通道
            DL_DMA_enableChannel(DMA, DMA_CH0_CHAN_ID);
						DL_TimerG_setTimerCount(TIMG0, 0);
						 DL_TimerG_startCounter(TIMG0);
        }
				
			}
    }





