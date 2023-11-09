/*****************************************************************************/
/*                                                                           */
/*            NCLab (Networked Control Lab) CORE version 1.0                 */
/*                                                                           */
/*****************************************************************************/
/*****************************************************************************/
/*                                                                           */
/*  File Name         : __socket.h                                           */
/*                                                                           */
/*  Description       : This file contains all the necessary constants ,     */
/*                      type definitions according to CAST specifications,	 */
/*                      common system headers.                               */
/*                                                                           */
/*                                                                           */
/*  Issues / Problems : None                                                 */
/*                                                                           */
/*  Revision History  :                                                      */
/*                                                                           */
/*        DD MM YYYY   Author(s)        Changes (Describe the changes made)  */
/*        18 03 2021   Shin Seunghyeok  Draft                                */
/*                                                                           */
/*****************************************************************************/
#ifndef ____SOCKET_H_F2DBDC40_6196_4E67_A689_D31A9310BEC0__
#define ____SOCKET_H_F2DBDC40_6196_4E67_A689_D31A9310BEC0__
#include <stdint.h>
#include <__common.h>
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//////                                                                   //////
//////                                                                   //////
//////                          User Variable                            //////
//////                                                                   //////
//////                                                                   //////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
/*****************************************************************************/
/*****************************************************************************/
/*********************************        ************************************/
/*********************************        ************************************/
/*********************************        ************************************/
/*****************************                ********************************/
/*******************************            **********************************/
/*********************************        ************************************/
/************************************   **************************************/
/************************************* ***************************************/
/*****************************************************************************/




///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//////                                                                   //////
//////                                                                   //////
//////                            Socket                                 //////
//////                                                                   //////
//////                                                                   //////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
/*****************************************************************************/
/*****************************************************************************/
/*********************************        ************************************/
/*********************************        ************************************/
/*********************************        ************************************/
/*****************************                ********************************/
/*******************************            **********************************/
/*********************************        ************************************/
/************************************   **************************************/
/************************************* ***************************************/
/*****************************************************************************/
#if __SOCKET__
#if __STATIC__

#if defined __cplusplus
extern "C"
#endif
int32_t __socket_read      (void* h, int32_t fd, int8_t* b, int32_t sz, void* extra, void* o);


#if defined __cplusplus
extern "C"
#endif
int32_t __socket_readfrom  (void* h, int32_t fd, int8_t* b, int32_t sz, void* extra, void* o);


#if defined __cplusplus
extern "C"
#endif
int32_t __socket_write     (void* h, int32_t fd, int8_t* b, int32_t sz, void* extra, void* o);


#if defined __cplusplus
extern "C"
#endif
int32_t __socket_writeto   (void* h, int32_t fd, int8_t* b, int32_t sz, void* extra, void* o);


#if defined __cplusplus
extern "C"
#endif
int32_t __socket_set_reader(void* h, uint8_t* mode, int32_t (*f[])(void*,int32_t,int8_t*,int32_t,void*,void*), void* o);

#if defined __cplusplus
extern "C"
#endif
int32_t __socket_close_fd(void* h, int32_t fd);

#if defined __cplusplus
extern "C"
#endif
int32_t __socket_delete_fd(void* h, int32_t fd);


#if defined __cplusplus
extern "C"
#endif
int32_t __socket_open      (void** h, int8_t* a, int8_t* b, int32_t sz, int32_t (*f[])(void*,int32_t,int8_t*,int32_t,void*,void*), void* o);


#if defined __cplusplus
extern "C"
#endif
int32_t __socket_close     (void** h);



#if defined __cplusplus
extern "C"
#endif
int32_t __socket_system_info(void* h);


#if defined __cplusplus
extern "C"
#endif
int32_t __socket_maxfd(void* h);


#if defined __cplusplus
extern "C"
#endif
int32_t __socket_print_fdset(void* h);


#if defined __cplusplus
extern "C"
#endif
void* __socket_fdset(void* h);




#endif
#endif


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//////                                                                   //////
//////                                                                   //////
//////                              HTTP                                 //////
//////                                                                   //////
//////                                                                   //////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
/*****************************************************************************/
/*****************************************************************************/
/*********************************        ************************************/
/*********************************        ************************************/
/*********************************        ************************************/
/*****************************                ********************************/
/*******************************            **********************************/
/*********************************        ************************************/
/************************************   **************************************/
/************************************* ***************************************/
/*****************************************************************************/
#if __IO__
#if __STATIC__
#if defined __cplusplus
extern "C"
#endif
int32_t __http_fd        (void* h, int32_t fd, int8_t* b, int32_t sz, void* extra, void* o);


