#ifndef UNIT_H_
#define UNIT_H_

class Unit {
public:
  Unit();
  virtual ~Unit();

private:
  int unit_id_;
  int max_hp_;
  int attactk_;
  int defense_;
  int move_left_;
  int max_movement_;
};



#endif
