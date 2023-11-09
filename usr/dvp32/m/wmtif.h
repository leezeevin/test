#ifndef __WMTIF_H_F2DBDC40_6196_4E67_A689_D31A9310BEC0__
#define __WMTIF_H_F2DBDC40_6196_4E67_A689_D31A9310BEC0__

#include "wcommon.h"
#include "mcommon.h"
#include <zmath.h>


enum
{
  EDIT_NET_IP = 0,
  EDIT_NET_PORT,
  COMBO_NET_PROTOCOL,
  COMBO_NET_CS,
  COMBO_NET_CASTTYPE,
  BUTTON_NET_OPEN_CLOSE,
  EDIT_NET_WRITE,
  BUTTON_NET_WRITE,
  CHECK_NET_CR,
  CHECK_NET_LF,
  COMBO_NET_DISPLAY_TYPE_W,
  COMBO_NET_DISPLAY_TYPE_R,
  EDIT_NET_MSEC,
  CHECK_NET_SIMUL,
  CHECK_NET_LOG,
  CHECK_NET_ECHO,
  CHECK_NET_STX,
  CHECK_NET_ETX,

  EDIT_WS_MSEC,
	EDIT_WS_URL,
  COMBO_WS_CS,
	COMBO_WS_DISPLAY_TYPE_W,
	COMBO_WS_DISPLAY_TYPE_R,
	BUTTON_WS_OPEN_CLOSE,
	EDIT_WS_WRITE,
	CHECK_WS_CR,
	CHECK_WS_LF,
	BUTTON_WS_WRITE,
  CHECK_WS_LOG,
  CHECK_WS_ECHO,
  CHECK_WS_SIMUL,
  CHECK_WS_STX,
  CHECK_WS_ETX,

  EDIT_SERIAL_PORT,
  EDIT_SERIAL_BAUDRATE,
  EDIT_SERIAL_DBIT,
  EDIT_SERIAL_SBIT,
  EDIT_SERIAL_PBIT,
  BUTTON_SERIAL_OPEN_CLOSE,
  COMBO_SERIAL_DISPLAY_TYPE_W,
  COMBO_SERIAL_DISPLAY_TYPE_R,
  EDIT_SERIAL_WRITE,
  BUTTON_SERIAL_WRITE,
  CHECK_SERIAL_CR,
  CHECK_SERIAL_LF,
  EDIT_SERIAL_MSEC,
  CHECK_SERIAL_LOG,
  CHECK_SERIAL_ECHO,
  CHECK_SERIAL_STX,
  CHECK_SERIAL_ETX,
  CHECK_SERIAL_SIMUL,

  EDIT_SERIAL_PORT2,
  EDIT_SERIAL_BAUDRATE2,
  EDIT_SERIAL_DBIT2,
  EDIT_SERIAL_SBIT2,
  EDIT_SERIAL_PBIT2,
  BUTTON_SERIAL_OPEN_CLOSE2,
  COMBO_SERIAL_DISPLAY_TYPE_W2,
  COMBO_SERIAL_DISPLAY_TYPE_R2,
  EDIT_SERIAL_WRITE2,
  BUTTON_SERIAL_WRITE2,
  CHECK_SERIAL_CR2,
  CHECK_SERIAL_LF2,
  EDIT_SERIAL_MSEC2,
  CHECK_SERIAL_LOG2,
  CHECK_SERIAL_ECHO2,
  CHECK_SERIAL_STX2,
  CHECK_SERIAL_ETX2,
  CHECK_SERIAL_ROUTER,

  BUTTON_CLEAR,
  LIST_READ,
  CHECK_READ_YN,
  EDIT_LIST_COUNT,

  MAX_ITEM
};


#if (defined XWIN32 || defined WINCE)
#pragma pack(1)
#endif
typedef struct wMTIF
{
  wLog               log;
  //tagMTIFSerial      srl[2];
  //tagMTIFSocket      sck;
  //tagMTIFWebSocket   ws;
  unsigned char      wshdr[1024];
  HWND               hitem[MAX_ITEM];
  unsigned char      read_lock;
  mtifInfo           info;
  MTIF*              pm;
  unsigned int       max_line;
}wMTIF;
#if (defined XWIN32 || defined WINCE)
#pragma pack()
#endif

void* wmtif_dlgproc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
void* wmtif_init(HWND hwnd, wMTIF* p);


#endif