#if defined __cplusplus
extern "C"
#endif
int32_t __http_fdset     (void* h, int32_t fd, int8_t* b, int32_t sz, void* extra, void* o);


#if defined __cplusplus
extern "C"
#endif
int32_t __http_read      (void* h, int32_t fd, int8_t* b, int32_t sz, void* extra, void* o);


#if defined __cplusplus
extern "C"
#endif
int32_t __http_write     (void* h, int32_t fd, int8_t* b, int32_t sz, void* extra, void* o);


#if defined __cplusplus
extern "C"
#endif
int32_t __http_set_reader(void* h, uint8_t* mode, int32_t (*f[])(void*,int32_t,int8_t*,int32_t,void*,void*), void* o);


#if defined __cplusplus
extern "C"
#endif
int32_t __http_open      (void** h, int8_t* a, int8_t* b, int32_t sz, int32_t (*f[])(void*,int32_t,int8_t*,int32_t,void*,void*), void* o);


#if defined __cplusplus
extern "C"
#endif
int32_t __http_close     (void** h);
#endif
#endif

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//////                                                                   //////
//////                                                                   //////
//////                              HTTPD                                //////
//////                                                                   //////
//////                                                                   //////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
/*****************************************************************************/
/*****************************************************************************/
/*********************************        ************************************/
/*********************************        ************************************/
/*********************************        ************************************/
/*****************************                ********************************/
/*******************************            **********************************/
/*********************************        ************************************/
/************************************   **************************************/
/************************************* ***************************************/
/*****************************************************************************/
#if __IO__
#if __STATIC__
#if defined __cplusplus
extern "C"
#endif
int32_t __httpd_fd        (void* h, int32_t fd, int8_t* b, int32_t sz, void* extra, void* o);


#if defined __cplusplus
extern "C"
#endif
int32_t __httpd_fdset     (void* h, int32_t fd, int8_t* b, int32_t sz, void* extra, void* o);


#if defined __cplusplus
extern "C"
#endif
int32_t __httpd_read      (void* h, int32_t fd, int8_t* b, int32_t sz, void* extra, void* o);


#if defined __cplusplus
extern "C"
#endif
int32_t __httpd_write     (void* h, int32_t fd, int8_t* b, int32_t sz, void* extra, void* o);


#if defined __cplusplus
extern "C"
#endif
int32_t __httpd_set_reader(void* h, uint8_t* mode, int32_t (*f[])(void*,int32_t,int8_t*,int32_t,void*,void*), void* o);


#if defined __cplusplus
extern "C"
#endif
int32_t __httpd_open      (void** h, int8_t* a, int8_t* b, int32_t sz, int32_t (*f[])(void*,int32_t,int8_t*,int32_t,void*,void*), void* o);


#if defined __cplusplus
extern "C"
#endif
int32_t __httpd_close     (void** h);


#if defined __cplusplus
extern "C"
#endif
int32_t __httpd_close_fd  (void* h, int32_t fd);
#endif
#endif

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//////                                                                   //////
//////                                                                   //////
//////                              HTTPS                                //////
//////                                                                   //////
//////                                                                   //////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
/*****************************************************************************/
/*****************************************************************************/
/*********************************        ************************************/
/*********************************        ************************************/
/*********************************        ************************************/
/*****************************                ********************************/
/*******************************            **********************************/
/*********************************        ************************************/
/************************************   **************************************/
/************************************* ***************************************/
/*****************************************************************************/
#if __IO__
#if __SSL_TLS__
#if __STATIC__
#if defined __cplusplus
extern "C"
#endif
int32_t __httpsd_fd        (void* h, int32_t fd, int8_t* b, int32_t sz, void* extra, void* o);


#if defined __cplusplus
extern "C"
#endif
int32_t __httpsd_fdset     (void* h, int32_t fd, int8_t* b, int32_t sz, void* extra, void* o);


#if defined __cplusplus
extern "C"
#endif
int32_t __httpsd_read      (void* h, int32_t fd, int8_t* b, int32_t sz, void* extra, void* o);


#if defined __cplusplus
extern "C"
#endif
int32_t __httpsd_write     (void* h, int32_t fd, int8_t* b, int32_t sz, void* extra, void* o);


#if defined __cplusplus
extern "C"
#endif
int32_t __httpsd_set_reader(void* h, uint8_t* mode, int32_t (*f[])(void*,int32_t,int8_t*,int32_t,void*,void*), void* o);


#if defined __cplusplus
extern "C"
#endif
int32_t __httpsd_open      (void** h, int8_t* a, int8_t* b, int32_t sz, int32_t (*f[])(void*,int32_t,int8_t*,int32_t,void*,void*), void* o);


