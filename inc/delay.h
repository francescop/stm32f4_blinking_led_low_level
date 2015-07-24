
// Private variables
volatile uint32_t time_var1, time_var2;

// Private function prototypes
void delay(volatile uint32_t nCount);

/* Called from systick handler
*/
void timing_handler() {
	if (time_var1) {
		time_var1--;
	}

	time_var2++;
}

// Delay a number of systick cycles (1ms)
void delay(volatile uint32_t nCount) {
	// ---------- SysTick timer -------- //
	if (SysTick_Config(SystemCoreClock / 1000)) {
		// Capture error
		while (1){};
	}
	time_var1 = nCount;
	while(time_var1){};
}
