/*
 * Race Capture printk
 */

#ifndef __PRINTK_H__
#define __PRINTK_H__

#include "serial.h"
#include <stddef.h>

enum log_level {
    EMERG = 0,
    ALERT = 1,
    CRIT = 2,
    ERR = 3,
    WARNING = 4,
    NOTICE = 5,
    INFO = 6,
    DEBUG = 7,
    TRACE = 8
};

#define pr_trace(arg1) printk(TRACE, arg1);
#define pr_trace_int(arg1) printk_int(TRACE, arg1);
#define pr_trace_str_msg(arg1, arg2) printk_str_msg(TRACE, arg1, arg2);
#define pr_trace_int_msg(arg1, arg2) printk_int_msg(TRACE, arg1, arg2);
#define pr_trace_float_msg(arg1, arg2) printk_float_msg(TRACE, arg1, arg2);

#define pr_debug(arg1) printk(DEBUG, arg1);
#define pr_debug_int(arg1) printk_int(DEBUG, arg1);
#define pr_debug_float(arg1) printk_float(DEBUG, arg1);
#define pr_debug_str_msg(arg1, arg2) printk_str_msg(DEBUG, arg1, arg2);
#define pr_debug_int_msg(arg1, arg2) printk_int_msg(DEBUG, arg1, arg2);
#define pr_debug_float_msg(arg1, arg2) printk_float_msg(DEBUG, arg1, arg2);

#define pr_info(arg1) printk(INFO, arg1)
#define pr_info_int(arg1) printk_int(INFO, arg1);
#define pr_info_float(arg1) printk_float(INFO, arg1);
#define pr_info_str_msg(arg1, arg2) printk_str_msg(INFO, arg1, arg2);
#define pr_info_int_msg(arg1, arg2) printk_int_msg(INFO, arg1, arg2);
#define pr_info_float_msg(arg1, arg2) printk_float_msg(INFO, arg1, arg2);

#define pr_warning(arg1) printk(WARNING, arg1);
#define pr_warning_int(arg1) printk_int(WARNING, arg1);
#define pr_warning_float(arg1) printk_float(WARNING, arg1);
#define pr_warning_str_msg(arg1, arg2) printk_str_msg(WARNING, arg1, arg2);
#define pr_warning_int_msg(arg1, arg2) printk_int_msg(WARNING, arg1, arg2);
#define pr_warning_float_msg(arg1, arg2) printk_float_msg(WARNING, arg1, arg2);

#define pr_error(arg1) printk(ERR, arg1);
#define pr_error_int(arg1) printk_int(ERR, arg1);
#define pr_error_float(arg1) printk_float(ERR, arg1);
#define pr_error_str_msg(arg1, arg2) printk_str_msg(ERR, arg1, arg2);
#define pr_error_int_msg(arg1, arg2) printk_int_msg(ERR, arg1, arg2);
#define pr_error_float_msg(arg1, arg2) printk_float_msg(ERR, arg1, arg2);

#define DEBUG_LEVEL get_log_level() >= DEBUG
#define INFO_LEVEL get_log_level() >= INFO
#define NOTICE_LEVEL get_log_level() >= NOTICE
#define WARNING_LEVEL get_log_level() >= WARNING
#define TRACE_LEVEL get_log_level() >= TRACE

size_t read_log_to_serial(Serial *s, int escape);
int writek(const char *msg);
int writek_int(int value);
int printk(enum log_level level, const char *msg);
int printk_int(enum log_level level, int value);
int printk_int_msg(enum log_level level, const char *msg, int value);
int printk_float(enum log_level level, float value);
int printk_float_msg(enum log_level level, const char *msg, float value);
int printk_str_msg(enum log_level level, const char *msg, const char *value);
enum log_level set_log_level(enum log_level level);
enum log_level get_log_level();

#endif /* __PRINTK_H__ */
