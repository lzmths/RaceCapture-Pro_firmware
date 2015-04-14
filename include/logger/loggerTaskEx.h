/*
 * loggerTaskEx.h
 *
 *  Created on: Mar 3, 2012
 *      Author: brent
 */

#ifndef LOGGERTASKEX_H_
#define LOGGERTASKEX_H_

#include "loggerNotifications.h"
#include <stdbool.h>
#include <stdint.h>

typedef enum {
	LOGGING_STATUS_OK = 0,
	LOGGING_STATUS_ERROR_WRITING
} logging_status_t;

bool isLogging();
logging_status_t get_logging_status();
int32_t logging_since();
void startLogging();
void stopLogging();

void startLoggerTaskEx( int priority);
void loggerTaskEx(void *params);

#endif /* LOGGERTASKEX_H_ */