#if defined __cplusplus
extern "C"
#endif
int32_t __httpsd_close     (void** h);
#endif
#endif
#endif
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//////                                                                   //////
//////                                                                   //////
//////                           WebSocket                               //////
//////                                                                   //////
//////                                                                   //////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
/*****************************************************************************/
/*****************************************************************************/
/*********************************        ************************************/
/*********************************        ************************************/
/*********************************        ************************************/
/*****************************                ********************************/
/*******************************            **********************************/
/*********************************        ************************************/
/************************************   **************************************/
/************************************* ***************************************/
/*****************************************************************************/
#if __IO__
#if __STATIC__
#if defined __cplusplus
extern "C"
#endif
int32_t __ws_fd        (void* h, int32_t fd, int8_t* b, int32_t sz, void* extra, void* o);


#if defined __cplusplus
extern "C"
#endif
int32_t __ws_fdset     (void* h, int32_t fd, int8_t* b, int32_t sz, void* extra, void* o);


#if defined __cplusplus
extern "C"
#endif
int32_t __ws_read      (void* h, int32_t fd, int8_t* b, int32_t sz, void* extra, void* o);


#if defined __cplusplus
extern "C"
#endif
int32_t __ws_write     (void* h, int32_t fd, int8_t* b, int32_t sz, void* extra, void* o);


#if defined __cplusplus
extern "C"
#endif
int32_t __ws_set_reader(void* h, uint8_t* mode, int32_t (*f[])(void*,int32_t,int8_t*,int32_t,void*,void*), void* o);


#if defined __cplusplus
extern "C"
#endif
int32_t __ws_open      (void** h, int8_t* a, int8_t* b, int32_t sz, int32_t (*f[])(void*,int32_t,int8_t*,int32_t,void*,void*), void* o);


#if defined __cplusplus
extern "C"
#endif
int32_t __ws_close     (void** h);
#endif
#endif

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//////                                                                   //////
//////                                                                   //////
//////                             Serial                                //////
//////                                                                   //////
//////                                                                   //////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
/*****************************************************************************/
/*****************************************************************************/
/*********************************        ************************************/
/*********************************        ************************************/
/*********************************        ************************************/
/*****************************                ********************************/
/*******************************            **********************************/
/*********************************        ************************************/
/************************************   **************************************/
/************************************* ***************************************/
/*****************************************************************************/
#if __IO__
#if __STATIC__
#if defined __cplusplus
extern "C"
#endif
int32_t __serial_fd        (void* h, int32_t fd, int8_t* b, int32_t sz, void* extra, void* o);


#if defined __cplusplus
extern "C"
#endif
int32_t __serial_read      (void* h, int32_t fd, int8_t* b, int32_t sz, void* extra, void* o);


#if defined __cplusplus
extern "C"
#endif
int32_t __serial_write     (void* h, int32_t fd, int8_t* b, int32_t sz, void* extra, void* o);


#if defined __cplusplus
extern "C"
#endif
int32_t __serial_set_reader(void* h, uint8_t* mode, int32_t (*f[])(void*,int32_t,int8_t*,int32_t,void*,void*), void* o);


#if defined __cplusplus
extern "C"
#endif
int32_t __serial_open      (void** h, int8_t* a, int8_t* b, int32_t sz, int32_t (*f[])(void*,int32_t,int8_t*,int32_t,void*,void*), void* o);


#if defined __cplusplus
extern "C"
#endif
int32_t __serial_close     (void** h);
#endif
#endif


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//////                                                                   //////
//////                                                                   //////
//////                              SHM                                  //////
//////                                                                   //////
//////                                                                   //////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
/*****************************************************************************/
/*****************************************************************************/
/*********************************        ************************************/
/*********************************        ************************************/
/*********************************        ************************************/
/*****************************                ********************************/
/*******************************            **********************************/
/*********************************        ************************************/
/************************************   **************************************/
/************************************* ***************************************/
/*****************************************************************************/
#if __IO__
#if __STATIC__
#if defined __cplusplus
extern "C"
#endif
int32_t __shm_get(void* h, void** p);


#if defined __cplusplus
extern "C"
#endif
int32_t __shm_read(void* h, int8_t* b, int32_t sz);


#if defined __cplusplus
extern "C"
#endif
int32_t __shm_write(void* h, int8_t* b, int32_t sz);


#if defined __cplusplus
extern "C"
#endif
int32_t __shm_open(void** h, int8_t* argv);


#if defined __cplusplus
extern "C"
#endif
int32_t __shm_close(void** h);
#endif
#endif




#endif