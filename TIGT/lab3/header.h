#define STATE_INIT 0
#define STATE_RUNNING 1
#define STATE_HOLD 3

#define LED_D19 (1<<6)
#define LED_D20 (1<<7)

#define KEY_S5 (1<<5)
#define KEY_S6 (1<<6)
#define KEY_S7 (1<<7)

#define FALSE 0
#define TRUE (!FALSE)

#define ON TRUE
#define OFF FALSE

typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

typedef int state_t;
typedef int key_t;
typedef int led_t;
typedef int bool_t;
typedef uint32_t time_t;

typedef state_t (*state_fn)(void);

