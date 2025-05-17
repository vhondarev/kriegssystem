#ifndef KS_MESSAGES_H
#define KS_MESSAGES_H

#ifdef DEBUG
#define PRINT(...) printf(__VA_ARGS__)
#else
#define PRINT(...) ((void)0)
#endif

#define MSG_ERR_MEMORY_FAIL "Memory allocation failed"

#define MSG_ERR_PARSE_LIMIT "Too many teams"
#define MSG_ERR_PARSE_SETTLER "Team settler argument absent"
#define MSG_ERR_PARSE_TYPE_OR_QUANTITY "Invalid vessel type or vessels number"

#define MSG_ERR_VESSEL_TYPE "Invalid vessel type"
#define MSG_ERR_VESSEL_NULL_INIT "Initialize vessel state on NULL"
#define MSG_ERR_VESSEL_LIMIT "Eached maximum vessel count"

#endif
