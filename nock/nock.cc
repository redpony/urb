#include <iostream>

struct N;
bool operator==(const N& a, const N& b);

// represents a "noun" [s f]
struct N {
  N() { a.cell.s = a.cell.f = 0; }
  explicit N(unsigned s) { a.cell.s = a.cell.f = 0; a.atom = s; }
  template<typename T, typename U> N(const T& s, const U& f) {
    a.cell.s = new N(s);
    a.cell.f = new N(f);
  }
  template<typename T, typename U, typename V> N(const T& x, const U& y, const V& z) {
    a.cell.s = new N(x);
    a.cell.f = new N(y,z);
  }
  N(const N& o) { copy(o, this); }
  ~N() { del(); }

  inline bool is_atom() const { return !a.cell.f; }
  inline bool is_cell() const { return a.cell.f; }

  const N& operator=(const N& src) {
    if (this == &src) return *this;
    del();
    copy(src, this);
    return *this;
  }

  // lines 4-5
  N ques() const { return N(is_atom() ? 1 : 0); }

  // lines 6-7
  N inc() const { if (is_atom()) { return N(a.atom + 1); } else return inc(); }

  // lines 8-10
  N eq() const {
    if (is_atom()) return eq(); // line 10
    return N((*a.cell.s == *a.cell.f) ? 0 : 1);
  }

  N slot() const {
    if (is_atom()) return slot(); // line 17
    if (a.cell.s->is_atom()) {
      const unsigned s = a.cell.s->a.atom;
      if (!s) { return slot(); }
      if (s == 1) return *a.cell.f;
      if (s == 2) return *a.cell.f->a.cell.s;
      if (s == 3) return *a.cell.f->a.cell.f;
      // TODO don't do this recursively
      if (s % 2 == 0) return N(N(2), N(N(s / 2), *a.cell.f).slot()).slot(); //15
      if (s % 2 == 1) return N(N(3), N(N(s / 2), *a.cell.f).slot()).slot(); //16
    }
    return slot();
  }

  N nock() const {
    if (is_atom()) return nock();
    const N& subj = *a.cell.s;
    if (a.cell.f->is_cell()) {
      if (a.cell.f->a.cell.s->is_cell()) {
        const N& b = *a.cell.f->a.cell.s->a.cell.s;
        const N& c = *a.cell.f->a.cell.s->a.cell.f;
        const N& d = *a.cell.f->a.cell.f;
        return N(N(subj, N(b, c)).nock(), N(subj, d).nock());
      }
      const unsigned v = a.cell.f->a.cell.s->a.atom;
      const N& b = *a.cell.f->a.cell.f;
      if (v == 0) {
        return N(b, subj).slot();
      } else if (v == 1) {
        return b;
      } else if (v == 2) {
        const N& b1 = *a.cell.f->a.cell.f->a.cell.s;
        const N& c1 = *a.cell.f->a.cell.f->a.cell.f;
        return N(N(subj, b1).nock(), N(subj, c1).nock()).nock();
      } else if (v == 3) {
        return N(subj, b).nock().ques();
      } else if (v == 4) {
        return N(subj, b).nock().inc();
      } else if (v == 5) {
        return N(subj, b).nock().eq();
      } else if (v == 6) {
        const N& cond = *a.cell.f->a.cell.f->a.cell.s;
        const N& then = *a.cell.f->a.cell.f->a.cell.f->a.cell.s;
        const N& otherwise = *a.cell.f->a.cell.f->a.cell.f->a.cell.f;
        const N test = N(subj, cond).nock();
        if (test.a.atom == 0) return N(subj, then).nock(); else return N(subj, otherwise).nock();
        //return N(subj,N(2,N(N(0,1),N(2,N(N(1,then,otherwise),N(N(1,0),N(2,N(N(1,2,3),N(N(1,0),N(4,4,cond)))))))))).nock();
      } else if (v == 7) {
        const N& f = *a.cell.f->a.cell.f->a.cell.s;
        const N& g = *a.cell.f->a.cell.f->a.cell.f;
        return N(N(subj, f).nock(), g).nock();
        //return N(subj,N(2,N(f,N(1,g)))).nock();
      } else if (v == 8) {
        const N& f = *a.cell.f->a.cell.f->a.cell.s;
        const N& g = *a.cell.f->a.cell.f->a.cell.f;
        return N(N(N(subj, f).nock(), subj), g).nock();
        //return N(subj,N(7,N(N(N(7,N(N(0,1),f)),N(0,1)),g))).nock();
      } else if (v == 9) {
        const N& b1 = *a.cell.f->a.cell.f->a.cell.s;
        const N& c1 = *a.cell.f->a.cell.f->a.cell.f;
        return N(subj,N(7,N(c1,N(2,N(N(0,1),N(0,b1)))))).nock();
      } else if (v == 10) {
        if (is_atom() && a.cell.f->is_atom() && a.cell.f->a.cell.f->is_atom()) {
          const N& c1 = *a.cell.f->a.cell.f->a.cell.s->a.cell.f;
          const N& d1 = *a.cell.f->a.cell.f->a.cell.f;
          return N(subj,N(8,N(c1,N(7,N(0,3,d1))))).nock();
        }
        const N& c1 = *a.cell.f->a.cell.f->a.cell.f;
        return N(subj, c1).nock();
      }
    }
    return nock();
  }
 private:
  void del() const { if (is_cell()) { delete a.cell.s; delete a.cell.f; } }
  static void copy(const N& src, N* trg) {
    if (src.is_cell()) {
      trg->a.cell.s = new N(*src.a.cell.s);
      trg->a.cell.f = new N(*src.a.cell.f);
    } else {
      trg->a.cell.f = 0;
      trg->a.atom = src.a.atom;
    }
  }
 public:
  union { unsigned atom; struct { N *s,*f; } cell; } a;
};

