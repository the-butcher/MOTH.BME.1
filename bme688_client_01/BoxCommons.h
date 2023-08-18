#ifndef BoxCommons_h
#define BoxCommons_h

typedef enum {
  INIT_MODUS_COLLECT,
  INIT_MODUS_MEASURE
} init_modus_t;

typedef enum {
  INIT_STATE_PENDING,
  INIT_STATE_SUCCESS,
  INIT_STATE_FAILURE
} init_state_t;

#endif