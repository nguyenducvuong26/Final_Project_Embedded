#define RCC_APB2ENR			(*((volatile unsigned long *) 0x40021018))

#define GPIO_A 				(*((volatile unsigned long *) 0x40010800))
#define GPIO_B 				(*((volatile unsigned long *) 0x40010C00))
#define GPIO_C 				(*((volatile unsigned long *) 0x40011000))

// Danh sach port
#define PA 1
#define PB 2
#define PC 3

// Danh sach dir

#define IN 		0	// dau vao
#define OUT_5 	1   // dau ra 5MHz
#define OUT_10 	2	// dau ra 10MHz
#define OUT_50 	3	// dau ra 50MHz

// Danh sach luea chon dir cho input
#define I_AN 	0 // analog
#define I_F 	1 // floating
#define I_PP 	2 // pull up/down

// Danh sach lua chon dir cho output
#define O_GP_PP 0  // General purpose output push-pull
#define O_GP_OD 1  // General purpose output Open-drain
#define O_AF_PP 2  // Alternate function output Push-pull
#define O_AF_OD 3  // Alternate function output Open-drain


void Init_GPIO(unsigned short port, unsigned short pin, unsigned short dir, unsigned short otp );
int R_GP(unsigned short port, unsigned short pin);
void W_GP(unsigned short port, unsigned short pin, unsigned short state);
void toggle_GP(unsigned short port, unsigned short pin);