bool operator==(const N& a, const N& b) {
  if (a.is_atom() != b.is_atom()) return false;
  if (a.is_atom()) return (a.a.atom == b.a.atom);
  return (*a.a.cell.s == *b.a.cell.s) && (*a.a.cell.f == *b.a.cell.f);
}

void print_noun(std::ostream& os, const N& n) {
  if (n.is_atom()) { os << n.a.atom; return; }
  if (n.a.cell.s->is_cell()) os << '[';
  print_noun(os, *n.a.cell.s);
  if (n.a.cell.s->is_cell()) os << ']';
  os << ' ';
  print_noun(os, *n.a.cell.f);
}

std::ostream& operator<<(std::ostream& os, const N& n) {
  if (n.is_cell()) { os << '['; }
  print_noun(os, n);
  if (n.is_cell()) { os << ']'; }
  return os;
}

// generate some code
template <typename T> N constant(const T& c) { return N(1, c); }
N make_if(const N& cond, const N& then, const N& otherwise) { return N(6, cond, N(then, otherwise)); }
template <typename T> N equals(const T& c) { return N(5, c); }

const N YES = constant(0);
const N NO = constant(1);

int main() {
  const N inc(4, N(0, 1));
  const N inc2(7, inc, inc);
  const N dec(8,
      N(constant(0),
      N(8,
        N(N(1,make_if(equals(N(N(0,7),N(4,0,6))),
                      N(0,6),
                      N(9,2,N(N(0,2),N(N(4,0,6),N(0,7)))
                     )))),
        N(9,2,N(0,1))
       )
      ));
  std::cerr << dec << std::endl;
  const N if_yes = make_if(YES, inc, dec);
  const N if_no = make_if(NO, inc, dec);
  std::cerr << N(42, inc).nock() << std::endl;
  std::cerr << N(42, inc2).nock() << std::endl;
  std::cerr << N(42, if_yes).nock() << std::endl;
  std::cerr << N(42, if_no).nock() << std::endl;
  std::cerr << N(42, dec).nock() << std::endl;
  return 0;
}
