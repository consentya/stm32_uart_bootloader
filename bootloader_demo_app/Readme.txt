To build such application, some special configuration has to be performed:
1. Set the program load address at 0x08004000, using your toolchain linker file
2. Relocate the vector table at address 0x08004000, using the "NVIC_SetVectorTable"
   function from the misc.h/.c driver or by modifying the value of the constant
   "VECT_TAB_OFFSET" defined in system_stm32f4xx.c file. 
   
   
/*!< Uncomment the following line if you need to relocate your vector Table in
     Internal SRAM. */
/* #define VECT_TAB_SRAM */
#define VECT_TAB_OFFSET  0x8000 /*!< Vector Table base offset field.
                                   This value must be a multiple of 0x200. */
                                   
  /* Configure the Vector Table location add offset address ------------------*/
#ifdef VECT_TAB_SRAM
  SCB->VTOR = SRAM_BASE | VECT_TAB_OFFSET; /* Vector Table Relocation in Internal SRAM */
#else
  SCB->VTOR = FLASH_BASE | VECT_TAB_OFFSET; /* Vector Table Relocation in Internal FLASH */
#endif                                                             