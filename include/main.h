#ifndef MAIN_H
#define MAIN_H

#define SDRAM_START_ADR             (uint32_t)0xD0000000

extern volatile bool FrameReady;
extern uint32_t* Frame1,Frame2;

#endif
