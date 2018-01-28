
typedef enum enemy_status {ENEMY_LOSS, NOT_DEAD_YET} enemy_status_t;


extern void enemy_init(void);
extern void enemy_update(void);
extern enemy_status_t enemy_event(u8 ship_x, u8 ship_y);

