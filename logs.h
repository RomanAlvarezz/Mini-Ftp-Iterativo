#pragma once

void log_init(void);
void log_close(void);

void log_info(const char *fmt, ...);
void log_error(const char *fmt, ...);