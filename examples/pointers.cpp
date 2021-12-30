#include <string>
#include <optional>
#include <cassert>
#include <algorithm>
#include <maybe_ext/maybe_ext.hpp>

using std::string, std::optional, std::nullopt;
using namespace maybe_ext;

struct Node {
    Node *lch = nullptr;
    Node *rch = nullptr;
    int height() const {
        int lh = lch | map_or(0, &Node::height);
        int rh = rch | map_or(0, &Node::height);
        return 1 + std::max(lh, rh);
    }
};

int main() {
    //     a
    //    / \
    //   b   c
    //  /
    // d
    Node *a = new Node(), *b = new Node(), *c = new Node(), *d = new Node();
    a->lch = b;
    a->rch = c;
    b->lch = d;
    assert(a->height() == 3);
    assert(b->height() == 2);
    assert(c->height() == 1);
    assert(d->height() == 1);
    delete a;
    delete b;
    delete c;
    delete d;
    return 0;
}