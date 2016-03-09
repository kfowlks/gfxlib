//
// Monitoring Functions
//
#if !defined (__KEYBOARD_H)
#define __KEYBOARD_H
class Keyboard
{ public:

  Keyboard();
  ~Keyboard();
  void ERROR(int);
  
  unsigned char *key_table;
};
#endif