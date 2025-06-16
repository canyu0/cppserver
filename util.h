#pragma once
extern int err1;
void errif(bool condition, const char *errmsg);
void errif(int condition, const char *errmsg);