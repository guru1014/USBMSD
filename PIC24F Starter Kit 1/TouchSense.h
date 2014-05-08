
#define ID_TOUCH_PAD                100
#define ID_TOUCH_BUTTON_01          101
#define ID_TOUCH_BUTTON_02          102
#define ID_TOUCH_BUTTON_03          103
#define ID_TOUCH_BUTTON_04          104
#define ID_TOUCH_BUTTON_05          105


int  AdjustCTMUTrimBits( int direction );
void AdjustCurrentSource( void );
void CTMUInit( void );
void ReadCTMU( void );
void TouchSenseButtonsMsg( GOL_MSG* msg );




