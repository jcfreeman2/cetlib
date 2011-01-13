#include "cetlib/value_ptr.h"

#include <memory>
#include <cassert>

class simple
{
  int i;

public:
  static int n_alive;
  static int n_born;

  simple() : i(0) { ++n_alive; ++n_born; }
  explicit simple(int j) : i(j) { ++n_alive; ++n_born; }

  simple(simple const& s) : i(s.i) { ++n_alive; ++n_born; }

  ~simple() { --n_alive; }

  bool operator==(simple const& o) const { return i == o.i; }
  bool isSame(simple const& o) const {return &o == this; }

};  // simple

int simple::n_alive = 0;
int simple::n_born = 0;


int main()
{
  assert(simple::n_alive == 0);
  {
    cet::value_ptr<simple> a(new simple(10));
    assert(simple::n_alive == 1);
    cet::value_ptr<simple> b(a);
    assert(simple::n_alive == 2);

    assert(*a==*b);
    assert(a->isSame(*b) == false);
  } // a and b destroyed
  assert(simple::n_alive == 0);

  {
    std::auto_ptr<simple> c(new simple(11));
    std::auto_ptr<simple> d(new simple(11));
    assert(c.get() != 0);
    assert(d.get() != 0);
    simple* pc = c.get();
    simple* pd = d.get();

    cet::value_ptr<simple> e(c);
    assert(c.get() == 0);
    assert(*d == *e);
    assert(e.operator->() == pc);

    cet::value_ptr<simple> f;
    if (f) {
      assert(0);
    }
    else {
    }
    f = d;
    assert(d.get() == 0);
    assert(*e == *f);
    assert(f.operator->() == pd);
    if (f) {
    }
    else {
      assert(0);
    }

    assert(simple::n_alive == 2);
    assert(simple::n_born == 4);
  }
  assert(simple::n_alive == 0);
  assert(simple::n_born == 4);
}
